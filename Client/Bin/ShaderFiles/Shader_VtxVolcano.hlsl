#include "Renderer_Shader_Defines.hlsli"

float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D g_DiffuseTexture;
texture2D g_MaskTexture;
texture2D g_TMaskTexture;

int g_SunMeshIndex;
int g_GroundCount;

float g_Time = 0.f;
texture2D g_EastGlowTexture;

vector g_vCamPosition;
float3 g_EastColor;
int g_LavaFallIndex;
float2 g_fSpriteCurPos;
float4 g_vCamPos;
float4 g_vDestructivePos;
vector TexScalar_ToSampling(float2 vScale, float2 vScroll, float2 vTexcoord);
vector ColorLerpScalarToDiffuse(vector vLerpParam, vector vDiffuseSrc, vector vDiffuseDest);

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

VS_OUT VS_MAIN_DESTRUCTIVE(VS_IN In)
{
    VS_OUT Out;

	/* mul : 곱하기가 가능한 모든 행렬(좌변의 열, 우변의 행 같다면)에 대해서 다 곱하기를 수행해준다. */
    float4x4 WorldMatrix = g_WorldMatrix;
    WorldMatrix._41_42_43_44 = g_vDestructivePos;
    
    vector vPosition = mul(vector(In.vPosition, 1.f), WorldMatrix);
    vPosition = mul(vPosition, g_ViewMatrix);
    vPosition = mul(vPosition, g_ProjMatrix);

	/* 투영행렬까지 곱한 위치벡터 */
	/* = x : fov적용 */
	/* = y : fov적용 */
	/* = z : 0 ~ f */
	/* = w : n ~ f */

    Out.vPosition = vPosition;
    Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), WorldMatrix));
    Out.vTexcoord = In.vTexcoord;
    Out.vWorldPos = mul(vector(In.vPosition, 1.f), WorldMatrix);
    Out.vProjPos = vPosition;
    Out.vTangent = normalize(mul(vector(In.vTangent, 0.f), WorldMatrix));
    Out.vBinormal = normalize(cross(Out.vNormal, Out.vTangent));

    return Out;
}

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
    VS_OUT Out;

   /* mul : 곱하기가 가능한 모든 행렬(좌변의 열, 우변의 행 같다면)에 대해서 다 곱하기를 수행해준다. */
    vector vPosition = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
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
    float4 vDiffuse : SV_TARGET0;
    float4 vNormal : SV_TARGET1;
    float4 vDepth : SV_TARGET2;
   //float4   vPickDepth : SV_TARGET3;
};
PS_OUT PS_MAIN_EAST(PS_IN In)
{
    PS_OUT Out;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
   // vector vMtrlAlpha = g_AlphaTexture.Sample(LinearSampler, In.vTexcoord);
    //if (vMtrlDiffuse.a < 0.99f)
    //    discard;

    Out.vDiffuse = vMtrlDiffuse;

    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
    return Out;
}
struct PS_OUT_SHADOW
{
    float4 vLightDepth : SV_TARGET0;
};


PS_OUT_SHADOW PS_MAIN_SHADOW(PS_IN In)
{
    PS_OUT_SHADOW Out;

    Out.vLightDepth = vector(In.vProjPos.w / 10000.f, In.vProjPos.z / In.vProjPos.w, 0.f, 1.f);
    
    return Out;
}

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
   // vector vMtrlAlpha = g_AlphaTexture.Sample(LinearSampler, In.vTexcoord);
    //if (vMtrlDiffuse.a < 0.99f)
    //    discard;
   
    vector vLavaColor = { 255.f / 255.f, 68.f / 255.f, 0.f / 255.f, 1.f };
    //vector vLavaColor = { 234.f / 255.f, 0.5f / 255.f, 0.f / 255.f, 1.f };
    //vLavaColor *= 0.7f;
    vector vResultColor;
    vResultColor.rgb = vMtrlDiffuse.rgb + vLavaColor.rgb * (1 - vMtrlDiffuse.a);
    vResultColor.a = 1.f;
    //vMtrlDiffuse.a
    Out.vDiffuse = vResultColor;
    
    Out.vNormal = vector(0, 0, 0, 0.f);
    Out.vDepth = vector(In.vProjPos.w / 10000.f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
    return Out;
}

PS_OUT PS_MAIN_ISLAND(PS_IN In)
{
    PS_OUT Out;
    
    Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    Out.vDiffuse.rgb *= float3(0.66f, 0.32247f, 0.0f);
    Out.vNormal = vector(Out.vDiffuse.rgb * Out.vDiffuse.a, 0.f);
    Out.vDiffuse.a *= 0.509f;
    Out.vDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
    return Out;
}

