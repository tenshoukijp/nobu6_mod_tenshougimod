#pragma once

#include <windows.h>

#define GAMEDATASTRUCT_BUSHOU_ADDRESSS			0x4BFD18	// [武将データ(47*532)]
#define GAMEDATASTRUCT_BUSHOUNAME_ADDRESSS		0x4C5FA8	// [武将名前テーブル(18*500と姫等用に18*32)]
#define	GAMEDATASTRUCT_BUSHOU_NUM				532


/*
			[武将データ(47*532)]

			0-1	前の武将
			2-3 後ろの武将
			4-5	？
			6	政才
			7	戦才
			8	智才
			9-10	政熟   ( min 500 ～ max 2000 )
			11-12	戦熟   ( min 500 ～ max 2000 )
			13-14	智熟   ( min 500 ～ max 2000 )
			15	魅力
			16	野望
			17	忠誠
			18-19	勲功
			20	血筋 0xFFは無縁
			21-22	大名(大名番号)  所属大名【大名番号】;	//大名
			23-24	軍団(軍団番号)  所属軍団【軍団番号】;	//軍団
			25-26	居城		    所属居城【城番号】;	//居城
			27-28	内応先 FFFF 無し
			29	兵数
			30	訓練
			31	兵忠
			32	兵態(0=足軽,1=騎馬,2=鉄砲,3=騎馬鉄砲,18まで)
			33	仕官
			34	妻 他に結婚済みフラグあり？
			35	生年
			36-37	親顔NO
			38	思考
			39	aaaabbbb a-相性 b-義理
			40	00aabbcc a-智適 b-戦適 c-政適
			41	技能
			42	職業 0x00無 0x10 忍者 0x20 茶人 0x30 剣豪 0x40 僧侶 0x50 海賊 0x60 高家 0x70 商人 0x80 庶民 0x90 切支丹 0xA0 騎士 0xB0 魔術士 0xC0 魔族
			43-44	兵科・騎馬鉄砲・大砲・鉄甲船
				0123 4445 5566 6777
				0=無関係？,1=鉄甲船,2=騎馬鉄砲,3=大砲
				4,5,6,7=水軍,鉄砲,騎馬,足軽の兵科、MAX5
			45	aabbbccc a-独立 b-勇猛 c-寿命
			46	1234aaa5 1=会見 2-遺恨 3 内応 4-行動 5独立のビットの続き
				a-口調 
*/
struct NB6BUSHOU
{
	WORD prev;
	WORD next;
	WORD _undef2;		//must 0xFFFF
	byte maxgov;		//政才
	byte maxbat;		//戦才
	byte maxint;		//智才
	WORD expgov;		//政熟
	WORD expbat;		//戦熟
	WORD expint;		//智熟
	char charm;			//魅力
	char ambition;		//野望
	char loyal;			//忠誠
	WORD merits;		//勲功
	byte blood;			//血筋 0xFFは無縁
	WORD attach;		//大名  所属大名【大名番号】;	//大名
	WORD gundan;		//軍団  所属軍団【軍団番号】;	//軍団
	WORD castle;		//居城  所属居城【城番号】;	//居城
	WORD rise;			//内応先 0xFFFF 内応先の大名番号
	byte soldier;		//兵数
	byte training;		//訓練
	byte solloyal;		//兵忠
	byte form;			//兵態(0=足軽,1=騎馬,2=鉄砲,3=騎馬鉄砲,18まで)
	byte work;			//仕官年数
	byte spouse;		//妻 (武将配列番号で500番以上が妻や姫となる条件である。相手が500番の時に、ここに0という数値が入る。0⇒0+500⇒帰蝶 といった具合。
						//妻 この番号が255なら妻はいない。255以外の時(例えば3)は、その番号に500足した【武将番号】(例えば503)の女性が妻ということ)
						// そして該当の武将(妻姫武将)の_妻も同じ(例えば3)の値になっていること。
	byte birth;			//生年 - 1454
	WORD parent;		//親顔NO
	byte algo;			//思考
	byte duty:4;		//義理
	byte affinity:4;	//相性
						//00aa bbcc a-智適 b-戦適 c-政適
	byte aptitudegov:2;	//c-政適 (2=A,1=B,0=C)
	byte aptitudebat:2;	//b-戦適 (2=A,1=B,0=C)
	byte aptitudeint:2;	//a-智適 (2=A,1=B,0=C)
	byte undef_5:2;		//
	byte tech_ikkatsu:1;	//技能・一喝
	byte tech_ansatsu:1;	//技能・暗殺
	byte tech_ryusyutsu:1;	//技能・流出
	byte tech_sendou:1;		//技能・煽動
	byte tech_ryugen:1;		//技能・流言
	byte tech_benzetsu:1;	//技能・弁舌
	byte tech_yakiuchi:1;	//技能・焼討
	byte tech_chohatsu:1;	//技能・挑発
	byte job;			//職業 0x00 無 0x10 忍者 0x20 茶人 0x30 剣豪 0x40 僧侶 0x50 海賊 0x60 高家 0x70 商人 0x80 庶民 0x90 切支丹 0xA0 騎士 0xB0 魔術士 0xC0 魔族

