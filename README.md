# Graphics Research Tools
## 0. 목록
### 1. Lighting

| 순번 | 조명 효과 |  비고 | 
| :---: |:---:|:---:| 
| 1.1. | Blinn-Phong |  | 
| 1.2. | PBR |  | 
| 1.3. | Fresnel Effect |  | 
| 1.4. | Rim Effect |  | 

### 2. 응용

| 순번 | 유틸리티 기능 효과 |  비고 | 
|:------------:|:----:|:----:| 
| 2.1. | Gizmo | Triangle, Sphere Intersection Algorithm | 
| 2.2. | Octomap | Geometry Shader 활용한 Octomap 랜더링 | 


### 3. 셰이더
#### 3.1. Vertex

#### 3.2. Pixel(fragment)

#### 3.3. Geometry

#### 3.4. Compute


### 4. 용어 정리

1. Graphics
    1. Texture 종류
    2. MASS
    3. ㅇㅇㅇ
2. 수학
    1. 좌표계 상호 변환 기법(구, 카르테시안)
    2. 변환
3. 물리

<br/>
<br/>

## 1. Light
- 빛이 매질에 충돌 시 1) 정반사가 발생하거나 물질 내부로 흡수된다. 
- 흡수되는 경우, 일부는 분자 단위의 충돌이 발생해 2) 난반사가 일어나거나 3) 감쇠한다.

<img src="./screenshot/LightTreePNG.PNG" />

### 1.1. Blinn-Phong

### 1.2. PBR(Physical-Based Rendering)
- 빛의 현상을 실제 물리법칙에 근거하여 근사 한 것이 BRDF Light(PBR)이다.

<img src="./screenshot/PBR.png" width="680" height="480"/>

#### 1.2.1. BRDF
#### ⬜ Diffuse BRDF(Lambertian BRDF, 난반사)
>- $l$ : Surface to Light vector
>- $v$ : Surface to Eye(Camera) vector
>- $c_diff$ : Diffuse Albedo(Cubemap의 diffuseHDR Texture 활용)
>- $\pi$로 나눠 정규화(에너지 보존법칙)

$$ f(l, v)  = \frac{c_diff}{\pi} $$

#### ⬜ Micropacet Specular BRDF(Cook-Torrance BRDF, 정반사)

>- $n$ : Normal vector
>- $l$ : Surface to Light vector
>- $v$ : Surface to Eye(Camera) vector
>- $h$ : Half way vector ( ${h = normalize(l + v)}$ )
>- $k$ : 표면 거칠기
>- $\alpha$ : ${표면 거칠기^2}$
>- $F_0$ : 매질의 정반사율
>- $F(l,h)$ : Fresnel-Schlick approximation
>- $G(l,v,h)$ : Smith's Schlick-GGX Equation
>- $D(h)$ : GGX/Trowbridge-Reitz Equation
>- $G_1(vector)$ : Schlick Equation

$$ 
\begin{aligned}
&f(l, v)  = \frac{F(l,h)G(l,v,h)D(h)}{4(m \cdot l)(n \cdot v )} \\ \\ 
&F(l,h) = F_0 + (1 - F_0) \cdot 2^{(-5.55473(v \cdot h) - 6.98316)(v \cdot h)} \\ \\
&G(l,v,h) = G_1(l)G_1(v)  \\ \\
&D(h) = \frac{\alpha ^2}{\pi ((n \cdot h)^2(\alpha^2 - 1) + 1)} \\ \\
&G_1(vector) = \frac{n \cdot vector}{(n \cdot vector)(1-k) + k} \\ \\
\end{aligned}
$$

#### ⬜ 최종 합산
```hlsl
float3 diffuseBRDF = ...;
float3 specularBRDF = ...;
float3 color (diffuseBRDF + specularBRDF);
```

### 1.3. Fresnel Effect

### 1.4. Rim Effect

<br/>
<br/>

## 2. Utilities
### 2.1. Ray-Casting
#### 2.1.1. Triangle Intersection

#### 2.1.2. Shpere Intersection

<br/>
<br/>


### 3. 셰이더
#### 3.1. Vertex

#### 3.2. Pixel(fragment)

#### 3.3. Geometry

#### 3.4. Compute
##### 3.4.1. 개념 설명
##### ⬜ 개요
- GPU 연산을 위한 셰이더
- Compute shader는 그래픽 파이프라인에 종속되지 않음

##### ⬜ 관련 용어

| 용어 | 설명 | 비고 | 
|:---:|:---:|:---:|
| SM | GPU의 독립적인 연산 단위 | Streaming Multiprocessors |
| SIMT | CUDA의 연산 방법, 동일 명령을 여러 쓰레드가 처리 | Single-Instruction, Multiple-Thread |
| Warp | 32개의 쓰레드 단위 실행 그룹 |  |

##### ⬜ 셰이더 키워드

| 용어 | 설명 | 비고 | 
|:---:|:---:|:---:|
| SV_DispatchThreadID | 쓰레드 고유 ID(SV_GroupID * numthreads + GroupThreadID) | DirectX11 |
| SV_GroupID | Group의 고유 ID |  |

##### 3.4.2. 예시

##### ⬜ Circle and Blur Example

<div style="display: flex; gap: 10px;">
  <img src="./screenshot/compute_circle.png" width="256" height="256"/>
  <img src="./screenshot/compute_blur.png" width="256" height="256"/>
</div>

<br/>
<br/>

## 4. 개념 정리
### 4.1. 그래픽스
#### 4.1.1. PBR Texture 종류

| 용어 | 설명 | 비고 | 
|---|---|---|
| Albedo(Color, Diffuse) | RGB 컬러 이미지 | 3D | 
| AO(Ambient Occlusion) | 영역 별 광원에 얼마나 노출되는지 판단 |  | 
| Displacement(height) | 영역 별 높낮이(z축) 정보 | 1D | 
| Normal | Normal vector 정보 | 3D | 
| Roughness | 빛의 분산 정도에 대한 정보 | 3D | 
| Opacity | 텍스쳐의 투명도 정보 | 1D | 
| BRDF | LUT, $x=Normal \cdot toEye$, $y=roughness$ | 2D |

#### 4.1.2. Buffer

##### ⬜ Staging Buffer

- CPU와 GPU 메모리의 중단다리 역할
- 각 디바이스의 메모리 복사 최적화를 도와줌

##### ⬜ Unordered Access View
- GPU 쓰레드가 동시에 읽고 쓸 수 있도록 지원하는 버퍼
- 주로 Compute shader에서 사용

##### ⬜ Structed Buffer
- Unordered Access View에서 자료구조를 커스텀 가능
- 



- GPU에서 읽기, 쓰기가 가능한 버퍼


### 4.2. 수학
#### 4.2.1. ㅇㅇ
#### 4.2.2. 변환

$$
\begin{aligned}
&R = 
\begin{bmatrix}
1-2n_x n_x & -2n_xn_y & -2n_xn_z & 0 \\
-2n_xn_y & 1-2n_yn_y & -2n_yn_z & 0 \\
-2n_xn_z & -2n_yn_z & 1-2n_zn_z & 0 \\
-2dn_x & -2dn_y & -2dn_z & 1 \\
\end{bmatrix}
\end{aligned} \\

이 행렬은 반사의 기준이되는 평면은 정규화 되어있다고 가정한다.\\

\begin{aligned}
    proj_n(p-p_0) &= [n \cdot (p-p_0)]n \\
    &= e + f + g
\end{aligned} 
$$ 