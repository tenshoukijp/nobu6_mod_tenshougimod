// bona_pipe.cpp
// bona��pipe�őΐ킷�邽��
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TMP_BUF_LEN 256
#define BUF_PIPE_SIZE 1024
char BufGetPipe[BUF_PIPE_SIZE];

HANDLE hOutReadDup,hInWriteDup,hErrReadDup;	// �e�ŃR�s�[���Ďg�����߁B
HANDLE hProcess;

void open_engine_pipe()
{
	// �����p�C�v���쐬
	SECURITY_ATTRIBUTES sa;
	sa.nLength              = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle       = TRUE;
	sa.lpSecurityDescriptor = NULL;
	HANDLE hOutRead, hOutWrite;	// �o�͂�ǂݎ�邽�߂̃p�C�v
	HANDLE hInRead,  hInWrite;	// ���͂𑗂邽�߂̃p�C�v
//	HANDLE hErrRead, hErrWrite;	// �G���[��ǂݎ�邽��
	HANDLE hCurProc = GetCurrentProcess();

	// ����(STDIN)�p�̃p�C�v�����B
	CreatePipe(&hInRead, &hInWrite, &sa, 0);
	DuplicateHandle(hCurProc, hInWrite, hCurProc, &hInWriteDup, 0, FALSE, DUPLICATE_SAME_ACCESS);
	CloseHandle(hInWrite);	// �ȍ~�̓R�s�[���g���̂ŕ���B

	// �o��(STDOUT)�p�̃p�C�v�����B
	CreatePipe(&hOutRead, &hOutWrite, &sa, 0);
	// �n���h���̃R�s�[������āA�e�ł͂�����𗘗p����B�q�Ɍp������Ȃ��悤�ɁB
	DuplicateHandle(hCurProc, hOutRead, hCurProc, &hOutReadDup, 0, FALSE, DUPLICATE_SAME_ACCESS);
	CloseHandle(hOutRead);

/*	// �G���[(STDERR)�p�̃p�C�v�����B
	CreatePipe(&hErrRead, &hErrWrite, &sa, 0);
	DuplicateHandle(hCurProc, hErrRead, hCurProc, &hErrReadDup, 0, FALSE, DUPLICATE_SAME_ACCESS);
	CloseHandle(hErrRead);
*/
	// �v���Z�X�ɓn�����̏���
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb          = sizeof(STARTUPINFO);
	si.dwFlags     = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.wShowWindow = SW_SHOWDEFAULT;
//	si.wShowWindow = SW_HIDE;	// �R�}���h�v�����v�g�̉�ʂ��B���ꍇ�B
	si.hStdInput   = hInRead;	// ���͂͂��̃n���h���ɑ�����B�p�C�v�Ōq�����Ă�̂�hInWriteDup���疽�߂𑗂�B
	si.hStdOutput  = hOutWrite;	// �o�͂͂��̃n���h���ɑ�����B�p�C�v�Ōq�����Ă�̂�hOutReadDup�œǂݎ���B
	si.hStdError   = hOutWrite;	// �G���[�o��	   hErrWrite;

	// Engine ���N��
	char engine_dir[TMP_BUF_LEN],engine_exe[TMP_BUF_LEN];
	// c:\junk\bonanza_v4.1.1\winbin\bonanza.exe �ɃC���X�g�[������Ă�ꍇ
	strcpy(engine_dir,"c:\\junk\\bonanza_v4.1.1\\winbin");
	sprintf(engine_exe,"%s%s",engine_dir,"\\bonanza.exe csa_shogi");
	if ( CreateProcess(NULL, engine_exe, NULL, NULL, TRUE, 0, NULL, engine_dir, &si, &pi) == 0 ) { printf("create Err\n"); exit(1); }

	hProcess = pi.hProcess;

	// �����g��Ȃ��n���h�������B
	CloseHandle(pi.hThread);
	CloseHandle(hInRead);
	CloseHandle(hOutWrite);
//	CloseHandle(hErrWrite);
	Sleep(500);	// ���S�̂��߂ɑ҂B
}

