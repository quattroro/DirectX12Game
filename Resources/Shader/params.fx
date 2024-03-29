#ifndef _PARAMS_FX_
#define _PARAMS_FX_


//Light의 정보들을 그대로 받아온다.
struct LightColor
{
    float4 diffuse;
    float4 ambient;
    float4 specular;
};

struct LightInfo
{
    LightColor  color;
    float4	    position;
    float4	    direction;
    int		    lightType;
    float	    range;
    float	    angle;
    int  	    padding;
};

cbuffer GLOBAL_PARAMS : register(b0)
{
    int         g_lightCount;
    float3      g_lightPadding;
    LightInfo   g_light[50];
}

//셰이더의 파람들을 보내주는건 TableDescriptorHeap에서 해준다.
cbuffer TRANSFORM_PARAMS : register(b1)
{
    row_major matrix g_matWorld;
    row_major matrix g_matView;
    row_major matrix g_matProjection;
    row_major matrix g_matWV;

    row_major/*행렬 접근순서가 셰이더랑 다렉이랑 다르기 때문에 순서를 맞춰주기 위해서*/  matrix g_matWVP;

    //셰이더에는 역행렬을 구하는 함수가 없기 때문
    row_major matrix g_matViewInv;
};

//머테리얼에 정보가 있다.
//메테리얼과 관련된 인자들을 넘겨 받는다. 메테리얼 파람즈와 같은 형식 이어야 한다.
cbuffer MATERIAL_PARAMS : register(b2)
{
    int g_int_0;
    int g_int_1;
    int g_int_2;
    int g_int_3;

    float g_float_0;
    float g_float_1;
    float g_float_2;
    float g_float_3;

    int g_tex_on_0;
    int g_tex_on_1;
    int g_tex_on_2;
    int g_tex_on_3;

    float2  g_vec2_0;
    float2  g_vec2_1;
    float2  g_vec2_2;
    float2  g_vec2_3;

    float4  g_vec4_0;
    float4  g_vec4_1;
    float4  g_vec4_2;
    float4  g_vec4_3;

    row_major float4x4 g_mat_0;
    row_major float4x4 g_mat_1;
    row_major float4x4 g_mat_2;
    row_major float4x4 g_mat_3;
};

//메테리얼의 텍스쳐
Texture2D g_tex_0 : register(t0);
Texture2D g_tex_1 : register(t1);
Texture2D g_tex_2 : register(t2);
Texture2D g_tex_3 : register(t3);
Texture2D g_tex_4 : register(t4);

SamplerState g_sam_0 : register(s0);

#endif 
