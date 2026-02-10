import Utils.Vertex;

[shader("fragment")]
float4 PSMain(Vertex3DOut input) {
    return input.color;
}