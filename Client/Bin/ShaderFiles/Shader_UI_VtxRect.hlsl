#include "Renderer_Shader_Defines.hlsli"

float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D g_Texture;
texture2D g_NextTexture;
texture2D g_MaskTexture;

bool g_bState;

float g_Radio;
float g_RedRadio;

float g_MaskTimer;
float g_DestroyTimer;

vector g_vColor;


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
    
    //float fRedRointA = float2(fPointA., 0.f);
    //float2 fRedPointB = float2(1.f + (g_Radio - 1), 1.f);
     
    float4 vBaseTex = g_Texture.Sample(LinearSampler, In.vTexcoord);
    
    float2 vMaskOffSet = float2(g_MaskTimer, g_MaskTimer);
    float2 vMaskTexCoord = ( In.vTexcoord + vMaskOffSet);
    
    float4 vMaskTex = g_MaskTexture.Sample(LinearSampler, vMaskTexCoord);
    
    Out.vColor = vBaseTex + (1 - (vMaskTex - 0.35f)) * vector(1.f, 0.831f, 0.f, 0.f);
    
    //if (g_Timer)
    
    float fLineY = (fPointB.y - fPointA.y) / (fPointB.x - fPointA.x) * (In.vTexcoord.x - fPointA.x) + fPointA.y - In.vTexcoord.y;
     
    if (fLineY > 0 )
    {
        if (g_bState)
        {
            Out.vColor.rgb = float3(1, 0, 0);     
            
            //discard;
        }
        else
        {
            discard;
        }
        
        
        //else
        //{
        //    Out.vColor.a -= g_DestroyTimer;
        //
        //}
         
        //Out.vColor.a *= (1 - g_DestroyTimer);
    }
    
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

PS_OUT PS_SKILL(PS_IN In)
{
    PS_OUT Out;
    
    float4 vCurrTexture = g_Texture.Sample(LinearSampler, In.vTexcoord);
    float4 vNextTexture = g_NextTexture.Sample(LinearSampler, In.vTexcoord);
    
    Out.vColor = vCurrTexture;
    //vCurrTexture
    if (g_Radio <= In.vTexcoord.x)
    {
        Out.vColor = vNextTexture;

    }
    

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
}
