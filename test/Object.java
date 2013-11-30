
class Object {

    public Object(String name, int size) {
        this.name = name; this.size = size;
    }

    public String getName() { return name + " " + size; }
    public void setName(String name) { this.name = name; }

    public int getSize() { return size; }
    public void setSize(int size) { this.size = size; }

    String name;
    int size;

    public native boolean equals(Object object);

    public static String className() { return Object.class.getName(); }
    public static final String className = "final " +className();
}

class AnotherObject extends Object {

    public AnotherObject(String name, int size) {
        super(name, size);
    }

    public String getName() { return "Another " + name; }

    public static String className() { return AnotherObject.class.getName(); }
    public static final String className = "final " +className();
}

class Application {

    static {
        System.loadLibrary("JNI_TL");
    }

    static native void test();

    static native Object[] copyObjects(Object[] objects);
    static Object[] createObjects(int count) {
        AnotherObject[] object = new AnotherObject[count];
        for (int i = 0; i < count; ++i) { object[i] = new AnotherObject("object", i); }
        return object;
    }

    public static void main(String [] args) {
        test();
//        final int count = 10;
//        Object[] object = createObjects(count);
//        Object[] copy = copyObjects(object);
//        for (int i = 0; i < count; ++i) {
//            object[i].equals(copy[i]);
//        }
    }
}
