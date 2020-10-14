#include "SourceRectangle.h"

SourceRectangle::SourceRectangle()
{
	X = 0;
	Y = 0;
	W = 0;
	H = 0;
						//by default symbol is whitespace.
	symbol = 32;
}

SourceRectangle::SourceRectangle(float x, float y, float w, float h)
{
	X = x;
	Y = y;
	W = w;
	H = h;

	symbol = 32;
}

SourceRectangle::SourceRectangle(float x, float y, float w, float h, char c)
{
	X = x;
	Y = y;
	W = w;
	H = h;

	symbol = c;
}

SourceRectangle::~SourceRectangle()
{

}

SourceRectangle SourceRectangle::Empty()
{
	return SourceRectangle(0.0f, 0.0f, 0.0f, 0.0f, 32);
}

SourceRectangle SourceRectangle::Bounds()
{
	return SourceRectangle(0.0f, 0.0f, 1.0f, 1.0f, 32);
}