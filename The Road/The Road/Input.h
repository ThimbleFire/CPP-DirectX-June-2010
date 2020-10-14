#ifndef _INPUT_H_
#define _INPUT_H_
#define INPUT_DEADZONE (0.3f * FLOAT(0x7FFF))
#define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE  7849
#define XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 8689
#define XINPUT_GAMEPAD_TRIGGER_THRESHOLD    30

#include <wbemidl.h>
#include <XInput.h>
#include <math.h>

class Analog
{
public:
	Analog(
		float Magnitude, 
		float Xcoordinate,
		float Ycoordinate
		  );

	Analog();

	~Analog();

	float GetMagnitude();

	float GetAngle();

	float GetX();

	float GetY();

private:
	float m_Magnitude;
	float m_X;
	float m_Y;
};

class MSXInput
{
public:
	typedef enum {
						LEFT,
						RIGHT,
	}					SIDE;
	typedef enum {
						BUTTON_A,
						BUTTON_B,
						BUTTON_X,
						BUTTON_Y,
						BUTTON_UP,
						BUTTON_DOWN,
						BUTTON_LEFT,
						BUTTON_RIGHT
	}					BUTTON;

public:
	MSXInput();
	~MSXInput();

	bool Initialize();
	void Update();

	bool IsControllerDetected();

	void GetThumbStick(
		_Out_ Analog&,
		_In_ SIDE
		);

	void GetTrigger(
		_Out_ BYTE&,
		_In_ SIDE
		);

	bool GetButtonState(BUTTON);

private:
	void Update_LeftThumbstick(	XINPUT_STATE state);
	void Update_RightThumbstick(XINPUT_STATE state);
	void Update_Triggers(		XINPUT_STATE state);

	void Vibrate(int conNum, int left, int right);

	void Bound(
		_Out_ float&,
		_In_ float,
		_In_ float
		);
	
private:

	Analog lThumAnalog;
	Analog rThumAnalog;

	BYTE lTrigger;
	BYTE rTrigger;

	bool active;

	XINPUT_STATE state;
};

class Input
{

public:

	bool Initialize
	(
		void
	);

	bool Shutdown();

	bool Update();

	MSXInput& GetXbox();

private:

	MSXInput XInput_;

};

#endif

