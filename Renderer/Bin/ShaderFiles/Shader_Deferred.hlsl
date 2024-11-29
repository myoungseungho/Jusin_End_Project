
#include "Renderer_Shader_Defines.hlsli"

float4x4		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

float4x4		g_ViewMatrixInv, g_ProjMatrixInv;

float4x4		g_LightViewMatrix, g_LightProjMatrix;
int             g_iPlayerDirection;
float4			g_vCamPosition;

float4			g_vLightDir;
float4			g_vLightPos;
float			g_fLightRange;
float4			g_vLightDiffuse;
float4			g_vLightAmbient;
float4			g_vLightSpecular;
bool g_isChase;
float g_fAccBlackTime;
float g_fLightAccTime;
float g_fLightLifeTime;
bool g_isStartBlackOut;
bool g_isEndWhiteOut;
bool g_isUsingEffectLight;
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
texture2D g_MetallicTexture;
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

PS_OUT_LIGHT PS_MAIN_DIRECTIONAL_PLAYER(PS_IN In)
{
    PS_OUT_LIGHT Out = (PS_OUT_LIGHT) 0;

    vector vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexcoord);
    vector vDepthDesc = g_DepthTexture.Sample(LinearSampler, In.vTexcoord);
    float fViewZ = vDepthDesc.x * 1000.f;
    
    float4 vPlayerLightDir = g_vLightDir;
    vPlayerLightDir.x *= g_iPlayerDirection;
    
	/* 0 ~ 1 -> -1 ~ 1 */
    float4 vNormal = float4(vNormalDesc.xyz * 2.f - 1.f, 0.f);

	 // 앰비언트 조명
    float4 vAmbient = g_vLightAmbient * g_vMtrlAmbient;

    float shadeIntensity = max(dot(normalize(vPlayerLightDir) * -1.f, vNormal), 0.f);
    shadeIntensity = saturate(shadeIntensity);

    float shadeStep = 2.0f;
    shadeIntensity = (floor(shadeIntensity * shadeStep) / shadeStep);
    float4 vResultShade = ((g_vLightDiffuse * (vNormalDesc.w ? 1.f : shadeIntensity) * 1.f) + vAmbient);
    vResultShade = (g_isChase == true ? vResultShade * g_vLightDiffuse : vResultShade);

    //vNormalDesc.w 가 마스크용
    //vResultShade = (vNormalDesc.w ? 1.f : vResultShade);
    //vResultShade = (vNormalDesc.w ? g_vLightDiffuse * 2 * vAmbient : vResultShade);
    vResultShade.a = vNormalDesc.w ? 1.f : 0;
    Out.vShade = vResultShade;
    
    float4 vWorldPos;

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

    float4 vReflect = reflect(normalize(vPlayerLightDir), vNormal);
    float4 vLook = vWorldPos - g_vCamPosition;
	// 따로 죽이기위해 분리
    float specularIntensity = max(dot(normalize(vReflect) * -1.f, normalize(vLook)), 0.f);
	
	//죽이기
    float specularStep = 2.0f;
    specularIntensity = floor(specularIntensity * specularStep) / specularStep;

	// 최소값
    specularIntensity = max(specularIntensity, 0.1f);
	
    float specularThreshold = 0.8f; // 0.9 이상의 값만 스펙큘러 적용
	
    //if (specularIntensity >= specularThreshold)
    //{
    //    // 스펙큘러 색상 적용
    //    Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(specularIntensity, 1.5f);

    //}
    //else
    //{
    //    // 스펙큘러 미적용 (혹은 매우 약하게 적용)
       
    //}
    Out.vSpecular = float4(0.f, 0.f, 0.f, 1.0f); // 스펙큘러를 제거
    return Out;
}

PS_OUT_LIGHT PS_MAIN_DIRECTIONAL(PS_IN In)
{
    PS_OUT_LIGHT Out = (PS_OUT_LIGHT) 0;
	
	vector			vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexcoord);
	vector			vDepthDesc = g_DepthTexture.Sample(LinearSampler, In.vTexcoord);
	float			fViewZ = vDepthDesc.x * 1000.f;

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

    //float4 vReflect = reflect(normalize(g_vLightDir), vNormal);
    //float4 vLook = vWorldPos - g_vCamPosition;
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

