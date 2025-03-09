# Graphics Research Tools

## 1. 유틸리티 기능 구현

그래픽스에서 사용되는 유틸리티 기능 구현 목록

| 기능 | 비고 | 
|---|---|
| [Object Picking](#11-object-picking) |  | 

### 1.1. Object Picking


## 2. Shader Effect

셰이더를 활용한 이팩트 실험

| 효과 이름 | 예제 | 셰이더 | 
|---|---|---|
| [Blinn Phong Effect](#21-blinn-phong-effect) | `examples/BlinnPhong` | Pixel(Fragment) | 
| [Fresnel Effect](#22-fresnel-effect) | `examples/BlinnPhong` | Pixel(Fragment) | 
| [Rim Effect](#23-rim-effect) | `examples/BlinnPhong` | Pixel(Fragment) |  
| [Billboard Effect](#24-billboard-effect) | `examples/billboard` | Geometry |  

### 2.1. Blinn Phong Effect
  * 

### 2.2. Fresnel Effect [(Wiki)](https://en.wikipedia.org/wiki/Fresnel_equations)
  * 수직으로 내려다 보면 투과
  * Object Normal과 시각 벡터 사이각이 90도에 다다를 수록 반사
  * 바다에서 발 아래 수면은 투과가 되어 내부가 잘 보이지만, 지평선에 가까워 질 수록 태양빛이 반사되는 현상

### 2.3. Rim Effect
  * 객체의 경계선 부근을 강조하는 효과
  * $ v = normal \cdot toEye$ 의 결과가 0에 가까울수록 Rim color를 출력

### 2.4. Billboard Effect
  * Vertex 데이터를 geometry shader에서 추가