# 1. 프로젝트 소개

![Image](https://github.com/user-attachments/assets/9304e96f-1e85-4476-8f96-5de72acb94bf)

<br>

## 1) 프로젝트 설명

| **게임 이름** | Pong Simulator |
| :--- | :--- |
| **플랫폼** | Windows |
| **장르** | 2D 물리 시뮬레이션 |
| **소개** | "DirectX11과 WinAPI를 사용하여 만든 간단한 Pong 스타일의 물리 시뮬레이터. |

<br>

## 2) 프로젝트 목표
- DirectX11을 활용한 그래픽 렌더링 학습
- 게임 루프 설계 및 프레임 관리 학습

<br>

# 2. 기술스택

- **프로그래밍 언어**: C++14
- **개발 도구**: Visual Studio 2022
- **API**: WinAPI, DirectX11

<br>

# 3. 핵심 기능

- **DirectX11 기반의 그래픽 렌더링**
    - Direct3D를 이용한 패널과 공의 렌더링
    - 기본적인 셰이더 처리
- **게임 루프 설계**
    - 고정 시간 스텝(Fixed Time Step) 방식으로 업데이트 처리
    - 델타 타임을 고려한 부드러운 공 이동
- **WinAPI를 활용한 입력 처리**
    - **`GetAsyncKeyState`** 를 활용한 키보드 입력
- **간단한 물리 시뮬레이션**
    - 원과 직사각형의 충돌 감지
    - 벽과 패널에 부딪힐 때 공의 반사 처리

<br>

> **게임 플레이 방식**
> 
1. 공은 자동으로 이동하며 벽과 패들에 부딪힐 때 반사됩니다.
2. **`W`**, **`S`** 키와 **`↑`**, **`↓`** 키를 사용해 각각 왼쪽과 오른쪽 패들을 위아래로 움직일 수 있습니다.

<br>

# 4. UML 클래스 다이어그램
![Image](https://github.com/user-attachments/assets/2c145b6d-c8dc-4fba-aed9-09c3ba85a63a)
