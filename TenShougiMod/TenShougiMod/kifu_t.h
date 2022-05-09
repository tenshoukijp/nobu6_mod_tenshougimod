#pragma once

#include "move_t.h"

using namespace System;
namespace KKC
{
	public value class kifu_t {
	public:
		String ^strFile;
		cli::array<move_t> ^Move;
		int numMove;
		int intResult;
		String ^strNameS;
		String ^strNameG;
		String ^strStartTime;
		String ^strEndTime;
		String ^strEvent;
		String ^strEvent2;
		String ^strSite;
		String ^strTimeLimit;
		String ^strTimeUsed;
		String ^strOpening;
		String ^strNote;
		String ^strComment;
	};
}
