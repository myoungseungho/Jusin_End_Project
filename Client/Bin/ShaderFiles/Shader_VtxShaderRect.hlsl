
#include "Renderer_Shader_Defines.hlsli"

float4x4		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D		g_DiffuseTexture;
texture2D		g_AlphaTexture;
texture2D		g_Texture;

texture2D		g_DepthTexture;
bool isBindTexture;
bool isAlpha;
bool isDiffuse;
float2 g_vDirection;
bool isMoveTex = false;
float g_Time = 0.f;
float g_Speed = 0.05f;

bool g_isSprite;
float2 g_fSpriteSize;
float2 g_fSpriteCurPos;

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

	/* mul : 곱하기가 가능한 모든 행렬(좌변의 열, 우변의 행 같다면)에 대해서 다 곱하기를 수행해준다. */
	vector		vPosition = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
	vPosition = mul(vPosition, g_ViewMatrix);
	vPosition = mul(vPosition, g_ProjMatrix);

	Out.vPosition = vPosition;
	Out.vTexcoord = In.vTexcoord;

	return Out;
}

struct VS_OUT_EFFECT
{
	float4 vPosition : SV_POSITION;
	float2 vTexcoord : TEXCOORD0;
	float4 vProjPos : TEXCOORD1;
};

VS_OUT_EFFECT VS_MAIN_EFFECT(VS_IN In)
{
	VS_OUT_EFFECT			Out;

	/* mul : 곱하기가 가능한 모든 행렬(좌변의 열, 우변의 행 같다면)에 대해서 다 곱하기를 수행해준다. */
	vector		vPosition = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
	vPosition = mul(vPosition, g_ViewMatrix);
	vPosition = mul(vPosition, g_ProjMatrix);

	Out.vPosition = vPosition;
	Out.vTexcoord = In.vTexcoord;
	Out.vProjPos = vPosition;

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
    float4 vAlpha : SV_TARGET1;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out;	

    if (isBindTexture == true)
    {
        if (isMoveTex == true)
        {
            In.vTexcoord += normalize(g_vDirection) * g_Speed * g_Time;
        }
        else if (g_isSprite == true)
        {

            float2 texFramePos = g_fSpriteCurPos * g_fSpriteSize;


            In.vTexcoord = In.vTexcoord * g_fSpriteSize + texFramePos;


           // Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

        }
        Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
       
	
        if (isDiffuse == true)
        {
            vector vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
            Out.vColor.rgb = vDiffuse.rgb;
            Out.vColor.a = step(0.01, Out.vColor.a);
        }
        
        if (isAlpha == true) 
		{
			vector vAlpha = g_AlphaTexture.Sample(LinearSampler, In.vTexcoord);
			Out.vColor.a -= vAlpha.a;
        }
		
    }
	else
        Out.vColor = float4(1.f, 1.f, 1.f, 1.f);
	
    Out.vAlpha = vector(Out.vColor.a, 0.f, 0.f, 1.f);
	return Out;
}

struct PS_IN_EFFECT
{
	float4 vPosition : SV_POSITION;
	float2 vTexcoord : TEXCOORD0;
	float4 vProjPos : TEXCOORD1;
};

PS_OUT PS_MAIN_EFFECT(PS_IN_EFFECT In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

	float2		vTexcoord;

	vTexcoord.x = (In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f;
	vTexcoord.y = (In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f;

	vector		vDepthDesc = g_DepthTexture.Sample(LinearSampler, vTexcoord);

	Out.vColor.a = Out.vColor.a * ((vDepthDesc.x * 1000.f) - In.vProjPos.w);

	return Out;	
}

technique11		DefaultTechnique
{
	/* PASS의 기준 : 셰이더 기법의 캡슐화. */
	pass Default
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

	pass BlendEffect
	{
		SetRasterizerState(RS_Cull_None);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_EFFECT();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_EFFECT();
	}

}














