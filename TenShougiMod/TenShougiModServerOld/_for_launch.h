//---------------------------------------------------------------------------------------------------------------------------------------------
#include <windows.h>

// 天将棋Mod.dllのモジュールハンドル
HMODULE hTenShougiModDll = NULL;


// 開始関数
typedef void (WINAPI *PFNTENSHOUGIMODBGN)();
PFNTENSHOUGIMODBGN p_TenShougiModBgn = NULL;

// 終了関数
typedef void (WINAPI *PFNTENSHOUGIMODEND)();
PFNTENSHOUGIMODEND p_TenShougiModEnd = NULL;

// 天将棋開始
void TenShougiModBgn() {
	hTenShougiModDll = LoadLibrary("TenShougiMod.dll");

	p_TenShougiModBgn = (PFNTENSHOUGIMODBGN)GetProcAddress(hTenShougiModDll, "sgTenShougiModBgn");
	p_TenShougiModEnd = (PFNTENSHOUGIMODBGN)GetProcAddress(hTenShougiModDll, "sgTenShougiModEnd");

	if ( p_TenShougiModBgn ) {
		p_TenShougiModBgn();
	}
}

// 天将棋終了
void TenShougiModEnd() {
	if ( p_TenShougiModEnd ) {
		p_TenShougiModEnd();
	}
	FreeLibrary(hTenShougiModDll);
}

// 天翔記は存在するのか？
BOOL IsExistsTenshouki() {
	return FindWindow("Tenshouki95", NULL) != NULL;
}


int _for_launch(int argc, char* argv[]) {

 	// TODO: ここにコードを挿入してください。
	MSG msg;

	if ( argc==2 && strcmp(argv[1], "launch")==0 ){

		OutputDebugString("LAUNCH");

		if ( !IsExistsTenshouki() ) {
			return 0;
		}

		// 天翔記は存在するので…
		// 天将棋の起動＆開始
		TenShougiModBgn();

		// メイン メッセージ ループ:
		while (GetMessage(&msg, NULL, 0, 0))
		{
			// 天翔記が起動していない…
			if ( !IsExistsTenshouki() ) {
				// 終了
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// 天将棋の終了
		TenShougiModEnd();

		PostQuitMessage(0);

		return TRUE;
	} else {
		OutputDebugString("NO LAUNCH");
	}
	return FALSE;
}
//---------------------------------------------------------------------------------------------------------------------------------------------
