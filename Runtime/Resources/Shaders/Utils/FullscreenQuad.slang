module Utils;
import Vertex;

public Vertex3DOut createFullscreenQuad(uint vertexID) {
    let uv = float2((vertexID << 1) & 2, vertexID & 2);

    Vertex3DOut output;
    output.uv = uv;
    output.position = float4(uv * 2.0 - 1.0, 0.0, 1.0);
    output.color = float4(1.0, 0.0, 0.0, 1.0);
    return output;
}