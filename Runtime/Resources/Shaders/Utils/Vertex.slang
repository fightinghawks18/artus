module Utils;

public struct Vertex3D {
    [[vk_location(0)]] public float3 position;
    [[vk_location(1)]] public float2 uv;
    [[vk_location(2)]] public float4 color;
}

public struct Vertex3DOut {
    public float4 position : SV_Position;
    [[vk_location(0)]] public float2 uv;
    [[vk_location(1)]] public float4 color;
}