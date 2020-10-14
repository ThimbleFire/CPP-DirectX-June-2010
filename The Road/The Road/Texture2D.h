#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <D3D11.h>
#include <D3DX11tex.h>

#include <iostream>
#include <string>

class Texture2D
{
public:

	Texture2D();
	~Texture2D();

	bool Initialize
	(
		_In_ ID3D11Device*, 
		_In_ std::string,
		_In_ int textureWidth,
		_In_ int textureHeight
	);

	void Shutdown
	(
		void
	);

	ID3D11ShaderResourceView* GetTexture2D
	(
		void
	);
	
	int GetWidth
	(
		void
	);

	int GetHeight
	(
		void
	);

private:
	ID3D11ShaderResourceView* m_texture;

	int textureWidth_;
	int textureHeight_;

};

#endif