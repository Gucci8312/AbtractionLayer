Texture2D		TextureData : register(t0);
SamplerState	Sampler		: register(s0);

cbuffer ConstantCamera : register(b0)
{
    float4 camPos;
    float4x4 view;
    float4x4 projection;
    float4x4 VP;
    float4x4 invView;
    float4x4 invProjection;
    float4x4 inversVp;
}

struct PS_IN
{
	float4 positon      : SV_POSITION;
    float4 worldPos     : POSITION;
	float4 normal       : NORMAL;
	float4 color        : COLOR;
	float2 texCoord     : TEXCOORD;
};

void main(in PS_IN In, out float4 Out : SV_Target0)
{
    float4 cor  = In.color;// * TextureData.Sample(Sampler, In.texCoord);
    float4 nor	= normalize(In.normal);
	float2 uv	= In.texCoord;	
    
    float3 L	= normalize(float3(1.0, -1.0, 1.0));
    float3 E    = normalize(camPos.xyz - In.worldPos.xyz);
    float3 H    = normalize(E - L);
    
	float  lit	= -dot(L, nor.xyz) * 0.5 + 0.5;
    float  spec = pow(saturate(dot(H, In.normal.xyz)), 10.0) * 0.5;
    
    cor.rgb = cor.rgb * lit + spec;
   //if(TextureData.Sample(Sampler, In.texCoord).r < 0.8) discard;

    Out = cor;
}