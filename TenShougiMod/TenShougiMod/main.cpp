// aaa.cpp : ���C�� �v���W�F�N�g �t�@�C���ł��B

#if 0

#include "TenShougiMod.h"
#include "TenShougiForm.cpp"

using namespace TenShougiMod;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: �����ɃR�[�h��}�����Ă��������B
	MSG msg;


	TenShougiModBgn();

	// ���C�� ���b�Z�[�W ���[�v:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (bTenShougiRunThread)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	TenShougiModEnd();

	return (int) msg.wParam;
}

#endif