module GeometryUtils;

public float4 VertexToScreenSpace(column_major float4x4 model, column_major float4x4 view, column_major float4x4 projection, float4 vertex) {
    let worldSpace = mul(model, vertex);
    let viewSpace = mul(view, worldSpace);
    let screenPosition = mul(projection, viewSpace);
    return screenPosition;
}