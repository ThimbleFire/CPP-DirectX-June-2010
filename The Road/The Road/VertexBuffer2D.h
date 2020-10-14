#ifndef _VertexBuffer2D_H_
#define _VertexBuffer2D_H_

#include <D3D11.h>
#include <D3DX10math.h>

#include "directX.h"
#include "Texture2D.h"
#include "TextureShader.h"
#include "SourceRectangle.h"

class VertexBuffer2D
{
public:

	VertexBuffer2D
	(
		void
	);

	~VertexBuffer2D
	(
		void
	);

	void Shutdown
	(
		void
	);

	int GetIndexCount
	(
		void
	);

	ID3D11ShaderResourceView* GetTexture
	(
		void
	);

	bool Initialize
	(
		_In_ ID3D11Device*									DEVICE,
		_In_ unsigned int									SCREEN_WIDTH, 
		_In_ unsigned int									SCREEN_HEIGHT
	);

	bool Render
	(
		_In_ ID3D11DeviceContext*					DEVICE_CONTEXT,
		_In_ float									POSITION_ON_SCREEN_X,
		_In_ float									POSITION_ON_SCREEN_Y,
		_In_ Texture2D*								TEXTURE,
		_In_ SourceRectangle						SOURCE_RECTANGLE,
		_In_ D3DXMATRIX								WORLD_MATRIX,
		_In_ D3DXMATRIX								VIEW_MATRIX,
		_In_ D3DXMATRIX								ORTHO_MATRIX,
		_In_ D3DXVECTOR4							COLOUR
	);

	void Translate
	(
			_In_ float								X,
			_In_ float								Y,
			_In_ float								Z
	);

	void Rotate
	(
			_In_ float								ANGLE
	);
	
	bool UpdateBuffers
	(
		_In_ ID3D11DeviceContext					*DEVICE_CONTEXT,
		_In_ float									POSITION_ON_SCREEN_X,
		_In_ float									POSITION_ON_SCREEN_Y,
		_In_ SourceRectangle						SOURCE_RECTANGLE,
		_In_ float									TEXTURE_WIDTH,
		_In_ float									TEXTURE_HEIGHT
	);

private:

	struct VertexType
	{
		D3DXVECTOR3									position;
		D3DXVECTOR2									texture;
	};

	bool InitializeShader
	(
		_In_ ID3D11Device*							DEVICE
	);

	bool InitializeBuffers
	(
		_In_ ID3D11Device*							DEVICE
	);

	void RenderBuffers
	(
		_In_ ID3D11DeviceContext*					DEVICE_CONTEXT
	);
	
private:

	unsigned int									m_vertexCount;
	unsigned int									m_indexCount;

	ID3D11Buffer*									m_vertexBuffer;
	ID3D11Buffer*									m_indexBuffer;
	
	D3DXMATRIX										matrix_translation_;
	D3DXMATRIX										matrix_rotation_;

	unsigned int									screenWidth_;
	unsigned int									screenHeight_;
	
	float											oldX;
	float											oldY;
};

#endif