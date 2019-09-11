#pragma once
class CModelAnimation
{
	struct MESH
	{
		ID3D11Buffer* VertexBuffer;
		ID3D11Buffer* IndexBuffer;
		unsigned int IndexNum;
	};
private:
	MESH* m_Mesh;
	unsigned int m_MeshNum;
	const aiScene* m_Scene;
public:
	void Draw();
	void Draw(XMMATRIX& Matrix);
	void DrawMesh(aiNode* Node, XMMATRIX& Matrix);
	void Load(const char* FileName);
	void Unload();
};

