#ifndef JNI_TL_H
#define JNI_TL_H

#include "athor/Andriy.Bulka"
#include "license/BSD"

#include <jni.h>
#include <string>
#include <algorithm>

namespace JNI {

struct Env {
    Env(JNIEnv *env) : env(env) {}
    JNIEnv *operator -> () const  { return *this; }
    operator JNIEnv * () const { return env; }
    private: JNIEnv *env;
};

struct String : Env {
    String(const Env &env, jstring string)
        : Env(env), string(string) {}
    String(const Env &env, jchar *string, jsize length)
        : Env(env), string(env->NewString(string, length)) {}
    String(const Env &env, const char *string)
        : Env(env), string(env->NewStringUTF(string)) {}
    String(const Env &env, const std::string &string)
        : Env(env), string(env->NewStringUTF(string.c_str())) {}

    template <typename Char> unsigned length() const;

    operator std::string () const {
        const char * cstr = (*this)->GetStringUTFChars(string, 0);
        std::string result = cstr;
        (*this)->ReleaseStringUTFChars(string, cstr);
        return result;
    }

    operator jstring () const { return string; }
    private: jstring string;
};

template <> unsigned String::length<char>() const;
template <> unsigned String::length<jchar>() const;

struct Region {
    Region(jsize start, jsize length = 1) : start(start), length(length) {}
    jsize start, length;
};

template <typename T>
struct Array : Env {

    template <typename C, typename U = void> struct Cast {};
    template <typename C,
              typename U> struct Cast<C[],      U> { typedef jobjectArray Type; };
    template <typename U> struct Cast<jboolean, U> { typedef jbooleanArray Type; };
    template <typename U> struct Cast<jbyte,    U> { typedef    jbyteArray Type; };
    template <typename U> struct Cast<jchar,    U> { typedef    jcharArray Type; };
    template <typename U> struct Cast<jshort,   U> { typedef   jshortArray Type; };
    template <typename U> struct Cast<jint,     U> { typedef     jintArray Type; };
    template <typename U> struct Cast<jlong,    U> { typedef    jlongArray Type; };
    template <typename U> struct Cast<jfloat,   U> { typedef   jfloatArray Type; };
    template <typename U> struct Cast<jdouble,  U> { typedef  jdoubleArray Type; };
    template <typename U> struct Cast<jobject,  U> { typedef  jobjectArray Type; };
    template <typename U> struct Cast<jstring,  U> { typedef  jobjectArray Type; };
    typedef typename Cast<T>::Type Type;

    template <typename C, typename U = void> struct CastInput { typedef T Type;};
    template <typename C, typename U> struct CastInput<C[], U> { typedef typename Array<T>::template Cast<C>::Type Type; };

    template <typename C, typename U = void> struct CastOutput { typedef T Type; };
    template <typename C, typename U> struct CastOutput<C[], U> { typedef Array<C> Type; };

    typedef typename CastInput <T>::Type Input;
    typedef typename CastOutput<T>::Type Output;

    struct Elements;
    struct Critical { struct Elements; };
    struct Region { struct Elements; };
    struct Object { struct Element; };

    jsize length() const { return (*this)->GetArrayLength(array); }

    Array(const Env &env, Type array) : Env(env), array(array) {}
    Array(const Env &env, jsize count);
    Array(const Env &env, jsize count, jclass clazz, jobject object = 0);
    Array(const Env &env, jsize count, jstring string);

    Elements elements(bool copyBack = false);
    const Elements elements(bool copyBack = false) const;
    Elements critical(bool copyBack = false);
    const Elements critical(bool copyBack = false) const;

    typename Region::Elements operator[] (const JNI::Region &region) {
        return typename Region::Elements(*this, region);
    }
    const typename Region::Elements operator[] (const JNI::Region &region) const {
        return typename Region::Elements(*this, region);
    }

    typename Object::Element operator[] (int index) {
        return typename Object::Element(*this, index);
    }
    const typename Object::Element operator[] (int index) const {
        return typename Object::Element(*this, index);
    }

    operator Type () const { return array; }
    private: Type array;
};

template <typename T>
struct Array<T>::Object::Element : Array<T> {

    operator typename Array<T>::Output () const;
    Element &operator = (const typename Array<T>::Input &value);

    Element(const Array<T> &array, int index) : Array<T>(array), index(index) {}
    private: int index;
};

template <typename T>
struct Array<T>::Region::Elements : private Array<T>, private JNI::Region {

    typedef typename Array<T>::Input Type;
    struct Element;

    Element operator [] (jsize index) { return Element(*this, index); }
    const Element operator [] (jsize index) const { return Element(*this, index); }

    void commit() { *this = (Type *)(*this); }
    Elements &operator = (const Type *values);
    operator Type * () { return region != 0 ? region :
                         (region = new Type[JNI::Region::length], get(), region); }

    Elements(const Array<T> &array, const JNI::Region &region)
        : Array<T>(array), JNI::Region(region), region(0) {}
    ~Elements() { if (region != 0) { delete [] region; } }

