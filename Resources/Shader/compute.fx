#ifndef _COMPUTE_FX_
#define _COMPUTE_FX_

#include "params.fx"

//unordered eccese view ��ǻƮ ���̴� ������ �������� �Ϲ����� ���̴����� ������� �ʴ´�.
//���ݱ����� �д¿뵵�θ� ��� ���������� �� ģ���� �б� ���� ��� �����ϴ�.
RWTexture2D<float4> g_rwtex_0 : register(u0);

// ������ �׷�� ������ ����
// max : 1024 (CS_5.0)
// - �ϳ��� ������ �׷��� �ϳ��� ����ó���⿡�� ����
//��ǻƮ ���̵����� ����� Dispath�Լ����� �ش� ������ �־��ش�.
[numthreads(1024, 1, 1)]// �� ���ڸ� ���� ���Ѱ��� �� ������(�ϰ�)�� ����
void CS_Main(int3 threadIndex : SV_DispatchThreadID)
{
    //������ ����, ������ �ʷϻ�
    if (threadIndex.y % 2 == 0)
        g_rwtex_0[threadIndex.xy] = float4(1.f, 0.f, 0.f, 1.f);
    else
        g_rwtex_0[threadIndex.xy] = float4(0.f, 1.f, 0.f, 1.f);
}

#endif