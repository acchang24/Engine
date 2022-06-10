#include "stdafx.h"
#include "Cube.h"
#include "Graphics.h"
#include "Shader.h"
#include "VertexBuffer.h"

Cube::Cube() : RenderObj()
{
	mVertexBuffer = new VertexBuffer(pps, sizeof(pps), sizeof(Vector3), indices, sizeof(indices), sizeof(uint16_t));
	
	mConstBuffer = Graphics::Get()->CreateGraphicsBuffer(
		&mObjConsts,
		sizeof(mObjConsts),
		0,
		D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE,
		D3D11_USAGE_DYNAMIC);

	mObjConsts.modelToWorld = Matrix4::Identity;

	mShader = new Shader();

	const D3D11_INPUT_ELEMENT_DESC colorIed[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	mShader->Load(L"Engine/Shaders/CubeVS.hlsl", ShaderType::Vertex, colorIed, sizeof(colorIed) / sizeof(colorIed[0]));
	mShader->Load(L"Engine/Shaders/CubePS.hlsl", ShaderType::Pixel, colorIed, sizeof(colorIed) / sizeof(colorIed[0]));
	mConstColorBuffer = Graphics::Get()->CreateGraphicsBuffer(&cb2, sizeof(cb2), 0, D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
}

Cube::~Cube()
{
	mConstColorBuffer->Release();
}

void Cube::Update(float deltaTime)
{

}

void Cube::Draw()
{
	// Set color const to pixel shader
	Graphics::Get()->GetContext()->PSSetConstantBuffers(0, 1, &mConstColorBuffer);

	mShader->SetActive();

	Graphics* graphics = Graphics::Get();

	// Update const buffer with current data and upload tp GPU
	graphics->UploadBuffer(mConstBuffer, &mObjConsts, sizeof(mObjConsts));

	// Bind constant buffer to vertex shader
	graphics->GetContext()->VSSetConstantBuffers(0, 1, &mConstBuffer);

	mVertexBuffer->Draw();
}
