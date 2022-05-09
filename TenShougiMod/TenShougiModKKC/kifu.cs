using System;
namespace KKC
{
    public class Kifu
    {
        private const int MAXMOVE = 1000;
        private static string[] strNumJP = new string[]
		{
			"",
			"１",
			"２",
			"３",
			"４",
			"５",
			"６",
			"７",
			"８",
			"９"
		};
        private static string[] strNumCN = new string[]
		{
			"",
			"一",
			"二",
			"三",
			"四",
			"五",
			"六",
			"七",
			"八",
			"九"
		};
        private static string[] strNumEN = new string[]
		{
			"",
			"1",
			"2",
			"3",
			"4",
			"5",
			"6",
			"7",
			"8",
			"9"
		};
        private static string[] strKIFKoma1 = new string[]
		{
			"",
			"歩",
			"香",
			"桂",
			"銀",
			"金",
			"角",
			"飛",
			"玉",
			"と",
			"杏",
			"圭",
			"全",
			"",
			"馬",
			"龍"
		};
        private static string[] strKIFKoma2 = new string[]
		{
			"",
			"歩",
			"香",
			"桂",
			"銀",
			"金",
			"角",
			"飛",
			"玉",
			"と",
			"成香",
			"成桂",
			"成銀",
			"",
			"馬",
			"龍"
		};
        private static string[] strCSAKoma = new string[]
		{
			"",
			"FU",
			"KY",
			"KE",
			"GI",
			"KI",
			"KA",
			"HI",
			"OU",
			"TO",
			"NY",
			"NK",
			"NG",
			"",
			"UM",
			"RY"
		};
        public static int KIF2Kifu(string strKIF, out kifu_t kifu, out string strMessage)
        {
            kifu = default(kifu_t);
            strMessage = string.Empty;
            try
            {
                int num = 0;
                int num2 = 0;
                int num3 = 0;
                bool flag = false;
                int[,] ban = new int[9, 9];
                int[,] dai = new int[2, 7];
                kifu.Move = new move_t[1000];
                Kifu.initBanDai(ban, dai);
                string[] array = strKIF.Split(new char[]
				{
					'\n',
					'\r'
				});
                string[] array2 = array;
                for (int i = 0; i < array2.Length; i++)
                {
                    string text = array2[i];
                    string text2 = text.Trim();
                    if (!(text2 == string.Empty) && !text2.StartsWith("&") && !text2.StartsWith("#"))
                    {
                        if (text2.StartsWith("変化"))
                        {
                            break;
                        }
                        string text3;
                        if (!flag)
                        {
                            if (text2.StartsWith("手合割") && (text2.Contains("落") || text2.Contains("その他")))
                            {
                                int result = 2;
                                return result;
                            }
                            if (text2.StartsWith("後手番"))
                            {
                                int result = 2;
                                return result;
                            }
                            if (Kifu.strContent(text2, "開始日時：", out text3))
                            {
                                kifu.strStartTime = text3;
                            }
                            else
                            {
                                if (Kifu.strContent(text2, "終了日時：", out text3))
                                {
                                    kifu.strEndTime = text3;
                                }
                                else
                                {
                                    if (Kifu.strContent(text2, "持ち時間：", out text3))
                                    {
                                        kifu.strTimeLimit = text3;
                                    }
                                    else
                                    {
                                        if (Kifu.strContent(text2, "消費時間：", out text3))
                                        {
                                            kifu.strTimeUsed = text3;
                                        }
                                        else
                                        {
                                            if (Kifu.strContent(text2, "先手：", out text3))
                                            {
                                                kifu.strNameS = text3;
                                            }
                                            else
                                            {
                                                if (Kifu.strContent(text2, "後手：", out text3))
                                                {
                                                    kifu.strNameG = text3;
                                                }
                                                else
                                                {
                                                    if (Kifu.strContent(text2, "棋戦：", out text3))
                                                    {
                                                        kifu.strEvent = text3;
                                                    }
                                                    else
                                                    {
                                                        if (Kifu.strContent(text2, "場所：", out text3))
                                                        {
                                                            kifu.strSite = text3;
                                                        }
                                                        else
                                                        {
                                                            if (Kifu.strContent(text2, "戦型：", out text3))
                                                            {
                                                                kifu.strOpening = text3;
                                                            }
                                                            else
                                                            {
                                                                if (Kifu.strContent(text2, "備考：", out text3))
                                                                {
                                                                    kifu.strNote = text3;
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        if (Kifu.strContent(text2, "*棋戦詳細：", out text3))
                        {
                            kifu.strEvent2 = text3;
                        }
                        if (text2.Contains("手数-") && text2.Contains("指手-"))
                        {
                            flag = true;
                        }
                        else
                        {
                            if (flag)
                            {
                                if (text2.StartsWith("*"))
                                {
                                    if (!text2.StartsWith("*棋戦詳細"))
                                    {
                                        if (num3 == 0)
                                        {
                                            if (Kifu.strContent(text2, "*", out text3))
                                            {
                                                kifu.strComment = kifu.strComment + (string.IsNullOrEmpty(kifu.strComment) ? "" : "\n") + text3;
                                            }
                                        }
                                        else
                                        {
                                            if (Kifu.strContent(text2, "*", out text3))
                                            {
                                                move_t[] expr_2F4_cp_0 = kifu.Move;
                                                int expr_2F4_cp_1 = num3 - 1;
                                                expr_2F4_cp_0[expr_2F4_cp_1].comment = expr_2F4_cp_0[expr_2F4_cp_1].comment + (string.IsNullOrEmpty(kifu.Move[num3 - 1].comment) ? "" : "\n") + text3;
                                            }
                                        }
                                    }
                                }
                                else
                                {
                                    if (text2.Contains("投了") || text2.Contains("詰み") || text2.Contains("勝"))
                                    {
                                        kifu.intResult = ((num3 % 2 == 1) ? 1 : 2);
                                        break;
                                    }
                                    if (text2.Contains("千日手"))
                                    {
                                        kifu.intResult = 3;
                                        break;
                                    }
                                    if (text2.Contains("持将棋"))
                                    {
                                        kifu.intResult = 4;
                                        break;
                                    }
                                    if (text2.Contains("中断"))
                                    {
                                        kifu.intResult = 0;
                                        break;
                                    }
                                    text2 = text2.Replace("同\u3000", "同");
                                    text2 = text2.Replace("同 ", "同");
                                    text2 = text2.Replace("同", "同\u3000");
                                    string[] array3 = text2.Split(new char[]
									{
										' '
									});
                                    int num4 = 0;
                                    string[] array4 = array3;
                                    for (int j = 0; j < array4.Length; j++)
                                    {
                                        string text4 = array4[j];
                                        if (!(text4 == string.Empty))
                                        {
                                            if (num4 == 0)
                                            {
                                                if (int.Parse(text4) != num3 + 1)
                                                {
                                                    strMessage = "手数が正しくありません\n" + text2;
                                                    int result = 1;
                                                    return result;
                                                }
                                                num4++;
                                            }
                                            else
                                            {
                                                if (num4 == 1)
                                                {
                                                    if (text4.Length < 4)
                                                    {
                                                        break;
                                                    }
                                                    string text5 = Kifu.makeKIFMove(text4);
                                                    int num5;
                                                    int num6;
                                                    if (!text5.Contains("同") && text5.Length > 1)
                                                    {
                                                        num5 = Kifu.Jp2Num(text5.Substring(0, 1), false);
                                                        num6 = Kifu.Jp2Num(text5.Substring(1, 1), true);
                                                    }
                                                    else
                                                    {
                                                        num5 = num;
                                                        num6 = num2;
                                                    }
                                                    if (text5.Length <= 2)
                                                    {
                                                        strMessage = "指し手が正しくありません\n" + text2;
                                                        int result = 1;
                                                        return result;
                                                    }
                                                    int num7 = Kifu.KIFKoma2Num(text5.Substring(2, 1), true);
                                                    if (num7 > 0)
                                                    {
                                                        num7 += ((num3 % 2 == 0) ? 16 : 32);
                                                    }
                                                    int promote = text5.Contains("成") ? 1 : 0;
                                                    int num8;
                                                    int num9;
                                                    if (text5.Contains("打"))
                                                    {
                                                        num8 = 0;
                                                        num9 = 0;
                                                    }
                                                    else
                                                    {
                                                        if (!text5.Contains("(") || !text5.Contains(")"))
                                                        {
                                                            strMessage = "指し手が正しくありません\n" + text2;
                                                            int result = 1;
                                                            return result;
                                                        }
                                                        int num10 = text5.IndexOf("(");
                                                        if (text5.Length <= num10 + 2)
                                                        {
                                                            strMessage = "指し手が正しくありません\n" + text2;
                                                            int result = 1;
                                                            return result;
                                                        }
                                                        int.TryParse(text5.Substring(num10 + 1, 1), out num8);
                                                        int.TryParse(text5.Substring(num10 + 2, 1), out num9);
                                                    }
                                                    if (num8 < 0 || 9 < num8 || num9 < 0 || 9 < num9 || num5 < 1 || 9 < num5 || num6 < 1 || 9 < num6 || num7 < 17 || 47 < num7)
                                                    {
                                                        strMessage = "指し手が正しくありません\n" + text2;
                                                        int result = 1;
                                                        return result;
                                                    }
                                                    kifu.Move[num3].fromsuji = num8;
                                                    kifu.Move[num3].fromdan = num9;
                                                    kifu.Move[num3].tosuji = num5;
                                                    kifu.Move[num3].todan = num6;
                                                    kifu.Move[num3].koma = num7;
                                                    kifu.Move[num3].promote = promote;
                                                    if (text2.Contains("/") && text2.Contains(":") && text2.Contains("(") && text2.Contains(")"))
                                                    {
                                                        string text6 = text2.Substring(text2.LastIndexOf("(") + 1, text2.LastIndexOf(")") - text2.LastIndexOf("(") - 1).Trim();
                                                        string[] array5 = text6.Split(new char[]
														{
															':',
															'/'
														});
                                                        int num11;
                                                        int.TryParse(array5[0], out num11);
                                                        int num12;
                                                        int.TryParse(array5[1], out num12);
                                                        kifu.Move[num3].dtime = 60 * num11 + num12;
                                                    }
                                                    else
                                                    {
                                                        kifu.Move[num3].dtime = 0;
                                                    }
                                                    kifu.Move[num3].ttime = ((num3 < 2) ? 0 : kifu.Move[num3 - 2].ttime) + kifu.Move[num3].dtime;
                                                    Kifu.MakeMove(ban, dai, kifu.Move[num3]);
                                                    num3++;
                                                    num = num5;
                                                    num2 = num6;
                                                    num4++;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                kifu.numMove = num3;
            }
            catch (Exception ex)
            {
                strMessage = ex.Message;
                int result = 1;
                return result;
            }
            return 0;
        }
        public static int KI22Kifu(string strKI2, out kifu_t kifu, out string strMessage)
        {
            kifu = default(kifu_t);
            strMessage = string.Empty;
            try
            {
                int num = 0;
                int num2 = 0;
                int num3 = 0;
                bool flag = false;
                int[,] ban = new int[9, 9];
                int[,] dai = new int[2, 7];
                kifu.Move = new move_t[1000];
                Kifu.initBanDai(ban, dai);
                string[] array = strKI2.Split(new char[]
				{
					'\n',
					'\r'
				});
                string[] array2 = array;
                for (int i = 0; i < array2.Length; i++)
                {
                    string text = array2[i];
                    string text2 = text.Trim();
                    if (!(text2 == string.Empty) && !text2.StartsWith("&") && !text2.StartsWith("#"))
                    {
                        if (text2.StartsWith("変化"))
                        {
                            break;
                        }
                        if (text2.StartsWith("*"))
                        {
                            string text3;
                            if (Kifu.strContent(text2, "*棋戦詳細：", out text3))
                            {
                                kifu.strEvent2 = text3;
                            }
                            else
                            {
                                if (num3 == 0)
                                {
                                    if (Kifu.strContent(text2, "*", out text3))
                                    {
                                        kifu.strComment = kifu.strComment + ((!string.IsNullOrEmpty(kifu.strComment)) ? "" : "\n") + text3;
                                    }
                                }
                                else
                                {
                                    if (Kifu.strContent(text2, "*", out text3))
                                    {
                                        move_t[] expr_15D_cp_0 = kifu.Move;
                                        int expr_15D_cp_1 = num3 - 1;
                                        expr_15D_cp_0[expr_15D_cp_1].comment = expr_15D_cp_0[expr_15D_cp_1].comment + ((!string.IsNullOrEmpty(kifu.Move[num3 - 1].comment)) ? "" : "\n") + text3;
                                    }
                                }
                            }
                        }
                        else
                        {
                            if (!text2.StartsWith("▲") && !text2.StartsWith("△") && !text2.StartsWith("▽"))
                            {
                                if (text2.StartsWith("手合割") && (text2.Contains("落") || text2.Contains("その他")))
                                {
                                    int result = 2;
                                    return result;
                                }
                                if (text2.StartsWith("後手番"))
                                {
                                    int result = 2;
                                    return result;
                                }
                                string text3;
                                if (Kifu.strContent(text2, "開始日時：", out text3))
                                {
                                    kifu.strStartTime = text3;
                                }
                                else
                                {
                                    if (Kifu.strContent(text2, "終了日時：", out text3))
                                    {
                                        kifu.strEndTime = text3;
                                    }
                                    else
                                    {
                                        if (Kifu.strContent(text2, "持ち時間：", out text3))
                                        {
                                            kifu.strTimeLimit = text3;
                                        }
                                        else
                                        {
                                            if (Kifu.strContent(text2, "消費時間：", out text3))
                                            {
                                                kifu.strTimeUsed = text3;
                                            }
                                            else
                                            {
                                                if (Kifu.strContent(text2, "先手：", out text3))
                                                {
                                                    kifu.strNameS = text3;
                                                }
                                                else
                                                {
                                                    if (Kifu.strContent(text2, "後手：", out text3))
                                                    {
                                                        kifu.strNameG = text3;
                                                    }
                                                    else
                                                    {
                                                        if (Kifu.strContent(text2, "棋戦：", out text3))
                                                        {
                                                            kifu.strEvent = text3;
                                                        }
                                                        else
                                                        {
                                                            if (Kifu.strContent(text2, "場所：", out text3))
                                                            {
                                                                kifu.strSite = text3;
                                                            }
                                                            else
                                                            {
                                                                if (Kifu.strContent(text2, "戦型：", out text3))
                                                                {
                                                                    kifu.strOpening = text3;
                                                                }
                                                                else
                                                                {
                                                                    if (Kifu.strContent(text2, "備考：", out text3))
                                                                    {
                                                                        kifu.strNote = text3;
                                                                    }
                                                                    else
                                                                    {
                                                                        if (flag)
                                                                        {
                                                                            if (text2.Contains("先手の勝ち"))
                                                                            {
                                                                                kifu.intResult = 1;
                                                                            }
                                                                            else
                                                                            {
                                                                                if (text2.Contains("後手の勝ち"))
                                                                                {
                                                                                    kifu.intResult = 2;
                                                                                }
                                                                                else
                                                                                {
                                                                                    if (text2.Contains("千日手"))
                                                                                    {
                                                                                        kifu.intResult = 3;
                                                                                    }
                                                                                    else
                                                                                    {
                                                                                        if (text2.Contains("持将棋"))
                                                                                        {
                                                                                            kifu.intResult = 4;
                                                                                        }
                                                                                        else
                                                                                        {
                                                                                            if (text2.Contains("中断"))
                                                                                            {
                                                                                                kifu.intResult = 0;
                                                                                            }
                                                                                        }
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            else
                            {
                                flag = true;
                                text2 = text2.Replace("▽", "△");
                                text2 = text2.Replace("\t", "");
                                text2 = text2.Replace(" ", "");
                                text2 = text2.Replace("\u3000", "");
                                text2 = text2.Replace("同", "同\u3000");
                                string[] array3 = text2.Split(new char[]
								{
									'▲',
									'△'
								});
                                string[] array4 = array3;
                                for (int j = 0; j < array4.Length; j++)
                                {
                                    string text4 = array4[j];
                                    string text5 = text4.Trim();
                                    if (!(text5 == string.Empty))
                                    {
                                        if (text2.Contains("先手の勝ち"))
                                        {
                                            kifu.intResult = 1;
                                            break;
                                        }
                                        if (text2.Contains("後手の勝ち"))
                                        {
                                            kifu.intResult = 2;
                                            break;
                                        }
                                        if (text5.Contains("千日手"))
                                        {
                                            kifu.intResult = 3;
                                            break;
                                        }
                                        if (text5.Contains("持将棋"))
                                        {
                                            kifu.intResult = 4;
                                            break;
                                        }
                                        if (text5.Contains("中断"))
                                        {
                                            kifu.intResult = 0;
                                            break;
                                        }
                                        text5 = Kifu.makeKIFMove(text5);
                                        if (text5.Length >= 3)
                                        {
                                            int num4;
                                            int num5;
                                            if (!text5.Contains("同") && text5.Length > 1)
                                            {
                                                num4 = Kifu.Jp2Num(text5.Substring(0, 1), false);
                                                num5 = Kifu.Jp2Num(text5.Substring(1, 1), true);
                                            }
                                            else
                                            {
                                                num4 = num;
                                                num5 = num2;
                                            }
                                            if (text5.Length <= 2)
                                            {
                                                strMessage = "指し手が正しくありません\n" + text2;
                                                int result = 1;
                                                return result;
                                            }
                                            int num6 = Kifu.KIFKoma2Num(text5.Substring(2, 1), true);
                                            if (num6 > 0)
                                            {
                                                num6 += ((num3 % 2 == 0) ? 16 : 32);
                                            }
                                            int promote = text5.Contains("成") ? 1 : 0;
                                            int num8;
                                            int num7;
                                            if (text5.Contains("打"))
                                            {
                                                num7 = (num8 = 0);
                                            }
                                            else
                                            {
                                                string strdir = (text5.Length > 3) ? text5.Substring(3) : string.Empty;
                                                Kifu.KI2Move2From(num6, num4, num5, strdir, ban, num3 % 2 == 0, out num8, out num7);
                                            }
                                            if (num8 < 0 || 9 < num8 || num7 < 0 || 9 < num7 || num4 < 1 || 9 < num4 || num5 < 1 || 9 < num5 || num6 < 17 || 47 < num6)
                                            {
                                                strMessage = "指し手が正しくありません\n" + text2;
                                                int result = 1;
                                                return result;
                                            }
                                            kifu.Move[num3].fromsuji = num8;
                                            kifu.Move[num3].fromdan = num7;
                                            kifu.Move[num3].tosuji = num4;
                                            kifu.Move[num3].todan = num5;
                                            kifu.Move[num3].koma = num6;
                                            kifu.Move[num3].promote = promote;
                                            kifu.Move[num3].dtime = (kifu.Move[num3].dtime = 0);
                                            Kifu.MakeMove(ban, dai, kifu.Move[num3]);
                                            num3++;
                                            num = num4;
                                            num2 = num5;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                kifu.numMove = num3;
            }
            catch (Exception ex)
            {
                strMessage = ex.Message;
                int result = 1;
                return result;
            }
            return 0;
        }
        public static int CSA2Kifu(string strCSA, out kifu_t kifu, out string strMessage)
        {
            kifu = default(kifu_t);
            strMessage = string.Empty;
            try
            {
                int num = 0;
                int[,] array = new int[9, 9];
                int[,] dai = new int[2, 7];
                kifu.Move = new move_t[1000];
                Kifu.initBanDai(array, dai);
                string[] array2 = strCSA.Split(new char[]
				{
					'\n',
					'\r'
				});
                int i = 0;
                while (!array2[i].StartsWith("+") && !array2[i].StartsWith("-") && i < array2.Length - 1)
                {
                    string text = array2[i++].Trim();
                    if (!(text == string.Empty) && !text.StartsWith("V"))
                    {
                        string text2;
                        if (Kifu.strContent(text, "N+", out text2))
                        {
                            kifu.strNameS = text2;
                        }
                        else
                        {
                            if (Kifu.strContent(text, "N-", out text2))
                            {
                                kifu.strNameG = text2;
                            }
                            else
                            {
                                if (Kifu.strContent(text, "$START_TIME:", out text2))
                                {
                                    kifu.strStartTime = text2;
                                }
                                else
                                {
                                    if (Kifu.strContent(text, "$END_TIME:", out text2))
                                    {
                                        kifu.strEndTime = text2;
                                    }
                                    else
                                    {
                                        if (Kifu.strContent(text, "$EVENT:", out text2))
                                        {
                                            kifu.strEvent = text2;
                                        }
                                        else
                                        {
                                            if (Kifu.strContent(text, "$EVENT2:", out text2))
                                            {
                                                kifu.strEvent2 = text2;
                                            }
                                            else
                                            {
                                                if (Kifu.strContent(text, "$SITE:", out text2))
                                                {
                                                    kifu.strSite = text2;
                                                }
                                                else
                                                {
                                                    if (Kifu.strContent(text, "$TIME_LIMIT:", out text2))
                                                    {
                                                        kifu.strTimeLimit = text2;
                                                    }
                                                    else
                                                    {
                                                        if (Kifu.strContent(text, "$OPENING:", out text2))
                                                        {
                                                            kifu.strOpening = text2;
                                                        }
                                                        else
                                                        {
                                                            if (Kifu.strContent(text, "$", out text2))
                                                            {
                                                                kifu.strNote = text2;
                                                            }
                                                            else
                                                            {
                                                                text.StartsWith("P");
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                if (array2[i++] == "-")
                {
                    strMessage = "駒落ち棋譜には対応していません";
                    int result = 2;
                    return result;
                }
                while (i < array2.Length)
                {
                    string text3 = array2[i++].Trim();
                    if (!(text3 == string.Empty))
                    {
                        if (text3.StartsWith("'"))
                        {
                            if (num == 0)
                            {
                                string text2;
                                if (Kifu.strContent(text3, "'", out text2))
                                {
                                    kifu.strComment = kifu.strComment + ((!string.IsNullOrEmpty(kifu.strComment)) ? "" : "\n") + text2;
                                }
                            }
                            else
                            {
                                string text2;
                                if (Kifu.strContent(text3, "'", out text2))
                                {
                                    move_t[] expr_299_cp_0 = kifu.Move;
                                    int expr_299_cp_1 = num - 1;
                                    expr_299_cp_0[expr_299_cp_1].comment = expr_299_cp_0[expr_299_cp_1].comment + ((!string.IsNullOrEmpty(kifu.Move[num - 1].comment)) ? "" : "\n") + text2;
                                }
                            }
                        }
                        else
                        {
                            if (text3.StartsWith("%TORYO"))
                            {
                                kifu.intResult = ((num % 2 == 1) ? 1 : 2);
                                break;
                            }
                            if (text3.StartsWith("%SENNICHITE"))
                            {
                                kifu.intResult = 3;
                                break;
                            }
                            if (text3.StartsWith("%JISHOGI"))
                            {
                                kifu.intResult = 4;
                                break;
                            }
                            if (text3.StartsWith("%"))
                            {
                                kifu.intResult = 0;
                                break;
                            }
                            string[] array3 = text3.Split(new char[]
							{
								','
							});
                            for (int j = 0; j < array3.Length; j++)
                            {
                                string text4 = array3[j].Trim();
                                if (!(text4 == string.Empty))
                                {
                                    if (text4.Length == 7 && (text4.StartsWith("+") || text4.StartsWith("-")))
                                    {
                                        if (text4.Substring(0, 1) == "+" == (num % 2 == 1))
                                        {
                                            strMessage = "手番(+/-)が不正です\n" + text3;
                                            int result = 1;
                                            return result;
                                        }
                                        int num2;
                                        int.TryParse(text4.Substring(1, 1), out num2);
                                        int num3;
                                        int.TryParse(text4.Substring(2, 1), out num3);
                                        int num4;
                                        int.TryParse(text4.Substring(3, 1), out num4);
                                        int num5;
                                        int.TryParse(text4.Substring(4, 1), out num5);
                                        int num6 = Kifu.CSAKoma2Num(text4.Substring(5, 2));
                                        num6 += (text4.StartsWith("+") ? 16 : 32);
                                        int num7 = (num2 > 0 && num3 > 0 && array[num2 - 1, num3 - 1] != num6) ? 1 : 0;
                                        if (num7 == 1)
                                        {
                                            num6 -= 8;
                                        }
                                        if (num2 < 0 || 9 < num2 || num3 < 0 || 9 < num3 || num4 < 1 || 9 < num4 || num5 < 1 || 9 < num5 || num6 < 17 || 47 < num6)
                                        {
                                            strMessage = "指し手が正しくありません\n" + text3;
                                            int result = 1;
                                            return result;
                                        }
                                        kifu.Move[num].fromsuji = num2;
                                        kifu.Move[num].fromdan = num3;
                                        kifu.Move[num].tosuji = num4;
                                        kifu.Move[num].todan = num5;
                                        kifu.Move[num].koma = num6;
                                        kifu.Move[num].promote = num7;
                                        Kifu.MakeMove(array, dai, kifu.Move[num]);
                                        num++;
                                    }
                                    else
                                    {
                                        string text2;
                                        if (text4.StartsWith("T") && num > 0 && Kifu.strContent(text4, "T", out text2))
                                        {
                                            int num8;
                                            int.TryParse(text2, out num8);
                                            kifu.Move[num - 1].dtime = num8;
                                            kifu.Move[num - 1].ttime = ((num < 3) ? 0 : kifu.Move[num - 3].ttime) + num8;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                kifu.numMove = num;
            }
            catch (Exception ex)
            {
                strMessage = ex.Message;
                int result = 1;
                return result;
            }
            return 0;
        }
        public static string Kifu2KIF(kifu_t kifu)
        {
            string str = string.Empty;
            if (!string.IsNullOrEmpty(kifu.strStartTime))
            {
                str = str + "開始日時：" + kifu.strStartTime + Environment.NewLine;
            }
            if (!string.IsNullOrEmpty(kifu.strEndTime))
            {
                str = str + "終了日時：" + kifu.strEndTime + Environment.NewLine;
            }
            if (!string.IsNullOrEmpty(kifu.strEvent))
            {
                str = str + "棋戦：" + kifu.strEvent + Environment.NewLine;
            }
            if (!string.IsNullOrEmpty(kifu.strTimeLimit))
            {
                str = str + "持ち時間：" + kifu.strTimeLimit + Environment.NewLine;
            }
            if (!string.IsNullOrEmpty(kifu.strTimeUsed))
            {
                str = str + "消費時間：" + kifu.strTimeUsed + Environment.NewLine;
            }
            if (!string.IsNullOrEmpty(kifu.strSite))
            {
                str = str + "場所：" + kifu.strSite + Environment.NewLine;
            }
            if (!string.IsNullOrEmpty(kifu.strNameS))
            {
                str = str + "先手：" + kifu.strNameS + Environment.NewLine;
            }
            if (!string.IsNullOrEmpty(kifu.strNameG))
            {
                str = str + "後手：" + kifu.strNameG + Environment.NewLine;
            }
            str = str + "手合割：平手" + Environment.NewLine;
            if (!string.IsNullOrEmpty(kifu.strOpening))
            {
                str = str + "戦型：" + kifu.strOpening + Environment.NewLine;
            }
            if (!string.IsNullOrEmpty(kifu.strNote))
            {
                str = str + "備考：" + kifu.strNote + Environment.NewLine;
            }
            str += Environment.NewLine;
            str = str + "手数----指手---------消費時間--" + Environment.NewLine;
            if (!string.IsNullOrEmpty(kifu.strEvent2))
            {
                str = str + "*棋戦詳細：" + kifu.strEvent2 + Environment.NewLine;
            }
            if (!string.IsNullOrEmpty(kifu.strComment))
            {
                string[] array = kifu.strComment.Split(new char[]
				{
					'\n'
				});
                string[] array2 = array;
                for (int i = 0; i < array2.Length; i++)
                {
                    string text = array2[i];
                    if (text != string.Empty)
                    {
                        str = str + "*" + text + Environment.NewLine;
                    }
                }
            }
            for (int j = 0; j < kifu.numMove; j++)
            {
                string text2 = string.Empty;
                text2 = text2 + (j + 1).ToString().PadLeft(3) + " ";
                move_t move_t = kifu.Move[j];
                text2 = text2 + Kifu.Num2Jp(move_t.tosuji, false) + Kifu.Num2Jp(move_t.todan, true);
                text2 += Kifu.Num2KIFKoma(move_t.koma, false);
                if (move_t.promote == 1)
                {
                    text2 += "成";
                }
                if (move_t.fromsuji > 0 && move_t.fromdan > 0)
                {
                    string text3 = text2;
                    text2 = string.Concat(new string[]
					{
						text3,
						"(",
						move_t.fromsuji.ToString(),
						move_t.fromdan.ToString(),
						")"
					});
                }
                else
                {
                    text2 += "打";
                }
                if (move_t.dtime > 0)
                {
                    text2 += (text2.Contains("打") ? "  " : "");
                    text2 += (text2.Contains("成") ? "" : "  ");
                    text2 += "  ";
                    int dtime = move_t.dtime;
                    int ttime = move_t.ttime;
                    string format = "({0:00}:{1:00}/{2:00}:{3:00}:{4:00})";
                    text2 += string.Format(format, new object[]
					{
						dtime / 60,
						dtime % 60,
						ttime / 3600,
						ttime % 3600 / 60,
						ttime % 60
					});
                }
                str = str + text2 + Environment.NewLine;
                if (!string.IsNullOrEmpty(move_t.comment))
                {
                    string[] array3 = move_t.comment.Split(new char[]
					{
						'\n'
					});
                    string[] array4 = array3;
                    for (int k = 0; k < array4.Length; k++)
                    {
                        string text4 = array4[k];
                        if (text4 != string.Empty)
                        {
                            str = str + "*" + text4 + Environment.NewLine;
                        }
                    }
                }
            }
            str = str + (kifu.numMove + 1).ToString().PadLeft(3) + " ";
            if (kifu.intResult == 1 || kifu.intResult == 2)
            {
                str += "投了";
            }
            else
            {
                if (kifu.intResult == 3)
                {
                    str += "千日手";
                }
                else
                {
                    if (kifu.intResult == 4)
                    {
                        str += "持将棋";
                    }
                    else
                    {
                        if (kifu.intResult == 0)
                        {
                            str += "中断";
                        }
                    }
                }
            }
            return str + Environment.NewLine;
        }
        public static string Kifu2KI2(kifu_t kifu)
        {
            string text = string.Empty;
            if (!string.IsNullOrEmpty(kifu.strStartTime))
            {
                text = text + "開始日時：" + kifu.strStartTime + Environment.NewLine;
            }
            if (!string.IsNullOrEmpty(kifu.strEndTime))
            {
                text = text + "終了日時：" + kifu.strEndTime + Environment.NewLine;
            }
            if (!string.IsNullOrEmpty(kifu.strEvent))
            {
                text = text + "棋戦：" + kifu.strEvent + Environment.NewLine;
            }
            if (!string.IsNullOrEmpty(kifu.strTimeLimit))
            {
                text = text + "持ち時間：" + kifu.strTimeLimit + Environment.NewLine;
            }
            if (!string.IsNullOrEmpty(kifu.strTimeUsed))
            {
                text = text + "消費時間：" + kifu.strTimeUsed + Environment.NewLine;
            }
            if (!string.IsNullOrEmpty(kifu.strSite))
            {
                text = text + "場所：" + kifu.strSite + Environment.NewLine;
            }
            if (!string.IsNullOrEmpty(kifu.strNameS))
            {
                text = text + "先手：" + kifu.strNameS + Environment.NewLine;
            }
            if (!string.IsNullOrEmpty(kifu.strNameG))
            {
                text = text + "後手：" + kifu.strNameG + Environment.NewLine;
            }
            text = text + "手合割：平手" + Environment.NewLine;
            if (!string.IsNullOrEmpty(kifu.strOpening))
            {
                text = text + "戦型：" + kifu.strOpening + Environment.NewLine;
            }
            if (!string.IsNullOrEmpty(kifu.strNote))
            {
                text = text + "備考：" + kifu.strNote + Environment.NewLine;
            }
            if (!string.IsNullOrEmpty(kifu.strEvent2))
            {
                text = text + "*棋戦詳細：" + kifu.strEvent2 + Environment.NewLine;
            }
            int[,] ban = new int[9, 9];
            int[,] dai = new int[2, 7];
            Kifu.initBanDai(ban, dai);
            text += Environment.NewLine;
            for (int i = 0; i < kifu.numMove; i++)
            {
                string text2 = string.Empty;
                move_t move = kifu.Move[i];
                text2 += ((i % 2 == 0) ? "▲" : "△");
                if (i > 0 && move.tosuji == kifu.Move[i - 1].tosuji && move.todan == kifu.Move[i - 1].todan)
                {
                    text2 += "同\u3000";
                }
                else
                {
                    text2 = text2 + Kifu.Num2Jp(move.tosuji, false) + Kifu.Num2Jp(move.todan, true);
                }
                text2 += Kifu.Num2KIFKoma(move.koma, false);
                text2 += Kifu.KI2MoveDir(move, ban);
                if (move.promote == 1)
                {
                    text2 += "成";
                }
                Kifu.MakeMove(ban, dai, move);
                text += text2;
                if (i < kifu.numMove - 1)
                {
                    if ((i + 1) % 8 == 0)
                    {
                        text += Environment.NewLine;
                    }
                    else
                    {
                        text += " ";
                    }
                }
            }
            text += Environment.NewLine;
            if (kifu.intResult == 1)
            {
                string text3 = text;
                text = string.Concat(new string[]
				{
					text3,
					"まで",
					kifu.numMove.ToString(),
					"手で先手の勝ち",
					Environment.NewLine
				});
            }
            else
            {
                if (kifu.intResult == 2)
                {
                    string text4 = text;
                    text = string.Concat(new string[]
					{
						text4,
						"まで",
						kifu.numMove.ToString(),
						"手で後手の勝ち",
						Environment.NewLine
					});
                }
                else
                {
                    if (kifu.intResult == 3)
                    {
                        string text5 = text;
                        text = string.Concat(new string[]
						{
							text5,
							"まで",
							kifu.numMove.ToString(),
							"手で千日手",
							Environment.NewLine
						});
                    }
                    else
                    {
                        if (kifu.intResult == 4)
                        {
                            string text6 = text;
                            text = string.Concat(new string[]
							{
								text6,
								"まで",
								kifu.numMove.ToString(),
								"手で持将棋",
								Environment.NewLine
							});
                        }
                    }
                }
            }
            return text;
        }
        public static string Kifu2CSA(kifu_t kifu)
        {
            string text = string.Empty;
            if (!string.IsNullOrEmpty(kifu.strNameS))
            {
                text = text + "N+" + kifu.strNameS + Environment.NewLine;
            }
            if (!string.IsNullOrEmpty(kifu.strNameG))
            {
                text = text + "N-" + kifu.strNameG + Environment.NewLine;
            }
            if (!string.IsNullOrEmpty(kifu.strStartTime))
            {
                text = text + "$START_TIME:" + kifu.strStartTime + Environment.NewLine;
            }
            if (!string.IsNullOrEmpty(kifu.strEndTime))
            {
                text = text + "$END_TIME:" + kifu.strEndTime + Environment.NewLine;
            }
            if (!string.IsNullOrEmpty(kifu.strEvent))
            {
                text = text + "$EVENT:" + kifu.strEvent + Environment.NewLine;
            }
            if (!string.IsNullOrEmpty(kifu.strTimeLimit))
            {
                text = text + "$TIME_LIMIT:" + kifu.strTimeLimit + Environment.NewLine;
            }
            if (!string.IsNullOrEmpty(kifu.strSite))
            {
                text = text + "$SITE:" + kifu.strSite + Environment.NewLine;
            }
            if (!string.IsNullOrEmpty(kifu.strOpening))
            {
                text = text + "$OPENING:" + kifu.strOpening + Environment.NewLine;
            }
            if (!string.IsNullOrEmpty(kifu.strTimeUsed))
            {
                text = text + "$" + kifu.strTimeUsed + Environment.NewLine;
            }
            if (!string.IsNullOrEmpty(kifu.strEvent2))
            {
                text = text + "$" + kifu.strEvent2 + Environment.NewLine;
            }
            if (!string.IsNullOrEmpty(kifu.strNote))
            {
                text = text + "$" + kifu.strNote + Environment.NewLine;
            }
            text = text + "PI" + Environment.NewLine;
            text = text + "+" + Environment.NewLine;
            if (!string.IsNullOrEmpty(kifu.strComment))
            {
                string[] array = kifu.strComment.Split(new char[]
				{
					'\n'
				});
                string[] array2 = array;
                for (int i = 0; i < array2.Length; i++)
                {
                    string text2 = array2[i];
                    if (text2 != string.Empty)
                    {
                        text = text + "'" + text2 + Environment.NewLine;
                    }
                }
            }
            for (int j = 0; j < kifu.numMove; j++)
            {
                move_t move_t = kifu.Move[j];
                text = text + ((j % 2 == 0) ? "+" : "-") + string.Format("{0}{1}{2}{3}{4}", new object[]
				{
					move_t.fromsuji,
					move_t.fromdan,
					move_t.tosuji,
					move_t.todan,
					Kifu.Num2CSAKoma(move_t.koma + ((move_t.promote == 1) ? 8 : 0))
				}) + Environment.NewLine;
                if (move_t.dtime > 0)
                {
                    text = text + "T" + move_t.dtime.ToString() + Environment.NewLine;
                }
                if (!string.IsNullOrEmpty(move_t.comment))
                {
                    string[] array3 = move_t.comment.Split(new char[]
					{
						'\n'
					});
                    string[] array4 = array3;
                    for (int k = 0; k < array4.Length; k++)
                    {
                        string text3 = array4[k];
                        if (text3 != string.Empty)
                        {
                            text = text + "'" + text3 + Environment.NewLine;
                        }
                    }
                }
            }
            if (kifu.intResult == 1 || kifu.intResult == 2)
            {
                text = text + "%TORYO" + Environment.NewLine;
            }
            else
            {
                if (kifu.intResult == 3)
                {
                    text = text + "%SENNICHITE" + Environment.NewLine;
                }
                else
                {
                    if (kifu.intResult == 4)
                    {
                        text = text + "%JISHOGI" + Environment.NewLine;
                    }
                }
            }
            return text;
        }
        private static bool strContent(string strin, string key, out string strout)
        {
            if (strin.StartsWith(key) && strin.Length > key.Length)
            {
                strout = strin.Substring(key.Length);
                return true;
            }
            strout = string.Empty;
            return false;
        }
        public static string Num2Jp(int i, bool cn)
        {
            if (1 > i || i >= Kifu.strNumJP.Length)
            {
                return string.Empty;
            }
            if (!cn)
            {
                return Kifu.strNumJP[i];
            }
            return Kifu.strNumCN[i];
        }
        private static int Jp2Num(string str, bool cn)
        {
            for (int i = 1; i < Kifu.strNumJP.Length; i++)
            {
                if (str == (cn ? Kifu.strNumCN[i] : Kifu.strNumJP[i]))
                {
                    return i;
                }
                if (str == Kifu.strNumEN[i])
                {
                    return i;
                }
            }
            return 0;
        }
        private static int KIFKoma2Num(string str, bool single)
        {
            for (int i = 1; i < Kifu.strKIFKoma1.Length; i++)
            {
                if (str == (single ? Kifu.strKIFKoma1[i] : Kifu.strKIFKoma2[i]))
                {
                    return i;
                }
            }
            return 0;
        }
        public static string Num2KIFKoma(int koma, bool single)
        {
            if (!single)
            {
                return Kifu.strKIFKoma2[koma & 15];
            }
            return Kifu.strKIFKoma1[koma & 15];
        }
        private static int CSAKoma2Num(string str)
        {
            for (int i = 0; i < Kifu.strCSAKoma.Length; i++)
            {
                if (str == Kifu.strCSAKoma[i])
                {
                    return i;
                }
            }
            return 0;
        }
        private static string Num2CSAKoma(int i)
        {
            return Kifu.strCSAKoma[i & 15];
        }
        private static bool MakeMove(int[,] ban, int[,] dai, move_t move)
        {
            bool result;
            try
            {
                int num = ((move.koma & 48) == 16) ? 0 : 1;
                if (move.fromsuji > 0 && move.fromdan > 0)
                {
                    if (ban[move.tosuji - 1, move.todan - 1] > 0)
                    {
                        int num2 = ban[move.tosuji - 1, move.todan - 1];
                        if (num2 != 24 && num2 != 40)
                        {
                            dai[num, num2 - 1 & 7]++;
                        }
                    }
                    ban[move.fromsuji - 1, move.fromdan - 1] = 0;
                    ban[move.tosuji - 1, move.todan - 1] = ((move.promote == 0) ? move.koma : (move.koma | 8));
                }
                else
                {
                    ban[move.tosuji - 1, move.todan - 1] = move.koma;
                    dai[num, (move.koma & 15) - 1]--;
                }
                result = true;
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                result = false;
            }
            return result;
        }
        private static string makeKIFMove(string str)
        {
            string text = str.Replace("成香", "杏");
            text = text.Replace("成桂", "圭");
            text = text.Replace("成銀", "全");
            text = text.Replace("王", "玉");
            text = text.Replace("竜", "龍");
            text = text.Replace("不成", "");
            text = text.Replace("行", "上");
            text = text.Replace("下", "引");
            if (text.IndexOf("同\u3000") < 0 && text.IndexOf("同") >= 0)
            {
                text = text.Replace("同", "同\u3000");
            }
            return text;
        }
        private static void KI2Move2From(int koma, int tosuji, int todan, string strdir, int[,] ban, bool sente, out int fromsuji, out int fromdan)
        {
            fromsuji = (fromdan = 0);
            int[] array = new int[4];
            int[] array2 = new int[4];
            int fromList = Kifu.getFromList(koma, tosuji, todan, ban, array, array2);
            if (fromList == 0)
            {
                fromsuji = (fromdan = 0);
                return;
            }
            if (fromList == 1)
            {
                fromsuji = array[0];
                fromdan = array2[0];
                return;
            }
            int fromID = Kifu.getFromID(strdir, sente, fromList, array, array2, tosuji, todan);
            fromsuji = array[fromID];
            fromdan = array2[fromID];
        }
        private static string KI2MoveDir(move_t move, int[,] ban)
        {
            string result = string.Empty;
            int[] array = new int[4];
            int[] array2 = new int[4];
            int fromList = Kifu.getFromList(move.koma, move.tosuji, move.todan, ban, array, array2);
            if (fromList > 0 && move.fromsuji == 0 && move.fromdan == 0)
            {
                result = "打";
            }
            else
            {
                if (fromList > 1)
                {
                    bool flag = move.koma < 32;
                    int[] array3 = new int[6];
                    for (int i = 0; i < fromList; i++)
                    {
                        if (array2[i] < move.todan)
                        {
                            array3[flag ? 0 : 1]++;
                        }
                        if (array2[i] > move.todan)
                        {
                            array3[flag ? 1 : 0]++;
                        }
                        if (array2[i] == move.todan)
                        {
                            array3[2]++;
                        }
                        if (array[i] < move.tosuji)
                        {
                            array3[flag ? 3 : 4]++;
                        }
                        if (array[i] > move.tosuji)
                        {
                            array3[flag ? 4 : 3]++;
                        }
                        if (array[i] == move.tosuji)
                        {
                            array3[5]++;
                        }
                    }
                    int ix = 0;
                    int iy = 0;
                    if (move.fromdan < move.todan)
                    {
                        iy = (flag ? 0 : 1);
                    }
                    if (move.fromdan > move.todan)
                    {
                        iy = (flag ? 1 : 0);
                    }
                    if (move.fromdan == move.todan)
                    {
                        iy = 2;
                    }
                    if (move.fromsuji < move.tosuji)
                    {
                        ix = (flag ? 3 : 4);
                    }
                    if (move.fromsuji > move.tosuji)
                    {
                        ix = (flag ? 4 : 3);
                    }
                    if (move.fromsuji == move.tosuji)
                    {
                        ix = 5;
                    }
                    result = Kifu.strDet(array3, ix, iy);
                }
            }
            return result;
        }
        private static string strDet(int[] snum, int ix, int iy)
        {
            string[] array = new string[]
			{
				"引",
				"上",
				"寄",
				"右",
				"左",
				"直"
			};
            int num = array.Length;
            if (ix < 0 || num <= ix || iy < 0 || num <= iy || snum.Length != num)
            {
                return string.Empty;
            }
            int num2 = 0;
            for (int i = 0; i < num; i++)
            {
                num2 += snum[i];
            }
            if (num2 < 4)
            {
                return string.Empty;
            }
            for (int j = 0; j < num; j++)
            {
                if ((j == ix || j == iy) && snum[j] == 1)
                {
                    return array[j];
                }
            }
            return array[ix] + array[iy];
        }
        private static int getFromList(int koma, int tosuji, int todan, int[,] ban, int[] fromsuji, int[] fromdan)
        {
            int num = 0;
            if (koma == 17)
            {
                if (Kifu.BAN(ban, tosuji, todan + 1) == koma)
                {
                    fromsuji[num] = tosuji;
                    fromdan[num] = todan + 1;
                    num++;
                }
            }
            else
            {
                if (koma == 33)
                {
                    if (Kifu.BAN(ban, tosuji, todan - 1) == koma)
                    {
                        fromsuji[num] = tosuji;
                        fromdan[num] = todan - 1;
                        num++;
                    }
                }
                else
                {
                    if (koma == 18)
                    {
                        int i = todan + 1;
                        while (i <= 9)
                        {
                            if (Kifu.BAN(ban, tosuji, i) != 0)
                            {
                                if (Kifu.BAN(ban, tosuji, i) == koma)
                                {
                                    fromsuji[num] = tosuji;
                                    fromdan[num] = i;
                                    num++;
                                    break;
                                }
                                break;
                            }
                            else
                            {
                                i++;
                            }
                        }
                    }
                    else
                    {
                        if (koma == 34)
                        {
                            int j = todan - 1;
                            while (j >= 1)
                            {
                                if (Kifu.BAN(ban, tosuji, j) != 0)
                                {
                                    if (Kifu.BAN(ban, tosuji, j) == koma)
                                    {
                                        fromsuji[num] = tosuji;
                                        fromdan[num] = j;
                                        num++;
                                        break;
                                    }
                                    break;
                                }
                                else
                                {
                                    j--;
                                }
                            }
                        }
                        else
                        {
                            if (koma == 19)
                            {
                                if (Kifu.BAN(ban, tosuji + 1, todan + 2) == koma)
                                {
                                    fromsuji[num] = tosuji + 1;
                                    fromdan[num] = todan + 2;
                                    num++;
                                }
                                if (Kifu.BAN(ban, tosuji - 1, todan + 2) == koma)
                                {
                                    fromsuji[num] = tosuji - 1;
                                    fromdan[num] = todan + 2;
                                    num++;
                                }
                            }
                            else
                            {
                                if (koma == 35)
                                {
                                    if (Kifu.BAN(ban, tosuji + 1, todan - 2) == koma)
                                    {
                                        fromsuji[num] = tosuji + 1;
                                        fromdan[num] = todan - 2;
                                        num++;
                                    }
                                    if (Kifu.BAN(ban, tosuji - 1, todan - 2) == koma)
                                    {
                                        fromsuji[num] = tosuji - 1;
                                        fromdan[num] = todan - 2;
                                        num++;
                                    }
                                }
                                else
                                {
                                    if (koma == 20)
                                    {
                                        if (Kifu.BAN(ban, tosuji + 1, todan + 1) == koma)
                                        {
                                            fromsuji[num] = tosuji + 1;
                                            fromdan[num] = todan + 1;
                                            num++;
                                        }
                                        if (Kifu.BAN(ban, tosuji, todan + 1) == koma)
                                        {
                                            fromsuji[num] = tosuji;
                                            fromdan[num] = todan + 1;
                                            num++;
                                        }
                                        if (Kifu.BAN(ban, tosuji - 1, todan + 1) == koma)
                                        {
                                            fromsuji[num] = tosuji - 1;
                                            fromdan[num] = todan + 1;
                                            num++;
                                        }
                                        if (Kifu.BAN(ban, tosuji + 1, todan - 1) == koma)
                                        {
                                            fromsuji[num] = tosuji + 1;
                                            fromdan[num] = todan - 1;
                                            num++;
                                        }
                                        if (Kifu.BAN(ban, tosuji - 1, todan - 1) == koma)
                                        {
                                            fromsuji[num] = tosuji - 1;
                                            fromdan[num] = todan - 1;
                                            num++;
                                        }
                                    }
                                    else
                                    {
                                        if (koma == 36)
                                        {
                                            if (Kifu.BAN(ban, tosuji + 1, todan - 1) == koma)
                                            {
                                                fromsuji[num] = tosuji + 1;
                                                fromdan[num] = todan - 1;
                                                num++;
                                            }
                                            if (Kifu.BAN(ban, tosuji, todan - 1) == koma)
                                            {
                                                fromsuji[num] = tosuji;
                                                fromdan[num] = todan - 1;
                                                num++;
                                            }
                                            if (Kifu.BAN(ban, tosuji - 1, todan - 1) == koma)
                                            {
                                                fromsuji[num] = tosuji - 1;
                                                fromdan[num] = todan - 1;
                                                num++;
                                            }
                                            if (Kifu.BAN(ban, tosuji + 1, todan + 1) == koma)
                                            {
                                                fromsuji[num] = tosuji + 1;
                                                fromdan[num] = todan + 1;
                                                num++;
                                            }
                                            if (Kifu.BAN(ban, tosuji - 1, todan + 1) == koma)
                                            {
                                                fromsuji[num] = tosuji - 1;
                                                fromdan[num] = todan + 1;
                                                num++;
                                            }
                                        }
                                        else
                                        {
                                            if (koma == 21 || koma == 25 || koma == 26 || koma == 27 || koma == 28)
                                            {
                                                if (Kifu.BAN(ban, tosuji + 1, todan + 1) == koma)
                                                {
                                                    fromsuji[num] = tosuji + 1;
                                                    fromdan[num] = todan + 1;
                                                    num++;
                                                }
                                                if (Kifu.BAN(ban, tosuji, todan + 1) == koma)
                                                {
                                                    fromsuji[num] = tosuji;
                                                    fromdan[num] = todan + 1;
                                                    num++;
                                                }
                                                if (Kifu.BAN(ban, tosuji - 1, todan + 1) == koma)
                                                {
                                                    fromsuji[num] = tosuji - 1;
                                                    fromdan[num] = todan + 1;
                                                    num++;
                                                }
                                                if (Kifu.BAN(ban, tosuji + 1, todan) == koma)
                                                {
                                                    fromsuji[num] = tosuji + 1;
                                                    fromdan[num] = todan;
                                                    num++;
                                                }
                                                if (Kifu.BAN(ban, tosuji - 1, todan) == koma)
                                                {
                                                    fromsuji[num] = tosuji - 1;
                                                    fromdan[num] = todan;
                                                    num++;
                                                }
                                                if (Kifu.BAN(ban, tosuji, todan - 1) == koma)
                                                {
                                                    fromsuji[num] = tosuji;
                                                    fromdan[num] = todan - 1;
                                                    num++;
                                                }
                                            }
                                            else
                                            {
                                                if (koma == 37 || koma == 41 || koma == 42 || koma == 43 || koma == 44)
                                                {
                                                    if (Kifu.BAN(ban, tosuji + 1, todan - 1) == koma)
                                                    {
                                                        fromsuji[num] = tosuji + 1;
                                                        fromdan[num] = todan - 1;
                                                        num++;
                                                    }
                                                    if (Kifu.BAN(ban, tosuji, todan - 1) == koma)
                                                    {
                                                        fromsuji[num] = tosuji;
                                                        fromdan[num] = todan - 1;
                                                        num++;
                                                    }
                                                    if (Kifu.BAN(ban, tosuji - 1, todan - 1) == koma)
                                                    {
                                                        fromsuji[num] = tosuji - 1;
                                                        fromdan[num] = todan - 1;
                                                        num++;
                                                    }
                                                    if (Kifu.BAN(ban, tosuji + 1, todan) == koma)
                                                    {
                                                        fromsuji[num] = tosuji + 1;
                                                        fromdan[num] = todan;
                                                        num++;
                                                    }
                                                    if (Kifu.BAN(ban, tosuji - 1, todan) == koma)
                                                    {
                                                        fromsuji[num] = tosuji - 1;
                                                        fromdan[num] = todan;
                                                        num++;
                                                    }
                                                    if (Kifu.BAN(ban, tosuji, todan + 1) == koma)
                                                    {
                                                        fromsuji[num] = tosuji;
                                                        fromdan[num] = todan + 1;
                                                        num++;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if (koma == 22 || koma == 30 || koma == 38 || koma == 46)
            {
                int k = tosuji + 1;
                int num2 = todan + 1;
                while (k <= 9 && num2 <= 9)
                {
                    if (Kifu.BAN(ban, k, num2) != 0)
                    {
                        if (Kifu.BAN(ban, k, num2) == koma)
                        {
                            fromsuji[num] = k;
                            fromdan[num] = num2;
                            num++;
                            break;
                        }
                        break;
                    }
                    else
                    {
                        k++;
                        num2++;
                    }
                }
                k = tosuji + 1;
                num2 = todan - 1;
                while (k <= 9 && num2 >= 1)
                {
                    if (Kifu.BAN(ban, k, num2) != 0)
                    {
                        if (Kifu.BAN(ban, k, num2) == koma)
                        {
                            fromsuji[num] = k;
                            fromdan[num] = num2;
                            num++;
                            break;
                        }
                        break;
                    }
                    else
                    {
                        k++;
                        num2--;
                    }
                }
                k = tosuji - 1;
                num2 = todan + 1;
                while (k >= 1 && num2 <= 9)
                {
                    if (Kifu.BAN(ban, k, num2) != 0)
                    {
                        if (Kifu.BAN(ban, k, num2) == koma)
                        {
                            fromsuji[num] = k;
                            fromdan[num] = num2;
                            num++;
                            break;
                        }
                        break;
                    }
                    else
                    {
                        k--;
                        num2++;
                    }
                }
                k = tosuji - 1;
                num2 = todan - 1;
                while (k >= 1)
                {
                    if (num2 < 1)
                    {
                        break;
                    }
                    if (Kifu.BAN(ban, k, num2) != 0)
                    {
                        if (Kifu.BAN(ban, k, num2) == koma)
                        {
                            fromsuji[num] = k;
                            fromdan[num] = num2;
                            num++;
                            break;
                        }
                        break;
                    }
                    else
                    {
                        k--;
                        num2--;
                    }
                }
            }
            else
            {
                if (koma == 23 || koma == 31 || koma == 39 || koma == 47)
                {
                    int l = tosuji + 1;
                    while (l <= 9)
                    {
                        if (Kifu.BAN(ban, l, todan) != 0)
                        {
                            if (Kifu.BAN(ban, l, todan) == koma)
                            {
                                fromsuji[num] = l;
                                fromdan[num] = todan;
                                num++;
                                break;
                            }
                            break;
                        }
                        else
                        {
                            l++;
                        }
                    }
                    int m = tosuji - 1;
                    while (m >= 1)
                    {
                        if (Kifu.BAN(ban, m, todan) != 0)
                        {
                            if (Kifu.BAN(ban, m, todan) == koma)
                            {
                                fromsuji[num] = m;
                                fromdan[num] = todan;
                                num++;
                                break;
                            }
                            break;
                        }
                        else
                        {
                            m--;
                        }
                    }
                    int n = todan + 1;
                    while (n <= 9)
                    {
                        if (Kifu.BAN(ban, tosuji, n) != 0)
                        {
                            if (Kifu.BAN(ban, tosuji, n) == koma)
                            {
                                fromsuji[num] = tosuji;
                                fromdan[num] = n;
                                num++;
                                break;
                            }
                            break;
                        }
                        else
                        {
                            n++;
                        }
                    }
                    int num3 = todan - 1;
                    while (num3 >= 1)
                    {
                        if (Kifu.BAN(ban, tosuji, num3) != 0)
                        {
                            if (Kifu.BAN(ban, tosuji, num3) == koma)
                            {
                                fromsuji[num] = tosuji;
                                fromdan[num] = num3;
                                num++;
                                break;
                            }
                            break;
                        }
                        else
                        {
                            num3--;
                        }
                    }
                }
            }
            if (koma == 24 || koma == 30 || koma == 40 || koma == 46)
            {
                if (Kifu.BAN(ban, tosuji + 1, todan) == koma)
                {
                    fromsuji[num] = tosuji + 1;
                    fromdan[num] = todan;
                    num++;
                }
                if (Kifu.BAN(ban, tosuji - 1, todan) == koma)
                {
                    fromsuji[num] = tosuji - 1;
                    fromdan[num] = todan;
                    num++;
                }
                if (Kifu.BAN(ban, tosuji, todan + 1) == koma)
                {
                    fromsuji[num] = tosuji;
                    fromdan[num] = todan + 1;
                    num++;
                }
                if (Kifu.BAN(ban, tosuji, todan - 1) == koma)
                {
                    fromsuji[num] = tosuji;
                    fromdan[num] = todan - 1;
                    num++;
                }
            }
            if (koma == 24 || koma == 31 || koma == 40 || koma == 47)
            {
                if (Kifu.BAN(ban, tosuji + 1, todan + 1) == koma)
                {
                    fromsuji[num] = tosuji + 1;
                    fromdan[num] = todan + 1;
                    num++;
                }
                if (Kifu.BAN(ban, tosuji + 1, todan - 1) == koma)
                {
                    fromsuji[num] = tosuji + 1;
                    fromdan[num] = todan - 1;
                    num++;
                }
                if (Kifu.BAN(ban, tosuji - 1, todan + 1) == koma)
                {
                    fromsuji[num] = tosuji - 1;
                    fromdan[num] = todan + 1;
                    num++;
                }
                if (Kifu.BAN(ban, tosuji - 1, todan - 1) == koma)
                {
                    fromsuji[num] = tosuji - 1;
                    fromdan[num] = todan - 1;
                    num++;
                }
            }
            return num;
        }
        private static int BAN(int[,] ban, int suji, int dan)
        {
            if (suji < 1 || suji > 9 || dan < 1 || dan > 9)
            {
                return 0;
            }
            return ban[suji - 1, dan - 1];
        }
        private static int getFromID(string str, bool sente, int nmove, int[] fromsuji, int[] fromdan, int tosuji, int todan)
        {
            if (str == string.Empty)
            {
                return 0;
            }
            if (str.StartsWith("右引"))
            {
                for (int i = 0; i < nmove; i++)
                {
                    if ((sente && fromsuji[i] < tosuji && fromdan[i] < todan) || (!sente && fromsuji[i] > tosuji && fromdan[i] > todan))
                    {
                        return i;
                    }
                }
            }
            else
            {
                if (str.StartsWith("左引"))
                {
                    for (int j = 0; j < nmove; j++)
                    {
                        if ((sente && fromsuji[j] > tosuji && fromdan[j] < todan) || (!sente && fromsuji[j] < tosuji && fromdan[j] > todan))
                        {
                            return j;
                        }
                    }
                }
                else
                {
                    if (str.StartsWith("右上"))
                    {
                        for (int k = 0; k < nmove; k++)
                        {
                            if ((sente && fromsuji[k] < tosuji && fromdan[k] > todan) || (!sente && fromsuji[k] > tosuji && fromdan[k] < todan))
                            {
                                return k;
                            }
                        }
                    }
                    else
                    {
                        if (str.StartsWith("左上"))
                        {
                            for (int l = 0; l < nmove; l++)
                            {
                                if ((sente && fromsuji[l] > tosuji && fromdan[l] > todan) || (!sente && fromsuji[l] < tosuji && fromdan[l] < todan))
                                {
                                    return l;
                                }
                            }
                        }
                        else
                        {
                            if (str.StartsWith("引"))
                            {
                                for (int m = 0; m < nmove; m++)
                                {
                                    if ((sente && fromdan[m] < todan) || (!sente && fromdan[m] > todan))
                                    {
                                        return m;
                                    }
                                }
                            }
                            else
                            {
                                if (str.StartsWith("上"))
                                {
                                    for (int n = 0; n < nmove; n++)
                                    {
                                        if ((sente && fromdan[n] > todan) || (!sente && fromdan[n] < todan))
                                        {
                                            return n;
                                        }
                                    }
                                }
                                else
                                {
                                    if (str.StartsWith("寄"))
                                    {
                                        for (int num = 0; num < nmove; num++)
                                        {
                                            if (fromdan[num] == todan)
                                            {
                                                return num;
                                            }
                                        }
                                    }
                                    else
                                    {
                                        if (str.StartsWith("右"))
                                        {
                                            int num2 = fromsuji[0];
                                            int result = 0;
                                            for (int num3 = 1; num3 < nmove; num3++)
                                            {
                                                if ((sente && fromsuji[num3] < num2) || (!sente && fromsuji[num3] > num2))
                                                {
                                                    num2 = fromsuji[num3];
                                                    result = num3;
                                                }
                                            }
                                            return result;
                                        }
                                        if (str.StartsWith("左"))
                                        {
                                            int num4 = fromsuji[0];
                                            int result2 = 0;
                                            for (int num5 = 1; num5 < nmove; num5++)
                                            {
                                                if ((sente && fromsuji[num5] > num4) || (!sente && fromsuji[num5] < num4))
                                                {
                                                    num4 = fromsuji[num5];
                                                    result2 = num5;
                                                }
                                            }
                                            return result2;
                                        }
                                        if (str.StartsWith("直"))
                                        {
                                            for (int num6 = 0; num6 < nmove; num6++)
                                            {
                                                if (fromsuji[num6] == tosuji && ((sente && fromdan[num6] > todan) || (!sente && fromdan[num6] < todan)))
                                                {
                                                    return num6;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            return 0;
        }
        private static void initBanDai(int[,] ban, int[,] dai)
        {
            int[,] array = new int[,]
			{

				{
					34,
					0,
					33,
					0,
					0,
					0,
					17,
					0,
					18
				},

				{
					35,
					38,
					33,
					0,
					0,
					0,
					17,
					23,
					19
				},

				{
					36,
					0,
					33,
					0,
					0,
					0,
					17,
					0,
					20
				},

				{
					37,
					0,
					33,
					0,
					0,
					0,
					17,
					0,
					21
				},

				{
					40,
					0,
					33,
					0,
					0,
					0,
					17,
					0,
					24
				},

				{
					37,
					0,
					33,
					0,
					0,
					0,
					17,
					0,
					21
				},

				{
					36,
					0,
					33,
					0,
					0,
					0,
					17,
					0,
					20
				},

				{
					35,
					39,
					33,
					0,
					0,
					0,
					17,
					22,
					19
				},

				{
					34,
					0,
					33,
					0,
					0,
					0,
					17,
					0,
					18
				}
			};
            for (int i = 1; i <= 9; i++)
            {
                for (int j = 1; j <= 9; j++)
                {
                    ban[i - 1, j - 1] = array[i - 1, j - 1];
                }
            }
            for (int k = 0; k < 2; k++)
            {
                for (int l = 0; l < 7; l++)
                {
                    dai[k, l] = 0;
                }
            }
        }
    }
}
