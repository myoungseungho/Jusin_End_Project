#include "Renderer_Shader_Defines.hlsli"

float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D g_Texture;

float g_HpRadio;

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

    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    
   
    if (In.vTexcoord.x >= g_HpRadio)
        discard;
    
    if (Out.vColor.a <= 0.1f)
        discard;
    
    Out.vColor.rgb = float3(1, 0, 0);
    
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

}
