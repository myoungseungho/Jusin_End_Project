#include "Renderer_Shader_Defines.hlsli"

float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D g_Texture;
texture2D g_NextTexture;

vector g_vColor;

//QTE_UI
int g_IconState;
float g_Time;
float g_MaxTime;
float g_Ratio;

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
    VS_OUT Out;
    

    vector vPosition = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
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
    float4 vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;

    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

	//Out.vColor.gb = Out.vColor.r;
    if (Out.vColor.a <= 0.1f)
        discard;
    
    return Out;
}

PS_OUT PS_QTE_UI(PS_IN In)
{
    PS_OUT Out;

    // 텍스처 샘플링
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    
    // CORRECTLY_PRESSED 상태일 때 색상 회색으로 변환
    if (g_IconState == 3) // CORRECTLY_PRESSED
    {
        Out.vColor.rgb *= 0.5f; // 회색 효과 (0.5는 조절 가능)
    }
    // WRONG_PRESSED 상태일 때 색상 빨간색으로 변환
    else if (g_IconState == 4) // WRONG_PRESSED
    {
        // 빨간색을 강조하기 위해 빨간 채널을 증가시키고, 녹색과 파란 채널은 감소시킴
        Out.vColor.rgb = Out.vColor.rgb * float3(1.0f, 0.2f, 0.2f);
    }

    // 알파값이 낮은 픽셀은 버림
    if (Out.vColor.a <= 0.1f)
        discard;
    
    return Out;
}


PS_OUT PS_QTE_UI_GAUGE(PS_IN In)
{
    PS_OUT Out;

    // UV 좌표 가져오기
    float2 uv = In.vTexcoord.xy;

    // ratio를 g_Time / g_MaxTime으로 계산하고 클램프 (0.0 ~ 1.0)
    float ratio = saturate(g_Time / g_MaxTime);

    // 마스크 생성: uv.x <= (1.0 - ratio)인 경우 1, 그렇지 않으면 0
    float mask = (uv.x <= (1.0f - ratio)) ? 1.0f : 0.0f;

    // Second_Texture 샘플링 (채워진 부분)
    float4 secondColor = g_NextTexture.Sample(LinearSampler, uv);

    // First_Texture 샘플링 (배경)
    float4 firstColor = g_Texture.Sample(LinearSampler, uv);

    // 마스크를 이용해 두 텍스처를 구분하여 합성
    // mask가 1인 영역은 secondColor, 0인 영역은 firstColor
    Out.vColor = mask * secondColor + (1.0f - mask) * firstColor;

    // 알파값이 낮은 픽셀은 버림 (필요 시)
    if (Out.vColor.a <= 0.1f)
        discard;

    return Out;
}

PS_OUT PS_QTE_Hit_UI(PS_IN In)
{
    PS_OUT Out;

    // ratio 계산 (0에서 1까지)
    float ratio = g_Time / g_MaxTime;

    // 중심 좌표 설정 (텍스처 좌표 기준 0~1)
    float2 center = float2(0.5f, 0.5f);

    // 링의 초기 반지름과 최소 반지름 설정
    float originalRadius = 0.5f; // 초기 링의 반지름 (텍스처 기준)
    float minRadius = 0.166666f; // ratio가 1일 때의 최소 반지름, 0.5/3 한 결과 (현재 딱 3배 큼 기존 링)

    // ratio에 따라 현재 반지름을 보간 (0: originalRadius, 1: minRadius)
    float currentRadius = lerp(originalRadius, minRadius, ratio);

    // 스케일 팩터 계산 (반지름을 줄이기 위해 originalRadius / currentRadius)
    float scaleFactor = originalRadius / currentRadius;

    // 텍스처 좌표를 중심을 기준으로 스케일링
    float2 scaledTexcoord = (In.vTexcoord - center) * scaleFactor + center;

    // 텍스처 샘플링
    Out.vColor = g_Texture.Sample(DestroySampler, scaledTexcoord);
    
    // 알파값이 낮은 픽셀은 버림
    if (Out.vColor.a <= 0.1f)
        discard;
    
    //주황색 빛깔 나게
    Out.vColor = float4(255.f / 255.f, 127.f / 255.f, 39.f / 255.f, 1.f);

    return Out;
}


PS_OUT PS_QTE_CONTINUOUS_GAUGE(PS_IN In)
{
    PS_OUT Out;

    // UV 좌표 가져오기
    float2 uv = In.vTexcoord;

    // g_Ratio가 0에서 1 사이로 클램프
    float ratio = saturate(g_Ratio);

    // 마스크 생성: uv.x <= ratio인 경우 1, 그렇지 않으면 0
    float mask = (uv.x <= ratio) ? 1.0f : 0.0f;

    // Second_Texture 샘플링
    float4 secondColor = g_NextTexture.Sample(LinearSampler, uv);

    // First_Texture 샘플링
    float4 firstColor = g_Texture.Sample(LinearSampler, uv);

    // 마스크를 이용해 두 텍스처를 구분하여 합성
    // mask가 1인 영역은 secondColor, 0인 영역은 firstColor
    Out.vColor = mask * secondColor + (1.0f - mask) * firstColor;

    // 알파값이 낮은 픽셀은 버림 (필요 시)
    if (Out.vColor.a <= 0.1f)
        discard;

    return Out;
}

PS_OUT PS_QTE_Hit_Effect(PS_IN In)
{
    PS_OUT Out;

     // 텍스처 샘플링
    float4 baseColor = g_Texture.Sample(DestroySampler, In.vTexcoord);

    if (baseColor.a < 0.1)
        discard;
    
    // 중심에서의 거리 계산
    float2 center = float2(0.5, 0.5);
    float distanceFromCenter = distance(In.vTexcoord, center);

    // 시간 비율 계산
    float ratio = saturate(g_Time / g_MaxTime);

    // 거리 기반 표시 영역 조절
    float visibilityRadius = ratio * 1.5; // ratio에 따라 원의 반경이 확장됨

    // 반경 바깥쪽은 보이지 않도록 discard
    if (distanceFromCenter > visibilityRadius)
    {
        discard;
    }

    // 투명도 페이드 인/아웃
    float alpha;
    if (ratio < 0.1)
    {
        alpha = saturate(ratio / 0.1); // 0에서 0.1까지 페이드 인
    }
    else if (ratio > 0.8)
    {
        alpha = saturate((1.0 - ratio) / 0.2); // 0.8에서 1.0까지 페이드 아웃
    }
    else
    {
        alpha = 1.0; // 중간 구간에서는 완전 불투명
    }

    Out.vColor = baseColor;
    // 최종 색상과 투명도 결합
    Out.vColor.a = alpha;

    return Out;
}


technique11 DefaultTechnique
{
	/* PASS의 기준 : 셰이더 기법의 캡슐화. */

//0
    pass Default
    {

        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

//1
    pass QTE_UI
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
 
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_QTE_UI();
    }

//2
    pass QTE_UI_GAUGE
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
 
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_QTE_UI_GAUGE();
    }

//3
    pass QTE_Hit_UI
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
 
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_QTE_Hit_UI();
    }

//4
    pass QTE_CONTINUOUS_GAUGE
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
 
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_QTE_CONTINUOUS_GAUGE();
    }


//5
    pass QTE_HIT_EFFECT
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
 
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_QTE_Hit_Effect();
    }
}
