cbuffer MATRIX_BUFFER : register(b0) {

	Matrix wvp;

};

struct GS_IN {

	float4 pos : POSITION;
	float3 normal : NORMAL;

};

struct GS_OUT {

	float4 pos : SV_POSITION;
	float3 normal : NORMAL;

};

[maxvertexcount(3)]
void GS_main(triangle GS_IN input[3], inout TriangleStream<GS_OUT> outputStream) {

	GS_OUT output = (GS_OUT)0;

	//Store values sent from VS and pass them to the stream
	for (int i = 0; i < 3; i++) {

		//Copy over values
		input[i].pos = mul(input[i].pos, wvp);
		output.pos = input[i].pos;
		output.normal = input[i].normal;

		//Send values to stream
		outputStream.Append(output);

	}

	outputStream.RestartStrip();

}