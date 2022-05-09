#pragma once


#include "TenShougiMod.h"


namespace TenShougiMod {

	// 天翔記の音イメージのクラス。
	// 音の名前→サウンドへの変換用
	ref class TenShougiSound {

	public:
		System::IO::Stream^ strmKomaHghSound; // 駒イメージ名→駒画像のハッシュオブジェクト
		System::Media::SoundPlayer^ spHghPlayer;
		System::IO::Stream^ strmKomaLowSound; // 駒イメージ名→駒画像のハッシュオブジェクト
		System::Media::SoundPlayer^ spLowPlayer;
		System::IO::Stream^ strmDummySound; // 駒イメージ名→駒画像のハッシュオブジェクト
		System::Media::SoundPlayer^ spDummyPlayer;
	public:
		TenShougiSound() {

			// このプロジェクトのアセンブリのタイプを取得。
			System::Reflection::Assembly^ prj_assebmly = GetType()->Assembly;
			System::Resources::ResourceManager^ r = gcnew System::Resources::ResourceManager(String::Format("{0}.TenShougiMod", prj_assebmly->GetName()->Name), prj_assebmly);

			// リソースファイルから読み取り
			strmKomaHghSound = (System::IO::Stream^)(r->GetObject("KomaHghMoveSound"));
			// 再生機を使いまわす。
			spHghPlayer = gcnew System::Media::SoundPlayer( strmKomaHghSound );

			strmKomaLowSound = (System::IO::Stream^)(r->GetObject("KomaLowMoveSound"));
			// 再生機を使いまわす。
			spLowPlayer = gcnew System::Media::SoundPlayer( strmKomaLowSound );

			strmDummySound = (System::IO::Stream^)(r->GetObject("DummySound"));
			// 再生機を使いまわす。
			spDummyPlayer = gcnew System::Media::SoundPlayer( strmDummySound );

		}

		void PlayKomaSound(std::string CSAstring) {

			// ウィンドウが開かれている
			if ( SharedMemory::iTenShougiFormShown ) {

				// 小さい駒は高めの音、それ以外の駒は低めの音を出す。			

				string highList[] = {"FU", "KY", "KE", "TO", "NY", "NK"}; // 小さな駒は音が高い

				int isHigh = false;

				for ( int i=0; i < sizeof(highList)/sizeof(highList[0]); i++) {
					// 小さい駒の文字列が見つかった!!
					if ( CSAstring.find(highList[i]) != string::npos ) {
						isHigh = true;
					}
				}

				if ( isHigh ) {
					//高い音を非同期再生する
					spHghPlayer->Play();
				} else {
					//低い音を非同期再生する
					spLowPlayer->Play();
				}
			}
		}

		void PlayDummySound() {
			// ダミーサウンド(無音)を出すことで、音再生が最初に遅れるのを防ぐ
			spDummyPlayer->Play();
		}
	};
}