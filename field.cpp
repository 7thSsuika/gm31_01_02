
#include "main.h"
#include "renderer.h"
#include "gameObject.h"
#include "field.h"
#include "texture.h"
#include <fstream>
#include <vector>



void CField::Init()
{
	std::string filePath = "asset/heightMap02.raw";
	std::ifstream inFile;
	inFile.open(filePath.c_str(), std::ios::binary);
	inFile.seekg(0, std::ios::end);
	std::vector<BYTE> inData(inFile.tellg());
	inFile.seekg(std::ios::beg);
	inFile.read((char*)&inData[0], inData.size());
	inFile.close();

	std::vector<float> heightData;
	heightData.resize(inData.size());
	for (int i = 0; i < inData.size(); ++i)
	{
		heightData[i] = inData[i] * 0.05f;
	}

	for (int z = 0; z < FIELD_Z; z++)
	{
		for (int x = 0; x < FIELD_X; x++)
		{
			m_Vertex[z * FIELD_X + x].Position.x = x * fieldSize - FIELD_X / 2 * fieldSize;
			m_Vertex[z * FIELD_X + x].Position.z = -z * fieldSize + FIELD_Z / 2 * fieldSize;
			m_Vertex[z * FIELD_X + x].Position.y = sinf( x * .5f ) * sinf(z * .3f) * 2.0f;
			m_Vertex[z * FIELD_X + x].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			m_Vertex[z * FIELD_X + x].TexCoord = XMFLOAT2(x, z);
			m_Vertex[z * FIELD_X + x].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		}
	}

	for (int z = 1; z < FIELD_Z - 1; z++)
	{
		for (int x = 1; x < FIELD_X - 1; x++)
		{
			XMFLOAT3 va, vb, n;
			float len;

			va.x = m_Vertex[(z - 1) * FIELD_X + x].Position.x - m_Vertex[(z + 1) * FIELD_X + x].Position.x;
			va.y = m_Vertex[(z - 1) * FIELD_X + x].Position.y - m_Vertex[(z + 1) * FIELD_X + x].Position.y;
			va.z = m_Vertex[(z - 1) * FIELD_X + x].Position.z - m_Vertex[(z + 1) * FIELD_X + x].Position.z;

			vb.x = m_Vertex[z * FIELD_X + (x + 1)].Position.x - m_Vertex[(z - 1) * FIELD_X + (x - 1)].Position.x;
			vb.y = m_Vertex[z * FIELD_X + (x + 1)].Position.y - m_Vertex[(z - 1) * FIELD_X + (x - 1)].Position.y;
			vb.z = m_Vertex[z * FIELD_X + (x + 1)].Position.z - m_Vertex[(z - 1) * FIELD_X + (x - 1)].Position.z;

			n.x = va.y * vb.z - va.z * vb.y;
			n.y = va.z * vb.x - va.x * vb.z;
			n.z = va.x * vb.y - va.y * vb.x;

			len = sqrtf(n.x * n.x + n.y * n.y + n.z * n.z);

			n.x /= len;
			n.y /= len;
			n.z /= len;

			m_Vertex[z * FIELD_X + x].Normal = n;
		}
	}

	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * FIELD_X * FIELD_Z;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = m_Vertex;

		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	}


	unsigned short index[(FIELD_X * 2 + 2) * (FIELD_Z - 1) - 2];

	unsigned short i = 0;
	for (int z = 0; z < FIELD_Z - 1; z++)
	{
		for (int x = 0; x < FIELD_X; x++)
		{
			index[i] = (z + 1) * FIELD_X + x;
			i++;
			index[i] = z * FIELD_X + x;
			i++;
		}

		if (z == FIELD_Z - 2)
			break;

		index[i] = z * FIELD_X + FIELD_X - 1;
		i++;
		index[i] = (z + 2) * FIELD_X;
		i++;
	}


	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned short) * ((FIELD_X * 2 + 2) * (FIELD_Z - 1) - 2);
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = index;

		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer);
	}




	m_Texture = new CTexture();
	m_Texture->Load("asset/field004.tga");
		


	m_Position = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	m_Rotation = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	m_Scale = XMFLOAT3( 1.0f, 1.0f, 1.0f );

}


