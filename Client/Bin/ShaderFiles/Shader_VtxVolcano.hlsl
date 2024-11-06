#include "Renderer_Shader_Defines.hlsli"

float4x4		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D		g_DiffuseTexture;
texture2D		g_MaskTexture;


int g_SunMeshIndex;
int g_GroundCount;

float g_Time = 0.f;

vector			g_vCamPosition;


float2 g_fSpriteCurPos;
float4 g_vCamPos;

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

VS_OUT VS_MAIN_RECT(VS_IN In)
{
    VS_OUT Out;

	/* mul : 곱하기가 가능한 모든 행렬(좌변의 열, 우변의 행 같다면)에 대해서 다 곱하기를 수행해준다. */
    vector vPosition = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
    vPosition = mul(vPosition, g_ViewMatrix);
    vPosition = mul(vPosition, g_ProjMatrix);

    Out.vPosition = vPosition;
    Out.vTexcoord = In.vTexcoord;

    return Out;
}

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

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
   // vector vMtrlAlpha = g_AlphaTexture.Sample(LinearSampler, In.vTexcoord);
    //if (vMtrlDiffuse.a < 0.99f)
    //    discard;
	
    vector vLavaColor = { 255.f/255.f, 68.f / 255.f, 0.f / 255.f, 1.f };
    //vector vLavaColor = { 234.f / 255.f, 0.5f / 255.f, 0.f / 255.f, 1.f };
    //vLavaColor *= 0.7f;
    vector vResultColor;
    vResultColor.rgb = vMtrlDiffuse.rgb + vLavaColor.rgb * (1 - vMtrlDiffuse.a);
    vResultColor.a = 1.f;
    //vMtrlDiffuse.a
    Out.vDiffuse = vResultColor;
    
    Out.vNormal = vector(vLavaColor.rgb * (1 - vMtrlDiffuse.a), 0.f);
    //Out.vDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
    return Out;
}

PS_OUT PS_MAIN_ISLAND(PS_IN In)
{
    PS_OUT Out;

    Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);

    Out.vNormal = vector(Out.vDiffuse.rgb * Out.vDiffuse.a, 0.f);
    Out.vDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
    return Out;
}

PS_OUT PS_MAIN_LAVA(PS_IN In)
{
    PS_OUT Out;
    float2 vTexcoord = In.vTexcoord;
    vTexcoord.y = vTexcoord.y * 7.f - g_Time * 0.01f;
    vTexcoord.x *= 6.f;
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, vTexcoord);
    vMtrlDiffuse.r = saturate(vMtrlDiffuse.r * 2.7f);
    vMtrlDiffuse.b = 0.f;
    vMtrlDiffuse.g *= 0.5f;
    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = vector(Out.vDiffuse.rgb, 0.f);
    Out.vDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
    return Out;
}

PS_OUT PS_MAIN_SKY(PS_IN In)
{
    PS_OUT Out;
    
    float2 vTexcoord = In.vTexcoord;
    vTexcoord.y += g_Time * 0.01f;
    
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, vTexcoord);
    vector vMtrlMask = g_MaskTexture.Sample(LinearSampler, In.vTexcoord);
    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = vector(0.f,0.f,0.f, 0.f);
    Out.vDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
    return Out;
}

technique11		DefaultTechnique
{	
    pass Default // 0
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

    pass Island // 1
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
        PixelShader = compile ps_5_0 PS_MAIN_ISLAND();
    }

    pass Lava // 2
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
        PixelShader = compile ps_5_0 PS_MAIN_LAVA();
    }

    pass Sky // 3
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
        PixelShader = compile ps_5_0 PS_MAIN_SKY();
    }
}














