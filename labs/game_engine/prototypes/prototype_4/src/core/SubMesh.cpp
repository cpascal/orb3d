#include "core_pch.h"
#include "..\..\include\core\SubMesh.h"
#include "core\Mesh.h"


namespace engine
{
	SubMesh::SubMesh()
	{
		
	}


	SubMesh::~SubMesh(void)
	{
	}
	void SubMesh::Create(MeshPtr pMesh, int indexOffset, int indexCount, int vertexOffset, int vertexCount, int vertexStride, int matIndex, int primCount)
	{
		m_pMesh						= pMesh;

		m_indexOffset				= indexOffset;
		m_vertexOffset				= vertexOffset;

		m_vertexCount				= vertexCount;
		m_indexCount				= indexCount;
		m_primitiveCount			= primCount;
		m_materialIndex				= matIndex;
		m_vertexStride				= vertexStride;
	}
	void* SubMesh::GetIndexData()
	{
		return m_pMesh->GetIndexData();
	}
	void* SubMesh::GetVertexData()
	{
		return m_pMesh->GetVertexData();
	}
	int SubMesh::GetIndexDataOffset()
	{
		return m_indexOffset;
	}
	int SubMesh::GetVertexDataOffset()
	{
		return m_vertexOffset;
	}
	int	SubMesh::GetIndexDataBytes()
	{
		return m_indexCount * sizeof(uint16);
	}
	int SubMesh::GetVertexDataBytes()
	{
		return m_vertexCount * m_vertexStride;
	}
	int SubMesh::GetIndexCount()
	{
		return m_indexCount;
	}
	int	SubMesh::GetVertexCount()
	{
		return m_vertexCount;
	}
	int	SubMesh::GetVertexStride()
	{
		return m_vertexStride;
	}

	GFXPtr SubMesh::GetMaterial()
	{
		return m_pMesh->GetMaterialByIndex(m_materialIndex);
	}
	int SubMesh::GetPrimitiveCount()
	{
		return m_primitiveCount;
	}
}
