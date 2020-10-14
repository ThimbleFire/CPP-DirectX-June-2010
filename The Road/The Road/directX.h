#ifndef _DIRECTX_H_
#define _DIRECTX_H_

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx10math.h>

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.01f;

class DirectX
{

public:

	/* Specify the return type of the matrix get method */
	enum eRasterState
	{
		SOLID,
		WIRE_FRAME
	};

	/* Specify the return type of the matrix get method */
	enum eMatrices
	{
		PROJECTION,
		WORLD,
		ORTHOGRAPHIC
	};
	
	/* Specify active or inactive state of an interface */
	enum eState
	{
		ENABLED_ALPHA,
		ENABLED,
		DISABLED
	};


public:
	
	bool Initialize
	(
		_In_ unsigned int			SCREEN_WIDTH,
		_In_ unsigned int			SCREEN_HEIGHT,
		_In_ HWND					WINDOW_HANDLE
	);
	
	bool BeginScene
	(
		_In_ unsigned __int8		X, 
		_In_ unsigned __int8		Y, 
		_In_ unsigned __int8		Z, 
		_In_ unsigned __int8		A
	);

	bool BeginScene
	(
		_In_ D3DXVECTOR4			COLOUR
	);
	
	bool EndScene
	(
									void
	);

	
	bool Shutdown
	(
									void
	);

	ID3D11Device* GetDevice
	(
									void
	);

	ID3D11DeviceContext* GetDeviceContext
	(
									void
	);

	IDXGISwapChain* GetSwapChain
	(
									void
	);

	void GetMatrix
	(
		_In_  eMatrices				TYPE_OF,
		_Out_ D3DXMATRIX&			MATRIX
	);
	
	void GetVideoCardInfo
	(
		_In_  char*					CARD_NAME,
		_Out_ int&					MEMORY
	);

	D3D11_VIEWPORT GetViewport
	(
		void
	);

	void SetAlphaBlending
	(
		_In_ eState					STATE
	);

	void SetZBuffer
	(
		_In_ eState					STATE
	);

	void SetRasterState
	(
		_In_ eRasterState			TYPE_OF
	);

private:
	
	D3D11_VIEWPORT viewport;

	//device
	ID3D11Device*				device_;
	IDXGISwapChain*				swapChain_;
	ID3D11DeviceContext*		deviceContext_;

	//rasterStates	
	ID3D11RasterizerState*		rasterState_Solid_CounterClockwise_;
	ID3D11RasterizerState*		rasterState_Solid_;
	ID3D11RasterizerState*		rasterState_Wire_;

	//depth stencil resources
	ID3D11DepthStencilState*	depthStencilState_Enabled_;
	ID3D11DepthStencilState*	depthStencilState_Disabled_;
	ID3D11Texture2D*			depthStencilBuffer_;
	ID3D11DepthStencilView*		depthStencilView_;

	//backbuffer
	ID3D11RenderTargetView*		renderTargetView_;

	//blend states
	ID3D11BlendState*			alphaBlendingState_Enabled_;
	ID3D11BlendState*			alphaBlendingState_Enabled_Alpha_;
	ID3D11BlendState*			alphaBlendingState_Disabled_;

	//video card name and memory
	int							videoCardMemory_;
	char						videoCardDescription_[128];

	//matrices
	D3DXMATRIX					matrix_Projection_;
	D3DXMATRIX					matrix_World_;
	D3DXMATRIX					matrix_Orthographic_;

};

#endif