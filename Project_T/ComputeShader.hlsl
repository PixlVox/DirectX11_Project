struct IN {

	int color;

};

StructuredBuffer<IN> glow : register(t0);
StructuredBuffer<IN> glowMap : register(t1);
RWStructuredBuffer<IN> output : register(u0);

[numthreads(1, 1, 1)]
void CS_main( uint3 DTid : SV_DispatchThreadID ){

	output[DTid.x].color = glow[DTid.x].color;

}