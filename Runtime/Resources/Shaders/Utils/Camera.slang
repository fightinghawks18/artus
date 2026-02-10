module Utils;
import Vertex;

public interface ICamera {
    public float4x4 convertVertex3DToScreenSpace(Vertex3D vertex, column_major float4x4 model);
}

public struct Camera3D {
    public column_major float4x4 view;
    public column_major float4x4 projection;

    public float4 convertVertex3DToScreenSpace(Vertex3D vertex, column_major float4x4 model) {
        let vertexPosition = float4(vertex.position, 1.0);
        let worldSpace = mul(model, vertexPosition);
        let viewSpace = mul(view, worldSpace);
        let screenPosition = mul(projection, viewSpace);
        return screenPosition;
    }
}