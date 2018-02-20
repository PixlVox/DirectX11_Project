struct VS_IN {

	float3 pos : POSITION;
	float2 texUV : TEXCOORD;
	float3 normal : NORMAL;

};

struct VS_OUT {

	float4 pos : POSITION;
	float2 texUV : TEXCOORD;
	float3 normal : NORMAL;

};

VS_OUT VS_main(VS_IN input){

	VS_OUT output = (VS_OUT)0;

	output.pos = float4(input.pos, 1.0f);
	output.normal = input.normal;
	output.texUV = input.texUV;

	return output;

}