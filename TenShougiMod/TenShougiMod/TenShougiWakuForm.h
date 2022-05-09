#pragma once

#include "TenShougiMod.h"

// 名前空間。通常.NET用コンポーネントでは、Dllと同じ名前の空間に入れておくと、参照する際に都合が良い。
namespace TenShougiMod {

	// 天将棋用のフォーム
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

			// タイトルやボーダー枠はなし
			this->FormBorderStyle = ::FormBorderStyle::None;

			// 背景を透明にする。
			this->BackColor = ::Color::Red; // 背景に赤を設定して…
			this->TransparencyKey = ::Color::Red; // 赤を透明にする。

			// マウスを上げた時は閉じる時
			this->MouseUp += gcnew MouseEventHandler(this, &TenShougiWakuForm::form_OnMouseUp);
			// マウスを動かしている時は、フォームオープンの時とのズレの分を座標に反映させる。
			this->MouseMove += gcnew MouseEventHandler(this, &TenShougiWakuForm::form_OnMouseMove);
			// フォームが表示されたタイミングで、マウスの座標を覚える
			this->Shown += gcnew EventHandler(this, &TenShougiWakuForm::form_OnShown);
			this->FormClosed += gcnew FormClosedEventHandler(this, &TenShougiWakuForm::form_FormClosed);

			// 背景の画像貼り付け
			SetBackImage();

			// 半透明度
			this->Opacity = 0.05;

			this->ResumeLayout(true);

		}

	private:
		void SetBackImage() {
			// このプロジェクトのアセンブリのタイプを取得。
			System::Reflection::Assembly^ prj_assebmly = GetType()->Assembly;
			System::Resources::ResourceManager^ r = gcnew System::Resources::ResourceManager(String::Format("{0}.TenShougiMod", prj_assebmly->GetName()->Name), prj_assebmly);

			//-------------------------------------背景画像
			// リソースファイルから読み取り
			Bitmap^ rcimgback = (Bitmap^)(r->GetObject(this->strBgImgName));

			// ラベル作成。ピクチャーボックス替わり。
			lblWakuImage = gcnew Label();
			lblWakuImage->BackgroundImage = rcimgback;
			// フォームの内側(WAKU_MARGIN分だけ内側)に配置
			lblWakuImage->Left = WAKU_MARGIN;
			lblWakuImage->Top = WAKU_MARGIN;
			// 背景の大きさ
			lblWakuImage->Width = rcimgback->Width; 
			lblWakuImage->Height = rcimgback->Height;
			lblWakuImage->BackColor = Color::Transparent;

			// コントロールとしては反応しない。全てフォームへと流す。
			lblWakuImage->Enabled = false;
			

			this->Controls->Add(lblWakuImage);
		}

	private:
		// フォームが表示された時
		void form_OnShown(Object^ sender, EventArgs^ e) {
			// マウスのグローバル座標を保存。フォーム基準の座標に直す
			// ptMemoryMouse = this->PointToClient(Windows::Forms::Cursor::Position);
			ptMemoryMouse = Windows::Forms::Cursor::Position;
		}

		// この枠フォームが閉じられる時に、
		void form_FormClosed(Object^ sender, FormClosedEventArgs^ e) {
			// このフォームの位置まで、親フォームを持ってくる。
			Point^ pos = gcnew Point(this->Location.X + lblWakuImage->Location.X, this->Location.Y + lblWakuImage->Location.Y);
			formParent->Left = pos->X;
			formParent->Top = pos->Y;
		}

		// マウスを上げた時に、自分自身を閉じる
		void form_OnMouseUp(Object^ sender, MouseEventArgs^ e) {
			this->Close();
		}

		// マウスを動かしている時は、保存しておいた分のマウス座標とのズレを、親フォームの位置に足しこむ
		void form_OnMouseMove(Object^ sender, MouseEventArgs^ e) {

			// 左クリックしつつ動かしている
			/*
			if ((e->Button & ::MouseButtons::Left) == ::MouseButtons::Left) {
				// フォーム上のマウスの位置-覚えておいたマウスのポジション
				this->Left += e->X - ptMemoryMouse->X;
				this->Top += e->Y - ptMemoryMouse->Y;
			}
			*/

			// 左クリックしつつ動かしている
			if ((e->Button & ::MouseButtons::Left) == ::MouseButtons::Left) {

				// 半透明度
				this->Opacity = 0.9;

				//                           ┌ 最初の位置から動かした分のベクトル座標
				// 親の位置 + (今のカーソル位置-保存しておいたカーソル位置)
				this->Left = formParent->Left + (Windows::Forms::Cursor::Position.X - ptMemoryMouse->X) - WAKU_MARGIN;
				this->Top = formParent->Top + (Windows::Forms::Cursor::Position.Y - ptMemoryMouse->Y) - WAKU_MARGIN;

				// 枠画像の位置を８の倍数にまるめることで、ガタガタと動く感じを出す
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