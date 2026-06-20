#pragma once

#include <stdint.h>

namespace Unity
{
    struct Range
    {
        int32_t start;
        int32_t length;

        Range()
            : start(0)
            , length(0)
        {
        }

        Range(int32_t start, int32_t length)
            : start(start)
            , length(length)
        {
        }
    };

    // See: https://android.googlesource.com/platform/frameworks/base/+/master/core/java/android/view/WindowInsets.java#1409
    enum InsetsType : int
    {
        StatusBars = 1 << 0,
        NavigationBars = 1 << 1 ,
        CaptionBar = 1 << 2,
        IME = 1 << 3,
        SystemGestures = 1 << 4,
        MandatorySystemGestures = 1 << 5,
        TappableElement = 1 << 6,
        DisplayCutout = 1 << 7
    };
}
