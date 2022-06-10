#include "stdafx.h"
#include "RenderObj.h"
#include "VertexBuffer.h"
#include "Graphics.h"

RenderObj::RenderObj() 
	: mVertexBuffer(nullptr)
	, mConstBuffer(nullptr)
	, pos(Vector3::Zero)
	, scale(0.0f)
	, rotation(0.0f)
{
}

RenderObj::RenderObj(const VertexBuffer* vBuffer) 
	: mVertexBuffer(vBuffer)
	, pos(Vector3::Zero)
	, scale(0.0f)
	, rotation(0.0f)
{
	mConstBuffer = Graphics::Get()->CreateGraphicsBuffer(
		&mObjConsts,
		sizeof(mObjConsts),
		0,
		D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE,
		D3D11_USAGE_DYNAMIC);
}

RenderObj::~RenderObj()
{
	delete mVertexBuffer;

	mConstBuffer->Release();
}

void RenderObj::Update(float deltaTime)
{

}

void RenderObj::Draw()
{
	Graphics* graphics = Graphics::Get();

	// Update const buffer with current data and upload tp GPU
	graphics->UploadBuffer(mConstBuffer, &mObjConsts, sizeof(mObjConsts));

	// Bind constant buffer to vertex shader
	graphics->GetContext()->VSSetConstantBuffers(0, 1, &mConstBuffer);

	mVertexBuffer->Draw();
}