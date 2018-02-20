Texture2D grass : register(t0);
Texture2D stone : register(t1);
SamplerState samp : register(s0);


struct PS_IN {

	float4 pos : SV_POSITION;
	float2 texUV : TEXCOORD;
	float3 normal : NORMAL;

};

struct PS_OUT {

	float4 col : COLOR;

};

PS_OUT PS_main(PS_IN input) : SV_Target{

	PS_OUT output = (PS_OUT)0;
	output.col = grass.Sample(samp, input.texUV);
	output.col += stone.Sample(samp, input.texUV);

	return output;

};