void close_engine_pipe()
{
    // �q�v���Z�X�̃n���h�������B
	CloseHandle(hProcess);
	// �ʐM�Ŏg���Ă����n���h�������B
	CloseHandle(hInWriteDup);
	CloseHandle(hOutReadDup);
//	CloseHandle(hErrReadDup);
}

// �p�C�v����̏o�͂�ǂݏo��
int ReadPipe()
{
	int n = 0;
	char szOut[2],c;
	DWORD dwRead;
  	char buf[BUF_PIPE_SIZE];

	for (;;) {
		if ( ReadFile(hOutReadDup, szOut, 1, &dwRead, NULL) == 0 ) {
			printf("ReadFile Err\n");
			return 0;
		}
		if ( dwRead == 0 ) continue;
		c = szOut[0];
		buf[n] = c;
		n++;
		if ( n >= BUF_PIPE_SIZE ) { printf("Over!\n"); exit(1); }
//		printf("%c(%x)",c,c);
		if ( c == '\n' ) break;	// �R�}���h���I������
	}

	buf[n] = 0;
	printf("ReadPipe %s",buf);
	strcpy(BufGetPipe,buf);
	return 2;
}

void SendPipe(char *str)
{
	DWORD dwRead;
	WriteFile(hInWriteDup, str, strlen(str), &dwRead, NULL);
	printf("SendPipe %s",str);
}

// ������0��Ԃ��B
int ReadCommmand()
{
	for (;;) {
		int fRet = ReadPipe();
		if ( strncmp(BufGetPipe,"move",4) == 0 ) break;
		if ( strstr(BufGetPipe,"resign") != NULL ) return 0;
		if ( strstr(BufGetPipe,"Bonanza") != NULL ) break;	// �N�����̃o�[�W�����ԍ�
//		if ( strstr(BufGetPipe,"info") != NULL ) break;
	}
	return 1;
}

int board[256];
int init_board[256] = {
	0x40, 0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40, 0,0,0,0,0,

	0x40, 0x22,0x23,0x24,0x25,0x28,0x25,0x24,0x23,0x22,0x40, 0,0,0,0,0,
	0x40, 0x00,0x27,0x00,0x00,0x00,0x00,0x00,0x26,0x00,0x40, 0,0,0,0,0,
	0x40, 0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x40, 0,0,0,0,0,
	0x40, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40, 0,0,0,0,0,
	0x40, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40, 0,0,0,0,0,
	0x40, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40, 0,0,0,0,0,
	0x40, 0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x40, 0,0,0,0,0,
	0x40, 0x00,0x16,0x00,0x00,0x00,0x00,0x00,0x17,0x00,0x40, 0,0,0,0,0,
	0x40, 0x12,0x13,0x14,0x15,0x18,0x15,0x14,0x13,0x12,0x40, 0,0,0,0,0,

	0x40, 0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40, 0,0,0,0,0,
};

char *koma[16] = {
"  ","FU","KY","KE","GI","KI","KA","HI","OU","TO","NY","NK","NG","","UM","RY",
};

char sPlayerName[TMP_BUF_LEN];
int time_limit = 3;		// 1��3�b
int tesuu;

// bona�̏�����
int init_bona_engine()
{
	char str[TMP_BUF_LEN];

	ReadCommmand();	// version��ǂݎ��
	char *p = strstr(BufGetPipe,"Version");	// "Version 4.1.1\n"
	if ( p ) { p[13] = 0; sprintf(sPlayerName,"bona%s",p+7); }
	SendPipe("new\n");
//	SendPipe("book off\n");
	SendPipe("ponder off\n");
	sprintf(str,"limit time 0 %d\n",time_limit);
	SendPipe(str);
//	SendPipe("limit time 0 3\n");	// 0��3�b
//	SendPipe("tlp num 2\n");		// 2�X���b�h
//	SendPipe("5968OU\n");
//	ReadCommmand();
	return 1;
}

