#pragma once


#include "TenShougiMod.h"
#include "TenShougiBoard.h"
#include "TenShougiKoma.h"
#include "TenShougiSound.h"
#include "TenShougiBoardPBMap.h"
#include "Kifu.h"


// 名前空間。通常.NET用コンポーネントでは、Dllと同じ名前の空間に入れておくと、参照する際に都合が良い。
namespace TenShougiMod {

	// 天将棋用のフォーム
	ref class TenShougiForm : public Form {
	public:
		// グローバル変数用。自分自身をグローバルで持つ
		static TenShougiForm^ tsgForm;

	private:
		PictureBox^ pbBtnClose; // 閉じるボタン
		Bitmap^ btnClose;
		Bitmap^ btnCloseMouseDown;

		TenShougiBoadPBMap^ tsgBPBM; // 盤上にある駒達
		TenShougiSound^ snd;

		TextBox^ tbInputCmd;	// プレイヤーの手を入力するところ
		ComboBox^ cbbxKifuList; // 棋譜用のリスト

		Button^ btnSaveKifu;	// セーブ
		Button^ btnLoadKifu;	// セーブ

		Label^ lblFormTitle;   // タイトルラベル。ウィンドウのドラッグはこいつが担うドラッグ対応

		Timer^ timer; // タイマー
		Timer^ tmOpacity; // Opacity 調整用のタイマー

	public:
		// 半透明にする。PictureBoxがポロポロ出てくるのの対策
		void SetOpacityTimer() {
			tmOpacity = gcnew Timer();
			tmOpacity->Interval = 100;

			// 0.1秒後に半透明にするのやめる
			tmOpacity->Tick += gcnew EventHandler(this, &TenShougiForm::StopOpacityTimer );

			tmOpacity->Start();
		}


		// 半透明開始。
		void StartOpacityTimer() {
			this->Opacity = 0.01;
		}

		// 半透明やめる。
		void StopOpacityTimer(Object^ sendern, EventArgs^ e) {
			// 表示されているならば…
			if ( SharedMemory::iTenShougiFormShown ) {
				this->Opacity = 1.0;
			}
		}

		TenShougiForm() {
			this->SuspendLayout();

			// フォーム自身のプロパティの設定
			SetFormProperty();

			// フォームにGUIコンポーネントを配置
			AddGUIControls();

			this->ResumeLayout(true);

			SharedMemory::iTenShougiFormShown = 1;

			// タイマーの作成
			MakeTimer();

			// 半透明タイマーの作成
			SetOpacityTimer();
		}

	public:
		// タイトルバーの設定。独自カスタムなので必要
		void SetTitle(String^ title) {
			lblFormTitle->Text = title;
		}

	private:
		// 対戦武将のタイトル
		void SetVSBushouTitle() {

			// 天翔記→天将棋へと必要な値をコピーする。
			CopyTenshoukiProcessToTenshougiProcess();
	
			// 棋力情報が有効であったなら
			if ( strlen(SharedMemory::BPower.szDaniName) >0 && strlen(SharedMemory::WPower.szDaniName) >0 ) {
				int iBBushouID = SharedMemory::BPower.iBushouID;
				int iWBushouID = SharedMemory::WPower.iBushouID;
				SetTitle( gcnew String(nb6bushouname[iBBushouID].fastname) + gcnew String(SharedMemory::BPower.szDaniName)+
					      " 対 " + 
						  gcnew String(nb6bushouname[iWBushouID].fastname) + gcnew String(SharedMemory::WPower.szDaniName) );

			// 無効ならそっけなく無難なタイトル
			} else {
				SetTitle("天将棋");
			}
		}

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
			Bitmap^ rcimgback = (Bitmap^)(r->GetObject("TenShougiFormBackImg"));

			// 背景画像
			this->BackgroundImage = rcimgback;

			// 背景画像の大きさがフォームの大きさ
			this->Width = rcimgback->Width;
			this->Height = rcimgback->Height;

			//-------------------------------------背景画像
			// リソースファイルから読み取り
			System::Drawing::Icon^ iconform = (System::Drawing::Icon^)(r->GetObject("TenShougiFormIcon"));
			this->Icon = iconform;

			//-------------------------------------閉じるボタン
			// リソースファイルから読み取り
			btnClose     = (Bitmap^)(r->GetObject("TenShougiFormCloseButtonImg"));
			btnCloseMouseDown = (Bitmap^)(r->GetObject("TenShougiFormCloseButtonDownImg"));

			// 閉じるボタン
			pbBtnClose = gcnew PictureBox();
			pbBtnClose->Width = btnClose->Width;
			pbBtnClose->Height = btnClose->Height;
			pbBtnClose->Image = btnClose;

			// フォームを閉じた際のイベントハンドラ
			this->FormClosed += gcnew FormClosedEventHandler( this, &TenShougiForm::form_FormClosed);
		}

