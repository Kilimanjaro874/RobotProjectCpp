#include <chrono>
#pragma comment(lib,"winmm.lib")

#include "DxLib.h"
#include "../library/tnl_input.h"
#include "../game/gm_main.h"
#include "../dxlib_ext/dxlib_ext.h"


static std::chrono::system_clock::time_point clock_start, clock_end ; 
static std::chrono::system_clock::time_point fps_clock_start, fps_clock_end;


// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
						LPSTR lpCmdLine, int nCmdShow )
{

	// �E�B���h�E���[�h�ŋN��
	ChangeWindowMode( true ) ;

	// �E�B���h�E�T�C�Y�ݒ�
	SetGraphMode(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, 32 ) ;

	// �c�w���C�u��������������
	if( DxLib_Init() == -1 )		
	{
		// �G���[���N�����璼���ɏI��
		return -1 ;				
	}
	SetDrawScreen(DX_SCREEN_BACK);

	// �v���J�n����
	clock_start = std::chrono::system_clock::now();

	// �L�[���͐���̏�����
	HWND hWnd = GetMainWindowHandle();
	HDC hdc = GetDC(hWnd);
	tnl::Input::Initialize(hInstance,hWnd, hdc, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);

	// ���b�Z�[�W���[�v
	while( 1 )
	{
		// �t���[���Ԃ̌o�ߎ���
		// �}�C�N���b�Ōv�����ĕb�ɕϊ�
		clock_end = std::chrono::system_clock::now();
		double micro_seconds = static_cast<double>( std::chrono::duration_cast<std::chrono::microseconds>(clock_end - clock_start).count() );
		float delta_time = static_cast<float>( micro_seconds / 1000.0 / 1000.0 );
		clock_start = clock_end;

		if( ProcessMessage() == -1 ){
			break ;
		}

		fps_clock_start = std::chrono::system_clock::now();

		// ��ʂ��N���A
		ClearDrawScreen() ;

		// �L�[��Ԃ̍X�V
		tnl::Input::Update();

		// �Q�[�����C�����[�`��
		gameMain(delta_time);

		// �o�b�N�o�b�t�@���t���b�v
		ScreenFlip() ;
		//SetWaitVSyncFlag(FALSE);

		// �t���[�����[�g�R���g���[��
		fps_clock_end = std::chrono::system_clock::now();
		double fps_mic = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(fps_clock_end - fps_clock_start).count());
		float fps_mils = static_cast<float>(fps_mic / 1000.0);
		float fps_lim = 1000.0f / 60.0f;

		if (fps_lim > fps_mils) {
			timeBeginPeriod(1);
			Sleep(DWORD(fps_lim - fps_mils));
			timeBeginPeriod(1);
		}

	}

	// �Q�[�����̏I������
	gameEnd();

	ReleaseDC(hWnd, hdc);

	// �c�w���C�u�����g�p�̏I������
	DxLib_End() ;			

	// �\�t�g�̏I��
	return 0 ;				 
}
