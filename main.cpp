#include <Windows.h>
#include <Device/WindowApp.h>
#include "MainGame.h"
#include <crtdbg.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	MainGame* testGame = new MainGame();
	WindowApp::GetInstance().Run(testGame);
}

