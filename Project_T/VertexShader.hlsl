struct VS_IN {

	float3 pos : POSITION;
	float2 texUV : TEXCOORD;
	float3 normal : NORMAL;

};

struct VS_IN2 {

	float3 pos : POSITION;
	float3 col : COLOR;
	float3 normal : NORMAL;

};

struct VS_OUT {

	float4 pos : POSITION;
	float2 texUV : TEXCOORD;
	float3 normal : NORMAL;

};

struct VS_OUT2 {

	float4 pos : POSITION;
	float3 col : COLOR;
	float3 normal : NORMAL;

};

VS_OUT VS_main(VS_IN input){

	VS_OUT output = (VS_OUT)0;

	output.pos = float4(input.pos, 1.0f);
	output.texUV = input.texUV;
	output.normal = input.normal;

	return output;

}

VS_OUT2 VS_main_Color(VS_IN2 input) {

	VS_OUT2 output = (VS_OUT2)0;

	output.pos = float4(input.pos, 1.0f);
	output.col = input.col;
	output.normal = input.normal;

	return output;

}