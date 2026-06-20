#pragma once

#include <stdint.h>
#include <string>

struct android_app;

namespace Unity
{
    class UnityApplication;

    class UnityConfiguration
    {
        android_app* m_AndroidAppInstance;
    public:
#include "MacroHeaderBegin.h"
    #include "UnityToGAConfigurationCallbacks.h"
#include "MacroEnd.h"
        UnityConfiguration(android_app* appInstance);

        void RefreshLocale();
    private:
        int _GetSdkVersionImpl() const;
        int _GetColorModeImpl() const;
        int _GetDensityDpiImpl() const;
        float _GetFontScaleImpl() const;
        int _GetFontWeightAdjustmentImpl() const;
        int _GetHardKeyboardHiddenImpl() const;
        int _GetKeyboardImpl() const;
        int _GetKeyboardHiddenImpl() const;
        int _GetMccImpl() const;
        int _GetMncImpl() const;
        int _GetNavigationImpl() const;
        int _GetNavigationHiddenImpl() const;
        int _GetOrientationImpl() const;
        int _GetScreenHeightDpImpl() const;
        int _GetScreenLayoutImpl() const;
        int _GetScreenWidthDpImpl() const;
        int _GetSmallestScreenWidthDpImpl() const;
        int _GetTouchscreenImpl() const;
        int _GetUIModeImpl() const;
        const char* _GetLocaleLanguageImpl() const;
        const char* _GetLocaleCountryImpl() const;
    private:
        std::string m_CachedLocaleLanguage;
        std::string m_CachedLocaleCountry;
    };
}
