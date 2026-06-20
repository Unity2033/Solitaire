#if !EXTERNAL_GAME_ACTIVITY_CODE
#include "UnityPrefix.h"
#endif
#include "UGAApplication.h"

static Unity::UnityApplication* s_Instance;

#if EXTERNAL_GAME_ACTIVITY_CODE

#include "game-activity/native_app_glue/android_native_app_glue.h"
#include "game-activity/GameActivity.h"
#include "UGAEvents.h"

Unity::UnityInitializePtr Unity::UnityApplication::UnityInitialize = nullptr;
Unity::UnityShutdownPtr Unity::UnityApplication::UnityShutdown = nullptr;
void ErrorLoop(struct android_app* app);

Unity::UnityApplication::UnityApplication(android_app* instance)
    : m_AndroidAppInstance(instance)
    , m_AndroidInputBuffer(nullptr)
    , m_HasFocus(false)
    , m_HasWindow(false)
    , m_IsVisible(false)
    , m_IsPaused(false)
    , m_Initialized(false)
    , m_QuitRequested(false)
{
    auto attachEnvResult = m_AndroidAppInstance->activity->vm->AttachCurrentThread(&m_MainThreadJniEnv, NULL);
    if (attachEnvResult != JNI_OK)
        UNITY_FATAL_ERROR("Couldn't acquire JNIEnv from android_app instance, result: %d", attachEnvResult);

    memset(&m_SavedState, 0, sizeof(m_SavedState));
    InitializeInputEvents();
    SetUnityToGameActivityCallbacks();

    m_SoftKeyboard = std::make_unique<UnitySoftKeyboard>(this);
    m_Configuration = std::make_unique<UnityConfiguration>(m_AndroidAppInstance);
}

Unity::UnityApplication::~UnityApplication()
{
    m_AndroidAppInstance->activity->vm->DetachCurrentThread();
}

Unity::UnityApplication* Unity::UnityApplication::CreateInstance(android_app* androidAppInstance)
{
    UNITY_LOG_INFO("UnityApplication::CreateInstance");
    UNITY_LOG_INFO("GameActivity Package Version '%d.%d.%d'", GAMEACTIVITY_MAJOR_VERSION, GAMEACTIVITY_MINOR_VERSION, GAMEACTIVITY_BUGFIX_VERSION);

    void* handle = dlopen("libunity.so", RTLD_LAZY);
    if (!handle)
    {
        UNITY_LOG_ERROR("Failed to load libunity.so");
        return NULL;
    }

    UnityInitialize = Unity::GetUnityInitializeFunc(handle);
    if (UnityInitialize == NULL)
    {
        UNITY_LOG_ERROR("Failed to acquire UnityInitialize function");
        return NULL;
    }

    UnityShutdown = Unity::GetUnityShutdownFunc(handle);
    if (UnityShutdown == NULL)
    {
        UNITY_LOG_ERROR("Failed to acquire UnityShutdown function");
        return NULL;
    }

    s_Instance = new UnityApplication(androidAppInstance);

    const char* error = UnityInitialize(s_Instance, sizeof(Unity::UnityApplication));

    if (error)
    {
        UNITY_LOG_ERROR("%s", error);
        // Don't call UnityShutdown if UnityInitialize failed
        delete s_Instance;
        s_Instance = NULL;
        return s_Instance;
    }

    return s_Instance;
}

void Unity::UnityApplication::DestroyInstance()
{
    UNITY_LOG_INFO("UnityApplication::DestroyInstance");
    if (s_Instance != NULL)
    {
        UnityShutdown();
        delete s_Instance;
    }
    s_Instance = NULL;
}

void Unity::UnityApplication::SetUnityToGameActivityCallbacks()
{
#include "MacroSourceBegin.h"
    #include "UnityToGACallbacks.h"
#include "MacroEnd.h"
}

void Unity::UnityApplication::_GetSavedStateImpl(void** savedState, size_t* savedStateSize) const
{
    *savedState = m_AndroidAppInstance->savedState;
    *savedStateSize = m_AndroidAppInstance->savedStateSize;
}

ALooper* Unity::UnityApplication::_GetLooperImpl() const
{
    return m_AndroidAppInstance->looper;
}

const ARect& Unity::UnityApplication::_GetContentRectImpl() const
{
    return m_AndroidAppInstance->contentRect;
}

int Unity::UnityApplication::_GetActivityStateImpl() const
{
    return m_AndroidAppInstance->activityState;
}

JavaVM* Unity::UnityApplication::_GetJavaVMImpl() const
{
    return m_AndroidAppInstance->activity->vm;
}

JNIEnv* Unity::UnityApplication::_GetUIThreadJNIEnvImpl() const
{
    UNITY_ASSERT_RUNNING_ON_UI_THREAD("GetUIThreadJNIEnv must be called from the UI thread");
    return m_AndroidAppInstance->activity->env;
}

