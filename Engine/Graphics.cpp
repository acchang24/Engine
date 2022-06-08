#include "stdafx.h"
#include <d3dcompiler.h>
#include "Graphics.h"
#include "Shader.h"
#include <cmath>


#pragma comment (lib, "d3d11.lib") 
#pragma comment(lib, "d3dCompiler.lib")



const Vertex vertices[] =
{
	{ 0.0f, 0.5f, Color4(1.0f, 0.0f, 0.0f, 1.0f)},
	{0.5f, -0.5f, Color4(0.0f, 1.0f, 0.0f, 1.0f)},
	{-0.5f, -0.5f, Color4(0.0f, 0.0f, 1.0f, 1.0f)},
	{-0.3f, 0.3f, Color4(0.0f, 1.0f, 0.0f, 1.0f)},
	{0.3f, 0.3f, Color4(0.0f, 0.0f, 1.0f, 1.0f)},
	{ 0.0f, -1.0f, Color4(1.0f, 0.0f, 0.0f, 1.0f)},
};

const uint16_t indices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 1,
	2, 1, 5,
};

Graphics::Graphics(HWND hWnd)
	: mSwapChain(nullptr)
	, mDevice(nullptr)
	, mContext(nullptr)
	, mBackBuffer(nullptr)
	, mTriVertexBuffer(nullptr)
	, mIndexBuffer(nullptr)
	, mShader(nullptr)
	, mConstBuffer(nullptr)
{
	cb =
	{
		std::cos(angle), std::sin(angle), 0.0f, 0.0f,
		-std::sin(angle), std::cos(angle), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};

	HRESULT hr = S_OK;

	// Setup device and swap chain
	{
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferDesc.Width = 0;
		sd.BufferDesc.Height = 0;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 0;
		sd.BufferDesc.RefreshRate.Denominator = 0;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 2;
		sd.OutputWindow = hWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		sd.Flags = 0;

		// Create device, front and back buffers, swap chain, and rendering context
		hr = D3D11CreateDeviceAndSwapChain(nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
#ifdef _DEBUG
			D3D11_CREATE_DEVICE_DEBUG,
#else
			0,
#endif
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&sd,
			&mSwapChain,
			&mDevice,
			nullptr,
			&mContext);
		DbgAssert(hr == S_OK, "Failed to create device");
	}

	// Grab the back buffer (access texture subresource in swap chain)
	{
		ID3D11Resource* pBackBuffer;
		hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), (LPVOID*)&pBackBuffer);
		DbgAssert(hr == S_OK, "Something wrong with back buffer");
		// Create a render target view with that resource
		hr = mDevice->CreateRenderTargetView(pBackBuffer, nullptr, &mBackBuffer);
		DbgAssert(hr == S_OK, "Something went wrong while creating a render target view");
		// Release the temporary resource
		pBackBuffer->Release();
	}

	// Create a vertex buffer
	mTriVertexBuffer = CreateGraphicsBuffer(vertices, sizeof(vertices), sizeof(Vertex), D3D11_BIND_VERTEX_BUFFER, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	
	// Shader
	mShader = new Shader(this);

	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	mShader->Load(L"Engine/Shaders/VertexShader.hlsl", ShaderType::Vertex, ied, sizeof(ied) / sizeof(ied[0]));
	mShader->Load(L"Engine/Shaders/PixelShader.hlsl", ShaderType::Pixel, ied, sizeof(ied) / sizeof(ied[0]));

	mShader->SetActive();

	// Create a index buffer
	mIndexBuffer = CreateGraphicsBuffer(indices, sizeof(indices), sizeof(uint16_t), D3D11_BIND_INDEX_BUFFER, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);

	// Create const buffer
	mConstBuffer = CreateGraphicsBuffer(&cb, sizeof(cb), 0, D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);

	// Draw triangle lists(groups of 3 vertices)
	mContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

Graphics::~Graphics()
{
#ifdef _DEBUG
	ID3D11Debug* pDbg = nullptr;
	HRESULT hr = mDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&pDbg));
	DbgAssert(S_OK == hr, "Unable to create debug device");
