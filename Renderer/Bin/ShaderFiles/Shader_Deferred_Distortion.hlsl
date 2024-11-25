
#include "Renderer_Shader_Defines.hlsli"

float4x4		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

float4x4		g_ViewMatrixInv, g_ProjMatrixInv;

texture2D       g_Texture, g_MaskTexture, g_BackBufferTexture;

float           fLifeMaxTime;
float			fLifeTime;
float			fFactor;
float g_Time;
float3 g_vDir;
float g_Factor;

float4 ApplySepia(float4 color);
float4 AddGrain(float4 color, float2 uv, float grainIntensity);
float4 AdjustBrightness(float4 color, float brightness);

float3 GammaCorrection(float3 vColor, float fGamma);
float3 ToneMap(float3 vColor);


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


VS_OUT VS_MAIN_DISTORTION(VS_IN In)
{
	VS_OUT			Out;
	
    //float cosTheta = g_vDir.x; 
    //float sinTheta = g_vDir.y; 

    //float4x4 zRotationMatrix = float4x4(
    //cosTheta, -sinTheta, 0, 0,
    //sinTheta, cosTheta, 0, 0,
    //0, 0, 1, 0,
    //0, 0, 0, 1
    //);

    //float4x4 rotatedWorldMatrix = mul(g_WorldMatrix, zRotationMatrix);
    //vector vPosition = mul(vector(In.vPosition, 1.f), rotatedWorldMatrix);
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
	float4	vColor : SV_TARGET0;
};

PS_OUT PS_MAIN_DRAW_DISTORTION(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;	

    
    float2 speedUV = In.vTexcoord;
    //speedUV.x += g_Time * 0.1f;
    
    vector vBaseDiffuse = g_Texture.Sample(LinearSampler, In.vTexcoord);
    vector vMaskDiffuse = g_MaskTexture.Sample(LinearSampler, speedUV);
    
    Out.vColor.g = ((vMaskDiffuse.g * vBaseDiffuse.a) * (fLifeTime / fLifeMaxTime)) * g_Factor * abs(g_vDir.x);
    Out.vColor.b = ((vMaskDiffuse.g * vBaseDiffuse.a) * (fLifeTime / fLifeMaxTime)) * g_Factor * abs(g_vDir.y);
    
	return Out;
}

PS_OUT PS_MAIN_DRAW_DISTORTION_MOVE(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    
    float2 speedUV = In.vTexcoord;
    speedUV.x += g_Time * 0.01f;
    
    vector vBaseDiffuse = g_Texture.Sample(LinearSampler, In.vTexcoord);
    vector vMaskDiffuse = g_MaskTexture.Sample(LinearSampler, speedUV);
    
    Out.vColor.g = ((vMaskDiffuse.g * vBaseDiffuse.a) * (fLifeTime / fLifeMaxTime)) * g_Factor * abs(g_vDir.x);
    Out.vColor.b = ((vMaskDiffuse.g * vBaseDiffuse.a) * (fLifeTime / fLifeMaxTime)) * g_Factor * abs(g_vDir.y);
    return Out;
}

PS_OUT PS_MAIN_DISTORTION_TO_BACKBUFFER(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    float3 vDistortion = g_Texture.Sample(LinearSampler, In.vTexcoord).rgb;

    float2 distortedUV = In.vTexcoord;
    /* 디스토션 렌더타겟에서 0 0 0 0 인 부분을 검출 해서 만약 그렇다면 UV좌표 자기 기존 텍스쿠드로 해야될듯 */
    distortedUV.x += (vDistortion.g) * 0.07f;
    distortedUV.y += (vDistortion.b) * 0.07f;
    
    float4 vBackBufferColor = g_BackBufferTexture.Sample(LinearSampler, distortedUV);

    Out.vColor = vBackBufferColor;

    return Out;
}

PS_OUT PS_MAIN_WHITE(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    float4 vBackBufferColor = g_BackBufferTexture.Sample(LinearSampler, In.vTexcoord);

    //float gray = dot(vBackBufferColor.rgb, float3(0.299, 0.587, 0.114));

    //Out.vColor = float4(gray, gray, gray, vBackBufferColor.a);
    //Out.vColor = ApplySepia(vBackBufferColor);
    float grainIntensity = 1.f; // 0.0 = 없음, 1.0 = 강함
    Out.vColor = ApplySepia(vBackBufferColor);
    Out.vColor = AddGrain(Out.vColor, In.vTexcoord, 0.2);
    return Out;
}


PS_OUT PS_MAIN_RESULT(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    
    return Out;
}
PS_OUT PS_MAIN_POST_PROCESS(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    vector vBackBufferColor = g_BackBufferTexture.Sample(LinearSampler, In.vTexcoord);
    
    //vBackBufferColor.rgb = ToneMap(vBackBufferColor.rgb);
    vBackBufferColor.rgb = GammaCorrection(vBackBufferColor.rgb, 1.8f);
    Out.vColor = vBackBufferColor;
    return Out;
}

technique11		DefaultTechnique
{	
	pass DrawDistortion //0
	{
		SetRasterizerState(RS_Cull_None);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_OneBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_DISTORTION();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DRAW_DISTORTION();
	}	

    pass DrawDistortionToBackBuffer//1
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DISTORTION_TO_BACKBUFFER();
    }

    pass Result//2
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_RESULT();
    }

    pass White //3
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_WHITE();
    }

    pass DistortionMove //4
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_OneBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DRAW_DISTORTION_MOVE();
    }

    pass PostProcess //5
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_POST_PROCESS();
    }

}

float4 ApplySepia(float4 color)
{
    float3 sepia = float3(
        dot(color.rgb, float3(0.393, 0.769, 0.189)), // R
        dot(color.rgb, float3(0.349, 0.686, 0.168)), // G
        dot(color.rgb, float3(0.272, 0.534, 0.131))  // B
    );
    return float4(saturate(sepia), color.a);
}

float4 AdjustBrightness(float4 color, float brightness)
{
    return float4(color.rgb * brightness, color.a); // 밝기 조절
}

float4 AddGrain(float4 color, float2 uv, float grainIntensity)
{
    float noise = frac(sin(dot(uv.xy, float2(12.9898, 78.233))) * 43758.5453); // 간단한 노이즈 생성
    float grain = lerp(1.0, noise, grainIntensity); // 그레인 강도 조절
    return float4(color.rgb * grain, color.a); // 색상에 곱하여 적용
}

float3 ToneMap(float3 vColor)
{
    // Reinhard Tone Mapping
    return vColor / (1.0 + vColor);
}

float3 GammaCorrection(float3 vColor, float fGamma)
{
    return pow(vColor, 1.0 / fGamma);
}






