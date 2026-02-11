import Utils.Vertex;
import Utils.FullscreenQuad;

[shader("vertex")]
Vertex3DOut VSMain(uint vertexID : SV_VertexID) {
    return createFullscreenQuad(vertexID);
}