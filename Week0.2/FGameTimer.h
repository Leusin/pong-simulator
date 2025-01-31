#pragma once

#include <chrono>

/*
* chrono 라이브러리를 통해 타이머 구현
*/
class FGameTimer
{
public:
	FGameTimer();

	float GetGameTime() const;
	float GetDeltaTime() const;
	float GetRunTime() const;

	// 메시지 루프 이전에 호출해야함.
	void Reset();
	void Start();

	// 이미 정지 상태면 이무 일도 하지 않는다.
	void Stop();

	// 매 프레임마다 호출해야 함
	void Tick(); 

private:
	using Clock = std::chrono::high_resolution_clock;
	using TimePoint = std::chrono::time_point<Clock>;

	double DeltaTime;

	TimePoint StartTime;
	TimePoint BaseTime;
	TimePoint PausedTime;
	TimePoint StopTime;
	TimePoint PrevTime;
	TimePoint CurrentTime;

	bool bStopped;
};