    Elements(const Elements &elements)
        : Array<T>(static_cast<const Array<T> &>(elements)),
          JNI::Region(static_cast<const JNI::Region &>(elements)) {
       if (region != 0) { delete [] region; }
       region = elements.region; const_cast<Elements &>(elements).region = 0;
    }

    Elements &operator = (const Elements &elements) {
        if (this != &elements) {
            static_cast<Array<T> &>(*this) = static_cast<const Array<T> &>(elements);
            static_cast<JNI::Region &>(*this) = static_cast<const JNI::Region &>(elements);
            if (region != 0) { delete [] region; }
            region = elements.region; const_cast<Elements &>(elements).region = 0;
        } return *this;
    }

    private: void get();
    private: Type *region;
};

template <typename T>
struct Array<T>::Region::Elements::Element {

    Element(const Elements &elements, jsize index)
        : elements(const_cast<Elements &>(elements)), index(index) {}

    operator typename Array<T>::Output () const {
        return ((typename Elements::Type *)elements)[index];
    }
    Element &operator = (const typename Array<T>::Input &value) {
        return ((typename Elements::Type *)elements)[index] = value, *this;
    }

    private: Elements &elements;
    private: jsize index;
};

//template <typename T>
//struct Array<T>::Elements : protected Array<T>, protected Region {

//    template <typename C, typename U = void> struct CastInput { typedef T Type;};
//    template <typename C, typename U> struct CastInput<C[], U> { typedef typename Array<T>::template Cast<C>::Type Type; };

//    template <typename C, typename U = void> struct CastOutput { typedef T Type; };
//    template <typename C, typename U> struct CastOutput<C[], U> { typedef Array<C> Type; };

//    typedef typename CastInput <T>::Type Input;
//    typedef typename CastOutput<T>::Type Output;

//    Elements(const Array<T> &array, const Region &region)
//        : Array<T>(array), Region(region), elements(0), reference(true) {}

//    Element operator[] (int index) { return Element(*this, start + index); }
//    const Element operator[] (int index) const { return Element(*this, start + index); }

//    Elements &operator = (const Input *values) {
//        for (jsize i = 0; i < Region::length; ++i) {
//            (*this)[Region::start + i] = values[i];
//        } return *this;
//    }

//    Elements(const Elements &elements, bool reference = false) :
//        Array<T>(elements), Region(elements),
//        elements(elements.elements), reference(reference) {
//        if (!reference) { const_cast<Elements &>(elements).reference = true; }
//    }
//    Elements &operator = (const Elements &elements) {
//        static_cast<Array<T> &>(*this) = elements;
//        static_cast<Region &>(*this) = elements;
//        elements = elements.elements;
//        reference = elements.reference; elements.reference = true;
//        return *this;
//    }
//    ~Elements() {
//        if (!reference && elements != 0) { release(); }
//    }
//    operator T *() const {
//        return elements != 0 ? elements : (reference = false, init(), elements);
//    }
//    private: void init();
//    private: void release() {}
//    private: T *elements;
//    private: bool reference;
//};

//template <typename T>
//struct Array<T>::Element : protected Elements {

//    Element(const Elements &elements, int index) : Elements(elements, true), index(index) {}

//    operator typename Elements::Output () const { return ((T *)(*this))[index]; }
//    Element &operator = (const typename Elements::Input &value) {
//        return ((static_cast<Array<T> &>(*this))[Region(index)] = &value), *this;
//    }

//    private: int index;
//};

//// type[] ...

struct Class : Env {

    struct Static {
        template <typename Type> struct Field;
        template <typename ReturnType> struct Method;
    };  template <typename ReturnType> struct Method; struct Constructor;

    static std::string signature(std::string name) {
        return std::replace(name.begin(), name.end(), '.', '/'), name;
    }

    Class(const Env &env, jclass clazz)
        : Env(env), clazz(clazz) {}
    Class(const Env &env, const char *clazz)
        : Env(env), clazz(env->FindClass(signature(clazz).c_str())) {}
    Class(const Env &env, const std::string &clazz)
        : Env(env), clazz(env->FindClass(signature(clazz).c_str())) {}
    Class(const Env &env, const char *clazz,
          jobject loader, const jbyte *buffer, jsize bufferLength)
        : Env(env), clazz(env->DefineClass(clazz,
                    loader, buffer, bufferLength)) {}
    Class(const Env &env, const std::string &clazz,
          jobject loader, const jbyte *buffer, jsize bufferLength)
        : Env(env), clazz(env->DefineClass(clazz.c_str(),
                    loader, buffer, bufferLength)) {}

    String string(const char *s) { return String(*this, s); }
    String string(const std::string &s) { return String(*this, s); }
    String string(const jstring &s) { return String(*this, s); }

