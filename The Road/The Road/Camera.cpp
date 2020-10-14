#include "Camera.h"

Camera::~Camera()
{

}

void Camera::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
}

void Camera::SetSpeed(float s)
{
	m_speed = s;
}

D3DXVECTOR3 Camera::GetPosition()
{
	return D3DXVECTOR3(m_positionX, m_positionY, m_positionZ);
}

void Camera::GetViewMatrix(D3DXMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
}

void Camera::SetRotation(float x, float y, float z)
{
	m_rotationX += x;
	if(m_rotationX < -90) m_rotationX = -90;
	if(m_rotationX >  90) m_rotationX =  90;
	m_rotationY += y;
	//if(m_rotationY <   0) m_rotationY = 360;
	//if(m_rotationY > 360) m_rotationY =   0;
	m_rotationZ += z;


	//BUG HERE! x will forever increase! We need to stop this function from recieving values greater than 360 / 90

}

D3DXVECTOR3 Camera::GetRotation()
{
	return D3DXVECTOR3(m_rotationX, m_rotationY, m_rotationZ);
}

void Camera::Update()
{

}

void Camera::Render()
{
	D3DXMATRIX rotationMatrix;

   						         float Pi = 3.14159265359f;
	float pitch = (m_rotationX*2.5f) * Pi / 180;
	float yaw   = (m_rotationY*2.5f) * Pi / 180;
	float roll  = (m_rotationZ*2.5f) * Pi / 180;

	//get our rotation matrix
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);
	
	//1.0 is the Z axis...
	D3DXVECTOR3 lookAt = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	
	//1.0 is the Y axis...
	D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);
	
	//1.0 is the X axis, we apply the direction with the rotaton matrix
	D3DXVECTOR3 right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	D3DXVec3TransformCoord(&right, &right, &rotationMatrix);

	//m_positionX is between 0.0 and 1.0 according to the X magnitude of our controller
	m_position += (m_positionX * right) * m_speed;
	m_position += (m_positionZ *lookAt) * m_speed;

	m_positionX = 0;
	m_positionZ = 0;

	if(!NOCLIP_ENABLED)
		m_position.y = 0;

	lookAt = m_position + lookAt;

	D3DXMatrixLookAtLH(&m_viewMatrix, &m_position, &lookAt, &up);
}
