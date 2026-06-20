// Assign value to class function pointer, MethodNameImpl function must exist.
#define DECLARE_INSTANCE_METHOD(ClassName, ReturnType, MethodName, Parameters, CallParameters) _##MethodName##Ptr = &ClassName::_##MethodName##Impl;
#define DECLARE_METHOD(ReturnType, MethodName, Parameters) MethodName = _##MethodName##Impl;
#define DECLARE_PUBLIC_METHOD(ReturnType, MethodName, Parameters) DECLARE_METHOD(ReturnType, MethodName, Parameters)
