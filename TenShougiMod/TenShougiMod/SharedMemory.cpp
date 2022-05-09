#include <windows.h>

#include "SharedMemory.h"
#include <string>

HWND    hTenshouWnd			= NULL; // 本体のウィンドウハンドル
HANDLE  hTenshouProcess		= NULL; // 本体のプロセスハンドル
DWORD   dwTenshouProcessID	= NULL; // 本体のプロセスID




// 共有メモリ
#pragma data_seg(".TenShougiModSharedMemory")

namespace SharedMemory {

	char szFontName[64] = ""; // 天翔記で使うこととなったフォント名。
							  //元来の天翔記とは別のポインタを指して指定するかもしれないので、大き目バッファで。

	char szCommand[64] = ""; // 天翔記から受け取るコマンド。CheckTenshougiStatus関数で対処する。

	TBUshouDaniPowerInfo BPower = {NULL};	// 後手の棋力
	TBUshouDaniPowerInfo WPower = {NULL};	// 先手の棋力

	int iCurrentTurnState = 0; // 現在のターンがプレイヤかＣＰＵか。0:プレイヤ, 1:CPU
	int iTenShougiFormShown = 0;
}

#pragma data_seg()




// 外部参照用。天翔記側からフォントを設定する。
void WINAPI sgSetTenShougiFontName(char *pszFontName) {
	strcpy(SharedMemory::szFontName, pszFontName);
}


//  外部参照用。天翔記で今表示しているフォント名を得る。
// 無ければ、ＭＳ 明朝とする。
char *sgGetTenShougiFontName() {
	if ( strlen(SharedMemory::szFontName) > 0 ) {
		return SharedMemory::szFontName;
	} else {
		return "ＭＳ 明朝";
	}
}

// 外部参照用。現在天将棋をプレイしている？
BOOL WINAPI sgIsTenShougiFormShown() {
	return SharedMemory::iTenShougiFormShown;
}


// 天翔記プロセスから天将棋プロセスへと値をコピーする。
void LoadTenshouHandle() {

	// ウィンドウハンドルを取得
	hTenshouWnd = FindWindow("Tenshouki95", NULL);

	if ( !hTenshouWnd ) { return; }

	// プロセスIDを取得
	DWORD _dumm = GetWindowThreadProcessId(hTenshouWnd, &dwTenshouProcessID);

	// プロセスハンドルを取得
	hTenshouProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwTenshouProcessID);
}

// 確保したプロセスハンドルの解放
void FreeTenshouHandle() {

    if(hTenshouProcess){
		// ハンドルを解放
        CloseHandle(hTenshouProcess);
    }

}

// 天翔記→天将棋へのコマンド。文字列で受け取る 
void WINAPI sgSendCommand(char *szCommand) {
	strcpy(SharedMemory::szCommand, szCommand);
}

// 天翔記から送られてきているコマンドを取り出す。
std::string PopCommand() {
	std::string command = std::string(SharedMemory::szCommand);
	SharedMemory::szCommand[0] = NULL;
	return command;
}



// 外部参照用。天翔記側から対局者の棋力を得る。dllexport用
void WINAPI sgSetTenShougiBushouPower(void* pBPower, void* pWPower ) {

	// 各々棋力型ポインタ。強さパワーと段位名が入っている。
	TBUshouDaniPowerInfo* BPower = (TBUshouDaniPowerInfo *)pBPower;
	TBUshouDaniPowerInfo* WPower = (TBUshouDaniPowerInfo *)pWPower;

	// ローカル変数にコピー
	SharedMemory::BPower.iPower = BPower->iPower;
	strcpy( SharedMemory::BPower.szDaniName, BPower->szDaniName);
	SharedMemory::BPower.iBushouID = BPower->iBushouID;


	// ローカル変数にコピー
	SharedMemory::WPower.iPower = WPower->iPower;
	strcpy( SharedMemory::WPower.szDaniName, WPower->szDaniName);
	SharedMemory::WPower.iBushouID = WPower->iBushouID;
}

// 天翔記のプロセスから天将棋のプロセスへと値をコピー
void CopyTenshoukiProcessToTenshougiProcess() {
	// 本体へのプロセス情報が得られていなければどうしようもない
	if ( !hTenshouProcess ) {
		return;
	}


	// 武将情報を全部、天翔記プロセス→天将棋プロセスにコピー
	ReadProcessMemory( hTenshouProcess, (LPCVOID)(GAMEDATASTRUCT_BUSHOU_ADDRESSS), nb6bushou, sizeof(NB6BUSHOU)*GAMEDATASTRUCT_BUSHOU_NUM, NULL);
	ReadProcessMemory( hTenshouProcess, (LPCVOID)(GAMEDATASTRUCT_BUSHOUNAME_ADDRESSS), nb6bushouname, sizeof(NB6BUSHOUNAME)*GAMEDATASTRUCT_BUSHOU_NUM, NULL);
}