JNIEnv* Unity::UnityApplication::_GetMainThreadJNIEnvImpl() const
{
    UNITY_ASSERT_RUNNING_ON_MAIN_THREAD("GetMainThreadJNIEnv must be called from the main thread");
    return m_MainThreadJniEnv;
}

jobject Unity::UnityApplication::_GetGameActivityJavaInstanceImpl() const
{
    return m_AndroidAppInstance->activity->javaGameActivity;
}

const char* Unity::UnityApplication::_GetInternalDataPathImpl() const
{
    return m_AndroidAppInstance->activity->internalDataPath;
}

const char* Unity::UnityApplication::_GetExternalDataPathImpl() const
{
    return m_AndroidAppInstance->activity->externalDataPath;
}

const char* Unity::UnityApplication::_GetObbPathImpl() const
{
    return m_AndroidAppInstance->activity->obbPath;
}

int32_t Unity::UnityApplication::_GetSdkVersionImpl () const
{
    // Note: This value is never set, thus returns always 0 ?
    //       Tested with androidx.games:games-activity:1.1.0
    return m_AndroidAppInstance->activity->sdkVersion;
}

int32_t Unity::UnityApplication::_GetMaxPointerCountImpl () const
{
    return GAMEACTIVITY_MAX_NUM_POINTERS_IN_MOTION_EVENT;
}

AAssetManager* Unity::UnityApplication::_GetAAssetManagerImpl() const
{
    return m_AndroidAppInstance->activity->assetManager;
}

bool Unity::UnityApplication::_GetWindowInsetsImpl(Unity::InsetsType insetType, ARect* outRect) const
{
    GameCommonInsetsType type;
    switch (insetType) {
        case Unity::InsetsType::StatusBars:
            type = GameCommonInsetsType::GAMECOMMON_INSETS_TYPE_STATUS_BARS;
            break;
        case Unity::InsetsType::NavigationBars:
            type = GameCommonInsetsType::GAMECOMMON_INSETS_TYPE_NAVIGATION_BARS;
            break;
        case Unity::InsetsType::CaptionBar:
            type = GameCommonInsetsType::GAMECOMMON_INSETS_TYPE_CAPTION_BAR;
            break;
        case Unity::InsetsType::IME:
            type = GameCommonInsetsType::GAMECOMMON_INSETS_TYPE_IME;
            break;
        case Unity::InsetsType::SystemGestures:
            type = GameCommonInsetsType::GAMECOMMON_INSETS_TYPE_SYSTEM_GESTURES;
            break;
        case Unity::InsetsType::MandatorySystemGestures:
            type = GameCommonInsetsType::GAMECOMMON_INSETS_TYPE_MANDATORY_SYSTEM_GESTURES;
            break;
        case Unity::InsetsType::TappableElement:
            type = GameCommonInsetsType::GAMECOMMON_INSETS_TYPE_TAPABLE_ELEMENT;
            break;
        case Unity::InsetsType::DisplayCutout:
            type = GameCommonInsetsType::GAMECOMMON_INSETS_TYPE_DISPLAY_CUTOUT;
            break;
        default:
            UNITY_LOG_WARNING("Unknown insets type: %d", insetType);
            return false;
    }

     GameActivity_getWindowInsets(m_AndroidAppInstance->activity, type, outRect);
     return true;
}

void Unity::UnityApplication::_ProcessApplicationCommandsForErrorDialogImpl()
{
    ErrorLoop(m_AndroidAppInstance);
}

bool Unity::UnityApplication::IsAnimating()  const
{
    bool gameLoopIsRunnable = m_HasFocus || ShouldRunInBackground();
    return m_Initialized && gameLoopIsRunnable && m_IsVisible && m_HasWindow && !m_QuitRequested;
}

void Unity::UnityApplication::HandleCmdFocusGained(ANativeWindow* window)
{
    m_SavedState.hasFocus = m_HasFocus = true;
    FocusChanged(window, 1);
}

void Unity::UnityApplication::HandleCmdFocusLost(ANativeWindow* window)
{
    m_SavedState.hasFocus = m_HasFocus = false;
    FocusChanged(window, 0);
}

void Unity::UnityApplication::HandleCmdPause(ANativeWindow* window)
{
    PauseChanged(window, 1);
}

void Unity::UnityApplication::HandleCmdResume(ANativeWindow* window)
{
    PauseChanged(window, 0);
}

bool Unity::UnityApplication::IgnorePauseEvent() const
{
    // Allow Unity to stay active during pause. For ex.,
    // Phones: The application is in split screen mode and permission dialog shows up.
    // XR: Permission dialog shows up.
    return m_Initialized && IsInMultiWindowMode();
}

