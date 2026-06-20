#if EXTERNAL_GAME_ACTIVITY_CODE
#include "UGAConfiguration.h"
#include "UGAApplication.h"
#include "game-activity/native_app_glue/android_native_app_glue.h"

Unity::UnityConfiguration::UnityConfiguration(android_app* appInstance)
    : m_AndroidAppInstance(appInstance)
{
#include "MacroSourceBegin.h"
    #include "UnityToGAConfigurationCallbacks.h"
#include "MacroEnd.h"

    RefreshLocale();
}

void Unity::UnityConfiguration::RefreshLocale()
{
    char lang[3] = { 0 };
    char country[3] = { 0 };

    AConfiguration_getLanguage(m_AndroidAppInstance->config, lang);
    AConfiguration_getCountry(m_AndroidAppInstance->config, country);

    m_CachedLocaleLanguage = lang;
    m_CachedLocaleCountry = country;
}

int Unity::UnityConfiguration::_GetSdkVersionImpl() const
{
    return AConfiguration_getSdkVersion(m_AndroidAppInstance->config);
}

int Unity::UnityConfiguration::_GetColorModeImpl() const
{
    return GameActivity_getColorMode(m_AndroidAppInstance->activity);
}

int Unity::UnityConfiguration::_GetDensityDpiImpl() const
{
    return GameActivity_getDensityDpi(m_AndroidAppInstance->activity);
}

float Unity::UnityConfiguration::_GetFontScaleImpl() const
{
    return GameActivity_getFontScale(m_AndroidAppInstance->activity);
}

int Unity::UnityConfiguration::_GetFontWeightAdjustmentImpl() const
{
    return GameActivity_getFontWeightAdjustment(m_AndroidAppInstance->activity);
}

int Unity::UnityConfiguration::_GetHardKeyboardHiddenImpl() const
{
    return GameActivity_getHardKeyboardHidden(m_AndroidAppInstance->activity);
}

int Unity::UnityConfiguration::_GetKeyboardImpl() const
{
    return GameActivity_getKeyboard(m_AndroidAppInstance->activity);
}

int Unity::UnityConfiguration::_GetKeyboardHiddenImpl() const
{
    return GameActivity_getKeyboardHidden(m_AndroidAppInstance->activity);
}

int Unity::UnityConfiguration::_GetMccImpl() const
{
    return GameActivity_getMcc(m_AndroidAppInstance->activity);
}

int Unity::UnityConfiguration::_GetMncImpl() const
{
    return GameActivity_getMnc(m_AndroidAppInstance->activity);
}

int Unity::UnityConfiguration::_GetNavigationImpl() const
{
    return GameActivity_getNavigation(m_AndroidAppInstance->activity);
}

int Unity::UnityConfiguration::_GetNavigationHiddenImpl() const
{
    return GameActivity_getNavigationHidden(m_AndroidAppInstance->activity);
}

int Unity::UnityConfiguration::_GetOrientationImpl() const
{
    return GameActivity_getOrientation(m_AndroidAppInstance->activity);
}

int Unity::UnityConfiguration::_GetScreenHeightDpImpl() const
{
    return GameActivity_getScreenHeightDp(m_AndroidAppInstance->activity);
}

int Unity::UnityConfiguration::_GetScreenLayoutImpl() const
{
    return GameActivity_getScreenLayout(m_AndroidAppInstance->activity);
}

int Unity::UnityConfiguration::_GetScreenWidthDpImpl() const
{
    return GameActivity_getScreenWidthDp(m_AndroidAppInstance->activity);
}

int Unity::UnityConfiguration::_GetSmallestScreenWidthDpImpl() const
{
    return GameActivity_getSmallestScreenWidthDp(m_AndroidAppInstance->activity);
}

int Unity::UnityConfiguration::_GetTouchscreenImpl() const
{
    return GameActivity_getTouchscreen(m_AndroidAppInstance->activity);
}

int Unity::UnityConfiguration::_GetUIModeImpl() const
{
    return GameActivity_getUIMode(m_AndroidAppInstance->activity);
}

const char* Unity::UnityConfiguration::_GetLocaleLanguageImpl() const
{
    return m_CachedLocaleLanguage.c_str();
}

const char* Unity::UnityConfiguration::_GetLocaleCountryImpl() const
{
    return m_CachedLocaleCountry.c_str();
}

#endif


