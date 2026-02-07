import VertexTypes;

[shader("vertex")]
VSOut VSMain(Vertex input) {
    VSOut output;
    output.position = float4(float3(input.position, 0.0), 1.0);
    output.uv = input.uv;
    output.color = input.color;
    return output;
}