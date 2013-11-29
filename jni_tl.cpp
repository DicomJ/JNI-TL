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


template <>
Object Class::newObject(const Class::Constructor &constructor, ...) {
    va_list vl; va_start(vl, constructor);
    Object object(*this, (*this)->NewObjectV(*this,
                            Class::Constructor::ID(*this, constructor), vl));
    va_end(vl);
    return object;
}

template <>
Object Class::newObject(const Class::Constructor::ID &id, ...) {
    va_list vl; va_start(vl, id);
    Object object(*this, (*this)->NewObjectV(*this, id, vl));
    va_end(vl);
    return object;
}

// Type specific specialization

// void specialization

template <> const char * Class::Field<Nop>::Signature::value = "";
template <> const char * Class::Field<void>::Signature::value = "V";

// jstring specialization
template <> const char * Class::Field<jstring   >::Signature::value = "Ljava/lang/String;";
template <> const char * Class::Field<jstring []>::Signature::value = "[Ljava/lang/String;";

template <>
Array<jstring>::Array(const Env &env, jsize count, jstring string)
    : Env(env), array(env->NewObjectArray(count, Class(env, "java.lang.String"), string)) {}


// ...

// jobject specialization

template <>
Array<jobject>::Array(const Env &env, jsize count, jclass clazz, jobject object)
    : Env(env), array(env->NewObjectArray(count, clazz, object)) {}

template <>
Array<jobject>::Elements &
Array<jobject>::Elements::operator = (const jobject *values) {
    for (jsize i = 0; i < Region::length; ++i) {
        (*this)[Region::start + i] = values[i];
    } return *this;
}

template <>
Array<jobject>::Element::operator jobject () const {
    return (*this)->GetObjectArrayElement(*this, index);
}

template <>
Array<jobject>::Element &
Array<jobject>::Element::operator = (const jobject &object) {
    return (*this)->SetObjectArrayElement(*this, index, object), *this;
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

template <>
Class::Field<jobject[]>::Reference::operator Array<jobject> () const {
    return Array<jobject>(*this, (jobjectArray) (*this)->GetStaticObjectField(*this, *this));
}

template <>
Class::Field<jobject[]>::Reference &
Class::Field<jobject[]>::Reference::operator = (const Array<jobject> &value) {
    return (*this)->SetStaticObjectField(*this, *this, value), *this;
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

template <>
Object::Field<jobject[]>::Reference::operator Array<jobject> () const {
    return Array<jobject>(*this, (jobjectArray)(*this)->GetStaticObjectField(*this, *this));
}

template <>
Object::Field<jobject[]>::Reference &
Object::Field<jobject[]>::Reference::operator = (const Array<jobject> &value) {
    return (*this)->SetStaticObjectField(*this, *this, value), *this;
}

template <>
Array<jobject> Object::Method<jobject[]>::Reference::call(va_list vl) const {
    return Array<jobject>(*this, (jobjectArray)(*this)->CallObjectMethodV(*this, *this, vl));
}

// jint specialization

template <>
Array<jint>::Array(const Env &env, jsize count)
    : Env(env), array(env->NewIntArray(count)) {}

template <>
jint *Array<jint>::Elements::array() const {
    return (*this)->GetIntArrayElements(*this, 0);
}

template <>
Array<jint>::Elements &
Array<jint>::Elements::operator = (const jint *values) {
    return (*this)->SetIntArrayRegion(*this, Region::start, Region::length, values), *this;
}


template <>
Array<jint[]>::Array(const Env &env, jsize count)
    : Env(env), array(env->NewObjectArray(count,
        Class(env, Class::Field<jint[]>::Signature()), 0)) {}

template <>
Array<jint[]>::Element &
Array<jint[]>::Element::operator = (const jintArray &value) {
    return (*this)->SetObjectArrayElement(*this, index, value), *this;
}

template <>
Array<jint[]>::Elements &
Array<jint[]>::Elements::operator = (const jintArray *values) {
    for (jsize i = 0; i < Region::length; ++i) {
        (*this)[Region::start + i] = values[i];
    } return *this;
}

template <>
Array<jint[]>::Element::operator Array<jint> () const {
    return Array<jint>(*this, (jintArray)(*this)->GetObjectArrayElement(*this, index));
}

template <> const char * Class::Field<jint      >::Signature::value = "I";
template <> const char * Class::Field<jint    []>::Signature::value = "[I";

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
Class::Field<jint[]>::Reference::operator Array<jint> () const {
    return Array<jint>(*this, (jintArray)(*this)->GetStaticObjectField(*this, *this));
}

template <>
Class::Field<jint[]>::Reference &
Class::Field<jint[]>::Reference::operator = (const Array<jint> &value) {
    return (*this)->SetStaticObjectField(*this, *this, value), *this;
}

template <>
Array<jint> Class::Method<jint[]>::Reference::call(va_list vl) const {
    return Array<jint>(*this, (jintArray)(*this)->CallStaticObjectMethodV(*this, *this, vl));
}

template <>
jint Class::Method<jint>::Reference::call(va_list vl) const {
    return (*this)->CallStaticIntMethodV(*this, *this, vl);
}

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
Object::Field<jint[]>::Reference::operator Array<jint> () const {
    return Array<jint>(*this, (jintArray)(*this)->GetStaticObjectField(*this, *this));
}

template <>
Object::Field<jint[]>::Reference &
Object::Field<jint[]>::Reference::operator = (const Array<jint> &value) {
    return (*this)->SetStaticObjectField(*this, *this, value), *this;
}

template <>
jint Object::Method<jint>::Reference::call(va_list vl) const {
    return (*this)->CallIntMethodV(*this, *this, vl);
}

template <>
Array<jint> Object::Method<jint[]>::Reference::call(va_list vl) const {
    return Array<jint>(*this, (jintArray)(*this)->CallObjectMethodV(*this, *this, vl));
}

// jXXX specialization

template <> const char * Class::Field<jboolean  >::Signature::value = "Z";
template <> const char * Class::Field<jbyte     >::Signature::value = "B";
template <> const char * Class::Field<jchar     >::Signature::value = "C";
template <> const char * Class::Field<jshort    >::Signature::value = "S";
template <> const char * Class::Field<jlong     >::Signature::value = "J";
template <> const char * Class::Field<jfloat    >::Signature::value = "F";
template <> const char * Class::Field<jdouble   >::Signature::value = "D";

template <> const char * Class::Field<jboolean[]>::Signature::value = "[Z";
template <> const char * Class::Field<jbyte   []>::Signature::value = "[B";
template <> const char * Class::Field<jchar   []>::Signature::value = "[C";
template <> const char * Class::Field<jshort  []>::Signature::value = "[S";
template <> const char * Class::Field<jlong   []>::Signature::value = "[J";
template <> const char * Class::Field<jfloat  []>::Signature::value = "[F";
template <> const char * Class::Field<jdouble []>::Signature::value = "[D";


} // namespace JNI
