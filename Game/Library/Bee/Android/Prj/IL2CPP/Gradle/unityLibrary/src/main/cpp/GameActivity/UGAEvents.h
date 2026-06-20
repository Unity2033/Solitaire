#pragma once

#include <functional>
#include <unordered_map>
#include <vector>
#include <typeinfo>
#include <iostream>
#include "UGADebug.h"

struct android_input_buffer;

namespace Unity
{
    class UnityApplication;
    /// <summary>
    /// Allows to globally register to the events provided.
    /// This makes it possible to add extra functionality in libgame.so without modifying existing files.
    /// Example usage, shows how to register and react to input:
    ///
    /// Create file in unityLibrary\src\main\cpp\GameActivity\UserFolder\Test.cpp
    ///
    /// #include "UGAApplication.h"
    /// #include "game-activity/native_app_glue/android_native_app_glue.h"
    /// void UnityGameActivityPluginLoad(Unity::UnityApplication& application)
    /// {
    ///     application.GetEvents().Register<Unity::UnityEventProcessInput>([](const Unity::UnityEvent& e)
    ///      {
    ///          auto inputBuffer = static_cast<const Unity::UnityEventProcessInput&>(e).GetInputBuffer();
    ///          if (inputBuffer->keyEventsCount != 0) {
    ///              for (uint64_t i = 0; i < inputBuffer->keyEventsCount; ++i) {
    ///                  GameActivityKeyEvent *keyEvent = &inputBuffer->keyEvents[i];
    ///                  if (keyEvent->action == AKEY_EVENT_ACTION_DOWN && keyEvent->keyCode == AKEYCODE_A)
    ///                      UNITY_LOG_INFO("A key pressed");
    ///              }
    ///          }
    ///      });
    /// }
    /// </summary>

    class UnityEvent
    {
        UnityApplication& m_Application;
    public:
        UnityEvent(UnityApplication& application) : m_Application(application) {}
        virtual ~UnityEvent() {}

        UnityApplication& GetApplication() const { return m_Application; }
    };

    class UnityEvents
    {
    public:
        typedef uint64_t RegistrationToken;
    private:
        struct Callback
        {
            RegistrationToken registrationToken;
            std::function<void(const UnityEvent&)> function;
        };

        typedef std::vector<Callback> Callbacks;
        typedef std::unordered_map<const std::type_info*, Callbacks> EventMap;
        EventMap m_EventMap;
        int m_InvocationInProgress;

        // Callbacks which were registered/unregistered during callback invocation
        EventMap m_ToRegister;
        EventMap m_ToUnregister;
        RegistrationToken m_RegistrationTokenCounter;
    public:
        UnityEvents()
            : m_InvocationInProgress(0)
            // Treat registration token 0 as invalid
            , m_RegistrationTokenCounter(1)
            {}

        template<typename EventType>
        RegistrationToken Register(const std::function<void(const EventType&)>& fn)
        {
#if EXTERNAL_GAME_ACTIVITY_CODE
            UNITY_ASSERT_RUNNING_ON_MAIN_THREAD("UnityEvents::Register must be called from game main loop thread.");
            auto callback = CreateCallback<EventType>(fn);
            auto eventType = &typeid(EventType);
            if (m_InvocationInProgress > 0) {
                m_ToRegister[eventType].push_back(callback);
                return callback.registrationToken;
            }

            RegisterInternal(eventType, callback);
            return callback.registrationToken;
#endif
        }

        template<typename EventType>
        void Unregister(RegistrationToken registrationToken)
        {
#if EXTERNAL_GAME_ACTIVITY_CODE
            UNITY_ASSERT_RUNNING_ON_MAIN_THREAD("UnityEvents::Unregister must be called from game main loop thread.");

            Callback callback;
            callback.registrationToken = registrationToken;
            auto eventType = &typeid(EventType);

            if (m_InvocationInProgress > 0) {
                m_ToUnregister[eventType].push_back(callback);
                return;
            }
            UnregisterInternal(eventType, callback);
#endif
        }

