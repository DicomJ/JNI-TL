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

template <>
Array<jstring>::Object::Element::operator jstring () const {
    return (jstring)(*this)->GetObjectArrayElement(*this, index);
}

template <>
Array<jstring>::Object::Element &
Array<jstring>::Object::Element::operator = (const jstring &string) {
    return (*this)->SetObjectArrayElement(*this, index, string), *this;
}

template <>
Array<jstring[]>::Array(const Env &env, jsize count)
    : Env(env), array(env->NewObjectArray(count,
        Class(env, Class::Static::Field<jstring[]>::Signature()), 0)) {}

template <>
Array<jstring[]>::Object::Element &
Array<jstring[]>::Object::Element::operator = (const jobjectArray &value) {
    return (*this)->SetObjectArrayElement(*this, index, value), *this;
}

template <>
Array<jstring[]>::Object::Element::operator Array<jstring> () const {
    return Array<jstring>(*this, (jobjectArray)(*this)->GetObjectArrayElement(*this, index));
}

template <>
Class::Static::Field<jstring>::Reference::operator jstring () const {
    return (jstring)(*this)->GetStaticObjectField(*this, *this);
}

template <>
Class::Static::Field<jstring>::Reference &
Class::Static::Field<jstring>::Reference::operator = (const jstring &value) {
    return (*this)->SetStaticObjectField(*this, *this, value), *this;
}

template <>
jstring Class::Static::Method<jstring>::Reference::call(va_list vl) const {
    return (jstring)(*this)->CallStaticObjectMethodV(*this, *this, vl);
}

template <>
Class::Static::Field<jstring[]>::Reference::operator Array<jstring> () const {
    return Array<jstring>(*this, (jobjectArray) (*this)->GetStaticObjectField(*this, *this));
}

template <>
Class::Static::Field<jstring[]>::Reference &
Class::Static::Field<jstring[]>::Reference::operator = (const Array<jstring> &value) {
    return (*this)->SetStaticObjectField(*this, *this, value), *this;
}

template <>
Object::Field<jstring>::Reference::operator jstring () const {
    return (jstring)(*this)->GetObjectField(static_cast<const Object &>(*this), *this);
}

template <>
Object::Field<jstring>::Reference &
Object::Field<jstring>::Reference::operator = (const jstring &value) {
    return (*this)->SetObjectField(*this, *this, value), *this;
}

template <>
jstring Class::Method<jstring>::Reference::call(va_list vl) const {
    return (jstring)(*this)->CallNonvirtualObjectMethodV(*this, *this, *this, vl);
}

template <>
jstring Object::Method<jstring>::Reference::call(va_list vl) const {
    return (jstring)(*this)->CallObjectMethodV(*this, *this, vl);
}

template <>
Object::Field<jstring[]>::Reference::operator Array<jstring> () const {
    return Array<jstring>(*this, (jobjectArray)(*this)->GetStaticObjectField(*this, *this));
}

template <>
Object::Field<jstring[]>::Reference &
Object::Field<jstring[]>::Reference::operator = (const Array<jstring> &value) {
    return (*this)->SetStaticObjectField(*this, *this, value), *this;
}

template <>
Array<jstring> Class::Method<jstring[]>::Reference::call(va_list vl) const {
    return Array<jstring>(*this, (jobjectArray)(*this)->CallNonvirtualObjectMethodV(*this, *this, *this, vl));
}

template <>
Array<jstring> Object::Method<jstring[]>::Reference::call(va_list vl) const {
    return Array<jstring>(*this, (jobjectArray)(*this)->CallObjectMethodV(*this, *this, vl));
}

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

// jboolean

template <> const char * Class::Static::Field<jboolean>::Signature::value = "Z";
template <> const char * Class::Static::Field<jboolean[]>::Signature::value = "[Z";

template <>
Array<jboolean>::Array(const Env &env, jsize count)
    : Env(env), array(env->NewBooleanArray(count)) {}

template <>
void Array<jboolean>::Elements::init() {
    array = (*this)->GetBooleanArrayElements(*this, &isCopy);
}

template <>
void Array<jboolean>::Elements::release() {
    (*this)->ReleaseBooleanArrayElements(*this, array, copyBack ? 0 : JNI_ABORT);
}

template <>
void Array<jboolean>::Elements::commit() {
    (*this)->ReleaseBooleanArrayElements(*this, array, JNI_COMMIT);
}

