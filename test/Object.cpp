#include "jni_tl.h"
#include "Object.h"

using namespace JNI;

JNIEXPORT jboolean JNICALL Java_Object_equals
  (JNIEnv *, jobject, jobject) {

    return false;
}

JNIEXPORT jobjectArray JNICALL Java_Application_copyObjects
  (JNIEnv *env, jclass, jobjectArray object_) {

    Array<jobject> object(env, object_);
    jsize count = object.length();
    Array<jobject> copy(env, count, Class(env, "Object"));
    Class AnotherObject(env, "AnotherObject");
    Class::Constructor::ID AnotherObjectConstructorID(
                           AnotherObject, Class::Constructor(Args<jstring, jint>()));

    for (jsize i = 0; i < count; ++i) {

        Object o = AnotherObject.newObject(AnotherObjectConstructorID,
                                           0, jint(i));

//        //copy[i] = ;
    }

    return 0;

}
