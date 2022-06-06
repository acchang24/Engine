#pragma once
#include "Window.h"
#include "LoopTimer.h"

class App
{
public:
	App();
	~App();
	App(const App&) = delete;
	App& operator=(const App&) = delete;

	void Init();

	// Start game loop
	int Go();
private:
	void RenderFrame();

	bool running;
	Window wnd;
	LoopTimer timer;
};