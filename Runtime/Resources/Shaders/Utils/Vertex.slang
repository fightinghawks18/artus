module Utils;

public struct Vertex3D {
    public float3 position;
    public float2 uv;
    public float4 color;
}

public struct Vertex3DOut {
    public float4 position : SV_Position;
    public float2 uv;
    public float4 color;
}