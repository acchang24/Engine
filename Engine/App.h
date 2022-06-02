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

	// Start game loop
	int Go();
private:
	void RenderFrame();

	bool running;
	Window wnd;
	LoopTimer timer;
};