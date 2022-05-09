#pragma once

#include "TenShougiMod.h"
#include "TenShougiBoard.h"
#include "SharedMemory.h"

#include <sstream>



// マジックナンバーが多いが、将棋の場合、永久に変わらないので特に問題なし。このクラスそのものがconst定数＋スロットみたいなものだ。

// 盤の最初の状態
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


static std::string BoardSlot[9][9] = {""}; // 9x9の盤のスロット
static std::map<std::string, int> BStandSlot;  // 先手の駒台のスロット。最大で40 ( 厳密には自分の王様を除いた39)
static std::map<std::string, int> WStandSlot;  // 後手の駒台のスロット。最大で40 ( 厳密には自分の王様を除いた39)

// ボードを平手配置で初期化
static void InitBoardSlotByHirate() {
	// 駒置き台も全部初期化
	BStandSlot.clear();
	WStandSlot.clear();

	// 盤の初期状態をコピー
	for (int row=0; row<9; row++) {
		for(int col=0; col<9; col++) {
			std::string koma = std::string(pBoardHirate[row*9+col]);

			BoardSlot[row][col] = koma;

			// 先手駒で王以外の駒なら、その種類の持ち駒をカウント0として駒台に登録する。
			if ( koma[0] == '+' && koma != "+OU" ) {
				BStandSlot[koma] = 0;
			}
			// 後手駒で王以外の駒なら、その種類の持ち駒をカウント0として駒台に登録する。
			else if ( koma[0] == '-' && koma != "-OU" ) {
				WStandSlot[koma] = 0;
			}

		}
	}
}


// 77FUの「77」などを、配列のindexへと変換するマップ
static map<std::string, int> mapStringIndexToArrayIndex;

// 上の「77などの盤読み上げ用→index」への変換番号の代入。
static void InitStringIndexToArrayIndex() {

	// すでに何か入ってたら、何もしない
	if ( !mapStringIndexToArrayIndex.empty() ) {
		return;
	}

	int iIndexOfpBoardHirate = 0;

	// 左上が「九一」。右下が「一九」
	for (int row=1; row<=9; row++) {
		for ( int col=9; col>=1; col--) {
			stringstream ban_pos;
			ban_pos << col << row; // 盤だと、一番左が９、一番右が１で、colを先に、rowを後に
			std::string bpos = ban_pos.str();

			// pStringIndexToArrayIndex["73"] = 
			mapStringIndexToArrayIndex[bpos] = iIndexOfpBoardHirate;
			if ( iIndexOfpBoardHirate < 9*9) {
				iIndexOfpBoardHirate++;
			}

		}
	}

	// 打ちごまの場合用。-1を返しておく。
	mapStringIndexToArrayIndex["00"] = -1;
}

// 鳴り駒→普通の駒へのマッピング
static map<std::string, std::string> mapNarikomaToNormalKoma;
static map<std::string, std::string> mapNormalKomaToNarikoma;

