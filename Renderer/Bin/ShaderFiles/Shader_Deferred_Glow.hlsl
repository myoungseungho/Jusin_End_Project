
#include "Renderer_Shader_Defines.hlsli"
float4 Blur_X_Aura(float2 vTexCoord);

float4 Blur_Y_Aura(float2 vTexCoord);

float4x4		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

float4x4		g_ViewMatrixInv, g_ProjMatrixInv;
texture2D		g_Texture;
texture2D		g_NormalTexture;

texture2D		g_DiffuseTexture; /* 적용해야하는 디퓨즈 재질이 픽셀마다 다르다면 각 픽셀을 그릴때 저장받아와야한다. */
texture2D g_AuraTexture;
texture2D g_AuraMaskTexture;
texture2D		g_DepthTexture;
texture2D g_GlowDescTexture;

float4 g_vAuraColor;
float g_GlowFactor;
float4 g_GlowFilterColor;
float2 g_DownSamplingSize;
float2 g_DownTexSize;

float g_fAllGlowFactor = 1.f;
float g_fGlowFactor = 3.2f;
float g_Time;
static const float g_fWeight[13] =
{
    // 0.0044, 0.0175, 0.0540, 0.1295, 0.2420, 0.3521, 0.3989, 0.3521, 0.2420, 0.1295, 0.0540, 0.0175, 0.0044
    0.0561, 0.1353, 0.278, 0.4868, 0.7261, 0.9231, 1.f, 0.9231, 0.7261, 0.4868, 0.278, 0.1353, 0.0561 // 기존 것보다 더 강한 효과
};

static const float g_fPlayerWeight[13] =
{
    0.0044, 0.0175, 0.0540, 0.1295, 0.2420, 0.3521, 0.3989, 0.3521, 0.2420, 0.1295, 0.0540, 0.0175, 0.0044
};

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
	VS_OUT			Out;
	
	vector		vPosition = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
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
	float4	vColor : SV_TARGET0;
};

struct PS_OUT_PLAYER
{
    float4 vColor : SV_TARGET0;
    float4 vMask : SV_TARGET1;
};

struct PS_OUT_SECOND
{
    float4 vColor : SV_TARGET1;
};

float4 DownsamplePS(float2 inputTexSize, float2 texCoord, float2 sampleSize)
{
    float2 texelSize = 1.0 / inputTexSize;

    int halfSizeX = int(sampleSize.x * 0.5);
    int halfSizeY = int(sampleSize.y * 0.5);

    float4 colorSample = 0;

    for (int y = -halfSizeY; y < halfSizeY; ++y)
    {
        for (int x = -halfSizeX; x < halfSizeX; ++x)
        {
            colorSample += g_Texture.Sample(DestroySampler, texCoord + float2(x, y) * texelSize);
        }
    }
    colorSample /= (sampleSize.x * sampleSize.y);

    return colorSample;
}

float4 Blur_X(float2 vTexCoord, float fFactor, float fRatio, bool bPlayer)
{
    float4 vOut = (float4) 0;
    float4 vTex = (float4) 0;
    float2 vUV = (float2) 0;

    for (int i = -6; i < 7; ++i)
    {
        vUV = vTexCoord + float2(fFactor / (1920.0f * fRatio) * i, 0.f);
        vTex = g_Texture.Sample(DestroySampler, vUV);

        vOut += bPlayer == false ? g_fWeight[6 + i] * vTex : g_fPlayerWeight[6 + i] * vTex;
    }

    vOut /= 6.5f;

    return vOut;
}

float4 Blur_Y(float2 vTexCoord, float fFactor, float fRatio, bool bPlayer)
{
    float4 vOut = (float4) 0;
    float4 vTex = (float4) 0;
    float2 vUV = (float2) 0;

    for (int i = -6; i < 7; ++i)
    {
        vUV = vTexCoord + float2(0, fFactor / (1080.0f * fRatio) * i);
        vTex = g_Texture.Sample(DestroySampler, vUV);
        vOut += bPlayer == false ? g_fWeight[6 + i] * vTex : g_fPlayerWeight[6 + i] * vTex;
    }

    vOut /= 6.5f;
    return vOut;
}

PS_OUT PS_MAIN_BLUR_X(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = Blur_X(In.vTexcoord, 3.f, 0.25f, false);
	
    return Out;
}