    Class super() const { return Class(*this, (*this)->GetSuperclass(clazz)); }
    bool cast(jclass clazz) const {
        return (*this)->IsAssignableFrom(*this, clazz);
    }
    jfieldID fieldIDStatic(const char *name, const char *sig) const {
        return (*this)->GetStaticFieldID(*this, name, sig);
    }
    jfieldID fieldID(const char *name, const char *sig) const {
        return (*this)->GetFieldID(*this, name, sig);
    }
    jmethodID methodIDStatic(const char *name, const char *sig) const {
        return (*this)->GetStaticMethodID(*this, name, sig);
    }
    jmethodID methodID(const char *name, const char *sig) const {
        return (*this)->GetMethodID(*this, name, sig);
    }

//    template <typename T>
//    typename Static::Field<T>::ID operator() (const typename Static::Field<T> &field) const {
//        return typename Static::Field<T>::ID(*this, field);
//    }

    template <typename Constructor>
    typename Constructor::Result newObject(const Constructor &constructor, ...);

    template <typename ID>
    typename ID::Reference
    operator [] (const ID &id) { return at(id); }

    template <typename ID>
    const typename ID::Reference
    operator [] (const ID &id) const { return at(id); }

    template <typename Type>
    typename Static::Field<Type>::Reference
    operator [] (const Static::Field<Type> &field) {
        return typename Static::Field<Type>::Reference
                (*this, typename Static::Field<Type>::ID(*this, field));
    }
    template <typename Type>
    const typename Static::Field<Type>::Reference
    operator [] (const Static::Field<Type> &field) const {
        return typename Static::Field<Type>::Reference
                (*this, typename Static::Field<Type>::ID(*this, field));
    }
    template <typename ReturnType>
    typename Static::Method<ReturnType>::Reference
    operator [] (const Static::Method<ReturnType> &method) {
        return typename Static::Method<ReturnType>::Reference
                (*this, typename Static::Method<ReturnType>::ID(*this, method));
    }
    template <typename ReturnType>
    const typename Static::Method<ReturnType>::Reference
    operator [] (const Static::Method<ReturnType> &method) const {
        return typename Static::Method<ReturnType>::Reference
                (*this, typename Static::Method<ReturnType>::ID(*this, method));
    }

    operator jclass () const { return clazz; }
    private: jclass clazz;

    // SFINAE workaround due to poor C++ compiler function template specialization logic
    private: template <typename ID>
    typename Static::Field<typename ID::Type>::Reference
    at(const ID &id, const typename ID::IsStaticFieldID & = typename ID::IsStaticFieldID()) {
        return typename Static::Field<typename ID::Type>::Reference(*this, id);
    }

    private: template <typename ID>
    const typename Static::Field<typename ID::Type>::Reference
    at(const ID &id, const typename ID::IsStaticFieldID & = typename ID::IsStaticFieldID()) const {
        return typename Static::Field<typename ID::Type>::Reference(*this, id);
    }

    private: template <typename ID>
    typename Static::Method<typename ID::ReturnType>::Reference
    at(const ID &id, const typename ID::IsStaticMethodID & = typename ID::IsStaticMethodID()) {
        return typename Static::Method<typename ID::ReturnType>::Reference(*this, id);
    }

    private: template <typename ID>
    const typename Static::Method<typename ID::ReturnType>::Reference
    at(const ID &id, const typename ID::IsStaticMethodID & = typename ID::IsStaticMethodID()) const {
        return typename Static::Method<typename ID::ReturnType>::Reference(*this, id);
    }
};

template <typename Type>
struct JType {
    struct Signature;
};

template <typename Type>
struct JType<Type>::Signature {
    operator const char * () const { return value; }
    private: static const char *value;
};

template <>
struct JType<jobject>::Signature {
    Signature(const char *clazz) : clazz(std::string("L") + Class::signature(clazz) + ";") {}
    operator const char * () const { return clazz.c_str(); }
    private: std::string clazz;
};

template <typename Type>
struct JType<Type[]> { struct Signature {
    operator const char * () const { return value; }
    private: static const char *value;
}; };

template <>
struct JType<jobject[]>::Signature {
    Signature(const char *clazz)
        : clazz(std::string("[") + (const char *)JType<jobject>::Signature(clazz)) {}
    operator const char * () const { return clazz.c_str(); }
    private: std::string clazz;
};

struct Nop { Nop() {} };

template <typename T0 = Nop, typename T1 = Nop, typename T2 = Nop,
          typename T3 = Nop, typename T4 = Nop, typename T5 = Nop,
          typename T6 = Nop, typename T7 = Nop, typename T8 = Nop,
          typename T9 = Nop > // extendable ...
struct Args {

    Args(const char *prepared = 0, ...) {
        va_list va_list;
        va_start(va_list, prepared);
        VaList vl(prepared, va_list);
        va_end(va_list);
        args += Arg<T0>(vl); args += Arg<T1>(vl); args += Arg<T2>(vl);
        args += Arg<T3>(vl); args += Arg<T4>(vl); args += Arg<T5>(vl);
        args += Arg<T6>(vl); args += Arg<T7>(vl); args += Arg<T8>(vl);
        args += Arg<T9>(vl); // extendable ...
    }

