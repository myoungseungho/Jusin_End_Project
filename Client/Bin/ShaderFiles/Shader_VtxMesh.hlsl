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

texture2D g_GroundPattern;
texture2D g_GroundCliff;
texture2D g_GroundCrater;
texture2D g_GroundShadow;

texture2D g_SunLight;
texture2D g_SunRainbow;

int g_SunMeshIndex;
int g_GroundCount;

float g_MaskStar_Value_1;
float g_MaskStar_Value_2;
float g_Time;
float g_TimeEarth;
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
    PS_OUT Out;
    float2 vTex, vTexEarth;
	
    vTex.x = In.vTexcoord.x * 2;
    vTex.y = In.vTexcoord.y;
    vTexEarth.x = In.vTexcoord.x + 0.8f;
    vTexEarth.y = In.vTexcoord.y + 0.3f;
    vector vMtrlCloud1 = g_EarthCloud1.Sample(LinearSampler, vTex + g_Time * 0.003f);
    vector vMtrlCloud2 = g_EarthCloud2.Sample(LinearSampler, vTex + g_Time * 0.003f);
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, vTexEarth + g_Time * 0.001f);
    vMtrlCloud1 *= vMtrlCloud2;
	
    vTex.x = In.vTexcoord.x * 2;
    vTex.y = In.vTexcoord.y;
    vector vMtrlLight = g_EarthLight.Sample(LinearSampler, vTex);
    vector vMtrlShadow = g_EarthShadow.Sample(LinearSampler, vTex);

    float Shadowluminance = 0.299f * vMtrlShadow.x + 0.587f * vMtrlShadow.y + 0.114f * vMtrlShadow.z;
    vMtrlShadow.rgba = saturate(Shadowluminance * 2.0f);

    vMtrlLight.rgb *= saturate(1 - vMtrlShadow.r);

    vMtrlDiffuse += vMtrlLight + vMtrlCloud1;
    vMtrlDiffuse.b *= 1.3f;
    
    //float contrast = 1.7f;
    //float3 enhancedColor = saturate((vMtrlDiffuse.rgb - 0.5f) * contrast + 0.5f);
    //vMtrlDiffuse.rgb = enhancedColor;
   // vMtrlDiffuse += vMtrlLight;
	
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
PS_OUT PS_MAIN_EARTH_LIGHT(PS_IN In)
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
	
    float2 vTex;
    vTex.x = In.vTexcoord.x * 2;
    vTex.y = In.vTexcoord.y;
    vector vMtrlLight = g_EarthLight.Sample(LinearSampler, vTex);
    vector vMtrlShadow = g_EarthShadow.Sample(LinearSampler, vTex);
	
    float Shadowluminance = 0.299f * vMtrlShadow.x + 0.587f * vMtrlShadow.y + 0.114f * vMtrlShadow.z;
    vMtrlShadow.a = saturate(Shadowluminance * 2.0f);

    vMtrlLight.a -= 1 - vMtrlShadow.a;


    Out.vDiffuse = vMtrlLight * 2;

    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
	//Out.vPickDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);

    return Out;
}
PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
   // vector vMtrlAlpha = g_AlphaTexture.Sample(LinearSampler, In.vTexcoord);
    //if (vMtrlDiffuse.a < 0.99f)
    //    discard;

    Out.vDiffuse = vMtrlDiffuse;

    Out.vDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);

    return Out;
}

PS_OUT PS_MAIN_HORIZON(PS_IN In)
{
    PS_OUT Out;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    float Shadowluminance = 0.299f * vMtrlDiffuse.x + 0.587f * vMtrlDiffuse.y + 0.114f * vMtrlDiffuse.z;
    vMtrlDiffuse.a = saturate(Shadowluminance * 1.5f);
 
    vMtrlDiffuse.rgb = float3(0.2431f, 0.4823f, 0.8117f);

    Out.vDiffuse = vMtrlDiffuse;

    Out.vDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);

    return Out;
}

PS_OUT PS_MAIN_GROUND(PS_IN In)
{
    PS_OUT Out;
    vector vMtrlDiffuse;
    switch (g_GroundCount)
    {
    case 0:
        {
        vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);    
        vector vMtrlPattern = g_GroundPattern.Sample(LinearSampler, In.vTexcoord);
                vMtrlDiffuse *= vMtrlPattern;
            }
        break;
    case 1:
        vMtrlDiffuse = g_GroundCliff.Sample(LinearSampler, In.vTexcoord);
        break;
    case 2:
        vMtrlDiffuse = g_GroundCrater.Sample(LinearSampler, In.vTexcoord);
        break;
    }

    Out.vDiffuse = vMtrlDiffuse;

    Out.vDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);

    return Out;
}

PS_OUT PS_MAIN_SUN(PS_IN In)
{
    PS_OUT Out;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
   // vector vMtrlAlpha = g_AlphaTexture.Sample(LinearSampler, In.vTexcoord);
    //if (vMtrlDiffuse.a < 0.99f)
    //    discard;

    Out.vDiffuse = vMtrlDiffuse;

    Out.vDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);

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
        SetDepthStencilState(DSS_None, 0);
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

    pass Earth_Light
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		//SetDepthStencilState();
		//SetBlendState();

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_EARTH_LIGHT();
    }

    pass Default // 4
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
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass Ground // 5
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
        PixelShader = compile ps_5_0 PS_MAIN_GROUND();
    }

    pass Horizon // 6
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		//SetDepthStencilState();
		//SetBlendState();

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_HORIZON();
    }

    pass Sun // 7
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		//SetDepthStencilState();
		//SetBlendState();

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SUN();
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