PS_OUT PS_MAIN_BLUR_Y(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = Blur_Y(In.vTexcoord, 3.f, 0.25f, false);
	
    return Out;
}

PS_OUT PS_MAIN_PLAYER_BLUR_X(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = Blur_X(In.vTexcoord, 1.f, 0.25f, true);

    //Out.vColor = Blur_X(In.vTexcoord, 1.f, 1.f, true);
	/*
    
    Out.vColor = Blur_X(In.vTexcoord, 1.f, 1.f, true) * 0.5f; // 밝기를 절반으로 줄임
    나중에 감마값 수치 조절해서 이쁜 연출 할 수 있을지도?
    float gamma = 2.2f;
    Out.vColor = pow(Blur_X(In.vTexcoord, 1.f, 1.f, true), 1.0f / gamma); // 감마 보정
    
    Out.vColor = Blur_X(In.vTexcoord, 0.5f, 0.5f, true); // 블러 강도 감소
    
    */
    return Out;
}

PS_OUT PS_MAIN_PLAYER_BLUR_Y(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    //float gamma = 2.2f;
    //Out.vColor = pow(Blur_Y(In.vTexcoord, 1.f, 1.f, true), 1.0f / gamma);
    
    Out.vColor = Blur_Y(In.vTexcoord, 1.f, 0.25f, true);

    return Out;
}

texture2D g_BlurTexture;
PS_OUT PS_MAIN_RESULT_PRI(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vResult = g_Texture.Sample(DestroySampler, In.vTexcoord);

    vector vBlur = g_BlurTexture.Sample(DestroySampler, In.vTexcoord);

    //vBlur *= 1.3f;
    //Out.vColor = vResult + vBlur;
    //float fAlpha = (Out.vColor.r + Out.vColor.g + Out.vColor.b) / 3;

    //Out.vColor.a = saturate(fAlpha - 0.15f);
    
    Out.vColor = saturate(vResult + vBlur * g_GlowFactor) /*+ vEffect*/;
    Out.vColor.a = saturate(Out.vColor.a - 0.1f);

    return Out;
}

PS_OUT PS_MAIN_RESULT_SUN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vResult = g_Texture.Sample(DestroySampler, In.vTexcoord);

    vector vBlur = g_BlurTexture.Sample(DestroySampler, In.vTexcoord);

    //vBlur *= 1.3f;
    //Out.vColor = vResult + vBlur;
    //float fAlpha = (Out.vColor.r + Out.vColor.g + Out.vColor.b) / 3;

    //Out.vColor.a = saturate(fAlpha - 0.15f);
    
    Out.vColor = saturate(vResult * 0.3f + vBlur * g_GlowFactor) /*+ vEffect*/;
    Out.vColor.a = saturate(Out.vColor.a - 0.3f);

    return Out;
}

PS_OUT PS_MAIN_RESULT(PS_IN In)
{

    PS_OUT Out = (PS_OUT) 0;

    vector vResult = g_Texture.Sample(DestroySampler, In.vTexcoord);

    vector vBlur = g_BlurTexture.Sample(DestroySampler, In.vTexcoord);
   /*vector      vEffect = g_EffectTexture.Sample(LinearSampler, In.vTexcoord);*/
     
    Out.vColor = saturate(vResult + vBlur * g_GlowFactor) /*+ vEffect*/;
    //Out.vColor.a = saturate(Out.vColor.a - 0.3f);
    return Out;

}

PS_OUT PS_MAIN_RESULT_EARTH(PS_IN In)
{

    PS_OUT Out = (PS_OUT) 0;

    vector vResult = g_Texture.Sample(DestroySampler, In.vTexcoord);

    vector vBlur = g_BlurTexture.Sample(DestroySampler, In.vTexcoord);
   /*vector      vEffect = g_EffectTexture.Sample(LinearSampler, In.vTexcoord);*/
    Out.vColor = saturate(vResult + vBlur * g_GlowFactor) /*+ vEffect*/;
    Out.vColor.a = saturate(Out.vColor.a - 0.3f);
    
    return Out;
}

PS_OUT PS_MAIN_DOWN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    float4 vResult = DownsamplePS(g_DownTexSize, In.vTexcoord, g_DownSamplingSize);

    Out.vColor = vResult;

    return Out;
}

