#include "stdafx.h"
#include "Graphics.h"
#include <d3dcompiler.h>

#pragma comment (lib, "d3d11.lib") 
#pragma comment(lib, "D3DCompiler.lib")

Graphics::Graphics(HWND hWnd)
	: mSwapChain(nullptr)
	, mDevice(nullptr)
	, mContext(nullptr)
	, mBackBuffer(nullptr)
	, mTriVertexBuffer(nullptr)
	, mTriangleVertexShader(nullptr)
	, mBlob(nullptr)
{
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
		//wrl::ComPtr<ID3D11Resource> pBackBuffer;
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
	{
		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(vertices);
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0u;
		bd.StructureByteStride = sizeof(Vertex);
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vertices;
		
		hr = mDevice->CreateBuffer(&bd, &sd, &mTriVertexBuffer);
		DbgAssert(hr == S_OK, "Unable to create vertex buffer");
	}	

	// Create Vertex Shader
	{
		// Read shader file
		hr = D3DReadFileToBlob(L"VertexShader.cso", &mBlob);
		DbgAssert(hr == S_OK, "Unable to read shader file");

		// Create Vertex shader
		hr = mDevice->CreateVertexShader(mBlob->GetBufferPointer(), mBlob->GetBufferSize(), nullptr, &mTriangleVertexShader);
		DbgAssert(hr == S_OK, "Failed to create vertex shader");
	
		// Bind vertex shader
		mContext->VSSetShader(mTriangleVertexShader, nullptr, 0);
	}
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
	if (mTriangleVertexShader != nullptr)
	{
		mTriangleVertexShader->Release();
	}
	if (mBlob != nullptr)
	{
		mBlob->Release();
	}

#ifdef _DEBUG
	pDbg->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL | D3D11_RLDO_IGNORE_INTERNAL);
	pDbg->Release();
#endif
}

void Graphics::ClearBuffer(const Color4& clearColor)
{
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

void Graphics::DrawTestTriangle()
{
	
	// Draw
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	mContext->IASetVertexBuffers(0, 1, &mTriVertexBuffer, &stride, &offset);
	mContext->Draw(sizeof(vertices) / sizeof(Vertex), 0u);
}
