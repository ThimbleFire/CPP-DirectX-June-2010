#ifndef _SOURCERECTANGLE_H_
#define _SOURCERECTANGLE_H_

class SourceRectangle
{
public:

	SourceRectangle();	
	~SourceRectangle();

	//used for mapping regular texture objects
	SourceRectangle
	(
		float, 
		float, 
		float, 
		float
	);

	//used for mapping font texture objects
	SourceRectangle
	(
		float, 
		float, 
		float, 
		float, 
		char
	);
	
	//returns a rectangle with all it's properties at 0.
	static SourceRectangle Empty
	(
		void
	);

	//returns a rectangle that wraps the entire texture.
	static SourceRectangle Bounds
	(
		void
	);

public:
	float X, Y, W, H;
	char symbol;
};

	struct characterData
	{
	public:
		characterData()
		{

		}
		
		int asciiValue;
		float x;
		float width;
	};

#endif