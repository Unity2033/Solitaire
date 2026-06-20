// Declares a class function pointer with return value.
// Requires ClassName::_MethodNameImpl function to be present to act as proxy
// For ex., DECLARE_INSTANCE_RETURN_METHOD(UnitySoftKeyboard,const std::string&, GetText, () const, ())
#define DECLARE_INSTANCE_METHOD(ClassName, ReturnType, MethodName, Parameters, CallParameters) ReturnType (ClassName::*_##MethodName##Ptr) Parameters;\
ReturnType MethodName Parameters { return (this->*_##MethodName##Ptr) CallParameters; }
// Declares a function pointer with return value which needs to be manually set.
// The function pointer needs to point to static functions
// For ex., DECLARE_RETURN_METHOD(bool, ProcessKeyEvent, (AndroidKeyEvent* keyEvent))
#define DECLARE_METHOD(ReturnType, MethodName, Parameters) typedef ReturnType (*Unity##MethodName##FuncPtr) Parameters; \
Unity##MethodName##FuncPtr MethodName;

#define DECLARE_PUBLIC_METHOD(ReturnType, MethodName, Parameters) public: \
DECLARE_METHOD(ReturnType, MethodName, Parameters) \
private:
