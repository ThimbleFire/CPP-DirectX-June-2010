#include "VertexBuffer2D.h"

VertexBuffer2D::VertexBuffer2D()
{
	m_vertexBuffer = nullptr;
	m_indexBuffer  = nullptr;

	Translate(0.0f, 0.0f, 0.0f);
	Rotate(0.0f);
}

VertexBuffer2D::~VertexBuffer2D()
{

}

bool VertexBuffer2D::Initialize(ID3D11Device* device, unsigned int screenWidth, unsigned int screenHeight)
{
	bool result;
	
	screenWidth_ = screenWidth;
	screenHeight_ = screenHeight;
	
	oldX = -1;
	oldY = -1;

	result = InitializeBuffers(device);
	
	if(!result)
		return false;

	return true;
}

void VertexBuffer2D::Shutdown()
{
	if(m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = nullptr;
	}

	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = nullptr;
	}
}

int VertexBuffer2D::GetIndexCount()
{
	return m_indexCount;
}

bool VertexBuffer2D::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	m_vertexCount = 6;
	m_indexCount = 6;

	vertices = new VertexType[m_vertexCount];

	if(!vertices)
		return false;

	indices = new unsigned long[m_indexCount];

	if(!indices)
		return false;
	
	memset(vertices, 0, (sizeof(VertexType) * m_vertexCount));
	
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 0;
	indices[4] = 4; //altern
	indices[5] = 5;

	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);

	if(FAILED(result))
		return false;

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	
	if(FAILED(result))
		return false;

	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

	return true;
}

bool VertexBuffer2D::UpdateBuffers(ID3D11DeviceContext* deviceContext, float positionOnScreenX, float positionOnScreenY, SourceRectangle sourceRectangle, float textureWidth, float textureHeight)
{
	if(oldX == positionOnScreenX && oldY == positionOnScreenY)
		return true;

	oldX = positionOnScreenX;
	oldY = positionOnScreenY;

	VertexType* vertices;

	unsigned long* indices;

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	vertices = nullptr;

	vertices = new VertexType[m_vertexCount];

	indices = nullptr;

	indices = new unsigned long[m_indexCount];

	positionOnScreenX =  (float)(((float)screenWidth_  / 2) * (-1) + positionOnScreenX);
	positionOnScreenY = -(float)(((float)screenHeight_ / 2) * (-1) + positionOnScreenY);

	//in future, try to find a way to manually get these numbers
	//as  we'll need to render a UI and not all objects will be of this resolution.

	float left		=	positionOnScreenX;
	float right		=	positionOnScreenX + ((sourceRectangle.W * textureWidth));
	float top		=	positionOnScreenY;
	float bot		=	positionOnScreenY - ((sourceRectangle.H * textureHeight));
		
	vertices[0].position = D3DXVECTOR3(	left,	top,	0.0f);
	vertices[1].position = D3DXVECTOR3(	right,	bot,	0.0f);
	vertices[2].position = D3DXVECTOR3(	left,	bot,	0.0f);
	vertices[3].position = D3DXVECTOR3(	left,	top,	0.0f);
	vertices[4].position = D3DXVECTOR3(	right,	top,	0.0f);
	vertices[5].position = D3DXVECTOR3(	right,	bot,	0.0f);

	//TOP LEFT
	vertices[0].texture  = D3DXVECTOR2(sourceRectangle.X, 
									   sourceRectangle.Y);
	//BOTTOM RIGHT
	vertices[1].texture  = D3DXVECTOR2(sourceRectangle.X + sourceRectangle.W, 
									   sourceRectangle.Y + sourceRectangle.H);
	//BOTTOM LEFT
	vertices[2].texture  = D3DXVECTOR2(sourceRectangle.X, 
									   sourceRectangle.Y + sourceRectangle.H);
	//TOP LEFT
	vertices[3].texture  = D3DXVECTOR2(sourceRectangle.X, 
									   sourceRectangle.Y);
	//TOP RIGHT
	vertices[4].texture  = D3DXVECTOR2(sourceRectangle.X + sourceRectangle.W, 
									   sourceRectangle.Y);	
	//BOTTOM LEFT
	vertices[5].texture  = D3DXVECTOR2(sourceRectangle.X + sourceRectangle.W, 
									   sourceRectangle.Y + sourceRectangle.H);
	
	//lock vertex buffer so it can be written to.
	HRESULT result = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	if(FAILED(result))
		return false;

	VertexType* src = nullptr;

	src = (VertexType*)mappedResource.pData;

	memcpy(src, (void*)vertices, (sizeof(VertexType) * m_vertexCount));

	deviceContext->Unmap(m_vertexBuffer, 0);

	delete [] vertices;
	vertices = nullptr;

	delete [] indices;
	indices = nullptr;

	return true;
}

void VertexBuffer2D::Translate(float x, float y, float z)
{
	D3DXMatrixTranslation(&matrix_translation_, x, y, z);
}

void VertexBuffer2D::Rotate(float angle)
{
	D3DXMatrixRotationY(&matrix_rotation_, angle);
}

bool VertexBuffer2D::Render(ID3D11DeviceContext* deviceContext, float positionOnScreenX, float positionOnScreenY, Texture2D* texture, SourceRectangle sourceRectangle, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, D3DXVECTOR4 colour)
{
	unsigned int stride, offset;
	
	stride= sizeof(VertexType);
	offset = 0;
	
	UpdateBuffers(deviceContext, positionOnScreenX, positionOnScreenY, sourceRectangle, texture->GetWidth(), texture->GetHeight());

	worldMatrix *= matrix_translation_ * matrix_rotation_;

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return true;
}