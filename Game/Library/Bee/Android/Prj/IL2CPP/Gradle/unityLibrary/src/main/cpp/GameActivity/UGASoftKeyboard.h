#pragma once

#include <string>
#include <jni.h>
#include "UGATypes.h"

namespace Unity
{
    struct UnitySoftKeyboardData;
    class UnityApplication;

    // Note: Don't change the layout of this class since it will become out of sync with Unity Runtime
    //       Changing the layout here, will require rebuilding the Unity runtime
    class UnitySoftKeyboard
    {
    public:
        typedef void (*TextInputFunc)();

        enum class KeyboardType
        {
            Default = 0,
            ASCIICapable = 1,
            NumbersAndPunctuation = 2,
            URL = 3,
            NumberPad = 4,
            PhonePad = 5,
            NamePhonePad = 6,
            EmailAddress = 7,
            // Reserved = 8,
            Social = 9,
            Search = 10,
            DecimalPad = 11,
            OneTimeCode = 12
        };

        struct ShowParams
        {
            TextInputFunc textInputFunc;
            std::string initialText;
            KeyboardType type;
            uint32_t characterLimit;
            bool correction;
            bool multiline;
            bool secure;
        };

        UnitySoftKeyboardData* m_Data;
        UnityApplication* m_Application;
        TextInputFunc m_OnTextInput;
    public:
        UnitySoftKeyboard(UnityApplication* application);
        ~UnitySoftKeyboard();

    public:
#include "MacroHeaderBegin.h"
    #include "UGASoftKeyboardCallbacks.h"
#include "MacroEnd.h"

        void OnTextInput();
    private:
        static bool InputWithinLimits(const char* text, uint32_t length, uint32_t limit);
        static uint32_t SubStringBytes(const char* text, uint32_t length, uint32_t chars);
        void _SetTextImpl(const std::string& text);
        void UpdateTextInputState();
        void RestartImeInput();
        const std::string& _GetTextImpl() const;
        void _SetTextSelectionImpl(const Range& textSelection);
        Range _GetTextSelectionImpl() const;
        void _ShowImpl(const ShowParams* showParams = NULL, uint32_t sizeOfShowParams = 0);
        void _HideImpl();
        void _SetCharacterLimitImpl(uint32_t characterLimit);
        bool TruncateToLimit();
        bool WaitUntilKeyboardVisibility(bool visible) const;
    };
}
