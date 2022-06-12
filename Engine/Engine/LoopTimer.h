#pragma once

class LoopTimer
{
public:
	LoopTimer();
	float Mark();
	float Peek() const;
private:
	std::chrono::steady_clock::time_point last;
};

