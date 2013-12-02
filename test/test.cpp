#include "athor/Andriy.Bulka"
#include "license/BSD"

#include "jni_tl.h"
#include "Object.h"

using namespace JNI;

struct Test : Object {
    Test(JNIEnv *env, jobject object)
        : Object(Class(env, jclass("com.test")), object) {}
    void execute();
};


void Test::execute() {

    Class &clazz = *this;

    (std::string)String(*this, "some string");
    (std::string)String(*this, std::string("some string"));
    (std::string)String(*this, jstring(String(*this, "some string")));

    (std::string)clazz.string("some string");
    (std::string)clazz.string(std::string("some string"));
    (std::string)clazz.string(jstring(clazz.string("some string")));

    (*this).string("some string");
    (*this).string(std::string("some string"));
    (*this).string(jstring((*this).string("some string")));

    std::string args = Args<jint, jint, jint, jint, jint, jint, jint, jint, jint, jint>();
    args = (const std::string &)Args<jobject, jobject[], jint, jint[]>("lang.String", "lang.Object");

    // Class
    clazz[Static::Field<jint>::ID(clazz, "static_field_name")];
    clazz[Static::Method<jint>::ID(clazz, "static_field_name")];

    (jint) clazz[Static::Field<jint>("static_field_name")];
    (jobject) clazz[Static::Field<jobject>("static_field_name","class_name")];

    clazz[Static::Field<jint>("static_field_name")] = jint(0);
    clazz[Static::Field<jobject>("static_field_name","class_name")] = jobject(0);

    (Array<jint>) clazz[Static::Field<jint[]>("static_array_field_name")];
    (Array<jobject>) clazz[Static::Field<jobject[]>("static_field_name","class_name")];

    clazz[Static::Field<jint[]>("static_array_field_name")] = Array<jint>(0, jintArray(0));
    clazz[Static::Field<jobject[]>("static_field_name","class_name")] = Array<jobject>(0, jobjectArray(0));

    clazz[Static::Method<jint>("static_method_name")];
    clazz[Static::Method<jobject>("static_method_name","class_name")];

    clazz[Static::Method<jint>::ID(clazz, "static_method_name")];
    clazz[Static::Method<jint>::ID(clazz,
          Static::Method<jint>("static_method_name", Args<jobject>("lang.String")))];
    clazz[Static::Method<jobject>::ID(clazz,
          Static::Method<jobject>("static_method_name", Args<jobject>("lang.String"), "lang.String"))];

    clazz[Static::Method<jobject>::ID(clazz,
          Static::Method<jobject>("static_method_name", Args<jobject>("lang.String"), "lang.String"))] (jint(0));

    // Object
    (*this)[Static::Field<jint>::ID(clazz, "static_field_name")];
    (*this)[Static::Method<jint>::ID(clazz, "static_field_name")];
    (*this)[Class::Method<jint>::ID(clazz, "method_name")];

    (jint) (*this)[Static::Field<jint>("static_field_name")];
    (jobject) (*this)[Static::Field<jobject>("static_field_name","class_name")];

    (*this)[Static::Field<jint>("static_field_name")] = jint(0);
    (*this)[Static::Field<jobject>("static_field_name","class_name")] = jobject(0);

    (Array<jint>) (*this)[Static::Field<jint[]>("static_array_field_name")];
    (Array<jobject>) (*this)[Static::Field<jobject[]>("static_field_name","class_name")];

    (*this)[Static::Field<jint[]>("static_array_field_name")] = Array<jint>(0, jintArray(0));
    (*this)[Static::Field<jobject[]>("static_field_name","class_name")] = Array<jobject>(0, jobjectArray(0));

    (*this)[Static::Method<jint>("static_method_name")];
    (*this)[Static::Method<jobject>("static_method_name","class_name")];
    (*this)[Class::Method<jobject>("method_name","class_name")];

    (*this)[Field<jint>::ID(clazz, "field_name")];
    (*this)[Method<jint>::ID(clazz, "method_name")];
    (*this)[Class::Method<jint>::ID(clazz, "method_name")];

    (jint) (*this)[Field<jint>("field_name")];
    (jobject) (*this)[Field<jobject>("field_name","class_name")];

    (*this)[Field<jint>("field_name")] = jint(0);
    (*this)[Field<jobject>("field_name","class_name")] = jobject(0);

    (Array<jint>) (*this)[Field<jint[]>("array field_name")];
    (Array<jobject>) (*this)[Field<jobject[]>("field_name","Object_name")];

    (*this)[Field<jint[]>("array field_name")] = Array<jint>(0, jintArray(0));
    (*this)[Field<jobject[]>("field_name","Object_name")] = Array<jobject>(0, jobjectArray(0));

    (*this)[Method<jint>("static_method_name")];
    (*this)[Method<jobject>("static_method_name","class_name")];
    (*this)[Class::Method<jobject>("static_method_name","class_name")];

    (*this)[Method<jint>::ID(clazz, "method_name")];
    (*this)[Method<jint>::ID(clazz,
            Method<jint>("method_name", Args<jobject>("lang.String")))];
    (*this)[Method<jobject>::ID(clazz,
            Method<jobject>("method_name", Args<jobject>("lang.String"), "lang.String"))];

    (*this)[Static::Method<jobject>::ID(clazz,
            Static::Method<jobject>("static_method_name", Args<jobject>("lang.String"), "lang.String"))] (jint(0));
    (*this)[Class::Method<jobject>::ID(clazz,
            Class::Method<jobject>("method_name", Args<jobject>("lang.String"), "lang.String"))] (jint(0));
    (*this)[Method<jobject>::ID(clazz,
            Method<jobject>("method_name", Args<jobject>("lang.String"), "lang.String"))] (jint(0));
}

JNIEXPORT void JNICALL Java_Application_test(JNIEnv *env, jclass clazz) {
    if (false) Test(0, 0).execute();

    {
        (Array<jobject>)(Array<jobject[]>(env, 10, clazz, 0)[Region(3)][0]);
        Array<jobject[]>(env, 10, clazz, 0)[Region(3)][0] = Array<jobject>(env, 10, clazz, 0);

        (Array<jint>)(Array<jint[]>(env, 10)[Region(3)][0]);
        Array<jint[]>(env, 10)[Region(3)][0] = Array<jint>(env, 10);


        // ...
    }
}

int main() {
    return Java_Application_test(0, 0), 0;
}