PS_OUT PS_MAIN_ISLAND2(PS_IN In)
{
    PS_OUT Out;

    Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    Out.vDiffuse.rgb *= float3(0.75f, 0.288649f, 0.0f);
    Out.vNormal = vector(Out.vDiffuse.rgb * Out.vDiffuse.a, 0.f);
    Out.vDiffuse.a *= 0.5f;
    Out.vDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
    return Out;
}

PS_OUT PS_MAIN_LAVAPOOL(PS_IN In)
{
    PS_OUT Out;
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord * float2(12.f, 14.f));
    vMtrlDiffuse.rgb = saturate(vMtrlDiffuse.rgb * float3(2.7f, 0.3f, 0.f));
    
    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = vector(Out.vDiffuse.rgb, 0.f);
    Out.vDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
    return Out;
}

PS_OUT PS_MAIN_LAVAGROUND(PS_IN In)
{
    PS_OUT Out;
    float2 vTexcoord = In.vTexcoord * float2(0.5f, 0.5f);
    vTexcoord.y -= g_Time * 0.03f;
    
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, vTexcoord);
    vMtrlDiffuse.rgb = saturate(vMtrlDiffuse.rgb * float3(2.7f, 0.3f, 0.f));
    
    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = vector(Out.vDiffuse.rgb, 0.f);
    Out.vDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
    return Out;
}

PS_OUT PS_MAIN_SKY(PS_IN In)
{
    PS_OUT Out;
    
    float2 vTexcoord = In.vTexcoord * 2.f;
    vTexcoord.y += g_Time * 0.03f;
        /* 마스크 x */
    float fSideMask = (step(-0.79f, In.vTexcoord.x) * step(In.vTexcoord.x, 1.924f)
                            * step(-0.821f, In.vTexcoord.y) * step(In.vTexcoord.y, 1.843f));
    //Tex.x < 0.039 || Tex.x > 0.961 --- 0.074 > Tex.y || 0.932 > Tex.y
    //170 34 68
    vector vSideColor = { 0.6666f, 0.1333f, 0.2666f, 1.f };
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, vTexcoord);
    vector vMtrlMask = g_MaskTexture.Sample(LinearSampler, In.vTexcoord);
    Out.vDiffuse = (vMtrlDiffuse * fSideMask) + (vSideColor * (1 - fSideMask));
    Out.vNormal = vector(0.f, 0.f, 0.f, 0.f);
    Out.vDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
    return Out;
}

PS_OUT PS_MAIN_LAVAFALL1(PS_IN In)
{
    PS_OUT Out;
    
    vector vMtrlDiffuseSrc = TexScalar_ToSampling(float2(7.f, 1.5f), float2(0.01f, -0.05f), In.vTexcoord);
    vector vMtrlDiffuseDest = TexScalar_ToSampling(float2(10.f, 2.f), float2(-0.01f, -0.01f), In.vTexcoord);
    
    vMtrlDiffuseSrc.rgb *= float3(0.27f, 0.137464f, 0.068872f);
    
    vector vLerpParam = vector(0.6f, 0.125939f, 0.0f, 0.399f);
    Out.vDiffuse = ColorLerpScalarToDiffuse(vLerpParam, vMtrlDiffuseSrc, vMtrlDiffuseDest);

    Out.vNormal = vector(Out.vDiffuse.rgb, 0.f);
    Out.vDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
    return Out;
}

PS_OUT PS_MAIN_LAVAFALL2(PS_IN In)
{
    PS_OUT Out;

    vector vMtrlDiffuseSrc = TexScalar_ToSampling(float2(7.f, 1.5f), float2(0.01f, -0.05f), In.vTexcoord);
    vector vMtrlDiffuseDest = TexScalar_ToSampling(float2(10.f, 2.f), float2(-0.01f, -0.01f), In.vTexcoord);
    
    vMtrlDiffuseSrc.rgb *= float3(0.53f, 0.269837f, 0.135193f);
    
    vector vLerpParam = vector(0.6f, 0.125939f, 0.0f, 0.399f);
    Out.vDiffuse = ColorLerpScalarToDiffuse(vLerpParam, vMtrlDiffuseSrc, vMtrlDiffuseDest);

    Out.vNormal = vector(Out.vDiffuse.rgb, 0.f);
    Out.vDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
    return Out;
}

