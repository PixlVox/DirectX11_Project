cbuffer MATRIX_BUFFER : register(b0) {

	Matrix wvp;
	Matrix world;

};

struct GS_IN {

	float4 pos : POSITION;
	float2 texUV : TEXCOORD;
	float3 normal : NORMAL;

};

struct GS_IN2 {

	float4 pos : POSITION;
	float3 col : COLOR;
	float3 normal : NORMAL;

};

struct GS_OUT {

	float4 pos : SV_POSITION;
	float3 worldPos : POSITION;
	float2 texUV : TEXCOORD;
	float3 normal : NORMAL;

};

struct GS_OUT2 {

	float4 pos : SV_POSITION;
	float3 col : COLOR;
	float3 normal : NORMAL;

};

[maxvertexcount(3)]
void GS_main(triangle GS_IN input[3], inout TriangleStream<GS_OUT> outputStream) {

	GS_OUT output = (GS_OUT)0;

	//Store values sent from VS and pass them to the stream
	for (int i = 0; i < 3; i++) {

		//Copy over values
		output.pos = mul(input[i].pos, wvp);
		output.worldPos = mul(input[i].pos, world);
		output.texUV = input[i].texUV;
		output.normal = input[i].normal;

		//Send values to stream
		outputStream.Append(output);

	}

	outputStream.RestartStrip();

}

[maxvertexcount(3)]
void GS_main_Color(triangle GS_IN2 input[3], inout TriangleStream<GS_OUT2> outputStream) {

	GS_OUT2 output = (GS_OUT2)0;

	//Store values sent from VS and pass them to the stream
	for (int i = 0; i < 3; i++) {

		//Copy over values
		output.pos = mul(input[i].pos, wvp);
		output.col= input[i].col;
		output.normal = input[i].normal;

		//Send values to stream
		outputStream.Append(output);

	}

	outputStream.RestartStrip();

}