#pragma once

#include "TenShougiMod.h"
#include "TenShougiBoard.h"
#include "SharedMemory.h"

#include <sstream>



// �}�W�b�N�i���o�[���������A�����̏ꍇ�A�i�v�ɕς��Ȃ��̂œ��ɖ��Ȃ��B���̃N���X���̂��̂�const�萔�{�X���b�g�݂����Ȃ��̂��B

// �Ղ̍ŏ��̏��
static char *pBoardHirate[9*9] = {
	"-KY", "-KE", "-GI", "-KI", "-OU", "-KI", "-GI", "-KE", "-KY",
	 "*",  "-HI", "*",    "*",   "*",   "*",   "*",  "-KA",  "*",  
	"-FU", "-FU", "-FU", "-FU", "-FU", "-FU", "-FU", "-FU", "-FU",
	 "*",   "*",   "*",   "*",   "*",   "*",   "*",   "*",   "*",  
	 "*",   "*",   "*",   "*",   "*",   "*",   "*",   "*",   "*",  
	 "*",   "*",   "*",   "*",   "*",   "*",   "*",   "*",   "*",  
	"+FU", "+FU", "+FU", "+FU", "+FU", "+FU", "+FU", "+FU", "+FU",
	 "*",  "+KA",  "*",   "*",   "*",   "*",   "*",  "+HI",  "*",  
	"+KY", "+KE", "+GI", "+KI", "+OU", "+KI", "+GI", "+KE", "+KY"
};


static std::string BoardSlot[9][9] = {""}; // 9x9�̔Ղ̃X���b�g
static std::map<std::string, int> BStandSlot;  // ���̋��̃X���b�g�B�ő��40 ( �����ɂ͎����̉��l��������39)
static std::map<std::string, int> WStandSlot;  // ���̋��̃X���b�g�B�ő��40 ( �����ɂ͎����̉��l��������39)

// �{�[�h�𕽎�z�u�ŏ�����
static void InitBoardSlotByHirate() {
	// ��u������S��������
	BStandSlot.clear();
	WStandSlot.clear();

	// �Ղ̏�����Ԃ��R�s�[
	for (int row=0; row<9; row++) {
		for(int col=0; col<9; col++) {
			std::string koma = std::string(pBoardHirate[row*9+col]);

			BoardSlot[row][col] = koma;

			// ����ŉ��ȊO�̋�Ȃ�A���̎�ނ̎�������J�E���g0�Ƃ��ċ��ɓo�^����B
			if ( koma[0] == '+' && koma != "+OU" ) {
				BStandSlot[koma] = 0;
			}
			// ����ŉ��ȊO�̋�Ȃ�A���̎�ނ̎�������J�E���g0�Ƃ��ċ��ɓo�^����B
			else if ( koma[0] == '-' && koma != "-OU" ) {
				WStandSlot[koma] = 0;
			}

		}
	}
}


// 77FU�́u77�v�Ȃǂ��A�z���index�ւƕϊ�����}�b�v
static map<std::string, int> mapStringIndexToArrayIndex;

// ��́u77�Ȃǂ̔Փǂݏグ�p��index�v�ւ̕ϊ��ԍ��̑���B
static void InitStringIndexToArrayIndex() {

	// ���łɉ��������Ă���A�������Ȃ�
	if ( !mapStringIndexToArrayIndex.empty() ) {
		return;
	}

	int iIndexOfpBoardHirate = 0;

	// ���オ�u���v�B�E�����u���v
	for (int row=1; row<=9; row++) {
		for ( int col=9; col>=1; col--) {
			stringstream ban_pos;
			ban_pos << col << row; // �Ղ��ƁA��ԍ����X�A��ԉE���P�ŁAcol���ɁArow�����
			std::string bpos = ban_pos.str();

			// pStringIndexToArrayIndex["73"] = 
			mapStringIndexToArrayIndex[bpos] = iIndexOfpBoardHirate;
			if ( iIndexOfpBoardHirate < 9*9) {
				iIndexOfpBoardHirate++;
			}

		}
	}

	// �ł����܂̏ꍇ�p�B-1��Ԃ��Ă����B
	mapStringIndexToArrayIndex["00"] = -1;
}

// �����ʂ̋�ւ̃}�b�s���O
static map<std::string, std::string> mapNarikomaToNormalKoma;
static map<std::string, std::string> mapNormalKomaToNarikoma;

