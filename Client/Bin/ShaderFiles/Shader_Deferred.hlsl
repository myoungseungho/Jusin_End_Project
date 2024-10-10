
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


PS_OUT_LIGHT PS_MAIN_DIRECTIONAL(PS_IN In)
{
	//PS_OUT_LIGHT		Out = (PS_OUT_LIGHT)0;
	//
	//vector			vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexcoord);
	//vector			vDepthDesc = g_DepthTexture.Sample(LinearSampler, In.vTexcoord);
	//float			fViewZ = vDepthDesc.x * 1000.f;
	//
	///* 0 ~ 1 -> -1 ~ 1 */
	//float4			vNormal = float4(vNormalDesc.xyz * 2.f - 1.f, 0.f);	
	//
	//float4			vAmbient = g_vLightAmbient * g_vMtrlAmbient;
	//
	//Out.vShade = g_vLightDiffuse * max(dot(normalize(g_vLightDir) * -1.f, vNormal), 0.f) + vAmbient;
	//
	//float4			vWorldPos;
	//
	///* ������ġ * ������� * ����� * ������� / View.z */
	///* ������������ ��ġ�� ���� ���Ѵ�. */
	//vWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
	//vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
	//vWorldPos.z = vDepthDesc.y;
	//vWorldPos.w = 1.f;
	//
	///* ������ġ * ������� * ����� * �������  */
	//vWorldPos = vWorldPos * fViewZ;
	//
	///* ������ġ * ������� * ����� */
	//vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
	//
	///* ������ġ * ������� */
	//vWorldPos = mul(vWorldPos, g_ViewMatrixInv);
	//
	//float4			vReflect = reflect(normalize(g_vLightDir), vNormal);
	//float4			vLook = vWorldPos - g_vCamPosition;
	//
	//Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(max(dot(normalize(vReflect) * -1.f, normalize(vLook)), 0.f), 30.f);
	//
	//return Out;

	
    PS_OUT_LIGHT Out = (PS_OUT_LIGHT) 0;

    vector vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexcoord);
    vector vDepthDesc = g_DepthTexture.Sample(LinearSampler, In.vTexcoord);
    float fViewZ = vDepthDesc.x * 1000.f;

   /* 0 ~ 1 -> -1 ~ 1 */
    float4 vNormal = float4(vNormalDesc.xyz * 2.f - 1.f, 0.f);

    // �ں��Ʈ ����
    float4 vAmbient = g_vLightAmbient * g_vMtrlAmbient;

    float shadeIntensity = max(dot(normalize(g_vLightDir) * -1.f, vNormal), 0.f);
    shadeIntensity = saturate(shadeIntensity);

    float shadeStep = 2.0f;
    shadeIntensity = floor(shadeIntensity * shadeStep) / shadeStep;

    Out.vShade = (g_vLightDiffuse * shadeIntensity) + vAmbient;
    float4 vWorldPos;

   /* ������ġ * ������� * ����� * ������� / View.z */
   /* ������������ ��ġ�� ���� ���Ѵ�. */
    vWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
    vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
    vWorldPos.z = vDepthDesc.y;
    vWorldPos.w = 1.f;

   /* ������ġ * ������� * ����� * �������  */
    vWorldPos = vWorldPos * fViewZ;

   /* ������ġ * ������� * ����� */
    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

   /* ������ġ * ������� */
    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

    float4 vReflect = reflect(normalize(g_vLightDir), vNormal);
    float4 vLook = vWorldPos - g_vCamPosition;
   // ���� ���̱����� �и�
    float specularIntensity = max(dot(normalize(vReflect) * -1.f, normalize(vLook)), 0.f);
   
   //���̱�
    float specularStep = 2.0f;
    specularIntensity = floor(specularIntensity * specularStep) / specularStep;

   // �ּҰ�
    specularIntensity = max(specularIntensity, 0.1f);
   
    float specularThreshold = 0.8f; // 0.9 �̻��� ���� ����ŧ�� ����
   
    if (specularIntensity >= specularThreshold)
    {
        // ����ŧ�� ���� ����
        Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(specularIntensity, 1.5f);

    }
    else
    {
        // ����ŧ�� ������ (Ȥ�� �ſ� ���ϰ� ����)
        Out.vSpecular = float4(0.f, 0.f, 0.f, 1.0f); // ����ŧ���� ����
    }
    return Out;

}

PS_OUT_LIGHT PS_MAIN_POINT(PS_IN In)
{
	PS_OUT_LIGHT		Out = (PS_OUT_LIGHT)0;

	vector			vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexcoord);
	vector			vDepthDesc = g_DepthTexture.Sample(LinearSampler, In.vTexcoord);
	float			fViewZ = vDepthDesc.x * 1000.f;

	float4			vWorldPos;

	/* ������ġ * ������� * ����� * ������� / View.z */
	/* ������������ ��ġ�� ���� ���Ѵ�. */
	vWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
	vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
	vWorldPos.z = vDepthDesc.y;
	vWorldPos.w = 1.f;

	/* ������ġ * ������� * ����� * �������  */
	vWorldPos = vWorldPos * fViewZ;

	/* ������ġ * ������� * ����� */
	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	/* ������ġ * ������� */
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	/* 0 ~ 1 -> -1 ~ 1 */
	float4			vNormal = float4(vNormalDesc.xyz * 2.f - 1.f, 0.f);

	float4			vAmbient = g_vLightAmbient * g_vMtrlAmbient;

	float4			vLightDir = vWorldPos - g_vLightPos;

	float			fAtt = max(g_fLightRange - length(vLightDir), 0.0f) / g_fLightRange;

	Out.vShade = (g_vLightDiffuse * max(dot(normalize(vLightDir) * -1.f, vNormal), 0.f) + vAmbient) * fAtt;

	float4			vReflect = reflect(normalize(vLightDir), vNormal);
	float4			vLook = vWorldPos - g_vCamPosition;

	Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(max(dot(normalize(vReflect) * -1.f, normalize(vLook)), 0.f), 30.f) * fAtt;

	return Out;

}

