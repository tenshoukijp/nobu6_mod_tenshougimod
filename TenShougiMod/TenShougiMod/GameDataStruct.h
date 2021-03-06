#pragma once

#include <windows.h>

#define GAMEDATASTRUCT_BUSHOU_ADDRESSS			0x4BFD18	// [«f[^(47*532)]
#define GAMEDATASTRUCT_BUSHOUNAME_ADDRESSS		0x4C5FA8	// [«¼Oe[u(18*500ÆPpÉ18*32)]
#define	GAMEDATASTRUCT_BUSHOU_NUM				532


/*
			[«f[^(47*532)]

			0-1	OÌ«
			2-3 ãëÌ«
			4-5	H
			6	­Ë
			7	íË
			8	qË
			9-10	­n   ( min 500 ` max 2000 )
			11-12	ín   ( min 500 ` max 2000 )
			13-14	qn   ( min 500 ` max 2000 )
			15	£Í
			16	ì]
			17	½
			18-19	M÷
			20	Ø 0xFFÍ³
			21-22	å¼(å¼Ô)  ®å¼yå¼Ôz;	//å¼
			23-24	Rc(RcÔ)  ®RcyRcÔz;	//Rc
			25-26	é		    ®éyéÔz;	//é
			27-28	àæ FFFF ³µ
			29	º
			30	Pû
			31	º
			32	ºÔ(0=«y,1=Rn,2=SC,3=RnSC,18ÜÅ)
			33	d¯
			34	È ¼É¥ÏÝtO èH
			35	¶N
			36-37	eçNO
			38	vl
			39	aaaabbbb a-« b-`
			40	00aabbcc a-qK b-íK c-­K
			41	Z\
			42	EÆ 0x00³ 0x10 EÒ 0x20 l 0x30  0x40 mµ 0x50 C¯ 0x60 Æ 0x70 ¤l 0x80 ¯ 0x90 ØxO 0xA0 Rm 0xB0 pm 0xC0 °
			43-44	ºÈERnSCEåCESbD
				0123 4445 5566 6777
				0=³ÖWH,1=SbD,2=RnSC,3=åC
				4,5,6,7=R,SC,Rn,«yÌºÈAMAX5
			45	aabbbccc a-Æ§ b-EÒ c-õ½
			46	1234aaa5 1=ï© 2-â¦ 3 à 4-s® 5Æ§ÌrbgÌ±«
				a-û² 
*/
struct NB6BUSHOU
{
	WORD prev;
	WORD next;
	WORD _undef2;		//must 0xFFFF
	byte maxgov;		//­Ë
	byte maxbat;		//íË
	byte maxint;		//qË
	WORD expgov;		//­n
	WORD expbat;		//ín
	WORD expint;		//qn
	char charm;			//£Í
	char ambition;		//ì]
	char loyal;			//½
	WORD merits;		//M÷
	byte blood;			//Ø 0xFFÍ³
	WORD attach;		//å¼  ®å¼yå¼Ôz;	//å¼
	WORD gundan;		//Rc  ®RcyRcÔz;	//Rc
	WORD castle;		//é  ®éyéÔz;	//é
	WORD rise;			//àæ 0xFFFF àæÌå¼Ô
	byte soldier;		//º
	byte training;		//Pû
	byte solloyal;		//º
	byte form;			//ºÔ(0=«y,1=Rn,2=SC,3=RnSC,18ÜÅ)
	byte work;			//d¯N
	byte spouse;		//È («zñÔÅ500ÔÈãªÈâPÆÈéðÅ éBèª500ÔÌÉA±±É0Æ¢¤lªüéB0Ë0+500ËA± Æ¢Á½ïB
						//È ±ÌÔª255ÈçÈÍ¢È¢B255ÈOÌ(á¦Î3)ÍA»ÌÔÉ500«µ½y«Ôz(á¦Î503)Ì«ªÈÆ¢¤±Æ)
						// »µÄYÌ«(ÈP«)Ì_Èà¯¶(á¦Î3)ÌlÉÈÁÄ¢é±ÆB
	byte birth;			//¶N - 1454
	WORD parent;		//eçNO
	byte algo;			//vl
	byte duty:4;		//`
	byte affinity:4;	//«
						//00aa bbcc a-qK b-íK c-­K
	byte aptitudegov:2;	//c-­K (2=A,1=B,0=C)
	byte aptitudebat:2;	//b-íK (2=A,1=B,0=C)
	byte aptitudeint:2;	//a-qK (2=A,1=B,0=C)
	byte undef_5:2;		//
	byte tech_ikkatsu:1;	//Z\Eê
	byte tech_ansatsu:1;	//Z\EÃE
	byte tech_ryusyutsu:1;	//Z\E¬o
	byte tech_sendou:1;		//Z\Eø®
	byte tech_ryugen:1;		//Z\E¬¾
	byte tech_benzetsu:1;	//Z\EÙã
	byte tech_yakiuchi:1;	//Z\EÄ¢
	byte tech_chohatsu:1;	//Z\E§­
	byte job;			//EÆ 0x00 ³ 0x10 EÒ 0x20 l 0x30  0x40 mµ 0x50 C¯ 0x60 Æ 0x70 ¤l 0x80 ¯ 0x90 ØxO 0xA0 Rm 0xB0 pm 0xC0 °

