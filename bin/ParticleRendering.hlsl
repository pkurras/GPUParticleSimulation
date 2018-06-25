// particle stucture
struct Particle
{
	float2 position;
	float2 prevPosition;
	float2 nextPosition;
};

// ParticlesRO as ShaderResourceView register 0
StructuredBuffer<Particle> ParticlesRO : register(t0);

// VertexShader output structure
struct VS_OUTPUT
{
    float4 position : SV_Position;
    float4 color : COLOR;
};

// This method is the main vertex shader
VS_OUTPUT ParticleVS(uint ID : SV_VertexID)
{
	VS_OUTPUT output;

	// extract the particles position
	output.position = float4(ParticlesRO[ID].position, 0, 1);

	// some screen space color because otherwise it's lame :)
	output.color = float4(
		sin(1.0 - ParticlesRO[ID].position.x),
		cos(1.0 - ParticlesRO[ID].position.y),
		sin(ParticlesRO[ID].position.x),
		1.0);

    return output;
}

// Pixel Shader to RenderTarget
float4 ParticlePS(VS_OUTPUT In) : SV_Target
{
	// just use the vertex color
    return In.color;
}