PS_OUT PS_MAIN_LAVAFALL3(PS_IN In)
{
    PS_OUT Out;

    vector vMtrlDiffuseSrc = TexScalar_ToSampling(float2(7.f, 1.5f), float2(0.01f, -0.05f), In.vTexcoord);
    vector vMtrlDiffuseDest = TexScalar_ToSampling(float2(10.f, 2.f), float2(-0.01f, -0.01f), In.vTexcoord);
    
    vMtrlDiffuseSrc.rgb *= float3(0.27f, 0.137464f, 0.068872f);
    
    vector vLerpParam = vector(0.875f, 0.084846f, 0.015132f, 0.518f);
    Out.vDiffuse = ColorLerpScalarToDiffuse(vLerpParam, vMtrlDiffuseSrc, vMtrlDiffuseDest);

    Out.vNormal = vector(Out.vDiffuse.rgb, 0.f);
    Out.vDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
    return Out;
}

PS_OUT PS_MAIN_LAVAFALL4(PS_IN In)
{
    PS_OUT Out;

    vector vMtrlDiffuseSrc = TexScalar_ToSampling(float2(7.f, 1.5f), float2(0.01f, -0.05f), In.vTexcoord);
    vector vMtrlDiffuseDest = TexScalar_ToSampling(float2(10.f, 2.f), float2(-0.01f, -0.01f), In.vTexcoord);
    
    vMtrlDiffuseSrc.rgb *= float3(0.27f, 0.137464f, 0.068872f);
    
    vector vLerpParam = vector(0.6f, 0.196243f, 0.086535f, 0.487f);
    Out.vDiffuse = ColorLerpScalarToDiffuse(vLerpParam, vMtrlDiffuseSrc, vMtrlDiffuseDest);

    Out.vNormal = vector(Out.vDiffuse.rgb, 0.f);
    Out.vDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
    return Out;
}

PS_OUT PS_MAIN_LAVAFALL5(PS_IN In)
{
    PS_OUT Out;

    vector vMtrlDiffuseSrc = TexScalar_ToSampling(float2(7.f, 1.5f), float2(0.01f, -0.05f), In.vTexcoord);
    vector vMtrlDiffuseDest = TexScalar_ToSampling(float2(10.f, 2.f), float2(-0.01f, -0.01f), In.vTexcoord);
    
    vMtrlDiffuseSrc.rgb *= float3(0.27f, 0.137464f, 0.068872f);
    
    vector vLerpParam = vector(0.66f, 0.126768f, 0.f, 0.685f);
    Out.vDiffuse = ColorLerpScalarToDiffuse(vLerpParam, vMtrlDiffuseSrc, vMtrlDiffuseDest);

    Out.vNormal = vector(Out.vDiffuse.rgb, 0.f);
    Out.vDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
    return Out;
}

PS_OUT PS_MAIN_LAVAFALL6(PS_IN In)
{
    PS_OUT Out;
    
    vector vMtrlDiffuseSrc = TexScalar_ToSampling(float2(15.f, 8.5f), float2(0.01f, -0.05f), In.vTexcoord);
    vector vMtrlDiffuseDest = TexScalar_ToSampling(float2(12.f, 7.f), float2(-0.01f, -0.01f), In.vTexcoord);
    
    vMtrlDiffuseSrc.rgb *= float3(0.27f, 0.137464f, 0.068872f);
    
    vector vLerpParam = vector(0.66f, 0.126768f, 0.f, 0.685f);
    Out.vDiffuse = ColorLerpScalarToDiffuse(vLerpParam, vMtrlDiffuseSrc, vMtrlDiffuseDest);


    Out.vNormal = vector(Out.vDiffuse.rgb, 0.f);
    Out.vDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
    return Out;
}

PS_OUT PS_MAIN_LAVAFALL7(PS_IN In)
{
    PS_OUT Out;
    
    vector vMtrlDiffuseSrc = TexScalar_ToSampling(float2(7.f, 1.5f), float2(0.01f, -0.05f), In.vTexcoord);
    vector vMtrlDiffuseDest = TexScalar_ToSampling(float2(10.f, 2.f), float2(-0.01f, -0.01f), In.vTexcoord);
    
    vMtrlDiffuseSrc.rgb *= float3(0.27f, 0.137464f, 0.068872f);
    
    vector vLerpParam = vector(0.66f, 0.126768f, 0.f, 0.685f);
    Out.vDiffuse = ColorLerpScalarToDiffuse(vLerpParam, vMtrlDiffuseSrc, vMtrlDiffuseDest);

    Out.vNormal = vector(Out.vDiffuse.rgb, 0.f);
    Out.vDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
    return Out;
}

