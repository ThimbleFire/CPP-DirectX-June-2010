#ifndef _TEXTURESHADER_H_
#define _TEXTURESHADER_H_

#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>
using namespace std;

class TextureShader
{
	ID3D11VertexShader*		  vertexShader_;
	ID3D11PixelShader*		   pixelShader_;
	ID3D11InputLayout*		        layout_;
	ID3D11Buffer*			constantBuffer_;
	ID3D11SamplerState*		   sampleState_;

	struct ConstantBufferType {

		D3DXMATRIX 
				    worldMatrix,
				    viewMatrix, 
				    projectionMatrix;
	};

	struct PixelBufferType {

		D3DXVECTOR4 
					pixelColor;
	};

public:
	
	TextureShader()
	{

	}
	
	~TextureShader()
	{

	}

	void Shutdown
	(
	  void
	);

	bool Initialize(
		_In_ ID3D11Device*
	);
	
	bool Render
	(
		_In_ ID3D11DeviceContext*, 
		_In_ int, 
		_In_ D3DXMATRIX, 
		_In_ D3DXMATRIX, 
		_In_ D3DXMATRIX, 
		_In_ ID3D11ShaderResourceView*, 
		_In_ D3DXVECTOR4
	);

private:

	bool RenderShader(_In_ ID3D11DeviceContext* deviceContext, 
					  _In_ int indexCount);

	bool InitializeShader
	(
		_In_ ID3D11Device*
	);

	bool SetShaderParameter
	(
		_In_ ID3D11DeviceContext*, 
		_In_ D3DXMATRIX, 
		_In_ D3DXMATRIX, 
		_In_ D3DXMATRIX, 
		_In_ ID3D11ShaderResourceView*, 
		_In_ D3DXVECTOR4
	);

private:

	ID3D11Buffer* pixelBuffer_;
};

#endif