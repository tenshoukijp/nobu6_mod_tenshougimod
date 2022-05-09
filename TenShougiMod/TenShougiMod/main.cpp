// aaa.cpp : メイン プロジェクト ファイルです。

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

 	// TODO: ここにコードを挿入してください。
	MSG msg;


	TenShougiModBgn();

	// メイン メッセージ ループ:
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