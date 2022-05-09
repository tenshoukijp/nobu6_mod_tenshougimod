
#pragma once


#include "TenShougiMod.h"
#include "TenShougiKoma.h"
#include "TenShougiYesNoForm.h"
#include <sstream>


namespace TenShougiMod {

	// 9x9�̏����Ղ̍���
	static const int iBoardTopThreshold = 41;       // �����̋�u������Top�̊J�n�s�N�Z�����W
	static const int iBoardLeftThreshold = 105;     // �����̋�u������Left�̊J�n�s�N�Z�����W

	// ���̋��̈�ԉ��̋�̊
	static const int iWKomadaiTopThreshold = 233;    // ���̋���Top�̊J�n�s�N�Z�����W
	static const int iWKomadaiLeftThreshold = 34;     // ���̋���Left�̊J�n�s�N�Z�����W

	static const int iBKomadaiTopThreshold = 105;    // ���̋���Top�̊J�n�s�N�Z�����W
	static const int iBKomadaiLeftThreshold = 408;     // ���̋���Left�̊J�n�s�N�Z�����W

	static std::string strBKomaNameOnKomaDai[7] = { "+FU", "+KY", "+KE", "+GI", "+KI", "+KA", "+HI" };
	static std::string strWKomaNameOnKomaDai[7] = { "-FU", "-KY", "-KE", "-GI", "-KI", "-KA", "-HI" };


