#include "main.h"
#include "renderer.h"
#include "modelAnimation.h"



void CModelAnimation::Draw()
{
	//CRenderer::SetVertexBuffers(m_VertexBuffer);
	//CRenderer::SetIndexBuffer(m_IndexBuffer);
	//CRenderer::DrawIndexed(m_IndexNum, 0, 0);
}

void CModelAnimation::Draw(XMMATRIX& Matrix)
{
	DrawMesh(m_Scene->mRootNode, Matrix);
}

void CModelAnimation::DrawMesh(aiNode * Node, XMMATRIX& Matrix)
{
	aiMatrix4x4 matrix = Node->mTransformation;
	aiTransposeMatrix4(&matrix);
	XMMATRIX world = XMLoadFloat4x4((XMFLOAT4X4*)&matrix);
	world *= Matrix;

	CRenderer::SetWorldMatrix(&world);
	for (int n = 0; n < Node->mNumMeshes; n++)
	{
		unsigned int m = Node->mMeshes[n];
		CRenderer::SetVertexBuffers(m_Mesh[m].VertexBuffer);
		CRenderer::SetIndexBuffer(m_Mesh[m].IndexBuffer);
		CRenderer::DrawIndexed(m_Mesh[m].IndexNum, 0, 0);
	}

	for (int n = 0; n < Node->mNumChildren; n++)
	{
		DrawMesh(Node->mChildren[n], world)
			;
	}
}

void CModelAnimation::Load(const char * FileName)
{
	this->m_Scene = aiImportFile(FileName, aiProcessPreset_TargetRealtime_MaxQuality);
	m_MeshNum = m_Scene->mNumMeshes;
	m_Mesh = new MESH[m_MeshNum];
	for (int m = 0; m < m_MeshNum; m++)
	{
		aiMesh* mesh = m_Scene->mMeshes[m];
		VERTEX_3D* vertex = new VERTEX_3D[mesh->mNumVertices];
		for (int i = 0; i < mesh->mNumVertices; i++)
		{
			vertex[i].Position = XMFLOAT3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
			vertex[i].Normal = XMFLOAT3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
			vertex[i].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
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

			CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_Mesh[m].VertexBuffer);
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
		m_Mesh[m].IndexNum = mesh->mNumFaces * 3;

		// インデックスバッファ生成
		{
			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(unsigned short) * m_Mesh[m].IndexNum;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = index;

			CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_Mesh[m].IndexBuffer);
		}
		delete[] index;
	}

	//aiMesh* mesh = scene->mMeshes[0];
	//VERTEX_3D* vertex = new VERTEX_3D[mesh->mNumVertices];
	//for (int i = 0; i < mesh->mNumVertices; i++)
	//{
	//	vertex[i].Position = XMFLOAT3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
	//	vertex[i].Normal = XMFLOAT3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
	//	vertex[i].Diffuse = XMFLOAT4(1.0f,1.0f,1.0f,1.0f);
	//	//vertex[i].TexCoord = XMFLOAT2(mesh->mTextureCoords[i]->x, mesh->mTextureCoords[i]->z);
	//}


	//// 頂点バッファ生成
	//{
	//	D3D11_BUFFER_DESC bd;
	//	ZeroMemory(&bd, sizeof(bd));
	//	bd.Usage = D3D11_USAGE_DEFAULT;
	//	bd.ByteWidth = sizeof(VERTEX_3D) * mesh->mNumVertices;
	//	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//	bd.CPUAccessFlags = 0;

	//	D3D11_SUBRESOURCE_DATA sd;
	//	ZeroMemory(&sd, sizeof(sd));
	//	sd.pSysMem = vertex;

	//	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	//}
	//delete[] vertex;

	//unsigned short* index;
	//index = new unsigned short[mesh->mNumFaces * 3];
	//for (int i = 0; i < mesh->mNumFaces; i++)
	//{
	//	index[i * 3 + 0] = mesh->mFaces[i].mIndices[0];
	//	index[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
	//	index[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
	//}
	//m_IndexNum = mesh->mNumFaces * 3;

	//// インデックスバッファ生成
	//{
	//	D3D11_BUFFER_DESC bd;
	//	ZeroMemory(&bd, sizeof(bd));
	//	bd.Usage = D3D11_USAGE_DEFAULT;
	//	bd.ByteWidth = sizeof(unsigned short) * m_IndexNum;
	//	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//	bd.CPUAccessFlags = 0;

	//	D3D11_SUBRESOURCE_DATA sd;
	//	ZeroMemory(&sd, sizeof(sd));
	//	sd.pSysMem = index;

	//	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer);
	//}
	//delete[] index;
	//aiReleaseImport(scene);
}

void CModelAnimation::Unload()
{
	for (int d = 0; d < m_MeshNum; d++)
	{
		m_Mesh[d].VertexBuffer->Release();
		m_Mesh[d].IndexBuffer->Release();
	}
	delete[] m_Mesh;
	aiReleaseImport(m_Scene);
}
