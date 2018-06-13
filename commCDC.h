#ifndef _COMMCDC_H__
#define _COMMCDC_H__

/************************************************************************
* 
* 文件名：commCDC.cpp

* 
* 文件描述：电压检测模块 
* 
*备注：模拟量读取采用板载5V为基准电压，由于板载电压不准（低于5V）导致
*读取数值偏大；可以通过设置Aref可以提高精度；
************************************************************************/


/*================================================================ 
* 函 数 1：
* dcVal() 
* 
* 参 数：
* 读取模拟量的引脚
*
* 功 能 描 述: 
* 检测输入电压
* 
* 返 回 值：
* 浮点型 0~25v电压
* 
* 作 者：赫文 2018年5月18日
================================================================*/ 
float dcVal(int voltagePin);

#endif