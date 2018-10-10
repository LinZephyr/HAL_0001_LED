#include <stdio.h>
#include <jni.h> /* /usr/lib/jvm/jdk1.6.0_43/include/ */
#include <stdlib.h>

#include <android/log.h> /* liblog */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>


//添加上面的头文件后可以使用下面这个函数来打印
//__android_log_print(ANDROID_LOG_DEBUG, "JNIDemo", "native add ...");

static jint fd;

jint ledOpen(JNIEnv *env, jobject cls)
{
  fd = open("/dev/zf_led", O_RDWR);//可读可写的方式打开
   __android_log_print(ANDROID_LOG_DEBUG, "LEDDemo", "native ledOpen : %d", fd);
  if(fd >= 0)
    return 0;

  else
    return -1;

}

void ledClose(JNIEnv *env, jobject cls)
{
  __android_log_print(ANDROID_LOG_DEBUG, "LEDDemo", "native ledClose...");
  close(fd);
}


jint ledCtrl(JNIEnv *env, jobject cls, jint which, jint status)
{
  __android_log_print(ANDROID_LOG_DEBUG, "LEDDemo", "fd_sta : %d", fd);

  int ret = ioctl(fd, which, status);//根据传入的参数控制驱动

  __android_log_print(ANDROID_LOG_DEBUG, "LEDDemo", "native ledCtrl : %d, %d, %d", which, status, ret);

  return ret;

}



/* 定义JNI字段描述符 */
static const JNINativeMethod methods[] = {//定义一个映射数组
    {"ledOpen", "()I", (void *)ledOpen},//对应c语言的ledOpen,这个函数没有参数,返回值是int类型
    {"ledClose", "()V", (void *)ledClose},//对应c语言的ledClose,这个函数没有参数,返回值是void类型
    {"ledCtrl", "(II)I", (void *)ledCtrl},//对应c语言的ledCtrl,这个函数有两个int参数,返回值是int类型
};



/* System.loadLibrary */
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *jvm, void *reserved)
{
  JNIEnv *env;
  jclass cls;

  /* 获得一个运行环境,JNI版本号为1.4 */
  if ((*jvm)->GetEnv(jvm, (void **)&env, JNI_VERSION_1_4)) {
    return JNI_ERR; /* JNI version not supported */
  }
  cls = (*env)->FindClass(env, "com/example/cfinvo/hardlibrary/HardControl");//查找路径下对应的类

  if (cls == NULL) {
    return JNI_ERR;
  }

  /* 注册native方法 */
  if((*env)->RegisterNatives(env, cls, methods, sizeof(methods)/sizeof(methods[0])) < 0)//把methods数组注册到这个环境里面的这个cls类里
  {
    return JNI_ERR;
  }
  return JNI_VERSION_1_4;//返回版本号
}