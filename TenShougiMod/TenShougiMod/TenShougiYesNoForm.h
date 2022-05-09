#pragma once


#include "TenShougiMod.h"
#include "TenShougiForm.h"
#include "TenShougiWakuForm.h"

// ���O��ԁB�ʏ�.NET�p�R���|�[�l���g�ł́ADll�Ɠ������O�̋�Ԃɓ���Ă����ƁA�Q�Ƃ���ۂɓs�����ǂ��B
namespace TenShougiMod {

	// �V�����p�̃t�H�[��
	ref class TenShougiYesNoForm : public Form {

		Bitmap^ bmButton;
		Bitmap^ bmButtonDown;

		Panel^ pnlButtonYes;
		Panel^ pnlButtonNo;

		Label^ lblYes;
		Label^ lblNo;

		Label^ lblMainMessage;

		String^ strMainMessage;
		String^ strYes;
		String^ strNo;

	public:

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
			Bitmap^ rcimgback = (Bitmap^)(r->GetObject("ModalFormBackImg"));

			// �w�i�摜
			this->BackgroundImage = rcimgback;

			// �w�i�摜�̑傫�����t�H�[���̑傫��
			this->Width = rcimgback->Width;
			this->Height = rcimgback->Height;

			//-------------------------------------�{�^���̃��\�[�X
			// ���\�[�X�t�@�C������ǂݎ��
			bmButton      = (Bitmap^)(r->GetObject("ModalFormButton"));
			bmButtonDown = (Bitmap^)(r->GetObject("ModalFormButtonMouseDown"));

			// �t�H�[���ł����łɓ����悤�ɂ���B
			this->MouseDown += gcnew MouseEventHandler(this, &TenShougiYesNoForm::form_MouseDown);
			this->MouseMove += gcnew MouseEventHandler(this, &TenShougiYesNoForm::form_MouseMove);
		}

		// GUI�R���|�[�l���g�̔z�u
		void AddGUIControls() {
			// �t�H���g�̐ݒ�
			System::Drawing::Font^ font = gcnew System::Drawing::Font(gcnew String(sgGetTenShougiFontName()), 12, FontStyle::Bold);

			lblMainMessage = gcnew Label();
			lblMainMessage->Text = strMainMessage; // �_�C�A���O�̐^�񒆂̕���
			lblMainMessage->Left = 0;
			lblMainMessage->Top = 28 + 8; // 1�s�Ȃ̂�16�̔�����8��������
			lblMainMessage->Width = this->Width - lblMainMessage->Left;
			lblMainMessage->Height = 32;
			lblMainMessage->TextAlign = ::ContentAlignment::TopCenter;
			lblMainMessage->Font = font;
			lblMainMessage->ForeColor = ::Color::Black;
			lblMainMessage->BackColor = ::Color::Transparent;

			// �{�^���Q�̏ꍇ
			bool is_bothbutton = ( strNo != "" );

			// YES�{�^��
			pnlButtonYes = gcnew Panel();
			pnlButtonYes->Width = bmButton->Width;
			pnlButtonYes->Height = bmButton->Height;
			pnlButtonYes->Left = 8;
			pnlButtonYes->Top = 70;
			pnlButtonYes->BackgroundImage = bmButton;

			// �{�^����2�̏ꍇ�����c
			if ( is_bothbutton ) {
				// NO�{�^��
				pnlButtonNo = gcnew Panel();
				pnlButtonNo->Width = bmButton->Width;
				pnlButtonNo->Height = bmButton->Height;
				pnlButtonNo->Left = 88;
				pnlButtonNo->Top = 70;
				pnlButtonNo->BackgroundImage = bmButton;

			// �{�^������̎��� Yes�{�^���̈ʒu�𒆉���
			} else {
			pnlButtonYes->Left = 48;
			}

			// YES�{�^��
			lblYes = gcnew Label();
			lblYes->Parent = pnlButtonYes;
			lblYes->Width = bmButton->Width;
			lblYes->Height = bmButton->Height;
			lblYes->Left = 0;
			lblYes->Top = 1;
			lblYes->Font = font;
			lblYes->TextAlign = ContentAlignment::MiddleCenter;
			lblYes->Text = strYes; // Yes���̕�����
			lblYes->ForeColor = ::Color::Black;
			lblYes->BackColor = ::Color::Transparent;
			ptLblYes = gcnew Point(lblYes->Left, lblYes->Top); // �I���W�i�����W���T���Ă���
			lblYes->MouseMove += gcnew MouseEventHandler(this, &TenShougiYesNoForm::lblYes_MouseMove);
			lblYes->MouseDown += gcnew MouseEventHandler(this, &TenShougiYesNoForm::lblYes_MouseDown);
			lblYes->MouseUp += gcnew MouseEventHandler(this, &TenShougiYesNoForm::lblYes_MouseUp);

			// �{�^����2�̏ꍇ�����c
			if ( is_bothbutton ) {
				// NO�{�^��
				lblNo = gcnew Label();
				lblNo->Parent = pnlButtonNo;
				lblNo->Width = bmButton->Width;
				lblNo->Height = bmButton->Height;
				lblNo->Left = 0;
				lblNo->Top = 1;
				lblNo->Font = font;
				lblNo->TextAlign = ContentAlignment::MiddleCenter;
				lblNo->Text =  strNo; // No���̕�����
				lblNo->ForeColor = ::Color::Black;
				lblNo->BackColor = ::Color::Transparent;
				ptLblNo = gcnew Point(lblNo->Left, lblNo->Top); // �I���W�i�����W���T���Ă���
				lblNo->MouseMove += gcnew MouseEventHandler(this, &TenShougiYesNoForm::lblNo_MouseMove);
				lblNo->MouseDown += gcnew MouseEventHandler(this, &TenShougiYesNoForm::lblNo_MouseDown);
				lblNo->MouseUp += gcnew MouseEventHandler(this, &TenShougiYesNoForm::lblNo_MouseUp);
			}

			this->Controls->Add(lblMainMessage);
			this->Controls->Add(pnlButtonYes);
			if ( is_bothbutton ) {
				this->Controls->Add(pnlButtonNo);
			}

		}

