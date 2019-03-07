#include <jni.h>
#include "aes.h"
#include "checksignature.h"
#include "check_emulator.h"
#include <string.h>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

#define CBC 1
#define ECB 1


/**
 * 获取数组的数量大小
 */
# define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))




/**
 * 指定要注册的类，对应完整的java类名
 */
#define JNIREG_CLASS "me/chaozhouzhang/security/AESEncrypt"

const char *UN_SIGN_NATURE = "UN_SIGN_NATURE";


/**
 * 字符转字符串
 * @param envPtr JNI环境
 * @param src 源字符
 * @return
 */
jstring charToJstring(JNIEnv *envPtr, char *src) {
    JNIEnv env = *envPtr;

    jsize len = strlen(src);
    /**
     * String类的Java类
     */
    jclass classString = env->FindClass(envPtr, "java/lang/String");
    /**
     * 加密的字符格式
     */
    jstring strEncode = env->NewStringUTF(envPtr, "UTF-8");
    /**
     * 初始化的方法id
     */
    jmethodID mid = env->GetMethodID(envPtr, classString, "<init>", "([BLjava/lang/String;)V");
    /**
     * byte数组
     */
    jbyteArray barr = env->NewByteArray(envPtr, len);
    /**
     * 设置byte数组的临界区域
     */
    env->SetByteArrayRegion(envPtr, barr, 0, len, (jbyte *) src);

    /**
     * 新建String对象
     */
    return (jstring) env->NewObject(envPtr, classString, mid, barr, strEncode);
}

/**
 * 获取加解密的密钥
 * @return
 */
char *getKey() {
    int n = 0;
    //"NMTIzNDU2Nzg5MGFiY2RlZg";
    char s[23];
    s[n++] = 'N';
    s[n++] = 'M';
    s[n++] = 'T';
    s[n++] = 'I';
    s[n++] = 'z';
    s[n++] = 'N';
    s[n++] = 'D';
    s[n++] = 'U';
    s[n++] = '2';
    s[n++] = 'N';
    s[n++] = 'z';
    s[n++] = 'g';
    s[n++] = '5';
    s[n++] = 'M';
    s[n++] = 'G';
    s[n++] = 'F';
    s[n++] = 'i';
    s[n++] = 'Y';
    s[n++] = '2';
    s[n++] = 'R';
    s[n++] = 'l';
    s[n++] = 'Z';
    s[n++] = 'g';
    char *encode_str = s + 1;
    return b64_decode(encode_str, strlen(encode_str));

}

/**
 * 加密
 * @param env
 * @param instance
 * @param context
 * @param str_
 * @return
 */
JNIEXPORT jstring JNICALL encode(JNIEnv *env, jobject instance, jobject context, jstring str_) {

    /**
     * 先进行apk被二次打包的校验：检验签名和是否是虚拟器
     */
//    if (check_signature(env, instance, context) != 1 || check_is_emulator(env) != 1) {
//        char *str = UN_SIGN_NATURE;
//        return charToJstring(env, str);
//    }

    uint8_t *AES_KEY = (uint8_t *) getKey();
    const char *in = (*env)->GetStringUTFChars(env, str_, JNI_FALSE);
    char *baseResult = AES_128_ECB_PKCS5Padding_Encrypt(in, AES_KEY);
    (*env)->ReleaseStringUTFChars(env, str_, in);
    return (*env)->NewStringUTF(env, baseResult);
}


/**
 * 解密
 * @param env
 * @param instance
 * @param context
 * @param str_
 * @return
 */
JNIEXPORT jstring JNICALL decode(JNIEnv *env, jobject instance, jobject context, jstring str_) {

    /**
     *先进行apk被二次打包的校验
     */
//    if (check_signature(env, instance, context) != 1 || check_is_emulator(env) != 1) {
//        char *str = UN_SIGN_NATURE;
//        return charToJstring(env, str);
//    }

    uint8_t *AES_KEY = (uint8_t *) getKey();
    const char *str = (*env)->GetStringUTFChars(env, str_, JNI_FALSE);
    char *desResult = AES_128_ECB_PKCS5Padding_Decrypt(str, AES_KEY);
    (*env)->ReleaseStringUTFChars(env, str_, str);
    /**
     * 不用系统自带的方法NewStringUTF是因为如果desResult是乱码，会抛出异常
     * return (*env)->NewStringUTF(env, desResult);
     */
    return charToJstring(env, desResult);
}


/**
 * if return 1 ,is check pass.
 */
JNIEXPORT jint JNICALL
check_jni(JNIEnv *env, jobject instance, jobject con) {
    return check_signature(env, instance, con);
}


/**
 * Java和JNI函数的绑定表
 */
static JNINativeMethod method_table[] = {
        {"checkSignature", "(Ljava/lang/Object;)I",                                    (void *) check_jni},
        {"decode",         "(Ljava/lang/Object;Ljava/lang/String;)Ljava/lang/String;", (void *) decode},
        {"encode",         "(Ljava/lang/Object;Ljava/lang/String;)Ljava/lang/String;", (void *) encode},
};

/**
 * 注册native方法到java中
 * @param env jni环境
 * @param className 类名
 * @param gMethods 方法列表
 * @param numMethods 方法数
 * @return
 */
static int registerNativeMethods(JNIEnv *env, const char *className, JNINativeMethod *gMethods,
                                 int numMethods) {
    /**
     * 初始化
     */
    jclass clazz;
    /**
     * 通过JNI环境和类名获得Java类
     */
    clazz = (*env)->FindClass(env, className);
    if (clazz == NULL) {
        return JNI_FALSE;
    }
    /**
     * 通过JNI环境、Java类、方法列表、方法数进行本地方法的注册
     */
    if ((*env)->RegisterNatives(env, clazz, gMethods, numMethods) < 0) {
        return JNI_FALSE;
    }

    return JNI_TRUE;
}



/**
 *
 * JNI加载入口
 *
 * @param vm Java虚拟机
 * @param reserved 保留的，预订的
 * @return
 */
JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    /**
     * 初始化
     */
    JNIEnv *env = NULL;
    jint result = -1;



    /**
     * 从JavaVM获取JNIEnv，一般使用1.4的版本
     */
    if ((*vm)->GetEnv(vm, (void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        return result;
    }

    /**
     * 注册本地方法，传入JNI环境，类名，方法列表，方法数量
     */
    registerNativeMethods(env, JNIREG_CLASS,
                          method_table, NELEM(method_table));
    /**
     * 返回jni的版本
     */
    return JNI_VERSION_1_4;
}

