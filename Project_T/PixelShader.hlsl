struct PS_IN {

	float4 pos : SV_POSITION;
	float3 normal : NORMAL;

};

struct PS_OUT {

	float4 col : COLOR;

};

PS_OUT PS_main(PS_IN input) : SV_Target{

	PS_OUT output = (PS_OUT)0;
	output.col = float4(input.normal, 1.0f);

	return output;

};