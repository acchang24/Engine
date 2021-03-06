#include "stdafx.h"
#include "RenderObj.h"
#include "Graphics.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "App.h"

RenderObj::RenderObj(App* app) 
	: mVertexBuffer(nullptr)
	, mConstBuffer(nullptr)
	, mShader(nullptr)
	, pos(Vector3::Zero)
	, scale(0.0f)
	, rotation(0.0f)
{
	app->AddRenderObj(this);
}

RenderObj::RenderObj(const VertexBuffer* vBuffer, Shader* shader, App* app) 
	: mVertexBuffer(vBuffer)
	, mShader(shader)
	, pos(Vector3::Zero)
	, scale(0.0f)
	, rotation(0.0f)
{
	app->AddRenderObj(this);

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
	if (mVertexBuffer)
	{
		delete mVertexBuffer;
	}
	if (mShader)
	{
		delete mShader;
	}
	if (mConstBuffer)
	{
		mConstBuffer->Release();
	}
}

void RenderObj::Update(float deltaTime)
{

}

void RenderObj::Draw()
{
	mShader->SetActive();

	Graphics* graphics = Graphics::Get();

	// Update const buffer with current data and upload tp GPU
	graphics->UploadBuffer(mConstBuffer, &mObjConsts, sizeof(mObjConsts));

	// Bind constant buffer to vertex shader
	graphics->GetContext()->VSSetConstantBuffers(Graphics::ConstantBuffer::CONSTANT_BUFFER_RENDEROBJ, 1, &mConstBuffer);

	mVertexBuffer->Draw();
}