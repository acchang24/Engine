#include "stdafx.h"
#include "RenderObj.h"
#include "VertexBuffer.h"
#include "Graphics.h"

RenderObj::RenderObj() : mVertexBuffer(nullptr), mConstBuffer(nullptr)
{
}

RenderObj::RenderObj(const VertexBuffer* vBuffer) : mVertexBuffer(vBuffer)
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

void RenderObj::Draw()
{
	Graphics* graphics = Graphics::Get();

	// Update const buffer with current data and upload tp GPU
	graphics->UploadBuffer(mConstBuffer, &mObjConsts, sizeof(mObjConsts));

	// Bind constant buffer to vertex shader
	graphics->GetContext()->VSSetConstantBuffers(0, 1, &mConstBuffer);

	mVertexBuffer->Draw();
}