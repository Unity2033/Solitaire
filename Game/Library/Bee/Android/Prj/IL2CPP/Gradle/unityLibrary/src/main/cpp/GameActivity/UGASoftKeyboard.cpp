#if EXTERNAL_GAME_ACTIVITY_CODE
#include "UGASoftKeyboard.h"
#include "UGAApplication.h"
#include "UGADebug.h"
#include "game-activity/native_app_glue/android_native_app_glue.h"
#include "game-activity/GameActivity.h"
#include <chrono>
#include <thread>


namespace Unity
{
    // For more information check out
    // https://android.googlesource.com/platform/frameworks/opt/gamesdk/+/refs/heads/master/GameTextInput/src/main/java/com/google/androidgamesdk/gametextinput

    struct UnitySoftKeyboardData
    {
        UnitySoftKeyboardData(const std::string &initial_string)
            : character_limit(0)
        {
            *this = initial_string;
        }

        UnitySoftKeyboardData &operator=(const GameTextInputState &rhs)
        {
            inner = rhs;
            owned_string.assign(rhs.text_UTF8, rhs.text_length);
            inner.text_UTF8 = owned_string.data();
            return *this;
        }

        UnitySoftKeyboardData& operator=(const std::string& text)
        {
            owned_string = text;
            inner.text_UTF8 = owned_string.data();
            inner.text_length = owned_string.length();
            inner.composingRegion.start = -1;
            inner.composingRegion.end = -1;
            ApplySelection(Range(owned_string.length(), 0));
            return *this;
        }

        void ApplySelection(const Range& selection)
        {
            inner.selection.start = std::min<int32_t>(owned_string.length(), selection.start);
            inner.selection.end = std::min<int32_t>(owned_string.length(), selection.start + selection.length);
            inner.selection.end = std::max<int32_t>(inner.selection.end, inner.selection.start);
        }

        GameTextInputState inner;
        std::string owned_string;
        uint32_t character_limit;
    };
}

// Used for synchronazing keyboard visibility state with Unity Main thread.
// Without synchronization, we might end up with invalid keyboard state on Unity side
// For example.
// * Let's assume initial keyboard state is opened because of the active UI input field
// * When clicking on the other input field, UI system is closing keyboard for first input field and openin keyboard for the second
// * Since under the hood there's only one keyboard instance, we're changing visibility for one keyboard
// * So when Hide, Show is performed in quick succession, we receive APP_CMD_SOFTWARE_KB_VIS_CHANGED with keyboard visibility false
//   Which informs Unity that keyboard was closed and we need to deactivate input field, even though the last request was to Show keyboard
static const auto kKeyboardVisibilitySynchronizationTimeOutMs = std::chrono::milliseconds(400);

Unity::UnitySoftKeyboard::UnitySoftKeyboard(Unity::UnityApplication* application)
    : m_Application(application)
    , m_OnTextInput(NULL)
{
    m_Data = new Unity::UnitySoftKeyboardData("");
#include "MacroSourceBegin.h"
    #include "UGASoftKeyboardCallbacks.h"
#include "MacroEnd.h"
}

Unity::UnitySoftKeyboard::~UnitySoftKeyboard()
{
    delete m_Data;
}

void Unity::UnitySoftKeyboard::OnTextInput()
{
    struct Context
    {
        UnitySoftKeyboard* keyboard;
        bool stateChanged;
    } context{this, false};

    auto activity = m_Application->GetAndroidAppInstance()->activity;
    GameActivity_getTextInputState(activity, [](void *context, const GameTextInputState *state) {
        auto ctx = static_cast<Context*>(context);
        auto limit = ctx->keyboard->m_Data->character_limit;
        if (limit == 0)
            ctx->stateChanged = true;
        else if (InputWithinLimits(state->text_UTF8, state->text_length, limit))
            ctx->stateChanged = true;
        else if (!InputWithinLimits(ctx->keyboard->m_Data->inner.text_UTF8, ctx->keyboard->m_Data->inner.text_length, limit)
            && ctx->keyboard->m_Data->inner.text_length >= state->text_length)
        {
            // abnormal situation where both old and new texts are too long
            // likely a bug or due to playing around with .text and .characterLimit
            // in such case take new if it's shorter (backspace works)
            ctx->stateChanged = true;
        }

        if (ctx->stateChanged)
            *ctx->keyboard->m_Data = *state;
    }, &context);

    // Do we need insets? Keep the code for reference
    /*
    ARect insets;
    GameTextInput_getImeInsets(GameActivity_getTextInput(activity), &insets);
    UNITY_LOG_INFO("SoftKeyboard: Text = %s, Selection = (Start = %d, End = %d), Composition = (Start = %d, End = %d), IME insets = (%d, %d, %d, %d)",
                   m_Data->owned_string.c_str(),
                   m_Data->inner.selection.start,
                   m_Data->inner.selection.end,
                   m_Data->inner.composingRegion.start,
                   m_Data->inner.composingRegion.end,
                   insets.left, insets.right, insets.top, insets.bottom);
    //*/
    if (context.stateChanged)
    {
        if (m_OnTextInput != NULL)
            m_OnTextInput();
    }
    else // reset state (reject the new one)
        UpdateTextInputState();
}

