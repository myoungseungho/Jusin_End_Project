 
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
float g_MeshIndex;
texture2D g_DecalTexture;
texture2D g_DiffuseTexture;
texture2D g_OutLineTexture;
texture2D g_DetailTexture;
/* 모델 전체의 뼈(x), 메시에게 영향을 주는 뼈(o)*/

float4x4 g_BoneMatrices[800];

float g_fTexcoordValue;


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
    Out.vNormal = normalize(mul(vNormal, g_WorldMatrix));
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
    float4 vAuraColor : SV_TARGET3;
};


PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
	
    vector vMtrlShadeDesc = g_OutLineTexture.Sample(LinearSampler, In.vTexcoord);
//    vector vHairColor = { 255.f / 255.f, 255.f / 255.f, 130.f / 255.f, 1.f };

    vector vHairColor = { vMtrlDiffuse.rgb, 1.f };
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
    float fFaceDetailMask2 = step(0.3f, In.vTexcoord.x) * step(In.vTexcoord.x, 0.427f) * step(0.031f, In.vTexcoord.y) * step(In.vTexcoord.y, 0.158f);
    //float fOffset = 0.02f;
    //float inRangeCondition = (step(0.095, In.vTexcoord.x) * step(In.vTexcoord.x, 0.2832)) *
    //                     (step(0.0, In.vTexcoord.y) * step(In.vTexcoord.y, 0.316)); 

    //float fSkillDetailUV = (abs(In.vTexcoord.x - 0.1) < fOffset) *
    //                           (abs(In.vTexcoord.y - 0.1) < fOffset);

    //float fFaceMask = inRangeCondition + fSkillDetailUV;
    //fFaceMask = saturate(fFaceMask);

    float isFace = fFaceMask;
    /* g값은 명암? r값이랑 같이 쓰는데 모호함 */
    vResultColor.rgb = saturate(vResultColor.rgb * saturate(vMtrlShadeDesc.g /* (isFace == 1 ? 0.502745f : vMtrlShadeDesc.g)*/ * 1.5f) + (fHairMask * (vHairColor.rgb / 4)));
    
	/* b값은 보니까 스펙큘러인거같음 그 처리 */
    vResultColor.rgb = saturate(vResultColor.rgb + vMtrlShadeDesc.b * 0.1f);
    vResultColor.a = 1.f;
    Out.vDiffuse = saturate(vResultColor * 1.2f);
    Out.vNormal = vector((In.vNormal.xyz * 0.5f + 0.5f), saturate(fHairMask + fFaceMask + fFaceDetailMask2 /*+ fFaceDetailMask*/));
    Out.vDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
    Out.vAuraColor = vector(1.f, 0.f, 0.f, 0.f);
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
    Out.vAuraColor = vector(1.f, 0.f, 0.f, 0.f);
    return Out;
}
PS_OUT PS_MAIN_FRIEZA(PS_IN In)
{
    PS_OUT Out;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
	
    vector vMtrlShadeDesc = g_OutLineTexture.Sample(LinearSampler, In.vTexcoord);
    //vector vMtrlDetailDesc = g_DetailTexture.Sample(LinearSampler, In.vTexcoord);
    
//    vector vHairColor = { 255.f / 255.f, 255.f / 255.f, 130.f / 255.f, 1.f };

    vector vHairColor = { vMtrlDiffuse.rgb, 1.f };
    vector vFaceColor = { 0.98823f, 0.8156f, 0.6862f, 1.0f };
    vector vResultColor = { 0.f, 0.f, 0.f, 1.f };  
    
    /* vMtrlShadeDesc 알파값으로 아웃라인을 생성 */
    vResultColor.rgb = saturate(vHairColor.rgb - (1 - vMtrlShadeDesc.a));

    ///* 프리저 헤어 텍스쿠드 좌표 
    /*
    Texcoord
    x 0.505 0.55
    y 0.353  0.495
    */
    float fFaceMask = (step(0.151, In.vTexcoord.x) * step(In.vTexcoord.x, 0.34)) * (step(0.136, In.vTexcoord.y) * step(In.vTexcoord.y, 0.385));
    float fFaceMask2 = step(0.3, vMtrlShadeDesc.g - vMtrlShadeDesc.r);
    float fFaceMask3 = (step(0.182, In.vTexcoord.x) * step(In.vTexcoord.x, 0.491)) * (step(0.004, In.vTexcoord.y) * step(In.vTexcoord.y, 0.057));
    float fLegMask = (step(0.505f, In.vTexcoord.x) * step(In.vTexcoord.x, 0.55f)) * (step(0.353f, In.vTexcoord.y) * step(In.vTexcoord.y, 0.495));
    float fArmMask = (step(0.507f, In.vTexcoord.x) * step(In.vTexcoord.x, 0.6291f)) * (step(0.916f, In.vTexcoord.y) * step(In.vTexcoord.y, 0.955));
    /* g값은 명암? r값이랑 같이 쓰는데 모호함 */
    //vResultColor.rgb = saturate(vResultColor.rgb * saturate(vMtrlShadeDesc.g));
    
    //* b값은 보니까 스펙큘러인거같음 그 처리 */
    vResultColor.rgb = saturate(vResultColor.rgb + vMtrlShadeDesc.b * 0.1f);
    vResultColor.a = 1.f;
    Out.vDiffuse = vResultColor;
    Out.vNormal = vector((In.vNormal.xyz * 0.5f + 0.5f), saturate(fFaceMask + fFaceMask2 + fFaceMask3));
    Out.vDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, g_MeshIndex * ((1 - fLegMask) * (1 - fArmMask)), 0.f);
    Out.vAuraColor = vector(1.f, 0.f, 0.f, 0.f);
    return Out;
}

