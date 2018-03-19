
struct invs
{
	float3 Position : POSITION;
	float2 uvs : TEXCOORD;
};

struct vsout
{
	float4 position : SV_Position;
	float2 uvs : TEXCOORD;
};

vsout VS_Entry(in invs input)
{
	vsout output = (vsout)0;

	output.position = float4(input.Position, 1.0f);
	output.uvs = input.uvs;

	return output;
}