PS_OUT PS_MAIN_CLIFF1(PS_IN In)
{
    PS_OUT Out;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    vector vLavaColor = { 1.44f, 0.422265f, 0.131311f, 1.f };
    vector vResultColor;
    
    vResultColor.rgb = vMtrlDiffuse.rgb + vLavaColor.rgb * (1 - vMtrlDiffuse.a);
    vResultColor.a = 1.f;
    
    Out.vDiffuse = vResultColor;
    Out.vNormal = vector(vLavaColor.rgb * (1 - vMtrlDiffuse.a), 0.f);
    return Out;
}

PS_OUT PS_MAIN_CLIFF2(PS_IN In)
{
    PS_OUT Out;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    vector vLavaColor = { 1.44f, 0.422265f, 0.131311f, 1.f };
    vector vResultColor;
    
    vResultColor.rgb = vMtrlDiffuse.rgb + vLavaColor.rgb * (1 - vMtrlDiffuse.a);
    vResultColor.a = 1.f;
    
    Out.vDiffuse = vResultColor;
    Out.vNormal = vector(vLavaColor.rgb * (1 - vMtrlDiffuse.a), 0.f);
    return Out;
}

PS_OUT PS_MAIN_CLIFF3(PS_IN In)
{
    PS_OUT Out;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    vector vLavaColor = { 1.6f, 0.3f, 0.02121901f, 1.f };
    vector vResultColor;
    
    vResultColor.rgb = vMtrlDiffuse.rgb + vLavaColor.rgb * (1 - vMtrlDiffuse.a);
    vResultColor.a = 1.f;
    
    Out.vDiffuse = vResultColor;
    Out.vNormal = vector(vLavaColor.rgb * (1 - vMtrlDiffuse.a), 0.f);
    return Out;
}

PS_OUT PS_MAIN_CLIFF4(PS_IN In)
{
    PS_OUT Out;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    vector vLavaColor = { 1.5f, 0.2564f, 0.021219f, 1.f };
    vector vResultColor;
    
    vResultColor.rgb = vMtrlDiffuse.rgb + vLavaColor.rgb * (1 - vMtrlDiffuse.a);
    vResultColor.a = 1.f;
    
    Out.vDiffuse = vResultColor;
    Out.vNormal = vector(vLavaColor.rgb * (1 - vMtrlDiffuse.a), 0.f);
    return Out;
}

PS_OUT PS_MAIN_SMOKE(PS_IN In)
{
    PS_OUT Out;
    
    float2 vTexcoord = In.vTexcoord;
    vTexcoord.y += g_Time * 0.03f;
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, vTexcoord);
    vTexcoord.y *= 1.5f;
    vector vMtrlMask = g_MaskTexture.Sample(LinearSampler, vTexcoord);
    vector vLavaColor = { 1.2f, 0.2564f, 0.001f, 1.f };
    vector vYellowColor = { 1.0f, 0.60f, 0.0f, 1.0f };
    vector vResultColor;
    
    float yellowFactor = saturate(1.373 - saturate(vTexcoord.y));
    
    vMtrlDiffuse = lerp(vMtrlDiffuse, vMtrlMask, 0.3f) * 1.5f;
    vResultColor.rgb = lerp(vLavaColor.rgb, vYellowColor.rgb, yellowFactor) * vMtrlDiffuse.g;
    vResultColor.a = ((1.0 - abs(In.vTexcoord.x - 0.5) * 1.5) * (In.vTexcoord.y * vResultColor.r)) * 1.373f;

    Out.vDiffuse = vResultColor;
    Out.vNormal = vector(vLavaColor.rgb * (1 - vMtrlDiffuse.a), 0.f);
    return Out;
}

PS_OUT PS_MAIN_DESTRUCTIVE(PS_IN In)
{
    PS_OUT Out;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
   // vector vMtrlAlpha = g_AlphaTexture.Sample(LinearSampler, In.vTexcoord);
    //if (vMtrlDiffuse.a < 0.99f)
    //    discard;
   
    vector vLavaColor = { 255.f / 255.f, 68.f / 255.f, 0.f / 255.f, 1.f };
    //vector vLavaColor = { 234.f / 255.f, 0.5f / 255.f, 0.f / 255.f, 1.f };
    //vLavaColor *= 0.7f;
    vector vResultColor;
    vResultColor.rgb = vMtrlDiffuse.rgb + vLavaColor.rgb * (vMtrlDiffuse.a);
    vResultColor.a = 1.f;
    //vMtrlDiffuse.a
    Out.vDiffuse = vResultColor;
    
    Out.vNormal = vector(vLavaColor.rgb * (vMtrlDiffuse.a), 0.f);
    //Out.vDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
    return Out;
}