PS_OUT PS_MAIN_UP(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vResult = g_Texture.Sample(DestroySampler, In.vTexcoord);

    vector vBlur = g_BlurTexture.Sample(DestroySampler, In.vTexcoord);
	/*vector		vEffect = g_EffectTexture.Sample(LinearSampler, In.vTexcoord);*/
    Out.vColor = vResult + vBlur /*+ vEffect*/;
    float fAlpha = (Out.vColor.r + Out.vColor.g + Out.vColor.b) / 3;
    Out.vColor.a = saturate(fAlpha * 2);
    return Out;
}

PS_OUT PS_MAIN_DEBUG(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;	

    Out.vColor = g_Texture.Sample(DestroySampler, In.vTexcoord);

	return Out;
}

PS_OUT PS_MAIN_RESULT_PLAYER(PS_IN In)
{

    PS_OUT Out = (PS_OUT) 0;

    vector vResult = g_Texture.Sample(DestroySampler, In.vTexcoord);

    vector vBlur = g_BlurTexture.Sample(DestroySampler, In.vTexcoord);
   /*vector      vEffect = g_EffectTexture.Sample(LinearSampler, In.vTexcoord);*/
     
    Out.vColor = saturate(vResult + vBlur) /*+ vEffect*/;
    //Out.vColor.a = saturate(Out.vColor.a - 0.3f);
    return Out;
}

PS_OUT PS_MAIN_RESULT_PLAYER_AURA(PS_IN In)
{

    PS_OUT Out = (PS_OUT) 0;

    float2 vTexcoord = In.vTexcoord * 5.f;
    vTexcoord.y += g_Time;
    vector vAura = g_AuraTexture.Sample(LinearSampler, vTexcoord);
    vector vAuraMask = g_AuraMaskTexture.Sample(LinearSampler, vTexcoord);
    
    vector vBlur = g_BlurTexture.Sample(DestroySampler, In.vTexcoord);
   /*vector      vEffect = g_EffectTexture.Sample(LinearSampler, In.vTexcoord);*/
    //vAura.rgb = float3(0.1f, 0.3f, 1.f);
    //vAura = lerp(vAuraMask, vAura, 0.5f);
    vAura = lerp(vAura, vAuraMask, 0.5f);
    
    Out.vColor.rgb = saturate(vAura.rgb * saturate(vBlur.r * 2.3f)) * g_vAuraColor.rgb;
    Out.vColor.a = saturate(vAura.a * saturate(vBlur.r * 2.3f)) * g_vAuraColor.a;
    //Out.vColor.a = saturate(Out.vColor.a - 0.3f);
    return Out;
}

PS_OUT PS_MAIN_RESULT_ALLEFFECT(PS_IN In)
{

    PS_OUT Out = (PS_OUT) 0;

    vector vResult = g_Texture.Sample(DestroySampler, In.vTexcoord);

    vector vBlur = g_BlurTexture.Sample(DestroySampler, In.vTexcoord);
    vector vFactor = g_GlowDescTexture.Sample(LinearSampler, In.vTexcoord);
   // clip(vBlur.a - 0.001f);
    //float fFactor = vFactor.r == 0.f ? 3.2f : vFactor.r;
    Out.vColor = saturate(vResult + vBlur * (g_fAllGlowFactor));
  //  Out.vColor = saturate(vResult * (1 - vBlur.a) + vBlur * vBlur.a * (g_fAllGlowFactor + 1.2f));

    return Out;
}

PS_OUT PS_MAIN_RESULT_MAP(PS_IN In)
{

    PS_OUT Out = (PS_OUT) 0;

    vector vResult = g_Texture.Sample(DestroySampler, In.vTexcoord);

    vector vBlur = g_BlurTexture.Sample(DestroySampler, In.vTexcoord);
   
   
    Out.vColor = saturate(vResult + vBlur * 1.5f);
  //  Out.vColor = saturate(vResult * (1 - vBlur.a) + vBlur * vBlur.a * (g_fAllGlowFactor + 1.2f));

    return Out;
}


float4 Blur_X_BACK(float2 vTexCoord)
{
    float4 vOut = (float4) 0;

    float2 vUV = (float2) 0;

    for (int i = -6; i < 7; ++i)
    {
        vUV = vTexCoord + float2(1.f / 1920.0f * i, 0.f);
        vOut += g_fPlayerWeight[6 + i] * g_Texture.Sample(LinearSampler, vUV);
    }

    vOut /= 6.5f;

    return vOut;
}