void Unity::UnitySoftKeyboard::_SetTextImpl(const std::string& text)
{
    *m_Data = text;
    TruncateToLimit();
    UpdateTextInputState();
}

void Unity::UnitySoftKeyboard::UpdateTextInputState()
{
    GameActivity_setTextInputState(m_Application->GetAndroidAppInstance()->activity, &m_Data->inner);
    // after external changes to text/selection/type etc. need to inform IME service
    RestartImeInput();
}

void Unity::UnitySoftKeyboard::RestartImeInput()
{
    GameActivity_restartInput(m_Application->GetAndroidAppInstance()->activity);
}

const std::string& Unity::UnitySoftKeyboard::_GetTextImpl() const
{
    return m_Data->owned_string;
}

void Unity::UnitySoftKeyboard::_SetTextSelectionImpl(const Range& textSelection)
{
    m_Data->ApplySelection(textSelection);
    UpdateTextInputState();
}

Unity::Range Unity::UnitySoftKeyboard::_GetTextSelectionImpl() const
{
    Range range;
    range.start = m_Data->inner.selection.start;
    range.length = m_Data->inner.selection.end - m_Data->inner.selection.start;
    return range;
}

bool Unity::UnitySoftKeyboard::InputWithinLimits(const char* text, uint32_t length, uint32_t limit)
{
    if (length <= limit)
        return true;
    // limit in Unicode code-points, so character can't be more than 4 bytes
    if (length > limit * 4)
        return false;

    auto bytesWithinLimit = SubStringBytes(text, length, limit);
    return bytesWithinLimit >= length;
}

// get number of bytes occupied by given number of chars
uint32_t Unity::UnitySoftKeyboard::SubStringBytes(const char* text, uint32_t length, uint32_t chars)
{
    // assume valid UTF-8, https://en.wikipedia.org/wiki/UTF-8#Encoding
    unsigned numChars = 0, i = 0;
    for (i = 0; i < length && numChars < chars; ++i)
    {
        if (0 == (text[i] & 0x80))  // single-byte char, highest bit 0
        {}
        else if (0xc0 == (text[i] & 0xe0))  // two-byte char, highest bits 110
            ++i; // skip next byte
        else if (0xe0 == (text[i] & 0xf0))  // three-byte char, highest bits 1110
            i += 2; // skip next two bytes
        else if (0xf0 == (text[i] & 0xf8))  // four byte char, highest bits 11110
            i += 3; // skip next three bytes
        else
            return UINT32_MAX;  // should not hit this point (invalid UTF8)

        ++numChars;
    }

    return i;
}

static GameTextInputType GetInputType(const Unity::UnitySoftKeyboard::ShowParams& showParams)
{
    int baseType = (showParams.correction ? TYPE_TEXT_FLAG_AUTO_CORRECT : TYPE_TEXT_FLAG_NO_SUGGESTIONS)
                   | (showParams.multiline ? TYPE_TEXT_FLAG_MULTI_LINE : 0)
                   | (showParams.secure ? TYPE_TEXT_VARIATION_PASSWORD : 0);

    int modifierType = TYPE_NULL;
    switch (showParams.type) {
        case Unity::UnitySoftKeyboard::KeyboardType::Default: modifierType = TYPE_CLASS_TEXT; break;
        case Unity::UnitySoftKeyboard::KeyboardType::ASCIICapable: modifierType = TYPE_CLASS_TEXT | TYPE_TEXT_FLAG_CAP_SENTENCES; break;
        case Unity::UnitySoftKeyboard::KeyboardType::NumbersAndPunctuation: modifierType = TYPE_CLASS_NUMBER | TYPE_NUMBER_FLAG_DECIMAL | TYPE_NUMBER_FLAG_SIGNED; break;
        case Unity::UnitySoftKeyboard::KeyboardType::URL: modifierType = TYPE_CLASS_TEXT | TYPE_TEXT_VARIATION_URI; break;
        case Unity::UnitySoftKeyboard::KeyboardType::NumberPad: modifierType = TYPE_CLASS_NUMBER; break;
        case Unity::UnitySoftKeyboard::KeyboardType::PhonePad: modifierType = TYPE_CLASS_PHONE; break;
        case Unity::UnitySoftKeyboard::KeyboardType::NamePhonePad: modifierType = TYPE_CLASS_TEXT | TYPE_TEXT_FLAG_CAP_WORDS | TYPE_TEXT_VARIATION_PERSON_NAME; break;
        case Unity::UnitySoftKeyboard::KeyboardType::EmailAddress: modifierType = TYPE_CLASS_TEXT | TYPE_TEXT_VARIATION_EMAIL_ADDRESS; break;
        case Unity::UnitySoftKeyboard::KeyboardType::Social: modifierType = TYPE_CLASS_TEXT | TYPE_TEXT_FLAG_CAP_SENTENCES | TYPE_TEXT_VARIATION_EMAIL_ADDRESS; break;
        case Unity::UnitySoftKeyboard::KeyboardType::Search: modifierType = TYPE_CLASS_TEXT | TYPE_TEXT_VARIATION_URI; break;
        case Unity::UnitySoftKeyboard::KeyboardType::DecimalPad: modifierType = TYPE_CLASS_TEXT | TYPE_CLASS_NUMBER; break;
        default: modifierType = TYPE_CLASS_TEXT; break;
    }

    // Discard TYPE_CLASS_TEXT bits if TYPE_CLASS_NUMBER bit is set
    if ((modifierType & TYPE_CLASS_NUMBER) != 0)
        return TYPE_CLASS_NUMBER;

    return (GameTextInputType)(baseType | modifierType);
}

