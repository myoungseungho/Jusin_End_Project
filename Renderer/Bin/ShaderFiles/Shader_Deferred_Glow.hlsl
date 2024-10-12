
#include "Renderer_Shader_Defines.hlsli"

float4x4		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

float4x4		g_ViewMatrixInv, g_ProjMatrixInv;

float4x4		g_LightViewMatrix, g_LightProjMatrix;

float4			g_vCamPosition;

float4			g_vLightDir;
float4			g_vLightPos;
float			g_fLightRange;
float4			g_vLightDiffuse;
float4			g_vLightAmbient;
float4			g_vLightSpecular;

texture2D		g_Texture;
texture2D		g_NormalTexture;
/* �ȼ����� �����ؾ��ϴ� ���� ������ �޶��ٶ�� �� �ȼ��� �׸��� ��ü���� �������� �� ���� Ÿ���� �߰��� �����Ͽ� �޾ƿԾ���Ѵ�. */
/* ���� ���� ��� ���ٶ�� Diffuse + Ambient + Normal */
/* �ȼ����� �ٸ��� ���� �������� ������ ó���Ѵٸ� ������ ���� ������ �ϳ� �����ص� �ǰڴ�. */
texture2D		g_DiffuseTexture; /* �����ؾ��ϴ� ��ǻ�� ������ �ȼ����� �ٸ��ٸ� �� �ȼ��� �׸��� ����޾ƿ;��Ѵ�. */
float4			g_vMtrlAmbient = float4(1.f, 1.f, 1.f, 1.f); /* �����ؾ��ϴ� �ں��Ʈ ������ �ȼ����� �ٸ��ٸ� �� �ȼ��� �׸��� ����޾ƿ;��Ѵ�. */
float4			g_vMtrlSpecular = float4(1.f, 1.f, 1.f, 1.f); /* �����ؾ��ϴ� �ں��Ʈ ������ �ȼ����� �ٸ��ٸ� �� �ȼ��� �׸��� ����޾ƿ;��Ѵ�. */
texture2D		g_ShadeTexture;
texture2D		g_DepthTexture;
texture2D		g_SpecularTexture;
texture2D		g_LightDepthTexture;

