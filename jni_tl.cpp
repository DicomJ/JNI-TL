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

template <> const char * Class::Static::Field<Nop>::Signature::value = "";
template <> const char * Class::Static::Field<void>::Signature::value = "V";

// jstring specialization
template <> const char * Class::Static::Field<jstring   >::Signature::value = "Ljava/lang/String;";
template <> const char * Class::Static::Field<jstring []>::Signature::value = "[Ljava/lang/String;";

template <>
Array<jstring>::Array(const Env &env, jsize count, jstring string)
    : Env(env), array(env->NewObjectArray(count, Class(env, "java.lang.String"), string)) {}

// ...

// jobject specialization

template <>
Array<jobject>::Array(const Env &env, jsize count, jclass clazz, jobject object)
    : Env(env), array(env->NewObjectArray(count, clazz, object)) {}

template <>
Array<jobject>::Object::Element::operator jobject () const {
    return (*this)->GetObjectArrayElement(*this, index);
}

template <>
Array<jobject>::Object::Element &
Array<jobject>::Object::Element::operator = (const jobject &object) {
    return (*this)->SetObjectArrayElement(*this, index, object), *this;
}

template <>
Array<jobject[]>::Array(const Env &env, jsize count, jclass clazz, jobject object)
    : Env(env), array(env->NewObjectArray(count, clazz, object)) {}

template <>
Array<jobject[]>::Object::Element &
Array<jobject[]>::Object::Element::operator = (const jobjectArray &value) {
    return (*this)->SetObjectArrayElement(*this, index, value), *this;
}

template <>
Array<jobject[]>::Object::Element::operator Array<jobject> () const {
    return Array<jobject>(*this, (jobjectArray)(*this)->GetObjectArrayElement(*this, index));
}

template <>
Class::Static::Field<jobject>::Reference::operator jobject () const {
    return (*this)->GetStaticObjectField(*this, *this);
}

template <>
Class::Static::Field<jobject>::Reference &
Class::Static::Field<jobject>::Reference::operator = (const jobject &value) {
    return (*this)->SetStaticObjectField(*this, *this, value), *this;
}

template <>
jobject Class::Static::Method<jobject>::Reference::call(va_list vl) const {
    return (*this)->CallStaticObjectMethodV(*this, *this, vl);
}

template <>
Class::Static::Field<jobject[]>::Reference::operator Array<jobject> () const {
    return Array<jobject>(*this, (jobjectArray) (*this)->GetStaticObjectField(*this, *this));
}