    private:
        template<typename EventType>
        Callback CreateCallback(const std::function<void(const EventType&)>& fn)
        {
            Callback callback;
            callback.registrationToken = m_RegistrationTokenCounter++;
            UNITY_ASSERT(m_RegistrationTokenCounter < UINT64_MAX, "Too many events registered");
            callback.function = [fn](const UnityEvent& evt) { fn(static_cast<const EventType&>(evt)); };

            return callback;
        }

        void RegisterInternal(const std::type_info* eventType, const Callback& callback)
        {
            m_EventMap[eventType].push_back(callback);
        }

        void UnregisterInternal(const std::type_info* eventType, const Callback& callback)
        {
            EventMap::iterator item = m_EventMap.find(eventType);
            if (item == m_EventMap.end())
                return;
            item->second.erase(std::remove_if(item->second.begin(), item->second.end(), [&](Callback tempCallback)
            {
                return callback.registrationToken == tempCallback.registrationToken;
            }), item->second.end());
        }

        void Invoke(const UnityEvent& event)
        {
#if EXTERNAL_GAME_ACTIVITY_CODE
            UNITY_ASSERT_RUNNING_ON_MAIN_THREAD("UnityEvents::Invoke must be called from game main loop thread.");
            EventMap::iterator item = m_EventMap.find(&typeid(event));
            if (item == m_EventMap.end())
                return;
            m_InvocationInProgress++;
            for (Callback& callback : item->second)
                callback.function(event);
            m_InvocationInProgress--;

            if (m_InvocationInProgress == 0) {
                for (auto eventType: m_ToRegister) {
                    for (const auto& callback : eventType.second) {
                        RegisterInternal(eventType.first, callback);
                    }
                }
                m_ToRegister.clear();

                for (auto eventType: m_ToUnregister) {
                    for (const auto& callback: eventType.second) {
                        UnregisterInternal(eventType.first, callback);
                    }
                }
                m_ToUnregister.clear();
            }
#endif
        }

        friend class Unity::UnityApplication;
    };

    /// <summary>
    /// Event called when first scene is loaded and the game is visible.
    /// </summary>
    class UnityEventFirstSceneLoaded final : public UnityEvent
    {
    public:
        UnityEventFirstSceneLoaded(UnityApplication& application) : UnityEvent(application) {}
    };

    /// <summary>
    /// Event called during input processing.
    /// </summary>
    class UnityEventProcessInput final : public UnityEvent
    {
        android_input_buffer* m_InputBuffer;
    public:
        UnityEventProcessInput(UnityApplication& application, android_input_buffer* inputBuffer) : UnityEvent(application), m_InputBuffer(inputBuffer) {}
        android_input_buffer* GetInputBuffer() const { return m_InputBuffer; }
    };


    /// <summary>
    /// Event called before command is processed.
    /// You can find available commands in android_native_app_glue.h or
    /// https://developer.android.com/reference/games/game-activity/group/android-native-app-glue#enumerations_1
    /// </summary>
    class UnityEventProcessApplicationCommandBefore final : public UnityEvent
    {
        int32_t m_Command;
    public:
        UnityEventProcessApplicationCommandBefore(UnityApplication& application, int32_t command) : UnityEvent(application), m_Command(command) {}
        int32_t GetCommand() const { return m_Command; }
    };

    /// <summary>
    /// Event called after command is processed.
    /// You can find available commands in android_native_app_glue.h or
    /// https://developer.android.com/reference/games/game-activity/group/android-native-app-glue#enumerations_1
    /// </summary>
    class UnityEventProcessApplicationCommandAfter final : public UnityEvent
    {
        int32_t m_Command;
    public:
        UnityEventProcessApplicationCommandAfter(UnityApplication& application, int32_t command) : UnityEvent(application), m_Command(command) {}
        int32_t GetCommand() const { return m_Command; }
    };

    /// <summary>
    /// Event called after each game loop tick while the game is animating.
    /// </summary>
    class UnityEventLoopAfter final : public UnityEvent
    {
    public:
        UnityEventLoopAfter(UnityApplication& application) : UnityEvent(application) {}
    };
}