// ���w��(�߂������ς��蔲���j
void move(int bz,int az,int tk,int nf)
{
	if ( bz==0xff ) board[az] = tk;
	else {
		board[az] = board[bz] + nf;
		board[bz] = 0;
	}
	tesuu++;
	printf("%d��� %02x,%02x,%02x,%02x\n",tesuu,bz,az,tk,nf);
}
// ������߂������v�l���[�`��
int thinking(int *bz,int *az, int *tk, int *nf)
{
	int ret = 1;
	if ( tesuu == 0 ) { *bz = 0x73; *az = 0x63; *tk = 0; *nf = 0; }	// ����1��ڂ́�76��
	if ( tesuu == 1 ) { *bz = 0x32; *az = 0x42; *tk = 0; *nf = 0; }	// ����1��ڂ́�84��
	if ( tesuu == 2 ) { *bz = 0x95; *az = 0x85; *tk = 0; *nf = 0; }	// ����2��ڂ́�58��
	if ( tesuu == 3 ) { *bz = 0x16; *az = 0x27; *tk = 0; *nf = 0; }	// ����2��ڂ́�32��
	if ( tesuu >= 4 ) ret = 0; // ����
	return ret;
}

void convert_from_csa_move(char *s, int *bz,int *az,int *tk, int *nf)
{
	int bx = s[0] - '0';
	int by = s[1] - '0';
	if ( bx==0 && by==0 ) *bz = 0xff;	// ���
	else *bz = by*16 + 10 - bx;
	int ax = s[2] - '0';
	int ay = s[3] - '0';
	*az = ay*16 + 10 - ax;
	*nf = 0;
	int i;
	for (i=1; i<16; i++) if ( strncmp(&s[4], koma[i], 2 ) == 0 ) break;
	if ( i==16 ) { printf("��̎�ނ������ł�\n"); exit(1); }
	if ( *bz == 0xff ) {
		*tk = i + 0x10;
		if ( tesuu & 1 ) *tk += 0x10;
	} else {
		*tk = board[*az];
		if ( (board[*bz] & 0x0f) != i ) *nf = 0x08;
	}
//	printf("board[bz]=%02x,i=%d\n",board[*bz],i);
}

void play_loop()
{
	int i,bz,az,tk,nf;
	int sente = 0;
	int all_games = 0;	// �΋ǉ�

	init_bona_engine();

	for (;;) {
		tesuu = 0;
		memcpy(board,init_board,sizeof(board));
		if ( sente == 1 ) goto computer_first;

		for (i=0;i<500;i++) {
			if ( i==0 ) SendPipe("move\n");					// bona�����̎���1��ڂ��w���āA�Ƒ���
			if ( ReadCommmand() == 0 ) break;
			convert_from_csa_move(BufGetPipe + 4, &bz, &az, &tk, &nf);
			move(bz,az,tk,nf);

computer_first:
			if ( thinking(&bz,&az,&tk,&nf) == 0 ) break;	// �����̎��̓��[�v�𔲂���B
			move(bz,az,tk,nf);
			int by = bz >> 4;
			int bx = 10 - (bz & 0x0f);
			if ( bz == 0xff ) bx = by = 0;
			int ay = az >> 4;
			int ax = 10 - (az & 0x0f);
			int k = board[az] & 0x0f;
			char str[BUF_PIPE_SIZE];
			sprintf(str,"%d%d%d%d%s\n",bx,by,ax,ay,koma[k]);
			SendPipe(str);
		}

		SendPipe("new\n");
		Sleep(1000);	// ���S�̂��߂ɑ҂B
		printf("\nall_games=%d\n\n",all_games);
		sente = (sente==0);
		if ( ++all_games >= 2 ) break;
	}
}

// �p�C�v���쐬
int main()
{
	open_engine_pipe();
	play_loop();
	close_engine_pipe();
	return 0;
}
