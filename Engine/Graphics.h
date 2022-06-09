#pragma once
#include <d3d11.h>
#include "VertexFormats.h"
#include "LoopTimer.h"
#include "EngineMath.h"

struct ConstBuffer
{
	Matrix4 modelToWorld;
};

class Shader;

class Graphics
{
public:
	Graphics();
	~Graphics();
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;

	void InitD3D(HWND hWnd, float width, float height);

	void SetBuffer(ID3D11RenderTargetView* targetBuffer, ID3D11DepthStencilView* dsv);
	void ClearBuffer(const Color4& clearColor);
	void ClearBuffer(float red, float green, float blue);
	ID3D11RenderTargetView* GetBackBuffer() { return mBackBuffer; }

	// Present swap chain
	void EndFrame();

	// Getters for D3D Device and Device Context
	ID3D11Device* GetDevice() { return mDevice; }
	ID3D11DeviceContext* GetContext() { return mContext; }

	// Create adn upload a buffer
	ID3D11Buffer* CreateGraphicsBuffer(const void* initData, UINT byteWidth, UINT structByteStride, D3D11_BIND_FLAG inBindFlag, D3D11_CPU_ACCESS_FLAG inCPUAccessFlags, D3D11_USAGE inUsage);
	void UploadBuffer(ID3D11Buffer* buffer, const void* data, UINT dataSize);

	// Viewport setter
	void SetViewport(float x, float y, float width, float height);

	// Create a depth-stencil state
	ID3D11DepthStencilState* CreateDepthStencilState(D3D11_COMPARISON_FUNC inDepthComparisonFunction);

	// Create depth stencil
	bool CreateDepthStencil(int inWidth, int inHeight,
		ID3D11Texture2D** pDepthTexture, ID3D11DepthStencilView** pDepthView);

	// Getter for depth stencil view
	ID3D11DepthStencilView* GetDepthStencilView() { return mDepthStencilView; }

	// Clear z buffer
	void ClearDepthBuffer(ID3D11DepthStencilView* depthView, float depth);

	void DrawTestTriangle(float x, float y, float dir);

private:
	// TEST TRIANGLE STUFF---------------------
	ID3D11Buffer* mTriVertexBuffer;
	Shader* mShader;
	ID3D11Buffer* mIndexBuffer;
	ID3D11Buffer* mConstBuffer;
	ID3D11Buffer* mConstColorBuffer;

	Shader* mCubeShader;

	// Z-buffer
	ID3D11Texture2D* mDepthTexture;
	ID3D11DepthStencilView* mDepthStencilView;


	float angle = 0.0f;
	ConstBuffer cb;

	// Normal stuff
	IDXGISwapChain* mSwapChain;
	ID3D11Device* mDevice;
	ID3D11DeviceContext* mContext;
	
	ID3D11RenderTargetView* mBackBuffer;
	ID3D11RenderTargetView* mCurrentRenderTarget;

	LoopTimer time;
};

