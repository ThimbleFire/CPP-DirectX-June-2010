#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <D3DX10math.h>

class Camera
{

public:
	Camera()
	{
			m_positionX = 0.0f;
			m_positionY = 0.0f;
			m_positionZ = -10.0f;

			m_rotationX = 0.0f;
			m_rotationY = 0.0f;
			m_rotationZ = 0.0f;

			NOCLIP_ENABLED = true;

			m_speed = 0.10f;

			m_position = D3DXVECTOR3(0.0f, 0.0f, -10.0f);

			SetRotation(0.0f, 0.0f, 0.0f);
	}
	~Camera();

	void SetPosition
		(
		_In_ float, 
		_In_ float, 
		_In_ float
		);

	void SetRotation
		(
		_In_ float, 
		_In_ float, 
		_In_ float
		);
	
	void SetSpeed
		(
		_In_ float
		);

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();

	void Render
		(
		);

	void GetViewMatrix(D3DXMATRIX&);

	void Update();

private:
	bool NOCLIP_ENABLED;

	//camera properties
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	float m_x_magnitude;
	float m_y_magnitude;

	D3DXMATRIX m_viewMatrix;

	float m_speed;
	D3DXVECTOR3 m_position;
};

#endif