template <>
void Array<jboolean>::Region::Elements::get() {
    (*this)->GetBooleanArrayRegion(*this, JNI::Region::start, JNI::Region::length, region);
}

template <>
Array<jboolean>::Region::Elements &
Array<jboolean>::Region::Elements::operator = (const jboolean *values) {
    return (*this)->SetBooleanArrayRegion(*this, JNI::Region::start, JNI::Region::length, values), *this;
}

template <>
Array<jboolean[]>::Array(const Env &env, jsize count)
    : Env(env), array(env->NewObjectArray(count,
        Class(env, Class::Static::Field<jboolean[]>::Signature()), 0)) {}

template <>
Array<jboolean[]>::Object::Element &
Array<jboolean[]>::Object::Element::operator = (const jbooleanArray &value) {
    return (*this)->SetObjectArrayElement(*this, index, value), *this;
}

template <>
Array<jboolean[]>::Object::Element::operator Array<jboolean> () const {
    return Array<jboolean>(*this, (jbooleanArray)(*this)->GetObjectArrayElement(*this, index));
}

template <>
Class::Static::Field<jboolean>::Reference::operator jboolean () const {
    return (*this)->GetStaticBooleanField(*this, *this);
}

template <>
Class::Static::Field<jboolean>::Reference &
Class::Static::Field<jboolean>::Reference::operator = (const jboolean &value) {
    return (*this)->SetStaticBooleanField(*this, *this, value), *this;
}

template <>
Class::Static::Field<jboolean[]>::Reference::operator Array<jboolean> () const {
    return Array<jboolean>(*this, (jbooleanArray)(*this)->GetStaticObjectField(*this, *this));
}

template <>
Class::Static::Field<jboolean[]>::Reference &
Class::Static::Field<jboolean[]>::Reference::operator = (const Array<jboolean> &value) {
    return (*this)->SetStaticObjectField(*this, *this, value), *this;
}

template <>
Array<jboolean> Class::Static::Method<jboolean[]>::Reference::call(va_list vl) const {
    return Array<jboolean>(*this, (jbooleanArray)(*this)->CallStaticObjectMethodV(*this, *this, vl));
}

template <>
jboolean Class::Static::Method<jboolean>::Reference::call(va_list vl) const {
    return (*this)->CallStaticBooleanMethodV(*this, *this, vl);
}

template <>
Object::Field<jboolean>::Reference::operator jboolean () const {
    return (*this)->GetBooleanField(*this, *this);
}

template <>
Object::Field<jboolean>::Reference &
Object::Field<jboolean>::Reference::operator = (const jboolean &value) {
    return (*this)->SetBooleanField(*this, *this, value), *this;
}

template <>
Object::Field<jboolean[]>::Reference::operator Array<jboolean> () const {
    return Array<jboolean>(*this, (jbooleanArray)(*this)->GetStaticObjectField(*this, *this));
}

template <>
Object::Field<jboolean[]>::Reference &
Object::Field<jboolean[]>::Reference::operator = (const Array<jboolean> &value) {
    return (*this)->SetStaticObjectField(*this, *this, value), *this;
}

template <>
jboolean Class::Method<jboolean>::Reference::call(va_list vl) const {
    return (*this)->CallNonvirtualBooleanMethodV(*this, *this, *this, vl);
}

template <>
jboolean Object::Method<jboolean>::Reference::call(va_list vl) const {
    return (*this)->CallBooleanMethodV(*this, *this, vl);
}

template <>
Array<jboolean> Class::Method<jboolean[]>::Reference::call(va_list vl) const {
    return Array<jboolean>(*this, (jbooleanArray)(*this)->CallNonvirtualObjectMethodV(*this, *this, *this, vl));
}

template <>
Array<jboolean> Object::Method<jboolean[]>::Reference::call(va_list vl) const {
    return Array<jboolean>(*this, (jbooleanArray)(*this)->CallObjectMethodV(*this, *this, vl));
}

// jbyte

template <> const char * Class::Static::Field<jbyte>::Signature::value = "B";
template <> const char * Class::Static::Field<jbyte[]>::Signature::value = "[B";

template <>
Array<jbyte>::Array(const Env &env, jsize count)
    : Env(env), array(env->NewByteArray(count)) {}

template <>
void Array<jbyte>::Elements::init() {
    array = (*this)->GetByteArrayElements(*this, &isCopy);
}

template <>
void Array<jbyte>::Elements::release() {
    (*this)->ReleaseByteArrayElements(*this, array, copyBack ? 0 : JNI_ABORT);
}

