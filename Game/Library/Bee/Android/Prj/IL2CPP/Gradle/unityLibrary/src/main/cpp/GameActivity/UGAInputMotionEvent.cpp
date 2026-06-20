#if EXTERNAL_GAME_ACTIVITY_CODE
#include "UGAApplication.h"
#include "UGADebug.h"
#include "UGAVersion.h"
#include "game-activity/native_app_glue/android_native_app_glue.h"

#define UNITY_ASSERT_POINTER_INDEX(idx) UNITY_ASSERT(idx < GAMEACTIVITY_MAX_NUM_POINTERS_IN_MOTION_EVENT, "Invalid pointerIndex (%d), must be less than %d", \
             idx, GAMEACTIVITY_MAX_NUM_POINTERS_IN_MOTION_EVENT)

static Unity::UnityMotionEventCallbacks s_MotionEventCallbacks;

Unity::UnityMotionEventCallbacks* Unity::UnityApplication::_GetMotionEventCallbacksImpl() const
{
    return &s_MotionEventCallbacks;
}

size_t Unity::UnityApplication::_GetMotionEventSizeImpl() const
{
    return sizeof(GameActivityMotionEvent);
}

size_t Unity::UnityApplication::_GetMotionEventMaxAxisImpl() const
{
    return GAME_ACTIVITY_POINTER_INFO_AXIS_COUNT;
}

Unity::UnityMotionEventCallbacks::UnityMotionEventCallbacks()
{
#include "MacroSourceBegin.h"
    #include "UnityToGAMotionEventCallbacks.h"
#include "MacroEnd.h"
}

int32_t Unity::UnityMotionEventCallbacks::_GetDeviceIdImpl(void* motionEvent)
{
    return reinterpret_cast<GameActivityMotionEvent*>(motionEvent)->deviceId;
}

int32_t Unity::UnityMotionEventCallbacks::_GetSourceImpl(void* motionEvent)
{
    return reinterpret_cast<GameActivityMotionEvent*>(motionEvent)->source;
}

int32_t Unity::UnityMotionEventCallbacks::_GetActionImpl(void* motionEvent)
{
    return reinterpret_cast<GameActivityMotionEvent*>(motionEvent)->action;
}

int64_t Unity::UnityMotionEventCallbacks::_GetEventTimeMillisImpl(void* motionEvent)
{
    return reinterpret_cast<GameActivityMotionEvent*>(motionEvent)->eventTime;
}

int64_t Unity::UnityMotionEventCallbacks::_GetDownTimeMillisImpl(void* motionEvent)
{
    return reinterpret_cast<GameActivityMotionEvent*>(motionEvent)->downTime;
}

int32_t Unity::UnityMotionEventCallbacks::_GetFlagsImpl(void* motionEvent)
{
    return reinterpret_cast<GameActivityMotionEvent*>(motionEvent)->flags;
}

int32_t Unity::UnityMotionEventCallbacks::_GetMetaStateImpl(void* motionEvent)
{
    return reinterpret_cast<GameActivityMotionEvent*>(motionEvent)->metaState;
}

int32_t Unity::UnityMotionEventCallbacks::_GetActionButtonImpl(void* motionEvent)
{
    return reinterpret_cast<GameActivityMotionEvent*>(motionEvent)->actionButton;
}

int32_t Unity::UnityMotionEventCallbacks::_GetButtonStateImpl(void* motionEvent)
{
    return reinterpret_cast<GameActivityMotionEvent*>(motionEvent)->buttonState;
}

int32_t Unity::UnityMotionEventCallbacks::_GetClassificationImpl(void* motionEvent)
{
    return reinterpret_cast<GameActivityMotionEvent*>(motionEvent)->classification;
}

int32_t Unity::UnityMotionEventCallbacks::_GetEdgeFlagsImpl(void* motionEvent)
{
    return reinterpret_cast<GameActivityMotionEvent*>(motionEvent)->edgeFlags;
}

int32_t Unity::UnityMotionEventCallbacks::_GetPointerCountImpl(void* motionEvent)
{
    return reinterpret_cast<GameActivityMotionEvent*>(motionEvent)->pointerCount;
}

static const GameActivityPointerAxes* GetPointerAxes(void* motionEvent, int32_t pointerIndex)
{
    UNITY_ASSERT_POINTER_INDEX(pointerIndex);
    return &reinterpret_cast<GameActivityMotionEvent*>(motionEvent)->pointers[pointerIndex];
}

int32_t Unity::UnityMotionEventCallbacks::_GetPointerIdImpl(void* motionEvent, int32_t pointerIndex)
{
    UNITY_ASSERT_POINTER_INDEX(pointerIndex);
    return GetPointerAxes(motionEvent, pointerIndex)->id;
}

