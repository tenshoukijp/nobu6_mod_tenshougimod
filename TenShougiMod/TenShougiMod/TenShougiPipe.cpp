#define _CRT_SECURE_NO_WARNINGS


// �{�i���U�Ƃ̃p�C�v�֘A�𒆐S�Ƃ�������
// TenShougiModBgn() �� TenShougiModEnd() �́ADLL EXPORTS ����Ă���
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

// �N�����̃v���Z�X����
void PrepAndLaunchRedirectedChild(HANDLE hChildStdOut, HANDLE hChildStdIn, HANDLE hChildStdErr);

// �{�i���U���W���o�͂��Ă�����̂��p�C�v�o�R�œǂݍ���
DWORD WINAPI ReadAndHandleOutput(LPVOID lpvThreadParam);
//DWORD WINAPI GetAndSendInputThread(LPVOID lpvThreadParam);
DWORD WINAPI CheckTenshougiStatus(LPVOID lpvThreadParam);

// ���̃A�v��������p�C�v�o�R�Ń{�i���U�ɏ�������
BOOL GetAndSendInputThread(String^ text);

static HANDLE hChildProcess = NULL;
static HANDLE hStdIn = NULL; // Handle to parents std input.

// �X���b�h��(���s����^�I������)�@�̂��߂̃t���O
BOOL bTenShougiRunThread = FALSE;

static HANDLE hThread1 = NULL;
static HANDLE hThread2 = NULL;
static HANDLE hThread3 = NULL;

CRITICAL_SECTION cs; // �X���b�h�����݂���̂ŃN���e�B�J���Z�N�V����(�r������)

static HANDLE hOutputReadTmp, hOutputRead, hOutputWrite;
static HANDLE hInputWriteTmp, hInputRead, hInputWrite;

vector<string> bufBonanzaAllOutput; // �{�i���U�̕W���o�͑S�Ă�z��Ŏ���������
vector<string> bufAllKifu;			// �v���C���ƃ{�i���U�̊���
std::string bufPlayerKifuLastest;		// �v���C���[���Ō�ɓ��͂�������

double fCurKeiseiPoint = 0;				// �����A�ǂ��炪�L�����B300���炢�Ő�]�I�B��肪+ ��肪-



static void OnBgnBonanzaOption();
static void OnEndBonanzaOption();

extern void OpenWindowAndNewTenShougiGame();




// �V�����J�n
void WINAPI sgTenShougiModBgn() {
	// �ŏ��Ƀn���h�����m�ۂ��邱�ƁB
	LoadTenshouHandle();

	// ���ɖ{���̏���
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

	bTenShougiRunThread = TRUE; // �V�����J�n

	//�N���e�B�J���Z�N�V�������쐬  071
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


	// �V��������������ׂ��H
	hThread3 = CreateThread(NULL, 0, CheckTenshougiStatus,
		(LPVOID)NULL, 0, &ThreadId3);
	if (hThread3 == NULL) DisplayError("CreateThread");


	OpenWindowAndNewTenShougiGame();
}


void OpenWindowAndNewTenShougiGame() {

	// �����Ղ̏�����
	TenShougiBoard::BoardAllInitialize();

	//--------------------------------------------------------------------------------
	TenShougiForm::tsgForm	 = gcnew TenShougiForm();
	TenShougiForm::tsgForm->Show();

	// �_�~�[�ŉ����Đ�����B�����L���b�V�������邽�߁B
	TenShougiForm::tsgForm->PlayKomaSound("DummySound");

	// �Q�[�����J�n����B
	ReNewTenShougiGame();
	//--------------------------------------------------------------------------------
}

void ReOpenWindowAndNewTenShougiGame() {

	// �����Ղ̏�����
	TenShougiBoard::BoardAllInitialize();

	//--------------------------------------------------------------------------------
	TenShougiForm::tsgForm->Show();

	// �_�~�[�ŉ����Đ�����B�����L���b�V�������邽�߁B
	TenShougiForm::tsgForm->PlayKomaSound("DummySound");

	// �Q�[�����J�n����B
	ReNewTenShougiGame();
	//--------------------------------------------------------------------------------
}