// 上の「77などの盤読み上げ用→index」への変換番号の代入。
static void InitNarikomaToNormalKoma() {

	// すでに何か入ってたら、何もしない
	if ( !mapNarikomaToNormalKoma.empty() ) {
		return;
	}

	// 成駒→普通のマッピング
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

	// 普通→成駒のマッピング
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

// 名前空間。通常.NET用コンポーネントでは、Dllと同じ名前の空間に入れておくと、参照する際に都合が良い。
namespace TenShougiBoard {

	// ボード関連全部初期化
	void BoardAllInitialize() {

		// 先手番で初期化
		SetCurBTurn();

		//  77FUの「77」などを、配列のindexへと変換するマップを初期化。
		InitStringIndexToArrayIndex();

		// 鳴り駒→普通駒へのマッピング
		InitNarikomaToNormalKoma();

		// ボードを平手配置で初期化
		InitBoardSlotByHirate();
	}

	// 左上を xyにみたてて0, 0 とみなした場合で、スロットの値を得る。
	// プログラムでアクセスしやすい形
	string GetBoardSlot( int x, int y ) {
		return BoardSlot[y][x];
	}

	// 左上を xyにみたてて0, 0 とみなした場合で、番目(77とか32とか)を文字列で得る
	string GetBoardMatrix( int x, int y ) {
		int yoko = 9-x;
		int tate = y+1;
		stringstream ss;
		ss << yoko << tate;
		string matrix = ss.str();
		return matrix;
	}


	// 指定のスロットに指定の駒を代入する。
	void SetBoardSlot( string koma, int x, int y ) {
		BoardSlot[y][x] = koma;
	}

	// 盤マトリックス→0基準の一列の配列番号へ
	int GetStringIndexToArrayIndex(string from) {
		return mapStringIndexToArrayIndex[from];
	}

	// CSA形式(7766FU)などの命令に従って駒を動かす。
	BOOL MoveBoardSlot( string CSAstring, char b_or_w ) {

		// "7776FU" とかを"77","66", "FU" に分解する。
		string from = string("")+CSAstring[0]+CSAstring[1];
		string to   = string("")+CSAstring[2]+CSAstring[3];
		string newkoma = string("")+CSAstring[4]+CSAstring[5];

		// 元の場所の配列の番号。
		int from_ix = mapStringIndexToArrayIndex[from];
		// 先の場所の配列の番号。
		int to_ix   = mapStringIndexToArrayIndex[to];

		// 移動元の直列のindexをxyのindex型に直す
		int from_y = from_ix / 9;
		int from_x = from_ix % 9;

		// 移動先の直列のindexをxyのindex型に直す
		int to_y = to_ix / 9;
		int to_x = to_ix % 9;

		// 通常の駒移動の場合
		if ( from != "00" ) {

			string from_koma = GetBoardSlot( from_x, from_y); // 移動元の駒情報
			string to_koma = GetBoardSlot( to_x, to_y);		  // 移動先の駒情報

			// 移動先が空白かどうかチェック
			if ( to_koma != "*" ) {
				// 相手の駒を自分のものにした。
				string reverse_to_koma = to_koma; // 相手の駒を複製して

				// もし鳴りごまなら、普通の駒に直す
				if (mapNarikomaToNormalKoma[reverse_to_koma] != "") {
					reverse_to_koma = mapNarikomaToNormalKoma[reverse_to_koma];
				}

				// 立場を反対の駒とする。
				// 先手が後手の駒をとったら
				if ( reverse_to_koma[0] == '-' ) {
					// 後手の駒を先手のものにして…
					reverse_to_koma[0] = '+';

					// 先手の指定の駒種類として数を足す
					BStandSlot[reverse_to_koma]++;

				// 後手が先手の駒をとったら
				} else if (reverse_to_koma[0] == '+' ) {
					// 先手の駒を後手のものにして…
					reverse_to_koma[0] = '-';

					// 後手の駒として駒台に置く
					WStandSlot[reverse_to_koma]++;

				}
			}

			// 移動元は必ず空白になる。
			SetBoardSlot("*" , from_x, from_y);
			// 移動先に、駒を移動させる。成りなどもあるのでnewkomaの方を使う。
			SetBoardSlot(b_or_w+newkoma , to_x, to_y);

		}

		// 打ち駒の場合
		else if ( from == "00" ) {

			// 先手番ならば…
			if ( b_or_w == '+' ) {
				// 駒台の対象種の駒を１つ減らす。
				BStandSlot[b_or_w+newkoma]--;
			}
			// 後手番ならば…
			else if  ( b_or_w == '-' ) {
				// 駒台の対象種の駒を１つ減らす。
				WStandSlot[b_or_w+newkoma]--;
			}

			// 移動先に、駒を移動させる。成りなどもあるのでnewkomaの方を使う。
			SetBoardSlot(b_or_w+newkoma , to_x, to_y);

		}
		
		return true;
	}

	// 先手の対象駒種の個数を得る
	int GetBStandSlot( string koma ) {
		return BStandSlot[koma];
	}

	// 後手の対象駒種の個数を得る
	int GetWStandSlot( string koma ) {
		return WStandSlot[koma];
	}

	// 現在の情報を先手番とする(後手は指せない)
	void SetCurBTurn() {
		SharedMemory::iCurrentTurnState = TenShougiBoard::CurTurnB; // 共有メモリ注意
	}

	// 現在の情報を先手番とする(後手は指せない)
	void SetCurWTurn() {
		SharedMemory::iCurrentTurnState = TenShougiBoard::CurTurnW; // 共有メモリ注意
	}

	// 現在は、後手投了した。(誰も指せない)
	void SetCurBResign() {
		SharedMemory::iCurrentTurnState = TenShougiBoard::CurTurnResignB; // 共有メモリ注意
	}

	// 現在は、先手投了した。(誰も指せない)
	void SetCurWResign() {
		SharedMemory::iCurrentTurnState = TenShougiBoard::CurTurnResignW; // 共有メモリ注意
	}

	// 千日手
	void SetCurPerPetual() {
		SharedMemory::iCurrentTurnState = TenShougiBoard::CurTurnPerPetual; // 共有メモリ注意
	}


	// 先手番・後手番・それとも先手投了？ 後手投了？
	int GetCurTurnState() {
		return SharedMemory::iCurrentTurnState; // 共有メモリ注意
	}

	// 指定の普通の駒→成り駒への変換文字列を得る
	string GetNormalKomaToNarikoma(string normalkoma) {
		return mapNormalKomaToNarikoma[normalkoma];
	}
}


