#pragma once

using namespace System;
namespace KKC
{
	public value class move_t {
	public:
		int fromsuji;
		int fromdan;
		int tosuji;
		int todan;
		int koma;
		int promote;
		int dtime;
		int ttime;
		String ^comment;
	};
}