void OnBgnBonanzaOption() {
	GetAndSendInputThread("new"); // Bonanza�ɉɂȎ�����ɗ\�����Ȃ��悤�Ɍ���

	GetAndSendInputThread("ponder off"); // Bonanza�ɉɂȎ�����ɗ\�����Ȃ��悤�Ɍ���

	GetAndSendInputThread("resign 9000"); // �ǂ̂��炢�œ������邩�H


	if ( SharedMemory::WPower.iPower != -1 ) { // �������̊��͂��ݒ肳��Ă���Ȃ��
		// limit depth 10 �Ȃǂ̐ݒ�
		String^ szDepthCommand = String::Format( "limit depth {0}", max(SharedMemory::WPower.iPower, 1) ); // ���l��3�`10���炢�Ȃ̂ŁA�f�t�H���g��10
		GetAndSendInputThread(szDepthCommand);

		// limit node 3 �Ȃǂ̐ݒ�
		String^ szNodeCommand = String::Format( "limit nodes {0}", max( int(SharedMemory::WPower.iPower/3), 1)); // 1�`3�ɂȂ�悤�ɁB�f�t�H���g�͂R
		GetAndSendInputThread(szNodeCommand); 

		String^ szTimeCommand = String::Format( "limit time 0 {0}", int(SharedMemory::WPower.iPower/3) ); // 0�`3�ɂȂ�悤�ɁB�f�t�H���g�͂R
		GetAndSendInputThread(szTimeCommand);
	} else {
		GetAndSendInputThread("limit time 0 3"); // CPU�̎������Ԃ�0���B���3�b�ȓ��B
	}

	GetAndSendInputThread("limit time strict"); // ���Ԑ�������
}

void ReNewTenShougiGame() {
	// �����Ղ̏�����
	TenShougiBoard::BoardAllInitialize();

	// �{�i���U�ɐݒ�𑗐M
	OnBgnBonanzaOption();

	//--------------------------------------------- C R I T I C A L  S E C T I O N  B E G I N -----------------------------------------------------
	// �N���e�B�J���Z�N�V�����J�n�B���̃N���e�B�J���Z�N�V���������X���b�h�̓N���e�B�J���Z�N�V�������ɏ������ݏo���Ȃ�
	EnterCriticalSection(&cs);

	bufBonanzaAllOutput.clear();		// �S�Ă̋L���z��̃N���A
	bufAllKifu.clear();					// �v���C���ƃ{�i���U�̊���
	bufPlayerKifuLastest = "";			// �v���C���[���Ō�ɓ��͂�������

	TenShougiBoard::SetCurBTurn();		// �v���C���[�̎��

	// �N���e�B�J���Z�N�V�����I���B
	LeaveCriticalSection(&cs);
	//--------------------------------------------- C R I T I C A L  S E C T I O N  E N D -----------------------------------------------------

	// �{�[�h�̕\�����X�V����B
	TenShougiForm::tsgForm->UpdatePositionByBoardSlot();
}



// TSMod����̃R�}���h���󂯂āA���낢��Ə������s���B
DWORD WINAPI CheckTenshougiStatus(LPVOID lpvThreadParam) {
	while (bTenShougiRunThread)
	{
		// TSMod����̃R�}���h�����o��
		string command = PopCommand();

		if ( command == "open_window_and_new_game" ) {
			ReOpenWindowAndNewTenShougiGame();
		}
		else if ( command == "open_window_and_continue_game" ) {
			ReOpenWindowAndNewTenShougiGame();
			LoadCSAKifu(); // ���Ԑ�������
		}
		else if ( command == "new_game" ) {
			ReNewTenShougiGame();
		}
		else if ( command == "continue_game" ) {
			ReNewTenShougiGame();
			LoadCSAKifu(); // ���Ԑ�������
		}
		// �t�H�[������Ă��܂��Ƃ������߁i�{���ɕ���ƃo�O��̂�Hide()
		else if ( command == "hide_window" ) {
			TenShougiForm::tsgForm->Hide();
			// �����Ȃ����āA��������B
			GetAndSendInputThread("resign");
		}
		System::Threading::Thread::Sleep(300);
	}

	return 1; // �X���b�h�I��
}


