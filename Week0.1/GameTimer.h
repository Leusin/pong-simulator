#pragma once
#include <cstdint>

/*
* Windows가 제공하는 성능 타이머
* (performance timer)를 통해 구현
*/
class GameTimer
{
public:
	GameTimer();

	float GetGameTime() const;
	float GetDeltaTime() const;
	float GetTotalTime() const;

	// 메시지 루프 이전에 호출해야함.
	void Reset();
	void Start();

	// 이미 정지 상태면 이무 일도 하지 않는다.
	void Stop();

	// 매 프레임마다 호출해야 함
	void Tick(); 

private:
	double SecondsPerCount;
	double DeltaTime;

	int64_t BaseTime;
	int64_t PausedTime;
	int64_t StopTime;
	int64_t PrevTime;
	int64_t CurrentTime;

	bool bStopped;
};

