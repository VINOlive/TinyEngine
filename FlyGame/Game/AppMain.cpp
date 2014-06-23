#include "FlyGame.h"
int WINAPI WinMain(HINSTANCE h_instance, HINSTANCE hPrevInstance, 
				   PSTR szCmdLine, int iCmdShow)
{
	LPCTSTR WinTitle = L"·É»ú´óÕ½V1.0";
	FlyGame* mygame = new FlyGame(h_instance,WIN_CLASS,WinTitle,NULL,NULL,400,600);
	T_Engine::pEngine = mygame;
	mygame->SetFrame(25);
	mygame->StartEngine();
	return TRUE;
}