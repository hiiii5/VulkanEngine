#pragma pack_matrix(row_major)

[[vk::push_constant]] cbuffer MESH_INDEX { uint mesh_ID; };

struct OBJ_ATTRIBUTES {
  float3 Kd;       // diffuse reflectivity
  float d;         // dissolve (transparency)
  float3 Ks;       // specular reflectivity
  float Ns;        // specular exponent
  float3 Ka;       // ambient reflectivity
  float sharpness; // local reflection map sharpness
  float3 Tf;       // transmission filter
  float Ni;        // optical density (index of refraction)
  float3 Ke;       // emissive reflectivity
  uint illum;      // illumination model
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

float3 lambert(float3 lightColor, float3 lightDir, float3 nrm)
{
    // Get the inverse normalized light direction.
    const float3 lightNormal = -normalize(lightDir);

    // Make sure the input normal is normalized.
    const float3 normalizedNormal = normalize(nrm);

    // Determine how much light is reflected off of the diffuse from the light direction.
    float lightDiffuseNormal = dot(lightNormal, normalizedNormal);

    // Don't let anything fall out of the minimum range.
    lightDiffuseNormal = max(lightDiffuseNormal, 0);

    // Colorize the lighting with the light color.
    return lightDiffuseNormal * lightColor;
}

float specular(float3 lightColor, float3 lightDir, float3 nrmW, float3 view, float specularExp) {
	// Calculate specular lighting
    const float3 r = reflect(normalize(lightDir), normalize(nrmW));

    // Get the amount of light reflected back to the camera.
    float spec = pow(saturate(dot(r, normalize(view))), specularExp);

    // Ensure spec doesn't fall below 0, causing incorrect lighting.
    spec = max(spec, 0);

    // Colorize the specular lighting by the light color.
    return lightColor * spec;
}

// an ultra simple hlsl pixel shader
float4 main(float4 posH : SV_POSITION, float4 nrmW : NORMAL, float4 posW : WORLD, float4 view : VIEW_DIR) : SV_TARGET {
	 // TODO: Part 1a
    const float3 ambientColor = float3(0.25f, 0.25f, 0.35f);
    const float3 diffuse = shaderData[0].attributes[mesh_ID].Kd;
    const float3 specReflectance = shaderData[0].attributes[mesh_ID].Ks;
    const float3 ambientReflectance = shaderData[0].attributes[mesh_ID].Ka;
    const float specularExp = shaderData[0].attributes[mesh_ID].Ns;

    // Calculate lambert lighting 
    float3 lamb = lambert(shaderData[0].sunColor.rgb, shaderData[0].sunDirection.xyz, nrmW.xyz);

    // Calculate specular lighting.
    const float3 spec = specular(shaderData[0].sunColor.rgb, 
		shaderData[0].sunDirection.xyz, 
		nrmW.xyz, 
		view.xyz,specularExp);

    float3 final = diffuse * ambientColor * ambientReflectance; // Add ambient light.
	final += diffuse * lamb; // Add lambert diffuse lighting.
    final += specReflectance * spec; // Add specular reflectance lighting.

    return float4(final, 1); // Convert final to a float4.
	// TODO: Part 3a
	// TODO: Part 4c
	// TODO: Part 4g (half-vector or reflect method your choice)
}