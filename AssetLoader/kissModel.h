
#ifndef KISS_MODEL
#define KISS_MODEL

#include <Defines.h>

namespace kiss
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

		int getPositionSize() const;

		void computeNormals();

		void compileModel(PrimType prim = eptTriangles);

		const float* getCompiledVertices() const;

		const kissU32* getCompiledIndices(PrimType prim = eptTriangles) const;
		
		int getCompiledVertexSize() const;

		int getCompiledVertexCount() const;

		int getCompiledIndexCount(PrimType prim = eptTriangles) const;

		int getCompiledNormalOffset() const;
	
	private:

	};
}

#endif