static void OnEndBonanzaOption() {
	// Bonanza�ɏI�����߂𑗂�
	GetAndSendInputThread( "ping" );
}




// �V�����I��
void WINAPI sgTenShougiModEnd() {

	//--------------------------------------------------------------------------------
	// Tell the thread to exit and wait for thread to die.

	// �{�i���U�Ɏ�����I��������B
	OnEndBonanzaOption();

	// GUI���I��
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

	// bTenShougiRunThread��false�ɂȂ�܂ł�����Ƒ҂B

	MSG msg;
	// ���C�� ���b�Z�[�W ���[�v:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		// �X���b�h�I���֌������Ă��Ȃ��Ȃ�΁A�҂B
		// ping��ł������ƂŁAbonanza��pong�ƕԂ��ƁA�S�̂̏I�����n�܂�B
		if (bTenShougiRunThread)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

		// �X���b�h�͏I���ւƌ��������A������B
		} else {
			break;
		}
	}

	// �e�X���b�h���Ō�܂Ŏ��s�����̂�҂��A�Ō�܂Ŏ��s���ꂽ��A�X���b�h�����B
	if (WaitForSingleObject(hThread3, INFINITE) == WAIT_FAILED) {
		DisplayError("WaitForSingleObject");
	}
	if (WaitForSingleObject(hThread2, INFINITE) == WAIT_FAILED) {
		DisplayError("WaitForSingleObject");
	}

	// ReadLine���A���䂪�Ԃ��Ă��Ȃ��֐��ŁA�t�@�C���n���h���g�p���ł����Ă��A
	// �S���L�����Z�����邽�߂̊֐��B
	CancelIo( hInputRead );
	CancelIo( hOutputRead );
	CancelIo( hStdIn );

	// �J���Ă����p�C�v�����
	if (hOutputRead) {
		if (!CloseHandle(hOutputRead)) DisplayError("OutputRead CloseHandle");
	}
	if (hInputWrite) {
		if (!CloseHandle(hInputWrite)) DisplayError("InputWrite CloseHandle");
	}

	//�N���e�B�J���Z�N�V�������폜
	DeleteCriticalSection(&cs);

	// �K�x�[�W�R���N�g
	GC::Collect();

	// �Ō�ɖ{�̂ւ̃n���h�����������B
	FreeTenshouHandle();

}


