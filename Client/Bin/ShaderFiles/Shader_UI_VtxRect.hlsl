#include "Renderer_Shader_Defines.hlsli"

float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D g_Texture;
texture2D g_NextTexture;
texture2D g_MaskTexture;
texture2D g_BGTexture;

bool g_bState;

float g_Radio;
float g_fRedRadio;

float g_MaskTimer = 1.f;
float g_DestroyTimer = 0.f;
float g_fAlphaTimer = 0.f;

vector g_vColor;
vector g_vEndColor;

vector g_vCamPosition;

//스프라이트
int iSpriteIndex = 0;
int iNumSprite = 0;


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
    
    float2 fPointA = float2(0.9519f + (g_Radio - 1), 0.f);
    float2 fPointB = float2(1.f + (g_Radio - 1), 1.f);
    
    float2 fRedRointA = float2(0.9519f + (g_fRedRadio - 1), 0.f);
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
    
    Out.vColor.rgb = (Out.vColor.rgb) * g_vColor;

    return Out;
}

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

PS_OUT PS_SUB_HP(PS_IN In)
{
    PS_OUT Out;
    
    float2 fPointA = float2(0.95f + (g_Radio - 1), 0.f);
    float2 fPointB = float2(1.f + (g_Radio - 1), 1.f);

     
    float4 vBaseTex = g_Texture.Sample(LinearSampler, In.vTexcoord);
    
    float2 vMaskOffSet = float2(g_MaskTimer, g_MaskTimer);
    float2 vMaskTexCoord = (In.vTexcoord + vMaskOffSet);
     
    float4 vMaskTex = g_MaskTexture.Sample(LinearSampler, vMaskTexCoord);
    
    Out.vColor = vBaseTex ;
    
    float fLineY = (fPointB.y - fPointA.y) / (fPointB.x - fPointA.x) * (In.vTexcoord.x - fPointA.x) + fPointA.y - In.vTexcoord.y;
     
    if (fLineY > 0)
    {
        Out.vColor.rgb = (1 - vBaseTex) * float4(1.f, 0.831f, 0.f, 0.f);    
    }
  
    if (Out.vColor.a <= 0.1f)
        discard;
        
    return Out;
}

PS_OUT PS_StartCircle(PS_IN In)
{
    PS_OUT Out;

    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

    Out.vColor.a *= (Out.vColor.rgb);
    
    if (Out.vColor.a <= 0.1f)
        discard;
    
    //0.5f
    return Out;
}



PS_OUT PS_READY(PS_IN In)
{
    PS_OUT Out;

    vector vDiffuseMaterial = g_Texture.Sample(LinearSampler, In.vTexcoord);
    vector vBGMaterial = g_MaskTexture.Sample(LinearSampler, In.vTexcoord);
    
    vDiffuseMaterial.rgb = 1 - g_MaskTimer; 
    vBGMaterial = lerp(vBGMaterial, vDiffuseMaterial, vDiffuseMaterial.a * g_MaskTimer);
        
    Out.vColor = vBGMaterial;
    Out.vColor.a -= g_DestroyTimer;
    

    return Out;
}

PS_OUT PS_FONT(PS_IN In)
{
    PS_OUT Out;
   
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    
    vector vBGMaterial = g_Texture.Sample(LinearSampler, In.vTexcoord);
    vector vDiffuseMaterial = g_MaskTexture.Sample(LinearSampler, In.vTexcoord);
    
    //vDiffuseMaterial.rgb = 1 - g_MaskTimer;
    vBGMaterial = lerp(vBGMaterial, vDiffuseMaterial, vDiffuseMaterial.a);
    
    Out.vColor = vBGMaterial;
    Out.vColor.a -= min(1.f, g_fAlphaTimer);
    
    if (Out.vColor.a < 0.15f)
        discard;
    
    return Out;
}

PS_OUT PS_SubIcon(PS_IN In)
{
    PS_OUT Out;

    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    
    
    if (In.vTexcoord.y < 0.7f || In.vTexcoord.y > 0.85f)
        discard;
    
    if (Out.vColor.a <= 0.1f)
        discard;
  
    return Out;
}