	byte aptitudeasi:3;		//足軽適正
	byte aptitudekib:3;		//騎馬適正
	byte _aptitudegun_hi:2; //鉄砲適正上位ビット
	byte _aptitudegun_lo:1; //鉄砲適正下位ビット
	byte aptitudesui:3;		//水軍適正

	byte biggun:1;		//大砲
	byte horsegun:1;	//騎馬鉄砲
	byte steelship:1;	//鉄甲船
	byte _undef:1;		//0
	byte life:3;		//寿命
	byte brave:3;		//勇猛
	byte _independence_hi:2; //独立上位ビット
	byte _independence_lo:1; //独立下位ビット
	byte tone:3;		//口調
	byte bEnd:1;		//行動
	byte bRise:1;		//内応
	byte grudge:1;		//遺恨
	byte kaiken:1;		//会見
};


/*
			武将名前テーブル(18*500と姫等用に18*32)

			[18バイト]
			0-6		名字
			7-13	名前
			14-15	武将ナンバー(下位12bitは列伝？)
			16	上位4ビットは身分(b0000=隠居,b0001=大名
						　b0010=宿老,b0011=家老
						　b0100=部将,b0101=侍大将
						　b0110=足軽頭)
				下位4ビット	0大名,1軍長,2現役,3隠居,4浪人,5姫,6?,7死亡
				
			17	性 0-男 1-女
*/
struct NB6BUSHOUNAME
{
	char familyname[7];	//名字
	char fastname[7];	//名前
	WORD number;		//武将ナンバー(下位12bitは列伝？)
	byte State:4;		//0大名,1軍長,2現役,3隠居,4浪人,5姫,6?,7死亡
	byte position:4;	//身分		(隠居=0, 大名=1, 宿老=2, 家老=3, 部将=4, 侍大将=5, 足軽頭=6)
	byte sex:1;			//性 0-男 1-女
	byte battleRetreat:1;	//1:戦場で退却した。0:通常通り
	byte battleDeath:1;	//1:戦場で戦死した。0:通常通り
	byte _undef:5;
};







// 武将の棋力に関数ｒパラメータを転送する。
struct TBUshouDaniPowerInfo {
	int iPower;
	char szDaniName[10];
	int iBushouID;
	int iDummy;		// 予備パラメタ
};



extern NB6BUSHOU     nb6bushou[GAMEDATASTRUCT_BUSHOU_NUM];		 // 武将情報
extern NB6BUSHOUNAME nb6bushouname[GAMEDATASTRUCT_BUSHOU_NUM];	 // 武将戸籍情報