template <>
void Array<jbyte>::Elements::commit() {
    (*this)->ReleaseByteArrayElements(*this, array, JNI_COMMIT);
}

template <>
void Array<jbyte>::Region::Elements::get() {
    (*this)->GetByteArrayRegion(*this, JNI::Region::start, JNI::Region::length, region);
}

template <>
Array<jbyte>::Region::Elements &
Array<jbyte>::Region::Elements::operator = (const jbyte *values) {
    return (*this)->SetByteArrayRegion(*this, JNI::Region::start, JNI::Region::length, values), *this;
}

template <>
Array<jbyte[]>::Array(const Env &env, jsize count)
    : Env(env), array(env->NewObjectArray(count,
        Class(env, Class::Static::Field<jbyte[]>::Signature()), 0)) {}

template <>
Array<jbyte[]>::Object::Element &
Array<jbyte[]>::Object::Element::operator = (const jbyteArray &value) {
    return (*this)->SetObjectArrayElement(*this, index, value), *this;
}

template <>
Array<jbyte[]>::Object::Element::operator Array<jbyte> () const {
    return Array<jbyte>(*this, (jbyteArray)(*this)->GetObjectArrayElement(*this, index));
}

template <>
Class::Static::Field<jbyte>::Reference::operator jbyte () const {
    return (*this)->GetStaticByteField(*this, *this);
}

template <>
Class::Static::Field<jbyte>::Reference &
Class::Static::Field<jbyte>::Reference::operator = (const jbyte &value) {
    return (*this)->SetStaticByteField(*this, *this, value), *this;
}

template <>
Class::Static::Field<jbyte[]>::Reference::operator Array<jbyte> () const {
    return Array<jbyte>(*this, (jbyteArray)(*this)->GetStaticObjectField(*this, *this));
}

template <>
Class::Static::Field<jbyte[]>::Reference &
Class::Static::Field<jbyte[]>::Reference::operator = (const Array<jbyte> &value) {
    return (*this)->SetStaticObjectField(*this, *this, value), *this;
}

template <>
Array<jbyte> Class::Static::Method<jbyte[]>::Reference::call(va_list vl) const {
    return Array<jbyte>(*this, (jbyteArray)(*this)->CallStaticObjectMethodV(*this, *this, vl));
}

template <>
jbyte Class::Static::Method<jbyte>::Reference::call(va_list vl) const {
    return (*this)->CallStaticByteMethodV(*this, *this, vl);
}

template <>
Object::Field<jbyte>::Reference::operator jbyte () const {
    return (*this)->GetByteField(*this, *this);
}

template <>
Object::Field<jbyte>::Reference &
Object::Field<jbyte>::Reference::operator = (const jbyte &value) {
    return (*this)->SetByteField(*this, *this, value), *this;
}

template <>
Object::Field<jbyte[]>::Reference::operator Array<jbyte> () const {
    return Array<jbyte>(*this, (jbyteArray)(*this)->GetStaticObjectField(*this, *this));
}

template <>
Object::Field<jbyte[]>::Reference &
Object::Field<jbyte[]>::Reference::operator = (const Array<jbyte> &value) {
    return (*this)->SetStaticObjectField(*this, *this, value), *this;
}

template <>
jbyte Class::Method<jbyte>::Reference::call(va_list vl) const {
    return (*this)->CallNonvirtualByteMethodV(*this, *this, *this, vl);
}

template <>
jbyte Object::Method<jbyte>::Reference::call(va_list vl) const {
    return (*this)->CallByteMethodV(*this, *this, vl);
}

template <>
Array<jbyte> Class::Method<jbyte[]>::Reference::call(va_list vl) const {
    return Array<jbyte>(*this, (jbyteArray)(*this)->CallNonvirtualObjectMethodV(*this, *this, *this, vl));
}

template <>
Array<jbyte> Object::Method<jbyte[]>::Reference::call(va_list vl) const {
    return Array<jbyte>(*this, (jbyteArray)(*this)->CallObjectMethodV(*this, *this, vl));
}

// jchar

template <> const char * Class::Static::Field<jchar>::Signature::value = "C";
template <> const char * Class::Static::Field<jchar[]>::Signature::value = "[C";

template <>
Array<jchar>::Array(const Env &env, jsize count)
    : Env(env), array(env->NewCharArray(count)) {}

template <>
void Array<jchar>::Elements::init() {
    array = (*this)->GetCharArrayElements(*this, &isCopy);
}

