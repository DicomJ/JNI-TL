#include "jni_tl.h"
#include "Object.h"

JNIEXPORT jboolean JNICALL Java_Object_equals
  (JNIEnv *, jobject, jobject) {

    return false;
}

JNIEXPORT jobjectArray JNICALL Java_Application_copyObjects
  (JNIEnv *, jclass, jobjectArray) {
    return 0;
}
