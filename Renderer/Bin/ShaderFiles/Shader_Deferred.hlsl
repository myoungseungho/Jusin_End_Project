
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
/* 픽셀마다 적용해야하는 재질 정보가 달랐다라면 그 픽셀을 그리는 객체들을 렌더링할 때 렌더 타겟을 추가로 생성하여 받아왔어야한다. */
/* 위와 같은 경우 였다라면 Diffuse + Ambient + Normal */
/* 픽셀마다 다르지 않은 전역적인 값으로 처리한다면 임의의 전역 변수를 하나 선언해도 되겠다. */
texture2D		g_DiffuseTexture; /* 적용해야하는 디퓨즈 재질이 픽셀마다 다르다면 각 픽셀을 그릴때 저장받아와야한다. */
float4			g_vMtrlAmbient = float4(1.f, 1.f, 1.f, 1.f); /* 적용해야하는 앰비언트 재질이 픽셀마다 다르다면 각 픽셀을 그릴때 저장받아와야한다. */
float4			g_vMtrlSpecular = float4(1.f, 1.f, 1.f, 1.f); /* 적용해야하는 앰비언트 재질이 픽셀마다 다르다면 각 픽셀을 그릴때 저장받아와야한다. */
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
	///* 로컬위치 * 월드행렬 * 뷰행렬 * 투영행렬 / View.z */
	///* 투영공간상의 위치를 먼저 구한다. */
	//vWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
	//vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
	//vWorldPos.z = vDepthDesc.y;
	//vWorldPos.w = 1.f;
	//
	///* 로컬위치 * 월드행렬 * 뷰행렬 * 투영행렬  */
	//vWorldPos = vWorldPos * fViewZ;
	//
	///* 로컬위치 * 월드행렬 * 뷰행렬 */
	//vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
	//
	///* 로컬위치 * 월드행렬 */
	//vWorldPos = mul(vWorldPos, g_ViewMatrixInv);
	//
	//float4			vReflect = reflect(normalize(g_vLightDir), vNormal);
	//float4			vLook = vWorldPos - g_vCamPosition;
	//
	//Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(max(dot(normalize(vReflect) * -1.f, normalize(vLook)), 0.f), 30.f);
	//
	//return Out;

	
    PS_OUT_LIGHT Out = (PS_OUT_LIGHT) 0;

    float4 vNormal = float4(vNormalDesc.xyz * 2.f - 1.f, 0.f);

    float4 vAmbient = g_vLightAmbient * g_vMtrlAmbient;



	float4			vWorldPos;
	/* 로컬위치 * 월드행렬 * 뷰행렬 * 투영행렬 / View.z */
	/* 투영공간상의 위치를 먼저 구한다. */
    vWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
    vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
    vWorldPos.z = vDepthDesc.y;
    vWorldPos.w = 1.f;

	/* 로컬위치 * 월드행렬 * 뷰행렬 * 투영행렬  */
    vWorldPos = vWorldPos * fViewZ;

	/* 로컬위치 * 월드행렬 * 뷰행렬 */
    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	/* 로컬위치 * 월드행렬 */
    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	float4			vReflect = reflect(normalize(g_vLightDir), vNormal);
	float4			vLook = vWorldPos - g_vCamPosition;
	
	Out.vShade = g_vLightDiffuse * max(dot(normalize(g_vLightDir) * -1.f, vNormal), 0.f) + vAmbient;
	Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(max(dot(normalize(vReflect) * -1.f, normalize(vLook)), 0.f), 30.f);

    float4 vReflect = reflect(normalize(g_vLightDir), vNormal);
    float4 vLook = vWorldPos - g_vCamPosition;
   // 따로 죽이기위해 분리
    float specularIntensity = max(dot(normalize(vReflect) * -1.f, normalize(vLook)), 0.f);
   
   //죽이기
    float specularStep = 2.0f;
    specularIntensity = floor(specularIntensity * specularStep) / specularStep;

   // 최소값
    specularIntensity = max(specularIntensity, 0.1f);
   
    float specularThreshold = 0.8f; // 0.9 이상의 값만 스펙큘러 적용
   
    if (specularIntensity >= specularThreshold)
    {
        // 스펙큘러 색상 적용
        Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(specularIntensity, 1.5f);

    }
    else
    {
        // 스펙큘러 미적용 (혹은 매우 약하게 적용)
        Out.vSpecular = float4(0.f, 0.f, 0.f, 1.0f); // 스펙큘러를 제거
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

	/* 로컬위치 * 월드행렬 * 뷰행렬 * 투영행렬 / View.z */
	/* 투영공간상의 위치를 먼저 구한다. */
	vWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
	vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
	vWorldPos.z = vDepthDesc.y;
	vWorldPos.w = 1.f;

	/* 로컬위치 * 월드행렬 * 뷰행렬 * 투영행렬  */
	vWorldPos = vWorldPos * fViewZ;

	/* 로컬위치 * 월드행렬 * 뷰행렬 */
	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	/* 로컬위치 * 월드행렬 */
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

// 외곽선 검출 함수
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

    Out.vColor = vDiffuse * vShade + vSpecular;

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









