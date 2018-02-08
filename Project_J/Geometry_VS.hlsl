
struct vs_input
{
	float3 Position : POSITION;
	float3 Normal : NORMAL;
};

struct vs_output
{
	float4 Position : SV_Position;
	float3 Normal : NORMAL;
};

vs_output VS_Entry(in vs_input input)
{
	vs_output output = (vs_output)0;

	output.Normal = input.Normal;
	output.Position = float4(input.Position, 1.0f);
	
	//pos = mul(pos, world);
	//pos = mul(pos, view);
	//pos = mul(pos, projection);


	//output.PositionWS = float4(input.Position, 1.0f);
	return output;
}