PS_OUT_LIGHT PS_MAIN_POINT_PLAYER(PS_IN In)
{
    PS_OUT_LIGHT Out = (PS_OUT_LIGHT) 0;

    vector vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexcoord);
    vector vDepthDesc = g_DepthTexture.Sample(LinearSampler, In.vTexcoord);
    float fViewZ = vDepthDesc.x * 1000.f;
    
    float4 vPlayerLightDir = g_vLightDir;
    vPlayerLightDir.x *= g_iPlayerDirection;
    
	/* 0 ~ 1 -> -1 ~ 1 */
    float4 vNormal = float4(vNormalDesc.xyz * 2.f - 1.f, 0.f);

	 // 앰비언트 조명
    float4 vAmbient = g_vLightAmbient * g_vMtrlAmbient;

    float shadeIntensity = max(dot(normalize(vPlayerLightDir) * -1.f, vNormal), 0.f);
    shadeIntensity = saturate(shadeIntensity);

    float shadeStep = 2.0f;
    shadeIntensity = floor(shadeIntensity * shadeStep) / shadeStep;

    float4 vResultShade = ((g_vLightDiffuse * shadeIntensity * 1.5f) + vAmbient);
    vResultShade = saturate(vNormalDesc.w ? g_vLightDiffuse * 0.8f : vResultShade);
    Out.vShade = vResultShade;
    //Out.vShade = (g_vLightDiffuse * shadeIntensity * 1.f) + vAmbient;
    

    float4 vWorldPos;
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
    float4 vReflect = reflect(normalize(vPlayerLightDir), vNormal);
    float4 vLook = vWorldPos - g_vCamPosition;
    Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(max(dot(normalize(vReflect) * -1.f, normalize(vLook)), 0.f), 30.f);
    
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
    clip(vDiffuse.a - 0.98f);

    vector vShade = g_ShadeTexture.Sample(LinearSampler, In.vTexcoord);
    //clip(vShade.a - 0.98f);
    vector vSpecular = g_SpecularTexture.Sample(LinearSampler, In.vTexcoord);

    //Out.vColor = vDiffuse * (g_isUsingEffectLight ? 1.0 : vShade) + vSpecular;
    Out.vColor = vDiffuse * vShade + vSpecular;

    vector vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexcoord);
    vector vDepthDesc = g_DepthTexture.Sample(LinearSampler, In.vTexcoord);
    vector vMetallicDesc = g_MetallicTexture.Sample(LinearSampler, In.vTexcoord);

    
    float fViewZ = vDepthDesc.x * 1000.f;
    float4 vNormal = float4(vNormalDesc.xyz * 2.f - 1.f, 0.f);

    float fEdgeNormalThreshold = 0.2f;
    float fEdgeDepthThreshold = 0.1f;
	
    float fEdge = CalculateEdge(In.vTexcoord, fViewZ, vNormal, 0.f, fEdgeNormalThreshold, fEdgeDepthThreshold);

    vector vOutlineBlack = float4(0.f, 0.f, 0.f, 1.f);
    Out.vColor = lerp(Out.vColor, vOutlineBlack, fEdge);
   
    /* 프리저용 */
    vector vMaskColor = { 0.f, 0.f, 0.f, 1.f };
    
    bool bLight = step(0.5f, vMetallicDesc.b * vMetallicDesc.a);
    bool bMid = step(0.4f, vMetallicDesc.r * vMetallicDesc.a) * (1 - bLight);
    bool bDark = step(0.9f, vMetallicDesc.a - vMetallicDesc.r) * step(0.39f, vDepthDesc.b);
       
    float fMaskFactor = step(0.39f, vDepthDesc.b) * ((0.6f * bLight) + (0.2f * bMid) + (-0.5f * bDark));
    
    /* 머리 메쉬지만 렉트 범위 밖인 애들 검출*/
    float fIn_RectOut = step(0.5f,
    /* 하나라도 렉트 범위 안에든 머리메쉬              머리메쉬인데 * 정말 렉트 범위 안에 안들었는지 */
    (1 - step(0.5f, bLight + bMid + bDark)) + step(0.39f, vDepthDesc.b * (1 - step(0.5f, bLight + bMid + bDark))));    
    
    /* 렉트 범위 밖인 머리메쉬가 가장 외곽 색영향을 안받기 때문에 색을 일단 저장 */
    vMaskColor.rgb = (Out.vColor.rgb * 0.6f) * fIn_RectOut;
    
    /* 기본적인 팩터 즉 메탈 텍스쳐에 적용된 팩터를 지정한메쉬로 거르기 */
    Out.vColor.rgb = (Out.vColor.rgb + (Out.vColor.rgb * fMaskFactor)) + (vMaskColor.rgb * step(0.39f, vDepthDesc.b));

    return Out;
}

