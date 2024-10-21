
#include "Renderer_Shader_Defines.hlsli"

float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D g_DiffuseTexture;
texture2D g_AlphaTexture;
vector g_vCamPosition;

int g_iUnique_Index = -1;

struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float3 vTangent : TANGENT;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;

    vector vPosition = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
    vPosition = mul(vPosition, g_ViewMatrix);
    vPosition = mul(vPosition, g_ProjMatrix);

    Out.vPosition = vPosition;
    Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
    Out.vTexcoord = In.vTexcoord;
    Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
    Out.vProjPos = vPosition;

    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};

struct PS_OUT
{
    float4 vDiffuse : SV_TARGET0;
    float4 vAlpha : SV_TARGET1;
    float4 vDepth : SV_TARGET2;
};

struct PS_OUT_PICK
{
    float4 vDiffuse : SV_TARGET0;
};

PS_OUT_PICK PS_MAIN_PICK(PS_IN In)
{
    PS_OUT_PICK Out;

    Out.vDiffuse = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, g_iUnique_Index, 1.f);;

    return Out;
}

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    vector vMtrlAlpha = g_AlphaTexture.Sample(LinearSampler, In.vTexcoord);
    if (vMtrlDiffuse.a < 0.99f)
        discard;
    
    Out.vDiffuse = vMtrlDiffuse;
    Out.vAlpha = vMtrlAlpha.r;
    Out.vDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, g_iUnique_Index, 0.f);

    return Out;
}

PS_OUT PS_MAIN_NONELIGHT(PS_IN In)
{
    PS_OUT Out;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    vector vMtrlAlpha = g_AlphaTexture.Sample(LinearSampler, In.vTexcoord);
    if (vMtrlDiffuse.a < 0.99f)
        discard;
    
    Out.vDiffuse = vMtrlDiffuse;
    Out.vAlpha = vMtrlAlpha.r;
    Out.vDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, g_iUnique_Index, 0.f);

    return Out;
}

PS_OUT PS_MAIN_ALPHABLEND_EFFECT(PS_IN In)
{
    PS_OUT Out;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    vector vMtrlAlpha = g_AlphaTexture.Sample(LinearSampler, In.vTexcoord);
    
    Out.vDiffuse = vMtrlDiffuse;
    Out.vAlpha = vMtrlAlpha.r;
    Out.vDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, g_iUnique_Index, 0.f);

    return Out;
}

PS_OUT PS_MAIN_DOUBLE_D(PS_IN In)
{
    PS_OUT Out;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    vector vMtrlAlpha = g_AlphaTexture.Sample(LinearSampler, In.vTexcoord);
    
    float3 vBrown = { 166.f / 255.f, 131.f / 255.f, 115.f / 255.f };
    
    vMtrlDiffuse.rgb *= vBrown;
    // 비율에 따라 색상 조정
   // Out.vDiffuse = vMtrlDiffuse;
//// 비율에 따라 색상 조정
    Out.vDiffuse = vector(
    min(vMtrlDiffuse.r + (vMtrlDiffuse.r * 0.3f), 1.f),
    min(vMtrlDiffuse.g + (vMtrlDiffuse.g * 0.3f), 1.f),
    min(vMtrlDiffuse.b + (vMtrlDiffuse.b * 0.3f), 1.f),
    //vMtrlDiffuse.a
    1.f
    );

    Out.vAlpha = vMtrlAlpha.r;
    Out.vDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, g_iUnique_Index, 0.f);

    return Out;
}

technique11 DefaultTechnique
{
    pass Default // 0
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		//SetDepthStencilState();
		//SetBlendState();

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass Pick // 1
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		//SetDepthStencilState();
		//SetBlendState();

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_PICK();
    }

    pass Blend // 2
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		//SetDepthStencilState();
		//SetBlendState();

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass Double_D //3
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		//SetDepthStencilState();
		//SetBlendState();

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DOUBLE_D();
        //모델 두겹용
    }
    pass NoneLight //4
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		//SetDepthStencilState();
		//SetBlendState();

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_NONELIGHT();
    }

    pass AlphaBlendEffect //5
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		//SetDepthStencilState();
		//SetBlendState();

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_ALPHABLEND_EFFECT();
    }

    pass Alpha_ZNone_Effect //6
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		//SetDepthStencilState();
		//SetBlendState();

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_ALPHABLEND_EFFECT();
    }
}