// �v���Z�X�̍\�z�B�R�}���h���C���́uTenShougiModServer.exe csa_shogi�v�B�o�[�W������4.1.3
// ���g�̓{�i���U�Ȃ̂ŁA�ŐV��Bonanza.exe �� TenShougiModServer.exe�ƃ��l�[�����Ă��g����B
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
	si.wShowWindow = SW_HIDE; // ��\���ŋN��
	// Use this if you want to hide the child:
	//     si.wShowWindow = SW_HIDE;
	// Note that dwFlags must include STARTF_USESHOWWINDOW if you want to
	// use the wShowWindow flags.


	// Launch the process that you want to redirect (in this case,
	// Child.exe). Make sure Child.exe is in the same directory as
	// redirect.c launch redirect from a command line to prevent location
	// confusion.
	//																				�� Window�͕\�����Ȃ�
	if (!CreateProcess(NULL, "TenShougiModServer.exe csa_shogi", NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
		DisplayError("CreateProcess");


	// Set global child process handle to cause threads to exit.
	hChildProcess = pi.hProcess;


	// Close any unnecessary handles.
	if (!CloseHandle(pi.hThread)) DisplayError("CloseHandle");
}

void AddPlayerKifu() {
	// �v���C���̍Ō�̎肪�c���Ă�Ȃ�A�v���C���̎�͗L���������B
	// ������𔽉f����B
	if ( bufPlayerKifuLastest != "" ) {
		bufAllKifu.push_back("+"+bufPlayerKifuLastest); // �{�i���U�����������̂ŁA�v���C���̍Ō�̎��ۑ��B
		// ������X�V����
		TenShougiBoard::MoveBoardSlot(bufPlayerKifuLastest, '+');

		// ���炷�B
		TenShougiForm::tsgForm->PlayKomaSound(bufPlayerKifuLastest);

		// �v���C�������͂����Ō�̎�͏���
		bufPlayerKifuLastest = "";

	}
}

// �{�i���U����ǂݎ���������A�s���Ƃɕۑ�������A���͂��āA�e�ϐ��ɕۑ�������B
bool AnalyzeBonanzaOutputAndSaveToVariableInCritialSection(string& bufCurBonanzaLine, char *lpBuffer) {

	bool bMustBreakLoop = false;

	// �P�s���C���ɗ�������
	bufCurBonanzaLine += lpBuffer;

	char *pBufCurBonanzaLine = (char *)bufCurBonanzaLine.c_str();

	// �P�s�̃��C���̃o�b�t�@�[�̒���\r\n���`�����ꂽ
	char *lpFind = strstr(pBufCurBonanzaLine, "\r\n");
	if (lpFind) {

		//--------------------------------------------- C R I T I C A L  S E C T I O N  B E G I N -----------------------------------------------------
		// �N���e�B�J���Z�N�V�����J�n�B���̃N���e�B�J���Z�N�V���������X���b�h�̓N���e�B�J���Z�N�V�������ɏ������ݏo���Ȃ�
		EnterCriticalSection(&cs);

		string strCurLine = "";
		//-------------------�s�ւ̕�������
		{
			int sub = lpFind - pBufCurBonanzaLine; // �ǂ��Ō������̂��B

			// ���s�̑O�܂ł̕�����
			string prev_rn = bufCurBonanzaLine.substr(0, sub);
			// ���s�̐Ղ̕�����
			string post_rn = bufCurBonanzaLine.substr(sub+2);

			// �S�{�i���U�o�b�t�@�[�̕ۑ��ɁA���̍s��ǉ�
			bufBonanzaAllOutput.push_back(prev_rn); // ���s���܂ł��x�N�g���ɑ�������

			// ���s�̌��̕�����
			bufCurBonanzaLine = post_rn;

			// ���s�܂ł̈ʒu�s�̃f�[�^���u���݂̍s�v�Ə̂���B
			strCurLine = prev_rn;
		}

		// ���s�̑O�܂ł��Y��ȂP�s��Bonanza���o�͂����f�[�^
		char *pStrCurLine = (char *)strCurLine.c_str();

		// ���������̃o�b�t�@�ɁA���̍s��ǉ�
		// ----------------------�����̕ۑ�-------------------------------------
		if ( strCurLine.find("move") == 0 ) {  // ����̂P�s���ŁA�u������̍ŏ��v�Ɂumove�v�Ƃ����̂�������

			String^ pattern = "^move[0-9][0-9][0-9][0-9][A-Z][A-Z]";
			Regex^ regex = gcnew Regex( pattern );
			String^ line = gcnew String(strCurLine.c_str());
			Match^ match = regex->Match( line );

			if ( match->Success) {

				AddPlayerKifu();

				std::string lastest = string( pStrCurLine+4 );
				bufAllKifu.push_back( "-"+lastest ); // �擪��move �ȊO���R�s�[

				TenShougiBoard::MoveBoardSlot(lastest, '-');

				// ���̕��̋��炷�B
				TenShougiForm::tsgForm->PlayKomaSound(lastest);

				// �{�[�h�̕\�����X�V����B
				TenShougiForm::tsgForm->UpdatePositionByBoardSlot();
	
				// ���������ۑ�
				SaveCSAKifu();

				// ���w��������B
				// ���͂���͖��炩�Ƀ_���B�l��ł�
				if ( -325.981 <= fCurKeiseiPoint && fCurKeiseiPoint < -325.90 ) {

					if ( TenShougiBoard::GetCurTurnState() == TenShougiBoard::CurTurnB ||  TenShougiBoard::GetCurTurnState() == TenShougiBoard::CurTurnW ) {

						// ��ԏ��́u��肪�l��ŕ������v�̂ŒN�������Ȃ�		
						TenShougiBoard::SetCurBResign();

						// ���̓����_�C�A���O�̕\��
						TenShougiForm::tsgForm->SetTitle("���A����");
					}				
				} else {

					// ���Ԃɖ߂��B
					TenShougiBoard::SetCurBTurn();
				}
			}

		// ----------------------�ړ��ԈႢ���̎��ɁA���N���A-------------------------------------
		} else if (strCurLine.find("WARNING: illegal move") == 0 ) {  // ����̂P�s���ŁA�v���C���̐�̎�͕s���ł���B
			// �v���C�������͂����Ō�̎�͏���
			bufPlayerKifuLastest = "";

			// ���Ԃɖ߂��B
			TenShougiBoard::SetCurBTurn();

		// ���
		} else if (strCurLine.find("double pawn") != string::npos ) {  // ����̂P�s���ŁA�v���C���̐�̎�͕s���ł���B
			// �v���C�������͂����Ō�̎�͏���
			bufPlayerKifuLastest = "";

			// ���Ԃɖ߂��B
			TenShougiBoard::SetCurBTurn();

			// ���̓����_�C�A���O�̕\��
			TenShougiYesNoForm^ tsynf = gcnew TenShougiYesNoForm("����B", "����", "");
			// ShowDialog�\�����ɁA�e�t�H�[���̂܂񒆂ɕ\������B
			tsynf->StartPosition = FormStartPosition::CenterParent;
			tsynf->ShowDialog(TenShougiForm::tsgForm);

		// �ł����l��
		} else if (strCurLine.find("mated by a droped pawn") != string::npos ) {  // ����̂P�s���ŁA�v���C���̐�̎�͕s���ł���B
			// �v���C�������͂����Ō�̎�͏���
			bufPlayerKifuLastest = "";

			// ���̓����_�C�A���O�̕\��
			TenShougiYesNoForm^ tsynf = gcnew TenShougiYesNoForm("�ł����l�݁B", "����", "");
			// ShowDialog�\�����ɁA�e�t�H�[���̂܂񒆂ɕ\������B
			tsynf->StartPosition = FormStartPosition::CenterParent;
			tsynf->ShowDialog(TenShougiForm::tsgForm);

			// ���Ԃɖ߂��B
			TenShougiBoard::SetCurBTurn();

		} else if (strCurLine.find("info ") == 0 ) {
			// �v���C���̍Ō�̎肪�c���Ă�Ȃ�A�v���C���̎�͗L���������B
			// ������𔽉f����B

			AddPlayerKifu();

			// �{�[�h�̕\�����X�V����B
			TenShougiForm::tsgForm->UpdatePositionByBoardSlot();

		// ----------------------�`�����f�̓��_�̕ۑ�-------------------------------------
		} else if (strCurLine.find("info+") == 0 || strCurLine.find("info-") == 0 ) {  // �`�����f�̓_���o�Ă���B�o�邽�тɑ������B

			// �v���C���̍Ō�̎肪�c���Ă�Ȃ�A�v���C���̎�͗L���������B
			// ������𔽉f����B
			AddPlayerKifu();

			// �{�[�h�̕\�����X�V����B
			TenShougiForm::tsgForm->UpdatePositionByBoardSlot();

			char szPoint[15] = ""; // ���_�̂��߂̃R�s�[�̈�
			// info+0.27 +7776FU -3334FU +6878OU -4232OU ... �݂����ȍs�̓��_���t���̂ŁA�����o���B 
			strncpy( szPoint, (pStrCurLine+4), 10); // �K����info+ �̎��̕�������10�����قǃR�s�[
			// �X�y�[�X�����������B
			char *pSpace = strchr( szPoint, ' ');
			// �X�y�[�X������������
			if (pSpace) {
				int sub = pSpace-szPoint; // ��������
				szPoint[sub] = NULL; // �����ŕ������Ԃ����؂�
				fCurKeiseiPoint = atof(szPoint); // �`����������B
			}

		// ��肪�����B
		} else if ( strCurLine.find("resign") == 0 ) {

			AddPlayerKifu();

			// �{�[�h�̕\�����X�V����B
			TenShougiForm::tsgForm->UpdatePositionByBoardSlot();

			// ���̓����_�C�A���O�̕\��
			TenShougiForm::tsgForm->SetTitle("���A�����B");

			// ��ԏ��́u��肪�������Ă���v�̂ŒN�������Ȃ�		
			TenShougiBoard::SetCurWResign();

		// ��肪�����B
		} else if ( strCurLine.find("The king is hang") != string::npos || strCurLine.find("move after a game was concluded") != string::npos ) {

			AddPlayerKifu();

			// ��ԏ��́u��肪�l��ŕ������v�̂ŒN�������Ȃ�		
			// �{�[�h�̕\�����X�V����B
			TenShougiForm::tsgForm->UpdatePositionByBoardSlot();

			// ���̓����_�C�A���O�̕\��
			TenShougiForm::tsgForm->SetTitle("���A�����B");

			TenShougiBoard::SetCurBResign();
		// �����B
		} else if ( strCurLine.find("perpetual check") != string::npos ) {

			AddPlayerKifu();

			// �{�[�h�̕\�����X�V����B
			TenShougiForm::tsgForm->UpdatePositionByBoardSlot();

			// ���̓����_�C�A���O�̕\��
			TenShougiForm::tsgForm->SetTitle("�����B");

			// ��ԏ��́u�����ɂȂ����̂Łv�̂ŒN�������Ȃ�		
			TenShougiBoard::SetCurPerPetual();

		} else if ( strCurLine.find("pong") == 0 ) {  // �I���̍��}
			// �{�i���U�ɏI������Ƃ����B
			GetAndSendInputThread("quit");

			// �����X���b�h�n����͒E�o�K�{�ł�!! �Ƃ����t���O
			bMustBreakLoop = true;
		}

		// �N���e�B�J���Z�N�V�����I���B
		LeaveCriticalSection(&cs);
		//--------------------------------------------- C R I T I C A L  S E C T I O N  E N D -----------------------------------------------------
	}

	return bMustBreakLoop;
}



// �{�i���U�̏o�͂�ǂݎ��A���̂܂ܕW���o�͂ւƏ��������B
/////////////////////////////////////////////////////////////////////// 
// ReadAndHandleOutput
// Monitors handle for input. Exits when child exits or pipe breaks.
/////////////////////////////////////////////////////////////////////// 
DWORD WINAPI ReadAndHandleOutput(LPVOID lpvThreadParam)
{
	string bufCurBonanzaLine; // �{�i���U���o�͂������e�̂����A�P�s�ɑ�������o�b�t�@�B

	char lpBuffer[256] = "";
	DWORD nBytesRead;
	DWORD nCharsWritten = 0;
	HANDLE hPipeRead = (HANDLE)lpvThreadParam;

	while (bTenShougiRunThread)
	{
		// �{�i���U�̕W���o�͂���ǂݎ��B���������o�͂��ĂȂ��ƁA����ReadFile���琧�䂪�Ԃ��Ă��Ȃ��̂Œ��ӁB
		// CancelIo �������Ă��邱�ƂŁA
		if (!ReadFile(hPipeRead, lpBuffer, sizeof(lpBuffer-1), // �Ō��NULL������悤��1�������炵�Ă���
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

		// �{�i���U����ǂݎ���������A�s���Ƃɕۑ�������A���͂��āA�e�ϐ��ɕۑ�������B
		int bMustBreakLoop = AnalyzeBonanzaOutputAndSaveToVariableInCritialSection(bufCurBonanzaLine, lpBuffer);
		// �����A�I������ׂ����B
		if ( bMustBreakLoop ) {
			// �S�X���b�h�V�K�ɂ܂킷�̂͂�߁B�X���b�h�͏I���֌}��
			bTenShougiRunThread = FALSE;
			break; // ���̃X���b�h���I����
		}


		// Display the character read on the screen.
		/*
		TenShougiMod�ɂ͕W���o�͂̉�ʂ��Ȃ��̂ŁA����͕s�v�B
		if (!WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), lpBuffer,
			nBytesRead, &nCharsWritten, NULL))
			DisplayError("WriteConsole");
		*/

	}

	return 1;
}

// �W�����͂���̓��͂�ǂݎ��A���̂܂܃{�i���U�ւƏo�͂���B
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
	// �V�����������Ă���Ԓ�
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

// .NET�^�C�v��String^ �� �}���`�o�C�g��C++�^std::string��
std::string DotNetStringToMultiByte(String^ dotnettext) {
	// �ȉ��ϊ��̃C�f�B�I��
	std::string text;  // ��������󂯎~�߂�o�b�t�@

	IntPtr mptr = Runtime::InteropServices::Marshal::StringToHGlobalAnsi(dotnettext);

	// char *�ɕϊ�
	text = (const char*)(mptr.ToPointer());

	// 
	Runtime::InteropServices::Marshal::FreeHGlobal(mptr);

	return text;
}

// �W����C++��std::string��delim�ŕ�������B
vector<string> string_split(const string &str, char delim){
	istringstream iss(str);
	string tmp; vector<string> res;
	while(getline(iss, tmp, delim)) {
		res.push_back(tmp);
	} 
	return res;
}


// �{�^���������ꂽ���Ɏ��s����֐�
BOOL GetAndSendInputThread(String^ text)
{
	// .NET�^��SJIS�}���`�o�C�g��
	string sjis = DotNetStringToMultiByte(text);
	if ( sjis[sjis.size()-1] != '\n' ) { // �����̕��������s����Ȃ���΁A���s��t�������B
		sjis += '\n'; // �����ɉ��s
	}

	// string�^��char []�^��
	char read_buff[256];
	strncpy(read_buff, sjis.c_str(), sizeof(read_buff)-1);

	DWORD nBytesRead, nBytesWrote;
	HANDLE hPipeWrite = (HANDLE)hInputWrite;

	// OutputDebugString(read_buff);
	nBytesRead = strlen(read_buff); // ������Ƃ��Ă̒������������ނׂ������B

	// �����܂��V�����������Ă����
	if (bTenShougiRunThread) {

		// �v���C�����u������炵�����̂��w�����̂��H�v�Ƃ��Č���
		String^ line = gcnew String(read_buff);

		String^ pattern = "^[0-9][0-9][0-9][0-9][A-Z][A-Z]$";
		Regex^ regex = gcnew Regex( pattern );
		Match^ match = regex->Match( line );

		// �ǂ����w����̌`�ɂ͂Ȃ��Ă���B�������A���������ǂ����͕ʁB
		// �Ԉ���Ă��邩�ǂ����́A�{�i���U�̕ԓ��ɔC����B
		if ( match->Success ) {

				// �v���C���[�̍Ō�̎w������Ƃ��ĕۑ�
				bufPlayerKifuLastest = read_buff;

				// �v���C���[�͎w�����̂ŁA���Ԃɐݒ肷��B(���͎w���Ȃ�)
				TenShougiBoard::SetCurWTurn();

		// ��蓊����
		} else if ( sjis.find("resign\n") == 0 ) {

			// ��ԏ��́u��肪�������Ă���̂Łv�N�������Ȃ��B
			TenShougiBoard::SetCurBResign();

			// �v���C�����������Ō�̎�͏���
			bufPlayerKifuLastest = "";

			// �����͈�x�ʂ蔲����WriteFile(hPipeWrite)��ʂ�悤�ɂ���B

		} else {

			// �v���C�����������Ō�̎�͏���
			bufPlayerKifuLastest = "";
		}


		// ���̕�������{�i���U�ɑ��M!!
		if (!WriteFile(hPipeWrite, read_buff, nBytesRead, &nBytesWrote, NULL))
		{
			if (GetLastError() == ERROR_NO_DATA)
				return TRUE; // Pipe was closed (normal exit path).
			else {
				DisplayError("WriteFile");
			}
		}


		// Bonanza�ɑ��M�����̂ŉ��߂āA�����̏ꍇ�����_�C�A���O���o���B
		// ��蓊�����B
		if ( sjis.find("resign\n") == 0 ) {

			// ��肪���������B
			TenShougiBoard::SetCurBResign();
			// ���̓����_�C�A���O�̕\��
			TenShougiForm::tsgForm->SetTitle("���A�����B");
		}
	}

	return FALSE;
}


// �G���[�̌x���\��
void DisplayError(char *pszMessage) {
	// OutputDebugString(pszMessage);
}


#define CSA_SAVEDATA_NAME	"TenShougiMod.csa"

// �Z�[�u����B
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

	// CSA�̋L����S���o��
	for each(string line in bufAllKifu) {
		fputs( line.c_str(), fp );
	
		if ( line[line.size()-1] != '\n' ) {
			fputs( "\n", fp );
		}
	}

	// ����
	fclose(fp);
}


void LoadCSAKifu() {
	ReNewTenShougiGame();

	// �Z�[�u�f�[�^������΁c
	if (System::IO::File::Exists(CSA_SAVEDATA_NAME)) {

		// read�����݂�B���e�G���[���ł����Ă��A(ReNewTenShougiGame()�ɂ��)���菉����Ԃł̊J�n�ɂȂ邾���ŉe���Ȃ��B
		GetAndSendInputThread("read " CSA_SAVEDATA_NAME);

		FILE *fp = fopen(CSA_SAVEDATA_NAME, "r");

		/* �t�@�C���̃I�[�v�� */
		if (fp == NULL) {
			return;
		}

		char read_buff[512] = {NULL};

		/* �t�@�C���̏I�[�܂ŕ�����ǂݎ��\������ */
		while ( fgets(read_buff, sizeof(read_buff), fp) != NULL ) {
			// ������炵������
			String^ line = gcnew String(read_buff);

			String^ pattern = "^[+-][0-9][0-9][0-9][0-9][A-Z][A-Z]";
			Regex^ regex = gcnew Regex( pattern );
			Match^ match = regex->Match( line );

			// �����肾!!
			if ( match->Success ) {
				// OutputDebugString("match!!");
				// OutputDebugString(read_buff);
				// +7766FU �݂����ɂȂ��Ă�̂ŁA"7766FU", '+' �݂����ɂ��ĔՖʂ𓮂����Ă���
				bufAllKifu.push_back( read_buff );
				TenShougiBoard::MoveBoardSlot(read_buff+1, read_buff[0]);
			}
		}

		/* �t�@�C���̃N���[�Y */
		fclose(fp);

		// �{�[�h�̕\�����X�V����B
		TenShougiForm::tsgForm->UpdatePositionByBoardSlot();

	}
}
