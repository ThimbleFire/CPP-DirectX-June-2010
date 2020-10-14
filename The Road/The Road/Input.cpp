#include "Input.h"

/*/ ____________
///
///	ANALOG
/*/

Analog::Analog(float mag, float x, float y)
{
	this->m_Magnitude = mag;
	this->m_X = x;
	this->m_Y = y;
}

Analog::Analog()
{

}

Analog::~Analog()
{
	m_Magnitude = 0;
	m_X = 0;
	m_Y = 0;
}

float Analog::GetMagnitude()
{
	return m_Magnitude;
}

float Analog::GetX()
{
	return m_X;
}

float Analog::GetY()
{
	return m_Y;
}

float Analog::GetAngle()
{
	return 0.0f;
}

/*/ ____________
///
///	MSXInput
/*/

MSXInput::MSXInput()
{
}

MSXInput::~MSXInput()
{

}


bool MSXInput::Initialize()
{
	XINPUT_CAPABILITIES caps;

	ZeroMemory(&caps, sizeof(XINPUT_CAPABILITIES));

	XInputGetCapabilities(1, XINPUT_FLAG_GAMEPAD, &caps);
	
	ZeroMemory(&state, sizeof(XINPUT_STATE));
	
	if(caps.Type != XINPUT_DEVTYPE_GAMEPAD)
		return false;
	 
	return true;
}

bool MSXInput::IsControllerDetected()
{
	return active;
}

void MSXInput::Update()
{		
	DWORD dwResult;

	dwResult = XInputGetState( 0, &state );
	
	if( dwResult == ERROR_SUCCESS )
	{
		Update_LeftThumbstick(state);
		Update_RightThumbstick(state);
		Update_Triggers(state);
	}
}

void MSXInput::GetThumbStick(Analog &analog, SIDE btn)
{
	switch(btn)
	{ 
		case LEFT:
			analog = this->lThumAnalog;
			return;
		case RIGHT:
			analog = this->rThumAnalog;
			return; 
	}
}

void MSXInput::GetTrigger(BYTE &magnitude, SIDE btn)
{
	switch(btn)
	{
		case LEFT:
			magnitude = this->lTrigger;
			return;
		case RIGHT:
			magnitude = this->rTrigger;
			return;
	}
}

bool MSXInput::GetButtonState(BUTTON btn)
{
	switch(btn) {
		case BUTTON_A: 
			return (state.Gamepad.wButtons & XINPUT_GAMEPAD_A) ? true : false;
		case BUTTON_B: 
			return (state.Gamepad.wButtons & XINPUT_GAMEPAD_B) ? true : false;
		case BUTTON_X: 
			return (state.Gamepad.wButtons & XINPUT_GAMEPAD_X) ? true : false;
		case BUTTON_Y: 
			return (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) ? true : false;
		case BUTTON_UP: 
			return (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) ? true : false;
		case BUTTON_DOWN: 
			return (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) ? true : false;
		case BUTTON_LEFT: 
			return (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) ? true : false;
		case BUTTON_RIGHT: 
			return (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) ? true : false;
	}

	return false;
}

void MSXInput::Update_LeftThumbstick(XINPUT_STATE state)
{
	float LX = state.Gamepad.sThumbLX;
	float LY = state.Gamepad.sThumbLY;

	//determine how far the controller is pushed
	float magnitude = sqrt(LX*LX + LY*LY);

	//determine the direction the controller is pushed
	float normalizedLX = LX / magnitude;
	float normalizedLY = LY / magnitude;

	float normalizedMagnitude = 0;

	//check if the controller is outside a circular dead zone
	if (magnitude > INPUT_DEADZONE)
	{
	  //clip the magnitude at its expected maximum value
	  if (magnitude > 32767) magnitude = 32767;
  
	  //adjust magnitude relative to the end of the dead zone
	  magnitude -= INPUT_DEADZONE;

	  //optionally normalize the magnitude with respect to its expected range
	  //giving a magnitude value of 0.0 to 1.0
	  normalizedMagnitude = magnitude / (32767 - INPUT_DEADZONE);
	}
	else //if the controller is in the deadzone zero out the magnitude
	{
		magnitude = 0.0;
		normalizedMagnitude = 0.0;
		normalizedLX = 0;
		normalizedLY = 0;
	}
	
	normalizedLX = normalizedMagnitude * normalizedLX;
	normalizedLY = normalizedMagnitude * normalizedLY;	

	Bound(normalizedLX, -1.0, 1.0);
	Bound(normalizedLY, -1.0, 1.0);

	lThumAnalog = Analog(normalizedMagnitude, normalizedLX, normalizedLY);
}

void MSXInput::Update_RightThumbstick(XINPUT_STATE state)
{
	float RX = state.Gamepad.sThumbRX;
	float RY = state.Gamepad.sThumbRY;

	//determine how far the controller is pushed
	float magnitude = sqrt(RX*RX + RY*RY);

	//determine the direction the controller is pushed
	float normalizedRX = RX / magnitude;
	float normalizedRY = RY / magnitude;

	float normalizedMagnitude = 0;

	//check if the controller is outside a circular dead zone
	if (magnitude > INPUT_DEADZONE)
	{
	  //clip the magnitude at its expected maximum value
	  if (magnitude > 32767) magnitude = 32767;
  
	  //adjust magnitude relative to the end of the dead zone
	  magnitude -= INPUT_DEADZONE;

	  //optionally normalize the magnitude with respect to its expected range
	  //giving a magnitude value of 0.0 to 1.0
	  normalizedMagnitude = magnitude / (32767 - INPUT_DEADZONE);
	}
	else //if the controller is in the deadzone zero out the magnitude
	{
		magnitude = 0.0;
		normalizedMagnitude = 0.0;
	}

	rThumAnalog = Analog(normalizedMagnitude, normalizedRX, normalizedRY);
}

void MSXInput::Update_Triggers(XINPUT_STATE state)
{	
	lTrigger = state.Gamepad.bLeftTrigger;
	rTrigger = state.Gamepad.bRightTrigger;
}

void MSXInput::Vibrate(int contNum = 0, int left = 65535, int right = 65535)
{
	    XINPUT_VIBRATION vibration;
	    ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
	    vibration.wLeftMotorSpeed = left;
	    vibration.wRightMotorSpeed = right;
	    XInputSetState( contNum, &vibration);
}

void MSXInput::Bound(float &value, float min, float max)
{
	if(value < min)
		value = min;

	if(value > max)
		value = max;
}

/*/ ____________
///
///	Input
/*/

bool Input::Initialize()
{
	XInput_.Initialize();

	return true;
}

bool Input::Shutdown()
{
	return true;
}

bool Input::Update()
{
	XInput_.Update();

	return true;
}

MSXInput& Input::GetXbox()
{
	return XInput_;
}

