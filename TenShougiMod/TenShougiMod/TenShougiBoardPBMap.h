
#pragma once


#include "TenShougiMod.h"
#include "TenShougiKoma.h"
#include "TenShougiYesNoForm.h"
#include <sstream>


namespace TenShougiMod {

	// 9x9の将棋盤の左上基準
	static const int iBoardTopThreshold = 41;       // 将棋の駒置く時のTopの開始ピクセル座標
	static const int iBoardLeftThreshold = 105;     // 将棋の駒置く時のLeftの開始ピクセル座標

	// 後手の駒台の一番下の駒の基準
	static const int iWKomadaiTopThreshold = 233;    // 後手の駒台のTopの開始ピクセル座標
	static const int iWKomadaiLeftThreshold = 34;     // 後手の駒台のLeftの開始ピクセル座標

	static const int iBKomadaiTopThreshold = 105;    // 後手の駒台のTopの開始ピクセル座標
	static const int iBKomadaiLeftThreshold = 408;     // 後手の駒台のLeftの開始ピクセル座標

	static std::string strBKomaNameOnKomaDai[7] = { "+FU", "+KY", "+KE", "+GI", "+KI", "+KA", "+HI" };
	static std::string strWKomaNameOnKomaDai[7] = { "-FU", "-KY", "-KE", "-GI", "-KI", "-KA", "-HI" };


