#include "stdafx.h"
#include "Graphics.h"
#include <d3dcompiler.h>

#pragma comment (lib, "d3d11.lib") 
#pragma comment(lib, "d3dCompiler.lib")



const Vertex vertices[] =
{
	{ 0.0f, 0.5f, Color4(1.0f, 0.0f, 0.0f, 1.0f)},
	{0.5f, -0.5f, Color4(0.0f, 1.0f, 0.0f, 1.0f)},
	{-0.5f, -0.5f, Color4(0.0f, 0.0f, 1.0f, 1.0f)},
	{-0.3f, 0.3f, Color4(0.0f, 1.0f, 0.0f, 1.0f)},
	{0.3f, 0.3f, Color4(0.0f, 0.0f, 1.0f, 1.0f)},
	{ 0.0f, -0.8f, Color4(1.0f, 0.0f, 0.0f, 1.0f)},
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
	, mTriangleVertexShader(nullptr)
	, mTrianglePixelShader(nullptr)
	, mVSBlob(nullptr)
	, mPSBlob(nullptr)
	, mInputLayout(nullptr)
	, mIndexBuffer(nullptr)
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
		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#ifdef _DEBUG
		// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
		// Setting this flag improves the shader debugging experience, but still allows 
		// the shaders to be optimized and to run exactly the way they will run in 
		// the release configuration of this program.
		dwShaderFlags |= D3DCOMPILE_DEBUG;

		// Disable optimizations to further improve shader debugging
		dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
		// Read shader file
		ID3DBlob* pErrorBlob = nullptr;
		hr = D3DCompileFromFile(L"Engine/Shaders/VertexShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_4_0",
			dwShaderFlags, 0, &mVSBlob, &pErrorBlob);

		if (FAILED(hr))
		{
			if (pErrorBlob)
			{
				static wchar_t szBuffer[4096];
				_snwprintf_s(szBuffer, 4096, _TRUNCATE,
					L"%hs",
					(char*)pErrorBlob->GetBufferPointer());
				OutputDebugString(szBuffer);
#ifdef _WINDOWS
				MessageBox(nullptr, szBuffer, L"Error", MB_OK);
#endif
				pErrorBlob->Release();
			}
		}
		if (pErrorBlob)
		{
			pErrorBlob->Release();
		}
		//hr = D3DReadFileToBlob(L"VertexShader.cso", &mVSBlob);
		//DbgAssert(hr == S_OK, "Unable to read vertex shader file");

		// Create Vertex shader
		hr = mDevice->CreateVertexShader(mVSBlob->GetBufferPointer(), mVSBlob->GetBufferSize(), nullptr, &mTriangleVertexShader);
		DbgAssert(hr == S_OK, "Failed to create vertex shader");

		// Bind vertex shader
		mContext->VSSetShader(mTriangleVertexShader, nullptr, 0);
	}

	// Create Pixel Shader
	{
		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#ifdef _DEBUG
		// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
		// Setting this flag improves the shader debugging experience, but still allows 
		// the shaders to be optimized and to run exactly the way they will run in 
		// the release configuration of this program.
		dwShaderFlags |= D3DCOMPILE_DEBUG;

		// Disable optimizations to further improve shader debugging
		dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
		// Read shader file
		ID3DBlob* pErrorBlob = nullptr;
		hr = D3DCompileFromFile(L"Engine/Shaders/PixelShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_4_0",
			dwShaderFlags, 0, &mPSBlob, &pErrorBlob);

		if (FAILED(hr))
		{
			if (pErrorBlob)
			{
				static wchar_t szBuffer[4096];
				_snwprintf_s(szBuffer, 4096, _TRUNCATE,
					L"%hs",
					(char*)pErrorBlob->GetBufferPointer());
				OutputDebugString(szBuffer);
#ifdef _WINDOWS
				MessageBox(nullptr, szBuffer, L"Error", MB_OK);
#endif
				pErrorBlob->Release();
			}
		}
		if (pErrorBlob)
		{
			pErrorBlob->Release();
		}
		//hr = D3DReadFileToBlob(L"PixelShader.cso", &mPSBlob);
		//DbgAssert(hr == S_OK, "Unable to read pixel shader file");

		hr = mDevice->CreatePixelShader(mPSBlob->GetBufferPointer(), mPSBlob->GetBufferSize(), nullptr, &mTrianglePixelShader);
		DbgAssert(hr == S_OK, "Failed to create vertex shader");

		mContext->PSSetShader(mTrianglePixelShader, nullptr, 0);
	}

	// Create Input Layout
	{
		const D3D11_INPUT_ELEMENT_DESC ied[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
		hr = mDevice->CreateInputLayout(ied, sizeof(ied) / sizeof(D3D11_INPUT_ELEMENT_DESC), mVSBlob->GetBufferPointer(), mVSBlob->GetBufferSize(), &mInputLayout);
		DbgAssert(hr == S_OK, "Failed to create input layout");

		// Bind input layout
		mContext->IASetInputLayout(mInputLayout);
	}

	// Create a index buffer
	{
		D3D11_BUFFER_DESC ibd = {};
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.Usage = D3D11_USAGE_DYNAMIC;
		ibd.ByteWidth = sizeof(indices);
		ibd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		ibd.MiscFlags = 0u;
		ibd.StructureByteStride = sizeof(uint16_t);
		D3D11_SUBRESOURCE_DATA isd = {};
		isd.pSysMem = indices;

		hr = mDevice->CreateBuffer(&ibd, &isd, &mIndexBuffer);
		DbgAssert(hr == S_OK, "Unable to create index buffer");

		// Bind index buffer
		mContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R16_UINT, 0u);
	}

	

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
	if (mTriangleVertexShader != nullptr)
	{
		mTriangleVertexShader->Release();
	}
	if (mTrianglePixelShader != nullptr)
	{
		mTrianglePixelShader->Release();
	}
	if (mVSBlob != nullptr)
	{
		mVSBlob->Release();
	}
	if (mPSBlob != nullptr)
	{
		mPSBlob->Release();
	}
	if (mInputLayout != nullptr)
	{
		mInputLayout->Release();
	}
	if (mIndexBuffer != nullptr)
	{
		mIndexBuffer->Release();
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
	
	// Bind render target
	mContext->OMSetRenderTargets(1u, &mBackBuffer, nullptr);


	// Set Viewport
	D3D11_VIEWPORT vp = {};
	vp.Width = 800;
	vp.Height = 600;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	mContext->RSSetViewports(1, &vp);


	// Draw
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	mContext->IASetVertexBuffers(0, 1, &mTriVertexBuffer, &stride, &offset);

	mContext->DrawIndexed(sizeof(indices) / sizeof(uint16_t), 0u, 0u);
	//mContext->Draw(sizeof(vertices) / sizeof(Vertex), 0u);
}
