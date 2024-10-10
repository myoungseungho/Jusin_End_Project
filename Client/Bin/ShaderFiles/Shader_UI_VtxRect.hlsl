#include "Renderer_Shader_Defines.hlsli"

float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D g_Texture;
texture2D g_NextTexture;
texture2D g_MaskTexture;

bool g_bState;

float g_Radio;
float g_fRedRadio;

float g_MaskTimer;
float g_DestroyTimer;

vector g_vColor;

vector g_vCamPosition;


struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;
    

    vector vPosition = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
    vPosition = mul(vPosition, g_ViewMatrix);
    vPosition = mul(vPosition, g_ProjMatrix);

    Out.vPosition = vPosition;
    Out.vTexcoord = In.vTexcoord;

    
    return Out;
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

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;

    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

	//Out.vColor.gb = Out.vColor.r;
    if (Out.vColor.a <= 0.1f)
        discard;
    
    return Out;
}

PS_OUT PS_HP(PS_IN In)
{
        PS_OUT Out;
    
    float2 fPointA = float2(0.85f + (g_Radio - 1), 0.f);
    float2 fPointB = float2(1.f + (g_Radio - 1), 1.f);
    
    float2 fRedRointA = float2(0.85f + (g_fRedRadio - 1), 0.f);
    float2 fRedPointB = float2(1.f + (g_fRedRadio - 1), 1.f);
     
    float4 vBaseTex = g_Texture.Sample(LinearSampler, In.vTexcoord);
    
    float2 vMaskOffSet = float2(g_MaskTimer, g_MaskTimer);
    float2 vMaskTexCoord = ( In.vTexcoord + vMaskOffSet);
     
    float4 vMaskTex = g_MaskTexture.Sample(LinearSampler, vMaskTexCoord);
    
    Out.vColor = vBaseTex + (1 - (vMaskTex - 0.35f)) * vector(1.f, 0.831f, 0.f, 0.f);
    
    float fLineY = (fPointB.y - fPointA.y) / (fPointB.x - fPointA.x) * (In.vTexcoord.x - fPointA.x) + fPointA.y - In.vTexcoord.y;
    float fRedLineY = (fRedPointB.y - fRedRointA.y) / (fRedPointB.x - fRedRointA.x) * (In.vTexcoord.x - fRedRointA.x) + fRedRointA.y - In.vTexcoord.y;
     
    if (fRedLineY < 0 && fLineY > 0)
    {
        Out.vColor.rgb = float3(1, 0, 0);
        
        Out.vColor.a -= g_DestroyTimer;

    }
    else if (fRedLineY > 0)
        discard;

    
    if (Out.vColor.a <= 0.1f)
        discard;
        
    return Out;
}


PS_OUT PS_COLOR(PS_IN In)
{
    PS_OUT Out;

    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

    if (Out.vColor.a <= 0.1f)
        discard;
    
    Out.vColor.rgb = (1 - Out.vColor.rgb) * g_vColor;

    return Out;
}

//float CalculateDepthDiff
//    (
//    float2 vTexcoord, float fViewZ)
//{
//    float2 fOffsetRight = float2(1.0f / 1280.f, 0.0f);
//    float2 fOffsetDown = float2(0.0f, 1.0f / 720.f);
//    float2 fOffsetLeft = float2(-1.0f / 1280.f, 0.0f);
//    float2 fOffsetUp = float2(0.0f, -1.0f / 720.f);
//
//    float fDepthRight = g_DepthTexture.Sample(LinearSampler, vTexcoord + fOffsetRight).x * 1000.f;
//    float fDepthDown = g_DepthTexture.Sample(LinearSampler, vTexcoord + fOffsetDown).x * 1000.f;
//    float fDepthLeft = g_DepthTexture.Sample(LinearSampler, vTexcoord + fOffsetLeft).x * 1000.f;
//    float fDepthUp = g_DepthTexture.Sample(LinearSampler, vTexcoord + fOffsetUp).x * 1000.f;
//
//    float fDepthDiff = abs(fViewZ - fDepthRight) + abs(fViewZ - fDepthDown)
//                       + abs(fViewZ - fDepthLeft) + abs(fViewZ - fDepthUp);
//
//    return fDepthDiff;
//}
// ¿Ü°û¼± °ËÃâ ÇÔ¼ö
//float CalculateEdge
//    (
//    float2 vTexcoord, float fViewZ, float4 vNormal, float fEdgeThreshold, float fEdgeNormalThreshold, float fEdgeDepthThreshold)
//{
//
//    float fDepthDiff = CalculateDepthDiff(vTexcoord, fViewZ);
//   
//    float fEdge = step(fEdgeDepthThreshold, fDepthDiff) ;
//
//    return fEdge;
//}


