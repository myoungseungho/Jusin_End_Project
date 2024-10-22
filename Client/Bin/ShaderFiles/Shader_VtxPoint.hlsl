
#include "Renderer_Shader_Defines.hlsli"

float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
float4 g_vColor;
float4 g_vCamPosition;

struct VS_IN
{
	/* ���� ���̴����� ���ŵǼ� ������ �����͵� */
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

	/* mul : ���ϱⰡ ������ ��� ���(�º��� ��, �캯�� �� ���ٸ�)�� ���ؼ� �� ���ϱ⸦ �������ش�. */
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

/* �簢���� �����ϱ����� ������ �����Ҳ���. */
/* �׻� ī�޶� �Ĵٺ��Բ� ������ٲ���. */
/* g_vCamPosition��ġ�� ������ ��ġ�� ���� ���� �簢���� �ٶ������ ���� ���Ұž�. */
/* ��������󿡼� ������ �Ͼ�ٰ� �� �� �ֱ⶧���� ������ ��ġ�� ��������� ��ȯ�Ͽ� �����Ѵ�. */

/* GeometryShader : ������ �߰� �������� �� �ִ�. */
/* Point�� �׸���. �Ķ���ͷ� ������ �ϳ��� �޴°ž�. point GS_IN In[1] */
/* Line�� �׸���. �Ķ���ͷ� ������ �ΰ��� �޴°ž�. line GS_IN In[2]*/
/* Triangle�� �׸���. �Ķ���ͷ� ������ ������ �޴°ž�. triangle GS_IN In[3]*/

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
	/* PASS�� ���� : ���̴� ����� ĸ��ȭ. */
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














