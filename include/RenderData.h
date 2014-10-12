
/*
	Nnanna Kama
	Structure for encapsulating generic render data
	includes VertexBuffer implementation @todo
	All members public for now @todo?
	Render modes/states could also be encapsulated in here @todo
*/



#pragma once


#include "defines.h"
#include "Matrix4x4.h"


/*
	redundant
*/
template <class T>
class VertexBuffer
{

public:


	T*		mBufferData;

};



/*
	redundant
	Kinda duplicates OpenGL rendermode info
*/

enum RENDERMODE
{
	ePoints = 0,
	eLines,
	eLineLoop,
	eLineStrip,
	eTriangles,
	eTriStrip,
	eTriFan,
	eQuad,
	eQuadStrip,
	ePolygon
};


struct	Material;



class RenderData
{
public:
	RenderData( const void* pVB, const kissU32* pIB, const Matrix4x4& pTrans ) : vertexBuffer(pVB),
																				indexBuffer(pIB),
																				Transform(pTrans),
																				material(NULL)
	{}

	const void*		vertexBuffer;

	kiss32			vertexSize;	/*Specifies the number of coordinates per vertex. Must be 2, 3, or 4.*/

	const kissU32*	indexBuffer;
	
	kiss32			renderMode;

	kiss32			stride;

	kiss32			numIndices;

	kiss32			normOffset;

	Material*		material;

	const Matrix4x4&	Transform;

};