PS_OUT PS_MAIN_DEFERRED_PART(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    clip(vDiffuse.a - 0.98f);

    vector vShade = g_ShadeTexture.Sample(LinearSampler, In.vTexcoord);
    //clip(vShade.a - 0.98f);
    vector vSpecular = g_SpecularTexture.Sample(LinearSampler, In.vTexcoord);

    //Out.vColor = vDiffuse * (g_isUsingEffectLight ? 1.0 : vShade) + vSpecular;
    Out.vColor = vDiffuse * vShade + vSpecular;

    vector vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexcoord);
    vector vDepthDesc = g_DepthTexture.Sample(LinearSampler, In.vTexcoord);
    vector vMetallicDesc = g_MetallicTexture.Sample(LinearSampler, In.vTexcoord);

    
    float fViewZ = vDepthDesc.x * 1000.f;
    float4 vNormal = float4(vNormalDesc.xyz * 2.f - 1.f, 0.f);

    float fEdgeNormalThreshold = 0.2f;
    float fEdgeDepthThreshold = 0.1f;
	
    float fEdge = CalculateEdge(In.vTexcoord, fViewZ, vNormal, 0.f, fEdgeNormalThreshold, fEdgeDepthThreshold);

    vector vOutlineBlack = float4(0.f, 0.f, 0.f, 1.f);
    Out.vColor = lerp(Out.vColor, vOutlineBlack, fEdge);
   
    
        /* 마젠타 색상 */
    
    bool bLight = step(0.5, vMetallicDesc.b * vMetallicDesc.a);
    bool bMid = step(0.4, vMetallicDesc.r * vMetallicDesc.a) * (1 - bLight);
    bool bDark = step(0.9, vMetallicDesc.a - vMetallicDesc.r);
// 기본            
    Out.vColor.rgb = (Out.vColor.rgb * (1.6f * bLight)) + (Out.vColor.rgb * (0.8f * bMid)) + (Out.vColor.rgb * (0.4f * bDark));
    return Out;
}

PS_OUT PS_MAIN_BLACKOUT(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vDiffuse = g_Texture.Sample(LinearSampler, In.vTexcoord);
    
    vDiffuse.a = saturate(vDiffuse.a - (0.8f - g_fAccBlackTime));
    
    Out.vColor = vDiffuse;
    return Out;
}
PS_OUT PS_MAIN_WHITEOUT(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    float2 vTexcoord = In.vTexcoord;
    
    vector vRockDiffuse;
    float2 texFramePos = g_fSpriteCurPos * g_fSpriteSize;
    vTexcoord = vTexcoord * g_fSpriteSize + texFramePos;
    
    vRockDiffuse.rgb = float3(0.035294f, 0.0078431f, 0.00392156f);
    vRockDiffuse.a = g_DiffuseTexture.Sample(LinearSampler, vTexcoord).a;
    
    vector vDiffuse = g_Texture.Sample(LinearSampler, In.vTexcoord);
    
    vDiffuse.a = saturate(vDiffuse.a - (0.5f - g_fAccBlackTime));
    Out.vColor = vDiffuse;
    if (g_isEndWhiteOut == false)
    {
    
        vRockDiffuse.rgb = vRockDiffuse.rgb + (vDiffuse.rgb * saturate(g_fAccBlackTime - 0.5f));
    
        vDiffuse.rgb = vDiffuse.rgb * (1.f - vRockDiffuse.a) + vRockDiffuse.rgb * vRockDiffuse.a;
        vDiffuse.a = vDiffuse.a * (1.f - vRockDiffuse.a) + vRockDiffuse.a;
        Out.vColor = vDiffuse * g_isStartBlackOut + (vRockDiffuse * (1.f - g_isStartBlackOut));
    }

    return Out;
}

