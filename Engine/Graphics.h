#pragma once
#include <d3d11.h>
#include "wrl.h"

//namespace wrl = Microsoft::WRL;


struct Vertex
{
	float x, y;
};
const Vertex vertices[] =
{
	{ 0.0f, 0.5f},
	{0.5f, -0.5f},
	{-0.5f, -0.5f}
};

class Graphics
{
public:
	class Color4
	{
	public:
		Color4()
			:r(0.0f), g(0.0f), b(0.0f), a(0.0f)
		{}
		Color4(float _r, float _g, float _b, float _a = 1.0f)
			:r(_r), g(_g), b(_b), a(_a)
		{}
		float r, g, b, a;
	};

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

	// Normal stuff
	IDXGISwapChain* mSwapChain;
	ID3D11Device* mDevice;
	ID3D11DeviceContext* mContext;
	
	ID3D11RenderTargetView* mBackBuffer;
};

