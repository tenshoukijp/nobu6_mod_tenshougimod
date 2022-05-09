#pragma once


#include "TenShougiMod.h"


namespace TenShougiMod {

	// �V�ċL�̉��C���[�W�̃N���X�B
	// ���̖��O���T�E���h�ւ̕ϊ��p
	ref class TenShougiSound {

	public:
		System::IO::Stream^ strmKomaHghSound; // ��C���[�W������摜�̃n�b�V���I�u�W�F�N�g
		System::Media::SoundPlayer^ spHghPlayer;
		System::IO::Stream^ strmKomaLowSound; // ��C���[�W������摜�̃n�b�V���I�u�W�F�N�g
		System::Media::SoundPlayer^ spLowPlayer;
		System::IO::Stream^ strmDummySound; // ��C���[�W������摜�̃n�b�V���I�u�W�F�N�g
		System::Media::SoundPlayer^ spDummyPlayer;
	public:
		TenShougiSound() {

			// ���̃v���W�F�N�g�̃A�Z���u���̃^�C�v���擾�B
			System::Reflection::Assembly^ prj_assebmly = GetType()->Assembly;
			System::Resources::ResourceManager^ r = gcnew System::Resources::ResourceManager(String::Format("{0}.TenShougiMod", prj_assebmly->GetName()->Name), prj_assebmly);

			// ���\�[�X�t�@�C������ǂݎ��
			strmKomaHghSound = (System::IO::Stream^)(r->GetObject("KomaHghMoveSound"));
			// �Đ��@���g���܂킷�B
			spHghPlayer = gcnew System::Media::SoundPlayer( strmKomaHghSound );

			strmKomaLowSound = (System::IO::Stream^)(r->GetObject("KomaLowMoveSound"));
			// �Đ��@���g���܂킷�B
			spLowPlayer = gcnew System::Media::SoundPlayer( strmKomaLowSound );

			strmDummySound = (System::IO::Stream^)(r->GetObject("DummySound"));
			// �Đ��@���g���܂킷�B
			spDummyPlayer = gcnew System::Media::SoundPlayer( strmDummySound );

		}

		void PlayKomaSound(std::string CSAstring) {

			// �E�B���h�E���J����Ă���
			if ( SharedMemory::iTenShougiFormShown ) {

				// ��������͍��߂̉��A����ȊO�̋�͒�߂̉����o���B			

				string highList[] = {"FU", "KY", "KE", "TO", "NY", "NK"}; // �����ȋ�͉�������

				int isHigh = false;

				for ( int i=0; i < sizeof(highList)/sizeof(highList[0]); i++) {
					// ��������̕����񂪌�������!!
					if ( CSAstring.find(highList[i]) != string::npos ) {
						isHigh = true;
					}
				}

				if ( isHigh ) {
					//��������񓯊��Đ�����
					spHghPlayer->Play();
				} else {
					//�Ⴂ����񓯊��Đ�����
					spLowPlayer->Play();
				}
			}
		}

		void PlayDummySound() {
			// �_�~�[�T�E���h(����)���o�����ƂŁA���Đ����ŏ��ɒx���̂�h��
			spDummyPlayer->Play();
		}
	};
}