	// �V�ċL�̋�C���[�W�̃N���X�B
	// ��̖��O���C���[�W�ւ̕ϊ��p
	ref class TenShougiBoadPBMap : Form {

	private:
		Form ^form;
		TenShougiKomaImage^ tski; // ��̉摜�C���[�W�W
		cli::array<PictureBox^, 2>^ mtrxShogiBoard; // �Տ�̋�B
		cli::array<PictureBox^>^ arrayBShogiDai; // ���̋��
		cli::array<PictureBox^>^ arrayWShogiDai; // ���̋��

		cli::array<PictureBox^>^ arrayBShogiDaiCount; // ���̋��̃J�E���^
		cli::array<PictureBox^>^ arrayWShogiDaiCount; // ���̋��̃J�E���^

	public:
		TenShougiBoadPBMap(Form^ form) { // C++/CLI�^�̎Q�Ɠn��
			this->form = form;


			// ����CSA���߂̏�����
			InitBCSAstring();

			// �K���ɂP��\�[�X�����āA��̕��E�����𓾂�
			tski = gcnew TenShougiKomaImage();

			Bitmap^ img = (Bitmap^)(tski->htKomaBitmap["+OU"]);
			int w = img->Width;
			int h = img->Height;

			// PictureBox��99�쐬�B
			mtrxShogiBoard = gcnew cli::array<PictureBox^, 2>(9,9); // 9x9���p��
			for(int y=0; y<9;  y++) {
				for (int x=0; x<9; x++) {
					PictureBox^ pb = gcnew PictureBox();
					pb->Width = w;
					pb->Height = h;
					pb->Left = x * w + iBoardLeftThreshold;
					pb->Top = y * h + iBoardTopThreshold;
					pb->Image = (Bitmap^)(tski->htKomaBitmap["*"]);

					pb->BackColor = Color::Transparent; // �w�i�͓���

					// xy �� �L���ł̔Ֆ�
					String^ mytag = gcnew String("O"); // Object^�^�ɂ���  // �Տ�ɂ́uO�v�Ƃ����V�O�i����t����
					mytag += System::Convert::ToString(x);
					mytag += System::Convert::ToString(y);
					pb->Tag = mytag; // �^�O�Ƃ��Ă��̏������Ă���

					// �z��ɑ�������
					mtrxShogiBoard[y,x] = pb;
					mtrxShogiBoard[y,x]->MouseDown += gcnew MouseEventHandler(this, &TenShougiBoadPBMap::pbKoma_MouseDown);
					form->Controls->Add(pb);
				}
			}

			// ���̋��
			arrayBShogiDai = gcnew cli::array<PictureBox^>(7); // 7�핪�p��
			for (int i=0; i<arrayBShogiDai->Length; i++) {
				PictureBox^ pb = gcnew PictureBox();
				pb->Width = w;
				pb->Height = h;
				pb->Left = iBKomadaiLeftThreshold;
				pb->Top = i * h + iBKomadaiTopThreshold;
				pb->Image = (Bitmap^)(tski->htKomaBitmap["*"]);
				pb->BackColor = Color::Transparent; // �w�i�͓���

				String^ mytag = gcnew String("B"); // ���̋��ɂ�B�Ƃ������邵�����Ă���
				mytag += System::Convert::ToString(i); // ���l�������ɂ��đ�������
				pb->Tag = mytag; // �^�O�Ƃ��Ă��̏������Ă���

				// �z��ɑ�������
				arrayBShogiDai[i] = pb;
				arrayBShogiDai[i]->MouseDown += gcnew MouseEventHandler(this, &TenShougiBoadPBMap::pbKoma_MouseDown);

				form->Controls->Add(pb);
			}

			// ���̋��
			arrayWShogiDai = gcnew cli::array<PictureBox^>(7); // 7�핪�p��
			for (int i=0; i<arrayWShogiDai->Length; i++) {
				PictureBox^ pb = gcnew PictureBox();
				pb->Width = w;
				pb->Height = h;
				pb->Left =iWKomadaiLeftThreshold;
				pb->Top = -i * h + iWKomadaiTopThreshold;
				pb->Image = (Bitmap^)(tski->htKomaBitmap["*"]);
				pb->BackColor = Color::Transparent; // �w�i�͓���

				String^ mytag = gcnew String("W"); // ���̋��ɂ�W�Ƃ������邵�����Ă���
				mytag += System::Convert::ToString(i); // ���l�������ɂ��đ�������
				pb->Tag = mytag; // �^�O�Ƃ��Ă��̏������Ă���

				// �z��ɑ�������
				arrayWShogiDai[i] = pb;
				arrayWShogiDai[i]->MouseDown += gcnew MouseEventHandler(this, &TenShougiBoadPBMap::pbKoma_MouseDown);

				form->Controls->Add(pb);
			}

			// ���̋��̃J�E���^
			arrayBShogiDaiCount = gcnew cli::array<PictureBox^>(7); // 7�핪�p��
			for (int i=0; i<arrayBShogiDaiCount->Length; i++) {
				PictureBox^ pb = gcnew PictureBox();
				pb->Width = 13;
				pb->Height = 11;
				pb->Left = iBKomadaiLeftThreshold + 31;
				pb->Top = i * h + iBKomadaiTopThreshold + 19;
				pb->BackColor = Color::Transparent; // �w�i�͓���]
				// �z��ɑ�������
				arrayBShogiDaiCount[i] = pb;

				form->Controls->Add(pb);
			}

			// ���̋��̃J�E���^
			arrayWShogiDaiCount = gcnew cli::array<PictureBox^>(7); // 7�핪�p��
			for (int i=0; i<arrayWShogiDai->Length; i++) {
				PictureBox^ pb = gcnew PictureBox();
				pb->Width = 13;
				pb->Height = 11;
				pb->Left =iWKomadaiLeftThreshold + 31;
				pb->Top = -i * h + iWKomadaiTopThreshold + 17;
				pb->BackColor = Color::Transparent; // �w�i�͓���
				// �z��ɑ�������
				arrayWShogiDaiCount[i] = pb;

				form->Controls->Add(pb);
			}

		}

		// �����Ղ̃X���b�g���ɏ]���āA�C���[�W��`�悷��B
		void UpdatePositionByBoardSlot() {

			// �܂�9x9�̔Ԃ̏�̉摜���A����̋���ɍ��킹�čX�V
			for ( int y=0; y<9; y++ ) {
				for ( int x=0; x<9; x++ ) {
					std::string strKomaMei = TenShougiBoard::GetBoardSlot(x, y); // ���̃X���b�g�ɂ����
					String^ objKomaMei = gcnew String(strKomaMei.c_str());
					// PictureBox�̉摜�Ƃ��āA����𓖂Ă�
					if ( mtrxShogiBoard[y, x]->Image != (Bitmap^)(tski->htKomaBitmap[objKomaMei]) ) {
						mtrxShogiBoard[y, x]->Image = (Bitmap^)(tski->htKomaBitmap[objKomaMei]);
					}
				}
			}


			// ���̋��̉摜�����A����̋���ɍ��킹�čX�V
			for ( int i=0; i<7; i++) {
				std::string strKomaMei = strBKomaNameOnKomaDai[i]; // ����̋�̖��O
				String^ objKomaMei = gcnew String(strKomaMei.c_str());
				// ���̑Ώۋ��̌��𓾂�
				int iKomaCount = TenShougiBoard::GetBStandSlot( strKomaMei );
				if (iKomaCount==0) {
					objKomaMei = "*";
				}
				// �摜���قȂ�΁A��ɍ��킹�ĉ摜���X�V
				if ( arrayBShogiDai[i]->Image != (Bitmap^)(tski->htKomaBitmap[objKomaMei]) ) {
					arrayBShogiDai[i]->Image = (Bitmap^)(tski->htKomaBitmap[objKomaMei]);
				}
				// ���l�𐔎���]
				String^ objKomaCount = System::Convert::ToString(iKomaCount);
				// ��̐��̉摜���قȂ�΁A�摜���X�V�B
				if ( arrayBShogiDaiCount[i]->Image != (Bitmap^)(tski->htKomaCount[objKomaCount]) ) {
					arrayBShogiDaiCount[i]->Image = (Bitmap^)(tski->htKomaCount[objKomaCount]);
				}
			}


			// ���̋��̉摜�����A����̋���ɍ��킹�čX�V
			for ( int i=0; i<7; i++) {
				std::string strKomaMei = strWKomaNameOnKomaDai[i]; // ����̋�̖��O
				String^ objKomaMei = gcnew String(strKomaMei.c_str());
				// ���̑Ώۋ��̌��𓾂�
				int iKomaCount = TenShougiBoard::GetWStandSlot( strKomaMei );
				if (iKomaCount==0) {
					objKomaMei = "*";
				}
				// �摜���قȂ�΁A��ɍ��킹�ĉ摜���X�V
				if ( arrayWShogiDai[i]->Image != (Bitmap^)(tski->htKomaBitmap[objKomaMei]) ) {
					arrayWShogiDai[i]->Image = (Bitmap^)(tski->htKomaBitmap[objKomaMei]);
				}
				// ���l�𐔎���
				String^ objKomaCount = System::Convert::ToString(iKomaCount);
				// ��̐��̉摜���قȂ�΁A�摜���X�V�B
				if ( arrayWShogiDaiCount[i]->Image != (Bitmap^)(tski->htKomaCount[objKomaCount]) ) {
					arrayWShogiDaiCount[i]->Image = (Bitmap^)(tski->htKomaCount[objKomaCount]);
				}
			}

			// �w�i��S���N���A
			PaintSelectKomaBack( 0, 0, '*');
		}

	private:
		String^ BCSAstringFromTo; // ��肪���グ��CSA����(7776FU�Ȃ�)
		String^ BCSAstringKoma;   // ��肪���グ��CSA����(7776FU�Ȃ�)

		// ����CSA���߂̏�����
		void InitBCSAstring() {
			BCSAstringFromTo = gcnew String("");
			BCSAstringKoma = gcnew String("");
		}


		// ���̏ꏊ�ɋ�͈ړ����邱�Ƃ��o���邩�H koma��"+HI"�Ȃǂ̕�����
		bool IsKomaCannotMoveThere(int from_x, int from_y, int to_x, int to_y, String^ koma) {

			// �ړ��悪������B�_��
			std::string to_koma = TenShougiBoard::GetBoardSlot(to_x, to_y); // �ړ���ɋ�͂���H
			if ( to_koma[0] == '+' ) {
				return false;
			}

			// ���ł���B�O�ɂP����
			if ( koma == "+FU" ) {
				// �񂪓����ŁA�c���P���Ȃ�
				if ( (from_x == to_x) && (to_y == from_y-1) ) {
					return true;
				}
			}
			else if ( koma == "+KY" ) {
				// �񂪓����ŁA�����O�i���Ă���B
				if ( (from_x == to_x) && (to_y < from_y)) {

					// ���̋�̈ʒu���P�ォ��A�ړ���̋��艺�̂Ƃ���܂ŁA�P�������
					for ( int y = from_y - 1; y > to_y; y-- ) {
						std::string slot = TenShougiBoard::GetBoardSlot(to_x, y); // ���̃X���b�g�ɋ�͂���H
						if ( slot != "*" ) { // �X���b�g�̊ԂɓG��
							return false;
						}
					}

					return true;
				}
			}

			else if ( koma == "+KE" ) {
				// �񂪓���
				if ( (to_y == from_y-2) && ( (to_x == from_x-1) || (to_x == from_x+1) ) ) { // �Q��ɐi�݁A�����E�ɂP�Y���� 
					return true;
				}
			}

			else if ( koma == "+GI" ) {
				// �ړ��悩��ړ�������������Βl��x�n�Ay�n�Ƃ�1�ȉ��A
				if ( (abs(to_x - from_x) <= 1 )&&(abs(to_y - from_y) <= 1) ) {

					// �^���A�^�E�ւ̈ړ��̓_��
					if ( (to_x - from_x == 1 || to_x - from_x == -1) && to_y == from_y ) {
						return false;
					}
					// �^���ړ��̓_��
					if (( to_x == from_x) && (to_y == from_y + 1) ) {
						return false;
					}
					// ����ȊO�Ȃ�n�j
					return true;
				}
			}

			else if ( koma == "+KI" || koma == "+TO" || koma == "+NY" || koma == "+NK" || koma == "+NG" ) {
				// �ړ��悩��ړ�������������Βl��x�n�Ay�n�Ƃ�1�ȉ��A
				if ( abs(to_x - from_x) <= 1 && abs(to_y - from_y) <= 1 ) {
					// �E���Ɉړ��̓_��
					if ( to_x - from_x == 1 && to_y - from_y == 1 ) {
						return false;
					}
					// �����Ɉړ��̓_��
					if ( to_x - from_x == -1 && to_y - from_y == 1 ) {
						return false;
					}
					// ����ȊO�Ȃ�n�j
					return true;
				}
			}

			else if ( koma == "+KA" ) {
				// �΂߂��Ƃn�j
				if ( abs(from_x - to_x) == abs(from_y - to_y) ) {

					// �������
					if ( to_x < from_x && to_y < from_y ) {

						// ���̋�̈ʒu���P�ォ��A�ړ���̋��艺�̂Ƃ���܂ŁA�P�������
						for ( int x=from_x-1, y=from_y-1; (x > to_x) && (y > to_y); x--, y-- ) {
							std::string slot = TenShougiBoard::GetBoardSlot(x, y); // ���̃X���b�g�ɋ�͂���H
							if ( slot != "*" ) { // �X���b�g�̊ԂɓG��
								return false;
							}
						}
					}
					// �E�����
					else if ( from_x < to_x  && to_y < from_y ) {

						// ���̋�̈ʒu���P�ォ��A�ړ���̋��艺�̂Ƃ���܂ŁA�P�������
						for ( int x=from_x+1, y=from_y-1; (x < to_x) && (y > to_y); x++, y-- ) {
							std::string slot = TenShougiBoard::GetBoardSlot(x, y); // ���̃X���b�g�ɋ�͂���H
							if ( slot != "*" ) { // �X���b�g�̊ԂɓG��
								return false;
							}
						}
					}
					// �E������
					if ( from_x < to_x && from_y < to_y ) {

						// ���̋�̈ʒu���P�ォ��A�ړ���̋��艺�̂Ƃ���܂ŁA�P�������
						for ( int x=from_x+1, y=from_y+1; (x < to_x) && (y < to_y); x++, y++ ) {
							std::string slot = TenShougiBoard::GetBoardSlot(x, y); // ���̃X���b�g�ɋ�͂���H
							if ( slot != "*" ) { // �X���b�g�̊ԂɓG��
								return false;
							}
						}
					}
					// ��������
					if ( to_x < from_x && from_y < to_y ) {

						// ���̋�̈ʒu���P�ォ��A�ړ���̋��艺�̂Ƃ���܂ŁA�P�������
						for ( int x=from_x-1, y=from_y+1; (x > to_x) && (y < to_y); x--, y++ ) {
							std::string slot = TenShougiBoard::GetBoardSlot(x, y); // ���̃X���b�g�ɋ�͂���H
							if ( slot != "*" ) { // �X���b�g�̊ԂɓG��
								return false;
							}
						}
					}
					return true;
				}
			}

			else if ( koma == "+HI" ) {
				// ���������A�������͏c�������B
				if ( (from_x == to_x) || (from_y == to_y) ) {

					if ( to_y < from_y ) {
						// ���̋�̈ʒu���P�ォ��A�ړ���̋��艺�̂Ƃ���܂ŁA�P�������
						for ( int y = from_y - 1; y > to_y; y-- ) {
							std::string slot = TenShougiBoard::GetBoardSlot(to_x, y); // ���̃X���b�g�ɋ�͂���H
							if ( slot != "*" ) { // �X���b�g�̊ԂɓG��
								return false;
							}
						}
					} else if ( from_y < to_y ) {
						// ���̋�̈ʒu���P�ォ��A�ړ���̋��艺�̂Ƃ���܂ŁA�P�������
						for ( int y = from_y + 1; y < to_y; y++ ) {
							std::string slot = TenShougiBoard::GetBoardSlot(to_x, y); // ���̃X���b�g�ɋ�͂���H
							if ( slot != "*" ) { // �X���b�g�̊ԂɓG��
								return false;
							}
						}
					} else if ( to_x < from_x ) {
						// ���̋�̈ʒu���P�ォ��A�ړ���̋��艺�̂Ƃ���܂ŁA�P�������
						for ( int x = from_x - 1; x > to_x; x-- ) {
							std::string slot = TenShougiBoard::GetBoardSlot(x, to_y); // ���̃X���b�g�ɋ�͂���H
							if ( slot != "*" ) { // �X���b�g�̊ԂɓG��
								return false;
							}
						}
					} else if ( from_x < to_x ) {
						// ���̋�̈ʒu���P�ォ��A�ړ���̋��艺�̂Ƃ���܂ŁA�P�������
						for ( int x = from_x + 1; x < to_x; x++ ) {
							std::string slot = TenShougiBoard::GetBoardSlot(x, to_y); // ���̃X���b�g�ɋ�͂���H
							if ( slot != "*" ) { // �X���b�g�̊ԂɓG��
								return false;
							}
						}
					}

					return true;
				}
			}

			else if ( koma == "+RY" ) {
				// �ړ��悩��ړ�������������Βl��x�n�Ay�n�Ƃ�1�ȉ��A���Ɠ����͈͂�true
				if ( abs(to_x - from_x) <= 1 && abs(to_y - from_y) <= 1 ) {
					return true;
				}

				// ���������A�������͏c�������B
				if ( (from_x == to_x) || (from_y == to_y) ) {

					if ( to_y < from_y ) {
						// ���̋�̈ʒu���P�ォ��A�ړ���̋��艺�̂Ƃ���܂ŁA�P�������
						for ( int y = from_y - 1; y > to_y; y-- ) {
							std::string slot = TenShougiBoard::GetBoardSlot(to_x, y); // ���̃X���b�g�ɋ�͂���H
							if ( slot != "*" ) { // �X���b�g�̊ԂɓG��
								return false;
							}
						}
					} else if ( from_y < to_y ) {
						// ���̋�̈ʒu���P�ォ��A�ړ���̋��艺�̂Ƃ���܂ŁA�P�������
						for ( int y = from_y + 1; y < to_y; y++ ) {
							std::string slot = TenShougiBoard::GetBoardSlot(to_x, y); // ���̃X���b�g�ɋ�͂���H
							if ( slot != "*" ) { // �X���b�g�̊ԂɓG��
								return false;
							}
						}
					} else if ( to_x < from_x ) {
						// ���̋�̈ʒu���P�ォ��A�ړ���̋��艺�̂Ƃ���܂ŁA�P�������
						for ( int x = from_x - 1; x > to_x; x-- ) {
							std::string slot = TenShougiBoard::GetBoardSlot(x, to_y); // ���̃X���b�g�ɋ�͂���H
							if ( slot != "*" ) { // �X���b�g�̊ԂɓG��
								return false;
							}
						}
					} else if ( from_x < to_x ) {
						// ���̋�̈ʒu���P�ォ��A�ړ���̋��艺�̂Ƃ���܂ŁA�P�������
						for ( int x = from_x + 1; x < to_x; x++ ) {
							std::string slot = TenShougiBoard::GetBoardSlot(x, to_y); // ���̃X���b�g�ɋ�͂���H
							if ( slot != "*" ) { // �X���b�g�̊ԂɓG��
								return false;
							}
						}
					}

					return true;
				}
			}
			else if ( koma == "+UM" ) {
				// �ړ��悩��ړ�������������Βl��x�n�Ay�n�Ƃ�1�ȉ��A
				if ( abs(to_x - from_x) <= 1 && abs(to_y - from_y) <= 1 ) {
					return true;
				}


				// �΂߂��Ƃn�j
				if ( abs(from_x - to_x) == abs(from_y - to_y) ) {

					// �������
					if ( to_x < from_x && to_y < from_y ) {

						// ���̋�̈ʒu���P�ォ��A�ړ���̋��艺�̂Ƃ���܂ŁA�P�������
						for ( int x=from_x-1, y=from_y-1; (x > to_x) && (y > to_y); x--, y-- ) {
							std::string slot = TenShougiBoard::GetBoardSlot(x, y); // ���̃X���b�g�ɋ�͂���H
							if ( slot != "*" ) { // �X���b�g�̊ԂɓG��
								return false;
							}
						}
					}
					// �E�����
					else if ( from_x < to_x  && to_y < from_y ) {

						// ���̋�̈ʒu���P�ォ��A�ړ���̋��艺�̂Ƃ���܂ŁA�P�������
						for ( int x=from_x+1, y=from_y-1; (x < to_x) && (y > to_y); x++, y-- ) {
							std::string slot = TenShougiBoard::GetBoardSlot(x, y); // ���̃X���b�g�ɋ�͂���H
							if ( slot != "*" ) { // �X���b�g�̊ԂɓG��
								return false;
							}
						}
					}
					// �E������
					if ( from_x < to_x && from_y < to_y ) {

						// ���̋�̈ʒu���P�ォ��A�ړ���̋��艺�̂Ƃ���܂ŁA�P�������
						for ( int x=from_x+1, y=from_y+1; (x < to_x) && (y < to_y); x++, y++ ) {
							std::string slot = TenShougiBoard::GetBoardSlot(x, y); // ���̃X���b�g�ɋ�͂���H
							if ( slot != "*" ) { // �X���b�g�̊ԂɓG��
								return false;
							}
						}
					}
					// ��������
					if ( to_x < from_x && from_y < to_y ) {

						// ���̋�̈ʒu���P�ォ��A�ړ���̋��艺�̂Ƃ���܂ŁA�P�������
						for ( int x=from_x-1, y=from_y+1; (x > to_x) && (y < to_y); x--, y++ ) {
							std::string slot = TenShougiBoard::GetBoardSlot(x, y); // ���̃X���b�g�ɋ�͂���H
							if ( slot != "*" ) { // �X���b�g�̊ԂɓG��
								return false;
							}
						}
					}
					return true;
				}

			}
			else if ( koma == "+OU" ) {
				// �ړ��悩��ړ�������������Βl��x�n�Ay�n�Ƃ�1�ȉ��A
				if ( abs(to_x - from_x) <= 1 && abs(to_y - from_y) <= 1 ) {
					return true;
				}
			}

			return false;
		}

		// ���MouseDown�C�x���g�n���h��
		// �}�E�X�̃{�^���������ꂽ�Ƃ�
		void pbKoma_MouseDown(Object^ sender, MouseEventArgs^ e) {

			// ���N���b�N�ł͂Ȃ��B�_��
			if ((e->Button & ::MouseButtons::Left) != ::MouseButtons::Left) {
				// �N���A�s��
				InitBCSAstring();
				return;
			}

			// ���Ԃł͂Ȃ��B�_��
			if ( TenShougiBoard::GetCurTurnState() != TenShougiBoard::CurTurnB ) {
				// �N���A�s��
				InitBCSAstring();
				return;
			}

			// PictureBox��Tag�ɂǂ̋L��Matrix(O76�Ƃ��̈ʒu)�������Ă���̂��𒲍��B
			PictureBox^ pb = (PictureBox^)sender;
			String^ tag = (String^)pb->Tag;

			// �Տ�̋�̌����N���b�N����
			if (tag[0] == 'O') {

				// xy�^�̒l�����o��
				int x = System::Convert::ToInt32(tag[1]);
				int y = System::Convert::ToInt32(tag[2]);
				x = x - '0';
				y = y - '0';

				// �Y���̏ꏊ�ɂ��������̋�c
				std::string koma = TenShougiBoard::GetBoardSlot(x, y);

				// �����̂��̂ł���΁A�����瓮�����\��̋�
				if (koma[0] == '+') {
					InitBCSAstring();

					// xy���W���Ճ}�g���b�N�X��
					std::string from = TenShougiBoard::GetBoardMatrix(x, y);

					// �ړ����E�ړ����CSA���߂Ƃ��č\�z�J�n
					BCSAstringFromTo = gcnew String(from.c_str());

					// ���ۑ����Ă���
					BCSAstringKoma = gcnew String(koma.c_str());

					// �I���������̂̔w�i��ύX�B
					PaintSelectKomaBack( x, y, 'O');


				// �����ȊO�̂��̂ł���΁A�ړ��悾�B
				} else if (koma[0] == '-' || koma[0] == '*' ) {

					// �ړ����̕����񂪋���ۂȂ�΁A�ړ������w�肵�Ă��Ȃ��B�������Ȃ��B
					if ( BCSAstringFromTo->Length < 2 ) {
						return;
					}


					// �Տォ�瓮�������ۂɂ́A���邩����Ȃ����̔��f���o����B
					if ( BCSAstringFromTo != "00" ) { // �ړ��������ȊO�ŁAxy���W�n�ŁAy <= 2���ړ���̎�

						// �Ȃ�̏����Ƃ́A�u�ړ����v��3�i�ڂ���A���A�������́A�u�ړ���v��3�i�ڂ���B

						// �ړ����̍��W�����߂�
						// �ړ����̕������ .net�^��C++�^��
						std::string bcsastringfromto = DotNetStringToMultiByte(BCSAstringFromTo);
						// �ړ����̔z��
						int from_ix = TenShougiBoard::GetStringIndexToArrayIndex(bcsastringfromto);

						// �ړ����̒����index��xy��index�^�ɒ���
						int from_y = from_ix / 9;
						int from_x = from_ix % 9;

						// ���̏ꏊ�ɂ͋�͈ړ����邱�Ƃ��o���Ȃ�
						if ( ! IsKomaCannotMoveThere( from_x, from_y, x, y, BCSAstringKoma ) ) {
							// �������瑦���ɏ������B
							InitBCSAstring();

							// �w�i�����Z�b�g�B
							PaintSelectKomaBack( x, y, 'X');

							return;
						}

						// �ړ��悪�A3�i�ڂ���A�������́A�ړ�����3�i�ڂ���
						if ( y <= 2 || from_y <= 2 ) {

							// �����.net�^�C�v��C++�^�C�v��
							std::string bcsastringkoma = DotNetStringToMultiByte(BCSAstringKoma);
							// ��̈ړ����̋�͂Ȃ���H
							std::string narikoma = TenShougiBoard::GetNormalKomaToNarikoma(bcsastringkoma);

							// �������A�������ł���΁c (����ς݂͊֌W�Ȃ�)
							if ( narikoma.size() > 1 ) { // ���̋�͐����B

								// ���肪���������󋵂��ǂ����B
								bool must_nari = false;

								// �������ԂȂ�A1�i�ڂȂ�K������Ȃ���΂Ȃ�Ȃ��B
								if ( y ==0 && (BCSAstringKoma == "+FU" || BCSAstringKoma == "+KY")  ) {
									must_nari = true;
								}
								// �j�n�Ȃ�A2�i�ڂ���Ȃ�K������Ȃ���΂Ȃ�Ȃ��B
								else if ( y<=1 && (BCSAstringKoma == "+KE")  ) {
									must_nari = true;
								}

								// ���肪�����ł���Ȃ�A���邾���B
								if ( must_nari ) {

									// ��𐬂��ւƍX�V����B
									BCSAstringKoma = gcnew String(narikoma.c_str());

								// ���邩����Ȃ����C�ӂ̏󋵂Ȃ�A�_�C�A���O�ŕ���
								} else {

									// ����_�C�A���O�\��
									TenShougiYesNoForm^ tsynf = gcnew TenShougiYesNoForm("����܂����H", "����", "���炸");
									// ShowDialog�\�����ɁA�e�t�H�[���̂܂񒆂ɕ\������B
									tsynf->StartPosition = FormStartPosition::CenterParent;
									// �x�d�r�Ȃ�  (�e�t�H�[�����w�肵��)
									if (tsynf->ShowDialog(form) == ::DialogResult::Yes) {
										// ��𐬂��ւƍX�V����B
										BCSAstringKoma = gcnew String(narikoma.c_str());
									}
								}
							}
						}
					}

					// xy���W���Ճ}�g���b�N�X��
					std::string to = TenShougiBoard::GetBoardMatrix(x, y);

					// �ړ����E�ړ����CSA���߂Ƃ��č\�z����
					BCSAstringFromTo += gcnew String(to.c_str());

					// �w�i��I����̏��ɁB
					// PaintSelectKomaBack( x, y, 'O');

					String^ command = BCSAstringFromTo+BCSAstringKoma->Substring(1,2);
					// Bonanza��Send����B
					GetAndSendInputThread(command);

					// �������瑦���ɏ������B
					InitBCSAstring();

				}
			}
			// ���̋�悩��w���Ă���
			else if (tag[0] == 'B') {

				// ���̓X�^�[�g�n�_�ɂ����Ȃ�Ȃ��̂ŕK���������B
				InitBCSAstring();

				// PictureBox�Ɋ܂܂��^�O����i�^�̒l�����o��
				int i = System::Convert::ToInt32(tag[1]);
				i = i - '0';

				// �Y���̔ԍ��ɓ����Ă�����B
				std::string komamei = strBKomaNameOnKomaDai[i];

				// ���̑Ώۋ��̌��𓾂�
				int htKomaCount = TenShougiBoard::GetBStandSlot( komamei );
				// 0�Ȃ牽�����Ȃ�
				if (htKomaCount==0) {
					return;
				}

				// �ړ����E�ړ����CSA���߂Ƃ��č\�z�J�n
				BCSAstringFromTo = gcnew String("00");

				// ���ۑ����Ă���
				BCSAstringKoma = gcnew String(komamei.c_str());

				// �w�i��I����̏��ɁB
				PaintSelectKomaBack( i, 0, 'B');
			}

			// ���̋�悩��w���Ă���
			else if (tag[0] == 'W') {

				// �N���A�s��
				InitBCSAstring();

				// �N���A�s��
				return;
			}

		}


	private:
		// xy (��������i)���W�ƁAw�́A'O', 'B', 'W'�̎��
		void PaintSelectKomaBack( int from_x, int from_y, char O_B_W ) {
			// �Տ�̋���h�蒼��
			for(int y=0; y<9;  y++) {
				for (int x=0; x<9; x++) {

					PictureBox^ bp = (PictureBox^)mtrxShogiBoard[y, x];
					bp->BackgroundImage = (Bitmap^)tski->htKomaBitmap["*"]; // �w�i�͓���

					/*

					// �ȉ��A��̈ړ��͈͂ɐF��t���鏈��!!
					// ���̃X���b�g�̋�𓾂�B
					std::string koma = TenShougiBoard::GetBoardSlot( from_x, from_y );
					String^ okoma = gcnew String(koma.c_str());
					if ( IsKomaCannotMoveThere( from_x, from_y, x, y, okoma ) ) {
						bp->BackgroundImage = tski->bmKomaSelectedBackImg; // �I�����p�̔w�i
					}
					*/
				}
			}

			// ���̋��
			for (int i=0; i<arrayBShogiDai->Length; i++) {
				PictureBox^ bp = (PictureBox^)arrayBShogiDai[i];
				bp->BackgroundImage = (Bitmap^)tski->htKomaBitmap["*"]; // �w�i�͓���
			}

			for (int i=0; i<arrayWShogiDai->Length; i++) {
				PictureBox^ bp = (PictureBox^)arrayWShogiDai[i];
				bp->BackgroundImage = (Bitmap^)tski->htKomaBitmap["*"]; // �w�i�͓���
			}

			// �Տ�̋����
			if ( O_B_W == 'O' ) {
				PictureBox^ bp = (PictureBox^)mtrxShogiBoard[from_y, from_x];
				bp->BackgroundImage = tski->bmKomaSelectedBackImg; // �I�����p�̔w�i
			}

			// �Տ�̋����
			if ( O_B_W == 'B' ) {
				PictureBox^ bp = (PictureBox^)arrayBShogiDai[from_x];
				bp->BackgroundImage = tski->bmKomaSelectedBackImg; // �I�����p�̔w�i
			}

			// �Տ�̋����
			if ( O_B_W == 'W' ) {
				PictureBox^ bp = (PictureBox^)arrayWShogiDai[from_x];
				bp->BackgroundImage = tski->bmKomaSelectedBackImg; // �I�����p�̔w�i
			}
		}
	};
}
