#include "Renderer_Shader_Defines.hlsli"

float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D g_Texture;
texture2D g_NextTexture;
texture2D g_MaskTexture;
texture2D g_MarkTexture;
texture2D g_BGTexture;

texture2D g_CircleTexture0;
texture2D g_CircleTexture1;
texture2D g_CircleTexture2;
texture2D g_CircleTexture3;

texture2D g_NameTexture;

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

float2 g_vCurrPos;
float2 g_vPrevPos;

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

float2 RotateUV(float2 uv, float2 center, float angle)
{
    // 중심점 기준으로 UV를 이동
    uv -= center;

    // 비율 보정 (세로 방향으로 0.5배 축소)
    uv.y *= 0.5;

    // 회전 변환
    float cosAngle = cos(angle);
    float sinAngle = sin(angle);
    float2 rotatedUV = float2(
        uv.x * cosAngle - uv.y * sinAngle,
        uv.x * sinAngle + uv.y * cosAngle
    );

    // 비율 복원 (세로 방향으로 2배 확대)
    rotatedUV.y *= 2.0;

    // 원래 위치로 이동
    rotatedUV += center;

    return rotatedUV;
}

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
    
    float2 fPointA = saturate(float2(0.9519f + (g_Radio * 0.9519f - 0.9519f), 0.f));
    float2 fPointB = float2(fPointA.x + 0.0481f , 1.f);
    
    float2 fRedRointA = float2(0.9519f + (g_fRedRadio * 0.9519f - 0.9519f), 0.f);
    float2 fRedPointB = float2(fRedRointA.x + 0.0481f, 1.f);
     
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
    
    return Out;
}


PS_OUT PS_COLOR(PS_IN In)
{
    PS_OUT Out;

    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

    if (Out.vColor.a <= 0.1f)
        discard;
    
    Out.vColor.rgb = (Out.vColor.rgb) * g_vColor;
    
    Out.vColor = saturate(Out.vColor);

    return Out;
}

