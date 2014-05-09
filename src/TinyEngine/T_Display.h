/* ========================================================== 
*	�� �� ����T_Display.h
*	��ǰ�汾��1.0.0
*	����ʱ�䣺2014-03-14 
*	�޸�ʱ�䣺2014-03-14
*	����˵������Ϸ��ʾ�ඨ��
*	���뻷����VS2008
*
*	��    ��������ʵ��111��
*	��    ��������
*	ѧ    �ţ�8000611063
*	ʵ�����ƣ�
*	ʵ����ţ�ʵ��3
*	ʵ��ص㣺ͼ���406
*	ʵ��ʱ�䣺2014-03-14 16:10-18:00
 ==========================================================*/
#pragma once
#include <Windows.h> 

class T_Display 
{
public:
	T_Display();
	~T_Display();

	void SaveMode();
	void ResetMode();
	bool ChangeMode(int width, int height);

private:
	DEVMODE	devmode_saved;
	bool mode_changed;
} ;