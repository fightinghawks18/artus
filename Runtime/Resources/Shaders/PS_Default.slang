import VertexTypes;
import ShaderTypes;

[[vk::binding(0, 0)]]
ConstantBuffer<TestModelData> testModelData;

[shader("fragment")]
float4 PSMain(VSOut input) {
    return testModelData.color;
}