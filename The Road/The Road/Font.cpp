#include "Font.h"

Font::Font()
{

}

Font::~Font()
{

}

bool Font::Initialize(ID3D11Device* device, std::string fontPath, unsigned int textureWidth, unsigned int textureHeight, unsigned int spacing)
{
	bool result;

	this->textureWidth = textureWidth;

	this->textureHeight = textureHeight;

	this->spacing_ = spacing;

			 texture_ = nullptr;
			 texture_ = new Texture2D;
	result = texture_->Initialize(device, fontPath, textureWidth, textureHeight);

	if(!result)
		return false;

	ifstream fin;		 	
			 fin.open(fontPath + ".txt");

	if(fin.fail())
		return false;

	fin >> symbolCount_;

	if(!characterData_)
		return false;
	
	for(unsigned int i = 0; i < symbolCount_; i++)
	{
		//w is the width which we don't read
		float w;

		fin >> characterData_[i].asciiValue;
		fin >> characterData_[i].x;
		fin >> characterData_[i].width;
		fin >> w;
	}

	fin.close();

	return true;
}

D3DXVECTOR2 Font::MeasureString(string string_)
{
	//D3DXVECTOR2 val = D3DXVECTOR2(0, 0);

	//int length = string_.length();

	//for(int i = 0; i < length; i++)
	//{
	//	char c = string_[i];
	//	int index = 0;
	//	
	//	while(true) 
	//	{
	//		if(c == characterData_[index].symbol)
	//			break;
	//		else
	//			index++;
	//	}

	//	if(c == ' ')
	//		val.x += 1.2f;
	//	else
	//		val.x += (characterData_[index].W * textureWidth) + (float)(this->spacing_ * 0.70f);
	//}
	//	val.y = characterData_[0].H;

		return D3DXVECTOR2(0.0f, 0.0f);
}

unsigned int Font::Length()
{
	return symbolCount_;
}

void Font::Shutdown()
{
	if(texture_)
	{
		texture_->Shutdown();
		delete texture_;
		texture_ = nullptr;
	}
}

SourceRectangle Font::GetFontData(char symbol)
{
	SourceRectangle src;

	for(unsigned int i = 0; i < symbolCount_; i++)
	{
		if((characterData_ + i)->asciiValue == symbol)
		{
			src = SourceRectangle(characterData_[i].x, 0, characterData_[i].width - characterData_[i].x + (1.0f / textureWidth), 1.0f, characterData_[i].asciiValue);

			return src;
		}
	}

	return SourceRectangle(0, 0, 0, 0, 32);
}

Texture2D* Font::GetTexture2D()
{
	return this->texture_;
}

unsigned int Font::GetSpacing()
{
	return this->spacing_;
}

unsigned int Font::GetTextureHeight()
{
	return textureHeight;
}