static const float g_fWeight[13] =
{
    0.0044, 0.0175, 0.0540, 0.1295, 0.2420, 0.3521, 0.3989, 0.3521, 0.2420, 0.1295, 0.0540, 0.0175, 0.0044
    //0.0561, 0.1353, 0.278, 0.4868, 0.7261, 0.9231, 1.f, 0.9231, 0.7261, 0.4868, 0.278, 0.1353, 0.0561

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

float4 Blur_X(float2 vTexCoord)
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

float4 Blur_Y(float2 vTexCoord)
{
    float4 vOut = (float4) 0;

    float2 vUV = (float2) 0;

    for (int i = -6; i < 7; ++i)
    {
        vUV = vTexCoord + float2(0, 1.f / 1080.0f * i);
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

PS_OUT PS_MAIN_RESULT(PS_IN In)
{

    PS_OUT Out = (PS_OUT) 0;

    vector vResult = g_Texture.Sample(LinearSampler, In.vTexcoord);

   // vector vBlur = g_BlurTexture.Sample(LinearSampler, In.vTexcoord);
	/*vector		vEffect = g_EffectTexture.Sample(LinearSampler, In.vTexcoord);*/

    Out.vColor = vResult; // + vBlur /*+ vEffect*/;

    return Out;

}

PS_OUT PS_MAIN_DEBUG(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;	

	Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

	return Out;
}

struct PS_OUT_LIGHT
{
	float4	vShade : SV_TARGET0;
	float4	vSpecular : SV_TARGET1;
};

bool isOutLine = false;
float CalculateNormalDiff(float2 vTexcoord, float4 vNormal)
{
    float2 fOffsetRight = float2(1.0f / 1920.f, 0.0f);
    float2 fOffsetDown = float2(0.0f, 1.0f / 1080.f);
    float2 fOffsetLeft = float2(-1.0f / 1920.f, 0.0f);
    float2 fOffsetUp = float2(0.0f, -1.0f / 1080.f);

    float3 vNormalRight = g_NormalTexture.Sample(LinearSampler, vTexcoord + fOffsetRight).xyz * 2.f - 1.f;
    float3 vNormalDown = g_NormalTexture.Sample(LinearSampler, vTexcoord + fOffsetDown).xyz * 2.f - 1.f;
    float3 vNormalLeft = g_NormalTexture.Sample(LinearSampler, vTexcoord + fOffsetLeft).xyz * 2.f - 1.f;
    float3 vNormalUp = g_NormalTexture.Sample(LinearSampler, vTexcoord + fOffsetUp).xyz * 2.f - 1.f;

    float fNormalDiff = length(vNormal.xyz - vNormalRight) + length(vNormal.xyz - vNormalDown)
                        + length(vNormal.xyz - vNormalLeft) + length(vNormal.xyz - vNormalUp);

    return fNormalDiff;
}

float CalculateDepthDiff(float2 vTexcoord, float fViewZ)
{
    float2 fOffsetRight = float2(1.0f / 1920.f, 0.0f);
    float2 fOffsetDown = float2(0.0f, 1.0f / 1080.f);
    float2 fOffsetLeft = float2(-1.0f / 1920.f, 0.0f);
    float2 fOffsetUp = float2(0.0f, -1.0f / 1080.f);

    float2 fOffsetRightUp = float2(1.0f / 1920.f, -1.0f / 1080.f);
    float2 fOffsetRightDown = float2(1.0f / 1920.f, 1.0f / 1080.f);
    float2 fOffsetLeftUp = float2(-1.0f / 1920.f, -1.0f / 1080.f);
    float2 fOffsetLeftDown = float2(-1.0f / 1920.f, 1.0f / 1080.f);

    float fDepthRight = g_DepthTexture.Sample(LinearSampler, vTexcoord + fOffsetRight).x * 1000.f;
    float fDepthDown = g_DepthTexture.Sample(LinearSampler, vTexcoord + fOffsetDown).x * 1000.f;
    float fDepthLeft = g_DepthTexture.Sample(LinearSampler, vTexcoord + fOffsetLeft).x * 1000.f;
    float fDepthUp = g_DepthTexture.Sample(LinearSampler, vTexcoord + fOffsetUp).x * 1000.f;

    float fDepthRightUp = g_DepthTexture.Sample(LinearSampler, vTexcoord + fOffsetRightUp).x * 1000.f;
    float fDepthRightDown = g_DepthTexture.Sample(LinearSampler, vTexcoord + fOffsetRightDown).x * 1000.f;
    float fDepthLeftUp = g_DepthTexture.Sample(LinearSampler, vTexcoord + fOffsetLeftUp).x * 1000.f;
    float fDepthLeftDown = g_DepthTexture.Sample(LinearSampler, vTexcoord + fOffsetLeftDown).x * 1000.f;

    float fDepthDiff = abs(fViewZ - fDepthRight) + abs(fViewZ - fDepthDown)
                     + abs(fViewZ - fDepthLeft) + abs(fViewZ - fDepthUp)
                     + abs(fViewZ - fDepthRightUp) + abs(fViewZ - fDepthRightDown)
                     + abs(fViewZ - fDepthLeftUp) + abs(fViewZ - fDepthLeftDown);

    return fDepthDiff;
}


// �ܰ��� ���� �Լ�
float CalculateEdge(float2 vTexcoord, float fViewZ, float4 vNormal, float fEdgeThreshold, float fEdgeNormalThreshold, float fEdgeDepthThreshold)
{
   // float fNormalDiff = CalculateNormalDiff(vTexcoord, vNormal);
   
    float fDepthDiff = CalculateDepthDiff(vTexcoord, fViewZ);
   
    float fEdge = step(fEdgeDepthThreshold, fDepthDiff); // * step(fEdgeNormalThreshold, fNormalDiff);

    return fEdge;
}

PS_OUT PS_MAIN_DEFERRED(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    if (0.0f == vDiffuse.a)
        discard;

    vector vShade = g_ShadeTexture.Sample(LinearSampler, In.vTexcoord);
    vector vSpecular = g_SpecularTexture.Sample(LinearSampler, In.vTexcoord);

    Out.vColor = vDiffuse; //* vShade + vSpecular;

    vector vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexcoord);
    vector vDepthDesc = g_DepthTexture.Sample(LinearSampler, In.vTexcoord);
   
    float fViewZ = vDepthDesc.x * 1000.f;
    float4 vNormal = float4(vNormalDesc.xyz * 2.f - 1.f, 0.f);
   
    if (isOutLine == true)
    {
		float fEdgeNormalThreshold = 0.2f;
		float fEdgeDepthThreshold = 0.5f;
   
		float fEdge = CalculateEdge(In.vTexcoord, fViewZ, vNormal, 0.f, fEdgeNormalThreshold, fEdgeDepthThreshold);

		vector vOutlineBlack = float4(0.f, 0.f, 0.f, 1.f);
		Out.vColor = lerp(Out.vColor, vOutlineBlack, fEdge);
    }

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
        SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_RESULT();
    }

	pass Debug
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

	pass Deferred
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEFERRED();
	}



}














