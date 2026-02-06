import VertexTypes;

[shader("vertex")]
VSOut VSMain(Vertex input) {
    VSOut output;
    output.position = float4(input.position, 1.0);
    output.uv = input.uv;
    output.color = input.color;
    return output;
}