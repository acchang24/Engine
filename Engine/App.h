#pragma once
#include "Window.h"
#include "LoopTimer.h"

class App
{
public:
	App();
	~App();

	// Start game loop
	int Go();
private:
	void DoFrame();

	bool running;
	Window wnd;
	//LoopTimer timer;
};