template <>
void Array<jchar>::Elements::release() {
    (*this)->ReleaseCharArrayElements(*this, array, copyBack ? 0 : JNI_ABORT);
}

template <>
void Array<jchar>::Elements::commit() {
    (*this)->ReleaseCharArrayElements(*this, array, JNI_COMMIT);
}

template <>
void Array<jchar>::Region::Elements::get() {
    (*this)->GetCharArrayRegion(*this, JNI::Region::start, JNI::Region::length, region);
}

template <>
Array<jchar>::Region::Elements &
Array<jchar>::Region::Elements::operator = (const jchar *values) {
    return (*this)->SetCharArrayRegion(*this, JNI::Region::start, JNI::Region::length, values), *this;
}

template <>
Array<jchar[]>::Array(const Env &env, jsize count)
    : Env(env), array(env->NewObjectArray(count,
        Class(env, Class::Static::Field<jchar[]>::Signature()), 0)) {}

template <>
Array<jchar[]>::Object::Element &
Array<jchar[]>::Object::Element::operator = (const jcharArray &value) {
    return (*this)->SetObjectArrayElement(*this, index, value), *this;
}

template <>
Array<jchar[]>::Object::Element::operator Array<jchar> () const {
    return Array<jchar>(*this, (jcharArray)(*this)->GetObjectArrayElement(*this, index));
}

template <>
Class::Static::Field<jchar>::Reference::operator jchar () const {
    return (*this)->GetStaticCharField(*this, *this);
}

template <>
Class::Static::Field<jchar>::Reference &
Class::Static::Field<jchar>::Reference::operator = (const jchar &value) {
    return (*this)->SetStaticCharField(*this, *this, value), *this;
}

template <>
Class::Static::Field<jchar[]>::Reference::operator Array<jchar> () const {
    return Array<jchar>(*this, (jcharArray)(*this)->GetStaticObjectField(*this, *this));
}

template <>
Class::Static::Field<jchar[]>::Reference &
Class::Static::Field<jchar[]>::Reference::operator = (const Array<jchar> &value) {
    return (*this)->SetStaticObjectField(*this, *this, value), *this;
}

template <>
Array<jchar> Class::Static::Method<jchar[]>::Reference::call(va_list vl) const {
    return Array<jchar>(*this, (jcharArray)(*this)->CallStaticObjectMethodV(*this, *this, vl));
}

template <>
jchar Class::Static::Method<jchar>::Reference::call(va_list vl) const {
    return (*this)->CallStaticCharMethodV(*this, *this, vl);
}

template <>
Object::Field<jchar>::Reference::operator jchar () const {
    return (*this)->GetCharField(*this, *this);
}

template <>
Object::Field<jchar>::Reference &
Object::Field<jchar>::Reference::operator = (const jchar &value) {
    return (*this)->SetCharField(*this, *this, value), *this;
}

template <>
Object::Field<jchar[]>::Reference::operator Array<jchar> () const {
    return Array<jchar>(*this, (jcharArray)(*this)->GetStaticObjectField(*this, *this));
}

template <>
Object::Field<jchar[]>::Reference &
Object::Field<jchar[]>::Reference::operator = (const Array<jchar> &value) {
    return (*this)->SetStaticObjectField(*this, *this, value), *this;
}

template <>
jchar Class::Method<jchar>::Reference::call(va_list vl) const {
    return (*this)->CallNonvirtualCharMethodV(*this, *this, *this, vl);
}

template <>
jchar Object::Method<jchar>::Reference::call(va_list vl) const {
    return (*this)->CallCharMethodV(*this, *this, vl);
}

template <>
Array<jchar> Class::Method<jchar[]>::Reference::call(va_list vl) const {
    return Array<jchar>(*this, (jcharArray)(*this)->CallNonvirtualObjectMethodV(*this, *this, *this, vl));
}

template <>
Array<jchar> Object::Method<jchar[]>::Reference::call(va_list vl) const {
    return Array<jchar>(*this, (jcharArray)(*this)->CallObjectMethodV(*this, *this, vl));
}

// jshort

template <> const char * Class::Static::Field<jshort>::Signature::value = "S";
template <> const char * Class::Static::Field<jshort[]>::Signature::value = "[S";

template <>
Array<jshort>::Array(const Env &env, jsize count)
    : Env(env), array(env->NewShortArray(count)) {}

template <>
void Array<jshort>::Elements::init() {
    array = (*this)->GetShortArrayElements(*this, &isCopy);
}

