#if EXTERNAL_GAME_ACTIVITY_CODE
#include "UGAInput.h"
#include "UGAApplication.h"
#include "UGADebug.h"
#include "UGAEvents.h"
#include "game-activity/native_app_glue/android_native_app_glue.h"

bool Unity::UnityApplication::InitializeInputEvents()
{
    // Allow Unity to handle all motion events (for ex., Gamepad)
    android_app_set_motion_event_filter(m_AndroidAppInstance, nullptr);
    // Don't set filter for key events, since Game Activity allows all key events except system ones:
    // Volume Up/Down/Mute, Camera, Zoom In/Out
    // Which is fine with us
    const int32_t kAxisToEnable[] = {
            AMOTION_EVENT_AXIS_X,
            AMOTION_EVENT_AXIS_Y,
            AMOTION_EVENT_AXIS_PRESSURE,
            AMOTION_EVENT_AXIS_SIZE,
            AMOTION_EVENT_AXIS_TOUCH_MAJOR,
            AMOTION_EVENT_AXIS_TOUCH_MINOR,
            AMOTION_EVENT_AXIS_ORIENTATION,
            AMOTION_EVENT_AXIS_VSCROLL,
            AMOTION_EVENT_AXIS_HSCROLL,
            AMOTION_EVENT_AXIS_Z,
            AMOTION_EVENT_AXIS_RX,
            AMOTION_EVENT_AXIS_RY,
            AMOTION_EVENT_AXIS_RZ,
            AMOTION_EVENT_AXIS_HAT_X,
            AMOTION_EVENT_AXIS_HAT_Y,
            AMOTION_EVENT_AXIS_LTRIGGER,
            AMOTION_EVENT_AXIS_RTRIGGER,
            AMOTION_EVENT_AXIS_GAS,
            AMOTION_EVENT_AXIS_BRAKE,
            AMOTION_EVENT_AXIS_TILT,
            AMOTION_EVENT_AXIS_RELATIVE_X,
            AMOTION_EVENT_AXIS_RELATIVE_Y
    };
    for (const auto& it : kAxisToEnable)
        GameActivityPointerAxes_enableAxis(it);

    return true;
}

bool Unity::UnityApplication::ProcessInputEvents()
{
    if (m_AndroidInputBuffer != nullptr)
        UNITY_LOG_ERROR("Previous input buffer was not cleaned up");
    m_AndroidInputBuffer = android_app_swap_input_buffers(m_AndroidAppInstance);
    if (m_AndroidInputBuffer == nullptr)
        return false;

    GetEvents().Invoke(UnityEventProcessInput(*this, m_AndroidInputBuffer));

    const int kTimeModifier = 1000000;
    if (m_AndroidInputBuffer->keyEventsCount != 0)
    {
        for (uint64_t i = 0; i < m_AndroidInputBuffer->keyEventsCount; ++i)
        {
            GameActivityKeyEvent* keyEvent = &m_AndroidInputBuffer->keyEvents[i];
            // Bug: https://partnerissuetracker.corp.google.com/u/1/issues/315097052
            //      "Inconsistent eventTime/downTime format between GameActivityKeyEvent & GameActivityMotionEvent in GameActivity package 3.0.0-beta01"
            //      The bug was closed as 'Won't fix', hopefully it will be fixed in GameActivity 4.x package, leaving this for reference.
            //      For now, manually convert nano to milli seconds
            keyEvent->eventTime /= kTimeModifier;
            keyEvent->downTime /= kTimeModifier;
            ProcessKeyEvent(keyEvent);
        }
    }

    if (m_AndroidInputBuffer->motionEventsCount != 0)
    {
        for (uint64_t i = 0; i < m_AndroidInputBuffer->motionEventsCount; ++i)
        {
            GameActivityMotionEvent* motionEvent = &m_AndroidInputBuffer->motionEvents[i];

            ProcessMotionEvent(motionEvent);
        }
    }

    return true;
}

bool Unity::UnityApplication::ProcessTextInputEvents()
{
    if (m_AndroidAppInstance->textInputState)
    {
        GetSoftKeyboard().OnTextInput();
        m_AndroidAppInstance->textInputState = 0;
        return true;
    }
    return false;
}

bool Unity::UnityApplication::CleanInputEvents()
{
    if (m_AndroidInputBuffer != nullptr)
    {
        android_app_clear_key_events(m_AndroidInputBuffer);
        android_app_clear_motion_events(m_AndroidInputBuffer);
        m_AndroidInputBuffer = nullptr;
        return true;
    }
    return false;
}

#endif
