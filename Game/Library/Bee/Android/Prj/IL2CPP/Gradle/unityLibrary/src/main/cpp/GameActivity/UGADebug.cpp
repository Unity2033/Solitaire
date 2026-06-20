#if EXTERNAL_GAME_ACTIVITY_CODE
#include "UGADebug.h"
#include "UGAApplication.h"
#include "game-activity/native_app_glue/android_native_app_glue.h"
#include <sstream>

const pthread_t kInvalidThread = 0;
static pthread_t s_MainThread = kInvalidThread;
static pthread_t s_UIThread = kInvalidThread;

void Unity::SetThisThreadAsUIThread()
{
    UNITY_ASSERT(pthread_equal(s_UIThread, kInvalidThread) != 0, "UI thread id was already set");
    if (pthread_equal(s_UIThread, kInvalidThread) != 0)
        s_UIThread = pthread_self();

    UNITY_ASSERT(pthread_equal(s_UIThread, s_MainThread) == 0, "UI and Main thread ids should be different");
}

void Unity::SetThisThreadAsMainThread()
{
    UNITY_ASSERT(pthread_equal(s_MainThread, kInvalidThread) != 0, "Main thread id was already set");
    if (pthread_equal(s_MainThread,kInvalidThread) != 0)
        s_MainThread = pthread_self();

    UNITY_ASSERT(pthread_equal(s_UIThread, s_MainThread) == 0, "UI and Main thread ids should be different");
}

pthread_t Unity::GetUIThreadId()
{
    return s_UIThread;
}

pthread_t Unity::GetMainThreadId()
{
    return s_MainThread;
}

bool Unity::RunningOnUIThread()
{
    UNITY_ASSERT(pthread_equal(s_UIThread, kInvalidThread) == 0, "UI thread id was not yet set");
    return pthread_equal(s_UIThread, kInvalidThread) == 0 && s_UIThread == pthread_self();
}

bool Unity::RunningOnMainThread()
{
    UNITY_ASSERT(pthread_equal(s_MainThread, kInvalidThread) == 0, "Main thread id was not yet set");
    return pthread_equal(s_MainThread, kInvalidThread) == 0 && s_MainThread == pthread_self();
}

void Unity::CheckAndClearException(JNIEnv *env, const char *methodName)
{
    if (env->ExceptionCheck()) {
        UNITY_LOG_ERROR("Exception while running %s", methodName);
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
}

void Unity::LogOutput(LogType logType, const char* format, ...)
{
    if (UnityApplication::InstanceAvailable() && UnityApplication::Instance()->Log != NULL)
    {
        va_list vl;
        va_start(vl, format);
        UnityApplication::Instance()->Log(logType, format, vl);
        va_end(vl);
    }
    else
    {
        int priority;
        switch (logType)
        {
            case kLogTypeInfo: priority = ANDROID_LOG_INFO; break;
            case kLogTypeWarning: priority = ANDROID_LOG_WARN; break;
            case kLogTypeAssert:
            default :
                priority = ANDROID_LOG_ERROR; break;
        }
        va_list vl;
        va_start(vl, format);
        __android_log_vprint(priority, "Unity", format, vl);
        va_end(vl);
    }
}

const std::string Unity::AppCmdName(int32_t cmd)
{
    const char* value;
#define CASE(EnumValue) case EnumValue: value = #EnumValue; break
    switch (cmd)
    {
        CASE(APP_CMD_INIT_WINDOW);
        CASE(APP_CMD_TERM_WINDOW);
        CASE(APP_CMD_WINDOW_RESIZED);
        CASE(APP_CMD_WINDOW_REDRAW_NEEDED);
        CASE(APP_CMD_CONTENT_RECT_CHANGED);
        CASE(APP_CMD_SOFTWARE_KB_VIS_CHANGED);
        CASE(APP_CMD_GAINED_FOCUS);
        CASE(APP_CMD_LOST_FOCUS);
        CASE(APP_CMD_CONFIG_CHANGED);
        CASE(APP_CMD_LOW_MEMORY);
        CASE(APP_CMD_START);
        CASE(APP_CMD_RESUME);
        CASE(APP_CMD_SAVE_STATE);
        CASE(APP_CMD_PAUSE);
        CASE(APP_CMD_STOP);
        CASE(APP_CMD_DESTROY);
        CASE(APP_CMD_WINDOW_INSETS_CHANGED);
        CASE(APP_CMD_EDITOR_ACTION);
        CASE(APP_CMD_KEY_EVENT);
        CASE(APP_CMD_TOUCH_EVENT);
    default:
        value = "UNKNOWN";
    }
#undef CASE

    std::stringstream ss;
    ss << value << "(" << cmd << ")";
    return ss.str();
}

const std::string Unity::EditorActionName(int32_t editorAction)
{
    const char* value;
#define CASE(EnumValue) case EnumValue: value = #EnumValue; break
    switch (editorAction)
    {
        CASE(IME_ACTION_UNSPECIFIED);
        CASE(IME_ACTION_NONE);
        CASE(IME_ACTION_GO);
        CASE(IME_ACTION_SEARCH);
        CASE(IME_ACTION_SEND);
        CASE(IME_ACTION_NEXT);
        CASE(IME_ACTION_DONE);
        CASE(IME_ACTION_PREVIOUS);
        default:
            value = "UNKNOWN";
    }
#undef CASE

    std::stringstream ss;
    ss << value << "(" << editorAction << ")";
    return ss.str();
}
#endif
