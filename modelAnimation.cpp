#include "main.h"
#include "renderer.h"
#include "modelAnimation.h"



void CModelAnimation::Draw()
{
	CRenderer::SetVertexBuffers(m_VertexBuffer);
	CRenderer::SetIndexBuffer(m_IndexBuffer);
	CRenderer::DrawIndexed(m_IndexNum, 0, 0);
}

void CModelAnimation::Draw(XMMATRIX Matrix)
{
}

void CModelAnimation::DrawMesh(aiNode * Node, XMMATRIX Matrix)
{
}

void CModelAnimation::Load(const char * FileName)
{
	const aiScene* scene = aiImportFile(FileName, aiProcessPreset_TargetRealtime_MaxQuality);
	aiMesh* mesh = scene->mMeshes[0];
	VERTEX_3D* vertex = new VERTEX_3D[mesh->mNumVertices];
	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		vertex[i].Position = XMFLOAT3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertex[i].Normal = XMFLOAT3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		vertex[i].Diffuse = XMFLOAT4(1.0f,1.0f,1.0f,1.0f);
		//vertex[i].TexCoord = XMFLOAT2(mesh->mTextureCoords[i]->x, mesh->mTextureCoords[i]->z);
	}


	// 頂点バッファ生成
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * mesh->mNumVertices;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = vertex;

		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	}
	delete[] vertex;

	unsigned short* index;
	index = new unsigned short[mesh->mNumFaces * 3];
	for (int i = 0; i < mesh->mNumFaces; i++)
	{
		index[i * 3 + 0] = mesh->mFaces[i].mIndices[0];
		index[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
		index[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
	}
	m_IndexNum = mesh->mNumFaces * 3;

	// インデックスバッファ生成
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned short) * m_IndexNum;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = index;

		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer);
	}
	delete[] index;
	aiReleaseImport(scene);
}

void CModelAnimation::Unload()
{
	m_VertexBuffer->Release();
	m_IndexBuffer->Release();
}
