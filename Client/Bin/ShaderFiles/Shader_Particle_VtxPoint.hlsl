
#include "Renderer_Shader_Defines.hlsli"

float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D g_Texture;
float4 g_vColor = vector(1.f, 1.f, 1.f, 1.f);
float4 g_vCamPosition;
float g_Time;
float g_MaxTime;

float g_ScaleX = 7.f;
float g_ScaleY = 0.5f;

struct VS_IN
{
	/* 정점 쉐이더마다 갱신되서 들어오는 데이터들 */
    float3 vPosition : POSITION;

	/* 정점 여섯개마다 한번씩 갱신되는 데이터들. */
    // 인스턴스 버퍼에 있는 녀석들
    row_major matrix LocalMatrix : WORLD;
    float2 vLifeTime : TEXCOORD0;
    float3 vMoveDir : TEXCOORD1; // New field for moveDir
};

struct VS_OUT
{
    float4 vPosition : POSITION;
    float2 vPSize : PSIZE;
    float2 vLifeTime : TEXCOORD0;
    float3 vMoveDir : TEXCOORD1; // New field for moveDir
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;

    vector vPosition = mul(vector(In.vPosition, 1.f), In.LocalMatrix);
    Out.vPosition = mul(vPosition, g_WorldMatrix);
    Out.vPSize = float2(length(In.LocalMatrix._11_12_13), length(In.LocalMatrix._21_22_23));
    Out.vLifeTime = In.vLifeTime;
    Out.vMoveDir = In.vMoveDir;
    
    return Out;
}

struct GS_IN
{
    float4 vPosition : POSITION;
    float2 vPSize : PSIZE;
    float2 vLifeTime : TEXCOORD0;
    float3 vMoveDir : TEXCOORD1;
};

struct GS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float2 vLifeTime : TEXCOORD1;
};

/* 사각형을 구성하기위한 정점을 생성할꺼야. */
/* 항상 카메라를 쳐다보게끔 만들어줄꺼야. */
/* g_vCamPosition위치랑 정점의 위치의 차를 통해 사각형이 바라봐야할 룩을 구할거야. */
/* 월드공간상에서 연산이 일어난다고 볼 수 있기때문에 정점의 위치를 월드까지면 변환하여 전달한다. */

/* GeometryShader : 정점을 추가 생성해줄 수 있다. */
/* Point를 그린다. 파라미터로 정점을 하나만 받는거야. point GS_IN In[1] */
/* Line를 그린다. 파라미터로 정점을 두개만 받는거야. line GS_IN In[2]*/
/* Triangle를 그린다. 파라미터로 정점을 세개만 받는거야. triangle GS_IN In[3]*/

[maxvertexcount(20)]
void GS_MAIN(point GS_IN In[1], inout TriangleStream<GS_OUT> VertexStream)
{
    GS_OUT Out[4];

    // 카메라를 향하는 벡터 계산
    float3 vLook = normalize(g_vCamPosition.xyz - In[0].vPosition.xyz);
    float3 vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook)) * In[0].vPSize.x * 0.5f;
    float3 vUp = normalize(cross(vLook, vRight)) * In[0].vPSize.y * 0.5f;

    // 여기에서 원하는 배율을 적용합니다.
    float xScaleFactor = 5.f; // X축 크기 배율
    float yScaleFactor = 2.0f; // Y축 크기 배율

    vRight *= xScaleFactor;
    vUp *= yScaleFactor;

    matrix matVP = mul(g_ViewMatrix, g_ProjMatrix);

    // 사각형의 각 정점 계산
    Out[0].vPosition = float4(In[0].vPosition.xyz + vRight + vUp, 1.f);
    Out[0].vTexcoord = float2(0.f, 0.f);
    Out[0].vPosition = mul(Out[0].vPosition, matVP);
    Out[0].vLifeTime = In[0].vLifeTime;

    Out[1].vPosition = float4(In[0].vPosition.xyz - vRight + vUp, 1.f);
    Out[1].vTexcoord = float2(1.f, 0.f);
    Out[1].vPosition = mul(Out[1].vPosition, matVP);
    Out[1].vLifeTime = In[0].vLifeTime;

    Out[2].vPosition = float4(In[0].vPosition.xyz - vRight - vUp, 1.f);
    Out[2].vTexcoord = float2(1.f, 1.f);
    Out[2].vPosition = mul(Out[2].vPosition, matVP);
    Out[2].vLifeTime = In[0].vLifeTime;

    Out[3].vPosition = float4(In[0].vPosition.xyz + vRight - vUp, 1.f);
    Out[3].vTexcoord = float2(0.f, 1.f);
    Out[3].vPosition = mul(Out[3].vPosition, matVP);
    Out[3].vLifeTime = In[0].vLifeTime;

    // 삼각형 스트립 생성
    VertexStream.Append(Out[0]);
    VertexStream.Append(Out[1]);
    VertexStream.Append(Out[2]);
    VertexStream.RestartStrip();

    VertexStream.Append(Out[0]);
    VertexStream.Append(Out[2]);
    VertexStream.Append(Out[3]);
    VertexStream.RestartStrip();
}

