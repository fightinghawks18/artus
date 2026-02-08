module ShaderTypes;

public struct ModelData {
    public column_major float4x4 model;
}

public struct CameraData {
    public column_major float4x4 view;
    public column_major float4x4 projection;
}