#pragma once

#include "TenShougiMod.h"

#include <string>
#include <vector>
#include <map>
#include <sstream>

using namespace std;

// TenShougiPipe���̃w�b�_
extern BOOL bTenShougiRunThread;

extern BOOL GetAndSendInputThread(String^ text); // ���M�{�^�����������Ɏ��s����֐�
extern void WINAPI TenShougiModBgn();
extern void WINAPI TenShougiModEnd();

extern CRITICAL_SECTION cs; // �X���b�h�����݂���̂ŃN���e�B�J���Z�N�V����(�r������)
extern vector<string> bufBonanzaAllOutput;  // �{�i���U�̕W���o�͑S�Ă�z��Ŏ���������
extern vector<string> bufAllKifu;			// �v���C���ƃ{�i���U�̊���
extern string bufPlayerKifuLastest;		    // �v���C���[���Ō�ɓ��͂�������
extern double fCurKeiseiPoint;				// �����A�ǂ��炪�L�����B300���炢�Ő�]�I�B��肪+ ��肪-

extern std::string DotNetStringToMultiByte(String^ dotnettext);
extern vector<string> string_split(const string &str, char delim);

extern void ReNewTenShougiGame();	// �����̍ăX�^�[�g
extern void SaveCSAKifu();
extern void LoadCSAKifu();