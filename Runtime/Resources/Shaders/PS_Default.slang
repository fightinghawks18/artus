import VertexTypes;
import ShaderTypes;

[shader("fragment")]
float4 PSMain(VSOut input) {
    return input.color;
}