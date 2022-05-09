#define _CRT_SECURE_NO_WARNINGS


// ボナンザとのパイプ関連を中心とした処理
// TenShougiModBgn() と TenShougiModEnd() は、DLL EXPORTS されている
// 

#include <windows.h>
#include "TenShougiPipe.h"
#include "TenShougiForm.h"
#include "TenShougiSound.h"
#include "TenShougiYesNoForm.h"
#include "SharedMemory.h"


using namespace System::Text;
using namespace System::Text::RegularExpressions;

using namespace TenShougiMod;

#pragma comment(lib, "user32.lib")


void DisplayError(char *pszMessage);

// 起動時のプロセス生成
void PrepAndLaunchRedirectedChild(HANDLE hChildStdOut, HANDLE hChildStdIn, HANDLE hChildStdErr);

// ボナンザが標準出力しているものをパイプ経由で読み込み
DWORD WINAPI ReadAndHandleOutput(LPVOID lpvThreadParam);
//DWORD WINAPI GetAndSendInputThread(LPVOID lpvThreadParam);
DWORD WINAPI CheckTenshougiStatus(LPVOID lpvThreadParam);

// このアプリ内からパイプ経由でボナンザに書き込み
BOOL GetAndSendInputThread(String^ text);

static HANDLE hChildProcess = NULL;
static HANDLE hStdIn = NULL; // Handle to parents std input.

// スレッドを(実行する／終了する)　のためのフラグ
BOOL bTenShougiRunThread = FALSE;

static HANDLE hThread1 = NULL;
static HANDLE hThread2 = NULL;
static HANDLE hThread3 = NULL;

CRITICAL_SECTION cs; // スレッドが存在するのでクリティカルセクション(排他制御)

static HANDLE hOutputReadTmp, hOutputRead, hOutputWrite;
static HANDLE hInputWriteTmp, hInputRead, hInputWrite;

vector<string> bufBonanzaAllOutput; // ボナンザの標準出力全てを配列で持ったもの
vector<string> bufAllKifu;			// プレイヤとボナンザの棋譜
std::string bufPlayerKifuLastest;		// プレイヤーが最後に入力した棋譜

double fCurKeiseiPoint = 0;				// 先手後手、どちらが有利か。300ぐらいで絶望的。先手が+ 後手が-



static void OnBgnBonanzaOption();
static void OnEndBonanzaOption();

extern void OpenWindowAndNewTenShougiGame();