// Note: GameActivityPointerAxes rawX/rawY seems to be populated only Android 11 or higher (and hold the same values as AMOTION_EVENT_AXIS_X/AMOTION_EVENT_AXIS_Y)
//       but rawX/rawY contains zeroes on Android 9.

float Unity::UnityMotionEventCallbacks::_GetXImpl(void* motionEvent, int32_t pointerIndex)
{
    UNITY_ASSERT_POINTER_INDEX(pointerIndex);
    return GameActivityPointerAxes_getX(GetPointerAxes(motionEvent, pointerIndex));
}

float Unity::UnityMotionEventCallbacks::_GetYImpl(void* motionEvent, int32_t pointerIndex)
{
    UNITY_ASSERT_POINTER_INDEX(pointerIndex);
    return GameActivityPointerAxes_getY(GetPointerAxes(motionEvent, pointerIndex));
}

float Unity::UnityMotionEventCallbacks::_GetPressureImpl(void* motionEvent, int32_t pointerIndex)
{
    UNITY_ASSERT_POINTER_INDEX(pointerIndex);
    return GameActivityPointerAxes_getPressure(GetPointerAxes(motionEvent, pointerIndex));
}

int32_t Unity::UnityMotionEventCallbacks::_GetPrecisionXImpl(void* motionEvent)
{
    return reinterpret_cast<GameActivityMotionEvent*>(motionEvent)->precisionX;
}

int32_t Unity::UnityMotionEventCallbacks::_GetPrecisionYImpl(void* motionEvent)
{
    return reinterpret_cast<GameActivityMotionEvent*>(motionEvent)->precisionY;
}

float Unity::UnityMotionEventCallbacks::_GetTouchMinorImpl(void* motionEvent, int32_t pointerIndex)
{
    UNITY_ASSERT_POINTER_INDEX(pointerIndex);
    return GameActivityPointerAxes_getTouchMinor(GetPointerAxes(motionEvent, pointerIndex));
}

float Unity::UnityMotionEventCallbacks::_GetTouchMajorImpl(void* motionEvent, int32_t pointerIndex)
{
    UNITY_ASSERT_POINTER_INDEX(pointerIndex);
    return GameActivityPointerAxes_getTouchMajor(GetPointerAxes(motionEvent, pointerIndex));
}

int32_t Unity::UnityMotionEventCallbacks::_GetHistorySizeImpl(void* motionEvent)
{
    return GameActivityMotionEvent_getHistorySize(reinterpret_cast<GameActivityMotionEvent*>(motionEvent));
}

int64_t Unity::UnityMotionEventCallbacks::_GetHistoricalEventTimeMillisImpl(void* motionEvent, int32_t position)
{
    return reinterpret_cast<GameActivityMotionEvent*>(motionEvent)->historicalEventTimesMillis[position];
}

float Unity::UnityMotionEventCallbacks::_GetHistoricalAxisValueImpl(void* motionEvent, int32_t axis, int32_t pointerIndex, int32_t position)
{
    UNITY_ASSERT_POINTER_INDEX(pointerIndex);
    return GameActivityMotionEvent_getHistoricalAxisValue(reinterpret_cast<GameActivityMotionEvent*>(motionEvent), axis, pointerIndex, position);
}

float Unity::UnityMotionEventCallbacks::_GetHistoricalPressureImpl(void* motionEvent, int32_t pointerIndex, int32_t position)
{
    UNITY_ASSERT_POINTER_INDEX(pointerIndex);
    return GameActivityMotionEvent_getHistoricalPressure(reinterpret_cast<GameActivityMotionEvent*>(motionEvent), pointerIndex, position);
}

float Unity::UnityMotionEventCallbacks::_GetHistoricalSizeImpl(void* motionEvent, int32_t pointerIndex, int32_t position)
{
    UNITY_ASSERT_POINTER_INDEX(pointerIndex);
    return GameActivityMotionEvent_getHistoricalSize(reinterpret_cast<GameActivityMotionEvent*>(motionEvent), pointerIndex, position);
}

float Unity::UnityMotionEventCallbacks::_GetHistoricalXImpl(void* motionEvent, int32_t pointerIndex, int position)
{
    UNITY_ASSERT_POINTER_INDEX(pointerIndex);
    return GameActivityMotionEvent_getHistoricalX(reinterpret_cast<GameActivityMotionEvent*>(motionEvent), pointerIndex, position);
}

float Unity::UnityMotionEventCallbacks::_GetHistoricalYImpl(void* motionEvent, int32_t pointerIndex, int position)
{
    UNITY_ASSERT_POINTER_INDEX(pointerIndex);
    return GameActivityMotionEvent_getHistoricalY(reinterpret_cast<GameActivityMotionEvent*>(motionEvent), pointerIndex, position);
}

