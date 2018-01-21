struct GS_IN {

	float4 pos : POSITION;
	float3 color : COLOR;

};

struct GS_OUT {

	float4 pos : SV_POSITION;
	float3 color : COLOR;

};

[maxvertexcount(6)]
void GS_main(triangle GS_IN input[3], inout TriangleStream<GS_OUT> outputStream){

	GS_OUT output = (GS_OUT)0;

	for (int i = 0; i < 3; i++){

		output.pos = input[i].pos;
		output.color = input[i].color;
		outputStream.Append(output);

	}

	outputStream.RestartStrip();

}