// 天将棋開始
void WINAPI sgTenShougiModBgn() {
	// 最初にハンドルを確保すること。
	LoadTenshouHandle();

	// 次に本来の処理
	HANDLE hErrorWrite;
	DWORD ThreadId1 = 0;
	DWORD ThreadId2 = 0;
	DWORD ThreadId3 = 0;
	SECURITY_ATTRIBUTES sa;


	// Set up the security attributes struct.
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;


	// Create the child output pipe.
	if (!CreatePipe(&hOutputReadTmp, &hOutputWrite, &sa, 0))
		DisplayError("CreatePipe");


	// Create a duplicate of the output write handle for the std error
	// write handle. This is necessary in case the child application
	// closes one of its std output handles.
	if (!DuplicateHandle(GetCurrentProcess(), hOutputWrite,
		GetCurrentProcess(), &hErrorWrite, 0,
		TRUE, DUPLICATE_SAME_ACCESS))
		DisplayError("DuplicateHandle");


	// Create the child input pipe.
	if (!CreatePipe(&hInputRead, &hInputWriteTmp, &sa, 0))
		DisplayError("CreatePipe");


	// Create new output read handle and the input write handles. Set
	// the Properties to FALSE. Otherwise, the child inherits the
	// properties and, as a result, non-closeable handles to the pipes
	// are created.
	if (!DuplicateHandle(GetCurrentProcess(), hOutputReadTmp,
		GetCurrentProcess(),
		&hOutputRead, // Address of new handle.
		0, FALSE, // Make it uninheritable.
		DUPLICATE_SAME_ACCESS))
		DisplayError("DupliateHandle");

	if (!DuplicateHandle(GetCurrentProcess(), hInputWriteTmp,
		GetCurrentProcess(),
		&hInputWrite, // Address of new handle.
		0, FALSE, // Make it uninheritable.
		DUPLICATE_SAME_ACCESS))
		DisplayError("DupliateHandle");


	// Close inheritable copies of the handles you do not want to be
	// inherited.
	if (!CloseHandle(hOutputReadTmp)) DisplayError("CloseHandle");
	if (!CloseHandle(hInputWriteTmp)) DisplayError("CloseHandle");


	// Get std input handle so you can close it and force the ReadFile to
	// fail when you want the input thread to exit.
	if ((hStdIn = GetStdHandle(STD_INPUT_HANDLE)) ==
		INVALID_HANDLE_VALUE)
		DisplayError("GetStdHandle");

	PrepAndLaunchRedirectedChild(hOutputWrite, hInputRead, hErrorWrite);


	// Close pipe handles (do not continue to modify the parent).
	// You need to make sure that no handles to the write end of the
	// output pipe are maintained in this process or else the pipe will
	// not close when the child process exits and the ReadFile will hang.
	if (!CloseHandle(hOutputWrite)) DisplayError("CloseHandle");
	if (!CloseHandle(hInputRead)) DisplayError("CloseHandle");
	if (!CloseHandle(hErrorWrite)) DisplayError("CloseHandle");

	bTenShougiRunThread = TRUE; // 天将棋開始

	//クリティカルセクションを作成  071
	InitializeCriticalSection(&cs);  

	/*
	// Launch the thread that gets the input and sends it to the child.
	hThread1 = CreateThread(NULL, 0, GetAndSendInputThread,
		(LPVOID)hInputWrite, 0, &ThreadId1);
	if (hThread1 == NULL) DisplayError("CreateThread");
	*/

	// Read the child's output.
	// ReadAndHandleOutput(hOutputRead);
	hThread2 = CreateThread(NULL, 0, ReadAndHandleOutput,
		(LPVOID)hOutputRead, 0, &ThreadId2);
	if (hThread2 == NULL) DisplayError("CreateThread");


	// 天将棋が何をするべき？
	hThread3 = CreateThread(NULL, 0, CheckTenshougiStatus,
		(LPVOID)NULL, 0, &ThreadId3);
	if (hThread3 == NULL) DisplayError("CreateThread");


	OpenWindowAndNewTenShougiGame();
}


void OpenWindowAndNewTenShougiGame() {

	// 将棋盤の初期化
	TenShougiBoard::BoardAllInitialize();

	//--------------------------------------------------------------------------------
	TenShougiForm::tsgForm	 = gcnew TenShougiForm();
	TenShougiForm::tsgForm->Show();

	// ダミーで音を再生する。音をキャッシュさせるため。
	TenShougiForm::tsgForm->PlayKomaSound("DummySound");

	// ゲームを開始する。
	ReNewTenShougiGame();
	//--------------------------------------------------------------------------------
}

void ReOpenWindowAndNewTenShougiGame() {

	// 将棋盤の初期化
	TenShougiBoard::BoardAllInitialize();

	//--------------------------------------------------------------------------------
	TenShougiForm::tsgForm->Show();

	// ダミーで音を再生する。音をキャッシュさせるため。
	TenShougiForm::tsgForm->PlayKomaSound("DummySound");

	// ゲームを開始する。
	ReNewTenShougiGame();
	//--------------------------------------------------------------------------------
}


void OnBgnBonanzaOption() {
	GetAndSendInputThread("new"); // Bonanzaに暇な時勝手に予測しないように言う

	GetAndSendInputThread("ponder off"); // Bonanzaに暇な時勝手に予測しないように言う

	GetAndSendInputThread("resign 9000"); // どのぐらいで投了するか？


	if ( SharedMemory::WPower.iPower != -1 ) { // 何か後手の棋力が設定されているならば
		// limit depth 10 などの設定
		String^ szDepthCommand = String::Format( "limit depth {0}", max(SharedMemory::WPower.iPower, 1) ); // 元値が3～10ぐらいなので、デフォルトは10
		GetAndSendInputThread(szDepthCommand);

		// limit node 3 などの設定
		String^ szNodeCommand = String::Format( "limit nodes {0}", max( int(SharedMemory::WPower.iPower/3), 1)); // 1～3になるように。デフォルトは３
		GetAndSendInputThread(szNodeCommand); 

		String^ szTimeCommand = String::Format( "limit time 0 {0}", int(SharedMemory::WPower.iPower/3) ); // 0～3になるように。デフォルトは３
		GetAndSendInputThread(szTimeCommand);
	} else {
		GetAndSendInputThread("limit time 0 3"); // CPUの持ち時間は0分。一手3秒以内。
	}

	GetAndSendInputThread("limit time strict"); // 時間制限厳守
}

