#include <windows.h>



// �V����Mod.dll�̃��W���[���n���h��
HMODULE hTenShougiModDll = NULL;


// �J�n�֐�
typedef void (WINAPI *PFNTENSHOUGIMODBGN)();
PFNTENSHOUGIMODBGN p_TenShougiModBgn = NULL;

// �I���֐�
typedef void (WINAPI *PFNTENSHOUGIMODEND)();
PFNTENSHOUGIMODEND p_TenShougiModEnd = NULL;

// �V�����J�n
void TenShougiModBgn() {
	hTenShougiModDll = LoadLibrary("TenShougiMod.dll");

	p_TenShougiModBgn = (PFNTENSHOUGIMODBGN)GetProcAddress(hTenShougiModDll, "sgTenShougiModBgn");
	p_TenShougiModEnd = (PFNTENSHOUGIMODBGN)GetProcAddress(hTenShougiModDll, "sgTenShougiModEnd");

	if ( p_TenShougiModBgn ) {
		p_TenShougiModBgn();
	}
}

// �V�����I��
void TenShougiModEnd() {
	if ( p_TenShougiModEnd ) {
		p_TenShougiModEnd();
	}
	FreeLibrary(hTenShougiModDll);
}

// �V�ċL�͑��݂���̂��H
BOOL IsExistsTenshouki() {
#ifdef _DEBUG
	return TRUE;
#endif
	return FindWindow("Tenshouki95", NULL) != NULL;
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {


 	// TODO: �����ɃR�[�h��}�����Ă��������B
	MSG msg;

	if ( !IsExistsTenshouki() ) {
		return 0;
	}

	// �V�ċL�͑��݂���̂Łc
	// �V�����̋N�����J�n
	TenShougiModBgn();

	// ���C�� ���b�Z�[�W ���[�v:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		// �V�ċL���N�����Ă��Ȃ��c
		if ( !IsExistsTenshouki() ) {
			// �I��
			break;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// �V�����̏I��
	TenShougiModEnd();

	PostQuitMessage(0);

	return 0;
}
