cbuffer ConstantCamera : register(b0)
{
    float4 camPos;
    float4x4 view;
    float4x4 projection;
    float4x4 VP;
    float4x4 invView;
    float4x4 invProjection;
    float4x4 invVp;
}

cbuffer ConstantObject : register(b1)
{
    float4 objectLocalPos;
    float4x4 world;
}

struct VS_INPUT 
{
	float4 position : POSITION;
	float4 normal	: NORMAL;
	float4 color	: COLOR;
	float2 texCoord	: TEXCOORD;
};

struct VS_OUTPUT
{
	float4 positon	: SV_Position;
    float4 worldPos : POSITION;
	float4 normal	: NORMAL;
	float4 color	: COLOR;
    float2 texCoord : TEXCOORD;
};

void main(in VS_INPUT In, out VS_OUTPUT Out)
{
    matrix WVP = mul(world, VP);
	
    Out.positon		= mul(In.position, WVP);
	Out.worldPos	= mul(In.position, world);
	Out.normal		= mul(In.normal	 , world);
	Out.color		= In.color;
	Out.texCoord	= In.texCoord;
}