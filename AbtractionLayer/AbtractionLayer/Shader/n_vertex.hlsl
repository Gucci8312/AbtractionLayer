cbuffer ConstantMatrix : register(b0)
{
    float4	 camPos;
    float4x4 world;
    float4x4 view;
    float4x4 projection;
    float4x4 VP;
}

struct VS_INPUT 
{
	float4 position : POSITION;
	float4 normal	: NORMAL;
	float4 color	: COLOR;
	float2 texCoord	: TEXCOORD;
    uint   instance : SV_InstanceID;
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
	
    //In.position.x += In.instance * 5;
	
    In.position		+= normalize(In.normal) * In.instance * 0.005;
	
    Out.positon		= mul(In.position, WVP);
	Out.worldPos	= mul(In.position, world);
	Out.normal		= mul(In.normal	 , world);
	Out.color		= In.color;
	Out.texCoord	= In.texCoord;
}