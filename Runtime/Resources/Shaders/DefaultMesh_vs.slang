import Utils.Camera;
import Utils.Vertex;
import Utils.Model;

ParameterBlock<Camera3D> camera;
ParameterBlock<Model> model;

[shader("vertex")]
Vertex3DOut VSMain(Vertex3D input) {
    Vertex3DOut output;
    output.position = camera.convertVertex3DToScreenSpace(input, model.model);
    output.color = input.color;
    output.uv = input.uv;
    return output;
}