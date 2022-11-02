#pragma once
#pragma warning(disable:4005)
#define DIRECTINPUT_VERSION 0x800
#include <dinput.h>
#include <dinputd.h>

#include <tuple>
#include <stdint.h>
#include <functional>
#include "../library/tnl_vector.h"

namespace tnl {
#define TNL_KEY_MAX 256

	class Input {
	public :

		#define TNL_MOUSE_INPUT_LEFT	(0x01)
		#define TNL_MOUSE_INPUT_RIGHT	(0x02)
		#define TNL_MOUSE_INPUT_MIDDLE	(0x04)
		#define TNL_MOUSE_OUTPUT_LEFT	(0x08)
		#define TNL_MOUSE_OUTPUT_RIGHT	(0x10)
		#define TNL_MOUSE_OUTPUT_MIDDLE	(0x20)

		// �}�E�X�Ή��L�[ �ꗗ
		enum class eMouse {
			LEFT,
			RIGHT,
			CENTER,
		};
		enum class eMouseTrigger {
			IN_LEFT		= TNL_MOUSE_INPUT_LEFT,
			IN_RIGHT	= TNL_MOUSE_INPUT_RIGHT,
			IN_MIDDLE	= TNL_MOUSE_INPUT_MIDDLE,
			OUT_LEFT	= TNL_MOUSE_OUTPUT_LEFT,
			OUT_RIGHT	= TNL_MOUSE_OUTPUT_RIGHT,
			OUT_MIDDLE	= TNL_MOUSE_OUTPUT_MIDDLE
		};

		// �L�[�{�[�h�Ή��L�[ �ꗗ
		enum class eKeys {
			KB_BACK		// �o�b�N�X�y�[�X
			, KB_TAB		// �^�u
			, KB_RETURN	// �G���^�[
			, KB_LSHIFT	// ���V�t�g
			, KB_RSHIFT	// �E�V�t�g
			, KB_LCONTROL	// ���R���g���[��
			, KB_RCONTROL	// �E�R���g���[��
			, KB_ESCAPE	// �G�X�P�[�v
			, KB_SPACE	    // �X�y�[�X
			, KB_PGUP		// �o�������t�o
			, KB_PGDN		// �o�������c������
			, KB_END		// �G���h
			, KB_HOME		// �z�[��
			, KB_LEFT		// ��
			, KB_UP		// ��
			, KB_RIGHT	    // �E
			, KB_DOWN		// ��
			, KB_INSERT	// �C���T�[�g
			, KB_DELETE	// �f���[�g

			, KB_MINUS		    // �|
			, KB_YEN			// ��
			, KB_PREVTRACK	    // �O
			, KB_PERIOD		// �D
			, KB_SLASH		    // �^
			, KB_LALT			// ���`�k�s
			, KB_RALT			// �E�`�k�s
			, KB_SCROLL		// ScrollLock
			, KB_SEMICOLON	    // �G
			, KB_COLON		    // �F
			, KB_LBRACKET		// �m
			, KB_RBRACKET		// �n
			, KB_AT			// ��
			, KB_BACKSLASH	    // �_
			, KB_COMMA		    // �C
			, KB_CAPSLOCK		// CaspLock
			, KB_PAUSE		    // PauseBreak

			, KB_NUMPAD0	// �e���L�[�O
			, KB_NUMPAD1	// �e���L�[�P
			, KB_NUMPAD2	// �e���L�[�Q
			, KB_NUMPAD3	// �e���L�[�R
			, KB_NUMPAD4	// �e���L�[�S
			, KB_NUMPAD5	// �e���L�[�T
			, KB_NUMPAD6	// �e���L�[�U
			, KB_NUMPAD7	// �e���L�[�V
			, KB_NUMPAD8	// �e���L�[�W
			, KB_NUMPAD9	// �e���L�[�X
			, KB_MULTIPLY	// �e���L�[��
			, KB_ADD		// �e���L�[�{
			, KB_SUBTRACT	// �e���L�[�|
			, KB_DECIMAL	// �e���L�[�D
			, KB_DIVIDE	// �e���L�[�^
			, KB_NUMPADENTER	// �e���L�[�̃G���^�[

			, KB_F1	// �e�P
			, KB_F2	// �e�Q
			, KB_F3	// �e�R
			, KB_F4	// �e�S
			, KB_F5	// �e�T
			, KB_F6	// �e�U
			, KB_F7	// �e�V
			, KB_F8	// �e�W
			, KB_F9	// �e�X
			, KB_F10	// �e�P�O
			, KB_F11	// �e�P�P
			, KB_F12	// �e�P�Q

			, KB_A	// �`
			, KB_B	// �a
			, KB_C	// �b
			, KB_D	// �c
			, KB_E	// �d
			, KB_F	// �e
			, KB_G	// �f
			, KB_H	// �g
			, KB_I	// �h
			, KB_J	// �i
			, KB_K	// �j
			, KB_L	// �k
			, KB_M	// �l
			, KB_N	// �m
			, KB_O	// �n
			, KB_P	// �o
			, KB_Q	// �p
			, KB_R	// �q
			, KB_S	// �r
			, KB_T	// �s
			, KB_U	// �t
			, KB_V	// �u
			, KB_W	// �v
			, KB_X	// �w
			, KB_Y	// �x
			, KB_Z	// �y
			, KB_0	// �O
			, KB_1	// �P
			, KB_2	// �Q
			, KB_3	// �R
			, KB_4	// �S
			, KB_5	// �T
			, KB_6	// �U
			, KB_7	// �V
			, KB_8	// �W
			, KB_9	// �X
			, KB_MAX
		};

		// �W���C�p�b�h�Ή��L�[ �ꗗ
		// �� �p�b�h�̎�ނőΉ��L�[�ԍ����قȂ�ꍇ������炵��
		enum class ePad {
			KEY_0		// �����̏ꍇ ��
			, KEY_1		// �����̏ꍇ �~
			, KEY_2		// �����̏ꍇ �Z
			, KEY_3		// �����̏ꍇ ��
			, KEY_4		// �����̏ꍇ L1
			, KEY_5		// �����̏ꍇ R1
			, KEY_6		// �����̏ꍇ L2
			, KEY_7		// �����̏ꍇ R2
			, KEY_8		// �����̏ꍇ SHARE
			, KEY_9		// �����̏ꍇ OPTION
			, KEY_10	// �����̏ꍇ LStickDown
			, KEY_11	// �����̏ꍇ RStickDown
			, KEY_12	// �����̏ꍇ HOME
			, KEY_UP	// �\���L�[ ��
			, KEY_RIGHT	// �\���L�[ �E
			, KEY_DOWN	// �\���L�[ ��
			, KEY_LEFT	// �\���L�[ ��
			, KEY_MAX
		};
		enum class eJoyStick {
			LEFT,
			RIGHT
		};


		// ������
		static void Initialize(HINSTANCE hinstance, HWND hwnd, HDC hdc, uint32_t window_w, uint32_t window_h);
		// �X�V
		static void Update();
		// ���
		static void Release();

		//------------------------------------------------------------------------------------------------------------------------------
		// �L�[�{�[�h�������o
		// arg.... eKey::KB_***
		// ret.... [ ��������Ă��� : true ] [ ��������Ă��Ȃ� : false ]
		// tips... �����̈����ɑΉ�
		//------------------------------------------------------------------------------------------------------------------------------
		template<typename... tKeys>
		static bool IsKeyDown(tKeys... iParams)
		{
			const eKeys array[] = { static_cast<eKeys>(iParams)... };
			for (eKeys elem : array) {
				if (0x80 & keys_[kb_keys[static_cast<uint32_t>(elem)]]) return true;
			}
			return false;
		}

		//------------------------------------------------------------------------------------------------------------------------------
		// �L�[�{�[�h�������ɃR�[���o�b�N�֐������s
		// arg1... �R�[���o�b�N�֐�
		// arg2... eKey::KB_***
		// tips1.. arg1 �̃R�[���o�b�N�֐��̈����� arg2 �̈����̃C���f�b�N�X���n�����
		// tips2.. �����̈����ɑΉ�
		//------------------------------------------------------------------------------------------------------------------------------
		template<typename... tKeys>
		static void RunIndexKeyDown(const std::function<void(uint32_t index)>& func, tKeys... iParams)
		{
			const eKeys array[] = { static_cast<eKeys>(iParams)... };
			uint32_t i = 0;
			for (eKeys elem : array) {
				if (0x80 & keys_[kb_keys[static_cast<uint32_t>(elem)]]) { func(i); }
				++i;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------------
		// �L�[�{�[�h�����g���K�[���o
		// arg.... eKey::KB_***
		// ret.... [ ��������Ă��� : true ] [ ��������Ă��Ȃ� : false ]
		// tips... �����̈����ɑΉ�
		//------------------------------------------------------------------------------------------------------------------------------
		template<typename... tKeys>
		static bool IsKeyDownTrigger(tKeys... iParams)
		{
			const eKeys array[] = { static_cast<eKeys>(iParams)... };
			for (eKeys elem : array) {
				if (0 != kb_trg_down[kb_keys[static_cast<uint32_t>(elem)]]) return true;
			}
			return false;
		}

		//------------------------------------------------------------------------------------------------------------------------------
		// �L�[�{�[�h�����g���K�[���ɃR�[���o�b�N�֐������s
		// arg1... �R�[���o�b�N�֐�
		// arg2... eKey::KB_***
		// tips1.. arg1 �̃R�[���o�b�N�֐��̈����� arg2 �̈����̃C���f�b�N�X���n�����
		// tips2.. �����̈����ɑΉ�
		//------------------------------------------------------------------------------------------------------------------------------
		template<typename... tKeys>
		static void RunIndexKeyDownTrigger(const std::function<void(uint32_t index)>& func, tKeys... iParams)
		{
			const eKeys array[] = { static_cast<eKeys>(iParams)... };
			uint32_t i = 0;
			for (eKeys elem : array) {
				if (0 != kb_trg_down[kb_keys[static_cast<uint32_t>(elem)]]) { func(i); }
				++i;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------------
		// ��������Ă����L�[�{�[�h�𗣂������̃g���K�[���o
		// arg.... eKey::KB_***
		// ret.... [ �������u�� : true ] [ ����ȊO : false ]
		// tips... �����̈����ɑΉ�
		//------------------------------------------------------------------------------------------------------------------------------
		template<typename... tKeys>
		static bool IsKeyReleaseTrigger(tKeys... iParams)
		{
			const eKeys array[] = { static_cast<eKeys>(iParams)... };
			for (eKeys elem : array) {
				if (0 != kb_trg_release[kb_keys[static_cast<uint32_t>(elem)]]) return true;
			}
			return false;
		}

		//------------------------------------------------------------------------------------------------------------------------------
		// ��������Ă����L�[�{�[�h�𗣂������̃g���K�[���ɃR�[���o�b�N�֐������s
		// arg1... �R�[���o�b�N�֐�
		// arg2... eKey::KB_***
		// tips1.. arg1 �̃R�[���o�b�N�֐��̈����� arg2 �̈����̃C���f�b�N�X���n�����
		// tips2.. �����̈����ɑΉ�
		//------------------------------------------------------------------------------------------------------------------------------
		template<typename... tKeys>
		static void RunIndexKeyReleaseTrigger(const std::function<void(uint32_t index)>& func, tKeys... iParams)
		{
			const eKeys array[] = { static_cast<eKeys>(iParams)... };
			uint32_t i = 0;
			for (eKeys elem : array) {
				if (0 != kb_trg_release[kb_keys[static_cast<uint32_t>(elem)]]) { func(i); }
				++i;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------------
		// �Q�[���p�b�h�������o
		// arg.... ePad::KEY_***
		// ret.... [ ��������Ă��� : true ] [ ��������Ă��Ȃ� : false ]
		// tips... �����̈����ɑΉ�
		//------------------------------------------------------------------------------------------------------------------------------
		template<typename... tPads>
		static bool IsPadDown(tPads... iParams)
		{
			const ePad array[] = { static_cast<ePad>(iParams)... };
			for (ePad elem : array) {
				if (static_cast<uint32_t>(elem) <= static_cast<uint32_t>(ePad::KEY_12)) {
					if( 0x80 & joy_state_.rgbButtons[static_cast<uint32_t>(elem)] ) return true;
				}
				else if (static_cast<uint32_t>(elem) >= static_cast<uint32_t>(ePad::KEY_UP)) {
					DWORD pov0 = joy_state_.rgdwPOV[0];
					uint32_t key = static_cast<uint32_t>(elem) - static_cast<uint32_t>(ePad::KEY_UP);
					uint32_t limit[4][2] = { {31500, 4500}, {4500, 13500}, {13500, 22500}, {22500, 31500} };
					bool check = (limit[key][0] <= pov0 && limit[key][1] >= pov0);
					if (0 == key) check = (limit[key][0] <= pov0 || limit[key][1] >= pov0);
					if (-1 != pov0 && check) { return true; }
				}
			}
			return false;
		}
		//------------------------------------------------------------------------------------------------------------------------------
		// �Q�[���p�b�h�������ɃR�[���o�b�N�֐������s
		// arg1... �R�[���o�b�N�֐�
		// arg2... ePad::KEY_***
		// tips1.. arg1 �̃R�[���o�b�N�֐��̈����� arg2 �̈����̃C���f�b�N�X���n�����
		// tips2.. �����̈����ɑΉ�
		//------------------------------------------------------------------------------------------------------------------------------
		template<typename... tPads>
		static void RunIndexPadDown(const std::function<void(uint32_t index)>& func, tPads... iParams)
		{
			const ePad array[] = { static_cast<ePad>(iParams)... };
			uint32_t i = 0;
			for (ePad elem : array) {
				if (static_cast<uint32_t>(elem) <= static_cast<uint32_t>(ePad::KEY_12)) {
					if (0x80 & joy_state_.rgbButtons[static_cast<uint32_t>(elem)]) {
						func(i);
					}
				}
				else if (static_cast<uint32_t>(elem) >= static_cast<uint32_t>(ePad::KEY_UP)) {
					DWORD pov0 = joy_state_.rgdwPOV[0];
					uint32_t key = static_cast<uint32_t>(elem) - static_cast<uint32_t>(ePad::KEY_UP);
					uint32_t limit[4][2] = { {31500, 4500}, {4500, 13500}, {13500, 22500}, {22500, 31500} };
					bool check = (limit[key][0] <= pov0 && limit[key][1] >= pov0);
					if (0 == key) check = (limit[key][0] <= pov0 || limit[key][1] >= pov0);
					if (-1 != pov0 && check) {
						func(i);
					}
				}
				i++;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------------
		// �Q�[���p�b�h�����g���K�[���o
		// arg.... ePad::KEY_***
		// ret.... [ ��������Ă��� : true ] [ ��������Ă��Ȃ� : false ]
		// tips... �����̈����ɑΉ�
		//------------------------------------------------------------------------------------------------------------------------------
		template<typename... tPads>
		static bool IsPadDownTrigger(tPads... iParams)
		{
			const ePad array[] = { static_cast<ePad>(iParams)... };
			for (ePad elem : array) {
				if (0 != pad_trg_down[static_cast<uint32_t>(elem)]) return true;
			}
			return false;
		}
		//------------------------------------------------------------------------------------------------------------------------------
		// �Q�[���p�b�h�����g���K�[���ɃR�[���o�b�N�֐������s
		// arg1... �R�[���o�b�N�֐�
		// arg2... ePad::KEY_***
		// tips1.. arg1 �̃R�[���o�b�N�֐��̈����� arg2 �̈����̃C���f�b�N�X���n�����
		// tips2.. �����̈����ɑΉ�
		//------------------------------------------------------------------------------------------------------------------------------
		template<typename... tPads>
		static void RunIndexPadDownTrigger(const std::function<void(uint32_t index)>& func, tPads... iParams)
		{
			const ePad array[] = { static_cast<ePad>(iParams)... };
			uint32_t i = 0;
			for (ePad elem : array) {
				if (0 != pad_trg_down[kb_keys[static_cast<uint32_t>(elem)]]) { func(i); }
				++i;
			}
		}
		//------------------------------------------------------------------------------------------------------------------------------
		// ��������Ă����Q�[���p�b�h�𗣂������̃g���K�[���o
		// arg.... ePad::KEY_***
		// ret.... [ �������u�� : true ] [ ����ȊO : false ]
		// tips... �����̈����ɑΉ�
		//------------------------------------------------------------------------------------------------------------------------------
		template<typename... tPads>
		static bool IsPadReleaseTrigger(tPads... iParams)
		{
			const ePad array[] = { static_cast<ePad>(iParams)... };
			for (ePad elem : array) {
				if (0 != pad_trg_release[static_cast<uint32_t>(elem)]) return true;
			}
			return false;
		}
		//------------------------------------------------------------------------------------------------------------------------------
		// ��������Ă����Q�[���p�b�h�𗣂������̃g���K�[���ɃR�[���o�b�N�֐������s
		// arg1... �R�[���o�b�N�֐�
		// arg2... ePad::KEY_***
		// tips1.. arg1 �̃R�[���o�b�N�֐��̈����� arg2 �̈����̃C���f�b�N�X���n�����
		// tips2.. �����̈����ɑΉ�
		//------------------------------------------------------------------------------------------------------------------------------
		template<typename... tPads>
		static void RunIndexPadReleaseTrigger(const std::function<void(uint32_t index)>& func, tPads... iParams)
		{
			const ePad array[] = { static_cast<ePad>(iParams)... };
			uint32_t i = 0;
			for (ePad elem : array) {
				if (0 != pad_trg_release[kb_keys[static_cast<uint32_t>(elem)]]) { func(i); }
				++i;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------------
		// �Q�[���p�b�h�� L2 R2 ���͒l�擾
		// ret... 0.0f �` 1.0f
		//------------------------------------------------------------------------------------------------------------------------------
		static float GetPadL2();
		static float GetPadR2();

		//------------------------------------------------------------------------------------------------------------------------------
		// �Q�[���p�b�h�� ���E�X�e�B�b�N���͒l�擾
		// ret... �e���ɑ΂��� -1.0f �` 1.0f
		//------------------------------------------------------------------------------------------------------------------------------
		static tnl::Vector3 GetLeftStick();
		static tnl::Vector3 GetRightStick();


		// arg... tnl::Input::eMouse::LEFT
		// arg... tnl::Input::eMouse::RIGHT
		// arg... tnl::Input::eMouse::CENTER
		static bool IsMouseDown(eMouse _mouse);

		// arg... tnl::Input::eMouseTrigger::ON_LEFT
		// arg... tnl::Input::eMouseTrigger::OUT_RIGHT
		static bool IsMouseTrigger(eMouseTrigger _mouse);

		// �}�E�X���W�̎擾
		static tnl::Vector3 GetMousePosition();
		// �}�E�X�z�C�[���̕ω��ʂ��擾
		static int32_t GetMouseWheel();
		// �}�E�X�𓮂��������̕ω���
		static tnl::Vector3 GetMouseVelocity();

	private:
		Input() {}

		static HWND hwnd_;
		static HDC hdc_;
		static HINSTANCE hinstance_;
		static uint32_t window_w_;
		static uint32_t window_h_;
		static LPDIRECTINPUTDEVICE8 key_;
		static LPDIRECTINPUTDEVICE8 mouse_;
		static LPDIRECTINPUTDEVICE8 joystick_ ;
		static DIJOYSTATE2 joy_state_;
		static DIMOUSESTATE2 ms_;
		static BYTE keys_[TNL_KEY_MAX];
		static int kb_xor_down[256] ;
		static int kb_trg_down[256] ;
		static int kb_xor_release[256] ;
		static int kb_trg_release[256] ;
		static unsigned char pad_trg_down[static_cast<uint32_t>(tnl::Input::ePad::KEY_MAX)];
		static unsigned char pad_xor_down[static_cast<uint32_t>(tnl::Input::ePad::KEY_MAX)];
		static unsigned char pad_trg_release[static_cast<uint32_t>(tnl::Input::ePad::KEY_MAX)];
		static unsigned char pad_xor_release[static_cast<uint32_t>(tnl::Input::ePad::KEY_MAX)];

		static unsigned char kb_keys[static_cast<uint32_t>(Input::eKeys::KB_MAX)];
		friend BOOL CALLBACK EnumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext) noexcept;
		friend BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext) noexcept;
	};


}

using ePad = tnl::Input::ePad;
using eKeys = tnl::Input::eKeys;
using eMouse = tnl::Input::eMouse;
using eMouseTrigger = tnl::Input::eMouseTrigger;

