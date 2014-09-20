
#include "kissModel.h"

namespace kiss
{
	Model::Model()
	{}

	Model::~Model()
	{}

	bool Model::loadModelFromFile(const char* file)
	{
		return false;
	}

	int Model::getPositionSize() const
	{
		return NULL;
	}

	void Model::computeNormals()
	{}

	void Model::compileModel( PrimType prim )
	{}

	const float* Model::getCompiledVertices() const
	{
		return NULL;
	}

	const kissU32* Model::getCompiledIndices(PrimType prim) const
	{
		return NULL;
	}

	int Model::getCompiledVertexSize() const
	{
		return NULL;
	}

	int Model::getCompiledVertexCount() const
	{
		return NULL;
	}
	
	int Model::getCompiledIndexCount(PrimType prim) const
	{
		return NULL;
	}

	int Model::getCompiledNormalOffset() const
	{
		return NULL;
	}
}