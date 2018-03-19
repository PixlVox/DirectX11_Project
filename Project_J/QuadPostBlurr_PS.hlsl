
Texture2D text: register(t0);
SamplerState samp : register(s0);

cbuffer cb
{
	float4 dirVec;
};

struct ps_input
{
	float4 PositionSS : SV_Position;
	float2 uvs : TEXCOORD;
};

struct ps_output
{
	float3 Texture : SV_Target;
};

//Gaussian Blurr
static float weights[5] = { 0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216 };


ps_output PS_Entry(in ps_input input)
{
	ps_output output = (ps_output)0;
	
	
	//Gaussian Blurr
	float w = 1024;
	float h = 1024;

	float xAxis = dirVec.x;
	float yAxis = dirVec.y;
	float3 pixelColor = text.Sample(samp, input.uvs) * weights[0];

	float offset_weight = 1.0f / w;
	float3 color = float3(0.0f, 0.0f, 0.0f);

	for (int i = 1; i < 5; i++)
	{
		color += text.Sample(samp, input.uvs + float2(offset_weight * i * xAxis, offset_weight * i * yAxis)) * weights[i];
		color += text.Sample(samp, input.uvs - float2(offset_weight * i * xAxis, offset_weight * i * yAxis)) * weights[i];
	}
	pixelColor += color;
	output.Texture = color;
	return output;
}