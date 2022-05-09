#include <windows.h>

#include "SharedMemory.h"
#include <string>

HWND    hTenshouWnd			= NULL; // �{�̂̃E�B���h�E�n���h��
HANDLE  hTenshouProcess		= NULL; // �{�̂̃v���Z�X�n���h��
DWORD   dwTenshouProcessID	= NULL; // �{�̂̃v���Z�XID




// ���L������
#pragma data_seg(".TenShougiModSharedMemory")

namespace SharedMemory {

	char szFontName[64] = ""; // �V�ċL�Ŏg�����ƂƂȂ����t�H���g���B
							  //�����̓V�ċL�Ƃ͕ʂ̃|�C���^���w���Ďw�肷�邩������Ȃ��̂ŁA�傫�ڃo�b�t�@�ŁB

	char szCommand[64] = ""; // �V�ċL����󂯎��R�}���h�BCheckTenshougiStatus�֐��őΏ�����B

	TBUshouDaniPowerInfo BPower = {NULL};	// ���̊���
	TBUshouDaniPowerInfo WPower = {NULL};	// ���̊���

	int iCurrentTurnState = 0; // ���݂̃^�[�����v���C�����b�o�t���B0:�v���C��, 1:CPU
	int iTenShougiFormShown = 0;
}

#pragma data_seg()




// �O���Q�Ɨp�B�V�ċL������t�H���g��ݒ肷��B
void WINAPI sgSetTenShougiFontName(char *pszFontName) {
	strcpy(SharedMemory::szFontName, pszFontName);
}


//  �O���Q�Ɨp�B�V�ċL�ō��\�����Ă���t�H���g���𓾂�B
// ������΁A�l�r �����Ƃ���B
char *sgGetTenShougiFontName() {
	if ( strlen(SharedMemory::szFontName) > 0 ) {
		return SharedMemory::szFontName;
	} else {
		return "�l�r ����";
	}
}

// �O���Q�Ɨp�B���ݓV�������v���C���Ă���H
BOOL WINAPI sgIsTenShougiFormShown() {
	return SharedMemory::iTenShougiFormShown;
}


// �V�ċL�v���Z�X����V�����v���Z�X�ւƒl���R�s�[����B
void LoadTenshouHandle() {

	// �E�B���h�E�n���h�����擾
	hTenshouWnd = FindWindow("Tenshouki95", NULL);

	if ( !hTenshouWnd ) { return; }

	// �v���Z�XID���擾
	DWORD _dumm = GetWindowThreadProcessId(hTenshouWnd, &dwTenshouProcessID);

	// �v���Z�X�n���h�����擾
	hTenshouProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwTenshouProcessID);
}

// �m�ۂ����v���Z�X�n���h���̉��
void FreeTenshouHandle() {

    if(hTenshouProcess){
		// �n���h�������
        CloseHandle(hTenshouProcess);
    }

}

// �V�ċL���V�����ւ̃R�}���h�B������Ŏ󂯎�� 
void WINAPI sgSendCommand(char *szCommand) {
	strcpy(SharedMemory::szCommand, szCommand);
}

// �V�ċL���瑗���Ă��Ă���R�}���h�����o���B
std::string PopCommand() {
	std::string command = std::string(SharedMemory::szCommand);
	SharedMemory::szCommand[0] = NULL;
	return command;
}



// �O���Q�Ɨp�B�V�ċL������΋ǎ҂̊��͂𓾂�Bdllexport�p
void WINAPI sgSetTenShougiBushouPower(void* pBPower, void* pWPower ) {

	// �e�X���͌^�|�C���^�B�����p���[�ƒi�ʖ��������Ă���B
	TBUshouDaniPowerInfo* BPower = (TBUshouDaniPowerInfo *)pBPower;
	TBUshouDaniPowerInfo* WPower = (TBUshouDaniPowerInfo *)pWPower;

	// ���[�J���ϐ��ɃR�s�[
	SharedMemory::BPower.iPower = BPower->iPower;
	strcpy( SharedMemory::BPower.szDaniName, BPower->szDaniName);
	SharedMemory::BPower.iBushouID = BPower->iBushouID;


	// ���[�J���ϐ��ɃR�s�[
	SharedMemory::WPower.iPower = WPower->iPower;
	strcpy( SharedMemory::WPower.szDaniName, WPower->szDaniName);
	SharedMemory::WPower.iBushouID = WPower->iBushouID;
}

// �V�ċL�̃v���Z�X����V�����̃v���Z�X�ւƒl���R�s�[
void CopyTenshoukiProcessToTenshougiProcess() {
	// �{�̂ւ̃v���Z�X��񂪓����Ă��Ȃ���΂ǂ����悤���Ȃ�
	if ( !hTenshouProcess ) {
		return;
	}


	// ��������S���A�V�ċL�v���Z�X���V�����v���Z�X�ɃR�s�[
	ReadProcessMemory( hTenshouProcess, (LPCVOID)(GAMEDATASTRUCT_BUSHOU_ADDRESSS), nb6bushou, sizeof(NB6BUSHOU)*GAMEDATASTRUCT_BUSHOU_NUM, NULL);
	ReadProcessMemory( hTenshouProcess, (LPCVOID)(GAMEDATASTRUCT_BUSHOUNAME_ADDRESSS), nb6bushouname, sizeof(NB6BUSHOUNAME)*GAMEDATASTRUCT_BUSHOU_NUM, NULL);
}