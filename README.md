# Graphics Research Tools

## 1. INDEX

### 1.2. Shader Effect

| 효과 이름 | 비고 | 
|---|---|
| [Fresnel Effect](#21-fresnel-effect) |  | 
| [Rim Effect](#22-rim-effect) |  | 

## 2. Shader Effect Effect

### 2.1. Fresnel Effect [(Wiki)](https://en.wikipedia.org/wiki/Fresnel_equations)
  * 수직으로 내려다 보면 투과
  * 시야각이 90도에 다다를 수록 반사
  * 바다에서 발 아래 수면은 투과가 되어 내부가 잘 보이지만, 지평선에 가까워 질 수록 태양빛이 반사

### 2.2. Rim Effect
  * 객체의 경계선 부근을 강조하는 효과
  * $ v = normal \cdot toEye$ 의 결과가 0에 가까울수록 Rim color를 출력

  