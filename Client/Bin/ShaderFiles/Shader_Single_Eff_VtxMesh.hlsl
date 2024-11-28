
#include "Renderer_Shader_Defines.hlsli"

float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D g_DiffuseTexture;
texture2D g_AlphaTexture;
vector g_vCamPosition;

vector g_vColor = { 255.f, 255.f, 255.f, 1.f };
bool    g_bColorChange;

int g_iUnique_Index = -1;
float g_fGlowFactor = 1.f;
float g_Time;

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
PS_OUT PS_MAIN_CUSTOM_0(PS_IN In);
PS_OUT PS_MAIN_CUSTOM_1(PS_IN In);
PS_OUT PS_MAIN_CUSTOM_2(PS_IN In);
PS_OUT PS_MAIN_CUSTOM_3(PS_IN In);

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
    Out.vAlpha = vector(g_fGlowFactor, 0.f, 0.f, 1.f);
    Out.vDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, g_iUnique_Index, 0.f);

    return Out;
}

PS_OUT PS_MAIN_NONELIGHT(PS_IN In)
{
    PS_OUT Out;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    vector vMtrlAlpha = g_AlphaTexture.Sample(LinearSampler, In.vTexcoord);
    if (vMtrlAlpha.a < 0.99f)
        discard;
    
    float3 vAddColor = { g_vColor.r / 255.f, g_vColor.g / 255.f, g_vColor.b / 255.f };
    float fAlpha = vMtrlAlpha.a;
    
    if (g_bColorChange)
    {
        vMtrlDiffuse.rgb *= vAddColor;
        vMtrlDiffuse.a = saturate(fAlpha * g_vColor.a);
    }
          
    if (!g_bColorChange)
    {
        vMtrlDiffuse.a = fAlpha;
    }
    Out.vDiffuse = vMtrlDiffuse;
    Out.vAlpha = vector(g_fGlowFactor, 0.f, 0.f, 1.f);
    Out.vDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, g_iUnique_Index, 0.f);

    return Out;
}

PS_OUT PS_MAIN_ALPHABLEND_EFFECT(PS_IN In)
{
    PS_OUT Out;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    vector vMtrlAlpha = g_AlphaTexture.Sample(LinearSampler, In.vTexcoord);
    
    float3 vAddColor = { g_vColor.r / 255.f, g_vColor.g / 255.f, g_vColor.b / 255.f };
    float fAlpha = vMtrlAlpha.a;
    
        if (g_bColorChange)
        {
            vMtrlDiffuse.rgb *= vAddColor;
            vMtrlDiffuse.a = saturate(fAlpha * g_vColor.a);
        }
          
        if (!g_bColorChange)
        {
            vMtrlDiffuse.a = fAlpha;
        }
        
        Out.vDiffuse = vMtrlDiffuse;
        Out.vAlpha = vector(0.f, 0.f, 0.f, 1.f);
        Out.vDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, g_iUnique_Index, 0.f);

        return Out;
}

PS_OUT PS_MAIN_MODELANIMATION(PS_IN In)
{
    PS_OUT Out;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    vector vMtrlAlpha = g_AlphaTexture.Sample(LinearSampler, In.vTexcoord);
    
    float3 vAddColor = { g_vColor.r / 255.f, g_vColor.g / 255.f, g_vColor.b / 255.f};
    float fAlpha = vMtrlAlpha.r;

    
    if (g_bColorChange)
    {
        vMtrlDiffuse.rgb *= vAddColor;
        vMtrlDiffuse.a = saturate(fAlpha * g_vColor.a);
    }
          
    if (!g_bColorChange)
    {
        vMtrlDiffuse.a = fAlpha;
    }
    
    Out.vDiffuse = vMtrlDiffuse;
    Out.vAlpha = vector(g_fGlowFactor, 0.f, 0.f, 1.f);
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
        PixelShader = compile ps_5_0 PS_MAIN_ALPHABLEND_EFFECT();
    }

    pass ModelAnimation //3
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
        PixelShader = compile ps_5_0 PS_MAIN_MODELANIMATION();
        //¸ðµ¨ µÎ°ã¿ë
    }

    pass NoneLight //4
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
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
        SetRasterizerState(RS_Cull_None);
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

    pass Alpha_ZNone_Effect //6
    {
        SetRasterizerState(RS_Cull_None);
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

    pass CustomEffect // 7
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		//SetDepthStencilState();
		//SetBlendState();

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_CUSTOM_0();
    }

    pass CustomEffectDSS_None // 8
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		//SetDepthStencilState();
		//SetBlendState();

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_CUSTOM_0();
    }

    pass CustomEffectCutBust // 9
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		//SetDepthStencilState();
		//SetBlendState();

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_CUSTOM_1();
    }

    pass CustomEffectDSSDefault // 10
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		//SetDepthStencilState();
		//SetBlendState();

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_CUSTOM_2();
    }

    pass CustomEffectGlass // 11
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		//SetDepthStencilState();
		//SetBlendState();

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_CUSTOM_3();
    }

}

