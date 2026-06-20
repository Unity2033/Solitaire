#pragma once

#include <jni.h>
#include <dlfcn.h>
#include <android/native_window.h>
#include <android/configuration.h>
#include <android/looper.h>

#include "UGADebug.h"
#include "UGAConfiguration.h"
#include "UGAInput.h"
#include "UGASoftKeyboard.h"
#include "UGAEvents.h"

struct android_app;

namespace Unity
{
    typedef const char* (*UnityInitializePtr)(UnityApplication* application, int applicationSize);
    inline UnityInitializePtr GetUnityInitializeFunc(void* libraryHandle)
    {
        return (UnityInitializePtr)dlsym(libraryHandle, "UnityInitialize");
    }

    typedef const char* (*UnityShutdownPtr)();
    inline UnityShutdownPtr GetUnityShutdownFunc(void* libraryHandle)
    {
        return (UnityShutdownPtr)dlsym(libraryHandle, "UnityShutdown");
    }

    // Note: Don't change the layout of this class since it will become out of sync with Unity Runtime
    //       Changing the layout here, will require rebuilding the Unity runtime
    class UnityApplication
    {
    public:
        enum class MemoryUsage
        {
            Unknown = 0,
            Low = 1,
            Medium = 2,
            High = 3,
            Critical = 4
        };

#include "MacroHeaderBegin.h"
    public:
    #include "UnityToGACallbacks.h"
    private:
    #include "GAToUnityCallbacks.h"
#include "MacroEnd.h"

    public:
#if EXTERNAL_GAME_ACTIVITY_CODE
        static UnityApplication* CreateInstance(android_app* androidAppInstance);
        static void DestroyInstance();
        static void OnApplicationCommand(struct android_app *app, int32_t cmd);

        void Loop();
        void ProcessApplicationCommands() const;
        bool ProcessInputEvents();
        bool ProcessTextInputEvents();
        // Cleans up motion and key event pointers
        bool CleanInputEvents();
#else
        // Set function pointers which will be called by GameActivity code from libgame.so
        static void SetGameActivityToUnityCallbacks(UnityApplication* instance);
#endif
        static UnityApplication* Instance();
        static bool InstanceAvailable();

        bool IsAnimating() const;

        inline android_app* GetAndroidAppInstance() const { return m_AndroidAppInstance; }
        inline UnitySoftKeyboard& GetSoftKeyboard() { return *m_SoftKeyboard; }
        inline UnityConfiguration& GetConfiguration() { return *m_Configuration; }
        inline UnityEvents& GetEvents() { return m_Events; }
        inline bool IsInitialized() const {return m_Initialized;}
    private:
        UnityApplication(android_app* instance);
        ~UnityApplication();

        void ProcessFrame();

        struct SavedState
        {
            bool hasFocus;
        };

        JNIEnv* m_MainThreadJniEnv;
        android_app* m_AndroidAppInstance;
        android_input_buffer* m_AndroidInputBuffer;

        std::unique_ptr<UnitySoftKeyboard> m_SoftKeyboard;
        std::unique_ptr<UnityConfiguration> m_Configuration;
        UnityEvents m_Events;
        bool m_HasFocus;
        bool m_HasWindow;
        bool m_IsVisible;
        bool m_IsPaused;
        bool m_Initialized;
        bool m_QuitRequested;
        SavedState m_SavedState;

        static UnityInitializePtr UnityInitialize;
        static UnityShutdownPtr UnityShutdown;
    private:
        bool InitializeInputEvents();
        void HandleCmdFocusGained(ANativeWindow* window);
        void HandleCmdFocusLost(ANativeWindow* window);
        void HandleCmdPause(ANativeWindow* window);
        void HandleCmdResume(ANativeWindow* window);

        friend void Unity::LogOutput(LogType logType, const char* format, ...);
    private:
        void SetUnityToGameActivityCallbacks();

        bool IgnorePauseEvent() const;

        // Unity -> GameActivity callbacks
        void _GetSavedStateImpl(void** savedState, size_t* savedStateSize) const;
        ALooper* _GetLooperImpl() const;
        const ARect& _GetContentRectImpl() const;
        int _GetActivityStateImpl() const;
        JavaVM* _GetJavaVMImpl() const;
        JNIEnv* _GetUIThreadJNIEnvImpl() const;
        JNIEnv* _GetMainThreadJNIEnvImpl() const;
        jobject _GetGameActivityJavaInstanceImpl() const;
        const char* _GetInternalDataPathImpl() const;
        const char* _GetExternalDataPathImpl() const;
        const char* _GetObbPathImpl() const;
        int32_t _GetSdkVersionImpl () const;
        int32_t _GetMaxPointerCountImpl () const;
        AAssetManager* _GetAAssetManagerImpl() const;

        UnityMotionEventCallbacks* _GetMotionEventCallbacksImpl() const;
        size_t _GetMotionEventSizeImpl() const;
        size_t _GetMotionEventMaxAxisImpl() const;
        UnityKeyEventCallbacks* _GetKeyEventCallbacksImpl() const;
        size_t _GetKeyEventSizeImpl() const;

        bool _GetWindowInsetsImpl(Unity::InsetsType insetsType, ARect* outRect) const;
        void _ProcessApplicationCommandsForErrorDialogImpl();
    };
}