[maxvertexcount(20)]
void GS_MAIN_NOTBillBoard(point GS_IN In[1], inout TriangleStream<GS_OUT> VertexStream)
{
  // moveDir을 정규화하여 방향 벡터 생성
    float3 vDir = normalize(In[0].vMoveDir);

    // vDir에 수직인 벡터 계산
    float3 worldUp = float3(0.0f, 1.0f, 0.0f);
    if (abs(dot(vDir, worldUp)) > 0.99f)
    {
        worldUp = float3(1.0f, 0.0f, 0.0f);
    }
    float3 vPerp = normalize(cross(worldUp, vDir));

    // 스케일링 팩터 적용
    float lengthDir = g_ScaleX * In[0].vPSize.x;
    float lengthPerp = g_ScaleY * In[0].vPSize.y;

    // 중심 위치
    float3 center = In[0].vPosition.xyz;

    // 네 꼭지점 계산 (순서 조정)
    float3 corner0 = center - (vDir * lengthDir * 0.5f) + (vPerp * lengthPerp * 0.5f); // 좌상단
    float3 corner1 = center + (vDir * lengthDir * 0.5f) + (vPerp * lengthPerp * 0.5f); // 우상단
    float3 corner2 = center + (vDir * lengthDir * 0.5f) - (vPerp * lengthPerp * 0.5f); // 우하단
    float3 corner3 = center - (vDir * lengthDir * 0.5f) - (vPerp * lengthPerp * 0.5f); // 좌하단

    // 텍스처 좌표 설정 (순서 조정)
    float2 texcoords[4] =
    {
        float2(0.0f, 0.0f), // corner0 (좌하단)
        float2(1.0f, 0.0f), // corner1 (우하단)
        float2(1.0f, 1.0f), // corner2 (우상단)
        float2(0.0f, 1.0f) // corner3 (좌상단)
    };

    // 뷰-프로젝션 행렬 계산
    matrix matVP = mul(g_ViewMatrix, g_ProjMatrix);

    // 출력 구조체에 값 설정 및 변환
    GS_OUT vertices[4];
    float3 corners[4] = { corner0, corner1, corner2, corner3 };
    for (int i = 0; i < 4; ++i)
    {
        vertices[i].vPosition = mul(float4(corners[i], 1.0f), matVP);
        vertices[i].vTexcoord = texcoords[i];
        vertices[i].vLifeTime = In[0].vLifeTime;
    }

    // 삼각형 생성 (순서 유지)
    VertexStream.Append(vertices[0]); // corner0
    VertexStream.Append(vertices[1]); // corner1
    VertexStream.Append(vertices[2]); // corner2

    VertexStream.Append(vertices[0]); // corner0
    VertexStream.Append(vertices[2]); // corner2
    VertexStream.Append(vertices[3]); // corner3
}



[maxvertexcount(20)]
void GS_QTE_PARTICLE(point GS_IN In[1], inout TriangleStream<GS_OUT> VertexStream)
{
    GS_OUT Out[4];

    float3 vRight = float3(In[0].vPSize.x * 0.5f, 0.f, 0.f);
    float3 vUp = float3(0.f, In[0].vPSize.y * 0.5f, 0.f);

    matrix matVP = mul(g_ViewMatrix, g_ProjMatrix);
    Out[0].vPosition = vector(In[0].vPosition.xyz - vRight + vUp, 1.f);
    Out[0].vTexcoord = float2(0.f, 0.f);
    Out[0].vPosition = mul(Out[0].vPosition, matVP);
    Out[0].vLifeTime = In[0].vLifeTime;

    Out[1].vPosition = vector(In[0].vPosition.xyz + vRight + vUp, 1.f);
    Out[1].vTexcoord = float2(1.f, 0.f);
    Out[1].vPosition = mul(Out[1].vPosition, matVP);
    Out[1].vLifeTime = In[0].vLifeTime;

    Out[2].vPosition = vector(In[0].vPosition.xyz + vRight - vUp, 1.f);
    Out[2].vTexcoord = float2(1.f, 1.f);
    Out[2].vPosition = mul(Out[2].vPosition, matVP);
    Out[2].vLifeTime = In[0].vLifeTime;

    Out[3].vPosition = vector(In[0].vPosition.xyz - vRight - vUp, 1.f);
    Out[3].vTexcoord = float2(0.f, 1.f);
    Out[3].vPosition = mul(Out[3].vPosition, matVP);
    Out[3].vLifeTime = In[0].vLifeTime;
	
    VertexStream.Append(Out[0]);
    VertexStream.Append(Out[1]);
    VertexStream.Append(Out[2]);
    VertexStream.RestartStrip();

    VertexStream.Append(Out[0]);
    VertexStream.Append(Out[2]);
    VertexStream.Append(Out[3]);
    VertexStream.RestartStrip();
}


