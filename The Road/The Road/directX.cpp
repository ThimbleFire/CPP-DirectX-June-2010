#include "directX.h"

bool DirectX::Initialize(unsigned int screenWidth_, unsigned int screenHeight_, HWND hwnd_)
{
				 /* number of adapter outputs */
	unsigned int numModes, 
				 /* monitor refresh rate numerator */
				 numerator, 
				 /* monitor refresh rate denominator  */
				 denominator,
				 /* video card description length */
				 stringLength;

				 /* Allows us to enumerate adapters (obtain a list of video cards) */

	IDXGIFactory*		factory;

				 /* We store the first of our adapters in this interface */

	IDXGIAdapter*		adapter;

				 /* We dump the adapters information in this output interface  */ 

	IDXGIOutput*		adapterOutput;

				 /* We create a display mode description using the adapters information */

	DXGI_MODE_DESC*		displayModeDesc;

				 /* We output the adapter description in this adapter description  */

	DXGI_ADAPTER_DESC	adapterDesc;

	HRESULT result;

				/* Create our factory */

	if(FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory))) return false;
	
				/* Get a list of graphics cards */

	if(FAILED(factory->EnumAdapters(0, &adapter))) return false;
	
				/* Get the adapters output */

	if(FAILED(adapter->EnumOutputs(0, &adapterOutput))) return false;
	
				/* Get a list of display modes */

	if(FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL))) return false;
	
	displayModeDesc = new DXGI_MODE_DESC[numModes];
	
				/* Get a description for each display mode */

	if(FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeDesc))) return false;
	
				/* determine the display device' refresh rate */

	for(unsigned int i = 0; i < numModes; i++)
	if(displayModeDesc[i].Width == screenWidth_ && displayModeDesc[i].Height == screenHeight_) {
		numerator =   displayModeDesc[i].RefreshRate.Numerator;
		denominator = displayModeDesc[i].RefreshRate.Denominator;
	}

	/* Get the adapter description */
	
	if(FAILED(adapter->GetDesc(&adapterDesc))) return false;

	/* Create a description for the video card */

	videoCardMemory_ = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	/* Convert the memory data type to text oriented so that it can be written to screen */

	if( wcstombs_s(&stringLength, videoCardDescription_, 128, adapterDesc.Description, 128) != 0) return false;	

	delete[] displayModeDesc;
	displayModeDesc = nullptr;

	adapterOutput->Release();
	adapterOutput = nullptr;

	adapter->Release();
	adapter = nullptr;

	factory->Release();
	factory = nullptr;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;

	//______________________________________________________________________________
	//
	// Swapchain
	//
	//	Describe our swapchain
	//

    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	//by setting the height and width to 0 the swapchain automatically sets the size to the current window resolution
    swapChainDesc.BufferDesc.Width = 0;
    swapChainDesc.BufferDesc.Height = 0;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

    swapChainDesc.BufferCount = 1;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd_;
	swapChainDesc.Flags = 0;
	swapChainDesc.Windowed = true;

	if(FULL_SCREEN)
	{
		swapChainDesc.Windowed = false;
	}

	featureLevel = D3D_FEATURE_LEVEL_11_0;

	result = D3D11CreateDeviceAndSwapChain(
		NULL, 
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0, 
		&featureLevel, 
		1, 
		D3D11_SDK_VERSION, 
		&swapChainDesc, 
		&swapChain_,
		&device_, 
		NULL,
		&deviceContext_);

	if(FAILED(result))
		return false;
	
	//______________________________________________________________________________
	//
	// Create our render target view
	//
	//	and remove backBufferPtr to prevent memory leaks
	//

	result = swapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	
	if(FAILED(result))
		return false;

	result = device_->CreateRenderTargetView(backBufferPtr, NULL, &renderTargetView_);
	
	if(FAILED(result))
		return false;

	backBufferPtr->Release();
	backBufferPtr = nullptr;

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		
	//______________________________________________________________________________
	//
	// Depth Stencil Buffer
	//
	//	Input: D3D11_TEXTURE2D_DESC
	//	Output: ID3D11Texture2D*
	//
	//	Allows us to render in the Z axis.
	//

	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = screenWidth_;
	depthBufferDesc.Height = screenHeight_;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	result = device_->CreateTexture2D(&depthBufferDesc, NULL, &depthStencilBuffer_);

	if(FAILED(result))
		return false;
	
	//______________________________________________________________________________
	//
	// Depth Stencil State (Enabled)
	//
	//	Input: D3D11_DEPTH_STENCIL_DESC	
	//	Output: ID3D11DepthStencilState*
	//
	//	A state in which the Z buffer is enabled.
	//

	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.StencilEnable =				TRUE;
	depthStencilDesc.DepthEnable =					TRUE;
	depthStencilDesc.DepthWriteMask =				D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc =					D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilReadMask =				D3D11_DEFAULT_STENCIL_READ_MASK;
	depthStencilDesc.StencilWriteMask =				D3D11_DEFAULT_STENCIL_WRITE_MASK;
	depthStencilDesc.FrontFace.StencilFailOp =		D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp =	D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp =		D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc =		D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp =		D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp =	D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp =		D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc =			D3D11_COMPARISON_ALWAYS;
	
	result = device_->CreateDepthStencilState(&depthStencilDesc, &depthStencilState_Enabled_);

	if(FAILED(result))
		return false;
	
	//______________________________________________________________________________
	//
	// Depth Stencil State (Disabled)
	//
	//	Input: D3D11_DEPTH_STENCIL_DESC
	//	Output: ID3D11DepthStencilState*
	//
	//	A state in which the Z buffer is disabled.
	//

	depthStencilDesc.DepthEnable = false;

	result = device_->CreateDepthStencilState(&depthStencilDesc, &depthStencilState_Disabled_);
	
	if(FAILED(result))
		return false;
	
	//______________________________________________________________________________
	//
	// Depth Stencil View
	//
	//	Input: ID3D11Texture2D*, D3D11_DEPTH_STENCIL_DESC
	//	Output: ID3D11DepthStencilView*
	//
	//	Sets the screen render target, limitting the area of rendering (stenciling).
	//

	deviceContext_->OMSetDepthStencilState(depthStencilState_Enabled_, 1);
	
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	result = device_->CreateDepthStencilView(depthStencilBuffer_, &depthStencilViewDesc, &depthStencilView_);

	deviceContext_->OMSetRenderTargets(1, &renderTargetView_, depthStencilView_);

	D3D11_BLEND_DESC blendStateDescription;

	//______________________________________________________________________________
	//
	// BlendState (enabled)
	//
	//	Input: D3D11_BLEND_DESC
	//	Output: ID3D11BlendState*
	//
	//	Enables the combination of output values from a pixel shader.
	//

	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));
	
	blendStateDescription.AlphaToCoverageEnable =                   FALSE;
	blendStateDescription.IndependentBlendEnable =                  FALSE;
	blendStateDescription.RenderTarget[0].BlendEnable =             TRUE;
	blendStateDescription.RenderTarget[0].BlendOp =                 D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].BlendOpAlpha =            D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlend =                D3D11_BLEND_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].DestBlend =               D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha =           D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha =          D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask  =  D3D11_COLOR_WRITE_ENABLE_ALL;

	result = device_->CreateBlendState(&blendStateDescription, &alphaBlendingState_Enabled_);

	if(FAILED(result))
		return false;
	
	//______________________________________________________________________________
	//
	// BlendState (alpha)
	//
	//	Input: D3D11_BLEND_DESC
	//	Output: ID3D11BlendState*
	//
	//	Enables the combination of output values from a pixel shader.
	//

	blendStateDescription.AlphaToCoverageEnable =                   TRUE;
	blendStateDescription.IndependentBlendEnable =                  FALSE;
	blendStateDescription.RenderTarget[0].BlendEnable =             TRUE;
	blendStateDescription.RenderTarget[0].BlendOp =                 D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].BlendOpAlpha =            D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlend =                D3D11_BLEND_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].DestBlend =               D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha =           D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha =          D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask  =  D3D11_COLOR_WRITE_ENABLE_ALL;

	result = device_->CreateBlendState(&blendStateDescription, &alphaBlendingState_Enabled_Alpha_);

	if(FAILED(result))
		return false;

	//______________________________________________________________________________
	//
	// BlendState (disabled)
	//
	//	Input: D3D11_BLEND_DESC
	//	Output: ID3D11BlendState*
	//
	//	Disables the combination of output values from a pixel shader.
	//
	
	blendStateDescription.AlphaToCoverageEnable =                   FALSE;
	blendStateDescription.IndependentBlendEnable =                  FALSE;
	blendStateDescription.RenderTarget[0].BlendEnable =             FALSE;
	blendStateDescription.RenderTarget[0].BlendOp =                 D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].BlendOpAlpha =            D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlend =                D3D11_BLEND_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].DestBlend =               D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha =           D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha =          D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask  =  D3D11_COLOR_WRITE_ENABLE_ALL;
	
	result = device_->CreateBlendState(&blendStateDescription, &alphaBlendingState_Disabled_);

	if(FAILED(result))
		return false;

	D3D11_RASTERIZER_DESC rasterDesc;
	
	//______________________________________________________________________________
	//
	// Rasterizer State
	//
	//	Input: D3D11_RASTERIZER_DESC
	//	Output: ID3D11RasterizerState*
	//
	//	Converts our vector graphics into pixels, allowing us to render 3D objects to screen.
	//

	ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));
	
	rasterDesc.AntialiasedLineEnable =	FALSE;
	rasterDesc.CullMode =				D3D11_CULL_BACK;
	rasterDesc.DepthBias =				0;
	rasterDesc.DepthBiasClamp =			0.0f;
	rasterDesc.DepthClipEnable =		TRUE;
	rasterDesc.FillMode =				D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise =	FALSE;
	rasterDesc.MultisampleEnable =		FALSE;
	rasterDesc.ScissorEnable =			FALSE;
	rasterDesc.SlopeScaledDepthBias =	0.0f;

	//create the standard raster state
	result = device_->CreateRasterizerState(&rasterDesc, &rasterState_Solid_);
	
	if(FAILED(result))
		return false;

	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;

	//create the wireframe raster state
	result = device_->CreateRasterizerState(&rasterDesc, &rasterState_Wire_);
	
	if(FAILED(result))
		return false;

	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = TRUE;

	result = device_->CreateRasterizerState(&rasterDesc, &rasterState_Solid_CounterClockwise_);
	
	if(FAILED(result))
		return false;

	deviceContext_->RSSetState(rasterState_Solid_);
	//create the standard raster state counterclockwise

	if(FAILED(result))
		return false;

	//______________________________________________________________________________
	//
	// Viewport
	//
	//	In/Out: D3D11_VIEWPORT;
	//
	//	Translates our 3D scene into a 2D viewport space.
	//

	viewport.Width =  (float)screenWidth_;
	viewport.Height = (float)screenHeight_;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	deviceContext_->RSSetViewports(1, &viewport);
	
	//______________________________________________________________________________
	//
	// Matrices
	//
	//	Creating our world, perspective and orthographic matrixes.
	//
	//	Our camera is responsible for our view matrix.
	//

	float fieldOfView = (float)D3DX_PI / 4.0f;
	float screenAspect = (float)screenWidth_ / (float)screenHeight_;

	D3DXMatrixPerspectiveFovLH(&matrix_Projection_, fieldOfView, screenAspect, SCREEN_NEAR, SCREEN_DEPTH);

	D3DXMatrixIdentity(&matrix_World_);

	D3DXMatrixOrthoLH(&matrix_Orthographic_, (float)screenWidth_, (float)screenHeight_, SCREEN_NEAR, SCREEN_DEPTH);

	return true;
}