template <>
void Array<jshort>::Elements::release() {
    (*this)->ReleaseShortArrayElements(*this, array, copyBack ? 0 : JNI_ABORT);
}

template <>
void Array<jshort>::Elements::commit() {
    (*this)->ReleaseShortArrayElements(*this, array, JNI_COMMIT);
}

template <>
void Array<jshort>::Region::Elements::get() {
    (*this)->GetShortArrayRegion(*this, JNI::Region::start, JNI::Region::length, region);
}

template <>
Array<jshort>::Region::Elements &
Array<jshort>::Region::Elements::operator = (const jshort *values) {
    return (*this)->SetShortArrayRegion(*this, JNI::Region::start, JNI::Region::length, values), *this;
}

template <>
Array<jshort[]>::Array(const Env &env, jsize count)
    : Env(env), array(env->NewObjectArray(count,
        Class(env, Class::Static::Field<jshort[]>::Signature()), 0)) {}

template <>
Array<jshort[]>::Object::Element &
Array<jshort[]>::Object::Element::operator = (const jshortArray &value) {
    return (*this)->SetObjectArrayElement(*this, index, value), *this;
}

template <>
Array<jshort[]>::Object::Element::operator Array<jshort> () const {
    return Array<jshort>(*this, (jshortArray)(*this)->GetObjectArrayElement(*this, index));
}

template <>
Class::Static::Field<jshort>::Reference::operator jshort () const {
    return (*this)->GetStaticShortField(*this, *this);
}

template <>
Class::Static::Field<jshort>::Reference &
Class::Static::Field<jshort>::Reference::operator = (const jshort &value) {
    return (*this)->SetStaticShortField(*this, *this, value), *this;
}

template <>
Class::Static::Field<jshort[]>::Reference::operator Array<jshort> () const {
    return Array<jshort>(*this, (jshortArray)(*this)->GetStaticObjectField(*this, *this));
}

template <>
Class::Static::Field<jshort[]>::Reference &
Class::Static::Field<jshort[]>::Reference::operator = (const Array<jshort> &value) {
    return (*this)->SetStaticObjectField(*this, *this, value), *this;
}

template <>
Array<jshort> Class::Static::Method<jshort[]>::Reference::call(va_list vl) const {
    return Array<jshort>(*this, (jshortArray)(*this)->CallStaticObjectMethodV(*this, *this, vl));
}

template <>
jshort Class::Static::Method<jshort>::Reference::call(va_list vl) const {
    return (*this)->CallStaticShortMethodV(*this, *this, vl);
}

template <>
Object::Field<jshort>::Reference::operator jshort () const {
    return (*this)->GetShortField(*this, *this);
}

template <>
Object::Field<jshort>::Reference &
Object::Field<jshort>::Reference::operator = (const jshort &value) {
    return (*this)->SetShortField(*this, *this, value), *this;
}

template <>
Object::Field<jshort[]>::Reference::operator Array<jshort> () const {
    return Array<jshort>(*this, (jshortArray)(*this)->GetStaticObjectField(*this, *this));
}

template <>
Object::Field<jshort[]>::Reference &
Object::Field<jshort[]>::Reference::operator = (const Array<jshort> &value) {
    return (*this)->SetStaticObjectField(*this, *this, value), *this;
}

template <>
jshort Class::Method<jshort>::Reference::call(va_list vl) const {
    return (*this)->CallNonvirtualShortMethodV(*this, *this, *this, vl);
}

template <>
jshort Object::Method<jshort>::Reference::call(va_list vl) const {
    return (*this)->CallShortMethodV(*this, *this, vl);
}

template <>
Array<jshort> Class::Method<jshort[]>::Reference::call(va_list vl) const {
    return Array<jshort>(*this, (jshortArray)(*this)->CallNonvirtualObjectMethodV(*this, *this, *this, vl));
}

template <>
Array<jshort> Object::Method<jshort[]>::Reference::call(va_list vl) const {
    return Array<jshort>(*this, (jshortArray)(*this)->CallObjectMethodV(*this, *this, vl));
}

// jlong

template <> const char * Class::Static::Field<jlong>::Signature::value = "J";
template <> const char * Class::Static::Field<jlong[]>::Signature::value = "[J";

template <>
Array<jlong>::Array(const Env &env, jsize count)
    : Env(env), array(env->NewLongArray(count)) {}

template <>
void Array<jlong>::Elements::init() {
    array = (*this)->GetLongArrayElements(*this, &isCopy);
}

