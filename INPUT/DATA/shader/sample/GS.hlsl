struct VS_OUTPUT
{
	float4 p : SV_POSITION;
	float4 c : COLOR;
};

[maxvertexcount(9)]
void GS(triangle VS_OUTPUT input[3], inout TriangleStream<VS_OUTPUT> TriStream)
{
	VS_OUTPUT output;

	float3 a = (input[0].p + input[1].p) / 2.0;
	float3 b = (input[1].p + input[2].p) / 2.0;
	float3 c = (input[2].p + input[0].p) / 2.0;

	//트라이앵글 리스트는 마지막 이전 삼각형의 마지막 정점을 다음 삼각형의 첫번째 정점으로 사용한다.

	output.p = float4(c, 1);
	output.c = float4(1, 1, 1, 1);
	TriStream.Append(output);

	output.p = input[0].p;
	output.c = float4(1, 1, 1, 1);
	TriStream.Append(output);

	output.p = float4(a, 1);
	output.c = float4(1, 1, 1, 1);
	TriStream.Append(output);

	output.p = input[1].p;
	output.c = float4(1, 1, 1, 1);
	TriStream.Append(output);

	output.p = float4(b, 1);
	output.c = float4(1, 1, 1, 1);
	TriStream.Append(output);

	output.p = input[2].p;
	output.c = float4(1, 1, 1, 1);
	TriStream.Append(output);

	output.p = float4(c, 1);
	output.c = float4(1, 1, 1, 1);
	TriStream.Append(output);

	output.p = float4(a, 1);
	output.c = float4(1, 1, 1, 1);
	TriStream.Append(output);

	output.p = float4(b, 1);
	output.c = float4(1, 1, 1, 1);
	TriStream.Append(output);

	TriStream.RestartStrip();
}