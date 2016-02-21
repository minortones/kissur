
#ifndef KISS_MODEL
#define KISS_MODEL

#include <Defines.h>

namespace ks
{
	enum PrimType {
		eptNone = 0x0,
		eptPoints = 0x1,
		eptEdges = 0x2,
		eptTriangles = 0x4,
		eptTrianglesWithAdjacency = 0x8,
		eptAll = 0xf
	};

	class Model
	{
	public:
		Model();

		~Model();

		bool loadModelFromFile(const char* file);

		bool makeCube(float pSize);

		bool makeQuad(float pSize);

		kiss32 getPositionSize() const;

		void computeNormals();

		void compileModel(PrimType prim = eptTriangles);

		const float* getCompiledVertices() const;

		const ksU32* getCompiledIndices(PrimType prim = eptTriangles) const;
		
		kiss32 getCompiledVertexSize() const;

		kiss32 getCompiledVertexCount() const;

		kiss32 getCompiledIndexCount(PrimType prim = eptTriangles) const;

		kiss32 getCompiledNormalOffset() const;

		PrimType getPrimType() const;
	
	private:

		float*	mCustomVB;

		kiss32		mVertexCount;
		kiss32		mIndexCount;
		kiss32		mVertexSize;		// this is actually the size of the vertex position data
		PrimType	mPrimType;
	};
}

#endif