Texture2D grass : register(t0);
Texture2D stone : register(t1);
Texture2D window : register(t0);
Texture2D glow : register(t0);
SamplerState samp : register(s0);


struct PS_IN {

	float4 pos : SV_POSITION;
	float3 worldPos : POSITION;
	float2 texUV : TEXCOORD;
	float3 normal : NORMAL;

};

struct PS_OUT {

	float4 col : COLOR;

};

//Shader that maps color from stone texture
PS_OUT PS_main(PS_IN input) : SV_Target{

	PS_OUT output = (PS_OUT)0;

	output.col = stone.Sample(samp, input.texUV);
	output.col += grass.Sample(samp, input.texUV);

	return output;

};

//Shader that maps grass texture that is blended in
PS_OUT PS_main_Blend(PS_IN input) : SV_Target{

	PS_OUT output = (PS_OUT)0;

	output.col = window.Sample(samp, input.texUV);

	return output;

};

//Shader that maps glow texture
PS_OUT PS_main_Glow(PS_IN input) : SV_Target{

	PS_OUT output = (PS_OUT)0;

	output.col = glow.Sample(samp, input.texUV);

	return output;

};