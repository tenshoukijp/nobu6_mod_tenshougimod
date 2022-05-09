#pragma once


#include "TenShougiMod.h"


namespace TenShougiMod {

	// �V�ċL�̋�C���[�W�̃N���X�B
	// ��̖��O���C���[�W�ւ̕ϊ��p
	ref class TenShougiKomaImage {

	public:
		Hashtable^ htKomaBitmap; // ��C���[�W������摜�̃n�b�V���I�u�W�F�N�g

		Hashtable^ htKomaCount; // ��̃J�E���^�̃C���[�W������摜�̃n�b�V���I�u�W�F�N�g

		Bitmap^ bmKomaSelectedBackImg;

	public:
		TenShougiKomaImage() {

			// ���̃v���W�F�N�g�̃A�Z���u���̃^�C�v���擾�B
			System::Reflection::Assembly^ prj_assebmly = GetType()->Assembly;
			System::Resources::ResourceManager^ r = gcnew System::Resources::ResourceManager(String::Format("{0}.TenShougiMod", prj_assebmly->GetName()->Name), prj_assebmly);

			//-------------------------------------�w�i�摜
			// ���\�[�X�t�@�C������ǂݎ��

			// �n�b�V���I�u�W�F�N�g�쐬
			htKomaBitmap = gcnew Hashtable();
			
			htKomaBitmap["+OU"] =(Bitmap^)(r->GetObject("B_OU"));
			htKomaBitmap["+HI"] =(Bitmap^)(r->GetObject("B_HI"));
			htKomaBitmap["+KA"] =(Bitmap^)(r->GetObject("B_KA"));
			htKomaBitmap["+KI"] =(Bitmap^)(r->GetObject("B_KI"));
			htKomaBitmap["+GI"] =(Bitmap^)(r->GetObject("B_GI"));
			htKomaBitmap["+KE"] =(Bitmap^)(r->GetObject("B_KE"));
			htKomaBitmap["+KY"] =(Bitmap^)(r->GetObject("B_KY"));
			htKomaBitmap["+FU"] =(Bitmap^)(r->GetObject("B_FU"));

			htKomaBitmap["+NG"] =(Bitmap^)(r->GetObject("B_NG"));
			htKomaBitmap["+NK"] =(Bitmap^)(r->GetObject("B_NK"));
			htKomaBitmap["+NY"] =(Bitmap^)(r->GetObject("B_NY"));
			htKomaBitmap["+TO"] =(Bitmap^)(r->GetObject("B_TO"));
			htKomaBitmap["+RY"] =(Bitmap^)(r->GetObject("B_RY"));
			htKomaBitmap["+UM"] =(Bitmap^)(r->GetObject("B_UM"));


			htKomaBitmap["-OU"] =(Bitmap^)(r->GetObject("W_OU"));
			htKomaBitmap["-HI"] =(Bitmap^)(r->GetObject("W_HI"));
			htKomaBitmap["-KA"] =(Bitmap^)(r->GetObject("W_KA"));
			htKomaBitmap["-KI"] =(Bitmap^)(r->GetObject("W_KI"));
			htKomaBitmap["-GI"] =(Bitmap^)(r->GetObject("W_GI"));
			htKomaBitmap["-KE"] =(Bitmap^)(r->GetObject("W_KE"));
			htKomaBitmap["-KY"] =(Bitmap^)(r->GetObject("W_KY"));
			htKomaBitmap["-FU"] =(Bitmap^)(r->GetObject("W_FU"));

			htKomaBitmap["-NG"] =(Bitmap^)(r->GetObject("W_NG"));
			htKomaBitmap["-NK"] =(Bitmap^)(r->GetObject("W_NK"));
			htKomaBitmap["-NY"] =(Bitmap^)(r->GetObject("W_NY"));
			htKomaBitmap["-TO"] =(Bitmap^)(r->GetObject("W_TO"));
			htKomaBitmap["-RY"] =(Bitmap^)(r->GetObject("W_RY"));
			htKomaBitmap["-UM"] =(Bitmap^)(r->GetObject("W_UM"));

			htKomaBitmap["*"] =(Bitmap^)(r->GetObject("NON"));


			// �n�b�V���I�u�W�F�N�g�쐬
			htKomaCount = gcnew Hashtable();

			htKomaCount["0"] = (Bitmap^)(r->GetObject("NON"));
			htKomaCount["1"] = (Bitmap^)(r->GetObject("NON"));
			htKomaCount["2"] = (Bitmap^)(r->GetObject("2"));
			htKomaCount["3"] = (Bitmap^)(r->GetObject("3"));
			htKomaCount["4"] = (Bitmap^)(r->GetObject("4"));
			htKomaCount["5"] = (Bitmap^)(r->GetObject("5"));
			htKomaCount["6"] = (Bitmap^)(r->GetObject("6"));
			htKomaCount["7"] = (Bitmap^)(r->GetObject("7"));
			htKomaCount["8"] = (Bitmap^)(r->GetObject("8"));
			htKomaCount["9"] = (Bitmap^)(r->GetObject("9"));
			htKomaCount["10"] = (Bitmap^)(r->GetObject("10"));
			htKomaCount["11"] = (Bitmap^)(r->GetObject("11"));
			htKomaCount["12"] = (Bitmap^)(r->GetObject("12"));
			htKomaCount["13"] = (Bitmap^)(r->GetObject("13"));
			htKomaCount["14"] = (Bitmap^)(r->GetObject("14"));
			htKomaCount["15"] = (Bitmap^)(r->GetObject("15"));
			htKomaCount["16"] = (Bitmap^)(r->GetObject("16"));
			htKomaCount["17"] = (Bitmap^)(r->GetObject("17"));
			htKomaCount["18"] = (Bitmap^)(r->GetObject("18"));

			// ���I���������̔w�i
			bmKomaSelectedBackImg = (Bitmap^)(r->GetObject("KOMA_SELECT_BK"));
		}
	};
}