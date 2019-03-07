//
// Created by wei on 16-12-4.
//

#include <jni.h>

#define LOG_TAG "native_log"

#ifndef AES_JNI_ENCRYPT_SIGN_NATURE_CHECK_H
#define AES_JNI_ENCRYPT_SIGN_NATURE_CHECK_H


/**
 * 合法的APP包名
 */
static const char *app_packageName = "me.chaozhouzhang.security";
/**
 * 合法的hashcode，-625644214:这个值是我生成的这个可以store文件的hash值，怎么生成？
 */
static const int app_signature_hash_code = -625644214;

/**
 * 校验APP 包名和签名是否合法
 *
 * 返回值为1 表示合法
 */
jint check_signature(JNIEnv *env, jobject thiz, jobject context);

#endif
