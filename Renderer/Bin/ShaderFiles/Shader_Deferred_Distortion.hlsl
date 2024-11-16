
#include "Renderer_Shader_Defines.hlsli"

float4x4		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

float4x4		g_ViewMatrixInv, g_ProjMatrixInv;

texture2D       g_Texture, g_MaskTexture, g_BackBufferTexture;

float			fLifeTime;
float			fFactor;
float g_Time;

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

PS_OUT PS_MAIN_DRAW_DISTORTION(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;	

    
    float2 speedUV = In.vTexcoord;
    speedUV.x += g_Time * 0.7f;
    
    vector vBaseDiffuse = g_Texture.Sample(LinearSampler, In.vTexcoord);
    vector vMaskDiffuse = g_MaskTexture.Sample(LinearSampler, speedUV);
    
	Out.vColor = vMaskDiffuse * vBaseDiffuse.a;
    
	return Out;
}

PS_OUT PS_MAIN_DISTORTION_TO_BACKBUFFER(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    float3 vDistortion = g_Texture.Sample(LinearSampler, In.vTexcoord).rgb;

    float2 distortedUV = In.vTexcoord;
    /* 디스토션 렌더타겟에서 0 0 0 0 인 부분을 검출 해서 만약 그렇다면 UV좌표 자기 기존 텍스쿠드로 해야될듯 */
    distortedUV.x += (vDistortion.r) * 0.05f;
    distortedUV.y += (vDistortion.g) * 0.05f;

    float4 vBackBufferColor = g_BackBufferTexture.Sample(LinearSampler, distortedUV);

    Out.vColor = vBackBufferColor;

    return Out;
}

PS_OUT PS_MAIN_RESULT(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    
    return Out;
}

technique11		DefaultTechnique
{	
	pass DrawDistortion
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DRAW_DISTORTION();
	}	

    pass DrawDistortionToBackBuffer
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DISTORTION_TO_BACKBUFFER();
    }

    pass Result
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_RESULT();
    }

}









