//*******************************************************************
// TinyEngine����  
// ����: ����
// ����: http://fangjie.sinaapp.com
// ����: 2014-4-17
// ��Ȩ���� 2014-  ����
// (C) 2014- JayFang All Rights Reserved
//*******************************************************************

#pragma once
#include "T_Config.h"

class T_Util
{
public:

	// �������������������ָ������㡢�յ�������ƶ��ٶ�
	// ���Ҫ��������ֱ���ƶ��ҳ����ٶ�Ϊָ���ƶ��ٶ�
	// ������������̱߶�Ӧ�ķ����ٶ�
	// xRatio��yRatioΪ�������, �����ռ�����
	// ���������Զ��жϷ���
	//	xRatio��yRatioΪ��ֵ����ʾ���ҡ����ƶ�
	//	xRatio��yRatioΪ��ֵ����ʾ�������ƶ�
	static void GetBevelSpeed(
		IN POINT startPT, IN POINT destPT, IN int Speed, 
		OUT int& xRatio, OUT int& yRatio
	);
};