#pragma once
#include "Window.h"
#include "LoopTimer.h"

// App is the main class that starts the game loop/engine

class Shader;
class RenderObj;

class App
{
public:
	App();
	~App();
	App(const App&) = delete;
	App& operator=(const App&) = delete;

	void Init();
	void ShutDown();

	// Start game loop
	int Run();

	void Update(float deltaTime);
	void RenderFrame();

private:
	RenderObj* testCube;
	RenderObj* testCube2;
	Shader* mShader;
	Shader* mCubeShader;
	ID3D11Buffer* mConstColorBuffer;

	float angle = 0.0f;
	float zoom = 0.0f;

	int fps = 0;

	bool running;
	Window* wnd;
	LoopTimer timer;
};