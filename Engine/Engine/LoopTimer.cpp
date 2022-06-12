#include "stdafx.h"
#include "LoopTimer.h"

LoopTimer::LoopTimer()
{
	last = std::chrono::steady_clock::now();
}

float LoopTimer::Mark()
{
	const auto old = last;
	last = std::chrono::steady_clock::now();
	const std::chrono::duration<float> frameTime = last - old;
	return frameTime.count();
}

float LoopTimer::Peek() const
{
	return std::chrono::duration<float>(std::chrono::steady_clock::now() - last).count();
}