	// 天翔記の駒イメージのクラス。
	// 駒の名前→イメージへの変換用
	ref class TenShougiBoadPBMap : Form {

	private:
		Form ^form;
		TenShougiKomaImage^ tski; // 駒の画像イメージ集
		cli::array<PictureBox^, 2>^ mtrxShogiBoard; // 盤上の駒達
		cli::array<PictureBox^>^ arrayBShogiDai; // 先手の駒台
		cli::array<PictureBox^>^ arrayWShogiDai; // 後手の駒台

		cli::array<PictureBox^>^ arrayBShogiDaiCount; // 先手の駒台のカウンタ
		cli::array<PictureBox^>^ arrayWShogiDaiCount; // 後手の駒台のカウンタ

	public:
		TenShougiBoadPBMap(Form^ form) { // C++/CLI型の参照渡し
			this->form = form;


			// 先手のCSA命令の初期化
			InitBCSAstring();

			// 適当に１つ駒リソースを見て、駒の幅・高さを得る
			tski = gcnew TenShougiKomaImage();

			Bitmap^ img = (Bitmap^)(tski->htKomaBitmap["+OU"]);
			int w = img->Width;
			int h = img->Height;

			// PictureBoxを99個作成。
			mtrxShogiBoard = gcnew cli::array<PictureBox^, 2>(9,9); // 9x9分用意
			for(int y=0; y<9;  y++) {
				for (int x=0; x<9; x++) {
					PictureBox^ pb = gcnew PictureBox();
					pb->Width = w;
					pb->Height = h;
					pb->Left = x * w + iBoardLeftThreshold;
					pb->Top = y * h + iBoardTopThreshold;
					pb->Image = (Bitmap^)(tski->htKomaBitmap["*"]);

					pb->BackColor = Color::Transparent; // 背景は透明

					// xy → 記譜での盤目
					String^ mytag = gcnew String("O"); // Object^型にして  // 盤上には「O」というシグナルを付ける
					mytag += System::Convert::ToString(x);
					mytag += System::Convert::ToString(y);
					pb->Tag = mytag; // タグとしてその情報を入れておく

					// 配列に足しこみ
					mtrxShogiBoard[y,x] = pb;
					mtrxShogiBoard[y,x]->MouseDown += gcnew MouseEventHandler(this, &TenShougiBoadPBMap::pbKoma_MouseDown);
					form->Controls->Add(pb);
				}
			}

			// 先手の駒台
			arrayBShogiDai = gcnew cli::array<PictureBox^>(7); // 7種分用意
			for (int i=0; i<arrayBShogiDai->Length; i++) {
				PictureBox^ pb = gcnew PictureBox();
				pb->Width = w;
				pb->Height = h;
				pb->Left = iBKomadaiLeftThreshold;
				pb->Top = i * h + iBKomadaiTopThreshold;
				pb->Image = (Bitmap^)(tski->htKomaBitmap["*"]);
				pb->BackColor = Color::Transparent; // 背景は透明

				String^ mytag = gcnew String("B"); // 先手の駒台にはBというしるしをつけておく
				mytag += System::Convert::ToString(i); // 数値→数字にして足し込む
				pb->Tag = mytag; // タグとしてその情報を入れておく

				// 配列に足しこみ
				arrayBShogiDai[i] = pb;
				arrayBShogiDai[i]->MouseDown += gcnew MouseEventHandler(this, &TenShougiBoadPBMap::pbKoma_MouseDown);

				form->Controls->Add(pb);
			}

			// 後手の駒台
			arrayWShogiDai = gcnew cli::array<PictureBox^>(7); // 7種分用意
			for (int i=0; i<arrayWShogiDai->Length; i++) {
				PictureBox^ pb = gcnew PictureBox();
				pb->Width = w;
				pb->Height = h;
				pb->Left =iWKomadaiLeftThreshold;
				pb->Top = -i * h + iWKomadaiTopThreshold;
				pb->Image = (Bitmap^)(tski->htKomaBitmap["*"]);
				pb->BackColor = Color::Transparent; // 背景は透明

				String^ mytag = gcnew String("W"); // 先手の駒台にはWというしるしをつけておく
				mytag += System::Convert::ToString(i); // 数値→数字にして足し込む
				pb->Tag = mytag; // タグとしてその情報を入れておく

				// 配列に足しこみ
				arrayWShogiDai[i] = pb;
				arrayWShogiDai[i]->MouseDown += gcnew MouseEventHandler(this, &TenShougiBoadPBMap::pbKoma_MouseDown);

				form->Controls->Add(pb);
			}

			// 先手の駒台のカウンタ
			arrayBShogiDaiCount = gcnew cli::array<PictureBox^>(7); // 7種分用意
			for (int i=0; i<arrayBShogiDaiCount->Length; i++) {
				PictureBox^ pb = gcnew PictureBox();
				pb->Width = 13;
				pb->Height = 11;
				pb->Left = iBKomadaiLeftThreshold + 31;
				pb->Top = i * h + iBKomadaiTopThreshold + 19;
				pb->BackColor = Color::Transparent; // 背景は透明]
				// 配列に足しこみ
				arrayBShogiDaiCount[i] = pb;

				form->Controls->Add(pb);
			}

			// 後手の駒台のカウンタ
			arrayWShogiDaiCount = gcnew cli::array<PictureBox^>(7); // 7種分用意
			for (int i=0; i<arrayWShogiDai->Length; i++) {
				PictureBox^ pb = gcnew PictureBox();
				pb->Width = 13;
				pb->Height = 11;
				pb->Left =iWKomadaiLeftThreshold + 31;
				pb->Top = -i * h + iWKomadaiTopThreshold + 17;
				pb->BackColor = Color::Transparent; // 背景は透明
				// 配列に足しこみ
				arrayWShogiDaiCount[i] = pb;

				form->Controls->Add(pb);
			}

		}

		// 将棋盤のスロット情報に従って、イメージを描画する。
		void UpdatePositionByBoardSlot() {

			// まず9x9の番の上の画像を、現状の駒情報に合わせて更新
			for ( int y=0; y<9; y++ ) {
				for ( int x=0; x<9; x++ ) {
					std::string strKomaMei = TenShougiBoard::GetBoardSlot(x, y); // そのスロットにある駒
					String^ objKomaMei = gcnew String(strKomaMei.c_str());
					// PictureBoxの画像として、それを当てる
					if ( mtrxShogiBoard[y, x]->Image != (Bitmap^)(tski->htKomaBitmap[objKomaMei]) ) {
						mtrxShogiBoard[y, x]->Image = (Bitmap^)(tski->htKomaBitmap[objKomaMei]);
					}
				}
			}


			// 先手の駒台の画像情報を、現状の駒情報に合わせて更新
			for ( int i=0; i<7; i++) {
				std::string strKomaMei = strBKomaNameOnKomaDai[i]; // 今回の駒の名前
				String^ objKomaMei = gcnew String(strKomaMei.c_str());
				// 先手の対象駒種の個数を得る
				int iKomaCount = TenShougiBoard::GetBStandSlot( strKomaMei );
				if (iKomaCount==0) {
					objKomaMei = "*";
				}
				// 画像が異なれば、駒に合わせて画像を更新
				if ( arrayBShogiDai[i]->Image != (Bitmap^)(tski->htKomaBitmap[objKomaMei]) ) {
					arrayBShogiDai[i]->Image = (Bitmap^)(tski->htKomaBitmap[objKomaMei]);
				}
				// 数値を数字に]
				String^ objKomaCount = System::Convert::ToString(iKomaCount);
				// 駒の数の画像が異なれば、画像を更新。
				if ( arrayBShogiDaiCount[i]->Image != (Bitmap^)(tski->htKomaCount[objKomaCount]) ) {
					arrayBShogiDaiCount[i]->Image = (Bitmap^)(tski->htKomaCount[objKomaCount]);
				}
			}


			// 後手の駒台の画像情報を、現状の駒情報に合わせて更新
			for ( int i=0; i<7; i++) {
				std::string strKomaMei = strWKomaNameOnKomaDai[i]; // 今回の駒の名前
				String^ objKomaMei = gcnew String(strKomaMei.c_str());
				// 先手の対象駒種の個数を得る
				int iKomaCount = TenShougiBoard::GetWStandSlot( strKomaMei );
				if (iKomaCount==0) {
					objKomaMei = "*";
				}
				// 画像が異なれば、駒に合わせて画像を更新
				if ( arrayWShogiDai[i]->Image != (Bitmap^)(tski->htKomaBitmap[objKomaMei]) ) {
					arrayWShogiDai[i]->Image = (Bitmap^)(tski->htKomaBitmap[objKomaMei]);
				}
				// 数値を数字に
				String^ objKomaCount = System::Convert::ToString(iKomaCount);
				// 駒の数の画像が異なれば、画像を更新。
				if ( arrayWShogiDaiCount[i]->Image != (Bitmap^)(tski->htKomaCount[objKomaCount]) ) {
					arrayWShogiDaiCount[i]->Image = (Bitmap^)(tski->htKomaCount[objKomaCount]);
				}
			}

			// 背景を全部クリア
			PaintSelectKomaBack( 0, 0, '*');
		}

	private:
		String^ BCSAstringFromTo; // 先手が作り上げるCSA命令(7776FUなど)
		String^ BCSAstringKoma;   // 先手が作り上げるCSA命令(7776FUなど)

		// 先手のCSA命令の初期化
		void InitBCSAstring() {
			BCSAstringFromTo = gcnew String("");
			BCSAstringKoma = gcnew String("");
		}


		// その場所に駒は移動することが出来るか？ komaは"+HI"などの文字列
		bool IsKomaCannotMoveThere(int from_x, int from_y, int to_x, int to_y, String^ koma) {

			// 移動先が味方駒。ダメ
			std::string to_koma = TenShougiBoard::GetBoardSlot(to_x, to_y); // 移動先に駒はある？
			if ( to_koma[0] == '+' ) {
				return false;
			}

			// 歩である。前に１つだけ
			if ( koma == "+FU" ) {
				// 列が同じで、縦が１つ少ない
				if ( (from_x == to_x) && (to_y == from_y-1) ) {
					return true;
				}
			}
			else if ( koma == "+KY" ) {
				// 列が同じで、元より前進している。
				if ( (from_x == to_x) && (to_y < from_y)) {

					// 元の駒の位置より１つ上から、移動先の駒より下のところまで、１つずつ上方に
					for ( int y = from_y - 1; y > to_y; y-- ) {
						std::string slot = TenShougiBoard::GetBoardSlot(to_x, y); // そのスロットに駒はある？
						if ( slot != "*" ) { // スロットの間に敵駒
							return false;
						}
					}

					return true;
				}
			}

			else if ( koma == "+KE" ) {
				// 列が同じ
				if ( (to_y == from_y-2) && ( (to_x == from_x-1) || (to_x == from_x+1) ) ) { // ２つ先に進み、左か右に１つズレル 
					return true;
				}
			}

			else if ( koma == "+GI" ) {
				// 移動先から移動元を引いた絶対値がx系、y系とも1以下、
				if ( (abs(to_x - from_x) <= 1 )&&(abs(to_y - from_y) <= 1) ) {

					// 真左、真右への移動はダメ
					if ( (to_x - from_x == 1 || to_x - from_x == -1) && to_y == from_y ) {
						return false;
					}
					// 真後ろ移動はダメ
					if (( to_x == from_x) && (to_y == from_y + 1) ) {
						return false;
					}
					// それ以外ならＯＫ
					return true;
				}
			}

			else if ( koma == "+KI" || koma == "+TO" || koma == "+NY" || koma == "+NK" || koma == "+NG" ) {
				// 移動先から移動元を引いた絶対値がx系、y系とも1以下、
				if ( abs(to_x - from_x) <= 1 && abs(to_y - from_y) <= 1 ) {
					// 右後ろに移動はダメ
					if ( to_x - from_x == 1 && to_y - from_y == 1 ) {
						return false;
					}
					// 左後ろに移動はダメ
					if ( to_x - from_x == -1 && to_y - from_y == 1 ) {
						return false;
					}
					// それ以外ならＯＫ
					return true;
				}
			}

			else if ( koma == "+KA" ) {
				// 斜めだとＯＫ
				if ( abs(from_x - to_x) == abs(from_y - to_y) ) {

					// 左上方向
					if ( to_x < from_x && to_y < from_y ) {

						// 元の駒の位置より１つ上から、移動先の駒より下のところまで、１つずつ上方に
						for ( int x=from_x-1, y=from_y-1; (x > to_x) && (y > to_y); x--, y-- ) {
							std::string slot = TenShougiBoard::GetBoardSlot(x, y); // そのスロットに駒はある？
							if ( slot != "*" ) { // スロットの間に敵駒
								return false;
							}
						}
					}
					// 右上方向
					else if ( from_x < to_x  && to_y < from_y ) {

						// 元の駒の位置より１つ上から、移動先の駒より下のところまで、１つずつ上方に
						for ( int x=from_x+1, y=from_y-1; (x < to_x) && (y > to_y); x++, y-- ) {
							std::string slot = TenShougiBoard::GetBoardSlot(x, y); // そのスロットに駒はある？
							if ( slot != "*" ) { // スロットの間に敵駒
								return false;
							}
						}
					}
					// 右下方向
					if ( from_x < to_x && from_y < to_y ) {

						// 元の駒の位置より１つ上から、移動先の駒より下のところまで、１つずつ上方に
						for ( int x=from_x+1, y=from_y+1; (x < to_x) && (y < to_y); x++, y++ ) {
							std::string slot = TenShougiBoard::GetBoardSlot(x, y); // そのスロットに駒はある？
							if ( slot != "*" ) { // スロットの間に敵駒
								return false;
							}
						}
					}
					// 左下方向
					if ( to_x < from_x && from_y < to_y ) {

						// 元の駒の位置より１つ上から、移動先の駒より下のところまで、１つずつ上方に
						for ( int x=from_x-1, y=from_y+1; (x > to_x) && (y < to_y); x--, y++ ) {
							std::string slot = TenShougiBoard::GetBoardSlot(x, y); // そのスロットに駒はある？
							if ( slot != "*" ) { // スロットの間に敵駒
								return false;
							}
						}
					}
					return true;
				}
			}

			else if ( koma == "+HI" ) {
				// 横が同じ、もしくは縦が同じ。
				if ( (from_x == to_x) || (from_y == to_y) ) {

					if ( to_y < from_y ) {
						// 元の駒の位置より１つ上から、移動先の駒より下のところまで、１つずつ上方に
						for ( int y = from_y - 1; y > to_y; y-- ) {
							std::string slot = TenShougiBoard::GetBoardSlot(to_x, y); // そのスロットに駒はある？
							if ( slot != "*" ) { // スロットの間に敵駒
								return false;
							}
						}
					} else if ( from_y < to_y ) {
						// 元の駒の位置より１つ上から、移動先の駒より下のところまで、１つずつ上方に
						for ( int y = from_y + 1; y < to_y; y++ ) {
							std::string slot = TenShougiBoard::GetBoardSlot(to_x, y); // そのスロットに駒はある？
							if ( slot != "*" ) { // スロットの間に敵駒
								return false;
							}
						}
					} else if ( to_x < from_x ) {
						// 元の駒の位置より１つ上から、移動先の駒より下のところまで、１つずつ上方に
						for ( int x = from_x - 1; x > to_x; x-- ) {
							std::string slot = TenShougiBoard::GetBoardSlot(x, to_y); // そのスロットに駒はある？
							if ( slot != "*" ) { // スロットの間に敵駒
								return false;
							}
						}
					} else if ( from_x < to_x ) {
						// 元の駒の位置より１つ上から、移動先の駒より下のところまで、１つずつ上方に
						for ( int x = from_x + 1; x < to_x; x++ ) {
							std::string slot = TenShougiBoard::GetBoardSlot(x, to_y); // そのスロットに駒はある？
							if ( slot != "*" ) { // スロットの間に敵駒
								return false;
							}
						}
					}

					return true;
				}
			}

			else if ( koma == "+RY" ) {
				// 移動先から移動元を引いた絶対値がx系、y系とも1以下、王と同じ範囲はtrue
				if ( abs(to_x - from_x) <= 1 && abs(to_y - from_y) <= 1 ) {
					return true;
				}

				// 横が同じ、もしくは縦が同じ。
				if ( (from_x == to_x) || (from_y == to_y) ) {

					if ( to_y < from_y ) {
						// 元の駒の位置より１つ上から、移動先の駒より下のところまで、１つずつ上方に
						for ( int y = from_y - 1; y > to_y; y-- ) {
							std::string slot = TenShougiBoard::GetBoardSlot(to_x, y); // そのスロットに駒はある？
							if ( slot != "*" ) { // スロットの間に敵駒
								return false;
							}
						}
					} else if ( from_y < to_y ) {
						// 元の駒の位置より１つ上から、移動先の駒より下のところまで、１つずつ上方に
						for ( int y = from_y + 1; y < to_y; y++ ) {
							std::string slot = TenShougiBoard::GetBoardSlot(to_x, y); // そのスロットに駒はある？
							if ( slot != "*" ) { // スロットの間に敵駒
								return false;
							}
						}
					} else if ( to_x < from_x ) {
						// 元の駒の位置より１つ上から、移動先の駒より下のところまで、１つずつ上方に
						for ( int x = from_x - 1; x > to_x; x-- ) {
							std::string slot = TenShougiBoard::GetBoardSlot(x, to_y); // そのスロットに駒はある？
							if ( slot != "*" ) { // スロットの間に敵駒
								return false;
							}
						}
					} else if ( from_x < to_x ) {
						// 元の駒の位置より１つ上から、移動先の駒より下のところまで、１つずつ上方に
						for ( int x = from_x + 1; x < to_x; x++ ) {
							std::string slot = TenShougiBoard::GetBoardSlot(x, to_y); // そのスロットに駒はある？
							if ( slot != "*" ) { // スロットの間に敵駒
								return false;
							}
						}
					}

					return true;
				}
			}
			else if ( koma == "+UM" ) {
				// 移動先から移動元を引いた絶対値がx系、y系とも1以下、
				if ( abs(to_x - from_x) <= 1 && abs(to_y - from_y) <= 1 ) {
					return true;
				}


				// 斜めだとＯＫ
				if ( abs(from_x - to_x) == abs(from_y - to_y) ) {

					// 左上方向
					if ( to_x < from_x && to_y < from_y ) {

						// 元の駒の位置より１つ上から、移動先の駒より下のところまで、１つずつ上方に
						for ( int x=from_x-1, y=from_y-1; (x > to_x) && (y > to_y); x--, y-- ) {
							std::string slot = TenShougiBoard::GetBoardSlot(x, y); // そのスロットに駒はある？
							if ( slot != "*" ) { // スロットの間に敵駒
								return false;
							}
						}
					}
					// 右上方向
					else if ( from_x < to_x  && to_y < from_y ) {

						// 元の駒の位置より１つ上から、移動先の駒より下のところまで、１つずつ上方に
						for ( int x=from_x+1, y=from_y-1; (x < to_x) && (y > to_y); x++, y-- ) {
							std::string slot = TenShougiBoard::GetBoardSlot(x, y); // そのスロットに駒はある？
							if ( slot != "*" ) { // スロットの間に敵駒
								return false;
							}
						}
					}
					// 右下方向
					if ( from_x < to_x && from_y < to_y ) {

						// 元の駒の位置より１つ上から、移動先の駒より下のところまで、１つずつ上方に
						for ( int x=from_x+1, y=from_y+1; (x < to_x) && (y < to_y); x++, y++ ) {
							std::string slot = TenShougiBoard::GetBoardSlot(x, y); // そのスロットに駒はある？
							if ( slot != "*" ) { // スロットの間に敵駒
								return false;
							}
						}
					}
					// 左下方向
					if ( to_x < from_x && from_y < to_y ) {

						// 元の駒の位置より１つ上から、移動先の駒より下のところまで、１つずつ上方に
						for ( int x=from_x-1, y=from_y+1; (x > to_x) && (y < to_y); x--, y++ ) {
							std::string slot = TenShougiBoard::GetBoardSlot(x, y); // そのスロットに駒はある？
							if ( slot != "*" ) { // スロットの間に敵駒
								return false;
							}
						}
					}
					return true;
				}

			}
			else if ( koma == "+OU" ) {
				// 移動先から移動元を引いた絶対値がx系、y系とも1以下、
				if ( abs(to_x - from_x) <= 1 && abs(to_y - from_y) <= 1 ) {
					return true;
				}
			}

			return false;
		}

		// 駒のMouseDownイベントハンドラ
		// マウスのボタンが押されたとき
		void pbKoma_MouseDown(Object^ sender, MouseEventArgs^ e) {

			// 左クリックではない。ダメ
			if ((e->Button & ::MouseButtons::Left) != ::MouseButtons::Left) {
				// クリア行為
				InitBCSAstring();
				return;
			}

			// 先手番ではない。ダメ
			if ( TenShougiBoard::GetCurTurnState() != TenShougiBoard::CurTurnB ) {
				// クリア行為
				InitBCSAstring();
				return;
			}

			// PictureBoxのTagにどの記譜Matrix(O76とかの位置)が入っているのかを調査。
			PictureBox^ pb = (PictureBox^)sender;
			String^ tag = (String^)pb->Tag;

			// 盤上の駒の元をクリックした
			if (tag[0] == 'O') {

				// xy型の値を取り出す
				int x = System::Convert::ToInt32(tag[1]);
				int y = System::Convert::ToInt32(tag[2]);
				x = x - '0';
				y = y - '0';

				// 該当の場所にあった元の駒が…
				std::string koma = TenShougiBoard::GetBoardSlot(x, y);

				// 自分のものであれば、今から動かす予定の駒
				if (koma[0] == '+') {
					InitBCSAstring();

					// xy座標→盤マトリックスへ
					std::string from = TenShougiBoard::GetBoardMatrix(x, y);

					// 移動元・移動先のCSA命令として構築開始
					BCSAstringFromTo = gcnew String(from.c_str());

					// 駒名を保存しておく
					BCSAstringKoma = gcnew String(koma.c_str());

					// 選択したものの背景を変更。
					PaintSelectKomaBack( x, y, 'O');


				// 自分以外のものであれば、移動先だ。
				} else if (koma[0] == '-' || koma[0] == '*' ) {

					// 移動元の文字列が空っぽならば、移動元を指定していない。何もしない。
					if ( BCSAstringFromTo->Length < 2 ) {
						return;
					}


					// 盤上から動かした際には、成るか成らないかの判断が出来る。
					if ( BCSAstringFromTo != "00" ) { // 移動元が駒台以外で、xy座標系で、y <= 2が移動先の時

						// なりの条件とは、「移動元」が3段目より上、か、もしくは、「移動先」が3段目より上。

						// 移動元の座標を求める
						// 移動元の文字列を .net型→C++型に
						std::string bcsastringfromto = DotNetStringToMultiByte(BCSAstringFromTo);
						// 移動元の配列
						int from_ix = TenShougiBoard::GetStringIndexToArrayIndex(bcsastringfromto);

						// 移動元の直列のindexをxyのindex型に直す
						int from_y = from_ix / 9;
						int from_x = from_ix % 9;

						// その場所には駒は移動することが出来ない
						if ( ! IsKomaCannotMoveThere( from_x, from_y, x, y, BCSAstringKoma ) ) {
							// 送ったら即座に初期化。
							InitBCSAstring();

							// 背景をリセット。
							PaintSelectKomaBack( x, y, 'X');

							return;
						}

						// 移動先が、3段目より上、もしくは、移動元が3段目より上
						if ( y <= 2 || from_y <= 2 ) {

							// 成駒名を.netタイプ→C++タイプへ
							std::string bcsastringkoma = DotNetStringToMultiByte(BCSAstringKoma);
							// 第の移動元の駒はなれる駒か？
							std::string narikoma = TenShougiBoard::GetNormalKomaToNarikoma(bcsastringkoma);

							// もしも、駒が成れる駒であれば… (成り済みは関係ない)
							if ( narikoma.size() > 1 ) { // この駒は成れる。

								// 成りが強制される状況かどうか。
								bool must_nari = false;

								// 歩か香車なら、1段目なら必ず成らなければならない。
								if ( y ==0 && (BCSAstringKoma == "+FU" || BCSAstringKoma == "+KY")  ) {
									must_nari = true;
								}
								// 桂馬なら、2段目より上なら必ず成らなければならない。
								else if ( y<=1 && (BCSAstringKoma == "+KE")  ) {
									must_nari = true;
								}

								// 成りが強制であるなら、成るだけ。
								if ( must_nari ) {

									// 駒を成り駒へと更新する。
									BCSAstringKoma = gcnew String(narikoma.c_str());

								// 成るか成らないか任意の状況なら、ダイアログで聞く
								} else {

									// 成りダイアログ表示
									TenShougiYesNoForm^ tsynf = gcnew TenShougiYesNoForm("成りますか？", "成る", "成らず");
									// ShowDialog表示時に、親フォームのまん中に表示する。
									tsynf->StartPosition = FormStartPosition::CenterParent;
									// ＹＥＳなら  (親フォームを指定しつつ)
									if (tsynf->ShowDialog(form) == ::DialogResult::Yes) {
										// 駒を成り駒へと更新する。
										BCSAstringKoma = gcnew String(narikoma.c_str());
									}
								}
							}
						}
					}

					// xy座標→盤マトリックスへ
					std::string to = TenShougiBoard::GetBoardMatrix(x, y);

					// 移動元・移動先のCSA命令として構築完了
					BCSAstringFromTo += gcnew String(to.c_str());

					// 背景を選択後の所に。
					// PaintSelectKomaBack( x, y, 'O');

					String^ command = BCSAstringFromTo+BCSAstringKoma->Substring(1,2);
					// BonanzaにSendする。
					GetAndSendInputThread(command);

					// 送ったら即座に初期化。
					InitBCSAstring();

				}
			}
			// 先手の駒第から指している
			else if (tag[0] == 'B') {

				// 駒台はスタート地点にしかならないので必ず初期化。
				InitBCSAstring();

				// PictureBoxに含まれるタグからi型の値を取り出す
				int i = System::Convert::ToInt32(tag[1]);
				i = i - '0';

				// 該当の番号に入っている駒種。
				std::string komamei = strBKomaNameOnKomaDai[i];

				// 先手の対象駒種の個数を得る
				int htKomaCount = TenShougiBoard::GetBStandSlot( komamei );
				// 0なら何もしない
				if (htKomaCount==0) {
					return;
				}

				// 移動元・移動先のCSA命令として構築開始
				BCSAstringFromTo = gcnew String("00");

				// 駒名を保存しておく
				BCSAstringKoma = gcnew String(komamei.c_str());

				// 背景を選択後の所に。
				PaintSelectKomaBack( i, 0, 'B');
			}

			// 後手の駒第から指している
			else if (tag[0] == 'W') {

				// クリア行為
				InitBCSAstring();

				// クリア行為
				return;
			}

		}


	private:
		// xy (もしくはi)座標と、wは、'O', 'B', 'W'の種別
		void PaintSelectKomaBack( int from_x, int from_y, char O_B_W ) {
			// 盤上の駒だけ塗り直し
			for(int y=0; y<9;  y++) {
				for (int x=0; x<9; x++) {

					PictureBox^ bp = (PictureBox^)mtrxShogiBoard[y, x];
					bp->BackgroundImage = (Bitmap^)tski->htKomaBitmap["*"]; // 背景は透明

					/*

					// 以下、駒の移動範囲に色を付ける処理!!
					// そのスロットの駒名を得る。
					std::string koma = TenShougiBoard::GetBoardSlot( from_x, from_y );
					String^ okoma = gcnew String(koma.c_str());
					if ( IsKomaCannotMoveThere( from_x, from_y, x, y, okoma ) ) {
						bp->BackgroundImage = tski->bmKomaSelectedBackImg; // 選択時用の背景
					}
					*/
				}
			}

			// 先手の駒台
			for (int i=0; i<arrayBShogiDai->Length; i++) {
				PictureBox^ bp = (PictureBox^)arrayBShogiDai[i];
				bp->BackgroundImage = (Bitmap^)tski->htKomaBitmap["*"]; // 背景は透明
			}

			for (int i=0; i<arrayWShogiDai->Length; i++) {
				PictureBox^ bp = (PictureBox^)arrayWShogiDai[i];
				bp->BackgroundImage = (Bitmap^)tski->htKomaBitmap["*"]; // 背景は透明
			}

			// 盤上の駒だった
			if ( O_B_W == 'O' ) {
				PictureBox^ bp = (PictureBox^)mtrxShogiBoard[from_y, from_x];
				bp->BackgroundImage = tski->bmKomaSelectedBackImg; // 選択時用の背景
			}

			// 盤上の駒だった
			if ( O_B_W == 'B' ) {
				PictureBox^ bp = (PictureBox^)arrayBShogiDai[from_x];
				bp->BackgroundImage = tski->bmKomaSelectedBackImg; // 選択時用の背景
			}

			// 盤上の駒だった
			if ( O_B_W == 'W' ) {
				PictureBox^ bp = (PictureBox^)arrayWShogiDai[from_x];
				bp->BackgroundImage = tski->bmKomaSelectedBackImg; // 選択時用の背景
			}
		}
	};
}
