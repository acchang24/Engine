#pragma once
#include <d3d11.h>

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

private:
	IDXGISwapChain* mSwapChain;
	ID3D11Device* mDevice;
	ID3D11DeviceContext* mDevCon;
	
	ID3D11RenderTargetView* mBackBuffer;
};