    operator const char * () const { return args.c_str(); }
    operator const std::string & () const { return args; }
    private: std::string args;

    struct VaList {
        VaList(const char *prepared, va_list vl)
            : preparedIsInvalid(false), prepared(prepared) { va_copy(this->vl, vl); }
        ~VaList()  { va_end(vl); }

        const char * next() { return preparedIsInvalid ?
            (va_arg(vl, const char *)) :
            (preparedIsInvalid = true, prepared);
        }
        bool preparedIsInvalid; const char *prepared; va_list vl;
    };

    template <typename T, typename U = void> struct Arg;

    template <typename T, typename U>
    struct Arg {
        operator const char * () const {
            return typename JType<T>::Signature();
        } Arg(VaList &vl) : vl(vl) {} private: VaList &vl;
    };
    template <typename U>
    struct Arg<jobject, U> {
        operator const char * () const {
            return typename JType<jobject>::Signature(vl.next());
        } Arg(VaList &vl) : vl(vl) {} private: VaList &vl;
    };
    template <typename U>
    struct Arg<jobject[], U> {
        operator const char * () const {
            return typename JType<jobject[]>::Signature(vl.next());
        } Arg(VaList &vl) : vl(vl) {} private: VaList &vl;
    };
};

template <typename T>
struct Class::Static::Field {
    template <typename C> struct Cast { typedef C Type; };
    template <typename C> struct Cast<C[]> { typedef Array<C> Type; };
    typedef typename Cast<T>::Type Accessor;

    typedef typename JType<T>::Signature Signature;
    struct Reference;
    struct ID;

    Field(const char *name, const Signature &signature = Signature())
        : name(name), signature(signature) {}
    operator const char * () const { return name; }
    const char *sig() const { return signature; }
    private: const char *name;
    private: Signature signature;
};

template <typename T>
struct Class::Static::Field<T>::ID {

    ID(const Class &clazz, const Field<T> &field) : id(clazz.fieldIDStatic(field, field.sig())) {}
    ID(jfieldID id) : id(id) {}

    operator jfieldID () const { return id; }
    private: jfieldID id;

    // Stuff for SFINAE workaround
    public: typedef T Type; struct IsStaticFieldID {};
    public: typedef typename Field<T>::Reference Reference;
};

template <typename T>
struct Class::Static::Field<T>::Reference : private Class, private ID {

    typedef typename Field<T>::Accessor Type;

    Reference(const Class &clazz, const ID &id) : Class(clazz), ID(id) {}

    operator Type () const;
    Reference &operator = (const Type &value);
};

template <typename Type>
struct Class::Static::Method {

    typedef typename Class::Static::Field<Type>::Signature Signature;
    struct Reference;
    struct ID;

    Method(const char *name, const Signature &signature = Signature())
        :  name(name), signature(std::string("()") + (const char *)signature) {}

    template <typename T0, typename T1, typename T2, typename T3,
              typename T4, typename T5, typename T6, typename T7,
              typename T8, typename T9 > // extendable ...
    Method(const char *name,
           const Args<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> &args,
           const Signature &signature = Signature())
        :  name(name), signature(std::string("(") +
                                 (const std::string &)args + ")" +
                                 (const char *)signature) {}

    operator const char * () const { return name; }
    const char *sig() const { return signature.c_str(); }
    private: const char *name;
    private: std::string signature;
};

template <typename T>
struct Class::Static::Method<T>::ID {

    ID(const Class &clazz, const Method<T> &method) : id(clazz.methodIDStatic(method, method.sig())) {}
    ID(jmethodID id) : id(id) {}

    operator jmethodID () const { return id; }
    private: jmethodID id;

    // Stuff for SFINAE workaround
    public: typedef T ReturnType; struct IsStaticMethodID {};
    public: typedef typename Method<T>::Reference Reference;
};

template <typename T>
struct Class::Static::Method<T>::Reference : private Class, private ID {

    typedef typename Class::Static::Field<T>::Accessor Type;

    Reference(const Class &clazz, const ID &id) : Class(clazz), ID(id) {}

    Type call(va_list) const;
    Type call(const Nop & nop, ...) const {
        va_list vl; va_start(vl, nop); Type ret = call(vl); va_end(vl);
        return ret;
    }