template <>
void Array<jlong>::Elements::release() {
    (*this)->ReleaseLongArrayElements(*this, array, copyBack ? 0 : JNI_ABORT);
}

template <>
void Array<jlong>::Elements::commit() {
    (*this)->ReleaseLongArrayElements(*this, array, JNI_COMMIT);
}

template <>
void Array<jlong>::Region::Elements::get() {
    (*this)->GetLongArrayRegion(*this, JNI::Region::start, JNI::Region::length, region);
}

template <>
Array<jlong>::Region::Elements &
Array<jlong>::Region::Elements::operator = (const jlong *values) {
    return (*this)->SetLongArrayRegion(*this, JNI::Region::start, JNI::Region::length, values), *this;
}

template <>
Array<jlong[]>::Array(const Env &env, jsize count)
    : Env(env), array(env->NewObjectArray(count,
        Class(env, Class::Static::Field<jlong[]>::Signature()), 0)) {}

template <>
Array<jlong[]>::Object::Element &
Array<jlong[]>::Object::Element::operator = (const jlongArray &value) {
    return (*this)->SetObjectArrayElement(*this, index, value), *this;
}

template <>
Array<jlong[]>::Object::Element::operator Array<jlong> () const {
    return Array<jlong>(*this, (jlongArray)(*this)->GetObjectArrayElement(*this, index));
}

template <>
Class::Static::Field<jlong>::Reference::operator jlong () const {
    return (*this)->GetStaticLongField(*this, *this);
}

template <>
Class::Static::Field<jlong>::Reference &
Class::Static::Field<jlong>::Reference::operator = (const jlong &value) {
    return (*this)->SetStaticLongField(*this, *this, value), *this;
}

template <>
Class::Static::Field<jlong[]>::Reference::operator Array<jlong> () const {
    return Array<jlong>(*this, (jlongArray)(*this)->GetStaticObjectField(*this, *this));
}

template <>
Class::Static::Field<jlong[]>::Reference &
Class::Static::Field<jlong[]>::Reference::operator = (const Array<jlong> &value) {
    return (*this)->SetStaticObjectField(*this, *this, value), *this;
}

template <>
Array<jlong> Class::Static::Method<jlong[]>::Reference::call(va_list vl) const {
    return Array<jlong>(*this, (jlongArray)(*this)->CallStaticObjectMethodV(*this, *this, vl));
}

template <>
jlong Class::Static::Method<jlong>::Reference::call(va_list vl) const {
    return (*this)->CallStaticLongMethodV(*this, *this, vl);
}

template <>
Object::Field<jlong>::Reference::operator jlong () const {
    return (*this)->GetLongField(*this, *this);
}

template <>
Object::Field<jlong>::Reference &
Object::Field<jlong>::Reference::operator = (const jlong &value) {
    return (*this)->SetLongField(*this, *this, value), *this;
}

template <>
Object::Field<jlong[]>::Reference::operator Array<jlong> () const {
    return Array<jlong>(*this, (jlongArray)(*this)->GetStaticObjectField(*this, *this));
}

template <>
Object::Field<jlong[]>::Reference &
Object::Field<jlong[]>::Reference::operator = (const Array<jlong> &value) {
    return (*this)->SetStaticObjectField(*this, *this, value), *this;
}

template <>
jlong Class::Method<jlong>::Reference::call(va_list vl) const {
    return (*this)->CallNonvirtualLongMethodV(*this, *this, *this, vl);
}

template <>
jlong Object::Method<jlong>::Reference::call(va_list vl) const {
    return (*this)->CallLongMethodV(*this, *this, vl);
}

template <>
Array<jlong> Class::Method<jlong[]>::Reference::call(va_list vl) const {
    return Array<jlong>(*this, (jlongArray)(*this)->CallNonvirtualObjectMethodV(*this, *this, *this, vl));
}

template <>
Array<jlong> Object::Method<jlong[]>::Reference::call(va_list vl) const {
    return Array<jlong>(*this, (jlongArray)(*this)->CallObjectMethodV(*this, *this, vl));
}

// jfloat

template <> const char * Class::Static::Field<jfloat>::Signature::value = "F";
template <> const char * Class::Static::Field<jfloat[]>::Signature::value = "[F";

template <>
Array<jfloat>::Array(const Env &env, jsize count)
    : Env(env), array(env->NewFloatArray(count)) {}

template <>
void Array<jfloat>::Elements::init() {
    array = (*this)->GetFloatArrayElements(*this, &isCopy);
}