bool DirectX::Shutdown()
{
	/*___________________________________________*/
	//
	//	Remember to shutdown in the opposite order that we initialized
	//	to ensure no interfaces depending on other interfaces are left
	//	empty.
	//

	//RASTER STATES
	
	 if(rasterState_Solid_) {
		rasterState_Solid_->Release();
		rasterState_Solid_ = nullptr;
	}

	 if(rasterState_Wire_) {
		rasterState_Wire_->Release();
		rasterState_Wire_ = nullptr;
	 }

	 if(rasterState_Solid_CounterClockwise_) {
		rasterState_Solid_CounterClockwise_->Release();
		rasterState_Solid_CounterClockwise_ = nullptr;
	 }
	
	//BLENDING STATES

	 if(alphaBlendingState_Disabled_) {
		alphaBlendingState_Disabled_->Release();
		alphaBlendingState_Disabled_ = nullptr;
	 }

	 if(alphaBlendingState_Enabled_ ) {
		alphaBlendingState_Enabled_->Release();
		alphaBlendingState_Enabled_ = nullptr;
	}

	 if(alphaBlendingState_Enabled_Alpha_ ) {
		alphaBlendingState_Enabled_Alpha_->Release();
		alphaBlendingState_Enabled_Alpha_ = nullptr;
	}

	//STENCIL STATES

	 if(depthStencilBuffer_) {
		depthStencilBuffer_->Release();
		depthStencilBuffer_ = nullptr;
	}

	 if(depthStencilView_) {
		depthStencilView_->Release();
		depthStencilView_ = nullptr;
	}

	 if(depthStencilState_Disabled_) {
		depthStencilState_Disabled_->Release();
		depthStencilState_Disabled_ = nullptr;
	}

	 if(depthStencilState_Enabled_) {
		depthStencilState_Enabled_->Release();
		depthStencilState_Enabled_ = nullptr;
	 }
	 if(renderTargetView_) {
		renderTargetView_->Release();
		renderTargetView_ = nullptr;
	 }

	//DEVICE & SWAPCHAIN

	if(swapChain_) {
		/* Must disable fullscreen else program will crash */
		swapChain_->SetFullscreenState(false, NULL);
		swapChain_->Release();
		swapChain_ = nullptr;
	}

	if(deviceContext_) {
		deviceContext_->Release();
		deviceContext_ = nullptr;
	}

	if(device_) {
		device_->Release();
		device_ = nullptr;
	}

	return true;
}