	public:
		// フォームを閉じる時
		virtual void Show() {
			// 半透明コントロールのタイマー作成
			StartOpacityTimer();

			SharedMemory::iTenShougiFormShown = 1;


			// 再表示などもあるので、対戦武将情報を更新しておく。
			SetVSBushouTitle();
			Form::Show();
		}

		// フォームを隠す
		virtual void Hide() {
			// 共有メモリに今見えてないよ!! 伝達。Closeしたわけではない。
			SharedMemory::iTenShougiFormShown = 0;
			Form::Hide();
		}

	private:
		// フォームを閉じる時
		void form_FormClosed( Object^ sender, FormClosedEventArgs^ e) {
			SharedMemory::iTenShougiFormShown = 0;
		}

	private:
		// GUIコンポーネントの配置
		void AddGUIControls() {
			//------------------------------------タイトルラベル
			// タイトルラベルの作成。
			lblFormTitle = gcnew Label();
			// ここから４つは目による微妙な値調整
			lblFormTitle->Top = 6;
			lblFormTitle->Left = 23;
			lblFormTitle->Width = 440;
			lblFormTitle->Height = 21;
			// フォントの設定
			System::Drawing::Font^ font = gcnew System::Drawing::Font(gcnew String(sgGetTenShougiFontName()), 12, FontStyle::Bold);
			lblFormTitle->Font = font;
			// フォントの色
			lblFormTitle->ForeColor = ::Color::White;
			// 配置一
			lblFormTitle->TextAlign = ContentAlignment::TopCenter;

			SetVSBushouTitle();

			lblFormTitle->BackColor = ::Color::Transparent;

			// タイトルがなくなったので、(ButtonやPictureBoxがない)form直接をドラッグすると動かせるようにする)
			lblFormTitle->MouseDown += gcnew MouseEventHandler(this, &TenShougiForm::form_MouseDown);
			lblFormTitle->MouseMove += gcnew MouseEventHandler(this, &TenShougiForm::form_MouseMove);
			// フォームでもついでに動くようにする。
			this->MouseDown += gcnew MouseEventHandler(this, &TenShougiForm::form_MouseDown);
			this->MouseMove += gcnew MouseEventHandler(this, &TenShougiForm::form_MouseMove);

			//------------------------------------送信・入力・棋譜結果コンボボックス
			// それぞれオブジェクト生成
			tbInputCmd = gcnew TextBox();
			tbInputCmd->Top = 35;
			tbInputCmd->Left = 400;
			tbInputCmd->Width = 70;
			tbInputCmd->KeyDown += gcnew KeyEventHandler(this, &TenShougiForm::tbInputCmd_KeyDown); // キー入力時のイベントハンドラ

			cbbxKifuList = gcnew ComboBox();
			cbbxKifuList->Top = 320;
			cbbxKifuList->Left = 10;
			cbbxKifuList->Width = 76;
			cbbxKifuList->BackColor = ::Color::PaleGoldenrod;
			System::Drawing::Font^ cbfont = gcnew System::Drawing::Font(gcnew String(sgGetTenShougiFontName()), 9);
			cbbxKifuList->Font = cbfont;

			btnSaveKifu = gcnew Button();
			btnSaveKifu->Text = "保存";
			btnSaveKifu->Left = 10;
			btnSaveKifu->Top = 290;
			btnSaveKifu->Width = 76;
			btnSaveKifu->Click += gcnew EventHandler(this, &TenShougiForm::btnSaveKifu_Click);

			btnLoadKifu = gcnew Button();
			btnLoadKifu->Text = "読込";
			btnLoadKifu->Left = 10;
			btnLoadKifu->Top = 290;
			btnLoadKifu->Width = 76;
			btnLoadKifu->Click += gcnew EventHandler(this, &TenShougiForm::btnLoadKifu_Click);

			pbBtnClose->Top = 5;  // 背景画像との位置関係により、この場所固定
			pbBtnClose->Left = 5; // 背景画像との位置関係により、この場所固定
			pbBtnClose->MouseMove += gcnew MouseEventHandler(this, &TenShougiForm::pbBtnClose_MouseMove);
			pbBtnClose->MouseDown += gcnew MouseEventHandler(this, &TenShougiForm::pbBtnClose_MouseDown);
			pbBtnClose->MouseUp += gcnew MouseEventHandler(this, &TenShougiForm::pbBtnClose_MouseUp);

			//------------------------------------コントロール配置
			// this->Controls->Add(tbInputCmd);
			this->Controls->Add(cbbxKifuList);
			this->Controls->Add(lblFormTitle);

			// ★セーブボタンを一端外す
			//	this->Controls->Add(btnSaveKifu);
			// this->Controls->Add(btnLoadKifu);

			this->Controls->Add(pbBtnClose); // 閉じるボタンの配置。

			// 駒達の初期化
			tsgBPBM = gcnew TenShougiBoadPBMap(this);
			tsgBPBM->UpdatePositionByBoardSlot();

			// 駒音
			snd = gcnew TenShougiSound();
		}

