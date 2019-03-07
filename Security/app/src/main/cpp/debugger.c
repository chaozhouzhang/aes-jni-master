//
// Created by wei on 17-12-5.
//

#include "debugger.h"
#include <stdio.h>
#include "string.h"
#include "logger.h"


int isdebug() {

    //android.mk 编译情况下
#ifdef NDK_DEBUG
    LOGI("is NDK_DEBUG. \n");
       return 1;
#endif
// cmakeList 编译情况下
#ifdef CMAKE_BUILD_TYPE
    LOGI("is CMAKE_BUILD_TYPE. \n");
   return 1;
#endif

    return 0;
}