template <>
Class::Static::Field<jobject[]>::Reference &
Class::Static::Field<jobject[]>::Reference::operator = (const Array<jobject> &value) {
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
jobject Class::Method<jobject>::Reference::call(va_list vl) const {
    return (*this)->CallNonvirtualObjectMethodV(*this, *this, *this, vl);
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
Array<jobject> Class::Method<jobject[]>::Reference::call(va_list vl) const {
    return Array<jobject>(*this, (jobjectArray)(*this)->CallNonvirtualObjectMethodV(*this, *this, *this, vl));
}

template <>
Array<jobject> Object::Method<jobject[]>::Reference::call(va_list vl) const {
    return Array<jobject>(*this, (jobjectArray)(*this)->CallObjectMethodV(*this, *this, vl));
}

// jint specialization

template <> const char * Class::Static::Field<jint>::Signature::value = "I";
template <> const char * Class::Static::Field<jint[]>::Signature::value = "[I";

template <>
Array<jint>::Array(const Env &env, jsize count)
    : Env(env), array(env->NewIntArray(count)) {}

template <>
void Array<jint>::Elements::init() {
    array = (*this)->GetIntArrayElements(*this, &isCopy);
}

template <>
void Array<jint>::Elements::release() {
    (*this)->ReleaseIntArrayElements(*this, array, copyBack ? 0 : JNI_ABORT);
}

template <>
void Array<jint>::Elements::commit() {
    (*this)->ReleaseIntArrayElements(*this, array, JNI_COMMIT);
}

template <>
void Array<jint>::Region::Elements::get() {
    (*this)->GetIntArrayRegion(*this, JNI::Region::start, JNI::Region::length, region);
}

template <>
Array<jint>::Region::Elements &
Array<jint>::Region::Elements::operator = (const jint *values) {
    return (*this)->SetIntArrayRegion(*this, JNI::Region::start, JNI::Region::length, values), *this;
}

template <>
Array<jint[]>::Array(const Env &env, jsize count)
    : Env(env), array(env->NewObjectArray(count,
        Class(env, Class::Static::Field<jint[]>::Signature()), 0)) {}

template <>
Array<jint[]>::Object::Element &
Array<jint[]>::Object::Element::operator = (const jintArray &value) {
    return (*this)->SetObjectArrayElement(*this, index, value), *this;
}

template <>
Array<jint[]>::Object::Element::operator Array<jint> () const {
    return Array<jint>(*this, (jintArray)(*this)->GetObjectArrayElement(*this, index));
}

template <>
Class::Static::Field<jint>::Reference::operator jint () const {
    return (*this)->GetStaticIntField(*this, *this);
}

template <>
Class::Static::Field<jint>::Reference &
Class::Static::Field<jint>::Reference::operator = (const jint &value) {
    return (*this)->SetStaticIntField(*this, *this, value), *this;
}

template <>
Class::Static::Field<jint[]>::Reference::operator Array<jint> () const {
    return Array<jint>(*this, (jintArray)(*this)->GetStaticObjectField(*this, *this));
}

template <>
Class::Static::Field<jint[]>::Reference &
Class::Static::Field<jint[]>::Reference::operator = (const Array<jint> &value) {
    return (*this)->SetStaticObjectField(*this, *this, value), *this;
}

template <>
Array<jint> Class::Static::Method<jint[]>::Reference::call(va_list vl) const {
    return Array<jint>(*this, (jintArray)(*this)->CallStaticObjectMethodV(*this, *this, vl));
}

template <>
jint Class::Static::Method<jint>::Reference::call(va_list vl) const {
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
jint Class::Method<jint>::Reference::call(va_list vl) const {
    return (*this)->CallNonvirtualIntMethodV(*this, *this, *this, vl);
}

template <>
jint Object::Method<jint>::Reference::call(va_list vl) const {
    return (*this)->CallIntMethodV(*this, *this, vl);
}

template <>
Array<jint> Class::Method<jint[]>::Reference::call(va_list vl) const {
    return Array<jint>(*this, (jintArray)(*this)->CallNonvirtualObjectMethodV(*this, *this, *this, vl));
}

template <>
Array<jint> Object::Method<jint[]>::Reference::call(va_list vl) const {
    return Array<jint>(*this, (jintArray)(*this)->CallObjectMethodV(*this, *this, vl));
}

// jXXX specialization

template <> const char * Class::Static::Field<jboolean>::Signature::value = "Z";
template <> const char * Class::Static::Field<jboolean[]>::Signature::value = "[Z";

// ...

template <> const char * Class::Static::Field<jbyte>::Signature::value = "B";
template <> const char * Class::Static::Field<jbyte[]>::Signature::value = "[B";

// ...

template <> const char * Class::Static::Field<jchar>::Signature::value = "C";
template <> const char * Class::Static::Field<jchar[]>::Signature::value = "[C";

// ...

template <> const char * Class::Static::Field<jshort>::Signature::value = "S";
template <> const char * Class::Static::Field<jshort[]>::Signature::value = "[S";

// ...

template <> const char * Class::Static::Field<jlong>::Signature::value = "J";
template <> const char * Class::Static::Field<jlong[]>::Signature::value = "[J";

// ...

template <> const char * Class::Static::Field<jfloat>::Signature::value = "F";
template <> const char * Class::Static::Field<jfloat[]>::Signature::value = "[F";

// ...

template <> const char * Class::Static::Field<jdouble>::Signature::value = "D";
template <> const char * Class::Static::Field<jdouble[]>::Signature::value = "[D";

// ...


} // namespace JNI