PS_OUT PS_Sprite(PS_IN In)
{
    PS_OUT Out;

    //(iSpriteIndex + 1);
    float fImageRadio = (1.f / iNumSprite);
    float fStartSprite = fImageRadio * (iSpriteIndex);
    float fEndSprite = fImageRadio * (iSpriteIndex + 1);
    
    //In.vTexcoord.y = In.vTexcoord.y * fSprite;
    In.vTexcoord.y = lerp(fStartSprite, fEndSprite, In.vTexcoord.y);
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    Out.vColor.a *= 0.75f;
  
    return Out;
}

PS_OUT PS_OnlyColor(PS_IN In)
{
    PS_OUT Out;

    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

    if (Out.vColor.a <= 0.1f)
        discard;
    
    Out.vColor.rgb = (1 - Out.vColor.rgb) * g_vColor;

    return Out;

}

PS_OUT PS_INPUT(PS_IN In)
{
    PS_OUT Out;

    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    vector vBGMaterial = g_BGTexture.Sample(LinearSampler, In.vTexcoord);
    
    if (Out.vColor.a != 1)
        Out.vColor = float4(0, 0, 0, 1);

    return Out;
}

PS_OUT PS_HPALPHA(PS_IN In)
{
    PS_OUT Out;
    
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    
    if (Out.vColor.a <= 0.1f)
        discard;
    
    Out.vColor.rgb =  g_vColor;
    Out.vColor.a = g_fAlphaTimer;
    
    return Out;
}

PS_OUT PS_Gradient(PS_IN In)
{
    PS_OUT Out;

    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

    if (Out.vColor.a <= 0.1f)
        discard;
    
    Out.vColor = lerp(g_vColor, g_vEndColor, In.vTexcoord.x);

    return Out;
}

PS_OUT PS_ALPHA(PS_IN In)
{
    PS_OUT Out;

    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

    if (Out.vColor.a <= 0.1f)
        discard;
  
    Out.vColor.a = g_fAlphaTimer;

    return Out;
}

PS_OUT PS_TEX_MOVE(PS_IN In)
{
    PS_OUT Out;

    float2 offset = float2(g_MaskTimer, g_MaskTimer);
    float2 newTexcoord = In.vTexcoord + offset ;
    
    float fDistance = abs(In.vTexcoord - float2(0.5f, 0.5f));
        
    
    Out.vColor = g_Texture.Sample(DestroySampler, newTexcoord);
    
    Out.vColor.a -= (fDistance * 2);
    
    if (Out.vColor.a <= 0.1f)
        discard;
   
    return Out;
}

PS_OUT PS_PANEL(PS_IN In)
{
    PS_OUT Out;

    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    
    if (g_MaskTimer <= In.vTexcoord.x)
        discard;
  
    //Out.vColor += g_fAlphaTimer;

    if (Out.vColor.a <= 0.1f)
        discard;

    return Out;
}


technique11 DefaultTechnique
{
	/* PASS의 기준 : 셰이더 기법의 캡슐화. */

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

//5

    pass SubHp
    {

        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_SUB_HP();
    }

//6

    pass StartCircle
    {

        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_StartCircle();
    }

//7
    pass Ready
    {

        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_READY();
    }

//8
    pass Font
    {
    
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
    
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_FONT();
    }


//9
 pass SubIcon
 {
 
     SetRasterizerState(RS_Cull_None);
     SetDepthStencilState(DSS_Default, 0);
     SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
 
     VertexShader = compile vs_5_0 VS_MAIN();
     GeometryShader = NULL;
     HullShader = NULL;
     DomainShader = NULL;
     PixelShader = compile ps_5_0 PS_SubIcon();
 }

//10
    pass SkillEff
    {
 
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
 
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_Sprite();
    }

//11
    pass OnlyColor
    {
 
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
 
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_OnlyColor();
    }

//12
    pass Input
    {
 
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
 
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_INPUT();
    }

//13
    pass HpAlpha
    {
 
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
 
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_HPALPHA();
    }

//14    
    pass Gradient
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
 
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_Gradient();
    }
   
//15
    pass Alpha
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
 
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_ALPHA();
    }

//16
    pass TEX
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
 
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_TEX_MOVE();
    }

//17
    pass PANEL
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
 
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_PANEL();
    }
}
