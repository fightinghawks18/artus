module VertexTypes;

public struct Vertex {
    [[vk::location(0)]] public float3 position;
    [[vk::location(1)]] public float2 uv;
    [[vk::location(2)]] public float4 color;
}

public struct VSOut {
    public float4 position : SV_Position;
    [[vk::location(0)]] public float2 uv;
    [[vk::location(1)]] public float4 color;
}

public struct FullscreenVSOut {
    public float4 position : SV_Position;
    [[vk::location(0)]] public float2 uv;
}