// ��́u77�Ȃǂ̔Փǂݏグ�p��index�v�ւ̕ϊ��ԍ��̑���B
static void InitNarikomaToNormalKoma() {

	// ���łɉ��������Ă���A�������Ȃ�
	if ( !mapNarikomaToNormalKoma.empty() ) {
		return;
	}

	// ������ʂ̃}�b�s���O
	mapNarikomaToNormalKoma["+NG"] = "+GI";
	mapNarikomaToNormalKoma["+NK"] = "+KE";
	mapNarikomaToNormalKoma["+NY"] = "+KY";
	mapNarikomaToNormalKoma["+TO"] = "+FU";
	mapNarikomaToNormalKoma["+RY"] = "+HI";
	mapNarikomaToNormalKoma["+UM"] = "+KA";
	mapNarikomaToNormalKoma["+TO"] = "+FU";

	mapNarikomaToNormalKoma["-NG"] = "-GI";
	mapNarikomaToNormalKoma["-NK"] = "-KE";
	mapNarikomaToNormalKoma["-NY"] = "-KY";
	mapNarikomaToNormalKoma["-TO"] = "-FU";
	mapNarikomaToNormalKoma["-RY"] = "-HI";
	mapNarikomaToNormalKoma["-UM"] = "-KA";
	mapNarikomaToNormalKoma["-TO"] = "-FU";

	// ���ʁ�����̃}�b�s���O
	mapNormalKomaToNarikoma["+GI"] = "+NG";
	mapNormalKomaToNarikoma["+KE"] = "+NK";
	mapNormalKomaToNarikoma["+KY"] = "+NY";
	mapNormalKomaToNarikoma["+FU"] = "+TO";
	mapNormalKomaToNarikoma["+HI"] = "+RY";
	mapNormalKomaToNarikoma["+KA"] = "+UM";
	mapNormalKomaToNarikoma["+FU"] = "+TO";

	mapNormalKomaToNarikoma["-GI"] = "-NG";
	mapNormalKomaToNarikoma["-KE"] = "-NK";
	mapNormalKomaToNarikoma["-KY"] = "-NY";
	mapNormalKomaToNarikoma["-FU"] = "-TO";
	mapNormalKomaToNarikoma["-HI"] = "-RY";
	mapNormalKomaToNarikoma["-KA"] = "-UM";
	mapNormalKomaToNarikoma["-FU"] = "-TO";

}

// ���O��ԁB�ʏ�.NET�p�R���|�[�l���g�ł́ADll�Ɠ������O�̋�Ԃɓ���Ă����ƁA�Q�Ƃ���ۂɓs�����ǂ��B
namespace TenShougiBoard {

	// �{�[�h�֘A�S��������
	void BoardAllInitialize() {

		// ���Ԃŏ�����
		SetCurBTurn();

		//  77FU�́u77�v�Ȃǂ��A�z���index�ւƕϊ�����}�b�v���������B
		InitStringIndexToArrayIndex();

		// �����ʋ�ւ̃}�b�s���O
		InitNarikomaToNormalKoma();

		// �{�[�h�𕽎�z�u�ŏ�����
		InitBoardSlotByHirate();
	}

	// ����� xy�ɂ݂��Ă�0, 0 �Ƃ݂Ȃ����ꍇ�ŁA�X���b�g�̒l�𓾂�B
	// �v���O�����ŃA�N�Z�X���₷���`
	string GetBoardSlot( int x, int y ) {
		return BoardSlot[y][x];
	}

	// ����� xy�ɂ݂��Ă�0, 0 �Ƃ݂Ȃ����ꍇ�ŁA�Ԗ�(77�Ƃ�32�Ƃ�)�𕶎���œ���
	string GetBoardMatrix( int x, int y ) {
		int yoko = 9-x;
		int tate = y+1;
		stringstream ss;
		ss << yoko << tate;
		string matrix = ss.str();
		return matrix;
	}


	// �w��̃X���b�g�Ɏw��̋��������B
	void SetBoardSlot( string koma, int x, int y ) {
		BoardSlot[y][x] = koma;
	}

	// �Ճ}�g���b�N�X��0��̈��̔z��ԍ���
	int GetStringIndexToArrayIndex(string from) {
		return mapStringIndexToArrayIndex[from];
	}

