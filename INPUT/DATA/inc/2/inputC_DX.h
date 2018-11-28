#pragma once
#include "header_DX.h"

#define KeyStateCount 256
#define DataBufferSize 16

enum push
{
	p_FREE = 0,
	p_DOWN = 1,
	p_HOLD = 2,
	p_UP = 3,
};

struct MouseInfo
{
	POINT xy;

	push left;
	push right;
	push middle;
};

class input_DX
{
public:
	LPDIRECTINPUT8 m_pDxInput;
	LPDIRECTINPUTDEVICE8 m_pDxKeypad;
	LPDIRECTINPUTDEVICE8 m_pDxMouse;

	BYTE  m_KeyCurState[KeyStateCount];
	BYTE  m_KeyBefState[KeyStateCount];
	DIMOUSESTATE m_MouseCurState;
	DIMOUSESTATE m_MouseBefState;

	MouseInfo m_MouseState;

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

	bool ResetDevice();
	bool ResetResource();

public:
	bool InitDirectInput(bool keypad, bool mouse);
	bool KeyProcess();
	bool MouseProcess();

	void SetAcquire(bool bActive);
	void DeviceAcquire();
	void DeviceUnacquire();

	bool IsKeyDown(DWORD dwKey);
	bool IsKeyUP(DWORD dwKey);
	bool IsKeyDownOnce(DWORD dwKey);

	MouseInfo getMouseInfo();

public:
	static input_DX& GetInstance();

protected:
	input_DX();
	virtual ~input_DX();
};

#define I_Input input_DX::GetInstance()