#include "athor/Andriy.Bulka"
#include "license/BSD"

#include "jni_tl.h"

using namespace JNI;

struct Test : Object {
    Test(JNIEnv *env, jobject object)
        : Object(Class(env, jclass("com.test")), object) {}
    void execute();
};


void Test::execute() {

    Class &clazz = *this;

    String(*this, "some string");
    String(*this, std::string("some string"));
    String(*this, jstring(String(*this, "some string")));

    clazz.string("some string");
    clazz.string(std::string("some string"));
    clazz.string(jstring(clazz.string("some string")));

    (*this).string("some string");
    (*this).string(std::string("some string"));
    (*this).string(jstring((*this).string("some string")));

    std::string args = Args<jint, jint, jint, jint, jint, jint, jint, jint, jint, jint>();
    args = (const std::string &)Args<jobject, jobject[], jint, jint[]>("lang.String", "lang.Object");

    // Class
    clazz[Class::Field<jint>::ID(clazz, "static_field_name")];
    clazz[Class::Method<jint>::ID(clazz, "static_field_name")];

    (jint) clazz[Class::Field<jint>("static_field_name")];
    (jobject) clazz[Class::Field<jobject>("static_field_name","class_name")];

    clazz[Class::Field<jint>("static_field_name")] = jint(0);
    clazz[Class::Field<jobject>("static_field_name","class_name")] = jobject(0);

    (Array<jint>) clazz[Class::Field<jint[]>("static_array_field_name")];
    (Array<jobject>) clazz[Class::Field<jobject[]>("static_field_name","class_name")];

    clazz[Class::Field<jint[]>("static_array_field_name")] = Array<jint>(0, jintArray(0));
    clazz[Class::Field<jobject[]>("static_field_name","class_name")] = Array<jobject>(0, jobjectArray(0));

    clazz[Class::Method<jint>("static_method_name")];
    clazz[Class::Method<jobject>("static_method_name","class_name")];

    clazz[Class::Method<jint>::ID(clazz, "static_method_name")];
    clazz[Class::Method<jint>::ID(clazz,
          Class::Method<jint>("static_method_name", Args<jobject>("lang.String")))];
    clazz[Class::Method<jobject>::ID(clazz,
          Class::Method<jobject>("static_method_name", Args<jobject>("lang.String"), "lang.String"))];

    clazz[Class::Method<jobject>::ID(clazz,
          Class::Method<jobject>("static_method_name", Args<jobject>("lang.String"), "lang.String"))] (jint(0));

    // Object
    (*this)[Class::Field<jint>::ID(clazz, "static_field_name")];
    (*this)[Class::Method<jint>::ID(clazz, "static_field_name")];

    (jint) (*this)[Class::Field<jint>("static_field_name")];
    (jobject) (*this)[Class::Field<jobject>("static_field_name","class_name")];

    (*this)[Class::Field<jint>("static_field_name")] = jint(0);
    (*this)[Class::Field<jobject>("static_field_name","class_name")] = jobject(0);

    (Array<jint>) (*this)[Class::Field<jint[]>("static_array_field_name")];
    (Array<jobject>) (*this)[Class::Field<jobject[]>("static_field_name","class_name")];

    (*this)[Class::Field<jint[]>("static_array_field_name")] = Array<jint>(0, jintArray(0));
    (*this)[Class::Field<jobject[]>("static_field_name","class_name")] = Array<jobject>(0, jobjectArray(0));

    (*this)[Class::Method<jint>("static_method_name")];
    (*this)[Class::Method<jobject>("static_method_name","class_name")];

    (*this)[Field<jint>::ID(clazz, "field_name")];
    (*this)[Method<jint>::ID(clazz, "method_name")];

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

    (*this)[Method<jint>::ID(clazz, "method_name")];
    (*this)[Method<jint>::ID(clazz,
            Method<jint>("method_name", Args<jobject>("lang.String")))];
    (*this)[Method<jobject>::ID(clazz,
            Method<jobject>("method_name", Args<jobject>("lang.String"), "lang.String"))];

    (*this)[Class::Method<jobject>::ID(clazz,
            Class::Method<jobject>("static_method_name", Args<jobject>("lang.String"), "lang.String"))] (jint(0));

    (*this)[Method<jobject>::ID(clazz,
            Method<jobject>("method_name", Args<jobject>("lang.String"), "lang.String"))] (jint(0));
}

int main() {
    return 0 ? (Test(0, 0).execute(), 0) : 0;
}
