#ifndef _COMPUTE_FX_
#define _COMPUTE_FX_

#include "params.fx"

//unordered eccese view 컴퓨트 셰이더 전용의 레지스터 일반적인 셰이더에서 사용하지 않는다.
//지금까지는 읽는용도로만 사용 가능했지만 이 친구는 읽기 쓰기 모두 가능하다.
RWTexture2D<float4> g_rwtex_0 : register(u0);

// 쓰레드 그룹당 쓰레드 개수
// max : 1024 (CS_5.0)
// - 하나의 쓰레드 그룹은 하나의 다중처리기에서 실행
//컴퓨트 셰이딩에서 사용할 Dispath함수에서 해당 값들을 넣어준다.
[numthreads(1024, 1, 1)]// 세 숫자를 전부 곱한것이 촐 스레드(일감)의 개수
void CS_Main(int3 threadIndex : SV_DispatchThreadID)
{
    //한줄은 빨강, 한줄은 초록색
    if (threadIndex.y % 2 == 0)
        g_rwtex_0[threadIndex.xy] = float4(1.f, 0.f, 0.f, 1.f);
    else
        g_rwtex_0[threadIndex.xy] = float4(0.f, 1.f, 0.f, 1.f);
}

#endif