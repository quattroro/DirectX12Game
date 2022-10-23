#ifndef _LIGHTING_FX_
#define _LIGHTING_FX_

#include "params.fx"
#include "utils.fx"

//인 아웃 들은 똑같이 사용한다.
struct VS_IN
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct VS_OUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

struct PS_OUT
{
    float4 diffuse : SV_Target0;
    float4 specular : SV_Target1;
};

// [Directional Light]
// g_int_0 : Light index 글로벌 파람에서 받아온 라이트 인포중에 몇번째 인지에 대한 인덱스 번호
// g_tex_0 : Position RT 포지션과 노말
// g_tex_1 : Normal RT
// Mesh : Rectangle 빛의 영역(화면 전체를 영역으로 한다.)

VS_OUT VS_DirLight(VS_IN input)
{
    VS_OUT output = (VS_OUT)0;

    output.pos = float4(input.pos * 2.f, 1.f); //2를 곱하는 이유는 리소스에서 rectangle을 만들때 -0.5에서+0.5 좌표 이기 때문에 화면 좌표인 -1~1 로 바꿔주기 위함
    output.uv = input.uv;

    return output;
}

PS_OUT PS_DirLight(VS_OUT input)
{
    PS_OUT output = (PS_OUT)0;

    //빛이 바라보는 곳에 물체가 있는지
    float3 viewPos = g_tex_0.Sample(g_sam_0, input.uv).xyz;
    if (viewPos.z <= 0.f)//카메라 뒤에있는 물체 (프로젝션 좌표계 이기 때문)
        clip(-1);//스킵, 그냥 리턴하는거랑 같은 효과



    float3 viewNormal = g_tex_1.Sample(g_sam_0, input.uv).xyz;

    LightColor color = CalculateLightColor(g_int_0, viewNormal, viewPos);
    output.diffuse = color.diffuse + color.ambient;
    output.specular = color.specular;

    return output;
}

// [Point Light]
// g_int_0 : Light index
// g_tex_0 : Position RT
// g_tex_1 : Normal RT
// g_vec2_0 : RenderTarget Resolution 렌더 타겟의 해상도
// Mesh : Sphere

VS_OUT VS_PointLight(VS_IN input)
{
    VS_OUT output = (VS_OUT)0;

    output.pos = mul(float4(input.pos, 1.f), g_matWVP);
    output.uv = input.uv;

    return output;
}

PS_OUT PS_PointLight(VS_OUT input)
{
    PS_OUT output = (PS_OUT)0;

    // input.pos = SV_Position = Screen 좌표
    float2 uv = float2(input.pos.x / g_vec2_0.x, input.pos.y / g_vec2_0.y);//픽셀 좌표를 다시 UV좌표로 만들어 준다.
    float3 viewPos = g_tex_0.Sample(g_sam_0, uv).xyz;
    if (viewPos.z <= 0.f)
        clip(-1);

    //좌표가 및의 영역안에 있으면 이번에느 거리를 측정해서 빛의 영향을 받을만한 거리에 있는지 확인한다.
    int lightIndex = g_int_0;
    float3 viewLightPos = mul(float4(g_light[lightIndex].position.xyz, 1.f), g_matView).xyz;
    float distance = length(viewPos - viewLightPos);
    if (distance > g_light[lightIndex].range)
        clip(-1);

    float3 viewNormal = g_tex_1.Sample(g_sam_0, uv).xyz;

    LightColor color = CalculateLightColor(g_int_0, viewNormal, viewPos);

    output.diffuse = color.diffuse + color.ambient;
    output.specular = color.specular;

    return output;
}

//지금까지 만들어준 것들을 조립해서 최종 결과물을 만들어 준다.
// [Final]
// g_tex_0 : Diffuse Color Target
// g_tex_1 : Diffuse Light Target
// g_tex_2 : Specular Light Target
// Mesh : Rectangle

VS_OUT VS_Final(VS_IN input)
{
    VS_OUT output = (VS_OUT)0;

    output.pos = float4(input.pos * 2.f, 1.f);
    output.uv = input.uv;

    return output;
}

float4 PS_Final(VS_OUT input) : SV_Target
{
    float4 output = (float4)0;

    //빛의 영향을 받는곧에 있는지 확인
    float4 lightPower = g_tex_1.Sample(g_sam_0, input.uv);
    if (lightPower.x == 0.f && lightPower.y == 0.f && lightPower.z == 0.f)
        clip(-1);

    
    float4 color = g_tex_0.Sample(g_sam_0, input.uv);
    float4 specular = g_tex_2.Sample(g_sam_0, input.uv);

    output = (color * lightPower) + specular;
    return output;
}

#endif 