#pragma once


#include "TenShougiMod.h"
#include "TenShougiBoard.h"
#include "TenShougiKoma.h"
#include "TenShougiSound.h"
#include "TenShougiBoardPBMap.h"
#include "Kifu.h"


// ���O��ԁB�ʏ�.NET�p�R���|�[�l���g�ł́ADll�Ɠ������O�̋�Ԃɓ���Ă����ƁA�Q�Ƃ���ۂɓs�����ǂ��B
namespace TenShougiMod {

	// �V�����p�̃t�H�[��
	ref class TenShougiForm : public Form {
	public:
		// �O���[�o���ϐ��p�B�������g���O���[�o���Ŏ���
		static TenShougiForm^ tsgForm;

	private:
		PictureBox^ pbBtnClose; // ����{�^��
		Bitmap^ btnClose;
		Bitmap^ btnCloseMouseDown;

		TenShougiBoadPBMap^ tsgBPBM; // �Տ�ɂ����B
		TenShougiSound^ snd;

		TextBox^ tbInputCmd;	// �v���C���[�̎����͂���Ƃ���
		ComboBox^ cbbxKifuList; // �����p�̃��X�g

		Button^ btnSaveKifu;	// �Z�[�u
		Button^ btnLoadKifu;	// �Z�[�u

		Label^ lblFormTitle;   // �^�C�g�����x���B�E�B���h�E�̃h���b�O�͂������S���h���b�O�Ή�

		Timer^ timer; // �^�C�}�[
		Timer^ tmOpacity; // Opacity �����p�̃^�C�}�[

	public:
		// �������ɂ���BPictureBox���|���|���o�Ă���̂̑΍�
		void SetOpacityTimer() {
			tmOpacity = gcnew Timer();
			tmOpacity->Interval = 100;

			// 0.1�b��ɔ������ɂ���̂�߂�
			tmOpacity->Tick += gcnew EventHandler(this, &TenShougiForm::StopOpacityTimer );

			tmOpacity->Start();
		}


		// �������J�n�B
		void StartOpacityTimer() {
			this->Opacity = 0.01;
		}

		// ��������߂�B
		void StopOpacityTimer(Object^ sendern, EventArgs^ e) {
			// �\������Ă���Ȃ�΁c
			if ( SharedMemory::iTenShougiFormShown ) {
				this->Opacity = 1.0;
			}
		}

		TenShougiForm() {
			this->SuspendLayout();

			// �t�H�[�����g�̃v���p�e�B�̐ݒ�
			SetFormProperty();

			// �t�H�[����GUI�R���|�[�l���g��z�u
			AddGUIControls();

			this->ResumeLayout(true);

			SharedMemory::iTenShougiFormShown = 1;

			// �^�C�}�[�̍쐬
			MakeTimer();

			// �������^�C�}�[�̍쐬
			SetOpacityTimer();
		}

	public:
		// �^�C�g���o�[�̐ݒ�B�Ǝ��J�X�^���Ȃ̂ŕK�v
		void SetTitle(String^ title) {
			lblFormTitle->Text = title;
		}

	private:
		// �ΐ핐���̃^�C�g��
		void SetVSBushouTitle() {

			// �V�ċL���V�����ւƕK�v�Ȓl���R�s�[����B
			CopyTenshoukiProcessToTenshougiProcess();
	
			// ���͏�񂪗L���ł������Ȃ�
			if ( strlen(SharedMemory::BPower.szDaniName) >0 && strlen(SharedMemory::WPower.szDaniName) >0 ) {
				int iBBushouID = SharedMemory::BPower.iBushouID;
				int iWBushouID = SharedMemory::WPower.iBushouID;
				SetTitle( gcnew String(nb6bushouname[iBBushouID].fastname) + gcnew String(SharedMemory::BPower.szDaniName)+
					      " �� " + 
						  gcnew String(nb6bushouname[iWBushouID].fastname) + gcnew String(SharedMemory::WPower.szDaniName) );

			// �����Ȃ炻�����Ȃ�����ȃ^�C�g��
			} else {
				SetTitle("�V����");
			}
		}

		// �t�H�[���̃v���p�e�B�ݒ�
		void SetFormProperty() {

			// �^�C�g���o�[���t�H�[���{�[�_�[�������B��q�Őݒ肷��w�i�摜���t�H�[���܂�܂Ɍ�����悤�ɂ���B
			// ���������������ƁA�^�C�g���o�[�����߂Ȃ��̂ŁA�E�B���h�E�𓮂������ׂ��Ȃ��Ȃ�B
			this->FormBorderStyle = ::FormBorderStyle::None;

			// ���̃v���W�F�N�g�̃A�Z���u���̃^�C�v���擾�B
			System::Reflection::Assembly^ prj_assebmly = GetType()->Assembly;
			System::Resources::ResourceManager^ r = gcnew System::Resources::ResourceManager(String::Format("{0}.TenShougiMod", prj_assebmly->GetName()->Name), prj_assebmly);


			//-------------------------------------�w�i�摜
			// ���\�[�X�t�@�C������ǂݎ��
			Bitmap^ rcimgback = (Bitmap^)(r->GetObject("TenShougiFormBackImg"));

			// �w�i�摜
			this->BackgroundImage = rcimgback;

			// �w�i�摜�̑傫�����t�H�[���̑傫��
			this->Width = rcimgback->Width;
			this->Height = rcimgback->Height;

			//-------------------------------------�w�i�摜
			// ���\�[�X�t�@�C������ǂݎ��
			System::Drawing::Icon^ iconform = (System::Drawing::Icon^)(r->GetObject("TenShougiFormIcon"));
			this->Icon = iconform;

			//-------------------------------------����{�^��
			// ���\�[�X�t�@�C������ǂݎ��
			btnClose     = (Bitmap^)(r->GetObject("TenShougiFormCloseButtonImg"));
			btnCloseMouseDown = (Bitmap^)(r->GetObject("TenShougiFormCloseButtonDownImg"));

			// ����{�^��
			pbBtnClose = gcnew PictureBox();
			pbBtnClose->Width = btnClose->Width;
			pbBtnClose->Height = btnClose->Height;
			pbBtnClose->Image = btnClose;

			// �t�H�[��������ۂ̃C�x���g�n���h��
			this->FormClosed += gcnew FormClosedEventHandler( this, &TenShougiForm::form_FormClosed);
		}

	public:
		// �t�H�[������鎞
		virtual void Show() {
			// �������R���g���[���̃^�C�}�[�쐬
			StartOpacityTimer();

			SharedMemory::iTenShougiFormShown = 1;


			// �ĕ\���Ȃǂ�����̂ŁA�ΐ핐�������X�V���Ă����B
			SetVSBushouTitle();
			Form::Show();
		}

		// �t�H�[�����B��
		virtual void Hide() {
			// ���L�������ɍ������ĂȂ���!! �`�B�BClose�����킯�ł͂Ȃ��B
			SharedMemory::iTenShougiFormShown = 0;
			Form::Hide();
		}

	private:
		// �t�H�[������鎞
		void form_FormClosed( Object^ sender, FormClosedEventArgs^ e) {
			SharedMemory::iTenShougiFormShown = 0;
		}

	private:
		// GUI�R���|�[�l���g�̔z�u
		void AddGUIControls() {
			//------------------------------------�^�C�g�����x��
			// �^�C�g�����x���̍쐬�B
			lblFormTitle = gcnew Label();
			// ��������S�͖ڂɂ������Ȓl����
			lblFormTitle->Top = 6;
			lblFormTitle->Left = 23;
			lblFormTitle->Width = 440;
			lblFormTitle->Height = 21;
			// �t�H���g�̐ݒ�
			System::Drawing::Font^ font = gcnew System::Drawing::Font(gcnew String(sgGetTenShougiFontName()), 12, FontStyle::Bold);
			lblFormTitle->Font = font;
			// �t�H���g�̐F
			lblFormTitle->ForeColor = ::Color::White;
			// �z�u��
			lblFormTitle->TextAlign = ContentAlignment::TopCenter;

			SetVSBushouTitle();

			lblFormTitle->BackColor = ::Color::Transparent;

			// �^�C�g�����Ȃ��Ȃ����̂ŁA(Button��PictureBox���Ȃ�)form���ڂ��h���b�O����Ɠ�������悤�ɂ���)
			lblFormTitle->MouseDown += gcnew MouseEventHandler(this, &TenShougiForm::form_MouseDown);
			lblFormTitle->MouseMove += gcnew MouseEventHandler(this, &TenShougiForm::form_MouseMove);
			// �t�H�[���ł����łɓ����悤�ɂ���B
			this->MouseDown += gcnew MouseEventHandler(this, &TenShougiForm::form_MouseDown);
			this->MouseMove += gcnew MouseEventHandler(this, &TenShougiForm::form_MouseMove);

			//------------------------------------���M�E���́E�������ʃR���{�{�b�N�X
			// ���ꂼ��I�u�W�F�N�g����
			tbInputCmd = gcnew TextBox();
			tbInputCmd->Top = 35;
			tbInputCmd->Left = 400;
			tbInputCmd->Width = 70;
			tbInputCmd->KeyDown += gcnew KeyEventHandler(this, &TenShougiForm::tbInputCmd_KeyDown); // �L�[���͎��̃C�x���g�n���h��

			cbbxKifuList = gcnew ComboBox();
			cbbxKifuList->Top = 320;
			cbbxKifuList->Left = 10;
			cbbxKifuList->Width = 76;
			cbbxKifuList->BackColor = ::Color::PaleGoldenrod;
			System::Drawing::Font^ cbfont = gcnew System::Drawing::Font(gcnew String(sgGetTenShougiFontName()), 9);
			cbbxKifuList->Font = cbfont;

			btnSaveKifu = gcnew Button();
			btnSaveKifu->Text = "�ۑ�";
			btnSaveKifu->Left = 10;
			btnSaveKifu->Top = 290;
			btnSaveKifu->Width = 76;
			btnSaveKifu->Click += gcnew EventHandler(this, &TenShougiForm::btnSaveKifu_Click);

			btnLoadKifu = gcnew Button();
			btnLoadKifu->Text = "�Ǎ�";
			btnLoadKifu->Left = 10;
			btnLoadKifu->Top = 290;
			btnLoadKifu->Width = 76;
			btnLoadKifu->Click += gcnew EventHandler(this, &TenShougiForm::btnLoadKifu_Click);

			pbBtnClose->Top = 5;  // �w�i�摜�Ƃ̈ʒu�֌W�ɂ��A���̏ꏊ�Œ�
			pbBtnClose->Left = 5; // �w�i�摜�Ƃ̈ʒu�֌W�ɂ��A���̏ꏊ�Œ�
			pbBtnClose->MouseMove += gcnew MouseEventHandler(this, &TenShougiForm::pbBtnClose_MouseMove);
			pbBtnClose->MouseDown += gcnew MouseEventHandler(this, &TenShougiForm::pbBtnClose_MouseDown);
			pbBtnClose->MouseUp += gcnew MouseEventHandler(this, &TenShougiForm::pbBtnClose_MouseUp);

			//------------------------------------�R���g���[���z�u
			// this->Controls->Add(tbInputCmd);
			this->Controls->Add(cbbxKifuList);
			this->Controls->Add(lblFormTitle);

			// ���Z�[�u�{�^������[�O��
			//	this->Controls->Add(btnSaveKifu);
			// this->Controls->Add(btnLoadKifu);

			this->Controls->Add(pbBtnClose); // ����{�^���̔z�u�B

			// ��B�̏�����
			tsgBPBM = gcnew TenShougiBoadPBMap(this);
			tsgBPBM->UpdatePositionByBoardSlot();

			// �
			snd = gcnew TenShougiSound();
		}

	private:
		// �^�C�}�[�̍쐬
		void MakeTimer() {

			timer = gcnew Timer();  // �^�C�}

			timer->Interval = 100;  // 0.1�b�Ԋu

			// �^�C�}�p�̃C�x���g�n���h����o�^
			timer->Tick += gcnew EventHandler(this, &TenShougiForm::UpdateCbbxKifuList);

			timer->Start();  // �^�C�} ON
		}


	private:
		// ����摜�{�^�����}�E�X��������
		void pbBtnClose_MouseDown(Object^ sender, MouseEventArgs^ e) {
			// ���{�^���Ȃ�
			if ( e->Button == ::MouseButtons::Left ) {
				// �N���[�Y�{�^���̃C���[�W�������ꂽ��Ԃɂ���B
				pbBtnClose->Image = btnCloseMouseDown;
			}
		}

		// ����摜�{�^�����}�E�X�����̂���߂���
		void pbBtnClose_MouseUp(Object^ sender, MouseEventArgs^ e) {
			pbBtnClose->Image = btnClose;

			// �}�E�X�𗣂������Ƀ}�E�X���{�^���̒��ɓ����Ă�����c
			if ( pbBtnClose->ClientRectangle.Contains(e->Location) ) {
				// ���{�^���Ȃ�
				if ( e->Button == ::MouseButtons::Left ) {
					// �������g(�t�H�[��)�����
					this->Hide();
				}
			}
		}

		// ����摜�{�^�����}�E�X�������Ă���
		void pbBtnClose_MouseMove(Object^ sender, MouseEventArgs^ e) {

			// �}�E�X���A����{�^��������O�ꂽ
			if ( ! pbBtnClose->ClientRectangle.Contains(e->Location) ) {
				// �摜�����ւƖ߂�
				pbBtnClose->Image = btnClose;
			}
		}

		// �R���{�{�b�N�X�̏펞�X�V�B(������)
		void UpdateCbbxKifuList(Object^ sender, EventArgs^ event) {

			// �R���{�{�b�N�X�̒����ƁA�����̒����������Ȃ牽�����Ȃ��B
			if (cbbxKifuList->Items->Count == bufAllKifu.size() ) {
				return;
			}

			// �R���{�{�b�N�X�ƋL���̒������Ⴄ�Ȃ�c�ȉ��̏���

			// �R���{�{�b�N�X���N���A���āc
			cbbxKifuList->Items->Clear();

			// �R���{�{�b�N�X�`���~
			cbbxKifuList->BeginUpdate();

			//--------------------------------------------- C R I T I C A L  S E C T I O N  B E G I N -----------------------------------------------------
			// �N���e�B�J���Z�N�V�����J�n�B���̃N���e�B�J���Z�N�V���������X���b�h�̓N���e�B�J���Z�N�V�������ɏ������ݏo���Ȃ�
			// �N���e�B�J���Z�N�V�����ɓ���AbufBonanzaKifu �̑�������b�N
			EnterCriticalSection(&cs);

			// TenShougiModKifu.dll������Ȃ��
			if ( true || System::IO::File::Exists("TenShougiModKKC.dll") ) {
				// KI2�^�C�v(���{��́��V�Z���Ƃ�)�̃^�C�v�ŁA�R���{�{�b�N�X�ɃR�s�[����BTenShougiModKifu.dll �K�{�B
				CopyToKI2KifuToComboBox();

				// TenShougiModKifu.dll�������Ȃ��
			} else {
				// CSA�^�C�v(�p���++7776FU)�̃^�C�v�ŁA�R���{�{�b�N�X�ɃR�s�[����B���ɂ�����Ȃ��B
				CopyToCSAKifuToComboBox();
			}

			// �N���e�B�J���Z�N�V��������o��B
			LeaveCriticalSection(&cs);
			//--------------------------------------------- C R I T I C A L  S E C T I O N  E N D -----------------------------------------------------

			// �R���{�{�b�N�X�`��ĊJ
			cbbxKifuList->EndUpdate();
		}

	private:

		// CSA�^�C�v(�p���++7776FU)�̃^�C�v�ŁA�R���{�{�b�N�X�ɃR�s�[����B���ɂ�����Ȃ��B
		void CopyToCSAKifuToComboBox() {
			// �L�����ACompoBox�ւƑS���R�s�[
			for each(string sjiskifu in bufAllKifu) {
				String^ objkifu = gcnew String(sjiskifu.c_str());
				cbbxKifuList->Items->Add(objkifu);
			}
		}

		// KI2�^�C�v(���{��́��V�Z���Ƃ�)�̃^�C�v�ŁA�R���{�{�b�N�X�ɃR�s�[����BTenShougiModKifu.dll �K�{�B
		void CopyToKI2KifuToComboBox() {

			// ----------------------------TenShougiModKifu���g���āACSA�^�̊���(+7766FU)��KI2�^�̊���(���V�Z��)�Ȃǂɒ����B
			// KI2�`���̈�ԏ��߂́A��肪�u+�v�Ƃ����L���ł��邱�Ƃ������B
			std::string allConcatStr = "+\n";
			// �L�����A���s�ŘA��
			for each(string sjiskifu in bufAllKifu) {
				allConcatStr += sjiskifu + "\n";
			}
			// �����ϊ��̂��߂̌^
			KKC::kifu_t obj_kifu;
			// KI2�^
			String^ message = gcnew String("");
			// CSA��Kifu�ւ̕ϊ�
			KKC::Kifu::CSA2Kifu( gcnew String(allConcatStr.c_str()), obj_kifu, message);
			// Kifu��KI2�ւ̕ϊ�
			String^ strDotNetKi2 = KKC::Kifu::Kifu2KI2( obj_kifu );
			// C#�^�����񁨃}���`�o�C�g�ւ̕ϊ�
			std::string strMBKi2 = DotNetStringToMultiByte(strDotNetKi2);
			// �S�����P�̕�����ɂȂ��Ă���̂ŁA'\n'�ŕ����B�s���ƂƂ���B
			vector<string> vStrMBKi2 = string_split( strMBKi2, '\n' ); // ���s�ŕ���
			// 
			// ���s���Ƃɕ������ꂽ�̂��Ȃ߂Ă����āc
			for each (string ki2line in vStrMBKi2) {
				// KI2�^�C�v�̊������A�����ꂽ�s��
				if ( ki2line.find("��") != string::npos ) {
					// ���V�Z�� ���R�l�� ���U�Z�� ���S��� ���Q�Z�� ���T��� ���Q�ܕ� ���R�O�p �݂����ɂȂ�̂ŁA���p�X�y�[�X�ŕ�������
					vector<std::string> vKi2Line = string_split(ki2line, ' '); // �X�y�[�X�ŋ�؂��ĕ�������B
					for each ( std::string c_te in vKi2Line ) {
						String^ o_te = gcnew String(c_te.c_str()); // C++�}���`�o�C�g��C++/CLI.NET�^�ւ�
						cbbxKifuList->Items->Add(o_te); // ���������Ō�̗v�f���R���{�{�b�N�X�ɒǉ�����B
					}
				}
			}

		}

	public:
		//---------------------------------------------�ՍX�V
		void UpdatePositionByBoardSlot() {
			tsgBPBM->UpdatePositionByBoardSlot();
		}

		//---------------------------------------------�
		void PlayKomaSound(std::string CSAstring) {
			if (CSAstring == "DummySound" ) {
				snd->PlayDummySound();
			} else {
				snd->PlayKomaSound(CSAstring);
			}
		}


	private:

		//�����ꂽ�L�[���G���^�[�L�[���ǂ����̏�������
		void tbInputCmd_KeyDown(Object^ sender, KeyEventArgs^ e) {

			// �C�x���g�������e�L�X�g�{�b�N�X�Łc
			if ((TextBox^)sender == tbInputCmd) {
				// �G���^�[�L�[���������Ȃ�΁c
				if (e->KeyCode == System::Windows::Forms::Keys::Enter) {
					if ( tbInputCmd->Text == "renew game" ) {
						ReNewTenShougiGame();
					} else {
						SendMessageToBonanza();
					}
				}
			}
		}

		// �{�i���U�ɁA�e�L�X�g�{�b�N�X�ɓ��ꂽ���e�𑗐M����B
		void SendMessageToBonanza() {

			// ���ԂłȂ���Ή������Ȃ��B
			if ( TenShougiBoard::GetCurTurnState() != TenShougiBoard::CurTurnB) {
				return;
			}

			// ����ۂ��łȂ��Ȃ��
			if ( tbInputCmd->Text != "" ) {

				// ���M���āA���M�ł��������ʂ����炤�B
				int issuccess = GetAndSendInputThread(tbInputCmd->Text);

				// ���������Ȃ�
				if (issuccess) {
					// ���g�������
					tbInputCmd->Text = "";
				}
			}
		}

		// �ۑ��{�^������������
		void btnSaveKifu_Click(Object^ sender, EventArgs^ e) {
			// �ۑ���ǂݏo���̃}�j���A������͐��Ԍ���
			if ( TenShougiBoard::GetCurTurnState() == TenShougiBoard::CurTurnB ) {
				SaveCSAKifu();
			}
		}

		// �Ǎ��{�^������������
		void btnLoadKifu_Click(Object^ sender, EventArgs^ e) {
			// �ۑ���ǂݏo���̃}�j���A������͐��Ԍ���
			if ( TenShougiBoard::GetCurTurnState() == TenShougiBoard::CurTurnB ) {
				LoadCSAKifu();
			}
		}

	private:
		//http://dobon.net/vb/dotnet/form/moveform.html
		//�}�E�X�̃N���b�N�ʒu���L��
		Point^ ptMemoryMouse;

	private:
		// ���̃t�H�[����MouseDown�C�x���g�n���h��
		// �}�E�X�̃{�^���������ꂽ�Ƃ�
		void form_MouseDown(Object^ sender, MouseEventArgs^ e) {
			try {
				if ((e->Button & ::MouseButtons::Left) == ::MouseButtons::Left) {
					//�ʒu���L������
					ptMemoryMouse = gcnew Point(e->X, e->Y);
				}
			} catch( NullReferenceException^ /*err*/) {
			}
		}

		//���̃t�H�[����MouseMove�C�x���g�n���h��
		//�}�E�X���������Ƃ�
		void form_MouseMove(Object^ sender, MouseEventArgs^ e) {

			/*
			// ���{�^�������Ăē����Ă���
			if ((e->Button & ::MouseButtons::Left) == ::MouseButtons::Left) {

			// �g�p�̃E�B���h�E�̍쐬
			TenShougiWakuForm^ waku = gcnew TenShougiWakuForm(this, "TenShougiWakuForm");
			// ���[�_���E�B���h�E���o�鎞�ɁA�����Ɠ����Z���^�[
			waku->StartPosition = ::FormStartPosition::CenterParent;
			// �\������B
			waku->ShowDialog();
			// �V��������������A�N�e�B�u�ɂ���B
			waku->Activate();
			}
			*/

			try {
				// ���W�𑫂�����œ�����
				if ((e->Button & ::MouseButtons::Left) == ::MouseButtons::Left) {
					this->Left += e->X - ptMemoryMouse->X;
					this->Top += e->Y - ptMemoryMouse->Y;
				}
			} catch( NullReferenceException^ /*err*/) {
			}
		}
	};
}
