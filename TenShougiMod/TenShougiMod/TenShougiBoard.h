#pragma once

#include "TenShougiMod.h"

namespace TenShougiBoard {
	// ボードを平手配置で初期化
	void BoardAllInitialize();

	// ボードのスロットの値を得る。左上が0, 0
	std::string GetBoardSlot( int x, int y );

	// 左上を xyにみたてて0, 0 とみなした場合で、番目(77とか32とか)を文字列で得る
	std::string GetBoardMatrix( int x, int y );

	// 盤マトリックス→0基準の一列の配列番号へ
	int GetStringIndexToArrayIndex(string from);

	// 指定の普通の駒→成り駒への変換文字列を得る
	std::string GetNormalKomaToNarikoma(string normalkoma);

	// スロット内の駒を動かす。CSA型の文字列を渡す。
	BOOL MoveBoardSlot( string CSAstring, char b_or_w );

	// 先手の対象駒種の個数を得る
	int GetBStandSlot( string koma );

	// 後手の対象駒種の個数を得る
	int GetWStandSlot( string koma );

	void SetCurBTurn(); // 現在先手番である。(後手は指せない) 
	void SetCurWTurn(); // 現在後手番である。(先手は指せない) 
	void SetCurBResign(); // 現在先手投了である。(両方指せない) 
	void SetCurWResign(); // 現在後手投了である。(両方は指せない) 
	void SetCurPerPetual();

	int GetCurTurnState(); // 現在先手番？ 後手番？　

	enum TurnState { CurTurnB = 0, CurTurnW = 1, CurTurnResignB = 2, CurTurnResignW = 3, CurTurnPerPetual=4 };
}