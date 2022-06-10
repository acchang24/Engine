#include "stdafx.h"
#include "App.h"
#include <iomanip>
#include "Graphics.h"
#include "EngineMath.h"
#include "Shader.h"
#include "RenderObj.h"
#include "VertexBuffer.h"

#define WINWIDTH 1280
#define WINHEIGHT 720

App::App()
	: mShader(nullptr)
	, mCubeShader(nullptr)
	, testCube(nullptr)
	, testCube2(nullptr)
	, mConstColorBuffer(nullptr)
{
	wnd = new Window(WINWIDTH, WINHEIGHT, L"Engine");
	running = true;
}

App::~App()
{

}

void App::Init()
{
	const Vertex vertices[] =
	{
		{ -1.0f, -1.0f, -1.0f, Color4(1.0f, 0.0f, 0.0f, 1.0f)},
		{ 1.0f, -1.0f, -1.0f, Color4(0.0f, 1.0f, 0.0f, 1.0f)},
		{ -1.0f, 1.0f, -1.0f, Color4(0.0f, 0.0f, 1.0f, 1.0f)},
		{ 1.0f, 1.0f, -1.0f, Color4(1.0f, 1.0f, 0.0f, 1.0f)},
		{ -1.0f, -1.0f, 1.0f, Color4(1.0f, 0.0f, 1.0f, 1.0f)},
		{ 1.0f, -1.0f, 1.0f, Color4(0.0f, 1.0f, 1.0f, 1.0f)},
		{ -1.0f, 1.0f, 1.0f, Color4(0.0f, 0.0f, 0.0f, 1.0f)},
		{ 1.0f, 1.0f, 1.0f, Color4(1.0f, 1.0f, 1.0f, 1.0f)},
	};

	const uint16_t indices[] =
	{
		0, 2, 1,
		2, 3, 1,
		1, 3, 5,
		3, 7, 5,
		2, 6, 3,
		3, 6, 7,
		4, 5, 7,
		4, 7, 6,
		0, 4, 2,
		2, 4, 6,
		0, 1, 4,
		1, 5, 4,
	};

	const Color4 cb2[] =
	{
		{Color4(1.0f, 0.0f, 1.0f, 1.0f)},
		{Color4(1.0f, 0.0f, 0.0f, 1.0f)},
		{Color4(0.0f, 1.0f, 0.0f, 1.0f)},
		{Color4(0.0f, 0.0f, 1.0f, 1.0f)},
		{Color4(1.0f, 1.0f, 0.0f, 1.0f)},
		{Color4(0.0f, 1.0f, 1.0f, 1.0f)},
	};

	// Create a render obj
	testCube = new RenderObj(new VertexBuffer(vertices, sizeof(vertices), sizeof(Vertex), indices, sizeof(indices), sizeof(uint16_t)));
	testCube2 = new RenderObj(new VertexBuffer(vertices, sizeof(vertices), sizeof(Vertex), indices, sizeof(indices), sizeof(uint16_t)));

	// Shader
	mShader = new Shader(wnd->GetGraphics());

	mCubeShader = new Shader(wnd->GetGraphics());

	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	const D3D11_INPUT_ELEMENT_DESC colorIed[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	mShader->Load(L"Engine/Shaders/VertexShader.hlsl", ShaderType::Vertex, ied, sizeof(ied) / sizeof(ied[0]));
	mShader->Load(L"Engine/Shaders/PixelShader.hlsl", ShaderType::Pixel, ied, sizeof(ied) / sizeof(ied[0]));

	mCubeShader->Load(L"Engine/Shaders/CubeVS.hlsl", ShaderType::Vertex, colorIed, sizeof(colorIed) / sizeof(colorIed[0]));
	mCubeShader->Load(L"Engine/Shaders/CubePS.hlsl", ShaderType::Pixel, colorIed, sizeof(colorIed) / sizeof(colorIed[0]));

	// Create the const color buffer
	mConstColorBuffer = wnd->GetGraphics()->CreateGraphicsBuffer(&cb2, sizeof(cb2), 0, D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
}

void App::ShutDown()
{
	
	if (mShader)
	{
		delete mShader;
	}
	if (mCubeShader)
	{
		delete mCubeShader;
	}
	if (testCube)
	{
		delete testCube;
	}
	if (testCube2)
	{
		delete testCube2;
	}
	if (mConstColorBuffer)
	{
		mConstColorBuffer->Release();
	}

	// Delete/release stuff before window
	if (wnd)
	{
		delete wnd;
	}
}

int App::Run()
{
	// Initialize all game objects, shaders, etc.
	Init();

	MSG msg = {};

	// Get the current time
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

	while (running)
	{
		// Check to see if any messages are waiting in the queue
		while ((PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			switch (msg.message)
			{
			case WM_QUIT:
				running = false;
				break;
			}

			static int i = 0;
			while (!wnd->mMouse->IsEmpty())
			{
				const auto e = wnd->mMouse->Read();
				switch (e.GetType())
				{
				/*case Mouse::Event::Type::Leave:
					wnd->SetTitle(L"GONE");
					break;
				case Mouse::Event::Type::Move:
					{
						std::ostringstream oss;
						oss << "Mouse Position: (" << e.GetX() << ", " << e.GetY() << ")";
						std::string s = oss.str();
						std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
						std::wstring ws = converter.from_bytes(s);
						wnd->SetTitle(ws);
					}
					break;*/
				case Mouse::Event::Type::WheelUp:
					zoom += 0.2f;
					break;
				case Mouse::Event::Type::WheelDown:
					zoom -= 0.2f;
					break;
				}
			}
			/*if (wnd->mKeyboard->KeyIsPressed('A'))
			{
				MessageBoxW(nullptr, L"Something HapponNN!", L"A Key Was Pressed", MB_OK | MB_ICONEXCLAMATION);
			}*/
		}

		if (running)
		{
			std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
			double duration = (double)std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
			float deltaTime = (float)(0.000000001 * duration);
			start = end;


			//time += deltaTime;
			fps = (int)(1.0f / deltaTime);
			/*std::ostringstream oss;
			oss << "FPS: " << fps;
			std::string str = oss.str();
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
			std::wstring ws = converter.from_bytes(str);
			wnd->SetTitle(ws);*/

			Update(deltaTime);
			RenderFrame();
		}
	}

	ShutDown();

	return (int)msg.wParam;
}

void App::Update(float deltaTime)
{
	angle += Math::Pi * deltaTime;

	Matrix4 transform = Matrix4::CreateRotationZ(0.0f) * Matrix4::CreateRotationY(angle) * Matrix4::CreateRotationX(0.25f * angle)
		* Matrix4::CreateTranslation(Vector3(0.0f, 0.0f, 0.0f + 4.0f))
		* Matrix4::CreatePerspectiveFOV(Math::ToRadians(100.0f), 1.0f, 9.0f / 16.0f, 1.0f, 10000.0f);
	transform.Transpose();
	testCube->mObjConsts.modelToWorld = transform;

	Matrix4 transform2 = Matrix4::CreateRotationZ(0.0f) * Matrix4::CreateRotationY(-angle) * Matrix4::CreateRotationX(0.25f * -angle)
		* Matrix4::CreateTranslation(Vector3(wnd->mMouse->GetPosX() / (wnd->GetGraphics()->GetScreenWidth() /2.0f) - 1.0f, -wnd->mMouse->GetPosY() / (wnd->GetGraphics()->GetScreenHeight()/2.0f) + 1.0f, zoom + 4.0f))
		* Matrix4::CreatePerspectiveFOV(Math::ToRadians(100.0f), 1.0f, 9.0f / 16.0f, 1.0f, 10000.0f);
	transform2.Transpose();
	testCube2->mObjConsts.modelToWorld = transform2;
}

void App::RenderFrame()
{
	Graphics* g = wnd->GetGraphics();

	// set render target
	g->SetBuffer(g->GetBackBuffer(), g->GetDepthStencilView());

	{
		// clear buffers
		g->ClearBuffer(0.0f, 0.0f, 0.0f);

		g->ClearDepthBuffer(g->GetDepthStencilView(), 1.0f);
	}

	mShader->SetActive();

	testCube->Draw();

	//mCubeShader->SetActive();

	testCube2->Draw();

	// Set color const to pixel shader
	g->GetContext()->PSSetConstantBuffers(0, 1, &mConstColorBuffer);

	g->EndFrame();
}