float Unity::UnityMotionEventCallbacks::_GetHistoricalTouchMinorImpl(void* motionEvent, int32_t pointerIndex, int position)
{
    UNITY_ASSERT_POINTER_INDEX(pointerIndex);
    return GameActivityMotionEvent_getHistoricalTouchMinor(reinterpret_cast<GameActivityMotionEvent*>(motionEvent), pointerIndex, position);
}

float Unity::UnityMotionEventCallbacks::_GetHistoricalTouchMajorImpl(void* motionEvent, int32_t pointerIndex, int position)
{
    UNITY_ASSERT_POINTER_INDEX(pointerIndex);
    return GameActivityMotionEvent_getHistoricalTouchMajor(reinterpret_cast<GameActivityMotionEvent*>(motionEvent), pointerIndex, position);
}

float Unity::UnityMotionEventCallbacks::_GetOrientationImpl(void* motionEvent, int32_t pointerIndex)
{
    UNITY_ASSERT_POINTER_INDEX(pointerIndex);
    return GameActivityPointerAxes_getOrientation(GetPointerAxes(motionEvent, pointerIndex));
}

float Unity::UnityMotionEventCallbacks::_GetAxisValueImpl(void* motionEvent, int32_t axis, int32_t pointerIndex)
{
    UNITY_ASSERT_POINTER_INDEX(pointerIndex);
    return GameActivityPointerAxes_getAxisValue(GetPointerAxes(motionEvent, pointerIndex), axis);
}

float Unity::UnityMotionEventCallbacks::_GetSizeImpl(void* motionEvent, int32_t pointerIndex)
{
    UNITY_ASSERT_POINTER_INDEX(pointerIndex);
    return GameActivityPointerAxes_getSize(GetPointerAxes(motionEvent, pointerIndex));
}

int32_t Unity::UnityMotionEventCallbacks::_GetToolTypeImpl(void* motionEvent, int32_t pointerIndex)
{
    UNITY_ASSERT_POINTER_INDEX(pointerIndex);
    return GameActivityPointerAxes_getToolType(GetPointerAxes(motionEvent, pointerIndex));
}

void* Unity::UnityMotionEventCallbacks::_DeepCloneImpl(void* motionEvent, void* (*allocate)(size_t))
{
    auto original = reinterpret_cast<const GameActivityMotionEvent*>(motionEvent);

    #ifndef UNITY_TYPE_COMPAT_ASSERT
        #define UNITY_TYPE_COMPAT_ASSERT(expected, actual) static_assert(std::is_same<expected, std::remove_cv<std::remove_pointer<decltype(actual)>::type>::type>::value, "GameActivityMotionEvent structure changed since deep clone was implemented")
    #endif

    UNITY_TYPE_COMPAT_ASSERT(int64_t, original->historicalEventTimesMillis);
    UNITY_TYPE_COMPAT_ASSERT(int64_t, original->historicalEventTimesNanos);
    UNITY_TYPE_COMPAT_ASSERT(float, original->historicalAxisValues);

    auto size = sizeof(GameActivityMotionEvent);
    size_t extraOffset = 0;
    size_t historyTimesSize = 0;
    size_t axisSize = 0;
    if (original->historySize > 0)
    {
        auto alignment = alignof(int64_t);
        size += alignment - (size % alignment);
        extraOffset = size;
        historyTimesSize = original->historySize * sizeof(int64_t);
        axisSize = original->historySize * sizeof(float) * original->pointerCount * GAME_ACTIVITY_POINTER_INFO_AXIS_COUNT;
        size += historyTimesSize * 2 + axisSize;
    }

    void* copy = allocate(size);
    memcpy(copy, motionEvent, sizeof(GameActivityMotionEvent));
    if (original->historySize <= 0)
        return copy;

    void* extra = static_cast<int8_t*>(copy) + extraOffset;
    auto evt = reinterpret_cast<GameActivityMotionEvent*>(copy);
    evt->historicalEventTimesMillis = static_cast<int64_t*>(extra);
    memcpy(evt->historicalEventTimesMillis, original->historicalEventTimesMillis, historyTimesSize);
    evt->historicalEventTimesNanos = evt->historicalEventTimesMillis + original->historySize;
    memcpy(evt->historicalEventTimesNanos, original->historicalEventTimesNanos, historyTimesSize);
    auto axisPos = evt->historicalEventTimesNanos + original->historySize;
    evt->historicalAxisValues = reinterpret_cast<float*>(axisPos);
    memcpy(evt->historicalAxisValues, original->historicalAxisValues, axisSize);
    return copy;
}

#endif
