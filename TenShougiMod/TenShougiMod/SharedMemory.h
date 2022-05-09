#pragma once

#include "GameDataStruct.h"
#include <string>


namespace SharedMemory {
	extern int iCurrentTurnState; // ���݂̃^�[�����v���C�����b�o�t���B0:�v���C��, 1:CPU
	extern int iTenShougiFormShown; // �t�H�[�����\������Ă��邩�ǂ����B

	extern TBUshouDaniPowerInfo BPower;	// ���̊���
	extern TBUshouDaniPowerInfo WPower;	// ���̊���
}

// �V�ċL�ō��\�����Ă���t�H���g���𓾂�B
// ������΁A�l�r �����Ƃ���B
extern char* sgGetTenShougiFontName();

// �V�ċL���瑗���Ă��Ă���R�}���h�����o���B
std::string PopCommand();

// �V�ċL�v���Z�X����V�����v���Z�X�ւƒl���R�s�[����B
void LoadTenshouHandle();

// �m�ۂ����v���Z�X�n���h���̉��
void FreeTenshouHandle();

// �V�ċL���V�����ւƃ��������R�s�[
void CopyTenshoukiProcessToTenshougiProcess();