		TenShougiYesNoForm(String^ msg, String^ yes, String^ no) { 

			strMainMessage = gcnew String(msg);
			strYes = gcnew String(yes);
			strNo  = gcnew String(no);

			this->SuspendLayout();

			SetFormProperty();

			// GUI�R���|�[�l���g�̔z�u
			AddGUIControls();

			this->ResumeLayout(false);
		}


	private:
		Point^ ptLblYes;

		// ����̃{�^�����}�E�X��������
		void lblYes_MouseDown(Object^ sender, MouseEventArgs^ e) {
			// ���{�^���Ȃ�
			if ( e->Button == ::MouseButtons::Left ) {
				// �C���[�W�������ꂽ��Ԃɂ���B
				pnlButtonYes->BackgroundImage = bmButtonDown;

				// ���x�����E����1�h�b�g������
				lblYes->Left = lblYes->Left + 1;
				lblYes->Top = lblYes->Top + 1;
			}
		}

		// ����{�^�����}�E�X�����̂���߂���
		void lblYes_MouseUp(Object^ sender, MouseEventArgs^ e) {
			pnlButtonYes->BackgroundImage = bmButton;

			// �ʒu�����ւƖ߂�
			lblYes->Left = ptLblYes->X;
			lblYes->Top = ptLblYes->Y;

			// �}�E�X�𗣂������Ƀ}�E�X���{�^���̒��ɓ����Ă�����c
			if ( lblYes->ClientRectangle.Contains(e->Location) ) {
				// ���{�^���Ȃ�
				if ( e->Button == ::MouseButtons::Left ) {
					// �������g(�t�H�[��)�����
					this->DialogResult = ::DialogResult::Yes; // ���炸������
				}
			}
		}

		// ����{�^���������Ă�}�E�X�������Ă���
		void lblYes_MouseMove(Object^ sender, MouseEventArgs^ e) {

			// �}�E�X���A����{�^��������O�ꂽ
			if ( ! lblYes->ClientRectangle.Contains(e->Location) ) {
				// �摜�����ւƖ߂�
				pnlButtonYes->BackgroundImage = bmButton;

				// �ʒu�����ւƖ߂�
				lblYes->Left = ptLblYes->X;
				lblYes->Top = ptLblYes->Y;

			}
		}


	private:
		Point^ ptLblNo;

		// ���炸�̃{�^�����}�E�X��������
		void lblNo_MouseDown(Object^ sender, MouseEventArgs^ e) {
			// ���{�^���Ȃ�
			if ( e->Button == ::MouseButtons::Left ) {
				// �C���[�W�������ꂽ��Ԃɂ���B
				pnlButtonNo->BackgroundImage = bmButtonDown;

				// ���x�����E����1�h�b�g������
				lblNo->Left = lblNo->Left + 1;
				lblNo->Top = lblNo->Top + 1;
			}
		}

		// ���炸�{�^�����}�E�X�����̂���߂���
		void lblNo_MouseUp(Object^ sender, MouseEventArgs^ e) {
			pnlButtonNo->BackgroundImage = bmButton;

			// �ʒu�����ւƖ߂�
			lblNo->Left = ptLblNo->X;
			lblNo->Top = ptLblNo->Y;

			// �}�E�X�𗣂������Ƀ}�E�X���{�^���̒��ɓ����Ă�����c
			if ( lblNo->ClientRectangle.Contains(e->Location) ) {
				// ���{�^���Ȃ�
				if ( e->Button == ::MouseButtons::Left ) {
					this->DialogResult = ::DialogResult::No; // ���炸������
				}
			}
		}

		// ���炸�{�^���������Ă�}�E�X�������Ă���
		void lblNo_MouseMove(Object^ sender, MouseEventArgs^ e) {

			// �}�E�X���A����{�^��������O�ꂽ
			if ( ! lblNo->ClientRectangle.Contains(e->Location) ) {
				// �摜�����ւƖ߂�
				pnlButtonNo->BackgroundImage = bmButton;

				// �ʒu�����ւƖ߂�
				lblNo->Left = ptLblNo->X;
				lblNo->Top = ptLblNo->Y;

			}
		}


		// ---------------------------------�}�E�X�ɍ��킹�ăt�H�[������
	private:
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
				TenShougiWakuForm^ waku = gcnew TenShougiWakuForm(this, "TenShougiWakuDialog");
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

