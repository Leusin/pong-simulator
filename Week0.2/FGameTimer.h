#pragma once

#include <chrono>

/*
* chrono ���̺귯���� ���� Ÿ�̸� ����
*/
class FGameTimer
{
public:
	FGameTimer();

	float GetGameTime() const;
	float GetDeltaTime() const;
	float GetRunTime() const;

	// �޽��� ���� ������ ȣ���ؾ���.
	void Reset();
	void Start();

	// �̹� ���� ���¸� �̹� �ϵ� ���� �ʴ´�.
	void Stop();

	// �� �����Ӹ��� ȣ���ؾ� ��
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