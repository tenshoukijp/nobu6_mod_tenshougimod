#pragma once

#include "GameDataStruct.h"
#include <string>


namespace SharedMemory {
	extern int iCurrentTurnState; // 現在のターンがプレイヤかＣＰＵか。0:プレイヤ, 1:CPU
	extern int iTenShougiFormShown; // フォームが表示されているかどうか。

	extern TBUshouDaniPowerInfo BPower;	// 後手の棋力
	extern TBUshouDaniPowerInfo WPower;	// 先手の棋力
}

// 天翔記で今表示しているフォント名を得る。
// 無ければ、ＭＳ 明朝とする。
extern char* sgGetTenShougiFontName();

// 天翔記から送られてきているコマンドを取り出す。
std::string PopCommand();

// 天翔記プロセスから天将棋プロセスへと値をコピーする。
void LoadTenshouHandle();

// 確保したプロセスハンドルの解放
void FreeTenshouHandle();

// 天翔記→天将棋へとメモリをコピー
void CopyTenshoukiProcessToTenshougiProcess();