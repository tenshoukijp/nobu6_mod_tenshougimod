#pragma once

#include "kifu_t.h"

using namespace System;
namespace KKC
{
	public ref class Kifu
	{
	private:
		literal int MAXMOVE = 1000;
		static cli::array<String^>^ strNumJP = gcnew cli::array<String^> {"", "１", "２", "３", "４", "５", "６", "７", "８", "９"};
		static cli::array<String^>^ strNumCN = gcnew cli::array<String^> {"", "一", "二", "三", "四", "五", "六", "七", "八", "九"};
		static cli::array<String^>^ strNumEN = gcnew cli::array<String^> {"", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
		static cli::array<String^>^ strKIFKoma1 = gcnew cli::array<String^> {"", "歩", "香", "桂", "銀", "金", "角", "飛", "玉", "と", "杏", "圭", "全", "", "馬", "龍"};
		static cli::array<String^>^ strKIFKoma2 = gcnew cli::array<String^> {"", "歩", "香", "桂", "銀", "金", "角", "飛", "玉", "と", "成香", "成桂", "成銀", "", "馬", "龍"};
		static cli::array<String^>^ strCSAKoma = gcnew cli::array<String^> {"", "FU", "KY", "KE", "GI", "KI", "KA", "HI", "OU", "TO", "NY", "NK", "NG", "", "UM", "RY"};
	public:
		static int KIF2Kifu(String^ strKIF, [System::Runtime::InteropServices::Out] kifu_t %kifu, [System::Runtime::InteropServices::Out] String^ %strMessage);
		static int KI22Kifu(String^ strKI2, [System::Runtime::InteropServices::Out] kifu_t %kifu, [System::Runtime::InteropServices::Out] String^ %strMessage);
	public:
		static int CSA2Kifu(String^ strCSA, [System::Runtime::InteropServices::Out] kifu_t %kifu, [System::Runtime::InteropServices::Out] String^ %strMessage);
		static String^ Kifu2KIF(kifu_t kifu);
		static String^ Kifu2KI2(kifu_t kifu);
		static String^ Kifu2CSA(kifu_t kifu);
	private:
		static bool strContent(String^ strin, String^ key, [System::Runtime::InteropServices::Out] String^ %strout);
	public:
		static String^ Num2Jp(int i, bool cn);
	private:
		static int Jp2Num(String^ str, bool cn);
		static int KIFKoma2Num(String^ str, bool single);
	public:
		static String^ Num2KIFKoma(int koma, bool single);
	private:
		static int CSAKoma2Num(String^ str);
		static String^ Num2CSAKoma(int i);
		static bool MakeMove(cli::array<int, 2>^ ban, cli::array<int, 2>^ dai, move_t move);
		static String^ makeKIFMove(String^ str);
	private:
		static void KI2Move2From(int koma, int tosuji, int todan, String ^strdir, cli::array<int, 2> ^ban, bool sente, [System::Runtime::InteropServices::Out] int %fromsuji, [System::Runtime::InteropServices::Out] int %fromdan);
		static String ^KI2MoveDir(move_t move, cli::array<int, 2> ^ban);
		static String ^strDet(cli::array<int> ^snum, int ix, int iy);
		static int getFromList(int koma, int tosuji, int todan, cli::array<int, 2> ^ban, cli::array<int> ^fromsuji, cli::array<int> ^fromdan);
	private:
		static int BAN(cli::array<int, 2> ^ban, int suji, int dan);
		static int getFromID(String ^str, bool sente, int nmove, cli::array<int> ^fromsuji, cli::array<int> ^fromdan, int tosuji, int todan);
		static void initBanDai(cli::array<int, 2> ^ban, cli::array<int, 2> ^dai);
	};
}


