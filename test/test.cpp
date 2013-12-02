#include "athor/Andriy.Bulka"
#include "license/BSD"

#include "jni_tl.h"
#include "Object.h"

using namespace JNI;

struct Test : Object {
    Test(JNIEnv *env, jobject object)
        : Object(Class(env, jclass("com.test")), object) {}
    void execute();

    template <typename T> void testField();
    template <typename T> void testMethod();
};

template<typename T>
void Test::testField() {
    Class &clazz = *this;
    {
        Static::Field<T> field("static_field_name");
        typename Static::Field<T>::ID fieldID(clazz, field);
        {
            (T)(*this)[field]; (T)(*this)[fieldID];
            (*this)[field] = T(0); (*this)[fieldID] = T(0);
        }{
            (T)clazz[field]; (T)clazz[fieldID];
            clazz[field] = T(0); clazz[fieldID] = T(0);
        }
    }{
        Field<T> field("field_name");
        typename Field<T>::ID fieldID(clazz, field);
        {
            (T)(*this)[field]; (T)(*this)[fieldID];
            (*this)[field] = T(0); (*this)[fieldID] = T(0);
        }
    }
    // Array
    {
        Static::Field<T[]> field("static_field_name");
        typename Static::Field<T[]>::ID fieldID(clazz, field);
        {
            (Array<T>)(*this)[field]; (Array<T>)(*this)[fieldID];
            (*this)[field] = Array<T>(*this, 0); (*this)[fieldID] = Array<T>(*this, 0);
        }{
            (Array<T>)clazz[field]; (Array<T>)clazz[fieldID];
            clazz[field] = Array<T>(*this, 0); clazz[fieldID] = Array<T>(*this, 0);
        }
    }{
        Field<T[]> field("field_name");
        typename Field<T[]>::ID fieldID(clazz, field);
        {
            (Array<T>)(*this)[field]; (Array<T>)(*this)[fieldID];
            (*this)[field] = Array<T>(*this, 0); (*this)[fieldID] = Array<T>(*this, 0);
        }
    }
}

template <typename T>
void Test::testMethod() {
    Class &clazz = *this;

    {
        Static::Method<T> method("method_name", Args<jobject>("lang.String"));
        typename Static::Method<T>::ID methodID(clazz, method);
        {
            (T)(*this)[method](jobject(0));
            (T)(*this)[methodID](jobject(0));
        }{
            (T)clazz[method](jobject(0));
            (T)clazz[methodID](jobject(0));
        }
    }{
        Class::Method<T> method("method_name", Args<jobject>("lang.String"));
        typename Class::Method<T>::ID methodID(clazz, method);
        {
            (T)(*this)[method](jobject(0));
            (T)(*this)[methodID](jobject(0));
        }
    }{
        Method<T> method("method_name", Args<jobject>("lang.String"));
        typename Method<T>::ID methodID(clazz, method);
        {
            (T)(*this)[method](jobject(0));
            (T)(*this)[methodID](jobject(0));
        }
    }
    // Array
    {
        Static::Method<T[]> method("method_name", Args<jobject>("lang.String"));
        typename Static::Method<T[]>::ID methodID(clazz, method);
        {
            (Array<T>)(*this)[method](jobject(0));
            (Array<T>)(*this)[methodID](jobject(0));
        }{
            (Array<T>)clazz[method](jobject(0));
            (Array<T>)clazz[methodID](jobject(0));
        }
    }{
        Class::Method<T[]> method("method_name", Args<jobject>("lang.String"));
        typename Class::Method<T[]>::ID methodID(clazz, method);
        {
            (Array<T>)(*this)[method](jobject(0));
            (Array<T>)(*this)[methodID](jobject(0));
        }
    }{
        Method<T[]> method("method_name", Args<jobject>("lang.String"));
        typename Method<T[]>::ID methodID(clazz, method);
        {
            (Array<T>)(*this)[method](jobject(0));
            (Array<T>)(*this)[methodID](jobject(0));
        }
    }
}

void Test::execute() {

    testField<jint>();
    testMethod<jint>();

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