PS_OUT PS_MAIN_DESTRUCTIVE_MOUNTAIN(PS_IN In)
{
    PS_OUT Out;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
   // vector vMtrlAlpha = g_AlphaTexture.Sample(LinearSampler, In.vTexcoord);
    //if (vMtrlDiffuse.a < 0.99f)
    //    discard;
   
    vector vLavaColor = { 255.f / 255.f, 68.f / 255.f, 0.f / 255.f, 1.f };
    //vector vLavaColor = { 234.f / 255.f, 0.5f / 255.f, 0.f / 255.f, 1.f };
    //vLavaColor *= 0.7f;
    vector vResultColor;
    vResultColor.rgb = vMtrlDiffuse.rgb + vLavaColor.rgb * (1 - vMtrlDiffuse.a);
    vResultColor.a = 1.f;
    //vMtrlDiffuse.a
    Out.vDiffuse = vResultColor;
    
    Out.vNormal = vector(vLavaColor.rgb * (vMtrlDiffuse.a), 0.f);
    //Out.vDepth = vector(In.vProjPos.w / 1000.f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
    return Out;
}

PS_OUT PS_MAIN_EAST_RECT(PS_IN In)
{
    PS_OUT Out;
    float2 vTexcoord = In.vTexcoord;
    
    Out.vDiffuse = g_EastGlowTexture.Sample(LinearSampler, vTexcoord);
    Out.vDiffuse.a *= 0.9f;
    Out.vDiffuse.rgb = g_EastColor;
   
    return Out;
}
technique11 DefaultTechnique
{
    pass Default // 0
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

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

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_ISLAND();
    }

    pass LavaPool // 2
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_LAVAPOOL();
    }

    pass Sky // 3
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SKY();
    }

    pass Island2 // 4
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_ISLAND2();
    }

    pass LavaFall1 // 5
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_LAVAFALL1();
    }

    pass LavaFall2 // 6
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
      
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_LAVAFALL2();
    }

    pass LavaFall3 // 7
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_LAVAFALL3();
    }

    pass LavaFall4 // 8
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_LAVAFALL4();
    }

    pass LavaFall5 // 9
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_LAVAFALL5();
    }

    pass LavaFall6 // 10
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_LAVAFALL6();
    }

    pass LavaFall7 // 11
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_LAVAFALL7();
    }

    pass LavaFallGround // 12
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_LAVAGROUND();
    }

    pass Cliff1 // 13
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_CLIFF1();
    }

    pass Cliff2 // 14
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_CLIFF2();
    }

    pass Cliff3 // 15
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_CLIFF3();
    }

    pass Cliff4 // 16
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_CLIFF4();
    }

    pass SMOKE // 17
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SMOKE();
    }
    pass Destructive // 18
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DESTRUCTIVE();
    }
    pass DestructiveMountain // 19
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DESTRUCTIVE_MOUNTAIN();
    }
    pass DestructivePaticle // 20
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_DESTRUCTIVE();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DESTRUCTIVE_MOUNTAIN();
    }
    pass EastRect // 21
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_EAST_RECT();
    }
    pass EastMAIN // 22
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_EAST();
    }
    pass Stage // 23
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

vector TexScalar_ToSampling(float2 vScale, float2 vScroll, float2 vTexcoord)
{
    vTexcoord *= vScale;
    vTexcoord += g_Time * vScroll;
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, vTexcoord);
    return vMtrlDiffuse;
}

vector ColorLerpScalarToDiffuse(vector vLerpParam, vector vDiffuseSrc, vector vDiffuseDest)
{
    vector vResult;
    vResult.r = lerp(vDiffuseSrc.r, vDiffuseDest.r, vLerpParam.r);
    vResult.g = lerp(vDiffuseSrc.g, vDiffuseDest.g, vLerpParam.g);
    vResult.b = lerp(vDiffuseSrc.b, vDiffuseDest.b, vLerpParam.b);
    vResult.a = lerp(vDiffuseSrc.a, vDiffuseDest.a, vLerpParam.a);
    return vResult;
}




