import VertexTypes;
import ShaderTypes;

[[vk::binding(0, 0)]]
ConstantBuffer<TestModelData> testModelData;

[[vk::push_constant]]
cbuffer PushConstants {
    float4 color;
}

[shader("fragment")]
float4 PSMain(VSOut input) {
    return testModelData.color - color;
}