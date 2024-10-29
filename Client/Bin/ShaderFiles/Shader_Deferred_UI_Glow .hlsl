
#include "Renderer_Shader_Defines.hlsli"

float4x4		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

float4x4		g_ViewMatrixInv, g_ProjMatrixInv;
texture2D		g_Texture;
texture2D		g_NormalTexture;

texture2D		g_DiffuseTexture; /* 적용해야하는 디퓨즈 재질이 픽셀마다 다르다면 각 픽셀을 그릴때 저장받아와야한다. */

texture2D		g_DepthTexture;

float2 g_DownSamplingSize;
float2 g_DownTexSize;

static const float g_fWeight[13] =
{
    // 0.0044, 0.0175, 0.0540, 0.1295, 0.2420, 0.3521, 0.3989, 0.3521, 0.2420, 0.1295, 0.0540, 0.0175, 0.0044
    0.0561, 0.1353, 0.278, 0.4868, 0.7261, 0.9231, 1.f, 0.9231, 0.7261, 0.4868, 0.278, 0.1353, 0.0561 // 기존 것보다 더 강한 효과
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
            colorSample += g_Texture.Sample(LinearSampler, texCoord + float2(x, y) * texelSize);
        }
    }
    colorSample /= (sampleSize.x * sampleSize.y);

    return colorSample;
}

float4 Blur_X(float2 vTexCoord)
{
    float4 vOut = (float4) 0;

    float2 vUV = (float2) 0;

    for (int i = -6; i < 7; ++i)
    {
        vUV = vTexCoord + float2(3.f / (1920.0f * 0.25f) * i, 0.f);
        vOut += g_fWeight[6 + i] * g_Texture.Sample(LinearSampler, vUV);

    }

    vOut /= 6.5f;

    return vOut;
}

float4 Blur_Y(float2 vTexCoord)
{
    float4 vOut = (float4) 0;

    float2 vUV = (float2) 0;

    for (int i = -6; i < 7; ++i)
    {
        vUV = vTexCoord + float2(0, 3.f / (1080.0f * 0.25f) * i);
        vOut += g_fWeight[6 + i] * g_Texture.Sample(LinearSampler, vUV);
    }

    vOut /= 6.5f;
    return vOut;
}

PS_OUT PS_MAIN_BLUR_X(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = Blur_X(In.vTexcoord);
	
    return Out;
}

PS_OUT PS_MAIN_BLUR_Y(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = Blur_Y(In.vTexcoord);
	

    return Out;
}
texture2D g_BlurTexture;
PS_OUT PS_MAIN_RESULT_PRI(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vResult = g_Texture.Sample(LinearSampler, In.vTexcoord);

    vector vBlur = g_BlurTexture.Sample(LinearSampler, In.vTexcoord);

    //vBlur *= 1.3f;
    //Out.vColor = vResult + vBlur;
    //float fAlpha = (Out.vColor.r + Out.vColor.g + Out.vColor.b) / 3;

    //Out.vColor.a = saturate(fAlpha - 0.15f);
    
    Out.vColor = saturate(vResult + vBlur * 2.5f) /*+ vEffect*/;
    Out.vColor.a = saturate(Out.vColor.a - 0.1f);

    return Out;
}

PS_OUT PS_MAIN_RESULT_SUN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vResult = g_Texture.Sample(LinearSampler, In.vTexcoord);

    vector vBlur = g_BlurTexture.Sample(LinearSampler, In.vTexcoord);

    //vBlur *= 1.3f;
    //Out.vColor = vResult + vBlur;
    //float fAlpha = (Out.vColor.r + Out.vColor.g + Out.vColor.b) / 3;

    //Out.vColor.a = saturate(fAlpha - 0.15f);
    
    Out.vColor = saturate(vResult * 0.3f + vBlur * 1.f) /*+ vEffect*/;
    Out.vColor.a = saturate(Out.vColor.a - 0.3f);

    return Out;
}

PS_OUT PS_MAIN_RESULT(PS_IN In)
{

    PS_OUT Out = (PS_OUT) 0;

    vector vResult = g_Texture.Sample(LinearSampler, In.vTexcoord);

    vector vBlur = g_BlurTexture.Sample(LinearSampler, In.vTexcoord);
   /*vector      vEffect = g_EffectTexture.Sample(LinearSampler, In.vTexcoord);*/
     
    Out.vColor = saturate(vResult + vBlur * 3.2f) /*+ vEffect*/;
    //Out.vColor.a = saturate(Out.vColor.a - 0.3f);
    return Out;

}

PS_OUT PS_MAIN_RESULT_EARTH(PS_IN In)
{

    PS_OUT Out = (PS_OUT) 0;

    vector vResult = g_Texture.Sample(LinearSampler, In.vTexcoord);

    vector vBlur = g_BlurTexture.Sample(LinearSampler, In.vTexcoord);
   /*vector      vEffect = g_EffectTexture.Sample(LinearSampler, In.vTexcoord);*/

    Out.vColor = saturate(vResult + vBlur * 3.2f) /*+ vEffect*/;
    Out.vColor.a = saturate(Out.vColor.a - 0.3f);
    return Out;
}

PS_OUT PS_MAIN_DOWN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    float4 vResult = DownsamplePS(g_DownTexSize, In.vTexcoord, g_DownSamplingSize);
    // 두개 받아서 겁나 줄이기
    Out.vColor = vResult;

    return Out;
}

PS_OUT PS_MAIN_UP(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vResult = g_Texture.Sample(LinearSampler, In.vTexcoord);

    vector vBlur = g_BlurTexture.Sample(LinearSampler, In.vTexcoord);
	/*vector		vEffect = g_EffectTexture.Sample(LinearSampler, In.vTexcoord);*/
    Out.vColor = vResult + vBlur /*+ vEffect*/;
    float fAlpha = (Out.vColor.r + Out.vColor.g + Out.vColor.b) / 3;
    Out.vColor.a = saturate(fAlpha * 2);
    return Out;
}

PS_OUT PS_MAIN_DEBUG(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;	

	Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

	return Out;
}

//UI
PS_OUT PS_MAIN_RESULT_UI(PS_IN In)
{

    PS_OUT Out = (PS_OUT) 0;

    vector vResult = g_Texture.Sample(LinearSampler, In.vTexcoord);

    vector vBlur = g_BlurTexture.Sample(LinearSampler, In.vTexcoord);
   /*vector      vEffect = g_EffectTexture.Sample(LinearSampler, In.vTexcoord);*/
     
    Out.vColor = saturate(vResult + vBlur * 4.7f) /*+ vEffect*/;
    //Out.vColor.a = saturate(Out.vColor.a - 0.3f);
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

    pass Result
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

//9 //UI
    pass Result_UI //9
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_RESULT_UI();
    }
    
}














