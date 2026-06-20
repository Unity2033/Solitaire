#pragma once

#include <stdint.h>

struct android_app;
struct android_input_buffer;

namespace Unity
{
    class UnityMotionEventCallbacks
    {
    public:
#include "MacroHeaderBegin.h"
    #include "UnityToGAMotionEventCallbacks.h"
#include "MacroEnd.h"
        UnityMotionEventCallbacks();
    private:
        static int32_t _GetDeviceIdImpl(void* motionEvent);
        static int32_t _GetSourceImpl(void* motionEvent);
        static int32_t _GetActionImpl(void* motionEvent);
        static int64_t _GetEventTimeMillisImpl(void* motionEvent);
        static int64_t _GetDownTimeMillisImpl(void* motionEvent);
        static int32_t _GetFlagsImpl(void* motionEvent);
        static int32_t _GetMetaStateImpl(void* motionEvent);
        static int32_t _GetActionButtonImpl(void* motionEvent);
        static int32_t _GetButtonStateImpl(void* motionEvent);
        static int32_t _GetClassificationImpl(void* motionEvent);
        static int32_t _GetEdgeFlagsImpl(void* motionEvent);
        static int32_t _GetPointerCountImpl(void* motionEvent);
        static int32_t _GetPointerIdImpl(void* motionEvent, int32_t pointerIndex);
        static float _GetXImpl(void* motionEvent, int32_t pointerIndex);
        static float _GetYImpl(void* motionEvent, int32_t pointerIndex);
        static float _GetPressureImpl(void* motionEvent, int32_t pointerIndex);
        static int32_t _GetPrecisionXImpl(void* motionEvent);
        static int32_t _GetPrecisionYImpl(void* motionEvent);
        static float _GetTouchMinorImpl(void* motionEvent, int32_t pointerIndex);
        static float _GetTouchMajorImpl(void* motionEvent, int32_t pointerIndex);
        static int32_t _GetHistorySizeImpl(void* motionEvent);
        static int64_t _GetHistoricalEventTimeMillisImpl(void* motionEvent, int32_t position);
        static float _GetHistoricalAxisValueImpl(void* motionEvent, int32_t axis, int32_t pointerIndex, int32_t position);
        static float _GetHistoricalPressureImpl(void* motionEvent, int32_t pointerIndex, int32_t position);
        static float _GetHistoricalSizeImpl(void* motionEvent, int32_t pointerIndex, int32_t position);
        static float _GetHistoricalXImpl(void* motionEvent, int32_t pointerIndex, int position);
        static float _GetHistoricalYImpl(void* motionEvent, int32_t pointerIndex, int position);
        static float _GetHistoricalTouchMinorImpl(void* motionEvent, int32_t pointerIndex, int position);
        static float _GetHistoricalTouchMajorImpl(void* motionEvent, int32_t pointerIndex, int position);
        static float _GetOrientationImpl(void* motionEvent, int32_t pointerIndex);
        static float _GetAxisValueImpl(void* motionEvent, int32_t axis, int32_t pointerIndex);
        static float _GetSizeImpl(void* motionEvent, int32_t pointerIndex);
        static int32_t _GetToolTypeImpl(void* motionEvent, int32_t pointerIndex);
        static void* _DeepCloneImpl(void* motionEvent, void* (*allocate)(size_t));
    };

    class UnityKeyEventCallbacks
    {
    public:
#include "MacroHeaderBegin.h"
    #include "UnityToGAKeyEventCallbacks.h"
#include "MacroEnd.h"
        UnityKeyEventCallbacks();
    private:
        static int32_t _GetDeviceIdImpl(void* keyEvent);
        static int32_t _GetSourceImpl(void* keyEvent);
        static int32_t _GetActionImpl(void* keyEvent);
        static int64_t _GetEventTimeMillisImpl(void* keyEvent);
        static int64_t _GetDownTimeMillisImpl(void* keyEvent);
        static int32_t _GetFlagsImpl(void* keyEvent);
        static int32_t _GetMetaStateImpl(void* keyEvent);
        static int32_t _GetModifiersImpl(void* keyEvent);
        static int32_t _GetRepeatCountImpl(void* keyEvent);
        static int32_t _GetKeyCodeImpl(void* keyEvent);
        static int32_t _GetUnicodeCharImpl(void* keyEvent);
        static const char* _GetCharactersImpl(void* keyEvent);
    };
}
