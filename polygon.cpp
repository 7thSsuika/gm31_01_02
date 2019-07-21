#include "main.h"
#include "renderer.h"
#include "polygon.h"
#include "texture.h"

void CPolygon::Init()
{
	VERTEX_3D vertex[4];
	vertex[0].Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[0].Normal   = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = XMFLOAT3(100.0f, 0.0f, 0.0f);
	vertex[1].Normal   = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = XMFLOAT3(0.0f, 100.0f, 0.0f);
	vertex[2].Normal   = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = XMFLOAT3(100.0f, 100.0f, 0.0f);
	vertex[3].Normal   = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;
	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	m_Texture = new CTexture();
	m_Texture->Load("asset/number.tga");
}

void CPolygon::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Unload();
	delete m_Texture;
}

void CPolygon::Update()
{
	// none
}

void CPolygon::Draw()
{
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
	CRenderer::SetTexture(m_Texture);
	XMMATRIX world;
	
	CRenderer::SetWorldViewProjection2D(world);
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CRenderer::GetDeviceContext()->Draw(4, 0);
}