struct PS_OUT_SHADOW
{
    float4 vLightDepth : SV_TARGET0;
};


PS_OUT_SHADOW PS_MAIN_SHADOW(PS_IN In)
{
    PS_OUT_SHADOW Out;

    Out.vLightDepth = vector(In.vProjPos.w / 10000.f, 0.f, 0.f, 1.f);
    
    return Out;
}

PS_OUT PS_MAIN_LOBBY_GOKU(PS_IN In)
{
    PS_OUT Out;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    if (vMtrlDiffuse.a < 0.1f)
        discard;

    Out.vDiffuse = vMtrlDiffuse;

	/* In.vNormal.xyz -> -1 ~ 1 */
	/* Out.vNormal.xyz -> 0 ~ 1 */

    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);

    return Out;
}

PS_OUT PS_MAIN_GOKUDECAL(PS_IN In)
{
    PS_OUT Out;
    float2 Test = { 0.1f, 0.1f };
    //vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, Test);
    vector vMtrlDecal = g_DecalTexture.Sample(LinearSampler, In.vTexcoord);
    //float vAlpha = (vMtrlDiffuse.r + vMtrlDiffuse.g + vMtrlDiffuse.b) / 3.f;
    //vMtrlDiffuse.a = vAlpha * 2.f;
  //  step(vMtrlDiffuse.r, 0)
   // vMtrlDiffuse.a = saturate((1 - (vMtrlDiffuse.r - vMtrlDiffuse.r)));
    clip(0.4f - vMtrlDecal.r);
    
    //vMtrlDecal.rgb = saturate(((0.5 - 0.00588) - vMtrlDecal.r) * 2.f);
    
    
   // vMtrlDiffuse.a = 0.f;
    //float fHairMask = step(In.vTexcoord.x, 0.9f) * step(In.vTexcoord.y, 0.9f);
    //vMtrlDecal.a = fHairMask;
    Out.vDiffuse = vMtrlDecal;

    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);

    return Out;
}

PS_OUT PS_MAIN_Loading_Dragon(PS_IN In)
{
    PS_OUT Out;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);

    if (vMtrlDiffuse.a < 0.1f)
        discard;
   
    vector vMtrlShadeDesc = g_OutLineTexture.Sample(LinearSampler, In.vTexcoord);

    vector vHairColor = { vMtrlDiffuse.rgb, 1.f };
    vector vFaceColor = { 0.98823f, 0.8156f, 0.6862f, 1.0f };
    vector vResultColor = { 0.f, 0.f, 0.f, 1.f };
    
    float2 vTexcoordFraction = fmod(In.vTexcoord, 1.0);
    vTexcoordFraction = vTexcoordFraction < 0 ? vTexcoordFraction + 1.0 : vTexcoordFraction;
    
    /* vMtrlShadeDesc 알파값으로 아웃라인을 생성 */
    vResultColor.rgb = saturate(vHairColor.rgb - (1 - vMtrlShadeDesc.a));

    Out.vDiffuse = vResultColor;

    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);

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
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SHADOW();
    }

//3
    pass Lobby_Goku
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);


        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_LOBBY_GOKU();
    }


    pass Goku_Decal // 4
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);


        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_GOKUDECAL();
    }

    pass Default_Frieza // 5
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);


        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_FRIEZA();
    }

//6
    pass Loading_Dragon
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);


        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_Loading_Dragon();
    }
}