    Type operator () () const
    { return call(Nop()); }
    template <typename T0>
    Type operator () (const T0 &v0) const
    { return call(Nop(), v0); }
    template <typename T0, typename T1>
    Type operator () (const T0 &v0, const T1 &v1) const
    { return call(Nop(), v0, v1); }
    template <typename T0, typename T1, typename T2>
    Type operator () (const T0 &v0, const T1 &v1, const T2 &v2) const
    { return call(Nop(), v0, v1, v2); }
    template <typename T0, typename T1, typename T2, typename T3>
    Type operator () (const T0 &v0, const T1 &v1, const T2 &v2,
                      const T3 &v3) const
    { return call(Nop(), v0, v1, v2, v3); }
    template <typename T0, typename T1, typename T2, typename T3,
              typename T4>
    Type operator () (const T0 &v0, const T1 &v1, const T2 &v2,
                      const T3 &v3, const T4 &v4) const
    { return call(Nop(), v0, v1, v2, v3, v4); }
    template <typename T0, typename T1, typename T2, typename T3,
              typename T4, typename T5>
    Type operator () (const T0 &v0, const T1 &v1, const T2 &v2,
                      const T3 &v3, const T4 &v4, const T5 &v5) const
    { return call(Nop(), v0, v1, v2, v3, v4, v5); }
    template <typename T0, typename T1, typename T2, typename T3,
              typename T4, typename T5, typename T6>
    Type operator () (const T0 &v0, const T1 &v1, const T2 &v2,
                      const T3 &v3, const T4 &v4, const T5 &v5,
                      const T6 &v6) const
    { return call(Nop(), v0, v1, v2, v3, v4, v5, v6); }
    template <typename T0, typename T1, typename T2, typename T3,
              typename T4, typename T5, typename T6, typename T7>
    Type operator () (const T0 &v0, const T1 &v1, const T2 &v2,
                      const T3 &v3, const T4 &v4, const T5 &v5,
                      const T6 &v6, const T7 &v7) const
    { return call(Nop(), v0, v1, v2, v3, v4, v5, v6, v7); }
    template <typename T0, typename T1, typename T2, typename T3,
              typename T4, typename T5, typename T6, typename T7,
              typename T8>
    Type operator () (const T0 &v0, const T1 &v1, const T2 &v2,
                      const T3 &v3, const T4 &v4, const T5 &v5,
                      const T6 &v6, const T7 &v7, const T8 &v8) const
    { return call(Nop(), v0, v1, v2, v3, v4, v5, v6, v7, v8); }
    template <typename T0, typename T1, typename T2, typename T3,
              typename T4, typename T5, typename T6, typename T7,
              typename T8, typename T9>
    Type operator () (const T0 &v0, const T1 &v1, const T2 &v2,
                      const T3 &v3, const T4 &v4, const T5 &v5,
                      const T6 &v6, const T7 &v7, const T8 &v8,
                      const T9 &v9) const
    { return call(Nop(), v0, v1, v2, v3, v4, v5, v6, v7, v8, v9); }
};

struct Object : Class {
    struct Monitor;
    template <typename Type> struct Field;
    template <typename ReturnType> struct Method;

    Object(const Env &env, jobject object)
        : Class(env, env->GetObjectClass(object)), object(object) {}
    Object(const Class &clazz, jobject object)
        : Class(clazz), object(object) {}

    bool isInstanceOf(jclass clazz) const { return (*this)->IsInstanceOf(*this, clazz); }
    bool operator == (jobject object) { return (*this)->IsSameObject(*this, object); }
    jobjectRefType refType() const { return (*this)->GetObjectRefType(*this); }

    template <typename ID>
    typename ID::Reference
    operator [] (const ID &id) { return at(id); }
    template <typename ID>
    const typename ID::Reference
    operator [] (const ID &id) const { return at(id); }

    template <typename Type>
    typename Class::Static::Field<Type>::Reference
    operator [] (const Class::Static::Field<Type> &field) {
        return Class::operator [] (field);
    }
    template <typename Type>
    const typename Class::Static::Field<Type>::Reference
    operator [] (const Class::Static::Field<Type> &field) const {
        return Class::operator [] (field);
    }
    template <typename ReturnType>
    typename Class::Static::Method<ReturnType>::Reference
    operator [] (const Class::Static::Method<ReturnType> &method) {
        return Class::operator [] (method);
    }
    template <typename ReturnType>
    const typename Class::Static::Method<ReturnType>::Reference
    operator [] (const Class::Static::Method<ReturnType> &method) const {
        return Class::operator [] (method);
    }

template <typename ReturnType>
typename Class::Method<ReturnType>::Reference
operator [] (const Class::Method<ReturnType> &method) {
    return typename Class::Method<ReturnType>::Reference(*this, typename Class::Method<ReturnType>::ID(*this, method));
}
template <typename ReturnType>
const typename Class::Method<ReturnType>::Reference
operator [] (const Class::Method<ReturnType> &method) const {
    return typename Class::Method<ReturnType>::Reference(*this, Class::Method<ReturnType>::ID(*this, method));
}

