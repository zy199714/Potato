#include "GameApp.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	// ��Щ������ʹ��
	UNREFERENCED_PARAMETER(prevInstance);
	UNREFERENCED_PARAMETER(cmdLine);
	UNREFERENCED_PARAMETER(showCmd);
	// ������Debug�汾��������ʱ�ڴ�����й©���
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// �ڴ�й©���
	//_CrtSetBreakAlloc(530280);
#endif

	GameApp theApp(hInstance, 800, 600);

	if (!theApp.Init())
		return 0;

	return theApp.Run();
}




