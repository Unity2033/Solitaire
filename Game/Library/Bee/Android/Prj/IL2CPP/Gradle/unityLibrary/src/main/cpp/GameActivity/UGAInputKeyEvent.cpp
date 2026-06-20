#if EXTERNAL_GAME_ACTIVITY_CODE
#include "UGAApplication.h"
#include "UGAVersion.h"
#include "game-activity/native_app_glue/android_native_app_glue.h"

static Unity::UnityKeyEventCallbacks s_KeyEventCallbacks;

Unity::UnityKeyEventCallbacks* Unity::UnityApplication::_GetKeyEventCallbacksImpl() const
{
    return &s_KeyEventCallbacks;
}

size_t Unity::UnityApplication::_GetKeyEventSizeImpl() const
{
    return sizeof(GameActivityKeyEvent);
}

Unity::UnityKeyEventCallbacks::UnityKeyEventCallbacks()
{
#include "MacroSourceBegin.h"
    #include "UnityToGAKeyEventCallbacks.h"
#include "MacroEnd.h"
}

int32_t Unity::UnityKeyEventCallbacks::_GetDeviceIdImpl(void* keyEvent)
{
    return reinterpret_cast<GameActivityKeyEvent*>(keyEvent)->deviceId;
}

int32_t Unity::UnityKeyEventCallbacks::_GetSourceImpl(void* keyEvent)
{
    return reinterpret_cast<GameActivityKeyEvent*>(keyEvent)->source;
}

int32_t Unity::UnityKeyEventCallbacks::_GetActionImpl(void* keyEvent)
{
    return reinterpret_cast<GameActivityKeyEvent*>(keyEvent)->action;
}

int64_t Unity::UnityKeyEventCallbacks::_GetEventTimeMillisImpl(void* keyEvent)
{
    return reinterpret_cast<GameActivityKeyEvent*>(keyEvent)->eventTime;
}

int64_t Unity::UnityKeyEventCallbacks::_GetDownTimeMillisImpl(void* keyEvent)
{
    return reinterpret_cast<GameActivityKeyEvent*>(keyEvent)->downTime;
}

int32_t Unity::UnityKeyEventCallbacks::_GetFlagsImpl(void* keyEvent)
{
    return reinterpret_cast<GameActivityKeyEvent*>(keyEvent)->flags;
}

int32_t Unity::UnityKeyEventCallbacks::_GetMetaStateImpl(void* keyEvent)
{
    return reinterpret_cast<GameActivityKeyEvent*>(keyEvent)->metaState;
}

int32_t Unity::UnityKeyEventCallbacks::_GetModifiersImpl(void* keyEvent)
{
    return reinterpret_cast<GameActivityKeyEvent*>(keyEvent)->modifiers;
}

int32_t Unity::UnityKeyEventCallbacks::_GetRepeatCountImpl(void* keyEvent)
{
    return reinterpret_cast<GameActivityKeyEvent*>(keyEvent)->repeatCount;
}

int32_t Unity::UnityKeyEventCallbacks::_GetKeyCodeImpl(void* keyEvent)
{
    return reinterpret_cast<GameActivityKeyEvent*>(keyEvent)->keyCode;
}

int32_t Unity::UnityKeyEventCallbacks::_GetUnicodeCharImpl(void* keyEvent)
{
#if GAMEACTIVITY_VERSION_CURRENT >= GAMEACTIVITY_VERSION_1_2_2
    return reinterpret_cast<GameActivityKeyEvent*>(keyEvent)->unicodeChar;
#else
    UNUSED(keyEvent);
    return 0;
#endif
}

const char* Unity::UnityKeyEventCallbacks::_GetCharactersImpl(void*)
{
    // No characters field in GameActivityKeyEvent
    return "";
}

#endif
