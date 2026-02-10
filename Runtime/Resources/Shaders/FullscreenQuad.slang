import Utils.Vertex;

[shader("vertex")]
Vertex3DOut VSMain(uint vertexID : SV_VertexID) {
    let uv = float2((vertexID << 1) & 2, vertexID & 2);

    Vertex3DOut output;
    output.uv = uv;
    output.position = float4(uv * 2.0 - 1.0, 0.0, 1.0);
    return output;
}