    template <typename Type>
    typename Field<Type>::Reference
    operator [] (const Field<Type> &field) {
        return typename Field<Type>::Reference(*this, typename Field<Type>::ID(*this, field));
    }
    template <typename Type>
    const typename Field<Type>::Reference
    operator [] (const Field<Type> &field) const {
        return typename Field<Type>::Reference(*this, typename Field<Type>::ID(*this, field));
    }
    template <typename ReturnType>
    typename Method<ReturnType>::Reference
    operator [] (const Method<ReturnType> &method) {
        return typename Method<ReturnType>::Reference(*this, typename Method<ReturnType>::ID(*this, method));
    }
    template <typename ReturnType>
    const typename Method<ReturnType>::Reference
    operator [] (const Method<ReturnType> &method) const {
        return typename Method<ReturnType>::Reference(*this, Method<ReturnType>::ID(*this, method));
    }

    operator jobject () const { return object; }
    private: jobject object;

    // SFINAE workaround due to poor C++ compiler function template specialization logic
    private: template <typename ID>
    typename Class::Static::Field<typename ID::Type>::Reference
    at(const ID &id, const typename ID::IsStaticFieldID & = typename ID::IsStaticFieldID()) {
        return Class::operator [] (id);
    }
    private: template <typename ID>
    const typename Class::Static::Field<typename ID::Type>::Reference
    at(const ID &id, const typename ID::IsStaticFieldID & = typename ID::IsStaticFieldID()) const {
        return Class::operator [] (id);
    }
    private: template <typename ID>
    typename Class::Static::Method<typename ID::ReturnType>::Reference
    at(const ID &id, const typename ID::IsStaticMethodID & = typename ID::IsStaticMethodID()) {
        return Class::operator [] (id);
    }
    private: template <typename ID>
    const typename Class::Static::Method<typename ID::ReturnType>::Reference
    at(const ID &id, const typename ID::IsStaticMethodID & = typename ID::IsStaticMethodID()) const {
        return Class::operator [] (id);
    }

    private: template <typename ID>
    typename Field<typename ID::Type>::Reference
    at(const ID &id, const typename ID::IsObjectFieldID & = typename ID::IsObjectFieldID()) {
        return typename Field<typename ID::Type>::Reference(*this, id);
    }
    private: template <typename ID>
    const typename Field<typename ID::Type>::Reference
    at(const ID &id, const typename ID::IsObjectFieldID & = typename ID::IsObjectFieldID()) const {
        return typename Field<typename ID::Type>::Reference(*this, id);
    }

    private: template <typename ID>
    typename Class::Method<typename ID::ReturnType>::Reference
    at(const ID &id, const typename ID::IsClassMethodID & = typename ID::IsClassMethodID()) {
        return typename Class::Method<typename ID::ReturnType>::Reference(*this, id);
    }
    private: template <typename ID>
    const typename Class::Method<typename ID::ReturnType>::Reference
    at(const ID &id, const typename ID::IsClassMethodID & = typename ID::IsClassMethodID()) const {
        return typename Class::Method<typename ID::ReturnType>::Reference(*this, id);
    }

    private: template <typename ID>
    typename Method<typename ID::ReturnType>::Reference
    at(const ID &id, const typename ID::IsObjectMethodID & = typename ID::IsObjectMethodID()) {
        return typename Method<typename ID::ReturnType>::Reference(*this, id);
    }
    private: template <typename ID>
    const typename Method<typename ID::ReturnType>::Reference
    at(const ID &id, const typename ID::IsObjectMethodID & = typename ID::IsObjectMethodID()) const {
        return typename Method<typename ID::ReturnType>::Reference(*this, id);
    }
};

template <typename Type>
struct Object::Field : private Class::Static::Field<Type> {

    typedef typename Class::Static::Field<Type>::Signature Signature;
    struct Reference;
    struct ID;

    operator const char * () const { return Class::Static::Field<Type>::operator const char * (); }
    const char *sig() const { return Class::Static::Field<Type>::sig(); }

    Field(const char *name, const Signature &signature = Signature())
        : Class::Static::Field<Type>(name, signature) {}
};

template <typename T>
struct Object::Field<T>::ID {

    ID(const Class &clazz, const Field<T> &field) : id(clazz.fieldID(field, field.sig())) {}
    ID(jfieldID id) : id(id) {}

    operator jfieldID () const { return id; }
    private: jfieldID id;

    // Stuff for SFINAE workaround
    public: typedef typename Class::Static::Field<T>::ID::Type Type;
    public: struct IsObjectFieldID {}; typedef typename Field<T>::Reference Reference;
};

template <typename T>
struct Object::Field<T>::Reference : private Object, private ID {

    typedef typename Field<T>::Accessor Type;

    Reference(const Object &object, const ID &id) : Object(object), ID(id) {}

    operator Type () const;
    Reference &operator = (const Type &value);
};

template <typename Type>
struct Class::Method : private Class::Static::Method<Type> {

    typedef typename Class::Static::Method<Type>::Signature Signature;
    struct Reference;
    struct ID;

    Method(const char *name, const Signature &signature = Signature())
        : Class::Static::Method<Type>(name, signature) {}

