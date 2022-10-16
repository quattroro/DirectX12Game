#ifndef _SKYBOX_HLSLI_
#define _SKYBOX_HLSLI_

#include "params.hlsli"

//�̰͵��� Shader.cpp ���� �־��ش�.
struct VS_IN
{
    float3 localPos : POSITION;
    float2 uv : TEXCOORD;
};

struct VS_OUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;

};

//���� ���̴�
VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output = (VS_OUT)0;

    //��ǥ ����
    // Translation�� ���� �ʰ� Rotation�� �����Ѵ�
    float4 viewPos = mul(float4(input.localPos, 0), g_matView);
    float4 clipSpacePos = mul(viewPos, g_matProjection);//�������� �ƴϰ� ������ǥ�� ��������.

    //���� ����
    //��ī�̹ڽ��� �׻� ���̰� 1�̿��� �ϱ� ����
    // w/w=1�̱� ������ �׻� ���̰� 1�� �����ȴ�
    output.pos = clipSpacePos.xyww;
    output.uv = input.uv;

    return output;
}

//�ȼ� ���̴�
float4 PS_Main(VS_OUT input) : SV_Target
{
     float4 color = g_tex_0.Sample(g_sam_0, input.uv);
     return color;
}

#endif 