PS_OUT PS_MAIN_EFFECT_OUTLINE(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    if (0.0f == vDiffuse.a)
        discard;

    Out.vColor = vDiffuse;

    vector vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexcoord);
    vector vDepthDesc = g_DepthTexture.Sample(LinearSampler, In.vTexcoord);
   
    float fViewZ = vDepthDesc.x * 1000.f;
    float4 vNormal = float4(vNormalDesc.xyz * 2.f - 1.f, 0.f);

    float fEdgeNormalThreshold = 0.2f;
    float fEdgeDepthThreshold = 0.7f;
	
    float fEdge = CalculateEdge(In.vTexcoord, fViewZ, vNormal, 0.f, fEdgeNormalThreshold, fEdgeDepthThreshold);

    vector vOutlineBlack = float4(0.f, 0.f, 0.f, 1.f);
    Out.vColor = lerp(Out.vColor, vOutlineBlack, fEdge);
   
    return Out;
}
PS_OUT PS_MAIN_DEFERRED_STAGE(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vDepthDesc = g_DepthTexture.Sample(LinearSampler, In.vTexcoord);
    float fViewZ = vDepthDesc.x * 10000.f;

    float4 vWorldPos;
    
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

    vWorldPos = mul(vWorldPos, g_LightViewMatrix);
    vWorldPos = mul(vWorldPos, g_LightProjMatrix);


	/* 투영공간상의 좌표로 변환하낟. */
    float2 vTexcoord = vWorldPos.xy / vWorldPos.w;

    vTexcoord.x = saturate(vTexcoord.x * 0.5f + 0.5f);
    vTexcoord.y = saturate(vTexcoord.y * -0.5f + 0.5f);

    vector vOldLightDepth = g_LightDepthTexture.Sample(LinearSampler, vTexcoord);
	
    float fDepth = vWorldPos.w;

    if (vOldLightDepth.x * 10000.f < fDepth - 0.3f)
        Out.vColor = vector(0.f, 0.f, 0.f, 0.4f);
    
    return Out;
}

PS_OUT PS_MAIN_DEFERRED_MAP(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    
    clip(vDiffuse.a - 0.01f);

    vector vShade = g_ShadeTexture.Sample(LinearSampler, In.vTexcoord);
    vector vSpecular = g_SpecularTexture.Sample(LinearSampler, In.vTexcoord);

    Out.vColor = vDiffuse * vShade + vSpecular;
	
    vector vDepthDesc = g_DepthTexture.Sample(LinearSampler, In.vTexcoord);
    float fViewZ = vDepthDesc.x * 10000.f;

    float4 vWorldPos;
    
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

    vWorldPos = mul(vWorldPos, g_LightViewMatrix);
    vWorldPos = mul(vWorldPos, g_LightProjMatrix);


	/* 투영공간상의 좌표로 변환하낟. */
    float2 vTexcoord = vWorldPos.xy / vWorldPos.w;

    vTexcoord.x = saturate(vTexcoord.x * 0.5f + 0.5f);
    vTexcoord.y = saturate(vTexcoord.y * -0.5f + 0.5f);

    vector vOldLightDepth = g_LightDepthTexture.Sample(LinearSampler, vTexcoord);
	
    float fDepth = vWorldPos.w;

    if (vOldLightDepth.x * 10000.f < fDepth - 0.3f)
        Out.vColor = Out.vColor * 0.5f;
    
    return Out;
}





PS_OUT PS_MAIN_ALLBLACKOUT(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vDiffuse = g_Texture.Sample(LinearSampler, In.vTexcoord);
    
    vDiffuse.a = saturate(vDiffuse.a - (1.f - saturate(g_fAccBlackTime)));
    
    Out.vColor = vDiffuse;
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

	pass Deferred // 3
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

    pass Deferred_MapObj //4
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN(); 
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DEFERRED_MAP();
    }

    pass Player_Light_Directional //5
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DIRECTIONAL_PLAYER();
    }

    pass Player_Light_Point //6
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_MultiplyBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_POINT_PLAYER();
    }

    pass Effect_OutLine //7
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
//BS_MultiplyBlend 이거 흑백 새상일때 할것
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_EFFECT_OUTLINE();
    }

    pass BlackOut // 8
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLACKOUT();
    }

    pass Deferred_Part // 9
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DEFERRED_PART();
    }

    pass WhiteOut // 10
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_WHITEOUT();
    }

    pass Player_Light_Directional_Chase //11
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_MultiplyBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DIRECTIONAL_PLAYER();
    }
    pass StageDefferd //12
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DEFERRED_STAGE();
    }

    pass AllBlackOut // 13
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_ALLBLACKOUT();
    }
}









