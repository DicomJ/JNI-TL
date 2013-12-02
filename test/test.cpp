#include "athor/Andriy.Bulka"
#include "license/BSD"

#include "jni_tl.h"
#include "Object.h"

using namespace JNI;

struct Test : Object {
    Test(JNIEnv *env, jobject object)
        : Object(Class(env, jclass("com.test")), object) {}
    void execute();
    void testString();
    void testArray();
    template <typename T> void testField();
    template <typename T> void testMethod();
};

void Test::testString() {

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
}

void Test::testArray() {
    Class &clazz = *this;
//    {
//        (Array<jobject>)(Array<jobject[]>(*this, 10, clazz, 0)[Region(3)][0]);
//        Array<jobject[]>(*this, 10, clazz, 0)[Region(3)][0] = Array<jobject>(*this, 10, clazz, 0);

//        (Array<jint>)(Array<jint[]>(*this, 10)[Region(3)][0]);
//        Array<jint[]>(*this, 10)[Region(3)][0] = Array<jint>(*this, 10);
//    }
}

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
        std::string args = Args<jint, jint, jint, jint, jint, jint, jint, jint, jint, jint>();
        args = (const std::string &)Args<jobject, jobject[], jint, jint[]>("lang.String", "lang.Object");
    }

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
    if (false) testString();
    if (false) testField<jint>();
    if (false) testMethod<jint>();
}

JNIEXPORT void JNICALL Java_Application_test(JNIEnv *env, jclass) {
    Test(env, 0).execute();
}

int main() {
    return Java_Application_test(0, 0), 0;
}
