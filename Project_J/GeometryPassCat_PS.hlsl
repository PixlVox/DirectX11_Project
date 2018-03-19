


Texture2D cat: register(t0);
SamplerState samp : register(s0);

struct ps_input
{
	float4 PositionSS : SV_Position;
	float3 NormalWS : NORMALWS;
	float4 PositionWS : POSITIONWS;
	float2 uvs : TEXCOORD;
	float4 lpos : TEXCOORD1;
};

struct ps_output
{
	float3 Normal : SV_Target0;
	float4 Position : SV_Target1;
	float3 Texture : SV_Target2;
	float4 lpos : SV_Target3;
};

ps_output PS_Entry(in ps_input input)
{
	ps_output output = (ps_output)0;

	input.NormalWS = normalize(input.NormalWS);

	output.Normal = input.NormalWS;
	output.Position = input.PositionWS;
	output.lpos = input.lpos;

	output.Texture = cat.Sample(samp, input.uvs);

	return output;
}