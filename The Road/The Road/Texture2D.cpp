#include "Texture2D.h"

Texture2D::Texture2D()
{
	m_texture = nullptr;
}

Texture2D::~Texture2D()
{

}

void Texture2D::Shutdown()
{
	if(m_texture) {
		m_texture->Release();
		m_texture = nullptr;
	}
}

ID3D11ShaderResourceView* Texture2D::GetTexture2D()
{
	return m_texture;
}

int Texture2D::GetWidth()
{
	return textureWidth_;
}

int Texture2D::GetHeight()
{
	return textureHeight_;
}

bool Texture2D::Initialize(ID3D11Device* device, std::string path, int textureWidth, int textureHeight)
{
	HRESULT result;

	this->textureWidth_ = textureWidth;
	this->textureHeight_= textureHeight;

	path += ".png";

	result = D3DX11CreateShaderResourceViewFromFile(device, path.c_str(), NULL, NULL, &m_texture, NULL);

	if(FAILED(result))
		return false;

	return true;
}