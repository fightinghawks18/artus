import Utils.Camera;
import Utils.Vertex;

ParameterBlock<Camera3D> camera;

struct PushConstants {
    column_major float4x4 model;
}

[shader("vertex")]
Vertex3DOut VSMain(uniform PushConstants constants, Vertex3D input) {
    Vertex3DOut output;
    output.position = camera.convertVertex3DToScreenSpace(input, constants.model);
    output.color = input.color;
    output.uv = input.uv;
    return output;
}