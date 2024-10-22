
#include "Renderer_Shader_Defines.hlsli"

float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
float4 g_vColor;
float4 g_vCamPosition;

struct VS_IN
{
	/* 정점 쉐이더마다 갱신되서 들어오는 데이터들 */
    float3 vPosition : POSITION;
};

struct VS_OUT
{
    float4 vPosition : POSITION;
    float2 vPSize : PSIZE;
    float2 vLifeTime : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;

    vector vPosition = mul(vector(In.vPosition, 1.f), In.LocalMatrix);

	/* mul : 곱하기가 가능한 모든 행렬(좌변의 열, 우변의 행 같다면)에 대해서 다 곱하기를 수행해준다. */
    vPosition = mul(vPosition, g_WorldMatrix);

    Out.vPosition = vPosition;
    Out.vPSize = float2(length(In.LocalMatrix._11_12_13), length(In.LocalMatrix._21_22_23));

    return Out;
}

struct GS_IN
{
    float4 vPosition : POSITION;
    float2 vPSize : PSIZE;
};

struct GS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

/* 사각형을 구성하기위한 정점을 생성할꺼야. */
/* 항상 카메라를 쳐다보게끔 만들어줄꺼야. */
/* g_vCamPosition위치랑 정점의 위치의 차를 통해 사각형이 바라봐야할 룩을 구할거야. */
/* 월드공간상에서 연산이 일어난다고 볼 수 있기때문에 정점의 위치를 월드까지면 변환하여 전달한다. */

/* GeometryShader : 정점을 추가 생성해줄 수 있다. */
/* Point를 그린다. 파라미터로 정점을 하나만 받는거야. point GS_IN In[1] */
/* Line를 그린다. 파라미터로 정점을 두개만 받는거야. line GS_IN In[2]*/
/* Triangle를 그린다. 파라미터로 정점을 세개만 받는거야. triangle GS_IN In[3]*/

[maxvertexcount(20)]
void GS_MAIN(point GS_IN In[1], inout TriangleStream<GS_OUT> VertexStream)
{
    GS_OUT Out[4];

    float3 vLook, vRight, vUp;
    vLook = g_vCamPosition.xyz - In[0].vPosition.xyz;
    vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook)) * In[0].vPSize.x * 0.5f;
    vUp = normalize(cross(vLook, vRight)) * In[0].vPSize.y * 0.5f;

    matrix matVP = mul(g_ViewMatrix, g_ProjMatrix);
    Out[0].vPosition = vector(In[0].vPosition.xyz + vRight + vUp, 1.f);
    Out[0].vTexcoord = float2(0.f, 0.f);
    Out[0].vPosition = mul(Out[0].vPosition, matVP);

    Out[1].vPosition = vector(In[0].vPosition.xyz - vRight + vUp, 1.f);
    Out[1].vTexcoord = float2(1.f, 0.f);
    Out[1].vPosition = mul(Out[1].vPosition, matVP);

    Out[2].vPosition = vector(In[0].vPosition.xyz - vRight - vUp, 1.f);
    Out[2].vTexcoord = float2(1.f, 1.f);
    Out[2].vPosition = mul(Out[2].vPosition, matVP);

    Out[3].vPosition = vector(In[0].vPosition.xyz + vRight - vUp, 1.f);
    Out[3].vTexcoord = float2(0.f, 1.f);
    Out[3].vPosition = mul(Out[3].vPosition, matVP);
	
    VertexStream.Append(Out[0]);
    VertexStream.Append(Out[1]);
    VertexStream.Append(Out[2]);
    VertexStream.RestartStrip();

    VertexStream.Append(Out[0]);
    VertexStream.Append(Out[2]);
    VertexStream.Append(Out[3]);
    VertexStream.RestartStrip();
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};

PS_OUT PS_Rain(PS_IN In)
{
    PS_OUT Out;

    Out.vColor = g_vColor;

    return Out;
}

technique11 DefaultTechnique
{
	/* PASS의 기준 : 셰이더 기법의 캡슐화. */
    pass Default
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

 
}














