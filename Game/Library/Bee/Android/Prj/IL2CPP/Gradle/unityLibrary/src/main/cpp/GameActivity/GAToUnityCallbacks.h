// This source code is compiled into libgame.so, while Unity runtime lives in libunity.so
// The macros below declare function pointers which are set in libunity.so
// Allowing libgame.so to call into libunity.so
DECLARE_METHOD(void, Log, (LogType logType, const char* format, va_list alist))
DECLARE_METHOD(bool, InitializeRuntime, ())
DECLARE_METHOD(bool, LoopRuntime, ())
DECLARE_METHOD(bool, ProcessKeyEvent, (void* keyEvent))
DECLARE_METHOD(bool, ProcessMotionEvent, (void* motionEvent))
DECLARE_METHOD(bool, HandleCmdInitWindow, (ANativeWindow* window, int focused))
DECLARE_METHOD(void, HandleCmdTermWindow, (ANativeWindow* window))
DECLARE_METHOD(void, HandleCmdWindowResized, (ANativeWindow* window))
DECLARE_METHOD(void, HandleCmdConfigChanged, (const UnityConfiguration& configuration))
DECLARE_METHOD(void, HandleCmdWindowInsetsChanged, (ANativeWindow* window))
DECLARE_METHOD(void, HandleCmdSoftwareKeyboardVisibilityChanged, (bool visible))
DECLARE_METHOD(void, HandleCmdLowMemory, (MemoryUsage memoryUsage))
DECLARE_METHOD(void, HandleCmdStart, ())
DECLARE_METHOD(void, HandleCmdStop, ())
DECLARE_METHOD(void, HandleCmdEditorAction, (int editorAction))
DECLARE_METHOD(bool, FocusChanged, (ANativeWindow* window, int focus))
DECLARE_METHOD(bool, PauseChanged, (ANativeWindow* window, int pause))
DECLARE_METHOD(bool, IsStaticSplashScreenEnabled, ())
DECLARE_METHOD(void, DisableStaticSplashScreen, ())

// Returns a hint if ReportFullyDrawn should be called
// This function will return false if unity.auto-report-fully-drawn is missing in the manifest
// unity.auto-report-fully-drawn is not added in manifest if Unity detects
// UnityEngine.Android.DiagnosticsReporting::CallReportFullyDrawn function call in C#
DECLARE_METHOD(bool, ShouldReportFullyDrawn, ())

// Call https://developer.android.com/reference/android/app/Activity#reportFullyDrawn
DECLARE_METHOD(void, ReportFullyDrawn, ())

// Returns true if player loop needs to be active while not focused
DECLARE_METHOD(bool, ShouldRunInBackground, ())

// Execute items queued via UnityPlayer.invokeOnMainThread
DECLARE_METHOD(void, ExecuteMainThreadJobs, ())

// Was UnityPlayer java layer running
DECLARE_METHOD(bool, IsUnityPlayerJavaRunning, ())

// Queues C# method call from a script attached to a game object
// Note: if your C# method doesn't have any parameters, parameter value is ignored, but still must be passed as empty string.
// Note: the C# method is not invoked instantaneously, it will be invoked during SendMessage queue processing
DECLARE_PUBLIC_METHOD(void, SendMessage, (const char* gameObjectName, const char* methodName, const char* parameter))

DECLARE_METHOD(bool, IsInMultiWindowMode, ())
