#include "Renderer_Shader_Defines.hlsli"
#include "Shader_Material_Defines.hlsli"

float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

MaterialDesc    g_MaterialDesc[100];    // All_In_One
int             g_iNumTextures;         // 루프 돌아야됨

int             g_iStack[100];

Texture2DArray g_TexturesArray : register(t0); // 구조체안에 넣으면 쉐이더에 넣기 애매해짐 텍스쳐 배열 접근은 변수접근 불가 이렇게 해야됨

//vector LoopMaterial(int iCurrentIndex, float2 vOriginTexcoord)
//{
//    float2 vTexcoord = vOriginTexcoord;
//    vector vBase;
	
//    if (true == g_MaterialDesc[iCurrentIndex].isSpriteAnimation)
//        vTexcoord = SpriteAnimation(vTexcoord, g_MaterialDesc[iCurrentIndex].SpriteAnim);
//    else if (true == g_MaterialDesc[iCurrentIndex].isTexMove)
//        vTexcoord = TexcoordMove(vTexcoord, g_MaterialDesc[iCurrentIndex].TexMove);

//    vBase = g_TexturesArray.Sample(LinearSampler, float3(vTexcoord, g_MaterialDesc[iCurrentIndex].iBaseIndex));

//    if (-1 != g_MaterialDesc[iCurrentIndex].iDiffuseInputIndex)
//        vBase.rgb = LoopMaterial(++iCurrentIndex, vOriginTexcoord).rgb;
//    else if (-1 != g_MaterialDesc[iCurrentIndex].iAlphaInputIndex)
//        vBase = LoopMaterial(++iCurrentIndex, vOriginTexcoord);
    
//    return vBase;
//}

//vector LoopMaterial(float2 vOriginTexcoord)
//{
//    int iNumTextures = g_iNumTextures;
//    vector g_vSampleColor[100];
//    for (int i = 0; i < iNumTextures; i++)
//    {
//        float2 vTexcoord = vOriginTexcoord;
        
//        if (true == g_MaterialDesc[i].isSpriteAnimation)
//            vTexcoord = SpriteAnimation(vTexcoord, g_MaterialDesc[i].SpriteAnim);
//        else if (true == g_MaterialDesc[i].isTexMove)
//            vTexcoord = TexcoordMove(vTexcoord, g_MaterialDesc[i].TexMove);
        
//        g_vSampleColor[g_MaterialDesc[i].iBaseIndex] = g_TexturesArray.Sample(LinearSampler, float3(vTexcoord, g_MaterialDesc[i].iBaseIndex));
//        //stack[i] = g_MaterialDesc[i].

//    }

//    for (int j = 0; j < g_iNumTextures; j++)
//    {
//        if (-1 == g_MaterialDesc[j].iDiffuseInputIndex && -1 == g_MaterialDesc[j].iAlphaInputIndex)
//        {
//            if (g_MaterialDesc[j].iBaseIndex == g_MaterialDesc[g_MaterialDesc[j].iOutputIndex].iDiffuseInputIndex)
//                g_vSampleColor[g_MaterialDesc[g_MaterialDesc[j].iOutputIndex].iBaseIndex].rgb = g_vSampleColor[g_MaterialDesc[j].iBaseIndex].rgb;
//            else
//                g_vSampleColor[g_MaterialDesc[g_MaterialDesc[j].iOutputIndex].iBaseIndex].a -= g_vSampleColor[g_MaterialDesc[j].iBaseIndex].a;
//        }
//    }

//    //if (-1 != g_MaterialDesc[iCurrentIndex].iDiffuseInputIndex)
//    //    vBase.rgb = LoopMaterial(++iCurrentIndex, vOriginTexcoord).rgb;
//    //else if (-1 != g_MaterialDesc[iCurrentIndex].iAlphaInputIndex)
//    //    vBase = LoopMaterial(++iCurrentIndex, vOriginTexcoord);
    
//    return g_vSampleColor[0];
//}

//vector LoopMaterialPostOrder(int iStartIndex, float2 vTexcoord)
//{

//    while (stackIndex > 0)
//    {
//        int currentIndex = g_iStack[--stackIndex];

//        float2 tempTexcoord = vTexcoord;

//        if (g_MaterialDesc[currentIndex].isSpriteAnimation)
//        {
//            tempTexcoord = SpriteAnimation(tempTexcoord, g_MaterialDesc[currentIndex].SpriteAnim);
//        }
//        else if (g_MaterialDesc[currentIndex].isTexMove)
//        {
//            tempTexcoord = TexcoordMove(tempTexcoord, g_MaterialDesc[currentIndex].TexMove);
//        }

//        vector tempColor = g_TexturesArray.Sample(LinearSampler, float3(tempTexcoord, g_MaterialDesc[currentIndex].iBaseIndex));

//        if (g_MaterialDesc[currentIndex].iDiffuseInputIndex != -1)
//        {
//            vResult.rgb = tempColor.rgb;
//        }
//        else if (g_MaterialDesc[currentIndex].iAlphaInputIndex != -1)
//        {
//            vResult.a *= tempColor.a;
//        }
//        else
//        {
//            vSampleColor = tempColor;
//        }
//    }

//    return vResult;
//}

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

	/* mul : 곱하기가 가능한 모든 행렬(좌변의 열, 우변의 행 같다면)에 대해서 다 곱하기를 수행해준다. */
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
    float4 vAlpha : SV_TARGET1;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;
    
   // Out.vColor = LoopMaterial(In.vTexcoord);
    
    Out.vAlpha = vector(Out.vColor.a, 0.f, 0.f, 1.f);

    return Out;
}

technique11	DefaultTechnique
{
	pass DefaultMaterial
	{
        SetRasterizerState(RS_Cull_None);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
}














