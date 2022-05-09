#pragma once

#include "TenShougiMod.h"

// ���O��ԁB�ʏ�.NET�p�R���|�[�l���g�ł́ADll�Ɠ������O�̋�Ԃɓ���Ă����ƁA�Q�Ƃ���ۂɓs�����ǂ��B
namespace TenShougiMod {

	// �V�����p�̃t�H�[��
	ref class TenShougiWakuForm : public Form {

		static const int WAKU_MARGIN = 30;

	private:
		Form^ formParent;

		Point^ ptMemoryMouse;

		Label^ lblWakuImage;

		String^ strBgImgName;

	public:
		TenShougiWakuForm(Form^ form, String^ strBgImgName) {

			this->SuspendLayout();

			this->formParent = form;
			this->strBgImgName = strBgImgName;

			this->Width = form->Width + WAKU_MARGIN*2;
			this->Height = form->Height + WAKU_MARGIN*2;

			// �^�C�g����{�[�_�[�g�͂Ȃ�
			this->FormBorderStyle = ::FormBorderStyle::None;

			// �w�i�𓧖��ɂ���B
			this->BackColor = ::Color::Red; // �w�i�ɐԂ�ݒ肵�āc
			this->TransparencyKey = ::Color::Red; // �Ԃ𓧖��ɂ���B

			// �}�E�X���グ�����͕��鎞
			this->MouseUp += gcnew MouseEventHandler(this, &TenShougiWakuForm::form_OnMouseUp);
			// �}�E�X�𓮂����Ă��鎞�́A�t�H�[���I�[�v���̎��Ƃ̃Y���̕������W�ɔ��f������B
			this->MouseMove += gcnew MouseEventHandler(this, &TenShougiWakuForm::form_OnMouseMove);
			// �t�H�[�����\�����ꂽ�^�C�~���O�ŁA�}�E�X�̍��W���o����
			this->Shown += gcnew EventHandler(this, &TenShougiWakuForm::form_OnShown);
			this->FormClosed += gcnew FormClosedEventHandler(this, &TenShougiWakuForm::form_FormClosed);

			// �w�i�̉摜�\��t��
			SetBackImage();

			// �������x
			this->Opacity = 0.05;

			this->ResumeLayout(true);

		}

	private:
		void SetBackImage() {
			// ���̃v���W�F�N�g�̃A�Z���u���̃^�C�v���擾�B
			System::Reflection::Assembly^ prj_assebmly = GetType()->Assembly;
			System::Resources::ResourceManager^ r = gcnew System::Resources::ResourceManager(String::Format("{0}.TenShougiMod", prj_assebmly->GetName()->Name), prj_assebmly);

			//-------------------------------------�w�i�摜
			// ���\�[�X�t�@�C������ǂݎ��
			Bitmap^ rcimgback = (Bitmap^)(r->GetObject(this->strBgImgName));

			// ���x���쐬�B�s�N�`���[�{�b�N�X�ւ��B
			lblWakuImage = gcnew Label();
			lblWakuImage->BackgroundImage = rcimgback;
			// �t�H�[���̓���(WAKU_MARGIN����������)�ɔz�u
			lblWakuImage->Left = WAKU_MARGIN;
			lblWakuImage->Top = WAKU_MARGIN;
			// �w�i�̑傫��
			lblWakuImage->Width = rcimgback->Width; 
			lblWakuImage->Height = rcimgback->Height;
			lblWakuImage->BackColor = Color::Transparent;

			// �R���g���[���Ƃ��Ă͔������Ȃ��B�S�ăt�H�[���ւƗ����B
			lblWakuImage->Enabled = false;
			

			this->Controls->Add(lblWakuImage);
		}

	private:
		// �t�H�[�����\�����ꂽ��
		void form_OnShown(Object^ sender, EventArgs^ e) {
			// �}�E�X�̃O���[�o�����W��ۑ��B�t�H�[����̍��W�ɒ���
			// ptMemoryMouse = this->PointToClient(Windows::Forms::Cursor::Position);
			ptMemoryMouse = Windows::Forms::Cursor::Position;
		}

		// ���̘g�t�H�[���������鎞�ɁA
		void form_FormClosed(Object^ sender, FormClosedEventArgs^ e) {
			// ���̃t�H�[���̈ʒu�܂ŁA�e�t�H�[���������Ă���B
			Point^ pos = gcnew Point(this->Location.X + lblWakuImage->Location.X, this->Location.Y + lblWakuImage->Location.Y);
			formParent->Left = pos->X;
			formParent->Top = pos->Y;
		}

		// �}�E�X���グ�����ɁA�������g�����
		void form_OnMouseUp(Object^ sender, MouseEventArgs^ e) {
			this->Close();
		}

		// �}�E�X�𓮂����Ă��鎞�́A�ۑ����Ă��������̃}�E�X���W�Ƃ̃Y�����A�e�t�H�[���̈ʒu�ɑ�������
		void form_OnMouseMove(Object^ sender, MouseEventArgs^ e) {

			// ���N���b�N���������Ă���
			/*
			if ((e->Button & ::MouseButtons::Left) == ::MouseButtons::Left) {
				// �t�H�[����̃}�E�X�̈ʒu-�o���Ă������}�E�X�̃|�W�V����
				this->Left += e->X - ptMemoryMouse->X;
				this->Top += e->Y - ptMemoryMouse->Y;
			}
			*/

			// ���N���b�N���������Ă���
			if ((e->Button & ::MouseButtons::Left) == ::MouseButtons::Left) {

				// �������x
				this->Opacity = 0.9;

				//                           �� �ŏ��̈ʒu���瓮���������̃x�N�g�����W
				// �e�̈ʒu + (���̃J�[�\���ʒu-�ۑ����Ă������J�[�\���ʒu)
				this->Left = formParent->Left + (Windows::Forms::Cursor::Position.X - ptMemoryMouse->X) - WAKU_MARGIN;
				this->Top = formParent->Top + (Windows::Forms::Cursor::Position.Y - ptMemoryMouse->Y) - WAKU_MARGIN;

				// �g�摜�̈ʒu���W�̔{���ɂ܂�߂邱�ƂŁA�K�^�K�^�Ɠ����������o��
				int x = this->Left + WAKU_MARGIN;
				int y = this->Top + WAKU_MARGIN;
				x = int(x/8)*8 + 8;
				y = int(y/8)*8 + 8;
				x = x - this->Left;
				y = y - this->Top;
				lblWakuImage->Left = x;
				lblWakuImage->Top = y;			}
		}
	};
}