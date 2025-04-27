# Mouse Interaction

목차
1. [Moler Trumbore Intersection](#1-moller-trumbore-intersection)
    - [Barycentric 좌표](#11-barycentric-좌표)
    - [Ray 방정식과 Barycentric 좌표계 통합](#12-ray-방정식과-barycentric-좌표계-통합)
    - [Cramer's Rule을 활용한 전개](#13-cramers-rule을-활용한-전개)
2. [GPU Intersection]
    - ㅇㅇㅇ?

## 1. Moller Trumbore Intersection

### 1.1. Barycentric 좌표

- Convex Combination
    - 개념
        - 점의 선형 조합을 의미한다.
        - 삼각형의 세 점을 활용해 삼각형 내 점의 위치를 찾아낸다.
        - 이 때, 가중치 `w`, `u`, `v`를 찾아내는 것이 핵심이다.

    - 수식 정리:
$$
P = \mathscr{w} v_1 + \mathscr{u} v_2 + \mathscr{v} v_3
$$

$$
P = (1 - \mathscr{u} - \mathscr{v}) v_1 + \mathscr{u} v_2 + \mathscr{v} v_3
$$

$$
P = v_1 + \mathscr{u} (v_2 - v_1) + \mathscr{v} (v_3 - v_1)
$$
> **참고사항**
> - `P`는 삼각형 위의 임의의 점이다.
> - 변은 다음과 같이 정의된다:
>   $$
>   \vec{e1} = v_2 - v_1, \quad \vec{e2} = v_3 - v_1
>   $$

<br/>
<br/>

### 1.2. Ray 방정식과 Barycentric 좌표계 통합

- Ray 방정식을 통한 Convex combination 수식 개선
    - 개념
        - 삼각형 위 임의의점 P를 광선 방정식으로 대입
    - 수식 정리:
$$
P = O + tD
$$

$$
O + tD = v_1 + \mathscr{u} (v_2 - v_1)  +  \mathscr{v} (v_3 - v_1)
$$

$$
O - v_1 = - tD + \mathscr{u} (v_2 - v_1)  +  \mathscr{v} (v_3 - v_1)
$$

$$
{
    {
        \begin{bmatrix}
            \vert &\vert &\vert \\
            -D&(v2-v1)&(v3-v1)\\
            \vert &\vert &\vert 
        \end{bmatrix}
    }
    {
        \begin{bmatrix}
        t\\
        u\\
        v
        \end{bmatrix}
    }
    =O-v_{1}
}
$$
> **참고사항**
> - $Ray = O + tD$ 광선을 P에 대입


<br/>
<br/>

### 1.3. Cramer's Rule을 활용한 전개
- 개요
    - $Ax=B$ 수식이 있을 때, B 벡터를 이용해 미지수 x를 계산
    - -D, $v_2-v_1$, $v_3-v_1$이 서로 선형 독립함을 가정한다.
        - **선형 독립이란?**
            - $det(...) \neq  0$이면 선형 독립이다.
                - 필요충분조건
            - 0벡터를 만들기 위해 모두 0배 하는 것 밖에 방법밖에 없으면 선형 독립이다.
                - 필요충분조건
            - Orthogonal하면 선형 독립이다.
                - 충분조건
        - **스칼라 삼중적의 순환 대칭성**
            - $A \cdot (B \times C) = B \cdot (C \times A) = C \cdot (A \times B) $
        - **외적의 반대칭성**
- 수식 정리:
$$
t = 
\frac{det(O - v_1, v_2-v_1, v_3-v_1)}{det(-D, v_2-v_1, v_3-v_1)} = 
(v_3 - v_1) \cdot ((O - v_1) \times (v_2 - v_1))
$$

$$
u = 
\frac{det(-D, O - v_1, v_3-v_1)}{det(-D, v_2-v_1, v_3-v_1)} = 
(O - v_1) \cdot (D \times (v_3 - v_1))
$$

$$
v = 
\frac{det(-D, v_2-v_1, O - v_1)}{det(-D, v_2-v_1, v_3-v_1)} = 
D \cdot ((O - v_1) \times (v_2 - v_1))
$$

- 예시로 u 수식 전계

$$
det(-D, O - v_1, v_3-v_1) = (-D) \cdot ((O - v_1) \times (v_3 - v_1))
$$

$$
(-D) \cdot ((O - v_1) \times (v_3 - v_1)) = D \cdot ((v_3 - v_1) \times (O - v_1))
$$

$$
따라서 \\ 
det(-D, O - v_1, v_3-v_1) = D \cdot ((v_3 - v_1) \times (O - v_1))
$$

$$
D \cdot ((v_3 - v_1) \times (O - v_1)) = (O - v_1) \cdot (D \times (v_3 - v_1))
$$

> **참고사항**
> - 단, 삼격형과 광선이 평행한 경우 성립하지 않음을 유의한다.(det값이 0에 근사한 경우)
> - $det(a,b,c) = a \cdot (b \times c)$ 이다. (scalar triple product)

<br/>
<br/>

 