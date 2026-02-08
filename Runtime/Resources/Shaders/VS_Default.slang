import VertexTypes;
import ShaderTypes;

[[vk::binding(0, 0)]]
ConstantBuffer<CameraData> cameraData;

[[vk::push_constant]]
ModelData model;

[shader("vertex")]
VSOut VSMain(Vertex input) {
    let viewProj = mul(cameraData.projection, cameraData.view);
    let vertexPosition = float4(input.position, 1.0);
    let worldPos = mul(model.model, vertexPosition);
    let projectedPos = mul(viewProj, worldPos);

    VSOut output;
    output.position = projectedPos;
    output.uv = input.uv;
    output.color = input.color;
    return output;
}