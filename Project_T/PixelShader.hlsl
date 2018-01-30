struct PS_IN {

	float4 pos : SV_POSITION;
	float3 color : COLOR;

};

struct PS_OUT {

	float4 color : COLOR;

};

PS_OUT PS_main(PS_IN input) : SV_TARGET{

	PS_OUT output = (PS_OUT)0;
	output.color = float4(input.color, 1.0f);

	return output;

}