void ReNewTenShougiGame() {
	// 将棋盤の初期化
	TenShougiBoard::BoardAllInitialize();

	// ボナンザに設定を送信
	OnBgnBonanzaOption();

	//--------------------------------------------- C R I T I C A L  S E C T I O N  B E G I N -----------------------------------------------------
	// クリティカルセクション開始。他のクリティカルセクションを持つスレッドはクリティカルセクション内に書き込み出来ない
	EnterCriticalSection(&cs);

	bufBonanzaAllOutput.clear();		// 全ての記譜配列のクリア
	bufAllKifu.clear();					// プレイヤとボナンザの棋譜
	bufPlayerKifuLastest = "";			// プレイヤーが最後に入力した棋譜

	TenShougiBoard::SetCurBTurn();		// プレイヤーの手番

	// クリティカルセクション終了。
	LeaveCriticalSection(&cs);
	//--------------------------------------------- C R I T I C A L  S E C T I O N  E N D -----------------------------------------------------

	// ボードの表示を更新する。
	TenShougiForm::tsgForm->UpdatePositionByBoardSlot();
}



// TSModからのコマンドを受けて、いろいろと処理を行う。
DWORD WINAPI CheckTenshougiStatus(LPVOID lpvThreadParam) {
	while (bTenShougiRunThread)
	{
		// TSModからのコマンドを取り出す
		string command = PopCommand();

		if ( command == "open_window_and_new_game" ) {
			ReOpenWindowAndNewTenShougiGame();
		}
		else if ( command == "open_window_and_continue_game" ) {
			ReOpenWindowAndNewTenShougiGame();
			LoadCSAKifu(); // 時間制限厳守
		}
		else if ( command == "new_game" ) {
			ReNewTenShougiGame();
		}
		else if ( command == "continue_game" ) {
			ReNewTenShougiGame();
			LoadCSAKifu(); // 時間制限厳守
		}
		// フォームを閉じてしまえという命令（本当に閉じるとバグるのでHide()
		else if ( command == "hide_window" ) {
			TenShougiForm::tsgForm->Hide();
			// 見えなくして、投了する。
			GetAndSendInputThread("resign");
		}
		System::Threading::Thread::Sleep(300);
	}

	return 1; // スレッド終了
}


static void OnEndBonanzaOption() {
	// Bonanzaに終了命令を送る
	GetAndSendInputThread( "ping" );
}




// 天将棋終了
void WINAPI sgTenShougiModEnd() {

	//--------------------------------------------------------------------------------
	// Tell the thread to exit and wait for thread to die.

	// ボナンザに自らを終了させる。
	OnEndBonanzaOption();

	// GUIを終了
	TenShougiForm::tsgForm->Close();
	//--------------------------------------------------------------------------------


	// Redirection is complete

	// Force the read on the input to return by closing the stdin handle.
	if (hStdIn) {
		if (!CloseHandle(hStdIn)) DisplayError("StdIn CloseHandle");
	}

	/*
	if (WaitForSingleObject(hThread1, INFINITE) == WAIT_FAILED)
		DisplayError("WaitForSingleObject");
	*/

	// bTenShougiRunThreadがfalseになるまでちょっと待つ。

	MSG msg;
	// メイン メッセージ ループ:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		// スレッド終了へ向かっていないならば、待つ。
		// pingを打ったことで、bonanzaがpongと返すと、全体の終了が始まる。
		if (bTenShougiRunThread)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

		// スレッドは終了へと向かった、抜ける。
		} else {
			break;
		}
	}

	// 各スレッドが最後まで実行されるのを待ち、最後まで実行されたら、スレッドを閉じる。
	if (WaitForSingleObject(hThread3, INFINITE) == WAIT_FAILED) {
		DisplayError("WaitForSingleObject");
	}
	if (WaitForSingleObject(hThread2, INFINITE) == WAIT_FAILED) {
		DisplayError("WaitForSingleObject");
	}

	// ReadLine等、制御が返ってこない関数で、ファイルハンドル使用中であっても、
	// 全部キャンセルするための関数。
	CancelIo( hInputRead );
	CancelIo( hOutputRead );
	CancelIo( hStdIn );

	// 開いていたパイプを閉じる
	if (hOutputRead) {
		if (!CloseHandle(hOutputRead)) DisplayError("OutputRead CloseHandle");
	}
	if (hInputWrite) {
		if (!CloseHandle(hInputWrite)) DisplayError("InputWrite CloseHandle");
	}

	//クリティカルセクションを削除
	DeleteCriticalSection(&cs);

	// ガベージコレクト
	GC::Collect();

	// 最後に本体へのハンドルを解放する。
	FreeTenshouHandle();

}