PS_OUT PS_SKILL(PS_IN In)
{
    PS_OUT Out;
    
    float4 vCurrTexture = g_Texture.Sample(LinearSampler, In.vTexcoord);
    float4 vNextTexture = g_NextTexture.Sample(LinearSampler, In.vTexcoord);
    
    float2 vMaskOffSet = float2(g_MaskTimer, 0.f);

    float2 vMaskTexCoord = (In.vTexcoord + vMaskOffSet);
    vMaskTexCoord.x *= 2.f;

    float4 vMaskTexture = g_MaskTexture.Sample(LinearSampler, vMaskTexCoord);
    
    Out.vColor = vCurrTexture;
    
    if (g_Radio <= In.vTexcoord.x)
    {
        Out.vColor = vNextTexture;
    }
    
    if ((g_Radio <= 0.5f || g_bState) && In.vTexcoord.x >= 0.5f)
    {
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
    
    Out.vColor.rbg += g_vEndColor * (1.f - g_MaskTimer);

    if (Out.vColor.a <= 0.1f)
        discard;

    return Out;
}

PS_OUT PS_SUB_HP(PS_IN In)
{
    PS_OUT Out;
    
    float2 fPointA = float2(0.9519f + (g_Radio * 0.9519f - 0.9519f), 0.f);
    float2 fPointB = float2(fPointA.x + (1.f -  0.9519f ), 1.f);

    float4 vBaseTex = g_Texture.Sample(LinearSampler, In.vTexcoord);
    
    float2 vMaskOffSet = float2(g_MaskTimer, g_MaskTimer);
    float2 vMaskTexCoord = (In.vTexcoord + vMaskOffSet);
     
    float4 vMaskTex = g_MaskTexture.Sample(LinearSampler, vMaskTexCoord);
    
    Out.vColor = vBaseTex ;
    
    float fLineY = (fPointB.y - fPointA.y) / (fPointB.x - fPointA.x) * (In.vTexcoord.x - fPointA.x) + fPointA.y - In.vTexcoord.y;
     
    if (fLineY < 0)
    {
        Out.vColor.rgb = ((1 - vBaseTex.rgb) * float3(1.f, 0.831f, 0.f)) + (vBaseTex.r * 1.25f);
        
        Out.vColor.rgb = min(Out.vColor.rgb, 1.f);
        
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
    
    if (g_bState)
    {
        Out.vColor.rgb *= 0.38f;
    }
  
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


PS_OUT PS_BG(PS_IN In)
{
    PS_OUT Out;

    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    vector vBGMaterial = g_BGTexture.Sample(LinearSampler, In.vTexcoord);
    
    vBGMaterial.a *= g_fAlphaTimer;
    
    if (Out.vColor.a < 0.1f)
        discard;
  
    vector AColor = lerp(Out.vColor, vBGMaterial, vBGMaterial.a);
    AColor += lerp(vBGMaterial, Out.vColor, 1 - vBGMaterial.a);
    Out.vColor = AColor;
    
    return Out;
}

PS_OUT PS_VIDEO(PS_IN In)
{
    PS_OUT Out;

    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    vector vBGMaterial = g_BGTexture.Sample(LinearSampler, In.vTexcoord);
    
    
    //vBGMaterial.a = 0.2f;
     vBGMaterial.rgb = min(vBGMaterial.rgb, 1.f);
   
//    Out.vColor = lerp(Out.vColor, vBGMaterial, vBGMaterial.a);
    vector AColor = lerp(Out.vColor, vBGMaterial, 0.5f);
    AColor += lerp(vBGMaterial, Out.vColor, 0.5f);
    
    Out.vColor = AColor;
    
    return Out;
}

PS_OUT PS_SpaceLight(PS_IN In)
{
    PS_OUT Out;

    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    Out.vColor.a = Out.vColor.r;

    Out.vColor.a *= g_fAlphaTimer;
    
    Out.vColor.rgb = float3(0.678f, 0.847f, 0.902f);

    
    return Out;
}


PS_OUT PS_SkillPanel(PS_IN In)
{
    PS_OUT Out;

    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    
   vector vEffectTexture = g_MaskTexture.Sample(LinearSampler, In.vTexcoord);
   
   if (g_bState)
   {
        Out.vColor.rgb = lerp(Out.vColor.rgb, vEffectTexture.rgb * (1 - g_Radio), In.vTexcoord.y);
      
    }
  
  if (Out.vColor.a <= 0.1f)
      discard;
   
    return Out;
}

PS_OUT PS_Volume(PS_IN In)
{
    PS_OUT Out;

    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    
    Out.vColor.a = 1.f;
        
    if (g_Radio >= In.vTexcoord.x)
        Out.vColor.rgb = float3(0.043f, 0.952f, 0.945f);
    else 
        Out.vColor.rgb = float3(0.2f, 0.2f, 0.2f);
    
    if (g_Radio == 0.5f)
    {
        if (g_Radio >= In.vTexcoord.x)
            Out.vColor.rgb = float3(0.996f, 0.729f, 0.f);
        else 
            Out.vColor.rgb = float3(0.2f, 0.2f, 0.2f);
    }
        
    
      return Out;
}

PS_OUT PS_SelectIcon(PS_IN In)
{
    PS_OUT Out;

    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    vector vMask = g_MaskTexture.Sample(LinearSampler, In.vTexcoord - 0.05f);
    
    if (vMask.a < 0.1f)
        discard;
    
    if (Out.vColor.a < 0.1f)
        discard;
    
       Out.vColor.rgb *= vMask.rgb;
        Out.vColor.a = vMask.r;
    
    float2 vTopTexcoord = { 0.5f , 0.f};
    float2 vBotTexcoord = { 0.7f ,1.f};
    
    float fLineY = (vBotTexcoord.y - vTopTexcoord.y) / (vBotTexcoord.x - vTopTexcoord.x) * (In.vTexcoord.x - vTopTexcoord.x) + vTopTexcoord.y - In.vTexcoord.y;
    
    if (fLineY > 0)
        discard;
    
    return Out;
}

PS_OUT PS_CHARA_BG(PS_IN In)
{
    PS_OUT Out;

    vector LineTexture = g_Texture.Sample(LinearSampler, In.vTexcoord);
    vector vBGTexutre = g_BGTexture.Sample(LinearSampler, In.vTexcoord);
    
    float2 fMaskTexcoord = float2(g_MaskTimer , 0.f);
    float2 vMaskTexCoord = (In.vTexcoord + fMaskTexcoord);
   
    vector vDustTexture = g_MaskTexture.Sample(LinearSampler, vMaskTexCoord);
    
    LineTexture.rgb *= vBGTexutre.rgb;
    vDustTexture.rgb *= vBGTexutre.rgb;
    
    vBGTexutre.rgb += LineTexture.rgb;
    vBGTexutre.rgb += vDustTexture.rgb;
    Out.vColor = vBGTexutre;
   
    return Out;
}

PS_OUT PS_Default_NoneAlpha(PS_IN In)
{
    PS_OUT Out;

    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

    Out.vColor.a = Out.vColor.r;
    
    Out.vColor.a *= g_fAlphaTimer;
    
    if (Out.vColor.a <= 0.1f)
        discard;
    
    return Out;
}

PS_OUT PS_LightCircle(PS_IN In)
{
    PS_OUT Out;

    Out.vColor = g_Texture.Sample(DestroySampler, In.vTexcoord);
        
    float fDistance = abs(In.vTexcoord - float2(0.5f, 0.5f));
            
    Out.vColor.a -= (fDistance * 2.f);
    Out.vColor.a = max(0.f, Out.vColor.a);
  
    return Out;
}

PS_OUT PS_VS_BG(PS_IN In)
{
    PS_OUT Out;
    
    vector BaseTex = g_Texture.Sample(LinearSampler, In.vTexcoord);
    vector BGTex = g_BGTexture.Sample(LinearSampler, In.vTexcoord);
    
    vector MaskTex = g_MaskTexture.Sample(LinearSampler, In.vTexcoord);
    
    vector CircleTex[4];
    

    float2 RotaCoord = RotateUV(In.vTexcoord, float2(0.5f, 0.5f), radians(g_MaskTimer));
    float2 ReverseRotaCoord = RotateUV(In.vTexcoord, float2(0.5f, 0.5f), radians(360 - g_MaskTimer));
    CircleTex[0] = g_CircleTexture0.Sample(LinearSampler, RotaCoord);
    CircleTex[1] = g_CircleTexture1.Sample(LinearSampler, ReverseRotaCoord);
    CircleTex[2] = g_CircleTexture2.Sample(LinearSampler, RotaCoord);
    CircleTex[3] = g_CircleTexture3.Sample(LinearSampler, ReverseRotaCoord);
    
        
     Out.vColor = saturate(BaseTex * BGTex);
    
     if (MaskTex.a <= 0.1f)
         MaskTex = 0.f;
      
        MaskTex *= BGTex;
    
    for (int i = 0; i < 4; ++i)
    {
        if (CircleTex[i].a <= 0.1f)
            CircleTex[i] = 0.f;
        
        CircleTex[i] *= BGTex;
        Out.vColor.rgb += CircleTex[i].rgb;
    }
     
     Out.vColor.rgb += MaskTex.rgb;
     Out.vColor = min(1.f, Out.vColor);
    
    return Out;
}

PS_OUT PS_VS_PANEL(PS_IN In)
{
    PS_OUT Out;
    
    vector BaseTex = g_Texture.Sample(LinearSampler, In.vTexcoord);
    
    float2 vMarkTex = float2(In.vTexcoord.x, In.vTexcoord.y + 0.25f);
    vector MarkTex = g_MarkTexture.Sample(LinearSampler, vMarkTex);
    vector NameTex = g_NameTexture.Sample(LinearSampler, vMarkTex);

    
    float2 vTex = (In.vTexcoord.x * 2, In.vTexcoord.y);
    vector BGTex = g_BGTexture.Sample(LinearSampler, vTex);
    
    Out.vColor = BaseTex * BGTex;
    
    float2 vTopTexcoord = { 0.f, 0.37f };
    float2 vBotTexcoord = { 1.f, 0.27f };
    
    float fLineX = (In.vTexcoord.y - vTopTexcoord.y) * ((vBotTexcoord.x - vTopTexcoord.x) / (vBotTexcoord.y - vTopTexcoord.y)) + vTopTexcoord.x - In.vTexcoord.x;
    
    if (fLineX > 0)
        MarkTex = 0.f;
    
    if (Out.vColor.a > 0.1f)
    {
        Out.vColor += MarkTex;
    }
   
    return Out;
}

PS_OUT PS_VS_DynamicLight(PS_IN In)
{
    PS_OUT Out;

    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    
    Out.vColor.a *= 0.8f;
    Out.vColor.rgb *= 1.5f;

    return Out;
}

PS_OUT PS_VS_Bar(PS_IN In)
{
    PS_OUT Out;

    vector BaseTexture  = g_Texture.Sample(LinearSampler, In.vTexcoord);
    
    float2 vTex = (In.vTexcoord.x , In.vTexcoord.y - 0.075f);
    vector MaskTexture = g_MaskTexture.Sample(LinearSampler, vTex);
        
    Out.vColor = BaseTexture *MaskTexture;
    
    return Out;
}

PS_OUT PS_VS_Name(PS_IN In)
{
    PS_OUT Out;

    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
     
    Out.vColor.rgb = lerp(float3(1.0, 1.0, 0.0), float3(1.0, 0.647, 0.0), In.vTexcoord.y * 2.f);
    
    
    //if (Out.vColor.a <= 0.5f)
    //{
    //    Out.vColor.rgb = float3(0.f, 0.f, 0.f);
    //    Out.vColor.a = 1.f;
    //}
   

    return Out;
}


PS_OUT PS_FLYEFF(PS_IN In)
{
    PS_OUT Out;
    
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    Out.vColor.a = Out.vColor.r;
    
    float fDistance = length(abs(In.vTexcoord - float2(0.5f, 0.5f)));
    
    if (fDistance >= 0.25f)
        discard;
    
    Out.vColor.rgb *= g_vColor.rgb;
    Out.vColor.a = 0.75f;
    
    return Out;
}

PS_OUT PS_DRAGONBALL(PS_IN In)
{
    PS_OUT Out;
    
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    Out.vColor.rgb += g_MaskTimer;
    
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

//18
    pass BG
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
 
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_BG();
    }

//19
    pass Video
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
 
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_VIDEO();
    }

//20
    pass SpaceLight
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
 
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_SpaceLight();
    }

//21
    pass SkillPanel
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
 
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_SkillPanel();
    }

//22
    pass Volume
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
 
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_Volume();
    }

//23
    pass SelectIcon
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
 
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_SelectIcon();
    }

//24
    pass CHARA_BG
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
 
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_CHARA_BG();
    }

//25
    pass Default_NoneAlpha
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
 
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_Default_NoneAlpha();
    }

//26
    pass LightCircle
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
 
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_LightCircle();
    }

//27
    pass VS_BG
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
 
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_VS_BG();
    }

//28
    pass VS_Panel
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
 
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_VS_PANEL();
    }

//29
    pass VS_DynamicLight
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
 
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_VS_DynamicLight();
    }

//30
    pass VS_Bar
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
 
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_VS_Bar();
    }

//31
    pass VS_Name
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
 
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_VS_Name();
    }

//32
    pass FLYEFF
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
 
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_FLYEFF();
    }

//33
    pass DRAGONBALL
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
 
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_DRAGONBALL();
    }

//34
    pass DepthNone
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

}