static bool IgnoreApplicationCommandLogging(int32_t cmd)
{
    // These can be spammy
    return cmd == APP_CMD_TOUCH_EVENT || cmd == APP_CMD_KEY_EVENT;
}

void Unity::UnityApplication::OnApplicationCommand(struct android_app *app, int32_t cmd)
{
    auto* unityApp = (Unity::UnityApplication*) app->userData;

    if (!IgnoreApplicationCommandLogging(cmd)) {
        UNITY_LOG_INFO("Handle cmd %s", Unity::AppCmdName(cmd).c_str());
    }

    unityApp->GetEvents().Invoke(UnityEventProcessApplicationCommandBefore(*unityApp, cmd));
    switch (cmd) {
        case APP_CMD_INIT_WINDOW:
            if (app->window != NULL)
            {
                unityApp->m_HasWindow = true;

                if (app->savedStateSize == sizeof(unityApp->m_SavedState) && app->savedState != nullptr)
                {
                    unityApp->m_SavedState = *((SavedState *) app->savedState);
                    unityApp->m_HasFocus  = unityApp->m_SavedState.hasFocus;
                }
                else
                {
                    // No saved state
                    // Workaround APP_CMD_GAINED_FOCUS issue where the focus state is not
                    // passed down from NativeActivity when restarting Activity
                    // see https://github.com/android/games-samples/blob/f57b3bb888bf0dc828219f528eee5fe49b716186/codelabs/native-gamepad/start/app/src/main/cpp/native_engine.cpp#L180C1-L181C80
                    // keeping m_HasFocus to be same as it was set by APP_CMD_LOST_FOCUS/APP_CMD_GAINED_FOCUS
                }

                unityApp->HandleCmdInitWindow(app->window, unityApp->m_HasFocus ? 1 : 0);

                // Note: Unity can only start if there's a window attached.
                //       We could probably start loading faster and thus save few miliseconds.
                //       Internal issue tracker: PLAT-3225
                if (!unityApp->m_Initialized) {
                    unityApp->m_Initialized = unityApp->InitializeRuntime();
                }
                else {
                    unityApp->HandleCmdResume(app->window);
                }
            }

            UNITY_LOG_INFO("    HasWindow = %d, HasFocus = %d", unityApp->m_HasWindow ? 1 : 0, unityApp->m_HasFocus ? 1 : 0);
            break;
        case APP_CMD_TERM_WINDOW:
            unityApp->HandleCmdPause(app->window);
            unityApp->HandleCmdTermWindow(app->window);
            unityApp->m_HasWindow = false;
            break;
        case APP_CMD_WINDOW_RESIZED:
            unityApp->HandleCmdWindowResized(app->window);
            break;
        case APP_CMD_CONFIG_CHANGED:
            unityApp->GetConfiguration().RefreshLocale();
            unityApp->HandleCmdConfigChanged(unityApp->GetConfiguration());
            break;
        case APP_CMD_WINDOW_INSETS_CHANGED:
            unityApp->HandleCmdWindowInsetsChanged(app->window);
            break;
        case APP_CMD_SOFTWARE_KB_VIS_CHANGED:
        {
            bool keyboardVisible = GameActivity_isSoftwareKeyboardVisible(app->activity);
            UNITY_LOG_INFO("    Keyboard Visible = %s", keyboardVisible ? "True" : "False");
            unityApp->HandleCmdSoftwareKeyboardVisibilityChanged(keyboardVisible);
            break;
        }
        case APP_CMD_GAINED_FOCUS:
            unityApp->HandleCmdFocusGained(app->window);
            break;
        case APP_CMD_LOST_FOCUS:
            unityApp->HandleCmdFocusLost(app->window);
            break;
        case APP_CMD_LOW_MEMORY:
            unityApp->HandleCmdLowMemory(MemoryUsage::Critical);
            break;
        case APP_CMD_START:
            unityApp->m_IsVisible = true;
            unityApp->HandleCmdStart();
            break;
        case APP_CMD_RESUME:
            // Only resume if we were paused previously.
            // Note: In multiwindow mode we ignore pause events
            // This also ensures the following scenario is handled correctly:
            // * Launch first app in full screen, multiwindow mode = false
            // * Launch a second app, but in split screen mode. Second app acquires focus
            // * The first app will get paused (since multiwindow mode didn't switch to true yet)
            // * Try switching back to first app, multiwindow is true now. The app needs to properly resume

            if (!unityApp->m_IsPaused)
                break;
            unityApp->m_IsPaused = false;

            unityApp->HandleCmdResume(app->window);
            break;
        case APP_CMD_SAVE_STATE:
            unityApp->m_SavedState.hasFocus = unityApp->m_HasFocus;
            app->savedStateSize = sizeof(unityApp->m_SavedState);
            app->savedState = malloc(app->savedStateSize);
            *((SavedState *) app->savedState) = unityApp->m_SavedState;
            break;
        case APP_CMD_PAUSE:
            if (unityApp->IgnorePauseEvent())
                break;

            unityApp->m_IsPaused = true;
            // It's not guaranteed that frame will be processed one more time before the pause command is handled
            // Thus sometimes we might miss the quit request event (when C# Application.Quit is called from pause),
            // to avoid that, explicitly process frame one more time.
            // We can process the pending frame only if the relative Native window is not null.
            // In some edge cases the Native window is temporarily null (e.g. when a fullscreen video player is running).
            // In that particular scenario, the process frame has already been executed before the Windows switch.
            if (app->window != nullptr)
                unityApp->ProcessFrame();

            unityApp->HandleCmdPause(app->window);
            break;
        case APP_CMD_STOP:
            unityApp->m_IsVisible = false;
            unityApp->HandleCmdStop();
            break;
        case APP_CMD_EDITOR_ACTION:
            UNITY_LOG_INFO("    EditorAction %s", Unity::EditorActionName(unityApp->m_AndroidAppInstance->editorAction).c_str());
            unityApp->HandleCmdEditorAction(unityApp->m_AndroidAppInstance->editorAction);
            break;
        case APP_CMD_KEY_EVENT:
            // New key events came to m_AndroidInputBuffer
            break;
        case APP_CMD_TOUCH_EVENT:
            // New motion events came to m_AndroidInputBuffer
            break;
    }

    unityApp->GetEvents().Invoke(UnityEventProcessApplicationCommandAfter(*unityApp, cmd));
}

