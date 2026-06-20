// This source code is compiled into libgame.so, while Unity runtime lives in libunity.so
// The macros below declare function pointers which are set in libgame.so
// Allowing libunity.so to call into libgame.so

// Returns the last activity saved state, as provided at creation time.
// It is NULL if there was no state. You can use this as you need; the
// memory will remain around until you call android_app_exec_cmd() for
// APP_CMD_RESUME, at which point it will be freed and savedState set to
// NULL.These variables should only be changed when processing a
// APP_CMD_SAVE_STATE, at which point they will be initialized to NULL and
// you can malloc your state and place the information here.  In that case
// the memory will be freed for you later.
DECLARE_INSTANCE_METHOD(UnityApplication, void, GetSavedState, (void** savedState, size_t* savedStateSize) const, (savedState, savedStateSize))

// Returns the ALooper associated with the app's thread.
DECLARE_INSTANCE_METHOD(UnityApplication, ALooper*, GetLooper, () const, ())

// Returns the current content rectangle of the window; this is the area where the
// window's content should be placed to be seen by the user.
DECLARE_INSTANCE_METHOD(UnityApplication, const ARect&, GetContentRect, () const, ())

// Returns the current state of the app's activity.
// May be either APP_CMD_START,APP_CMD_RESUME, APP_CMD_PAUSE, or APP_CMD_STOP.
DECLARE_INSTANCE_METHOD(UnityApplication, int, GetActivityState, () const, ())

// Returns the global handle on the process's Java VM.
DECLARE_INSTANCE_METHOD(UnityApplication, JavaVM*, GetJavaVM, () const, ())

// Returns the JNI context for the UI thread of the app.  Note that this field
// can ONLY be used from the UI thread of the process; that is, the
// thread that calls into the GameActivityCallbacks.
DECLARE_INSTANCE_METHOD(UnityApplication, JNIEnv*, GetUIThreadJNIEnv, () const, ())

// Returns the JNI context for the Main thread of the app.  Note that this field
// can ONLY be used from the Main thread of the process; that is, the
// thread that calls into the GameActivityCallbacks.
DECLARE_INSTANCE_METHOD(UnityApplication, JNIEnv*, GetMainThreadJNIEnv, () const, ())

// Returns the GameActivity object handle.
DECLARE_INSTANCE_METHOD(UnityApplication, jobject, GetGameActivityJavaInstance, () const, ())

// Returns the path to this application's internal data directory.
DECLARE_INSTANCE_METHOD(UnityApplication, const char*, GetInternalDataPath, () const, ())

// Returns the path to this application's external (removable/mountable) data directory
DECLARE_INSTANCE_METHOD(UnityApplication, const char*, GetExternalDataPath, () const, ())

// Returns the path to the directory containing the application's OBB files (if any).
// If the app doesn't have any OBB files, this directory may not exist.
DECLARE_INSTANCE_METHOD(UnityApplication, const char*, GetObbPath, () const, ())

// Returns the platform's SDK version code
DECLARE_INSTANCE_METHOD(UnityApplication, int32_t, GetSdkVersion, () const, ())

DECLARE_INSTANCE_METHOD(UnityApplication, int32_t, GetMaxPointerCount, () const, ())

// Returns the pointer to the Asset Manager instance for the application.
// The application uses this to access binary assets bundled inside its own .apk file.
DECLARE_INSTANCE_METHOD(UnityApplication, AAssetManager*, GetAAssetManager, () const, ())

DECLARE_INSTANCE_METHOD(UnityApplication, Unity::UnityMotionEventCallbacks*, GetMotionEventCallbacks, () const, ())
DECLARE_INSTANCE_METHOD(UnityApplication, size_t, GetMotionEventSize, () const, ())
DECLARE_INSTANCE_METHOD(UnityApplication, size_t, GetMotionEventMaxAxis, () const, ())
DECLARE_INSTANCE_METHOD(UnityApplication, Unity::UnityKeyEventCallbacks*, GetKeyEventCallbacks, () const, ())
DECLARE_INSTANCE_METHOD(UnityApplication, size_t, GetKeyEventSize, () const, ())

DECLARE_INSTANCE_METHOD(UnityApplication, bool, GetWindowInsets, (Unity::InsetsType insetsType, ARect* outRect) const, (insetsType, outRect))

DECLARE_INSTANCE_METHOD(UnityApplication, void, ProcessApplicationCommandsForErrorDialog, (), ())
