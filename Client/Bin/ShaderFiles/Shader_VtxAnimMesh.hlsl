 
#include "Renderer_Shader_Defines.hlsli"

float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

float4 g_vLightDir = float4(1.f, -1.f, 1.f, 0.f);
float4 g_vLightDiffuse = float4(1.f, 1.f, 1.f, 1.f);
float4 g_vLightAmbient = float4(1.f, 1.f, 1.f, 1.f);
float4 g_vLightSpecular = float4(1.f, 1.f, 1.f, 1.f);

float4 g_vMtrlAmbient = float4(0.3f, 0.3f, 0.3f, 1.f);
float4 g_vMtrlSpecular = float4(1.f, 1.f, 1.f, 1.f);

vector g_vCamPosition;
int g_iPlayerDirection;
texture2D g_DiffuseTexture;
texture2D g_OutLineTexture;
/* 모델 전체의 뼈(x), 메시에게 영향을 주는 뼈(o)*/
float4x4 g_BoneMatrices[800];


struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float3 vTangent : TANGENT;

	/* 이 정점에게 적용되어야할 네개 뼈의 인덱스들 */
    uint4 vBlendIndex : BLENDINDEX;

	/* 위에서 이야기한 뼈가 각각 정점에게 몇 퍼센트나 영향(가중치 0 ~ 1)을 주는지 */
    float4 vBlendWeight : BLENDWEIGHT;
	
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;

    float fWeightW = 1.f - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);

    matrix BoneMatrix = g_BoneMatrices[In.vBlendIndex.x] * In.vBlendWeight.x +
		g_BoneMatrices[In.vBlendIndex.y] * In.vBlendWeight.y +
		g_BoneMatrices[In.vBlendIndex.z] * In.vBlendWeight.z +
		g_BoneMatrices[In.vBlendIndex.w] * fWeightW;

    vector vPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);
    vector vNormal = mul(vector(In.vNormal, 0.f), BoneMatrix);

	/* mul : 곱하기가 가능한 모든 행렬(좌변의 열, 우변의 행 같다면)에 대해서 다 곱하기를 수행해준다. */
    vPosition = mul(vPosition, g_WorldMatrix);
    vPosition = mul(vPosition, g_ViewMatrix);
    vPosition = mul(vPosition, g_ProjMatrix);
    float4x4 TestMatrix = g_WorldMatrix;
    
    //TestMatrix._11 = 1; /* 노말 던질때 행렬 역방향 강제 1 로 */
    Out.vPosition = vPosition;
    Out.vNormal = normalize(mul(vNormal, TestMatrix));
    Out.vTexcoord = In.vTexcoord;
    Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
    Out.vProjPos = vPosition;

    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};

struct PS_OUT
{
    float4 vDiffuse : SV_TARGET0;
    float4 vNormal : SV_TARGET1;
    float4 vDepth : SV_TARGET2;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
	
    vector vMtrlShadeDesc = g_OutLineTexture.Sample(LinearSampler, In.vTexcoord);
//    vector vHairColor = { 255.f / 255.f, 255.f / 255.f, 130.f / 255.f, 1.f };
    vector vHairColor = { vMtrlDiffuse.rgb * 0.9f, 1.f };
    vector vFaceColor = { 0.98823f, 0.8156f, 0.6862f, 1.0f };
    vector vResultColor = { 0.f, 0.f, 0.f, 1.f };
    
    float2 vTexcoordFraction = fmod(In.vTexcoord, 1.0);
    vTexcoordFraction = vTexcoordFraction < 0 ? vTexcoordFraction + 1.0 : vTexcoordFraction;
    
    /* vMtrlShadeDesc 알파값으로 아웃라인을 생성 */
    vResultColor.rgb = saturate(vHairColor.rgb - (1 - vMtrlShadeDesc.a));
    
    /* 손오공 헤어 텍스쿠드 좌표 */
    float fHairMask = step(0.486f, vTexcoordFraction.x) * step(vTexcoordFraction.y, 0.287f);

    float fFaceMask = (step(0.095, In.vTexcoord.x) * step(In.vTexcoord.x, 0.2832)) * (step(0.0, In.vTexcoord.y) * step(In.vTexcoord.y, 0.316));
    float fFaceDetailMask = (step(0.013, In.vTexcoord.x) * step(In.vTexcoord.x, 0.016)) * (step(0.015, In.vTexcoord.y) * step(In.vTexcoord.y, 0.017));
    
    //float fOffset = 0.02f;
    //float inRangeCondition = (step(0.095, In.vTexcoord.x) * step(In.vTexcoord.x, 0.2832)) *
    //                     (step(0.0, In.vTexcoord.y) * step(In.vTexcoord.y, 0.316)); 

    //float fSkillDetailUV = (abs(In.vTexcoord.x - 0.1) < fOffset) *
    //                           (abs(In.vTexcoord.y - 0.1) < fOffset);

    //float fFaceMask = inRangeCondition + fSkillDetailUV;
    //fFaceMask = saturate(fFaceMask);

    
    
    /* g값은 명암? r값이랑 같이 쓰는데 모호함 */
    vResultColor.rgb = saturate(vResultColor.rgb * saturate(vMtrlShadeDesc.r + vMtrlShadeDesc.g * 1.5f) + (fHairMask * (vHairColor.rgb / 4)));
	
	/* b값은 보니까 스펙큘러인거같음 그 처리 */
    vResultColor.rgb = saturate(vResultColor.rgb + vMtrlShadeDesc.b * 0.1f);

