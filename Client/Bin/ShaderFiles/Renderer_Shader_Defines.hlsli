

sampler DestroySampler = sampler_state
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = clamp;
    AddressV = clamp;
};

sampler LinearSampler = sampler_state
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = wrap;
	AddressV = wrap;
};

sampler PointSampler = sampler_state
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = wrap;
	AddressV = wrap;
};

/* D3D11_RASTERIZER_DESC */
RasterizerState RS_Default
{
	FillMode = Solid;
	CullMode = Back;
	FrontCounterClockwise = false;
};

RasterizerState RS_Fill_WireFrame
{
	FillMode = WireFrame;
	CullMode = Back;	
	FrontCounterClockwise = false;

};

RasterizerState RS_Cull_CW
{
	FillMode = Solid;
	CullMode = Front;
	FrontCounterClockwise = false;
};

RasterizerState RS_Cull_None
{
	FillMode = Solid;
	CullMode = None;
	FrontCounterClockwise = false;
};

/* D3D11_DEPTH_STENCIL_DESC */
DepthStencilState DSS_Default
{
	DepthEnable = true;
	DepthWriteMask = all;
	DepthFunc = less_equal;
};

DepthStencilState DSS_None
{
	DepthEnable = false;
	DepthWriteMask = zero;
};

/* D3D11_BLEND_DESC */
BlendState BS_Default
{
	BlendEnable[0] = false;		

};

BlendState BS_AlphaBlend
{
	BlendEnable[0] = true;
	BlendEnable[1] = true;

	SrcBlend = Src_Alpha;
	DestBlend = Inv_Src_Alpha;
	BlendOp = Add;
};

BlendState BS_MultiplyBlend
{
    BlendEnable[0] = true;

    SrcBlend = Dest_Color; 
    DestBlend = Zero;
    BlendOp = Add;
};

BlendState BS_OneBlend
{
	BlendEnable[0] = true;	
	BlendEnable[1] = true;

	SrcBlend = one;
	DestBlend = one;
	BlendOp = Add;
};

