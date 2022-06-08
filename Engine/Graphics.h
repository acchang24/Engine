#pragma once
#include <d3d11.h>
#include "VertexFormats.h"

class Shader;

class Graphics
{
public:
	Graphics(HWND hWnd);
	~Graphics();
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;

	void ClearBuffer(const Color4& clearColor);
	void ClearBuffer(float red, float green, float blue);
	void EndFrame();

	// Getters for D3D Device and Device Context
	ID3D11Device* GetDevice() { return mDevice; }
	ID3D11DeviceContext* GetContext() { return mContext; }

	ID3D11Buffer* CreateGraphicsBuffer(const void* initData, UINT byteWidth, UINT structByteStride, D3D11_BIND_FLAG inBindFlag, D3D11_CPU_ACCESS_FLAG inCPUAccessFlags, D3D11_USAGE inUsage);

	void SetViewport(float x, float y, float width, float height);

	void DrawTestTriangle();

private:
	// TEST TRIANGLE STUFF---------------------
	ID3D11Buffer* mTriVertexBuffer;
	Shader* mShader;
	ID3D11Buffer* mIndexBuffer;

	// Normal stuff
	IDXGISwapChain* mSwapChain;
	ID3D11Device* mDevice;
	ID3D11DeviceContext* mContext;
	
	ID3D11RenderTargetView* mBackBuffer;
};