	// CSA�`��(7766FU)�Ȃǂ̖��߂ɏ]���ċ�𓮂����B
	BOOL MoveBoardSlot( string CSAstring, char b_or_w ) {

		// "7776FU" �Ƃ���"77","66", "FU" �ɕ�������B
		string from = string("")+CSAstring[0]+CSAstring[1];
		string to   = string("")+CSAstring[2]+CSAstring[3];
		string newkoma = string("")+CSAstring[4]+CSAstring[5];

		// ���̏ꏊ�̔z��̔ԍ��B
		int from_ix = mapStringIndexToArrayIndex[from];
		// ��̏ꏊ�̔z��̔ԍ��B
		int to_ix   = mapStringIndexToArrayIndex[to];

		// �ړ����̒����index��xy��index�^�ɒ���
		int from_y = from_ix / 9;
		int from_x = from_ix % 9;

		// �ړ���̒����index��xy��index�^�ɒ���
		int to_y = to_ix / 9;
		int to_x = to_ix % 9;

		// �ʏ�̋�ړ��̏ꍇ
		if ( from != "00" ) {

			string from_koma = GetBoardSlot( from_x, from_y); // �ړ����̋���
			string to_koma = GetBoardSlot( to_x, to_y);		  // �ړ���̋���

			// �ړ��悪�󔒂��ǂ����`�F�b�N
			if ( to_koma != "*" ) {
				// ����̋�������̂��̂ɂ����B
				string reverse_to_koma = to_koma; // ����̋�𕡐�����

				// �����育�܂Ȃ�A���ʂ̋�ɒ���
				if (mapNarikomaToNormalKoma[reverse_to_koma] != "") {
					reverse_to_koma = mapNarikomaToNormalKoma[reverse_to_koma];
				}

				// ����𔽑΂̋�Ƃ���B
				// ��肪���̋���Ƃ�����
				if ( reverse_to_koma[0] == '-' ) {
					// ���̋����̂��̂ɂ��āc
					reverse_to_koma[0] = '+';

					// ���̎w��̋��ނƂ��Đ��𑫂�
					BStandSlot[reverse_to_koma]++;

				// ��肪���̋���Ƃ�����
				} else if (reverse_to_koma[0] == '+' ) {
					// ���̋�����̂��̂ɂ��āc
					reverse_to_koma[0] = '-';

					// ���̋�Ƃ��ċ��ɒu��
					WStandSlot[reverse_to_koma]++;

				}
			}

			// �ړ����͕K���󔒂ɂȂ�B
			SetBoardSlot("*" , from_x, from_y);
			// �ړ���ɁA����ړ�������B����Ȃǂ�����̂�newkoma�̕����g���B
			SetBoardSlot(b_or_w+newkoma , to_x, to_y);

		}

		// �ł���̏ꍇ
		else if ( from == "00" ) {

			// ���ԂȂ�΁c
			if ( b_or_w == '+' ) {
				// ���̑Ώێ�̋���P���炷�B
				BStandSlot[b_or_w+newkoma]--;
			}
			// ���ԂȂ�΁c
			else if  ( b_or_w == '-' ) {
				// ���̑Ώێ�̋���P���炷�B
				WStandSlot[b_or_w+newkoma]--;
			}

			// �ړ���ɁA����ړ�������B����Ȃǂ�����̂�newkoma�̕����g���B
			SetBoardSlot(b_or_w+newkoma , to_x, to_y);

		}
		
		return true;
	}

	// ���̑Ώۋ��̌��𓾂�
	int GetBStandSlot( string koma ) {
		return BStandSlot[koma];
	}

	// ���̑Ώۋ��̌��𓾂�
	int GetWStandSlot( string koma ) {
		return WStandSlot[koma];
	}

	// ���݂̏�����ԂƂ���(���͎w���Ȃ�)
	void SetCurBTurn() {
		SharedMemory::iCurrentTurnState = TenShougiBoard::CurTurnB; // ���L����������
	}

	// ���݂̏�����ԂƂ���(���͎w���Ȃ�)
	void SetCurWTurn() {
		SharedMemory::iCurrentTurnState = TenShougiBoard::CurTurnW; // ���L����������
	}

	// ���݂́A��蓊�������B(�N���w���Ȃ�)
	void SetCurBResign() {
		SharedMemory::iCurrentTurnState = TenShougiBoard::CurTurnResignB; // ���L����������
	}

	// ���݂́A��蓊�������B(�N���w���Ȃ�)
	void SetCurWResign() {
		SharedMemory::iCurrentTurnState = TenShougiBoard::CurTurnResignW; // ���L����������
	}

	// �����
	void SetCurPerPetual() {
		SharedMemory::iCurrentTurnState = TenShougiBoard::CurTurnPerPetual; // ���L����������
	}


	// ���ԁE���ԁE����Ƃ���蓊���H ��蓊���H
	int GetCurTurnState() {
		return SharedMemory::iCurrentTurnState; // ���L����������
	}

	// �w��̕��ʂ̋�����ւ̕ϊ�������𓾂�
	string GetNormalKomaToNarikoma(string normalkoma) {
		return mapNormalKomaToNarikoma[normalkoma];
	}
}


