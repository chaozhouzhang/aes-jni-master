# NDK


# CMake


问题：不支持armeabi, mips, mips64

```
ERROR: ABIs [armeabi, mips, mips64] are not supported for platform. Supported ABIs are [arm64-v8a, armeabi-v7a, x86, x86_64].
```

解决：将NDK改为android-ndk-r16b版本

```
ndk.dir=/Users/zhangchaozhou/Library/Android/sdk/android-ndk-r16b
```