[maxvertexcount(20)]
void GS_DUST(point GS_IN In[1], inout TriangleStream<GS_OUT> VertexStream)
{
    GS_OUT Out[4];

    float3 vLook, vRight, vUp;
    vLook = g_vCamPosition.xyz - In[0].vPosition.xyz;
    vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook)) * In[0].vPSize.x * 0.5f;
    vUp = normalize(cross(vLook, vRight)) * In[0].vPSize.y * 0.5f;

    matrix matVP = mul(g_ViewMatrix, g_ProjMatrix);
    Out[0].vPosition = vector(In[0].vPosition.xyz - vRight + vUp, 1.f);
    Out[0].vTexcoord = float2(0.f, 0.f);
    Out[0].vPosition = mul(Out[0].vPosition, matVP);
    Out[0].vLifeTime = In[0].vLifeTime;

    Out[1].vPosition = vector(In[0].vPosition.xyz + vRight + vUp, 1.f);
    Out[1].vTexcoord = float2(1.f, 0.f);
    Out[1].vPosition = mul(Out[1].vPosition, matVP);
    Out[1].vLifeTime = In[0].vLifeTime;

    Out[2].vPosition = vector(In[0].vPosition.xyz + vRight - vUp, 1.f);
    Out[2].vTexcoord = float2(1.f, 1.f);
    Out[2].vPosition = mul(Out[2].vPosition, matVP);
    Out[2].vLifeTime = In[0].vLifeTime;

    Out[3].vPosition = vector(In[0].vPosition.xyz - vRight - vUp, 1.f);
    Out[3].vTexcoord = float2(0.f, 1.f);
    Out[3].vPosition = mul(Out[3].vPosition, matVP);
    Out[3].vLifeTime = In[0].vLifeTime;
	
    VertexStream.Append(Out[0]);
    VertexStream.Append(Out[1]);
    VertexStream.Append(Out[2]);
    VertexStream.RestartStrip();

    VertexStream.Append(Out[0]);
    VertexStream.Append(Out[2]);
    VertexStream.Append(Out[3]);
    VertexStream.RestartStrip();
}


struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float2 vLifeTime : TEXCOORD1;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;

    // 중심 좌표
    float2 center = float2(0.5f, 0.5f);

    // 중심으로부터의 거리 계산
    float distance = length(In.vTexcoord - center);

    // 거리 기반으로 그라데이션 강도 계산 (0.0 ~ 1.0)
    float gradient = saturate(1.0f - distance * 1.5f); // 멀수록 0, 가까울수록 1

    // 흰색(밝은 부분)과 g_vColor(보라색)의 혼합
    float4 white = float4(1.0f, 1.0f, 1.0f, 1.0f);
    Out.vColor = saturate(lerp(g_vColor, white, gradient));

    // 알파 값 조건에 따라 픽셀 버리기
    if (In.vLifeTime.y >= In.vLifeTime.x || Out.vColor.a < 0.1f)
        discard;

    return Out;
}


PS_OUT PS_QTE_PARTICLE(PS_IN In)
{
    PS_OUT Out;

    Out.vColor = g_vColor;

    if (In.vLifeTime.y >= In.vLifeTime.x || Out.vColor.a < 0.1f)
        discard;

    return Out;
}

PS_OUT PS_RUN_DUST(PS_IN In)
{
    PS_OUT Out;

    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    //Out.vColor.a = Out.vColor.r;

    if (In.vLifeTime.y >= In.vLifeTime.x || Out.vColor.a < 0.1f)
        discard;
    
    Out.vColor.a *= min((In.vLifeTime.x - In.vLifeTime.y) + 0.5f, 1.f);
    //Out.vColor.rgb = Out.vColor.rgb * float3(0.529, 0.290, 0.078);

    return Out;
}

technique11 DefaultTechnique
{
	/* PASS의 기준 : 셰이더 기법의 캡슐화. */
//0
    pass Default
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

//1
    pass QTE_PARTICLE
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_QTE_PARTICLE();
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_QTE_PARTICLE();
    }

//2 
    pass RUN_DUST
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_DUST();
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_RUN_DUST();
    }

//3
    pass Default_NotBillBoard
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN_NOTBillBoard();
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}














