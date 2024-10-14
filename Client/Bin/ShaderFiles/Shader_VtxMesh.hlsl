#include "Renderer_Shader_Defines.hlsli"

float4x4		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D		g_DiffuseTexture;
texture2D		g_NormalTexture;

texture2D g_MaskStar1;
texture2D g_MaskStar2;

texture2D g_EarthCloud0;
texture2D g_EarthCloud1;
texture2D g_EarthCloud2;
texture2D g_EarthLight;
texture2D g_EarthShadow;

float g_MaskStar_Value_1;
float g_MaskStar_Value_2;
float g_Time;
vector			g_vCamPosition;

float2 g_fSpriteSize;
float2 g_fSpriteCurPos;


struct VS_IN
{
	float3 vPosition : POSITION;
	float3 vNormal : NORMAL;
	float2 vTexcoord : TEXCOORD0;
	float3 vTangent : TANGENT;
};

struct VS_OUT
{
	float4 vPosition : SV_POSITION;
	float3 vNormal : NORMAL;
	float2 vTexcoord : TEXCOORD0;
	float4 vWorldPos : TEXCOORD1;
	float4 vProjPos : TEXCOORD2;
	float3 vTangent : TANGENT;
	float3 vBinormal : BINORMAL;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out;

	/* mul : 곱하기가 가능한 모든 행렬(좌변의 열, 우변의 행 같다면)에 대해서 다 곱하기를 수행해준다. */
	vector		vPosition = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
	vPosition = mul(vPosition, g_ViewMatrix);
	vPosition = mul(vPosition, g_ProjMatrix);

	/* 투영행렬까지 곱한 위치벡터 */
	/* = x : fov적용 */
	/* = y : fov적용 */
	/* = z : 0 ~ f */
	/* = w : n ~ f */

	Out.vPosition = vPosition;
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
	Out.vTexcoord = In.vTexcoord;
	Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
	Out.vProjPos = vPosition;
	Out.vTangent = normalize(mul(vector(In.vTangent, 0.f), g_WorldMatrix));
	Out.vBinormal = normalize(cross(Out.vNormal, Out.vTangent));

	return Out;
}

struct PS_IN
{
	float4 vPosition : SV_POSITION;
	float3 vNormal : NORMAL;
	float2 vTexcoord : TEXCOORD0;
	float4 vWorldPos : TEXCOORD1;
	float4 vProjPos : TEXCOORD2;
	float3 vTangent : TANGENT;
	float3 vBinormal : BINORMAL;
};

struct PS_OUT
{
	float4	vDiffuse : SV_TARGET0;
	float4	vNormal : SV_TARGET1;
	float4	vDepth : SV_TARGET2;
	//float4	vPickDepth : SV_TARGET3;
};

PS_OUT PS_MAIN_SKY(PS_IN In)
{
	PS_OUT			Out;	 
	
    vector vMtrlMask1 = g_MaskStar1.Sample(LinearSampler, In.vTexcoord * 2);
    vector vMtrlMask2 = g_MaskStar2.Sample(LinearSampler, In.vTexcoord * 2);

	vector		vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord  + g_Time * 0.03f);

    vMtrlMask1 *= 1.f - g_MaskStar_Value_1;
    vMtrlMask2 *= 1.f - g_MaskStar_Value_2;

    vector vColorMask1 = vMtrlMask1;
    vMtrlMask1 = lerp(vMtrlMask1, vMtrlMask2, g_MaskStar_Value_1);
    vMtrlMask2 = lerp(vMtrlMask2, vColorMask1, g_MaskStar_Value_2);
	
    vMtrlMask1 += vMtrlMask2;

    Out.vDiffuse = vMtrlMask1 + vMtrlDiffuse * 0.7f;

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
	//Out.vPickDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);

	return Out;
}

PS_OUT PS_MAIN_FSTAR(PS_IN In)
{
    PS_OUT Out;

    float2 texFramePos = g_fSpriteCurPos * g_fSpriteSize;

    In.vTexcoord = In.vTexcoord * g_fSpriteSize + texFramePos;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    vMtrlDiffuse *= 3.3f;
	
    float luminance = 0.299f * vMtrlDiffuse.x + 0.587f * vMtrlDiffuse.y + 0.114f * vMtrlDiffuse.z;
    vMtrlDiffuse.a = saturate(luminance * 2.0f);

    //vMtrlDiffuse.a = (vMtrlDiffuse.x + vMtrlDiffuse.y + vMtrlDiffuse.z) / 3;
	
    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);

    return Out;
}

PS_OUT PS_MAIN_EARTH(PS_IN In)
{
/*
g_EarthCloud0;
g_EarthCloud1;
g_EarthCloud2;
g_EarthLight;
g_EarthShadow;
*/
    PS_OUT Out;
	
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
	
    vector vMtrlLight = g_EarthLight.Sample(LinearSampler, In.vTexcoord);
    vector vMtrlShadow = g_EarthShadow.Sample(LinearSampler, In.vTexcoord);

    float Lightluminance = 0.299f * vMtrlLight.x + 0.587f * vMtrlLight.y + 0.114f * vMtrlLight.z;
    vMtrlLight.a = saturate(Lightluminance * 1.0f);
	
    float Shadowluminance = 0.299f * vMtrlShadow.x + 0.587f * vMtrlShadow.y + 0.114f * vMtrlShadow.z;
    vMtrlShadow.a = saturate(Shadowluminance * 2.0f);
	
  //  vector vMtrlLerpLight = lerp(vMtrlLight, -vMtrlShadow, 0.5);
	
    vMtrlDiffuse += vMtrlLight * 2;
	
    //vMtrlDiffuse = lerp(vMtrlDiffuse, vMtrlShadow, vMtrlShadow.a);
	
    //vMtrlMask1 *= 1.f - g_MaskStar_Value_1;
    //vMtrlMask2 *= 1.f - g_MaskStar_Value_2;

    //vector vColorMask1 = vMtrlMask1;
    //vMtrlMask1 = lerp(vMtrlMask1, vMtrlMask2, g_MaskStar_Value_1);
    //vMtrlMask2 = lerp(vMtrlMask2, vColorMask1, g_MaskStar_Value_2);
	
    //vMtrlMask1 += vMtrlMask2;

    Out.vDiffuse = vMtrlDiffuse; //vMtrlMask1 + vMtrlDiffuse * 0.7f;

    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
	//Out.vPickDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);

    return Out;
}

PS_OUT PS_MAIN_NORMALMAPPING(PS_IN In)
{
	PS_OUT			Out;

	vector		vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
	if (vMtrlDiffuse.a < 0.1f)
		discard;

	vector		vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexcoord);
	float3		vNormal = vNormalDesc.xyz * 2.f - 1.f;

	float3x3	WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal);

	vNormal = mul(vNormal, WorldMatrix);




	Out.vDiffuse = vMtrlDiffuse;
	Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
	//Out.vPickDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);

	return Out;
}


technique11		DefaultTechnique
{	
// 0
	pass Sky
	{		
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		//SetDepthStencilState();
		//SetBlendState();

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SKY();
    }
// 1
    pass FallingStar
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		//SetDepthStencilState();
		//SetBlendState();

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_FSTAR();
    }
// 2
    pass Earth
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		//SetDepthStencilState();
		//SetBlendState();

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_EARTH();
    }

	pass NormalMapping
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		//SetDepthStencilState();
		//SetBlendState();

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_NORMALMAPPING();
	}
}














