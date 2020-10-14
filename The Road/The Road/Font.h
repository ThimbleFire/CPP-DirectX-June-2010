#ifndef _FONT_H_
#define _FONT_H_

#include <iostream>
#include <fstream>
#include <string>

#include "directX.h"
#include "SourceRectangle.h"

#include "Texture2D.h"

using namespace std;

class Font
{

public:
	Font();

	~Font();

	bool Initialize
	(
		_In_ ID3D11Device*					DEVICE,
		_In_ std::string					FONT_FILE_PATH,
		_In_ unsigned int					TEXTURE_WIDTH,
		_In_ unsigned int					TEXTURE_HEIGHT,
		_In_ unsigned int					SPACING
	);

	//returns a 2D vector containing the width and height of string
	D3DXVECTOR2 MeasureString
	(
		_In_ string							MESSAGE
	);

	void Shutdown
	(
		void
	);
	
	SourceRectangle GetFontData
	(
		char symbol
	);

	//returns the number of symbols stored in the font 
	unsigned int Length
	(
		void
	);

	Texture2D* GetTexture2D
	(
		void
	);

	unsigned int GetSpacing
	(
		void
	);

	unsigned int GetTextureHeight
	(
		void
	);

private:

	characterData									characterData_[256];

	Texture2D*										texture_;
	
	unsigned int									symbolCount_;
	
	unsigned int									spacing_;
	
	unsigned int									textureWidth;
	
	unsigned int									textureHeight;

};

#endif