// プロセスの構築。コマンドラインは「TenShougiModServer.exe csa_shogi」。バージョンは4.1.3
// 中身はボナンザなので、最新のBonanza.exe を TenShougiModServer.exeとリネームしても使える。
/////////////////////////////////////////////////////////////////////// 
// PrepAndLaunchRedirectedChild
// Sets up STARTUPINFO structure, and launches redirected child.
/////////////////////////////////////////////////////////////////////// 
void PrepAndLaunchRedirectedChild(HANDLE hChildStdOut,
	HANDLE hChildStdIn,
	HANDLE hChildStdErr)
{
	PROCESS_INFORMATION pi;
	STARTUPINFO si;

	// Set up the start up info struct.
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdOutput = hChildStdOut;
	si.hStdInput = hChildStdIn;
	si.hStdError = hChildStdErr;
	si.wShowWindow = SW_HIDE; // 非表示で起動
	// Use this if you want to hide the child:
	//     si.wShowWindow = SW_HIDE;
	// Note that dwFlags must include STARTF_USESHOWWINDOW if you want to
	// use the wShowWindow flags.


	// Launch the process that you want to redirect (in this case,
	// Child.exe). Make sure Child.exe is in the same directory as
	// redirect.c launch redirect from a command line to prevent location
	// confusion.
	//																				┌ Windowは表示しない
	if (!CreateProcess(NULL, "TenShougiModServer.exe csa_shogi", NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
		DisplayError("CreateProcess");


	// Set global child process handle to cause threads to exit.
	hChildProcess = pi.hProcess;


	// Close any unnecessary handles.
	if (!CloseHandle(pi.hThread)) DisplayError("CloseHandle");
}

void AddPlayerKifu() {
	// プレイヤの最後の手が残ってるなら、プレイヤの手は有効だった。
	// 差し手を反映する。
	if ( bufPlayerKifuLastest != "" ) {
		bufAllKifu.push_back("+"+bufPlayerKifuLastest); // ボナンザが反応したので、プレイヤの最後の手を保存。
		// 駒情報を更新する
		TenShougiBoard::MoveBoardSlot(bufPlayerKifuLastest, '+');

		// 駒音を鳴らす。
		TenShougiForm::tsgForm->PlayKomaSound(bufPlayerKifuLastest);

		// プレイヤが入力した最後の手は除去
		bufPlayerKifuLastest = "";

	}
}

// ボナンザから読み取った情報を、行ごとに保存したり、分析して、各変数に保存したり。
bool AnalyzeBonanzaOutputAndSaveToVariableInCritialSection(string& bufCurBonanzaLine, char *lpBuffer) {

	bool bMustBreakLoop = false;

	// １行ラインに立し込み
	bufCurBonanzaLine += lpBuffer;

	char *pBufCurBonanzaLine = (char *)bufCurBonanzaLine.c_str();

	// １行のラインのバッファーの中に\r\nが形成された
	char *lpFind = strstr(pBufCurBonanzaLine, "\r\n");
	if (lpFind) {

		//--------------------------------------------- C R I T I C A L  S E C T I O N  B E G I N -----------------------------------------------------
		// クリティカルセクション開始。他のクリティカルセクションを持つスレッドはクリティカルセクション内に書き込み出来ない
		EnterCriticalSection(&cs);

		string strCurLine = "";
		//-------------------行への分解処理
		{
			int sub = lpFind - pBufCurBonanzaLine; // どこで見つけたのか。

			// 改行の前までの文字列
			string prev_rn = bufCurBonanzaLine.substr(0, sub);
			// 改行の跡の文字列
			string post_rn = bufCurBonanzaLine.substr(sub+2);

			// 全ボナンザバッファーの保存に、この行を追加
			bufBonanzaAllOutput.push_back(prev_rn); // 改行分までをベクトルに足しこむ

			// 改行の後ろの文字列
			bufCurBonanzaLine = post_rn;

			// 改行までの位置行のデータを「現在の行」と称する。
			strCurLine = prev_rn;
		}

		// 改行の前までの綺麗な１行のBonanzaが出力したデータ
		char *pStrCurLine = (char *)strCurLine.c_str();

		// 棋譜だけのバッファに、この行を追加
		// ----------------------棋譜の保存-------------------------------------
		if ( strCurLine.find("move") == 0 ) {  // 今回の１行分で、「文字列の最初」に「move」とつくものを見つけた

			String^ pattern = "^move[0-9][0-9][0-9][0-9][A-Z][A-Z]";
			Regex^ regex = gcnew Regex( pattern );
			String^ line = gcnew String(strCurLine.c_str());
			Match^ match = regex->Match( line );

			if ( match->Success) {

				AddPlayerKifu();

				std::string lastest = string( pStrCurLine+4 );
				bufAllKifu.push_back( "-"+lastest ); // 先頭のmove 以外をコピー

				TenShougiBoard::MoveBoardSlot(lastest, '-');

				// 後手の方の駒音を鳴らす。
				TenShougiForm::tsgForm->PlayKomaSound(lastest);

				// ボードの表示を更新する。
				TenShougiForm::tsgForm->UpdatePositionByBoardSlot();
	
				// ★★自動保存
				SaveCSAKifu();

				// 後手指した直後。
				// もはや先手は明らかにダメ。詰んでる
				if ( -325.981 <= fCurKeiseiPoint && fCurKeiseiPoint < -325.90 ) {

					if ( TenShougiBoard::GetCurTurnState() == TenShougiBoard::CurTurnB ||  TenShougiBoard::GetCurTurnState() == TenShougiBoard::CurTurnW ) {

						// 手番情報は「先手が詰んで負けた」ので誰もさせない		
						TenShougiBoard::SetCurBResign();

						// 後手の投了ダイアログの表示
						TenShougiForm::tsgForm->SetTitle("後手、勝利");
					}				
				} else {

					// 先手番に戻す。
					TenShougiBoard::SetCurBTurn();
				}
			}

		// ----------------------移動間違い等の時に、情報クリア-------------------------------------
		} else if (strCurLine.find("WARNING: illegal move") == 0 ) {  // 今回の１行分で、プレイヤの先の手は不正である。
			// プレイヤが入力した最後の手は除去
			bufPlayerKifuLastest = "";

			// 先手番に戻す。
			TenShougiBoard::SetCurBTurn();

		// 二歩
		} else if (strCurLine.find("double pawn") != string::npos ) {  // 今回の１行分で、プレイヤの先の手は不正である。
			// プレイヤが入力した最後の手は除去
			bufPlayerKifuLastest = "";

			// 先手番に戻す。
			TenShougiBoard::SetCurBTurn();

			// 後手の投了ダイアログの表示
			TenShougiYesNoForm^ tsynf = gcnew TenShougiYesNoForm("二歩。", "了解", "");
			// ShowDialog表示時に、親フォームのまん中に表示する。
			tsynf->StartPosition = FormStartPosition::CenterParent;
			tsynf->ShowDialog(TenShougiForm::tsgForm);

		// 打ち歩詰み
		} else if (strCurLine.find("mated by a droped pawn") != string::npos ) {  // 今回の１行分で、プレイヤの先の手は不正である。
			// プレイヤが入力した最後の手は除去
			bufPlayerKifuLastest = "";

			// 後手の投了ダイアログの表示
			TenShougiYesNoForm^ tsynf = gcnew TenShougiYesNoForm("打ち歩詰み。", "了解", "");
			// ShowDialog表示時に、親フォームのまん中に表示する。
			tsynf->StartPosition = FormStartPosition::CenterParent;
			tsynf->ShowDialog(TenShougiForm::tsgForm);

			// 先手番に戻す。
			TenShougiBoard::SetCurBTurn();

		} else if (strCurLine.find("info ") == 0 ) {
			// プレイヤの最後の手が残ってるなら、プレイヤの手は有効だった。
			// 差し手を反映する。

			AddPlayerKifu();

			// ボードの表示を更新する。
			TenShougiForm::tsgForm->UpdatePositionByBoardSlot();

		// ----------------------形成判断の得点の保存-------------------------------------
		} else if (strCurLine.find("info+") == 0 || strCurLine.find("info-") == 0 ) {  // 形成判断の点が出ている。出るたびに代入する。

			// プレイヤの最後の手が残ってるなら、プレイヤの手は有効だった。
			// 差し手を反映する。
			AddPlayerKifu();

			// ボードの表示を更新する。
			TenShougiForm::tsgForm->UpdatePositionByBoardSlot();

			char szPoint[15] = ""; // 得点のためのコピー領域
			// info+0.27 +7776FU -3334FU +6878OU -4232OU ... みたいな行の得点が付くので、抜き出す。 
			strncpy( szPoint, (pStrCurLine+4), 10); // 適当にinfo+ の次の文字から10文字ほどコピー
			// スペースが見つかった。
			char *pSpace = strchr( szPoint, ' ');
			// スペースが見つかったら
			if (pSpace) {
				int sub = pSpace-szPoint; // 何文字目
				szPoint[sub] = NULL; // そこで文字をぶった切る
				fCurKeiseiPoint = atof(szPoint); // 形成情報を入れる。
			}

		// 後手が投了。
		} else if ( strCurLine.find("resign") == 0 ) {

			AddPlayerKifu();

			// ボードの表示を更新する。
			TenShougiForm::tsgForm->UpdatePositionByBoardSlot();

			// 後手の投了ダイアログの表示
			TenShougiForm::tsgForm->SetTitle("後手、投了。");

			// 手番情報は「後手が投了している」ので誰もさせない		
			TenShougiBoard::SetCurWResign();

		// 先手が自爆。
		} else if ( strCurLine.find("The king is hang") != string::npos || strCurLine.find("move after a game was concluded") != string::npos ) {

			AddPlayerKifu();

			// 手番情報は「先手が詰んで負けた」ので誰もさせない		
			// ボードの表示を更新する。
			TenShougiForm::tsgForm->UpdatePositionByBoardSlot();

			// 後手の投了ダイアログの表示
			TenShougiForm::tsgForm->SetTitle("後手、勝利。");

			TenShougiBoard::SetCurBResign();
		// 千日手。
		} else if ( strCurLine.find("perpetual check") != string::npos ) {

			AddPlayerKifu();

			// ボードの表示を更新する。
			TenShougiForm::tsgForm->UpdatePositionByBoardSlot();

			// 後手の投了ダイアログの表示
			TenShougiForm::tsgForm->SetTitle("千日手。");

			// 手番情報は「千日手になったので」ので誰もさせない		
			TenShougiBoard::SetCurPerPetual();

		} else if ( strCurLine.find("pong") == 0 ) {  // 終了の合図
			// ボナンザに終了せよという。
			GetAndSendInputThread("quit");

			// もうスレッド系からは脱出必須です!! というフラグ
			bMustBreakLoop = true;
		}

		// クリティカルセクション終了。
		LeaveCriticalSection(&cs);
		//--------------------------------------------- C R I T I C A L  S E C T I O N  E N D -----------------------------------------------------
	}

	return bMustBreakLoop;
}



// ボナンザの出力を読み取り、そのまま標準出力へと書きだす。
/////////////////////////////////////////////////////////////////////// 
// ReadAndHandleOutput
// Monitors handle for input. Exits when child exits or pipe breaks.
/////////////////////////////////////////////////////////////////////// 
DWORD WINAPI ReadAndHandleOutput(LPVOID lpvThreadParam)
{
	string bufCurBonanzaLine; // ボナンザが出力した内容のうち、１行に相当するバッファ。

	char lpBuffer[256] = "";
	DWORD nBytesRead;
	DWORD nCharsWritten = 0;
	HANDLE hPipeRead = (HANDLE)lpvThreadParam;

	while (bTenShougiRunThread)
	{
		// ボナンザの標準出力から読み取り。もし何も出力してないと、このReadFileから制御が返ってこないので注意。
		// CancelIo が書いてあることで、
		if (!ReadFile(hPipeRead, lpBuffer, sizeof(lpBuffer-1), // 最後にNULL足せるように1文字減らしておく
			&nBytesRead, NULL) || !nBytesRead)
		{
			if (GetLastError() == ERROR_BROKEN_PIPE)
				break; // pipe done - normal exit path.
			else {
				DisplayError("ReadFile"); // Something bad happened.
				break;
			}
		}

		lpBuffer[nBytesRead] = '\0';


		// OutputDebugString(lpBuffer);

		// ボナンザから読み取った情報を、行ごとに保存したり、分析して、各変数に保存したり。
		int bMustBreakLoop = AnalyzeBonanzaOutputAndSaveToVariableInCritialSection(bufCurBonanzaLine, lpBuffer);
		// もう、終了するべきだ。
		if ( bMustBreakLoop ) {
			// 全スレッド新規にまわすのはやめ。スレッドは終了へ迎え
			bTenShougiRunThread = FALSE;
			break; // このスレッドも終了へ
		}


		// Display the character read on the screen.
		/*
		TenShougiModには標準出力の画面がないので、これは不要。
		if (!WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), lpBuffer,
			nBytesRead, &nCharsWritten, NULL))
			DisplayError("WriteConsole");
		*/

	}

	return 1;
}

// 標準入力からの入力を読み取り、そのままボナンザへと出力する。
/////////////////////////////////////////////////////////////////////// 
// GetAndSendInputThread
// Thread procedure that monitors the console for input and sends input
// to the child process through the input pipe.
// This thread ends when the child application exits.
/////////////////////////////////////////////////////////////////////// 
#if 0
DWORD WINAPI GetAndSendInputThread(LPVOID lpvThreadParam)
{
	char read_buff[256];
	DWORD nBytesRead, nBytesWrote;
	HANDLE hPipeWrite = (HANDLE)lpvThreadParam;

	// Get input from our console and send it to child through the pipe.
	// 天将棋が続いている間中
	while (bTenShougiRunThread)
	{
		if (!ReadConsole(hStdIn, read_buff, 1, &nBytesRead, NULL))
			DisplayError("ReadConsole");

		read_buff[nBytesRead] = '\0'; // Follow input with a NULL.

		OutputDebugString(read_buff);
		if (!WriteFile(hPipeWrite, read_buff, nBytesRead, &nBytesWrote, NULL))
		{
			if (GetLastError() == ERROR_NO_DATA) {
				break; // Pipe was closed (normal exit path).
			}
			else {
				DisplayError("WriteFile");
				break;
			}
		}
	}

	return 1;
}
#endif

// .NETタイプのString^ → マルチバイトのC++型std::stringへ
std::string DotNetStringToMultiByte(String^ dotnettext) {
	// 以下変換のイディオム
	std::string text;  // 文字列を受け止めるバッファ

	IntPtr mptr = Runtime::InteropServices::Marshal::StringToHGlobalAnsi(dotnettext);

	// char *に変換
	text = (const char*)(mptr.ToPointer());

	// 
	Runtime::InteropServices::Marshal::FreeHGlobal(mptr);

	return text;
}

// 標準のC++のstd::stringをdelimで分解する。
vector<string> string_split(const string &str, char delim){
	istringstream iss(str);
	string tmp; vector<string> res;
	while(getline(iss, tmp, delim)) {
		res.push_back(tmp);
	} 
	return res;
}


// ボタンが押された時に実行する関数
BOOL GetAndSendInputThread(String^ text)
{
	// .NET型→SJISマルチバイトへ
	string sjis = DotNetStringToMultiByte(text);
	if ( sjis[sjis.size()-1] != '\n' ) { // 末尾の文字が改行じゃなければ、改行を付けたす。
		sjis += '\n'; // 末尾に改行
	}

	// string型→char []型へ
	char read_buff[256];
	strncpy(read_buff, sjis.c_str(), sizeof(read_buff)-1);

	DWORD nBytesRead, nBytesWrote;
	HANDLE hPipeWrite = (HANDLE)hInputWrite;

	// OutputDebugString(read_buff);
	nBytesRead = strlen(read_buff); // 文字列としての長さが書き込むべき長さ。

	// もしまだ天将棋が続いていれば
	if (bTenShougiRunThread) {

		// プレイヤが「差し手らしきものを指したのか？」として検証
		String^ line = gcnew String(read_buff);

		String^ pattern = "^[0-9][0-9][0-9][0-9][A-Z][A-Z]$";
		Regex^ regex = gcnew Regex( pattern );
		Match^ match = regex->Match( line );

		// どうやら指し手の形にはなっている。ただし、正しいかどうかは別。
		// 間違っているかどうかは、ボナンザの返答に任せる。
		if ( match->Success ) {

				// プレイヤーの最後の指した手として保存
				bufPlayerKifuLastest = read_buff;

				// プレイヤーは指したので、後手番に設定する。(先手は指せない)
				TenShougiBoard::SetCurWTurn();

		// 先手投了だ
		} else if ( sjis.find("resign\n") == 0 ) {

			// 手番情報は「先手が投了しているので」誰もさせない。
			TenShougiBoard::SetCurBResign();

			// プレイヤが差した最後の手は除去
			bufPlayerKifuLastest = "";

			// ここは一度通り抜けてWriteFile(hPipeWrite)を通るようにする。

		} else {

			// プレイヤが差した最後の手は除去
			bufPlayerKifuLastest = "";
		}


		// 先手の文字列をボナンザに送信!!
		if (!WriteFile(hPipeWrite, read_buff, nBytesRead, &nBytesWrote, NULL))
		{
			if (GetLastError() == ERROR_NO_DATA)
				return TRUE; // Pipe was closed (normal exit path).
			else {
				DisplayError("WriteFile");
			}
		}


		// Bonanzaに送信したので改めて、投了の場合だけダイアログを出す。
		// 先手投了だ。
		if ( sjis.find("resign\n") == 0 ) {

			// 先手が投了した。
			TenShougiBoard::SetCurBResign();
			// 後手の投了ダイアログの表示
			TenShougiForm::tsgForm->SetTitle("先手、投了。");
		}
	}

	return FALSE;
}


// エラーの警告表示
void DisplayError(char *pszMessage) {
	// OutputDebugString(pszMessage);
}


#define CSA_SAVEDATA_NAME	"TenShougiMod.csa"

// セーブする。
void SaveCSAKifu() {
	static char *szFileName = CSA_SAVEDATA_NAME;

	static char *szHeader = 
	"P1-KY-KE-GI-KI-OU-KI-GI-KE-KY\n"
	"P2 * -HI *  *  *  *  * -KA * \n"
	"P3-FU-FU-FU-FU-FU-FU-FU-FU-FU\n"
	"P4 *  *  *  *  *  *  *  *  * \n"
	"P5 *  *  *  *  *  *  *  *  * \n"
	"P6 *  *  *  *  *  *  *  *  * \n"
	"P7+FU+FU+FU+FU+FU+FU+FU+FU+FU\n"
	"P8 * +KA *  *  *  *  * +HI * \n"
	"P9+KY+KE+GI+KI+OU+KI+GI+KE+KY\n"
	"+\n";

	FILE *fp = fopen(szFileName, "w");

	fputs(szHeader, fp);

	// CSAの記譜を全部出力
	for each(string line in bufAllKifu) {
		fputs( line.c_str(), fp );
	
		if ( line[line.size()-1] != '\n' ) {
			fputs( "\n", fp );
		}
	}

	// 閉じる
	fclose(fp);
}


void LoadCSAKifu() {
	ReNewTenShougiGame();

	// セーブデータがあれば…
	if (System::IO::File::Exists(CSA_SAVEDATA_NAME)) {

		// readを試みる。内容エラー等であっても、(ReNewTenShougiGame()により)平手初期状態での開始になるだけで影響なし。
		GetAndSendInputThread("read " CSA_SAVEDATA_NAME);

		FILE *fp = fopen(CSA_SAVEDATA_NAME, "r");

		/* ファイルのオープン */
		if (fp == NULL) {
			return;
		}

		char read_buff[512] = {NULL};

		/* ファイルの終端まで文字を読み取り表示する */
		while ( fgets(read_buff, sizeof(read_buff), fp) != NULL ) {
			// 差し手らしきもの
			String^ line = gcnew String(read_buff);

			String^ pattern = "^[+-][0-9][0-9][0-9][0-9][A-Z][A-Z]";
			Regex^ regex = gcnew Regex( pattern );
			Match^ match = regex->Match( line );

			// 差し手だ!!
			if ( match->Success ) {
				// OutputDebugString("match!!");
				// OutputDebugString(read_buff);
				// +7766FU みたいになってるので、"7766FU", '+' みたいにして盤面を動かしていく
				bufAllKifu.push_back( read_buff );
				TenShougiBoard::MoveBoardSlot(read_buff+1, read_buff[0]);
			}
		}

		/* ファイルのクローズ */
		fclose(fp);

		// ボードの表示を更新する。
		TenShougiForm::tsgForm->UpdatePositionByBoardSlot();

	}
}
