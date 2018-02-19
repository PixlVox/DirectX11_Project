Texture2D grass : register(t0);
SamplerState samp : register(s0);


struct PS_IN {

	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 texUV : TEXCOORD;

};

struct PS_OUT {

	float4 col : COLOR;

};

PS_OUT PS_main(PS_IN input) : SV_Target{

	PS_OUT output = (PS_OUT)0;
	output.col = grass.Sample(samp, input.texUV);

	return output;

};