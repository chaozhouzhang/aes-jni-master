//
// Created by wei on 16-12-4.
//

#include <string.h>
#include <android/log.h>
#include <jni.h>
#include "checksignature.h"
#include "aes.h"


/**
 *
 * @param env
 * @param thiz
 * @param context
 * @return
 */
jint check_signature(JNIEnv *env, jobject thiz, jobject context) {
    /**
     * 获取Context上下文类
     */
    jclass context_clazz = (*env)->GetObjectClass(env, context);
    /**
     * 获取getPackageManager方法的方法ID
     */
    jmethodID methodID_getPackageManager = (*env)->GetMethodID(env, context_clazz,
                                                               "getPackageManager",
                                                               "()Landroid/content/pm/PackageManager;");
    /**
     * 获取PackageManager对象
     */
    jobject packageManager = (*env)->CallObjectMethod(env, context, methodID_getPackageManager);

    /**
     * PackageManager的Java类
     */
    jclass pm_clazz = (*env)->GetObjectClass(env, packageManager);
    /**
     * 获取getPackageInfo方法的方法ID
     */
    jmethodID methodID_pm = (*env)->GetMethodID(env, pm_clazz, "getPackageInfo",
                                                "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
    /**
     * 获取getPackageName方法的方法ID
     */
    jmethodID methodID_pack = (*env)->GetMethodID(env, context_clazz,
                                                  "getPackageName", "()Ljava/lang/String;");
    /**
     * 获取当前应用的包名
     */
    jstring application_package = (*env)->CallObjectMethod(env, context,
                                                           methodID_pack);
    /**
     * string转字符串得到包名的char
     */
    const char *package_name = (*env)->GetStringUTFChars(env,
                                                         application_package, 0);
    LOGE("packageName: %s\n", package_name);

    /**
     * 获取PackageInfo
     */
    jobject packageInfo = (*env)->CallObjectMethod(env, packageManager,
                                                   methodID_pm, application_package, 64);

    /**
     * 获取PackageInfo类
     */
    jclass packageInfo_clazz = (*env)->GetObjectClass(env, packageInfo);
    /**
     *
     */
    jfieldID fieldID_signatures = (*env)->GetFieldID(env, packageInfo_clazz, "signatures",
                                                     "[Landroid/content/pm/Signature;");
    /**
     *
     */
    jobjectArray signature_arr = (jobjectArray) (*env)->GetObjectField(env, packageInfo,
                                                                       fieldID_signatures);
    /**
     *Signature数组中取出第一个元素
     */
    jobject signature = (*env)->GetObjectArrayElement(env, signature_arr, 0);
    /**
     *读signature的hashcode
     */
    jclass signature_clazz = (*env)->GetObjectClass(env, signature);
    /**
     *
     */
    jmethodID methodID_hashcode = (*env)->GetMethodID(env, signature_clazz, "hashCode", "()I");
    /**
     *
     */
    jint hashCode = (*env)->CallIntMethod(env, signature, methodID_hashcode);
    LOGE("hashcode: %d\n", hashCode);
    /**
     *
     */
    if (strcmp(package_name, app_packageName) != 0) {
        return -1;
    }
    /**
     *
     */
    if (hashCode != app_signature_hash_code) {
        return -2;
    }
    return 1;
}