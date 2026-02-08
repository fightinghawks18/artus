import VertexTypes;
import ShaderTypes;
import GeometryUtils;

[[vk::binding(0, 0)]]
ConstantBuffer<CameraData> cameraData;

[[vk::push_constant]]
ModelData model;

[shader("vertex")]
VSOut VSMain(Vertex input) {
    let vertexPosition = float4(input.position, 1.0);
    VSOut output;
    output.position = VertexToScreenSpace(model.model, cameraData.view, cameraData.projection, vertexPosition);
    output.uv = input.uv;
    output.color = input.color;
    return output;
}