#pragma once
#include <cstdint>

/*
* Windows�� �����ϴ� ���� Ÿ�̸�
* (performance timer)�� ���� ����
*/
class GameTimer
{
public:
	GameTimer();

	float GetGameTime() const;
	float GetDeltaTime() const;
	float GetTotalTime() const;

	// �޽��� ���� ������ ȣ���ؾ���.
	void Reset();
	void Start();

	// �̹� ���� ���¸� �̹� �ϵ� ���� �ʴ´�.
	void Stop();

	// �� �����Ӹ��� ȣ���ؾ� ��
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