    template <typename T0, typename T1, typename T2, typename T3,
              typename T4, typename T5, typename T6, typename T7,
              typename T8, typename T9 > // extendable ...
    Method(const char *name,
           const Args<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> &args,
           const Signature &signature = Signature())
        : Class::Static::Method<Type>(name, args, signature) {}
};

template <typename T>
struct Class::Method<T>::ID {

    ID(const Class &clazz, const Method<T> &method) : id(clazz.methodID(method, method.sig())) {}
    ID(jmethodID id) : id(id) {}

    operator jmethodID () const { return id; }
    private: jmethodID id;

    // Stuff for SFINAE workaround
    public: typedef typename Class::Static::Method<T>::ID::ReturnType ReturnType;
    public: struct IsClassMethodID{}; typedef typename Method<T>::Reference Reference;
};

template <typename T>
struct Class::Method<T>::Reference : private Object, private ID {

    typedef typename Class::Static::Method<T>::Reference::Type Type;

    Reference(const Object &object, const ID &id) : Object(object), ID(id) {}

    Type call(va_list) const;
    Type call(const Nop & nop, ...) const {
        va_list vl; va_start(vl, nop); Type ret = call(vl); va_end(vl);
        return ret;
    }

    Type operator () () const
    { return call(Nop()); }
    template <typename T0>
    Type operator () (const T0 &v0) const
    { return call(Nop(), v0); }
    template <typename T0, typename T1>
    Type operator () (const T0 &v0, const T1 &v1) const
    { return call(Nop(), v0, v1); }
    template <typename T0, typename T1, typename T2>
    Type operator () (const T0 &v0, const T1 &v1, const T2 &v2) const
    { return call(Nop(), v0, v1, v2); }
    template <typename T0, typename T1, typename T2, typename T3>
    Type operator () (const T0 &v0, const T1 &v1, const T2 &v2,
                      const T3 &v3) const
    { return call(Nop(), v0, v1, v2, v3); }
    template <typename T0, typename T1, typename T2, typename T3,
              typename T4>
    Type operator () (const T0 &v0, const T1 &v1, const T2 &v2,
                      const T3 &v3, const T4 &v4) const
    { return call(Nop(), v0, v1, v2, v3, v4); }
    template <typename T0, typename T1, typename T2, typename T3,
              typename T4, typename T5>
    Type operator () (const T0 &v0, const T1 &v1, const T2 &v2,
                      const T3 &v3, const T4 &v4, const T5 &v5) const
    { return call(Nop(), v0, v1, v2, v3, v4, v5); }
    template <typename T0, typename T1, typename T2, typename T3,
              typename T4, typename T5, typename T6>
    Type operator () (const T0 &v0, const T1 &v1, const T2 &v2,
                      const T3 &v3, const T4 &v4, const T5 &v5,
                      const T6 &v6) const
    { return call(Nop(), v0, v1, v2, v3, v4, v5, v6); }
    template <typename T0, typename T1, typename T2, typename T3,
              typename T4, typename T5, typename T6, typename T7>
    Type operator () (const T0 &v0, const T1 &v1, const T2 &v2,
                      const T3 &v3, const T4 &v4, const T5 &v5,
                      const T6 &v6, const T7 &v7) const
    { return call(Nop(), v0, v1, v2, v3, v4, v5, v6, v7); }
    template <typename T0, typename T1, typename T2, typename T3,
              typename T4, typename T5, typename T6, typename T7,
              typename T8>
    Type operator () (const T0 &v0, const T1 &v1, const T2 &v2,
                      const T3 &v3, const T4 &v4, const T5 &v5,
                      const T6 &v6, const T7 &v7, const T8 &v8) const
    { return call(Nop(), v0, v1, v2, v3, v4, v5, v6, v7, v8); }
    template <typename T0, typename T1, typename T2, typename T3,
              typename T4, typename T5, typename T6, typename T7,
              typename T8, typename T9>
    Type operator () (const T0 &v0, const T1 &v1, const T2 &v2,
                      const T3 &v3, const T4 &v4, const T5 &v5,
                      const T6 &v6, const T7 &v7, const T8 &v8,
                      const T9 &v9) const
    { return call(Nop(), v0, v1, v2, v3, v4, v5, v6, v7, v8, v9); }
    // extendable ...
};

template <typename Type>
struct Object::Method : private Class::Static::Method<Type> {

    typedef typename Class::Static::Method<Type>::Signature Signature;
    struct Reference;
    struct ID;

    Method(const char *name, const Signature &signature = Signature())
        : Class::Static::Method<Type>(name, signature) {}

    template <typename T0, typename T1, typename T2, typename T3,
              typename T4, typename T5, typename T6, typename T7,
              typename T8, typename T9 > // extendable ...
    Method(const char *name,
           const Args<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> &args,
           const Signature &signature = Signature())
        : Class::Static::Method<Type>(name, args, signature) {}
};

template <typename T>
struct Object::Method<T>::ID {

    ID(const Class &clazz, const Method<T> &method) : id(clazz.methodID(method, method.sig())) {}
    ID(jmethodID id) : id(id) {}

