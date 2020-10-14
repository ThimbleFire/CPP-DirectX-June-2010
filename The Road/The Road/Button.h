#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "Font.h"
#include "Texture2D.h"
#include "TextureShader.h"
#include "VertexBuffer2D.h"

#include <iostream>
#include <string>

using namespace std;

class Button
{
public:
	Button(std::string text);
	~Button();
	
	bool LoadContent(ID3D11Device* device, unsigned int screenWidth, unsigned int screenHeight);
	bool HandleInput();
	bool Render(ID3D11DeviceContext* deviceContext, float x, float y, bool selected, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX orthoMatrix);
	
	void Shutdown();

	std::string GetText();

private:

	bool LoadTexture(	  ID3D11Device* device);
	bool LoadFont(		  ID3D11Device* device, unsigned int screenWidth, unsigned int screenHeight);
	bool InitializeBuffer(ID3D11Device* device, unsigned int screenWidth, unsigned int screenHeight);
	bool InitializeShader(ID3D11Device* device);

private:

	//button text
	std::string text_;
	
	//shaders
	TextureShader*		textureShader_;

	//button
	Texture2D*			spriteButtonDown_;
	Texture2D*			spriteButtonUp_;
	VertexBuffer2D*		bufferButton_;

	//font
	Font*				fontObject_;
	VertexBuffer2D*		bufferFont_;
};

#endif