	private:
		// タイマーの作成
		void MakeTimer() {

			timer = gcnew Timer();  // タイマ

			timer->Interval = 100;  // 0.1秒間隔

			// タイマ用のイベントハンドラを登録
			timer->Tick += gcnew EventHandler(this, &TenShougiForm::UpdateCbbxKifuList);

			timer->Start();  // タイマ ON
		}


	private:
		// 閉じる画像ボタンをマウス押したら
		void pbBtnClose_MouseDown(Object^ sender, MouseEventArgs^ e) {
			// 左ボタンなら
			if ( e->Button == ::MouseButtons::Left ) {
				// クローズボタンのイメージを押された状態にする。
				pbBtnClose->Image = btnCloseMouseDown;
			}
		}

		// 閉じる画像ボタンをマウス押すのをやめたら
		void pbBtnClose_MouseUp(Object^ sender, MouseEventArgs^ e) {
			pbBtnClose->Image = btnClose;

			// マウスを離した時にマウスがボタンの中に入っていたら…
			if ( pbBtnClose->ClientRectangle.Contains(e->Location) ) {
				// 左ボタンなら
				if ( e->Button == ::MouseButtons::Left ) {
					// 自分自身(フォーム)を閉じる
					this->Hide();
				}
			}
		}

		// 閉じる画像ボタンをマウスが動いている
		void pbBtnClose_MouseMove(Object^ sender, MouseEventArgs^ e) {

			// マウスが、閉じるボタン内から外れた
			if ( ! pbBtnClose->ClientRectangle.Contains(e->Location) ) {
				// 画像を元へと戻す
				pbBtnClose->Image = btnClose;
			}
		}

		// コンボボックスの常時更新。(仮処理)
		void UpdateCbbxKifuList(Object^ sender, EventArgs^ event) {

			// コンボボックスの長さと、棋譜の長さが同じなら何もしない。
			if (cbbxKifuList->Items->Count == bufAllKifu.size() ) {
				return;
			}

			// コンボボックスと記譜の長さが違うなら…以下の処理

			// コンボボックスをクリアして…
			cbbxKifuList->Items->Clear();

			// コンボボックス描画停止
			cbbxKifuList->BeginUpdate();

			//--------------------------------------------- C R I T I C A L  S E C T I O N  B E G I N -----------------------------------------------------
			// クリティカルセクション開始。他のクリティカルセクションを持つスレッドはクリティカルセクション内に書き込み出来ない
			// クリティカルセクションに入り、bufBonanzaKifu の操作をロック
			EnterCriticalSection(&cs);

			// TenShougiModKifu.dllがあるならば
			if ( true || System::IO::File::Exists("TenShougiModKKC.dll") ) {
				// KI2タイプ(日本語の▲７六歩とか)のタイプで、コンボボックスにコピーする。TenShougiModKifu.dll 必須。
				CopyToKI2KifuToComboBox();

				// TenShougiModKifu.dllが無いならば
			} else {
				// CSAタイプ(英語の++7776FU)のタイプで、コンボボックスにコピーする。何にも頼らない。
				CopyToCSAKifuToComboBox();
			}

			// クリティカルセクションから出る。
			LeaveCriticalSection(&cs);
			//--------------------------------------------- C R I T I C A L  S E C T I O N  E N D -----------------------------------------------------

			// コンボボックス描画再開
			cbbxKifuList->EndUpdate();
		}

	private:

		// CSAタイプ(英語の++7776FU)のタイプで、コンボボックスにコピーする。何にも頼らない。
		void CopyToCSAKifuToComboBox() {
			// 記譜を、CompoBoxへと全部コピー
			for each(string sjiskifu in bufAllKifu) {
				String^ objkifu = gcnew String(sjiskifu.c_str());
				cbbxKifuList->Items->Add(objkifu);
			}
		}

