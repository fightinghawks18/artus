import VertexTypes;
import ShaderTypes;

[[vk::binding(0, 0)]]
ConstantBuffer<CameraData> cameraData;

[[vk::push_constant]]
ModelData model;

[shader("vertex")]
VSOut VSMain(Vertex input) {
    let vertexPosition = float4(input.position, 1.0);
    let worldSpace = mul(model.model, vertexPosition);
    let viewSpace = mul(cameraData.view, worldSpace);
    let screenPosition = mul(cameraData.projection, viewSpace);

    VSOut output;
    output.position = screenPosition;
    output.uv = input.uv;
    output.color = input.color;
    return output;
}