//PS_OUT PS_MAIN_DEFERRED(PS_IN In)
//{
//	PS_OUT			Out = (PS_OUT)0;
//
//	vector			vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
//	if (0.0f == vDiffuse.a)
//		discard;
//
//	vector			vShade = g_ShadeTexture.Sample(LinearSampler, In.vTexcoord);
//	vector			vSpecular = g_SpecularTexture.Sample(LinearSampler, In.vTexcoord);
//
//	Out.vColor = vDiffuse * vShade + vSpecular;
//
//
//	vector			vDepthDesc = g_DepthTexture.Sample(LinearSampler, In.vTexcoord);
//	float			fViewZ = vDepthDesc.x * 1000.f;
//
//
//	float4			vWorldPos;
//
//	/* ������ġ * ������� * ����� * ������� / View.z */
//	/* ������������ ��ġ�� ���� ���Ѵ�. */
//	vWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
//	vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
//	vWorldPos.z = vDepthDesc.y;
//	vWorldPos.w = 1.f;
//
//	/* ������ġ * ������� * ����� * �������  */
//	vWorldPos = vWorldPos * fViewZ;
//
//	/* ������ġ * ������� * ����� */
//	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
//
//	/* ������ġ * ������� */
//	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);
//
//	vWorldPos = mul(vWorldPos, g_LightViewMatrix);
//	vWorldPos = mul(vWorldPos, g_LightProjMatrix);
//
//
//	/* ������������ ��ǥ�� ��ȯ�ϳ�. */
//	float2			vTexcoord = vWorldPos.xy / vWorldPos.w;
//
//	vTexcoord.x = saturate(vTexcoord.x * 0.5f + 0.5f);
//	vTexcoord.y = saturate(vTexcoord.y * -0.5f + 0.5f);
//
//	vector			vOldLightDepth = g_LightDepthTexture.Sample(LinearSampler, vTexcoord);
//	
//	float	fDepth = vWorldPos.w;
//
//	if (vOldLightDepth.x * 1000.f < fDepth - 0.3f)
//		Out.vColor = Out.vColor * 0.5f;
//
//	return Out;
//}


float CalculateNormalDiff(float2 vTexcoord, float4 vNormal)
{
    float2 fOffsetRight = float2(1.0f / 1280.f, 0.0f);
    float2 fOffsetDown = float2(0.0f, 1.0f / 720.f);
    float2 fOffsetLeft = float2(-1.0f / 1280.f, 0.0f);
    float2 fOffsetUp = float2(0.0f, -1.0f / 720.f);

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
    float2 fOffsetRight = float2(1.0f / 1280.f, 0.0f);
    float2 fOffsetDown = float2(0.0f, 1.0f / 720.f);
    float2 fOffsetLeft = float2(-1.0f / 1280.f, 0.0f);
    float2 fOffsetUp = float2(0.0f, -1.0f / 720.f);

    float fDepthRight = g_DepthTexture.Sample(LinearSampler, vTexcoord + fOffsetRight).x * 1000.f;
    float fDepthDown = g_DepthTexture.Sample(LinearSampler, vTexcoord + fOffsetDown).x * 1000.f;
    float fDepthLeft = g_DepthTexture.Sample(LinearSampler, vTexcoord + fOffsetLeft).x * 1000.f;
    float fDepthUp = g_DepthTexture.Sample(LinearSampler, vTexcoord + fOffsetUp).x * 1000.f;

    float fDepthDiff = abs(fViewZ - fDepthRight) + abs(fViewZ - fDepthDown)
                       + abs(fViewZ - fDepthLeft) + abs(fViewZ - fDepthUp);

    return fDepthDiff;
}

// �ܰ��� ���� �Լ�
float CalculateEdge(float2 vTexcoord, float fViewZ, float4 vNormal, float fEdgeThreshold, float fEdgeNormalThreshold, float fEdgeDepthThreshold)
{
    float fNormalDiff = CalculateNormalDiff(vTexcoord, vNormal);
   
    float fDepthDiff = CalculateDepthDiff(vTexcoord, fViewZ);
   
    float fEdge = step(fEdgeDepthThreshold, fDepthDiff) * step(fEdgeNormalThreshold, fNormalDiff);

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

    Out.vColor = vDiffuse * vShade + vSpecular;

    vector vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexcoord);
    vector vDepthDesc = g_DepthTexture.Sample(LinearSampler, In.vTexcoord);
   
    float fViewZ = vDepthDesc.x * 1000.f;
    float4 vNormal = float4(vNormalDesc.xyz * 2.f - 1.f, 0.f);
   
    float fEdgeNormalThreshold = 0.2f;
    float fEdgeDepthThreshold = 0.05f;
   
   // float fEdge = CalculateEdge(In.vTexcoord, fViewZ, vNormal, 0.f, fEdgeNormalThreshold, fEdgeDepthThreshold);
   //
   // vector vOutlineBlack = float4(0.f, 0.f, 0.f, 1.f);
   // Out.vColor = lerp(Out.vColor, vOutlineBlack, fEdge);

    return Out;
}






technique11		DefaultTechnique
{	
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

	pass Light_Directional
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_OneBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DIRECTIONAL();
	}


	pass Light_Point
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_OneBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_POINT();
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