void Unity::UnitySoftKeyboard::_ShowImpl(const ShowParams* showParams, uint32_t sizeOfShowParams)
{
    if (showParams != NULL)
    {
        if (sizeOfShowParams != sizeof(ShowParams))
        {
            UNITY_LOG_ERROR("SoftKeyboard: Invalid size of ShowParams, expected %d, was %d",
                            sizeof(ShowParams),
                            sizeOfShowParams);
        }
        m_OnTextInput = showParams->textInputFunc;
        *m_Data = showParams->initialText;
        m_Data->character_limit = showParams->characterLimit;

        auto inputType = (GameTextInputType)GetInputType(*showParams);
        auto actionFlags = IME_ACTION_UNSPECIFIED;
        auto imeOptions = IME_FLAG_NO_FULLSCREEN;

        if ((inputType & TYPE_TEXT_FLAG_MULTI_LINE) == 0)
        {
            actionFlags = IME_ACTION_DONE;

            // Workaround for a bug where Action button doesn't switch to the correct one when showing text keyboard with no multiline
            // If this is not specified APP_CMD_EDITOR_ACTION never comes
            // More details - https://partnerissuetracker.corp.google.com/u/1/issues/276725330
            imeOptions = (GameTextInputImeOptions) (IME_ACTION_DONE | imeOptions);
        }
        GameActivity_setImeEditorInfo(m_Application->GetAndroidAppInstance()->activity,
                                      inputType,
                                      actionFlags,
                                      imeOptions);
    }
    else
    {
        m_OnTextInput = NULL;
        *m_Data = "";
        m_Data->character_limit = 0;
    }

    UpdateTextInputState();

    // Note: Flags docs - https://developer.android.com/reference/android/view/inputmethod/InputMethodManager#SHOW_IMPLICIT
    int flags = 0;


    auto waitUntilVisible = !GameActivity_isSoftwareKeyboardVisible(m_Application->GetAndroidAppInstance()->activity);
    GameActivity_showSoftInput(m_Application->GetAndroidAppInstance()->activity, flags);
    if (waitUntilVisible)
    {
        auto result = WaitUntilKeyboardVisibility(true);
        if (!result)
            UNITY_LOG_WARNING("Timeout while waiting for keyboard to become visible");
    }
}

void Unity::UnitySoftKeyboard::_HideImpl()
{
    m_OnTextInput = NULL;
    int flags = 0;
    auto waitUntilHidden = GameActivity_isSoftwareKeyboardVisible(m_Application->GetAndroidAppInstance()->activity);
    GameActivity_hideSoftInput(m_Application->GetAndroidAppInstance()->activity, flags);
    if (waitUntilHidden)
    {
        auto result = WaitUntilKeyboardVisibility(false);
        if (!result)
            UNITY_LOG_WARNING("Timeout while waiting for keyboard to become hidden.");
    }
}

void Unity::UnitySoftKeyboard::_SetCharacterLimitImpl(uint32_t characterLimit)
{
    m_Data->character_limit = characterLimit;
    if (TruncateToLimit())
        UpdateTextInputState();
}

bool Unity::UnitySoftKeyboard::TruncateToLimit()
{
    // As per Unity documentation setting character limit to 0 means there's no limit
    if (m_Data->character_limit == 0)
        return false;

    auto bytes = SubStringBytes(m_Data->owned_string.c_str(), m_Data->owned_string.size(), m_Data->character_limit);
    if (bytes < m_Data->owned_string.size())
    {
        m_Data->owned_string.erase(bytes);
        *m_Data = m_Data->owned_string; // reassign to fix other props
        return true;
    }

    return false;
}

bool Unity::UnitySoftKeyboard::WaitUntilKeyboardVisibility(bool visible) const
{
    auto startTime = std::chrono::steady_clock::now();
    while (std::chrono::steady_clock::now() - startTime < kKeyboardVisibilitySynchronizationTimeOutMs) {
        m_Application->ProcessApplicationCommands();

        if (GameActivity_isSoftwareKeyboardVisible(m_Application->GetAndroidAppInstance()->activity) == visible)
            return true;

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return false;
}

#endif