	byte aptitudeasi:3;		//«yK³
	byte aptitudekib:3;		//RnK³
	byte _aptitudegun_hi:2; //SCK³ãÊrbg
	byte _aptitudegun_lo:1; //SCK³ºÊrbg
	byte aptitudesui:3;		//RK³

	byte biggun:1;		//åC
	byte horsegun:1;	//RnSC
	byte steelship:1;	//SbD
	byte _undef:1;		//0
	byte life:3;		//õ½
	byte brave:3;		//EÒ
	byte _independence_hi:2; //Æ§ãÊrbg
	byte _independence_lo:1; //Æ§ºÊrbg
	byte tone:3;		//û²
	byte bEnd:1;		//s®
	byte bRise:1;		//à
	byte grudge:1;		//â¦
	byte kaiken:1;		//ï©
};


/*
			«¼Oe[u(18*500ÆPpÉ18*32)

			[18oCg]
			0-6		¼
			7-13	¼O
			14-15	«io[(ºÊ12bitÍñ`H)
			16	ãÊ4rbgÍgª(b0000=B,b0001=å¼
						@b0010=hV,b0011=ÆV
						@b0100=«,b0101=å«
						@b0110=«yª)
				ºÊ4rbg	0å¼,1R·,2»ð,3B,4Ql,5P,6?,7S
				
			17	« 0-j 1-
*/
struct NB6BUSHOUNAME
{
	char familyname[7];	//¼
	char fastname[7];	//¼O
	WORD number;		//«io[(ºÊ12bitÍñ`H)
	byte State:4;		//0å¼,1R·,2»ð,3B,4Ql,5P,6?,7S
	byte position:4;	//gª		(B=0, å¼=1, hV=2, ÆV=3, «=4, å«=5, «yª=6)
	byte sex:1;			//« 0-j 1-
	byte battleRetreat:1;	//1:íêÅÞpµ½B0:ÊíÊè
	byte battleDeath:1;	//1:íêÅíµ½B0:ÊíÊè
	byte _undef:5;
};







// «ÌûÍÉÖp[^ð]·éB
struct TBUshouDaniPowerInfo {
	int iPower;
	char szDaniName[10];
	int iBushouID;
	int iDummy;		// \õp^
};



extern NB6BUSHOU     nb6bushou[GAMEDATASTRUCT_BUSHOU_NUM];		 // «îñ
extern NB6BUSHOUNAME nb6bushouname[GAMEDATASTRUCT_BUSHOU_NUM];	 // «ËÐîñ
