#pragma once

#include <map>
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
	const aiScene* m_Scene[3];
	u_int sceneCount = 0;
	u_int currentScene = 0;

	std::map<std::string, aiQuaternion> m_NodeRotation;
	std::map<std::string, aiVector3D> m_NodePosition;
public:
	//void CModelAnimationUpdate(int Animation1, int Animation2, float Blend, int Frame);
	void Draw();
	void Draw(XMMATRIX& Matrix);
	void DrawMesh(aiNode* Node, XMMATRIX& Matrix);
	void Load(const char* FileName);
	void LoadAnim(const char* FileName);
	void SetAnim(u_int animCount);
	void Unload();
	void Update(int Frame);
};

//  Scene[3]    Update(int Animtaion, int Frame)
// void CModelAnimation Load(-)
// m_Scene[0] = aiImportFile("Human_Stop.fbx")
// m_Scene[1] =...
//
// void CModelAnimation::Update(...
//   aiAnimation* animation = m_Scene[Animation],mAnimation[0]