PS_OUT PS_SKILL(PS_IN In)
{
    
    //------------------------------------------------------------------------------------
    PS_OUT Out;
    
    float4 vCurrTexture = g_Texture.Sample(LinearSampler, In.vTexcoord);
    float4 vNextTexture = g_NextTexture.Sample(LinearSampler, In.vTexcoord);
    
    float2 vMaskOffSet = float2(g_MaskTimer, 0.f);

    float2 vMaskTexCoord = (In.vTexcoord + vMaskOffSet);
    vMaskTexCoord.x *= 2.f;

    float4 vMaskTexture = g_MaskTexture.Sample(LinearSampler, vMaskTexCoord);
    //vMaskTexture.a = 0.78f;
    
    Out.vColor = vCurrTexture;
    
    if (g_Radio <= In.vTexcoord.x)
    {
        Out.vColor = vNextTexture;
        //Out.vColor = vNextTexture + (1 - vMaskTexture - 0.35f);
    }
    
    if (g_Radio <= 0.5f && In.vTexcoord.x >= 0.5f)
    {
        //Out.vColor += (1 -vMaskTexture - 0.15f);
        Out.vColor.rgb += Out.vColor.rgb * vMaskTexture.r * 2.f;

    }
    

    if (Out.vColor.a <= 0.1f)
        discard;
    
    //if (In.vTexcoord.y == 0 || In.vTexcoord.y == 1)
    //{
    //    Out.vColor = float4(0.f, 0.f, 0.f, 1.0f);
    //
    //}
    //else if (In.vTexcoord.x == 0 || In.vTexcoord.x == 1)
    //{
    //    Out.vColor = float4(0.f, 0.f, 0.f, 1.0f);
    //}
    
    
    //float fEdgeNormalThreshold = 0.2f;
    //float fEdgeDepthThreshold = 0.5f;
   
    //float fEdge = CalculateEdge(In.vTexcoord,1 , 1, 0.f, fEdgeNormalThreshold, fEdgeDepthThreshold);

    //vector vOutlineBlack = float4(0.f, 0.f, 0.f, 1.f);
    //Out.vColor = lerp(Out.vColor, vOutlineBlack, fEdge);

        return Out;
}

PS_OUT PS_COMBO(PS_IN In)
{
    PS_OUT Out;

    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    
    Out.vColor.rbg += g_vColor * (1.f - g_MaskTimer);

    if (Out.vColor.a <= 0.1f)
        discard;
  

    return Out;
}

technique11 DefaultTechnique
{
	/* PASSÀÇ ±âÁØ : ¼ÎÀÌ´õ ±â¹ýÀÇ Ä¸½¶È­. */

//0
    pass Default
    {

        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }


//1
   pass Hp
  {

      SetRasterizerState(RS_Cull_None);
      SetDepthStencilState(DSS_Default, 0);
      SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

      VertexShader = compile vs_5_0 VS_MAIN();
      GeometryShader = NULL;
      HullShader = NULL;
      DomainShader = NULL;
      PixelShader = compile ps_5_0 PS_HP();
  }

//2
    pass Color
    {

        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_COLOR();
    }

//3 
    pass Skill
    {

        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_SKILL();
    }

//4
    pass Combo
    {

        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_COMBO();
    }
}
