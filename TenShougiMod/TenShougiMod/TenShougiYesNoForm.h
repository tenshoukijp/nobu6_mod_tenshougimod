#pragma once


#include "TenShougiMod.h"
#include "TenShougiForm.h"
#include "TenShougiWakuForm.h"

// 名前空間。通常.NET用コンポーネントでは、Dllと同じ名前の空間に入れておくと、参照する際に都合が良い。
namespace TenShougiMod {

	// 天将棋用のフォーム
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

		// フォームのプロパティ設定
		void SetFormProperty() {

			// タイトルバー＆フォームボーダーを消去。後述で設定する背景画像がフォームまんまに見えるようにする。
			// ただしこれをすると、タイトルバーをつかめないので、ウィンドウを動かすすべがなくなる。
			this->FormBorderStyle = ::FormBorderStyle::None;

			// このプロジェクトのアセンブリのタイプを取得。
			System::Reflection::Assembly^ prj_assebmly = GetType()->Assembly;
			System::Resources::ResourceManager^ r = gcnew System::Resources::ResourceManager(String::Format("{0}.TenShougiMod", prj_assebmly->GetName()->Name), prj_assebmly);


			//-------------------------------------背景画像
			// リソースファイルから読み取り
			Bitmap^ rcimgback = (Bitmap^)(r->GetObject("ModalFormBackImg"));

			// 背景画像
			this->BackgroundImage = rcimgback;

			// 背景画像の大きさがフォームの大きさ
			this->Width = rcimgback->Width;
			this->Height = rcimgback->Height;

			//-------------------------------------ボタンのリソース
			// リソースファイルから読み取り
			bmButton      = (Bitmap^)(r->GetObject("ModalFormButton"));
			bmButtonDown = (Bitmap^)(r->GetObject("ModalFormButtonMouseDown"));

			// フォームでもついでに動くようにする。
			this->MouseDown += gcnew MouseEventHandler(this, &TenShougiYesNoForm::form_MouseDown);
			this->MouseMove += gcnew MouseEventHandler(this, &TenShougiYesNoForm::form_MouseMove);
		}

