#pragma once

#include <cstdint>

/*
* ����: ���� ���� ��Ű��ó p344
*/
class FClock
{
	uint64_t TimeCycles;
	float TimeScale;
	bool bPaused;

	static float CyclesPerSecond;

	static inline uint64_t SecondsToCycles(float TimeSeconds)
	{
		return static_cast<uint64_t>(TimeSeconds * CyclesPerSecond);
	}

	// ����: ª�� �ð��� �ʷ� ��ȯ�� ���� ���.
	static inline float CyclesToSeconds(uint64_t TimeCycles)
	{
		return (float)TimeCycles / CyclesPerSecond;
	}

public:
	static void Init();

	explicit FClock(float StartTImeSeconds = 0.0f);

	// ���� �ð��� �ֱ������ ����.
	uint64_t GetTimeCycles() const;

	// �� Ŭ�ϰ� �ٸ� Ŭ���� ���� �ð� ���̸� ���� �� �� ������ ����.
	float CalcDeltaSeconds(const FClock& Other);

	// ������ �� �� ���� �ҷ��� ��.
	// ������ ������ ������ �ð��� �ʴ����� ����.
	void Update(float DeltaRealSeconds);

	void SetPaused(bool WantPaused);

	bool IsPaused()const;

	void SetTimeScale(float Scale);

	float GetTimeScale() const;

	void SingleStep();
};

