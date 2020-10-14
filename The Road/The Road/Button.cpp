#include "Button.h"

Button::Button(std::string text)
{
	this->text_ = text;

	textureShader_= nullptr;

	fontObject_   = nullptr;
	bufferFont_   = nullptr;

	bufferButton_ = nullptr;
	spriteButtonDown_ = nullptr;
	spriteButtonUp_   = nullptr;
}

Button::~Button()
{

}

bool Button::LoadContent(ID3D11Device* device, unsigned int screenWidth, unsigned int screenHeight)
{
	bool result = true;

	result = LoadTexture(device);

	if(!result)
		return false;

	result = LoadFont(device, screenWidth, screenHeight);

	if(!result)
		return false;

	result = InitializeBuffer(device, screenWidth, screenHeight);

	if(!result)
		return false;

	result = InitializeShader(device);

	if(!result)
		return false;

	return true;
}

bool Button::HandleInput()
{
	return true;
}

bool Button::Render(ID3D11DeviceContext* deviceContext, float x, float y, bool selected, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX orthoMatrix)
{
	bool result;

	float temp = 0.0;

	for(unsigned int i = 0; i < text_.length(); i++)
	{
		result = bufferFont_->Render(deviceContext, x+14.0f + temp, y+(spriteButtonUp_->GetHeight() / 2) - fontObject_->GetTexture2D()->GetHeight() / 2, fontObject_->GetTexture2D(), fontObject_->GetFontData(text_[i]), worldMatrix, viewMatrix, orthoMatrix, D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
			
		if(!result)
			return false;

		result = textureShader_->Render(deviceContext, bufferFont_->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, fontObject_->GetTexture2D()->GetTexture2D(), D3DXVECTOR4(0.6f, 0.6f, 0.6f, 1.0f));

		if(!result)
			return false;
	
		temp += ((fontObject_->GetFontData(text_[i]).W * fontObject_->GetTexture2D()->GetWidth()) + 1.0f);
	}

	if(!selected)
	{
		result = bufferButton_->Render(deviceContext, x, y, spriteButtonUp_, SourceRectangle(0, 0, 1, 1), worldMatrix, viewMatrix, orthoMatrix, D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));

		if(!result)
			return false;

		result = textureShader_->Render(deviceContext, bufferButton_->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, spriteButtonUp_->GetTexture2D(), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));

		if(!result)
			return false;
	}
	else
	{
		result = bufferButton_->Render(deviceContext, x, y, spriteButtonDown_, SourceRectangle(0, 0, 1, 1), worldMatrix, viewMatrix, orthoMatrix, D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));

		if(!result)
			return false;

		result = textureShader_->Render(deviceContext, bufferButton_->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, spriteButtonDown_->GetTexture2D(), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));

		if(!result)
			return false;
	}


	return true;
}

void Button::Shutdown()
{
	if(bufferFont_) { for(int i = 0; i < text_.length(); i++)
		bufferFont_->Shutdown();
		delete [] bufferFont_;
		bufferFont_ = nullptr;
	}

	if(fontObject_) {
		fontObject_->Shutdown();
		delete fontObject_;
		fontObject_ = nullptr;
	}

	if(spriteButtonDown_) {
		spriteButtonDown_->Shutdown();
		delete spriteButtonDown_;
		spriteButtonDown_ = nullptr;
	}

	if(spriteButtonUp_) {
		spriteButtonUp_->Shutdown();
		delete spriteButtonUp_;
		spriteButtonUp_ = nullptr;
	}

	if(textureShader_) {
		textureShader_->Shutdown();
		delete textureShader_;
		textureShader_ = nullptr;
	}

	if(bufferButton_) {
		bufferButton_->Shutdown();
		delete bufferButton_;
		bufferButton_ = nullptr;
	}
}

std::string Button::GetText()
{
	return text_;
}

bool Button::LoadTexture(ID3D11Device* device)
{
	bool result;

			 spriteButtonDown_ = new Texture2D;
	result = spriteButtonDown_->Initialize(device, "../The Road/content/textures/btn_down", 174, 28);

	if(!result)
		return false;

			 spriteButtonUp_ = new Texture2D;
	result = spriteButtonUp_->Initialize(device, "../The Road/content/textures/btn_up", 174, 28);

	if(!result)
		return false;

	return true;
}

bool Button::LoadFont(ID3D11Device* device, unsigned int screenWidth, unsigned int screenHeight)
{
	bool result;

				 fontObject_ = new Font;
		result = fontObject_->Initialize(device, "../The Road/content/fonts/Small_Pixel_S9_Embosed", 414, 12, 2);

	if(!result)
		return false;

			 bufferFont_ = new VertexBuffer2D[text_.length()];

			 for(unsigned int i = 0; i < text_.length(); i++)
			 {
				 result = bufferFont_->Initialize(device, screenWidth, screenHeight);
		     if(!result)
				 return false;
			 }

	return true;
}

bool Button::InitializeBuffer(ID3D11Device* device, unsigned int screenWidth, unsigned int screenHeight)
{
	bool result;

			 bufferButton_ = new VertexBuffer2D;
	result = bufferButton_->Initialize(device, screenWidth, screenHeight);

	if(!result)
		return false;

	return true;
}

bool Button::InitializeShader(ID3D11Device* device)
{
	bool result;

			 textureShader_ = new TextureShader;
	result = textureShader_->Initialize(device);

	if(!result)
		return false;

	return true;
}