void CField::Uninit()
{

	m_VertexBuffer->Release();
	m_IndexBuffer->Release();

	m_Texture->Unload();
	delete m_Texture;
	
}


void CField::Update()
{


}


void CField::Draw()
{
	

	// 頂点バッファ設定
	UINT stride = sizeof( VERTEX_3D );
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers( 0, 1, &m_VertexBuffer, &stride, &offset );

	// インデックスバッファ設定
	CRenderer::GetDeviceContext()->IASetIndexBuffer( m_IndexBuffer, DXGI_FORMAT_R16_UINT, 0 );


	// テクスチャ設定
	CRenderer::SetTexture( m_Texture );

	// マトリクス設定
	XMMATRIX world;
	world = XMMatrixScaling( m_Scale.x, m_Scale.y, m_Scale.z );
	world *= XMMatrixRotationRollPitchYaw( m_Rotation.x, m_Rotation.y, m_Rotation.z );
	world *= XMMatrixTranslation( m_Position.x, m_Position.y, m_Position.z );
	CRenderer::SetWorldMatrix( &world );

	// プリミティブトポロジ設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );


	// ポリゴン描画
	CRenderer::GetDeviceContext()->DrawIndexed(((FIELD_X * 2 + 2) * (FIELD_Z - 1) - 2), 0, 0 );

}


float CField::GetHeight(XMFLOAT3 position)
{
	int x, z;
	XMFLOAT3 p0, p1, p2, v, va, vb, hp;
	XMVECTOR v01, v02, n;
	float dp0n, dvn, dpn, t;
	v = XMFLOAT3(0.0f, -10.0f, 0.0f);
	// なんブロックにいるのか
	x = (position.x + FIELD_X / 2 * fieldSize) / fieldSize; // 0.1はブロックのサイズ
	z = (-position.z + FIELD_Z / 2 * fieldSize) / fieldSize;

	va.x = m_Vertex[(z + 1) * FIELD_X/*横頂点数*/ + (x + 1)].Position.x - m_Vertex[z * FIELD_X + x].Position.x;
	va.y = m_Vertex[(z + 1) * FIELD_X + (x + 1)].Position.y - m_Vertex[z * FIELD_X + x].Position.y;
	va.z = m_Vertex[(z + 1) * FIELD_X + (x + 1)].Position.z - m_Vertex[z * FIELD_X + x].Position.z;

	vb.x = position.x - m_Vertex[z * FIELD_X + x].Position.x;
	vb.y = position.y - m_Vertex[z * FIELD_X + x].Position.y;
	vb.z = position.z - m_Vertex[z * FIELD_X + x].Position.z;

	if (va.z * vb.x - va.x * vb.z/*y成分だけを計算*/ > 0.0f)
	{
		p0 = m_Vertex[(z + 1) * FIELD_X + x].Position;
		p1 = m_Vertex[(z * FIELD_X + x)].Position;
		p2 = m_Vertex[(z + 1) * FIELD_X + (x + 1)].Position;
	}
	else
	{
		p0 = m_Vertex[z * FIELD_X + (x + 1)].Position;
		p1 = m_Vertex[(z + 1) * FIELD_X + (x + 1)].Position;
		p2 = m_Vertex[(z * FIELD_X + x)].Position;
	}
	XMVECTOR vp0, vp1, vp2;
	vp0 = XMLoadFloat3(&p0);
	vp1 = XMLoadFloat3(&p1);
	vp2 = XMLoadFloat3(&p2);
	v01 = vp1 - vp0;
	v02 = vp2 - vp0;
	n = XMVector3Cross(v01, v02);//v01 外積 v02;
	XMFLOAT3 fn;
	XMStoreFloat3(&fn, n);
	dvn = v.x * fn.x + v.y * fn.y + v.z * fn.z; //v 内積 n;
	dp0n = p0.x * fn.x + p0.y * fn.y + p0.z * fn.z; //p0 内積 n;
	dpn = position.x * fn.x + position.y * fn.y + position.z * fn.z; //position 内積 n;
	t = (dp0n - dpn) / dvn;
	hp.x = position.x + v.x * t;
	hp.y = position.y + v.y * t;
	hp.z = position.z + v.z * t;
	return hp.y;

}

