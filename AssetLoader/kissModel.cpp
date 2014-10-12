
#include "kissModel.h"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>


static kissU32 mCubeIndices[] = {	0, 1, 2, 3,			// front
									4, 5, 1, 0,			// top
									3, 2, 6, 7,			// bottom
									5, 4, 7, 6,			// back
									1, 5, 6, 2,			// right
									4, 0, 3, 7 };		// left


namespace kiss
{
	Model::Model() : mCustomVB(nullptr)
	{}

	Model::~Model()
	{
		if (mCustomVB)
			delete [] mCustomVB;
	}

	bool Model::loadModelFromFile(const char* file)
	{
		/*Assimp::Importer importer;
		const aiScene* pScene = importer.ReadFile(file, 0);*/
		return false;
	}

	bool Model::makeCube(float dim)
	{
		if (mCustomVB)
			delete [] mCustomVB;

		mCustomVB = new float[60] {-dim,	dim,	dim,
									dim,	dim,	dim,
									dim,   -dim,	dim,
								   -dim,   -dim,	dim,
								   -dim,	dim,	-dim,
									dim,	dim,	-dim,
									dim,   -dim,	-dim,
								   -dim,   -dim,	-dim,
									// normals begin here
									0.0, 0.0, 1.0,
									0.0, 0.0, 1.0,

									0.0, 1.0, 0.0,
									0.0, 1.0, 0.0,

									0.0, -1.0, 0.0,
									0.0, -1.0, 0.0,

									0.0, 0.0, -1.0,
									0.0, 0.0, -1.0,

									1.0, 0.0, 0.0,
									1.0, 0.0, 0.0,

									-1.0, 0.0, 0.0,
									-1.0, 0.0, 0.0 };

		mVertexSize		= 3;
		mVertexCount	= 24;
		mIndexCount		= 24;
		mPrimType		= eptNone;

		return true;
	}

	bool Model::makeQuad(float dim)
	{
		if (mCustomVB)
			delete [] mCustomVB;

		mCustomVB = new float[24] {-dim,	dim,	dim,
									dim,	dim,	dim,
									dim,   -dim,	dim,
								   -dim,   -dim,	dim,
									// normals begin here
									0.0, 0.0, 1.0,
									0.0, 0.0, 1.0,
									0.0, 0.0, 1.0,
									0.0, 0.0, 1.0, };

		mVertexSize		= 3;
		mVertexCount	= 12;
		mIndexCount		= 4;
		mPrimType		= eptNone;

		return true;
	}

	kiss32 Model::getPositionSize() const
	{
		return mCustomVB ? mVertexSize : NULL;
	}

	void Model::computeNormals()
	{}

	void Model::compileModel( PrimType prim )
	{}

	const float* Model::getCompiledVertices() const
	{
		return mCustomVB ? mCustomVB : NULL;
	}

	const kissU32* Model::getCompiledIndices(PrimType prim) const
	{
		return mCustomVB ? mCubeIndices : NULL;
	}

	kiss32 Model::getCompiledVertexSize() const
	{
		return NULL;
	}

	kiss32 Model::getCompiledVertexCount() const
	{
		return mCustomVB ? mVertexCount : NULL;
	}
	
	kiss32 Model::getCompiledIndexCount(PrimType prim) const
	{
		return mCustomVB ? mIndexCount : NULL;
	}

	kiss32 Model::getCompiledNormalOffset() const
	{
		return mCustomVB ? mVertexCount : NULL;
	}

	PrimType Model::getPrimType() const
	{
		return mPrimType;
	}
}