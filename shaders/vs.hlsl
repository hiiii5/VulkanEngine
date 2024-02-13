// an ultra simple hlsl vertex shader
#pragma pack_matrix(row_major)

[[vk::push_constant]]
cbuffer MESH_INDEX {
	uint mesh_ID;
};

struct OBJ_ATTRIBUTES {
	float3    Kd; // diffuse reflectivity
	float	    d; // dissolve (transparency) 
	float3    Ks; // specular reflectivity
	float       Ns; // specular exponent
	float3    Ka; // ambient reflectivity
	float       sharpness; // local reflection map sharpness
	float3    Tf; // transmission filter
	float       Ni; // optical density (index of refraction)
	float3    Ke; // emissive reflectivity
	uint    illum; // illumination model
};

struct SHADER_MODEL_DATA {
	float4 sunDirection;
	float4 sunColor;
    float4 eyePos;
	matrix viewMatrix;
	matrix projectionMatrix;
	matrix matricies[1024];
	OBJ_ATTRIBUTES attributes[1024];
};

StructuredBuffer<SHADER_MODEL_DATA> shaderData;

struct OUTPUT_TO_RASTERIZER {
	float4 posH : SV_POSITION;
	float4 nrmW : NORMAL;
	float4 posW : WORLD;
    float4 view : VIEW_DIR;
};

OUTPUT_TO_RASTERIZER main(float4 pos : POSITION, float2 uv : UV, float4 norm)
{
    matrix model = shaderData[0].matricies[mesh_ID];
    matrix view = shaderData[0].viewMatrix;
    matrix projection = shaderData[0].projectionMatrix;
    float4 v = float4(pos.xyz, 1);
    OUTPUT_TO_RASTERIZER output = (OUTPUT_TO_RASTERIZER)0;

    // TODO: Part 1h
    matrix modelView = mul(model, view);
	matrix modelViewProjection = mul(modelView, projection);

	// TODO: Part 2i
	// Transform the vertex into model view projection.
    output.posH = mul(v, modelViewProjection);

	// TODO: Part 4e
	// TODO: Part 4b
	// TODO: Part 4e
    output.nrmW = mul(float4(norm.xyz, 0.0f), model);
    output.posW = mul(v, model);
    output.view = normalize(shaderData[0].eyePos - output.posW);

	return output;
}