float4 Blur_Y_BACK(float2 vTexCoord)
{
    float4 vOut = (float4) 0;

    float2 vUV = (float2) 0;

    for (int i = -6; i < 7; ++i)
    {
        vUV = vTexCoord + float2(0, 1.f / 1080.f * i);
        vOut += g_fPlayerWeight[6 + i] * g_Texture.Sample(LinearSampler, vUV);
    }

    vOut /= 6.5f;
    return vOut;
}

PS_OUT PS_MAIN_BLUR_X_BACK(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = Blur_X_BACK(In.vTexcoord);
	

    return Out;
}

PS_OUT PS_MAIN_BLUR_Y_BACK(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = Blur_Y_BACK(In.vTexcoord);
	
    return Out;
}


PS_OUT PS_MAIN_RESULT_PLUS(PS_IN In)
{

    PS_OUT Out = (PS_OUT) 0;

    vector vResult = g_Texture.Sample(DestroySampler, In.vTexcoord);

    vector vBlur = g_BlurTexture.Sample(DestroySampler, In.vTexcoord);
   /*vector      vEffect = g_EffectTexture.Sample(LinearSampler, In.vTexcoord);*/
     
    Out.vColor = saturate(vResult + vBlur) /*+ vEffect*/;
    Out.vColor.rgb *= 0.85f;
    
    //Out.vColor.rgb = pow(Out.vColor.rgb, 1.0 / 1.8f);

   // Out.vColor.rgb = Out.vColor.rgb / (1.0 + Out.vColor.rgb);
    //Out.vColor.a = saturate(Out.vColor.a - 0.3f);

    return Out;

}

PS_OUT PS_MAIN_BLUR_X_AURA(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = Blur_X_Aura(In.vTexcoord);
	

    return Out;
}

PS_OUT PS_MAIN_BLUR_Y_AURA(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = Blur_Y_Aura(In.vTexcoord);
	
    return Out;
}

technique11		DefaultTechnique
{	
    pass Blur_X
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLUR_X();
    }

    pass Blur_Y
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLUR_Y();
    }

    pass Result // 2
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_RESULT();
    }

    pass DownSample // 3
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DOWN();
    }

    pass UpSampleSecond // 4
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_UP();
    }

    pass Result_Pri // 5
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_RESULT_PRI();
    }


	pass Debug // 6
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEBUG();
	}	

    pass Result_Earth // 7
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_RESULT_EARTH();
    }

    pass Result_Sun // 8
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_RESULT_SUN();
    }

    pass Player_Result //9
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_RESULT_PLAYER();
    }

    pass Player_Blur_X //10
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_PLAYER_BLUR_X();
    }

    pass Player_Blur_Y //11
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_PLAYER_BLUR_Y();
    }
    
    pass AllEffectResult // 12
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_OneBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_RESULT_ALLEFFECT();
    }

    pass AllMapBloomResult // 13
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_RESULT_MAP();
    }

    pass ResultPlus // 14
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_RESULT_PLUS();
    }

    pass ResultBlurX // 15
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLUR_X_BACK();
    }

    pass ResultBlurY // 16
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLUR_Y_BACK();
    }

    pass PlayerAura_Result //17
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_RESULT_PLAYER_AURA();
    }

    pass AuraBlurX // 18
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLUR_X_AURA();
    }

    pass AuraBlurY // 19
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLUR_Y_AURA();
    }
}

float4 Blur_X_Aura(float2 vTexCoord)
{
    float4 vOut = (float4) 0;

    float2 vUV = (float2) 0;

    for (int i = -6; i < 7; ++i)
    {
        vUV = vTexCoord + float2(1.f / 1920.0f * i, 0.f);
        vOut += g_fWeight[6 + i] * g_Texture.Sample(LinearSampler, vUV);
    }

    vOut /= 6.5f;

    return vOut;
}

float4 Blur_Y_Aura(float2 vTexCoord)
{
    float4 vOut = (float4) 0;

    float2 vUV = (float2) 0;

    for (int i = -6; i < 7; ++i)
    {
        vUV = vTexCoord + float2(0, 1.f / 1080.f * i);
        vOut += g_fWeight[6 + i] * g_Texture.Sample(LinearSampler, vUV);
    }

    vOut /= 6.5f;
    return vOut;
}