    operator jmethodID () const { return id; }
    private: jmethodID id;

    // Stuff for SFINAE workaround
    public: typedef typename Class::Static::Method<T>::ID::ReturnType ReturnType;
    public: struct IsObjectMethodID{}; typedef typename Method<T>::Reference Reference;
};

template <typename T>
struct Object::Method<T>::Reference : private Object, private ID {

    typedef typename Class::Static::Method<T>::Reference::Type Type;

    Reference(const Object &object, const ID &id) : Object(object), ID(id) {}

    Type call(va_list) const;
    Type call(const Nop & nop, ...) const {
        va_list vl; va_start(vl, nop); Type ret = call(vl); va_end(vl);
        return ret;
    }

    Type operator () () const
    { return call(Nop()); }
    template <typename T0>
    Type operator () (const T0 &v0) const
    { return call(Nop(), v0); }
    template <typename T0, typename T1>
    Type operator () (const T0 &v0, const T1 &v1) const
    { return call(Nop(), v0, v1); }
    template <typename T0, typename T1, typename T2>
    Type operator () (const T0 &v0, const T1 &v1, const T2 &v2) const
    { return call(Nop(), v0, v1, v2); }
    template <typename T0, typename T1, typename T2, typename T3>
    Type operator () (const T0 &v0, const T1 &v1, const T2 &v2,
                      const T3 &v3) const
    { return call(Nop(), v0, v1, v2, v3); }
    template <typename T0, typename T1, typename T2, typename T3,
              typename T4>
    Type operator () (const T0 &v0, const T1 &v1, const T2 &v2,
                      const T3 &v3, const T4 &v4) const
    { return call(Nop(), v0, v1, v2, v3, v4); }
    template <typename T0, typename T1, typename T2, typename T3,
              typename T4, typename T5>
    Type operator () (const T0 &v0, const T1 &v1, const T2 &v2,
                      const T3 &v3, const T4 &v4, const T5 &v5) const
    { return call(Nop(), v0, v1, v2, v3, v4, v5); }
    template <typename T0, typename T1, typename T2, typename T3,
              typename T4, typename T5, typename T6>
    Type operator () (const T0 &v0, const T1 &v1, const T2 &v2,
                      const T3 &v3, const T4 &v4, const T5 &v5,
                      const T6 &v6) const
    { return call(Nop(), v0, v1, v2, v3, v4, v5, v6); }
    template <typename T0, typename T1, typename T2, typename T3,
              typename T4, typename T5, typename T6, typename T7>
    Type operator () (const T0 &v0, const T1 &v1, const T2 &v2,
                      const T3 &v3, const T4 &v4, const T5 &v5,
                      const T6 &v6, const T7 &v7) const
    { return call(Nop(), v0, v1, v2, v3, v4, v5, v6, v7); }
    template <typename T0, typename T1, typename T2, typename T3,
              typename T4, typename T5, typename T6, typename T7,
              typename T8>
    Type operator () (const T0 &v0, const T1 &v1, const T2 &v2,
                      const T3 &v3, const T4 &v4, const T5 &v5,
                      const T6 &v6, const T7 &v7, const T8 &v8) const
    { return call(Nop(), v0, v1, v2, v3, v4, v5, v6, v7, v8); }
    template <typename T0, typename T1, typename T2, typename T3,
              typename T4, typename T5, typename T6, typename T7,
              typename T8, typename T9>
    Type operator () (const T0 &v0, const T1 &v1, const T2 &v2,
                      const T3 &v3, const T4 &v4, const T5 &v5,
                      const T6 &v6, const T7 &v7, const T8 &v8,
                      const T9 &v9) const
    { return call(Nop(), v0, v1, v2, v3, v4, v5, v6, v7, v8, v9); }
    // extendable ...
};

struct Class::Constructor : Object::Method<void> {
    struct ID;
    typedef Object Result;

    Constructor() : Object::Method<void>("<init>") {}
    template <typename T0, typename T1, typename T2, typename T3,
              typename T4, typename T5, typename T6, typename T7,
              typename T8, typename T9 > // extendable ...
    Constructor(const Args<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> &args = Args<>())
        : Object::Method<void>("<init>", args) {}
};

struct Class::Constructor::ID : Object::Method<void>::ID {
    typedef Object Result;

    ID(const Class &clazz, const Constructor &constructor = Constructor())
        : Object::Method<void>::ID(clazz, constructor) {}
    ID(jmethodID id) : Object::Method<void>::ID(id) {}
};

struct Object::Monitor : Env {
    Monitor(const Env &env, jobject object)
        : Env(env), object(object) { (*this)->MonitorEnter(object); }
    Monitor(const Object &object)
        : Env(object), object(object) { (*this)->MonitorEnter(object); }

    ~Monitor() { (*this)->MonitorEnter(object); }
    private: jobject object;
};

} // JNI

#endif // JNI_TL_H
