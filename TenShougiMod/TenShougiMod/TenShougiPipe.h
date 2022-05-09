#pragma once

#include "TenShougiMod.h"

#include <string>
#include <vector>
#include <map>
#include <sstream>

using namespace std;

// TenShougiPipe側のヘッダ
extern BOOL bTenShougiRunThread;

extern BOOL GetAndSendInputThread(String^ text); // 送信ボタン押した時に実行する関数
extern void WINAPI TenShougiModBgn();
extern void WINAPI TenShougiModEnd();

extern CRITICAL_SECTION cs; // スレッドが存在するのでクリティカルセクション(排他制御)
extern vector<string> bufBonanzaAllOutput;  // ボナンザの標準出力全てを配列で持ったもの
extern vector<string> bufAllKifu;			// プレイヤとボナンザの棋譜
extern string bufPlayerKifuLastest;		    // プレイヤーが最後に入力した棋譜
extern double fCurKeiseiPoint;				// 先手後手、どちらが有利か。300ぐらいで絶望的。先手が+ 後手が-

extern std::string DotNetStringToMultiByte(String^ dotnettext);
extern vector<string> string_split(const string &str, char delim);

extern void ReNewTenShougiGame();	// 将棋の再スタート
extern void SaveCSAKifu();
extern void LoadCSAKifu();