void Unity::UnityApplication::Loop()
{
    m_AndroidAppInstance->userData = this;
    m_AndroidAppInstance->onAppCmd = Unity::UnityApplication::OnApplicationCommand;
    UNITY_LOG_INFO("Starting Game Loop");

    int framesToRemoveStaticSplashScreen = 5;
    while (1)
    {
        int events;
        struct android_poll_source* source;

        // If not animating, block until we get an event; if animating, don't block.
        while ((ALooper_pollOnce(IsAnimating() ? 0 : -1, NULL, &events, (void**)&source)) >= 0)
        {
            auto app = GetAndroidAppInstance();
            // process event
            if (source != NULL)
            {
                source->process(app, source);
            }

            // are we exiting?
            if (app->destroyRequested)
            {
                return;
            }
        }

        if (framesToRemoveStaticSplashScreen >= 0)
        {
            if (framesToRemoveStaticSplashScreen == 0)
            {
                if (IsStaticSplashScreenEnabled())
                    DisableStaticSplashScreen();

                if (ShouldReportFullyDrawn())
                    ReportFullyDrawn();

                GetEvents().Invoke(UnityEventFirstSceneLoaded(*this));
            }

            framesToRemoveStaticSplashScreen--;
        }

        ExecuteMainThreadJobs();

        // Note: IsUnityPlayerRunning check is not inside IsAnimating, because it doesn't depend on APP_CMD_* commands
        //       Thus ALooper_pollOnce might not give us back control in case IsUnityPlayerJavaRunning returned value changes
        if (IsAnimating() && IsUnityPlayerJavaRunning())
            ProcessFrame();
    }
}

void Unity::UnityApplication::ProcessApplicationCommands() const
{
    int events;
    struct android_poll_source* source = NULL;

    if (ALooper_pollOnce(0 , NULL, &events, (void**)&source) >= 0)
    {
        // process event
        if (source == NULL)
            return;

        source->process(GetAndroidAppInstance(), source);
    }
}

void Unity::UnityApplication::ProcessFrame()
{
    ProcessInputEvents();
    ProcessTextInputEvents();
    bool invokeGameActivityFinish = false;
    if (!LoopRuntime() && !m_QuitRequested) {
        UNITY_LOG_INFO("Quit requested");
        m_QuitRequested = true;
        invokeGameActivityFinish = true;
    }
    CleanInputEvents();
    GetEvents().Invoke(UnityEventLoopAfter(*this));

    // Ensure GameActivity_finish is called once.
    if (invokeGameActivityFinish)
        GameActivity_finish(m_AndroidAppInstance->activity);
}

#else

#include "Runtime/Logging/LogAssert.h"

void UnityGameActivitySetInstance(Unity::UnityApplication* instance)
{
    UNITY_ASSERT(s_Instance == NULL || instance == NULL, "UnityApplication instance was already set");
    s_Instance = instance;
}

#endif

Unity::UnityApplication* Unity::UnityApplication::Instance()
{
    UNITY_ASSERT(s_Instance != NULL, "UnityApplication was not yet created!");
    return s_Instance;
}

bool Unity::UnityApplication::InstanceAvailable()
{
    return s_Instance != NULL;
}
