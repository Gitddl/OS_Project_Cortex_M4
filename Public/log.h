/*************************************************************************************
 * 文件名：log.c
 * 描述：用于输出调试信息
 * 作者：DongDL
 * 创建时间：2021年10月02日
*************************************************************************************/

#ifndef __LOG_H
#define __LOG_H

#include "common.h"

#define LOG_SWITCH		MACR_ON


#if (LOG_SWITCH == MACR_ON)
	#define LOG()			LOG_Printf
#else
	#define LOG(...)
#endif

void LOG_Printf( const char* format, ...);

#endif