    Out.vDiffuse = vResultColor;
    Out.vNormal = vector((In.vNormal.xyz * 0.5f + 0.5f), saturate(fHairMask + fFaceMask + fFaceDetailMask));
    Out.vDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
    
    //float2 vTexcoordFloor = In.vTexcoord - vTexcoordFraction;
    //float vResultFloor = (vTexcoordFloor.x + vTexcoordFloor.y) * 0.1f;
    //vHairColor.rgb = vHairColor.rgb * (1 - vResultFloor);

 //   float3 vShadeColor = ((vHairColor.rgb * vTexcoordFloor.x) * ((1.f - saturate(vMtrlShadeDesc.r + vMtrlShadeDesc.g)) * fHairMask));
 //   vResultColor.rgb = saturate(vResultColor.rgb * saturate(vShadeColor + saturate(vMtrlShadeDesc.r + vMtrlShadeDesc.g)));

    //float3 vShadeColor = (vHairColor.rgb * ((1 - saturate(vMtrlShadeDesc.r + vMtrlShadeDesc.g)) * fHairMask));
    //vResultColor.rgb = saturate(vResultColor.rgb * saturate(vShadeColor + saturate(vMtrlShadeDesc.r + vMtrlShadeDesc.g)));

    return Out;
}

PS_OUT PS_MAIN_21(PS_IN In)
{
    PS_OUT Out;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
	
    vector vMtrlShadeDesc = g_OutLineTexture.Sample(LinearSampler, In.vTexcoord);

    vector vResultColor = { 0.f, 0.f, 0.f, 1.f };
    vector vHairColor = { 195.f / 255.f, 119.f / 255.f, 183.f / 255.f, 1.f };
    
    float4 vPlayerLightDir = g_vLightDir;
    vPlayerLightDir.x *= g_iPlayerDirection;
    
	/* vMtrlShadeDesc 에 알파값으로 일단 아웃라인을 생성 */
    vResultColor.rgb = saturate(vMtrlDiffuse.rgb - (1 - vMtrlShadeDesc.a));
    
	/* g값은 명암? r값이랑 같이 쓰는데 모호함 */
    // In.vTexcoord.x, In.vTexcoord.y 머리카락 분할 해야될듯 if문?

    //vResultColor.rgb = saturate(vResultColor.rgb * (saturate(vMtrlShadeDesc.r + vMtrlShadeDesc.g)));
	
    //vResultColor.rgb = saturate(vResultColor.rgb * (vHairColor.rgb * ((saturate(vMtrlShadeDesc.r + vMtrlShadeDesc.g)))));
    	///* 0.0f ~ 1.f */
 //   float fShade = max(dot(normalize(g_vLightDir) * -1.f, normalize(In.vNormal)), 0.f);

	///* 0.3f ~ 1.f */
 //   vector vShade = saturate(fShade + g_vLightAmbient * g_vMtrlAmbient);
 //   float shadeIntensity = max(dot(normalize(vPlayerLightDir) * -1.f, In.vNormal), 0.f);
 //   shadeIntensity = saturate(shadeIntensity);
     
 //   float shadeStep = 2.0f;
 //   shadeIntensity = floor(shadeIntensity * shadeStep) / shadeStep;

 //   vShade = (g_vLightDiffuse * shadeIntensity * 1.f) + vShade;
    
 //   vector vReflect = reflect(normalize(g_vLightDir), normalize(In.vNormal));
 //   vector vLook = In.vWorldPos - g_vCamPosition;

 //   float fSpecular = pow(max(dot(normalize(vReflect) * -1.f, normalize(vLook)), 0.f), 30.f);
 //   vector vSpecular = g_vLightSpecular * g_vMtrlSpecular * fSpecular;


  //  Out.vDiffuse = (g_vLightDiffuse * vResultColor) * vShade;
    float fPlayerDir = saturate(g_iPlayerDirection);
    
    float fHairMask = step(0.5f, In.vTexcoord.x) * step(In.vTexcoord.y, 0.5f);
    float3 vShadeColor = (vHairColor.rgb * ((1.f - saturate(vMtrlShadeDesc.r + vMtrlShadeDesc.g)) * fHairMask));
    vResultColor.rgb = saturate(vResultColor.rgb * saturate(vShadeColor + saturate(vMtrlShadeDesc.r + vMtrlShadeDesc.g)));

	/* b값은 보니까 스펙큘러인거같음 그 처리 */
    vResultColor.rgb = saturate(vResultColor.rgb + vResultColor.rgb * (vMtrlShadeDesc.b /** 0.3f*/));
	
    Out.vDiffuse = vResultColor;
    Out.vNormal = vector((In.vNormal.xyz * 0.5f + 0.5f), fHairMask);
    Out.vDepth = vector((In.vProjPos.w / 1000.f), In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);

    return Out;
}

struct PS_OUT_SHADOW
{
    float4 vLightDepth : SV_TARGET0;
};


PS_OUT_SHADOW PS_MAIN_SHADOW(PS_IN In)
{
    PS_OUT_SHADOW Out;

    Out.vLightDepth = vector(In.vProjPos.w / 1000.f, 0.f, 0.f, 0.f);

    return Out;
}

technique11 DefaultTechnique
{
    pass Default_Goku
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);


        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass Default_21
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);


        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_21();
    }

    pass Shadow
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);


        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SHADOW();
    }

	
}