		// KI2タイプ(日本語の▲７六歩とか)のタイプで、コンボボックスにコピーする。TenShougiModKifu.dll 必須。
		void CopyToKI2KifuToComboBox() {

			// ----------------------------TenShougiModKifuを使って、CSA型の棋譜(+7766FU)をKI2型の棋譜(▲７六歩)などに直す。
			// KI2形式の一番初めは、先手が「+」という記号であることを示す。
			std::string allConcatStr = "+\n";
			// 記譜を、改行で連結
			for each(string sjiskifu in bufAllKifu) {
				allConcatStr += sjiskifu + "\n";
			}
			// 棋譜変換のための型
			KKC::kifu_t obj_kifu;
			// KI2型
			String^ message = gcnew String("");
			// CSA→Kifuへの変換
			KKC::Kifu::CSA2Kifu( gcnew String(allConcatStr.c_str()), obj_kifu, message);
			// Kifu→KI2への変換
			String^ strDotNetKi2 = KKC::Kifu::Kifu2KI2( obj_kifu );
			// C#型文字列→マルチバイトへの変換
			std::string strMBKi2 = DotNetStringToMultiByte(strDotNetKi2);
			// 全部が１つの文字列になっているので、'\n'で分割。行ごととする。
			vector<string> vStrMBKi2 = string_split( strMBKi2, '\n' ); // 改行で分割
			// 
			// 改行ごとに分解されたのをなめていって…
			for each (string ki2line in vStrMBKi2) {
				// KI2タイプの棋譜が連結された行だ
				if ( ki2line.find("▲") != string::npos ) {
					// ▲７六歩 △３四歩 ▲６六歩 △４二銀 ▲２六歩 △５二飛 ▲２五歩 △３三角 みたいになるので、半角スペースで分解して
					vector<std::string> vKi2Line = string_split(ki2line, ' '); // スペースで区切って分解する。
					for each ( std::string c_te in vKi2Line ) {
						String^ o_te = gcnew String(c_te.c_str()); // C++マルチバイト→C++/CLI.NET型へｔ
						cbbxKifuList->Items->Add(o_te); // 分解した最後の要素をコンボボックスに追加する。
					}
				}
			}

		}

	public:
		//---------------------------------------------盤更新
		void UpdatePositionByBoardSlot() {
			tsgBPBM->UpdatePositionByBoardSlot();
		}

		//---------------------------------------------駒音
		void PlayKomaSound(std::string CSAstring) {
			if (CSAstring == "DummySound" ) {
				snd->PlayDummySound();
			} else {
				snd->PlayKomaSound(CSAstring);
			}
		}


	private:

		//押されたキーがエンターキーかどうかの条件分岐
		void tbInputCmd_KeyDown(Object^ sender, KeyEventArgs^ e) {

			// イベント発声がテキストボックスで…
			if ((TextBox^)sender == tbInputCmd) {
				// エンターキーを押したならば…
				if (e->KeyCode == System::Windows::Forms::Keys::Enter) {
					if ( tbInputCmd->Text == "renew game" ) {
						ReNewTenShougiGame();
					} else {
						SendMessageToBonanza();
					}
				}
			}
		}

		// ボナンザに、テキストボックスに入れた内容を送信する。
		void SendMessageToBonanza() {

			// 先手番でなければ何もしない。
			if ( TenShougiBoard::GetCurTurnState() != TenShougiBoard::CurTurnB) {
				return;
			}

			// 空っぽいでないならば
			if ( tbInputCmd->Text != "" ) {

				// 送信して、送信できたか結果をもらう。
				int issuccess = GetAndSendInputThread(tbInputCmd->Text);

				// もし成功なら
				if (issuccess) {
					// 中身からっぽ
					tbInputCmd->Text = "";
				}
			}
		}

		// 保存ボタンを押した時
		void btnSaveKifu_Click(Object^ sender, EventArgs^ e) {
			// 保存や読み出しのマニュアル操作は先手番限定
			if ( TenShougiBoard::GetCurTurnState() == TenShougiBoard::CurTurnB ) {
				SaveCSAKifu();
			}
		}

		// 読込ボタンを押した時
		void btnLoadKifu_Click(Object^ sender, EventArgs^ e) {
			// 保存や読み出しのマニュアル操作は先手番限定
			if ( TenShougiBoard::GetCurTurnState() == TenShougiBoard::CurTurnB ) {
				LoadCSAKifu();
			}
		}

	private:
		//http://dobon.net/vb/dotnet/form/moveform.html
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
			TenShougiWakuForm^ waku = gcnew TenShougiWakuForm(this, "TenShougiWakuForm");
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
