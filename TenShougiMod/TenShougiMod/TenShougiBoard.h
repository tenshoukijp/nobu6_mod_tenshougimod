#pragma once

#include "TenShougiMod.h"

namespace TenShougiBoard {
	// �{�[�h�𕽎�z�u�ŏ�����
	void BoardAllInitialize();

	// �{�[�h�̃X���b�g�̒l�𓾂�B���オ0, 0
	std::string GetBoardSlot( int x, int y );

	// ����� xy�ɂ݂��Ă�0, 0 �Ƃ݂Ȃ����ꍇ�ŁA�Ԗ�(77�Ƃ�32�Ƃ�)�𕶎���œ���
	std::string GetBoardMatrix( int x, int y );

	// �Ճ}�g���b�N�X��0��̈��̔z��ԍ���
	int GetStringIndexToArrayIndex(string from);

	// �w��̕��ʂ̋�����ւ̕ϊ�������𓾂�
	std::string GetNormalKomaToNarikoma(string normalkoma);

	// �X���b�g���̋�𓮂����BCSA�^�̕������n���B
	BOOL MoveBoardSlot( string CSAstring, char b_or_w );

	// ���̑Ώۋ��̌��𓾂�
	int GetBStandSlot( string koma );

	// ���̑Ώۋ��̌��𓾂�
	int GetWStandSlot( string koma );

	void SetCurBTurn(); // ���ݐ��Ԃł���B(���͎w���Ȃ�) 
	void SetCurWTurn(); // ���݌��Ԃł���B(���͎w���Ȃ�) 
	void SetCurBResign(); // ���ݐ�蓊���ł���B(�����w���Ȃ�) 
	void SetCurWResign(); // ���݌�蓊���ł���B(�����͎w���Ȃ�) 
	void SetCurPerPetual();

	int GetCurTurnState(); // ���ݐ��ԁH ���ԁH�@

	enum TurnState { CurTurnB = 0, CurTurnW = 1, CurTurnResignB = 2, CurTurnResignW = 3, CurTurnPerPetual=4 };
}