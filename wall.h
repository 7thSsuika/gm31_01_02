#pragma once

class CWall
{
private:
	ID3D11Buffer* vertexBuffer = NULL;
	CTexture* texture = NULL;
	float wallSize;

public:
	CWall(float _wallSize,  const char* texPath);
	~CWall();

	void Update();
	void Draw();
};