		// GUIコンポーネントの配置
		void AddGUIControls() {
			// フォントの設定
			System::Drawing::Font^ font = gcnew System::Drawing::Font(gcnew String(sgGetTenShougiFontName()), 12, FontStyle::Bold);

			lblMainMessage = gcnew Label();
			lblMainMessage->Text = strMainMessage; // ダイアログの真ん中の文章
			lblMainMessage->Left = 0;
			lblMainMessage->Top = 28 + 8; // 1行なので16の半分の8足し込み
			lblMainMessage->Width = this->Width - lblMainMessage->Left;
			lblMainMessage->Height = 32;
			lblMainMessage->TextAlign = ::ContentAlignment::TopCenter;
			lblMainMessage->Font = font;
			lblMainMessage->ForeColor = ::Color::Black;
			lblMainMessage->BackColor = ::Color::Transparent;

			// ボタン２個の場合
			bool is_bothbutton = ( strNo != "" );

			// YESボタン
			pnlButtonYes = gcnew Panel();
			pnlButtonYes->Width = bmButton->Width;
			pnlButtonYes->Height = bmButton->Height;
			pnlButtonYes->Left = 8;
			pnlButtonYes->Top = 70;
			pnlButtonYes->BackgroundImage = bmButton;

			// ボタンが2個の場合だけ…
			if ( is_bothbutton ) {
				// NOボタン
				pnlButtonNo = gcnew Panel();
				pnlButtonNo->Width = bmButton->Width;
				pnlButtonNo->Height = bmButton->Height;
				pnlButtonNo->Left = 88;
				pnlButtonNo->Top = 70;
				pnlButtonNo->BackgroundImage = bmButton;

			// ボタンが一個の時は Yesボタンの位置を中央に
			} else {
			pnlButtonYes->Left = 48;
			}

			// YESボタン
			lblYes = gcnew Label();
			lblYes->Parent = pnlButtonYes;
			lblYes->Width = bmButton->Width;
			lblYes->Height = bmButton->Height;
			lblYes->Left = 0;
			lblYes->Top = 1;
			lblYes->Font = font;
			lblYes->TextAlign = ContentAlignment::MiddleCenter;
			lblYes->Text = strYes; // Yes側の文字列
			lblYes->ForeColor = ::Color::Black;
			lblYes->BackColor = ::Color::Transparent;
			ptLblYes = gcnew Point(lblYes->Left, lblYes->Top); // オリジナル座標を控えておく
			lblYes->MouseMove += gcnew MouseEventHandler(this, &TenShougiYesNoForm::lblYes_MouseMove);
			lblYes->MouseDown += gcnew MouseEventHandler(this, &TenShougiYesNoForm::lblYes_MouseDown);
			lblYes->MouseUp += gcnew MouseEventHandler(this, &TenShougiYesNoForm::lblYes_MouseUp);

			// ボタンが2個の場合だけ…
			if ( is_bothbutton ) {
				// NOボタン
				lblNo = gcnew Label();
				lblNo->Parent = pnlButtonNo;
				lblNo->Width = bmButton->Width;
				lblNo->Height = bmButton->Height;
				lblNo->Left = 0;
				lblNo->Top = 1;
				lblNo->Font = font;
				lblNo->TextAlign = ContentAlignment::MiddleCenter;
				lblNo->Text =  strNo; // No側の文字列
				lblNo->ForeColor = ::Color::Black;
				lblNo->BackColor = ::Color::Transparent;
				ptLblNo = gcnew Point(lblNo->Left, lblNo->Top); // オリジナル座標を控えておく
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

			// GUIコンポーネントの配置
			AddGUIControls();

			this->ResumeLayout(false);
		}


	private:
		Point^ ptLblYes;

		// 成るのボタンをマウス押したら
		void lblYes_MouseDown(Object^ sender, MouseEventArgs^ e) {
			// 左ボタンなら
			if ( e->Button == ::MouseButtons::Left ) {
				// イメージを押された状態にする。
				pnlButtonYes->BackgroundImage = bmButtonDown;

				// ラベルを右下に1ドット下げる
				lblYes->Left = lblYes->Left + 1;
				lblYes->Top = lblYes->Top + 1;
			}
		}

		// 成るボタンをマウス押すのをやめたら
		void lblYes_MouseUp(Object^ sender, MouseEventArgs^ e) {
			pnlButtonYes->BackgroundImage = bmButton;

			// 位置を元へと戻す
			lblYes->Left = ptLblYes->X;
			lblYes->Top = ptLblYes->Y;

			// マウスを離した時にマウスがボタンの中に入っていたら…
			if ( lblYes->ClientRectangle.Contains(e->Location) ) {
				// 左ボタンなら
				if ( e->Button == ::MouseButtons::Left ) {
					// 自分自身(フォーム)を閉じる
					this->DialogResult = ::DialogResult::Yes; // 成らずを決定
				}
			}
		}

		// 成るボタンを押してるマウスが動いている
		void lblYes_MouseMove(Object^ sender, MouseEventArgs^ e) {

			// マウスが、閉じるボタン内から外れた
			if ( ! lblYes->ClientRectangle.Contains(e->Location) ) {
				// 画像を元へと戻す
				pnlButtonYes->BackgroundImage = bmButton;

				// 位置を元へと戻す
				lblYes->Left = ptLblYes->X;
				lblYes->Top = ptLblYes->Y;

			}
		}


	private:
		Point^ ptLblNo;

		// 成らずのボタンをマウス押したら
		void lblNo_MouseDown(Object^ sender, MouseEventArgs^ e) {
			// 左ボタンなら
			if ( e->Button == ::MouseButtons::Left ) {
				// イメージを押された状態にする。
				pnlButtonNo->BackgroundImage = bmButtonDown;

				// ラベルを右下に1ドット下げる
				lblNo->Left = lblNo->Left + 1;
				lblNo->Top = lblNo->Top + 1;
			}
		}

		// 成らずボタンをマウス押すのをやめたら
		void lblNo_MouseUp(Object^ sender, MouseEventArgs^ e) {
			pnlButtonNo->BackgroundImage = bmButton;

			// 位置を元へと戻す
			lblNo->Left = ptLblNo->X;
			lblNo->Top = ptLblNo->Y;

			// マウスを離した時にマウスがボタンの中に入っていたら…
			if ( lblNo->ClientRectangle.Contains(e->Location) ) {
				// 左ボタンなら
				if ( e->Button == ::MouseButtons::Left ) {
					this->DialogResult = ::DialogResult::No; // 成らずを決定
				}
			}
		}

		// 成らずボタンを押してるマウスが動いている
		void lblNo_MouseMove(Object^ sender, MouseEventArgs^ e) {

			// マウスが、閉じるボタン内から外れた
			if ( ! lblNo->ClientRectangle.Contains(e->Location) ) {
				// 画像を元へと戻す
				pnlButtonNo->BackgroundImage = bmButton;

				// 位置を元へと戻す
				lblNo->Left = ptLblNo->X;
				lblNo->Top = ptLblNo->Y;

			}
		}


		// ---------------------------------マウスに合わせてフォーム動く
	private:
		//マウスのクリック位置を記憶
		Point^ ptMemoryMouse;

	private:
		// このフォームのMouseDownイベントハンドラ
		// マウスのボタンが押されたとき
		void form_MouseDown(Object^ sender, MouseEventArgs^ e) {
			try {
				if ((e->Button & ::MouseButtons::Left) == ::MouseButtons::Left) {
					//位置を記憶する
					ptMemoryMouse = gcnew Point(e->X, e->Y);
				}
			} catch( NullReferenceException^ /*err*/) {
			}
		}

		//このフォームのMouseMoveイベントハンドラ
		//マウスが動いたとき
		void form_MouseMove(Object^ sender, MouseEventArgs^ e) {

			/*
			// 左ボタンおしてて動いている
			if ((e->Button & ::MouseButtons::Left) == ::MouseButtons::Left) {

				// 枠用のウィンドウの作成
				TenShougiWakuForm^ waku = gcnew TenShougiWakuForm(this, "TenShougiWakuDialog");
				// モーダルウィンドウが出る時に、自分と同じセンター
				waku->StartPosition = ::FormStartPosition::CenterParent;
				// 表示する。
				waku->ShowDialog();
				// 新しく作った方をアクティブにする。
				waku->Activate();
			}
			*/

			try {
				// 座標を足しこんで動かす
				if ((e->Button & ::MouseButtons::Left) == ::MouseButtons::Left) {
					this->Left += e->X - ptMemoryMouse->X;
					this->Top += e->Y - ptMemoryMouse->Y;
				}
			} catch( NullReferenceException^ /*err*/) {
			}
		}
	};
}

