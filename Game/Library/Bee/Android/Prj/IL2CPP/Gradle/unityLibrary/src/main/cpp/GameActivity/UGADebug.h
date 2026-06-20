#pragma once

#include <string>
#include <android/log.h>
#include <jni.h>

namespace Unity
{
    enum LogType
    {
        kLogTypeError,
        kLogTypeAssert,
        kLogTypeWarning,
        kLogTypeInfo
    };

    const std::string AppCmdName(int32_t cmd);
    const std::string EditorActionName(int32_t editorAction);

    void LogOutput(LogType logType, const char* format, ...);
    void SetThisThreadAsUIThread();
    void SetThisThreadAsMainThread();
    pthread_t GetUIThreadId();
    pthread_t GetMainThreadId();
    bool RunningOnUIThread();
    bool RunningOnMainThread();
    void CheckAndClearException(JNIEnv *env, const char *methodName);
}

#if EXTERNAL_GAME_ACTIVITY_CODE

    #define UNITY_LOG_INFO(...) Unity::LogOutput(Unity::kLogTypeInfo, __VA_ARGS__)
    #define UNITY_LOG_WARNING(...) Unity::LogOutput(Unity::kLogTypeWarning, __VA_ARGS__)
    #define UNITY_LOG_ERROR(...) Unity::LogOutput(Unity::kLogTypeError, __VA_ARGS__)
    #define UNITY_ASSERT(Condition, ...) if (!(Condition)) {  Unity::LogOutput(Unity::kLogTypeAssert, __VA_ARGS__); raise(SIGTRAP); }
    #define UNITY_ASSERT_RUNNING_ON_UI_THREAD(...) if (!RunningOnUIThread()) { Unity::LogOutput(Unity::kLogTypeAssert, __VA_ARGS__); raise(SIGTRAP); }
    #define UNITY_ASSERT_RUNNING_ON_MAIN_THREAD(...) if (!RunningOnMainThread()) { Unity::LogOutput(Unity::kLogTypeAssert, __VA_ARGS__); raise(SIGTRAP); }

#else

    #define UNITY_LOG_INFO(...) printf_console(__VA_ARGS__)
    #define UNITY_LOG_WARNING(...) WarningStringMsg(__VA_ARGS__)
    #define UNITY_LOG_ERROR(...) ErrorStringMsg(__VA_ARGS__)
    #define UNITY_ASSERT(Condition, ...) AssertMsg(Condition, __VA_ARGS__)
    #define UNITY_ASSERT_RUNNING_ON_UI_THREAD(...) ASSERT_RUNNING_ON_UI_THREAD_MSG(__VA_ARGS__)
    #define UNITY_ASSERT_RUNNING_ON_MAIN_THREAD(...) ASSERT_RUNNING_ON_MAIN_THREAD_MSG(__VA_ARGS__)

#endif

#define UNITY_ABORT() abort()
#define UNITY_FATAL_ERROR(...) { Unity::LogOutput(Unity::kLogTypeError, __VA_ARGS__); UNITY_ABORT(); }
#define UNITY_CHECK_JNI(JniEnv) { CheckAndClearException(JniEnv, __FUNCTION__); }
#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif
