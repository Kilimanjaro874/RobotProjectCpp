#ifndef _WIN32_DCOM
#define _WIN32_DCOM
#endif

#include <wrl.h>
#include <assert.h>
#include "tnl_input.h"
#include "tnl_util.h"
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

namespace tnl {
	HWND Input::hwnd_;
	HINSTANCE Input::hinstance_;
	HDC Input::hdc_;
	uint32_t Input::window_h_;
	uint32_t Input::window_w_;
	LPDIRECTINPUT8 g_input = nullptr;
	LPDIRECTINPUTDEVICE8 Input::key_ = nullptr;
	LPDIRECTINPUTDEVICE8 Input::mouse_;
	DIMOUSESTATE2 Input::ms_;
	DIJOYSTATE2 Input::joy_state_;
	LPDIRECTINPUTDEVICE8 Input::joystick_ = nullptr;

	BYTE Input::keys_[TNL_KEY_MAX];
	int Input::kb_xor_down[256] = { 0 };
	int Input::kb_trg_down[256] = { 0 };
	int Input::kb_xor_release[256] = { 0 };
	int Input::kb_trg_release[256] = { 0 };


	BOOL CALLBACK    EnumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext) noexcept;
	BOOL CALLBACK    EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext) noexcept;


	struct DI_ENUM_CONTEXT
	{
		DIJOYCONFIG* pPreferredJoyCfg;
		bool bPreferredJoyCfgValid;
	};


	//-----------------------------------------------------------------------------
	// Name: EnumObjectsCallback()
	// Desc: Callback function for enumerating objects (axes, buttons, POVs) on a 
	//       joystick. This function enables user interface elements for objects
	//       that are found to exist, and scales axes min/max values.
	//-----------------------------------------------------------------------------
	BOOL CALLBACK EnumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi,
		VOID* pContext) noexcept
	{
		auto hDlg = static_cast<HWND>(pContext);

		static int nSliderCount = 0;  // Number of returned slider controls
		static int nPOVCount = 0;     // Number of returned POV controls

		// For axes that are returned, set the DIPROP_RANGE property for the
		// enumerated axis in order to scale min/max values.
		if (pdidoi->dwType & DIDFT_AXIS)
		{
			DIPROPRANGE diprg;
			diprg.diph.dwSize = sizeof(DIPROPRANGE);
			diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
			diprg.diph.dwHow = DIPH_BYID;
			diprg.diph.dwObj = pdidoi->dwType; // Specify the enumerated axis
			diprg.lMin = -1000;
			diprg.lMax = +1000;

			// Set the range for the axis
			if (FAILED(Input::joystick_->SetProperty(DIPROP_RANGE, &diprg.diph)))
				return DIENUM_STOP;

		}

		return DIENUM_CONTINUE;
	}

	//-----------------------------------------------------------------------------
	// Name: EnumJoysticksCallback()
	// Desc: Called once for each enumerated joystick. If we find one, create a
	//       device interface on it so we can play with it.
	//-----------------------------------------------------------------------------
	BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance,
		VOID* pContext) noexcept
	{
		auto pEnumContext = static_cast<DI_ENUM_CONTEXT*>(pContext);


		// Skip anything other than the perferred joystick device as defined by the control panel.  
		// Instead you could store all the enumerated joysticks and let the user pick.
		if (pEnumContext->bPreferredJoyCfgValid &&
			!IsEqualGUID(pdidInstance->guidInstance, pEnumContext->pPreferredJoyCfg->guidInstance))
			return DIENUM_CONTINUE;

		// Obtain an interface to the enumerated joystick.
		HRESULT hr = g_input->CreateDevice(pdidInstance->guidInstance, &Input::joystick_, nullptr);

		// If it failed, then we can't use this joystick. (Maybe the user unplugged
		// it while we were in the middle of enumerating it.)
		if (FAILED(hr))
			return DIENUM_CONTINUE;

		// Stop enumeration. Note: we're just taking the first joystick we get. You
		// could store all the enumerated joysticks and let the user pick.
		return DIENUM_STOP;
	}



	static int xor_mouse[33] = {
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1
	};
	static int trg_mouse[33] = {
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0
	};

	static unsigned char mouses[6] = {
		TNL_MOUSE_INPUT_LEFT
		,TNL_MOUSE_INPUT_RIGHT
		,TNL_MOUSE_INPUT_MIDDLE
		,TNL_MOUSE_OUTPUT_LEFT
		,TNL_MOUSE_OUTPUT_RIGHT
		,TNL_MOUSE_OUTPUT_MIDDLE
	};

	unsigned char Input::kb_keys[static_cast<uint32_t>(Input::eKeys::KB_MAX)] = {
		 DIK_BACK		// �o�b�N�X�y�[�X�L�[
		,DIK_TAB		// �^�u�L�[
		,DIK_RETURN		// �G���^�[�L�[
		,DIK_LSHIFT		// ���V�t�g�L�[
		,DIK_RSHIFT		// �E�V�t�g�L�[
		,DIK_LCONTROL	// ���R���g���[���L�[
		,DIK_RCONTROL	// �E�R���g���[���L�[
		,DIK_ESCAPE		// �G�X�P�[�v�L�[
		,DIK_SPACE		// �X�y�[�X�L�[
		,DIK_PGUP		// �o�������t�o�L�[
		,DIK_PGDN		// �o�������c�������L�[
		,DIK_END		// �G���h�L�[
		,DIK_HOME		// �z�[���L�[
		,DIK_LEFT		// ���L�[
		,DIK_UP			// ��L�[
		,DIK_RIGHT		// �E�L�[
		,DIK_DOWN		// ���L�[
		,DIK_INSERT		// �C���T�[�g�L�[
		,DIK_DELETE		// �f���[�g�L�[

		,DIK_MINUS		// �|�L�[
		,DIK_YEN		// ���L�[
		,DIK_PREVTRACK	// �O�L�[
		,DIK_PERIOD		// �D�L�[
		,DIK_SLASH		// �^�L�[
		,DIK_LALT		// ���`�k�s�L�[
		,DIK_RALT		// �E�`�k�s�L�[
		,DIK_SCROLL		// ScrollLock�L�[
		,DIK_SEMICOLON	// �G�L�[
		,DIK_COLON		// �F�L�[
		,DIK_LBRACKET	// �m�L�[
		,DIK_RBRACKET	// �n�L�[
		,DIK_AT			// ���L�[
		,DIK_BACKSLASH	// �_�L�[
		,DIK_COMMA		// �C�L�[
		,DIK_CAPSLOCK	// CaspLock�L�[
		,DIK_PAUSE		// PauseBreak�L�[

		,DIK_NUMPAD0	// �e���L�[�O
		,DIK_NUMPAD1	// �e���L�[�P
		,DIK_NUMPAD2	// �e���L�[�Q
		,DIK_NUMPAD3	// �e���L�[�R
		,DIK_NUMPAD4	// �e���L�[�S
		,DIK_NUMPAD5	// �e���L�[�T
		,DIK_NUMPAD6	// �e���L�[�U
		,DIK_NUMPAD7	// �e���L�[�V
		,DIK_NUMPAD8	// �e���L�[�W
		,DIK_NUMPAD9	// �e���L�[�X
		,DIK_MULTIPLY	// �e���L�[���L�[
		,DIK_ADD		// �e���L�[�{�L�[
		,DIK_SUBTRACT	// �e���L�[�|�L�[
		,DIK_DECIMAL	// �e���L�[�D�L�[
		,DIK_DIVIDE	// �e���L�[�^�L�[
		,DIK_NUMPADENTER	// �e���L�[�̃G���^�[�L�[

		,DIK_F1	// �e�P�L�[
		,DIK_F2	// �e�Q�L�[
		,DIK_F3	// �e�R�L�[
		,DIK_F4	// �e�S�L�[
		,DIK_F5	// �e�T�L�[
		,DIK_F6	// �e�U�L�[
		,DIK_F7	// �e�V�L�[
		,DIK_F8	// �e�W�L�[
		,DIK_F9	// �e�X�L�[
		,DIK_F10	// �e�P�O�L�[
		,DIK_F11	// �e�P�P�L�[
		,DIK_F12	// �e�P�Q�L�[

		,DIK_A	// �`�L�[
		,DIK_B	// �a�L�[
		,DIK_C	// �b�L�[
		,DIK_D	// �c�L�[
		,DIK_E	// �d�L�[
		,DIK_F	// �e�L�[
		,DIK_G	// �f�L�[
		,DIK_H	// �g�L�[
		,DIK_I	// �h�L�[
		,DIK_J	// �i�L�[
		,DIK_K	// �j�L�[
		,DIK_L	// �k�L�[
		,DIK_M	// �l�L�[
		,DIK_N	// �m�L�[
		,DIK_O	// �n�L�[
		,DIK_P	// �o�L�[
		,DIK_Q	// �p�L�[
		,DIK_R	// �q�L�[
		,DIK_S	// �r�L�[
		,DIK_T	// �s�L�[
		,DIK_U	// �t�L�[
		,DIK_V	// �u�L�[
		,DIK_W	// �v�L�[
		,DIK_X	// �w�L�[
		,DIK_Y	// �x�L�[
		,DIK_Z	// �y�L�[
		,DIK_0	// �O�L�[
		,DIK_1	// �P�L�[
		,DIK_2	// �Q�L�[
		,DIK_3	// �R�L�[
		,DIK_4	// �S�L�[
		,DIK_5	// �T�L�[
		,DIK_6	// �U�L�[
		,DIK_7	// �V�L�[
		,DIK_8	// �W�L�[
		,DIK_9	// �X�L�[
	};

	unsigned char Input::pad_trg_down[static_cast<uint32_t>(tnl::Input::ePad::KEY_MAX)] = { 0 };
	unsigned char Input::pad_xor_down[static_cast<uint32_t>(tnl::Input::ePad::KEY_MAX)] = { 0 };
	unsigned char Input::pad_trg_release[static_cast<uint32_t>(tnl::Input::ePad::KEY_MAX)] = { 
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
	};
	unsigned char Input::pad_xor_release[static_cast<uint32_t>(tnl::Input::ePad::KEY_MAX)] = { 
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
	};

	void Input::Initialize(HINSTANCE hinstance, HWND hwnd, HDC hdc, uint32_t window_w, uint32_t window_h) {
		hwnd_ = hwnd;
		hinstance_ = hinstance;
		hdc_ = hdc;
		window_w_ = window_w;
		window_h_ = window_h;

		HRESULT hr = S_OK;
		memset(keys_, 0, sizeof(keys_));

		hr = DirectInput8Create(hinstance_, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)(&g_input), NULL);
		if (FAILED(hr)) { TNL_DEBUG_ASSERTE("input initialize errror"); }

		//
		// �L�[�{�[�h�ݒ�
		//
		hr = g_input->CreateDevice(GUID_SysKeyboard, &key_, NULL);
		if (FAILED(hr)) { TNL_DEBUG_ASSERTE("input initialize errror"); }

		hr = key_->SetDataFormat(&c_dfDIKeyboard);
		if (FAILED(hr)) { TNL_DEBUG_ASSERTE("input initialize errror"); }

		hr = key_->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
		if (FAILED(hr)) { TNL_DEBUG_ASSERTE("input initialize errror"); }

		key_->Acquire();


		//
		// �}�E�X�ݒ�
		//
		hr = g_input->CreateDevice(GUID_SysMouse, &mouse_, NULL);
		if (FAILED(hr)) { TNL_DEBUG_ASSERTE("input initialize errror"); }

		hr = mouse_->SetDataFormat(&c_dfDIMouse2);
		if (FAILED(hr)) { TNL_DEBUG_ASSERTE("input initialize errror"); }

		hr = mouse_->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
		if (FAILED(hr)) { TNL_DEBUG_ASSERTE("input initialize errror"); }

		// �f�o�C�X�̐ݒ�   
		DIPROPDWORD diprop;
		diprop.diph.dwSize = sizeof(diprop);
		diprop.diph.dwHeaderSize = sizeof(diprop.diph);
		diprop.diph.dwObj = 0;
		diprop.diph.dwHow = DIPH_DEVICE;
		diprop.dwData = DIPROPAXISMODE_REL;
		hr = mouse_->SetProperty(DIPROP_AXISMODE, &diprop.diph);
		if (FAILED(hr)) { TNL_DEBUG_ASSERTE("input initialize errror"); }
		mouse_->Acquire();

		for (int i = 0; i < static_cast<uint32_t>(Input::eKeys::KB_MAX); ++i) {
			kb_xor_release[i] = 1;
		}
		
		//
		// �p�b�h�ݒ�
		//
		DIJOYCONFIG PreferredJoyCfg = {};
		DI_ENUM_CONTEXT enumContext;
		enumContext.pPreferredJoyCfg = &PreferredJoyCfg;
		enumContext.bPreferredJoyCfgValid = false;

		Microsoft::WRL::ComPtr<IDirectInputJoyConfig8> pJoyConfig;
		if (FAILED(g_input->QueryInterface(IID_IDirectInputJoyConfig8, (void**)&pJoyConfig))) {
			tnl::DebugTrace("game pad setting error");
		}

		PreferredJoyCfg.dwSize = sizeof(PreferredJoyCfg);
		if (SUCCEEDED(pJoyConfig->GetConfig(0, &PreferredJoyCfg, DIJC_GUIDINSTANCE)))
		{
			// This function is expected to fail if no joystick is attached
			enumContext.bPreferredJoyCfgValid = true;
		}

		// Look for a simple joystick we can use for this sample program.
		if (FAILED(g_input->EnumDevices(DI8DEVCLASS_GAMECTRL,
			EnumJoysticksCallback,
			&enumContext, DIEDFL_ATTACHEDONLY))) {
			tnl::DebugTrace("game pad setting error");
		}

		// Make sure we got a joystick
		if (joystick_)
		{
			// Set the data format to "simple joystick" - a predefined data format 
			//
			// A data format specifies which controls on a device we are interested in,
			// and how they should be reported. This tells DInput that we will be
			// passing a DIJOYSTATE2 structure to IDirectInputDevice::GetDeviceState().
			if (FAILED(joystick_->SetDataFormat(&c_dfDIJoystick2))) {
				tnl::DebugTrace("game pad setting error");
			}

			// Set the cooperative level to let DInput know how this device should
			// interact with the system and with other DInput applications.
			if (FAILED(joystick_->SetCooperativeLevel(hwnd_,
				DISCL_EXCLUSIVE | DISCL_FOREGROUND))) {
				tnl::DebugTrace("game pad setting error");
			}

			// Enumerate the joystick objects. The callback function enabled user
			// interface elements for objects that are found, and sets the min/max
			// values property for discovered axes.
			if (FAILED(joystick_->EnumObjects(EnumObjectsCallback,
				(VOID*)hwnd_, DIDFT_ALL))) {
				tnl::DebugTrace("game pad setting error");
			}
		}

	}

	void Input::Update() {
		key_->GetDeviceState(sizeof(keys_), &keys_);
		if (FAILED(mouse_->GetDeviceState(sizeof(DIMOUSESTATE2), &ms_))) {
			mouse_->Acquire();
		}

		//
		// key bord
		//
		memset(kb_trg_down, 0, sizeof(kb_trg_down));
		memset(kb_trg_release, 0, sizeof(kb_trg_release));

		for (uint32_t i = 0; i < static_cast<uint32_t>(Input::eKeys::KB_MAX); i++) {
			if (0x80 & keys_[kb_keys[i]]) {
				kb_trg_down[kb_keys[i]] = (kb_xor_down[kb_keys[i]] ^ 0x01);
				kb_xor_down[kb_keys[i]] = 0x01;

				kb_xor_release[kb_keys[i]] = 0;

			}
			else {
				kb_xor_down[kb_keys[i]] = 0;

				kb_trg_release[kb_keys[i]] = (kb_xor_release[kb_keys[i]] ^ 0x01);
				kb_xor_release[kb_keys[i]] = 0x01;
			}
		}

		//
		// mouse
		//
		memset(trg_mouse, 0, sizeof(trg_mouse));

		for (int i = 0; i < 3; i++) {
			if (ms_.rgbButtons[i] & 0x80) {

				// input
				trg_mouse[mouses[i]] = (xor_mouse[mouses[i]] ^ 0x01);
				xor_mouse[mouses[i]] = 0x01;

				// output
				xor_mouse[mouses[i + 3]] = 0;
			}
			else {
				// input
				xor_mouse[mouses[i]] = 0;

				// output
				trg_mouse[mouses[i + 3]] = (xor_mouse[mouses[i + 3]] ^ 0x01);
				xor_mouse[mouses[i + 3]] = 0x01;
			}
		}


		//
		// game pad
		//
		memset(pad_trg_down, 0, sizeof(pad_trg_down));
		memset(pad_trg_release, 0, sizeof(pad_trg_release));


		// Poll the device to read the current state
		if (!joystick_) return;
		HRESULT hr = joystick_->Poll();
		if (FAILED(hr))
		{
			// DInput is telling us that the input stream has been
			// interrupted. We aren't tracking any state between polls, so
			// we don't have any special reset that needs to be done. We
			// just re-acquire and try again.
			hr = joystick_->Acquire();
			while (hr == DIERR_INPUTLOST)
				hr = joystick_->Acquire();
		}

		// Get the input's device state
		if (FAILED(hr = joystick_->GetDeviceState(sizeof(DIJOYSTATE2), &joy_state_))) {
		}

		for (uint32_t i = 0; i < static_cast<uint32_t>(tnl::Input::ePad::KEY_12); ++i) {
			if (joy_state_.rgbButtons[i] & 0x80) {
				pad_trg_down[i] = (pad_xor_down[i] ^ 0x01);
				pad_xor_down[i] = 0x01;
				pad_xor_release[i] = 0;
			}
			else {
				pad_trg_release[i] = (pad_xor_release[i] ^ 0x01);
				pad_xor_release[i] = 0x01;
				pad_xor_down[i] = 0;
			}
		}

		DWORD pov0 = joy_state_.rgdwPOV[0];
		uint32_t keys[4] = {
			static_cast<uint32_t>(tnl::Input::ePad::KEY_UP),
			static_cast<uint32_t>(tnl::Input::ePad::KEY_RIGHT),
			static_cast<uint32_t>(tnl::Input::ePad::KEY_DOWN),
			static_cast<uint32_t>(tnl::Input::ePad::KEY_LEFT)
		};
		uint32_t limit[4][2] = {
			{31500, 4500},
			{4500, 13500},
			{13500, 22500},
			{22500, 31500}
		};
		for (uint32_t i = 0; i < 4; ++i) {
			bool check = (limit[i][0] <= pov0 && limit[i][1] >= pov0);
			if(0 == i) check = (limit[i][0] <= pov0 || limit[i][1] >= pov0);
			if (-1 != pov0 && check ) {
				pad_trg_down[keys[i]] = (pad_xor_down[keys[i]] ^ 0x01);
				pad_xor_down[keys[i]] = 0x01;
				pad_xor_release[keys[i]] = 0;
			}
			else {
				pad_trg_release[keys[i]] = (pad_xor_release[keys[i]] ^ 0x01);
				pad_xor_release[keys[i]] = 0x01;
				pad_xor_down[keys[i]] = 0;
			}
		}

	}

	void Input::Release() {
		key_->Release();
		g_input->Release();
	}


	bool Input::IsMouseDown(eMouse _mouse) {
		return 0x80 & ms_.rgbButtons[static_cast<uint32_t>(_mouse)];
	}

	bool Input::IsMouseTrigger(eMouseTrigger _mouse) {
		if (0 != trg_mouse[static_cast<uint32_t>(_mouse)]) return true;
		return false;
	}

	float Input::GetPadL2() {
		return float(joy_state_.lRx + 1000) / 2000.0f;
	}
	float Input::GetPadR2() {
		return float(joy_state_.lRy + 1000) / 2000.0f;
	}
	tnl::Vector3 Input::GetLeftStick() {
		return { float(joy_state_.lX) / 1000.0f, float(joy_state_.lY) / 1000.0f, 0 };
	}
	tnl::Vector3 Input::GetRightStick() {
		return { float(joy_state_.lZ) / 1000.0f, float(joy_state_.lRz) / 1000.0f, 0 };
	}

	tnl::Vector3 Input::GetMousePosition() {
		POINT pt;
		RECT rec;
		GetClientRect(hwnd_, &rec);
		float rw = (float)window_w_ / (float)rec.right;
		float rh = (float)window_h_ / (float)rec.bottom;

		GetCursorPos(&pt);
		ScreenToClient(hwnd_, &pt);

		pt.x = (LONG)((float)pt.x * rw);
		pt.y = (LONG)((float)pt.y * rh);
		return {(float)pt.x, (float)pt.y, 0};
	}

	int32_t Input::GetMouseWheel() {
		return ms_.lZ;
	}

	tnl::Vector3 Input::GetMouseVelocity() {
		return { (float)ms_.lX, (float)ms_.lY, 0 };
	}
}
