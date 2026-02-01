struct Vertex {
    float3 position;
    float2 uv;
    float4 color;
}

struct VSOut {
    float3 position : SV_Position;
    float2 uv;
    float4 color;
}

[shader("vertex")]
VSOut VSMain(Vertex input) {
    VSOut output;
    output.position = input.position;
    output.uv = input.uv;
    output.color = input.color;
    return output;
}

[shader("fragment")]
float4 PSMain(VSOut input) {
    return float4(0, 0, 0, 1);
}