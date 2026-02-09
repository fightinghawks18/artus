import VertexTypes;

[shader("vertex")]
FullscreenVSOut VSMain(uint vertexID : SV_VertexID) {
    FullscreenVSOut output;
    float2 uv = float2((vertexID << 1) & 2, vertexID & 2);
    output.uv = uv;
    output.position = float4(uv * 2.0 - 1.0, 0.0, 1.0);
    return output;
}