template <>
void Array<jfloat>::Elements::release() {
    (*this)->ReleaseFloatArrayElements(*this, array, copyBack ? 0 : JNI_ABORT);
}

template <>
void Array<jfloat>::Elements::commit() {
    (*this)->ReleaseFloatArrayElements(*this, array, JNI_COMMIT);
}

template <>
void Array<jfloat>::Region::Elements::get() {
    (*this)->GetFloatArrayRegion(*this, JNI::Region::start, JNI::Region::length, region);
}

template <>
Array<jfloat>::Region::Elements &
Array<jfloat>::Region::Elements::operator = (const jfloat *values) {
    return (*this)->SetFloatArrayRegion(*this, JNI::Region::start, JNI::Region::length, values), *this;
}

template <>
Array<jfloat[]>::Array(const Env &env, jsize count)
    : Env(env), array(env->NewObjectArray(count,
        Class(env, Class::Static::Field<jfloat[]>::Signature()), 0)) {}

template <>
Array<jfloat[]>::Object::Element &
Array<jfloat[]>::Object::Element::operator = (const jfloatArray &value) {
    return (*this)->SetObjectArrayElement(*this, index, value), *this;
}

template <>
Array<jfloat[]>::Object::Element::operator Array<jfloat> () const {
    return Array<jfloat>(*this, (jfloatArray)(*this)->GetObjectArrayElement(*this, index));
}

template <>
Class::Static::Field<jfloat>::Reference::operator jfloat () const {
    return (*this)->GetStaticFloatField(*this, *this);
}

template <>
Class::Static::Field<jfloat>::Reference &
Class::Static::Field<jfloat>::Reference::operator = (const jfloat &value) {
    return (*this)->SetStaticFloatField(*this, *this, value), *this;
}

template <>
Class::Static::Field<jfloat[]>::Reference::operator Array<jfloat> () const {
    return Array<jfloat>(*this, (jfloatArray)(*this)->GetStaticObjectField(*this, *this));
}

template <>
Class::Static::Field<jfloat[]>::Reference &
Class::Static::Field<jfloat[]>::Reference::operator = (const Array<jfloat> &value) {
    return (*this)->SetStaticObjectField(*this, *this, value), *this;
}

template <>
Array<jfloat> Class::Static::Method<jfloat[]>::Reference::call(va_list vl) const {
    return Array<jfloat>(*this, (jfloatArray)(*this)->CallStaticObjectMethodV(*this, *this, vl));
}

template <>
jfloat Class::Static::Method<jfloat>::Reference::call(va_list vl) const {
    return (*this)->CallStaticFloatMethodV(*this, *this, vl);
}

template <>
Object::Field<jfloat>::Reference::operator jfloat () const {
    return (*this)->GetFloatField(*this, *this);
}

template <>
Object::Field<jfloat>::Reference &
Object::Field<jfloat>::Reference::operator = (const jfloat &value) {
    return (*this)->SetFloatField(*this, *this, value), *this;
}

template <>
Object::Field<jfloat[]>::Reference::operator Array<jfloat> () const {
    return Array<jfloat>(*this, (jfloatArray)(*this)->GetStaticObjectField(*this, *this));
}

template <>
Object::Field<jfloat[]>::Reference &
Object::Field<jfloat[]>::Reference::operator = (const Array<jfloat> &value) {
    return (*this)->SetStaticObjectField(*this, *this, value), *this;
}

template <>
jfloat Class::Method<jfloat>::Reference::call(va_list vl) const {
    return (*this)->CallNonvirtualFloatMethodV(*this, *this, *this, vl);
}

template <>
jfloat Object::Method<jfloat>::Reference::call(va_list vl) const {
    return (*this)->CallFloatMethodV(*this, *this, vl);
}

template <>
Array<jfloat> Class::Method<jfloat[]>::Reference::call(va_list vl) const {
    return Array<jfloat>(*this, (jfloatArray)(*this)->CallNonvirtualObjectMethodV(*this, *this, *this, vl));
}

template <>
Array<jfloat> Object::Method<jfloat[]>::Reference::call(va_list vl) const {
    return Array<jfloat>(*this, (jfloatArray)(*this)->CallObjectMethodV(*this, *this, vl));
}

// jbouble

template <> const char * Class::Static::Field<jdouble>::Signature::value = "D";
template <> const char * Class::Static::Field<jdouble[]>::Signature::value = "[D";

template <>
Array<jdouble>::Array(const Env &env, jsize count)
    : Env(env), array(env->NewDoubleArray(count)) {}

