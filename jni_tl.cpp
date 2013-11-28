#include "athor/Andriy.Bulka"
#include "license/BSD"

#include "jni_tl.h"

namespace JNI {

template <> unsigned String::length<char>() const {
    return (*this)->GetStringUTFLength(string);
}
template <> unsigned String::length<jchar>() const {
    return (*this)->GetStringLength(string);
}

template <> const char * Class::Field<Nop>::Signature::value = "";
template <> const char * Class::Field<void>::Signature::value = "V";

template <> const char * Class::Field<jboolean  >::Signature::value = "Z";
template <> const char * Class::Field<jbyte     >::Signature::value = "B";
template <> const char * Class::Field<jchar     >::Signature::value = "C";
template <> const char * Class::Field<jshort    >::Signature::value = "S";
template <> const char * Class::Field<jint      >::Signature::value = "I";
template <> const char * Class::Field<jlong     >::Signature::value = "J";
template <> const char * Class::Field<jfloat    >::Signature::value = "F";
template <> const char * Class::Field<jdouble   >::Signature::value = "D";

template <> const char * Class::Field<jboolean[]>::Signature::value = "[Z";
template <> const char * Class::Field<jbyte   []>::Signature::value = "[B";
template <> const char * Class::Field<jchar   []>::Signature::value = "[C";
template <> const char * Class::Field<jshort  []>::Signature::value = "[S";
template <> const char * Class::Field<jint    []>::Signature::value = "[I";
template <> const char * Class::Field<jlong   []>::Signature::value = "[J";
template <> const char * Class::Field<jfloat  []>::Signature::value = "[F";
template <> const char * Class::Field<jdouble []>::Signature::value = "[D";

template <>
Class::Field<jint>::Reference::operator jint () const {
    return (*this)->GetStaticIntField(*this, *this);
}

template <>
Class::Field<jint>::Reference &
Class::Field<jint>::Reference::operator = (const jint &value) {
    return (*this)->SetStaticIntField(*this, *this, value), *this;
}

template <>
jint Class::Method<jint>::Reference::call(va_list vl) const {
    return (*this)->CallStaticIntMethodV(*this, *this, vl);
}

template <>
Class::Field<jobject>::Reference::operator jobject () const {
    return (*this)->GetStaticObjectField(*this, *this);
}

template <>
Class::Field<jobject>::Reference &
Class::Field<jobject>::Reference::operator = (const jobject &value) {
    return (*this)->SetStaticObjectField(*this, *this, value), *this;

}

template <>
jobject Class::Method<jobject>::Reference::call(va_list vl) const {
    return (*this)->CallStaticObjectMethodV(*this, *this, vl);
}
// ...



template <>
Class::Field<jint[]>::Reference::operator Array<jint> () const {
    return Array<jint>(*this, (jintArray)(*this)->GetStaticObjectField(*this, *this));
}

template <>
Class::Field<jint[]>::Reference &
Class::Field<jint[]>::Reference::operator = (const Array<jint> &value) {
    return (*this)->SetStaticObjectField(*this, *this, value), *this;
}

template <>
Class::Field<jobject[]>::Reference::operator Array<jobject> () const {
    return Array<jobject>(*this, (jobjectArray) (*this)->GetStaticObjectField(*this, *this));
}

template <>
Class::Field<jobject[]>::Reference &
Class::Field<jobject[]>::Reference::operator = (const Array<jobject> &value) {
    return (*this)->SetStaticObjectField(*this, *this, value), *this;
}
// ...


template <>
Object::Field<jint>::Reference::operator jint () const {
    return (*this)->GetIntField(*this, *this);
}

template <>
Object::Field<jint>::Reference &
Object::Field<jint>::Reference::operator = (const jint &value) {
    return (*this)->SetIntField(*this, *this, value), *this;
}

template <>
jint Object::Method<jint>::Reference::call(va_list vl) const {
    return (*this)->CallIntMethodV(*this, *this, vl);
}

template <>
Object::Field<jobject>::Reference::operator jobject () const {
    return (*this)->GetObjectField(static_cast<const Object &>(*this), *this);
}

template <>
Object::Field<jobject>::Reference &
Object::Field<jobject>::Reference::operator = (const jobject &value) {
    return (*this)->SetObjectField(*this, *this, value), *this;
}

template <>
jobject Object::Method<jobject>::Reference::call(va_list vl) const {
    return (*this)->CallObjectMethodV(*this, *this, vl);
}

// ...

template <>
Object::Field<jint[]>::Reference::operator Array<jint> () const {
    return Array<jint>(*this, (jintArray)(*this)->GetStaticObjectField(*this, *this));
}

template <>
Object::Field<jint[]>::Reference &
Object::Field<jint[]>::Reference::operator = (const Array<jint> &value) {
    return (*this)->SetStaticObjectField(*this, *this, value), *this;
}

template <>
Object::Field<jobject[]>::Reference::operator Array<jobject> () const {
    return Array<jobject>(*this, (jobjectArray)(*this)->GetStaticObjectField(*this, *this));
}

template <>
Object::Field<jobject[]>::Reference &
Object::Field<jobject[]>::Reference::operator = (const Array<jobject> &value) {
    return (*this)->SetStaticObjectField(*this, *this, value), *this;
}

// ...

} // namespace JNI
