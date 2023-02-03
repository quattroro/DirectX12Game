#ifndef _TESSELLATION_FX_
#define _TESSELLATION_FX_

#include "params.fx"

// --------------
// Vertex Shader
// --------------

struct VS_IN
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct VS_OUT
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output = input;

    return output;
}

// --------------
// Hull Shader
// --------------

struct PatchTess
{
    float edgeTess[3] : SV_TessFactor;
    float insideTess : SV_InsideTessFactor;
};

struct HS_OUT
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

// Constant HS
//각각의 엣지별로 실행
PatchTess ConstantHS(InputPatch<VS_OUT, 3> input, int patchID : SV_PrimitiveID)
{
    PatchTess output = (PatchTess)0.f;

    //dkokjkjojiokjhy

    //이걸 이용해소 동적LOD를 구현한다
    output.edgeTess[0] = 1;//0번 엣지는 분할하지 않는다
    output.edgeTess[1] = 2;//1번 엣지는 2개로 분할한다
    output.edgeTess[2] = 3;//2번 엣지는 3개로 분할한다
    output.insideTess = 3;//내부에 삼각형

    return output;
}

// Control Point HS
[domain("tri")] // 패치의 종류 (tri, quad, isoline)
[partitioning("integer")] // subdivision mode (integer 소수점 무시, fractional_even, fractional_odd)
[outputtopology("triangle_cw")] // (triangle_cw, triangle_ccw, line)
[outputcontrolpoints(3)] // 하나의 입력 패치에 대해, HS가 출력할 제어점 개수
[patchconstantfunc("ConstantHS")] // ConstantHS 함수 이름
HS_OUT HS_Main(InputPatch<VS_OUT, 3> input, int vertexIdx : SV_OutputControlPointID, int patchID : SV_PrimitiveID)
{
    HS_OUT output = (HS_OUT)0.f;

    output.pos = input[vertexIdx].pos;
    output.uv = input[vertexIdx].uv;

    return output;
}

// --------------
// Domain Shader
// --------------

struct DS_OUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

//만들어진control point 들도 포함해서 모든 control point들이 들어온다.
//control point 배열과 
//기본 정점 3개와 해당 점점에서의 상대적인 위치(0~1)값인 location이 매개변수로 주어진다.
[domain("tri")]
DS_OUT DS_Main(const OutputPatch<HS_OUT, 3> input, float3 location : SV_DomainLocation, PatchTess patch)
{
    DS_OUT output = (DS_OUT)0.f;

    float3 localPos = input[0].pos * location[0] + input[1].pos * location[1] + input[2].pos * location[2];
    float2 uv = input[0].uv * location[0] + input[1].uv * location[1] + input[2].uv * location[2];

    output.pos = mul(float4(localPos, 1.f), g_matWVP);
    output.uv = uv;

    return output;
}

// --------------
// Pixel Shader
// --------------

float4 PS_Main(DS_OUT input) : SV_Target
{
    return float4(1.f, 0.f, 0.f, 1.f);
}

#endif