#endif

	if (mBackBuffer != nullptr)
	{
		mBackBuffer->Release();
	}
	if (mContext != nullptr)
	{
		mContext->Release();
	}
	if (mSwapChain != nullptr)
	{
		mSwapChain->Release();
	}
	if (mDevice != nullptr)
	{
		mDevice->Release();
	}
	if (mTriVertexBuffer != nullptr)
	{
		mTriVertexBuffer->Release();
	}
	if (mIndexBuffer != nullptr)
	{
		mIndexBuffer->Release();
	}
	if (mShader != nullptr)
	{
		delete mShader;
	}
	if (mConstBuffer != nullptr)
	{
		mConstBuffer->Release();
	}

#ifdef _DEBUG
	pDbg->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL | D3D11_RLDO_IGNORE_INTERNAL);
	pDbg->Release();
#endif
}

void Graphics::ClearBuffer(const Color4& clearColor)
{
	// Clears screen to a certain color
	mContext->ClearRenderTargetView(mBackBuffer, reinterpret_cast<const float*>(&clearColor));
}

void Graphics::ClearBuffer(float red, float green, float blue)
{
	const float color[] = { red, green, blue, 1.0f };
	mContext->ClearRenderTargetView(mBackBuffer, color);
}

void Graphics::EndFrame()
{
	mSwapChain->Present(1, 0);
}

ID3D11Buffer* Graphics::CreateGraphicsBuffer(const void* initData, UINT byteWidth, UINT structByteStride, D3D11_BIND_FLAG inBindFlag, D3D11_CPU_ACCESS_FLAG inCPUAccessFlags, D3D11_USAGE inUsage)
{
	UINT test = sizeof(initData);
	ID3D11Buffer* buffer = nullptr;

	// Setup buffer description
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = inBindFlag;
	bd.Usage = inUsage;
	bd.ByteWidth = byteWidth;
	bd.CPUAccessFlags = inCPUAccessFlags;
	bd.MiscFlags = 0u;
	bd.StructureByteStride = structByteStride;
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = initData;

	HRESULT hr = mDevice->CreateBuffer(&bd, &sd, &buffer);
	DbgAssert(hr == S_OK, "Unable to create a graphics buffer");

	if (initData != nullptr)
	{
		// upload buffer info to gpu
		UploadBuffer(buffer, initData, byteWidth);
	}

	return buffer;
}

void Graphics::UploadBuffer(ID3D11Buffer* buffer, const void* data, UINT dataSize)
{
	D3D11_MAPPED_SUBRESOURCE map;
	HRESULT hr = mContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
	DbgAssert(hr == S_OK, "Map failed");
	memcpy(map.pData, data, dataSize);
	mContext->Unmap(buffer, 0);
}

void Graphics::SetViewport(float x, float y, float width, float height)
{
	// Set Viewport
	D3D11_VIEWPORT vp = {};
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = x;
	vp.TopLeftY = y;
	mContext->RSSetViewports(1, &vp);
}

void Graphics::DrawTestTriangle()
{
	angle = time.Peek();
	cb =
	{
		(3.0f/4.0f)*std::cos(angle), std::sin(angle), 0.0f, 0.0f,
		(3.0f/4.0f)*-std::sin(angle), std::cos(angle), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};
	// Update object buffer
	UploadBuffer(mConstBuffer, &cb, sizeof(cb));

	// Bind render target
	mContext->OMSetRenderTargets(1u, &mBackBuffer, nullptr);

	// Set the viewport
	SetViewport(0.0f, 0.0f, 800.0f, 600.0f);

	// bind constant buffer to vertex shader
	mContext->VSSetConstantBuffers(0, 1, &mConstBuffer);

	// Draw
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	// Bind vertex buffer
	mContext->IASetVertexBuffers(0, 1, &mTriVertexBuffer, &stride, &offset);
	// Bind index buffer
	mContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R16_UINT, 0u);
	mContext->DrawIndexed(sizeof(indices) / sizeof(uint16_t), 0u, 0u);
}
