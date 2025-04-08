# 입자 시뮬레이션

## 1. SPH 

### 1.1. 개요

- SPH(Smooth Particle Hydrodynamics)는 유체 시뮬레이션 기법 중 하나
- 유체를 `연속적인 매질`이 아닌, `개별 입자`들의 집합으로 모델링
- 이 때, `개별 입자`간 특성(거리)을 이용해 유체 중 특정 위치의 밀도를 계산하는 것이 SPH 알고리즘
- 다시 말해, `입자` 간 상호작용을 통해 유체의 거동을 표현
  - 따라서, 3D로 차원이 확장되면 연산량이 급격하게 늘어남

### 1.2. 수식

#### 1.2.1. Navie-Stock 방정식

$$
\frac{d\textbf{v}_i}{dt} = -\frac{1}{\rho_i}\nabla{p_i} + v\nabla^2\textbf{v}_i + \frac{\textbf{F}^{other}_{i}}{m_i}
$$

- $-\frac{1}{\rho_i}\nabla{p_i}$ : 유체 속에서 압력 미분에 의한 입자 가속도 항
  - 유체의 부피를 보존하기 위해 사용
  - 비압축성 특성을 구현
- $v\nabla^2\textbf{v}_i$ : 입자 사이의 마찰력으로 인한 가속도 항
  - 유체의 마찰력을 구현
- $\frac{\textbf{F}^{other}_{i}}{m_i}$ : 중력에 의한 가속도 항


#### 1.2.2. 공간적 미분 계산

- 물리량 A에 대한 공간적 미분 계산 방법은 아래와 같다.

$$
\nabla{A_i} = \rho_{i} \sum_{j}m_{j}\left( \frac{A_i}{\rho^{2}_i} + \frac{A_j}{\rho^{2}_j}\right)\nabla{W_{ij}} \\

\nabla \cdot A_{i} = -\frac{1}{\rho_i}\sum_{j}m_{j}A_{ij} \cdot \nabla{W_{ij}} \\ 
\nabla^{2}A_{i} = 2\sum_{j}\frac{m_j}{\rho_{j}} A_{ij} \frac{x_{ij} \cdot \nabla{W_{ij}}}{x_{ij} \cdot x_{ij} + 0.01h^{2}}
$$
- $A_{ij} = A_{i} - A{j} 이다.$

#### 1.2.3. 보간법

- 공간에 대한 물리량을 근사하는 방법
$$A_{i} = \sum_{j}\frac{m_j}{\rho_j}A_jW_{ij}$$
- $x_{i}$ : 특정 위치
- $A_{i}$ : $x_{j}$ 위치애서 알 수 있는 이웃한 입자들의 양 $A_{j}$으로 근사된 양
- $\rho$ : 밀도
- $m$ : 질량
- $W_{ij}$ : 가중치(거리에 반비례)



#### 1.2.1. 비압축성


#### 1.2.2. 점성

- 입자들 간 속도 차이를 줄여주는 메커니즘
- 빠르면 느리게, 느리면 빠르게 하여 속도를 균일하게 조정하는 것을 목적으로 함




## 2. 유체의 특성

### 2.1. 비압축성(Incomresibility)