template <>
void Array<jdouble>::Elements::init() {
    array = (*this)->GetDoubleArrayElements(*this, &isCopy);
}

template <>
void Array<jdouble>::Elements::release() {
    (*this)->ReleaseDoubleArrayElements(*this, array, copyBack ? 0 : JNI_ABORT);
}

template <>
void Array<jdouble>::Elements::commit() {
    (*this)->ReleaseDoubleArrayElements(*this, array, JNI_COMMIT);
}

template <>
void Array<jdouble>::Region::Elements::get() {
    (*this)->GetDoubleArrayRegion(*this, JNI::Region::start, JNI::Region::length, region);
}

template <>
Array<jdouble>::Region::Elements &
Array<jdouble>::Region::Elements::operator = (const jdouble *values) {
    return (*this)->SetDoubleArrayRegion(*this, JNI::Region::start, JNI::Region::length, values), *this;
}

template <>
Array<jdouble[]>::Array(const Env &env, jsize count)
    : Env(env), array(env->NewObjectArray(count,
        Class(env, Class::Static::Field<jdouble[]>::Signature()), 0)) {}

template <>
Array<jdouble[]>::Object::Element &
Array<jdouble[]>::Object::Element::operator = (const jdoubleArray &value) {
    return (*this)->SetObjectArrayElement(*this, index, value), *this;
}

template <>
Array<jdouble[]>::Object::Element::operator Array<jdouble> () const {
    return Array<jdouble>(*this, (jdoubleArray)(*this)->GetObjectArrayElement(*this, index));
}

template <>
Class::Static::Field<jdouble>::Reference::operator jdouble () const {
    return (*this)->GetStaticDoubleField(*this, *this);
}

template <>
Class::Static::Field<jdouble>::Reference &
Class::Static::Field<jdouble>::Reference::operator = (const jdouble &value) {
    return (*this)->SetStaticDoubleField(*this, *this, value), *this;
}

template <>
Class::Static::Field<jdouble[]>::Reference::operator Array<jdouble> () const {
    return Array<jdouble>(*this, (jdoubleArray)(*this)->GetStaticObjectField(*this, *this));
}

template <>
Class::Static::Field<jdouble[]>::Reference &
Class::Static::Field<jdouble[]>::Reference::operator = (const Array<jdouble> &value) {
    return (*this)->SetStaticObjectField(*this, *this, value), *this;
}

template <>
Array<jdouble> Class::Static::Method<jdouble[]>::Reference::call(va_list vl) const {
    return Array<jdouble>(*this, (jdoubleArray)(*this)->CallStaticObjectMethodV(*this, *this, vl));
}

template <>
jdouble Class::Static::Method<jdouble>::Reference::call(va_list vl) const {
    return (*this)->CallStaticDoubleMethodV(*this, *this, vl);
}

template <>
Object::Field<jdouble>::Reference::operator jdouble () const {
    return (*this)->GetDoubleField(*this, *this);
}

template <>
Object::Field<jdouble>::Reference &
Object::Field<jdouble>::Reference::operator = (const jdouble &value) {
    return (*this)->SetDoubleField(*this, *this, value), *this;
}

template <>
Object::Field<jdouble[]>::Reference::operator Array<jdouble> () const {
    return Array<jdouble>(*this, (jdoubleArray)(*this)->GetStaticObjectField(*this, *this));
}

template <>
Object::Field<jdouble[]>::Reference &
Object::Field<jdouble[]>::Reference::operator = (const Array<jdouble> &value) {
    return (*this)->SetStaticObjectField(*this, *this, value), *this;
}

template <>
jdouble Class::Method<jdouble>::Reference::call(va_list vl) const {
    return (*this)->CallNonvirtualDoubleMethodV(*this, *this, *this, vl);
}

template <>
jdouble Object::Method<jdouble>::Reference::call(va_list vl) const {
    return (*this)->CallDoubleMethodV(*this, *this, vl);
}

template <>
Array<jdouble> Class::Method<jdouble[]>::Reference::call(va_list vl) const {
    return Array<jdouble>(*this, (jdoubleArray)(*this)->CallNonvirtualObjectMethodV(*this, *this, *this, vl));
}

template <>
Array<jdouble> Object::Method<jdouble[]>::Reference::call(va_list vl) const {
    return Array<jdouble>(*this, (jdoubleArray)(*this)->CallObjectMethodV(*this, *this, vl));
}

} // namespace JNI
