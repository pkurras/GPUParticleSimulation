#pragma once

// EXTERNAL INCLUDES
// INTERNAL INCLUDES
#include "math/mat4x4.h"
#include "renderer.h"
#include "types.h"

class ParticleRenderer : public Renderer
{
public:

	struct Particle
	{
		Math::Vec2 position;
		Math::Vec2 prevPosition;
		Math::Vec2 nextPosition;
	};
	__declspec(align(16)) struct SimulationConstants
	{
		uint numParticles;
		Math::Vec2 gravitySource;
		float gravityStrength;
		float lastTimestep;
		float timestep;
		float damping;
	};

	ParticleRenderer();
	~ParticleRenderer();

	HRESULT SetupParticles();
	void UpdateParticles(float deltaTime);
	void RenderParticles(float deltaTime);

private:

	HRESULT CompileShaders();

	// Shaders
	ID3D11VertexShader * pParticleVS;
	ID3D11PixelShader* pParticlePS;

	ID3D11ComputeShader* pParticleSimulationCS;
	ID3D11ComputeShader* pParticleCreation;

	ID3D11Buffer* pSimulationBuffer;

	ID3D11Buffer* pIndirectDrawBuffer;

	// Structured Buffers
	ID3D11Buffer* pCurrentSimulationState;
	ID3D11ShaderResourceView* pCurrentSimulationStateSRV;
	ID3D11UnorderedAccessView* pCurrentSimulationStateUAV;

	ID3D11Buffer* pNextSimulationState;
	ID3D11ShaderResourceView* pNextSimulationStateSRV;
	ID3D11UnorderedAccessView* pNextSimulationStateUAV;

	size_t numMaxParticles;
	Particle* pCurrentSimulationData;
	Particle* pNextSimulationData;

};