PS_OUT PS_MAIN_CUSTOM_0(PS_IN In)
{
    PS_OUT Out;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    vector vMtrlAlpha = g_AlphaTexture.Sample(LinearSampler, In.vTexcoord);
    
    float3 vAddColor = { g_vColor.r / 255.f, g_vColor.g / 255.f, g_vColor.b / 255.f };
    float fAlpha = vMtrlAlpha.a;
    

    vMtrlDiffuse.rgb *= vAddColor;
    vMtrlDiffuse.a = vMtrlDiffuse.a;
  
        
    Out.vDiffuse = vMtrlDiffuse;
    Out.vAlpha = vector(0.f, 0.f, 0.f, 1.f);
    Out.vDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, g_iUnique_Index, 0.f);

    return Out;
}

PS_OUT PS_MAIN_CUSTOM_1(PS_IN In)
{
    PS_OUT Out;
    float2 vTexcoord = In.vTexcoord;
    vTexcoord.y = vTexcoord.y * 0.5f + 0.5f;
    
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, vTexcoord);
    vector vMtrlAlpha = g_AlphaTexture.Sample(LinearSampler, vTexcoord);
    
    float3 vAddColor = { g_vColor.r / 255.f, g_vColor.g / 255.f, g_vColor.b / 255.f };
    float fAlpha = vMtrlAlpha.a;
    
    
    vMtrlDiffuse.rgb *= vAddColor;
    vMtrlDiffuse.a = vMtrlDiffuse.a;
  
        
    Out.vDiffuse = vMtrlDiffuse;
    Out.vAlpha = vector(0.f, 0.f, 0.f, 1.f);
    Out.vDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, g_iUnique_Index, 0.f);

    return Out;
}


PS_OUT PS_MAIN_CUSTOM_2(PS_IN In)
{
    PS_OUT Out;
    float2 vTexcoord = In.vTexcoord;
    vTexcoord.y = vTexcoord.y * 0.5f + 0.5f;
    
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, vTexcoord);
    vector vMtrlAlpha = g_AlphaTexture.Sample(LinearSampler, vTexcoord);
    
    float3 vAddColor = { g_vColor.r / 255.f, g_vColor.g / 255.f, g_vColor.b / 255.f };
    float fAlpha = vMtrlAlpha.a;
    
    
    vMtrlDiffuse.rgb *= vAddColor;
    vMtrlDiffuse.a = vMtrlDiffuse.a;
  
        
    Out.vDiffuse = vMtrlDiffuse;
    Out.vAlpha = vector(0.f, 0.f, 0.f, 1.f);
    Out.vDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, g_iUnique_Index, 0.f);

    return Out;
}

PS_OUT PS_MAIN_CUSTOM_3(PS_IN In)
{
    PS_OUT Out;
    float2 vTexcoord = In.vTexcoord + g_Time * 5.f;
    
    
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    vector vMtrlAlpha = g_AlphaTexture.Sample(LinearSampler, In.vTexcoord);
    
    float3 vAddColor = { g_vColor.r / 255.f, g_vColor.g / 255.f, g_vColor.b / 255.f };
    float fAlpha = vMtrlAlpha.a;
    
    
    vMtrlDiffuse.rgb *= vAddColor;
    vMtrlDiffuse.a = vMtrlDiffuse.a;
  
        
    Out.vDiffuse = vMtrlDiffuse;
    Out.vAlpha = vector(0.f, 0.f, 0.f, 1.f);
    Out.vDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, g_iUnique_Index, 0.f);

    return Out;
}









