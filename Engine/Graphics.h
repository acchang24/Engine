#pragma once
#include <d3d11.h>
#include "VertexFormats.h"



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

	void DrawTestTriangle();

private:
	// TEST TRIANGLE STUFF---------------------
	ID3D11Buffer* mTriVertexBuffer;
	ID3D11VertexShader* mTriangleVertexShader;
	ID3D11PixelShader* mTrianglePixelShader;
	ID3DBlob* mVSBlob;
	ID3DBlob* mPSBlob;
	ID3D11InputLayout* mInputLayout;
	ID3D11Buffer* mIndexBuffer;

	// Normal stuff
	IDXGISwapChain* mSwapChain;
	ID3D11Device* mDevice;
	ID3D11DeviceContext* mContext;
	
	ID3D11RenderTargetView* mBackBuffer;
};