bool DirectX::BeginScene(unsigned __int8 r, unsigned __int8 g, unsigned __int8 b, unsigned __int8 a)
{
					   // 1 / 256 = 0.00390625f
	float color[4] = {  (float)(r * 0.00390625f),
						(float)(g * 0.00390625f),
						(float)(b * 0.00390625f),
						(float)(a * 0.00390625f)
					 };

	deviceContext_->ClearRenderTargetView(renderTargetView_, color);

	deviceContext_->ClearDepthStencilView(depthStencilView_, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return true;
}

bool DirectX::BeginScene(D3DXVECTOR4 colour)
{
	return DirectX::BeginScene(colour.x, colour.y, colour.z, colour.w);
}

bool DirectX::EndScene()
{
	HRESULT result;

	if(VSYNC_ENABLED)
		result = swapChain_->Present(1, 0);
	else
		result = swapChain_->Present(0, 0);

	//Presents a rendered image to the user.
	//Note  Starting with Direct3D 11.1, we recommend not to use Present anymore 
	//to present a rendered image. Instead, use IDXGISwapChain1::Present1.

	if(FAILED(result))
		return false;

	return true;
}

ID3D11Device* DirectX::GetDevice()
{
	return device_;
}

ID3D11DeviceContext* DirectX::GetDeviceContext()
{
	return deviceContext_;
}

IDXGISwapChain* DirectX::GetSwapChain()
{
	return swapChain_;
}

void DirectX::GetMatrix(eMatrices mType, D3DXMATRIX &matrix)
{
	switch(mType)
	{
	case DirectX::PROJECTION:
		matrix = matrix_Projection_;
		return;
	case DirectX::WORLD:
		matrix = matrix_World_;
		return;
	case DirectX::ORTHOGRAPHIC:
		matrix = matrix_Orthographic_;
		return;
	}
}

void DirectX::GetVideoCardInfo(char* cardName, int &memory)
{
	strcpy_s(cardName, 128, videoCardDescription_);
	memory = videoCardMemory_;
}

D3D11_VIEWPORT DirectX::GetViewport()
{
	return viewport;
}

void DirectX::SetZBuffer(eState state)
{
	switch(state)
	{
	case DirectX::ENABLED:
	deviceContext_->OMSetDepthStencilState(depthStencilState_Enabled_, 1);
		return;
	case DirectX::DISABLED:
	deviceContext_->OMSetDepthStencilState(depthStencilState_Disabled_, 1);
		return;
	}
}

void DirectX::SetAlphaBlending(eState state)
{
    float blendFactor[] = {0.75f, 0.75f, 0.75f, 1.0f};

	switch(state)
	{
	case DirectX::ENABLED:
		deviceContext_->OMSetBlendState(alphaBlendingState_Enabled_, blendFactor, 0xffffffff);
		return;
	case DirectX::ENABLED_ALPHA:
		deviceContext_->OMSetBlendState(alphaBlendingState_Enabled_Alpha_, blendFactor, 0xffffffff);
		return;
	case DirectX::DISABLED:
		deviceContext_->OMSetBlendState(alphaBlendingState_Disabled_, blendFactor, 0xffffffff);
		return;
	}
}

void DirectX::SetRasterState(eRasterState state)
{
	switch(state)
	{
	case DirectX::SOLID:
		return;
	case DirectX::WIRE_FRAME:
		return;
	}
}