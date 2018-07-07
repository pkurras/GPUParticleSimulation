// EXTERNAL INCLUDES
#include <cmath>
// INTERNAL INCLUDES
#include "inputevent.h"
#include "particlerenderer.h"
#include "utils.h"

#define THREAD_NUM_X 64

ID3D11ShaderResourceView* gNullSRV = nullptr;
ID3D11UnorderedAccessView* gNullUAV = nullptr;
ID3D11Buffer* gNullBuffer = nullptr;
uint32 gNullUINT = 0;

ParticleRenderer::SimulationConstants simulationData = { 0 };

// Input Callbacks
MouseClickedScreenSpaceCallback OnMouseClickedScreenSpace = [](Math::Vec2 position)
{
	simulationData.gravitySource = position;
};
UpArrowPressedCallback OnUpArrowPressed = []()
{
	simulationData.gravityStrength += 0.1f;
	LOG("Gravity strength: %f", simulationData.gravityStrength);
};
DownArrowPressedCallback OnDownArrowPressed = []()
{
	simulationData.gravityStrength -= 0.1f;
	LOG("Gravity strength: %f", simulationData.gravityStrength);
};
RightArrowPressedCallback OnRightArrowPressed = []()
{
	simulationData.damping += 0.001f;
	LOG("Damping strength: %f", 1.0f - simulationData.damping);
};
LeftArrowPressedCallback OnLeftArrowPressed = []()
{
	simulationData.damping -= 0.001f;
	LOG("Damping strength: %f", 1.0f - simulationData.damping);
};

ParticleRenderer::ParticleRenderer() :
	pParticleVS(nullptr),
	pParticlePS(nullptr),
	pIndirectDrawBuffer(nullptr),
	pParticleSimulationCS(nullptr),
	pParticleCreation(nullptr),
	pSimulationBuffer(nullptr),
	pCurrentSimulationState(nullptr),
	pCurrentSimulationStateSRV(nullptr),
	pCurrentSimulationStateUAV(nullptr),
	pCurrentSimulationData(nullptr),
	pNextSimulationState(nullptr),
	pNextSimulationStateSRV(nullptr),
	pNextSimulationStateUAV(nullptr),
	pNextSimulationData(nullptr),
	numMaxParticles(50000)
{

}

ParticleRenderer::~ParticleRenderer()
{
	// Clean everything up
	SAFE_DELETE(this->pCurrentSimulationData);
	SAFE_DELETE(this->pNextSimulationData);

	SAFE_RELEASE(this->pParticlePS);
	SAFE_RELEASE(this->pParticleVS);
	SAFE_RELEASE(this->pParticleSimulationCS);

	SAFE_RELEASE(this->pSimulationBuffer);

	SAFE_RELEASE(this->pIndirectDrawBuffer);

	SAFE_RELEASE(this->pCurrentSimulationState);
	SAFE_RELEASE(this->pCurrentSimulationStateSRV);
	SAFE_RELEASE(this->pCurrentSimulationStateUAV);

	SAFE_RELEASE(this->pNextSimulationState);
	SAFE_RELEASE(this->pNextSimulationStateSRV);
	SAFE_RELEASE(this->pNextSimulationStateUAV);
}

HRESULT ParticleRenderer::SetupParticles()
{
	HRESULT hr = S_OK;

	// Make space for the particles on heap
	this->pCurrentSimulationData = new Particle[this->numMaxParticles];
	// this->pNextSimulationData = new Particle[this->numMaxParticles];

	// Set the memory to 0
	ZeroMemory(this->pCurrentSimulationData, sizeof(Particle) * this->numMaxParticles);
	// ZeroMemory(this->pNextSimulationData, sizeof(Particle) * this->numMaxParticles);

	float row = 0;
	// Iterate over all particles and set their positions
	for (size_t i = 0; i < numMaxParticles; i++)
	{
		float column = float(i % 1000);

		this->pCurrentSimulationData[i].position = { (column * 0.0009f) - 0.5f, (row * 0.0009f) - 0.5f };
		this->pCurrentSimulationData[i].prevPosition = this->pCurrentSimulationData[i].position;
		this->pCurrentSimulationData[i].nextPosition = this->pCurrentSimulationData[i].position;

		if (i % 50 == 49)
			row++;
	}

	// Compile the shaders
	V_RETURN(this->CompileShaders());

	// Create buffers on the graphics card and CPU side
	V_RETURN(this->GenerateStructuredBuffer<Particle>(
		static_cast<uint>(this->numMaxParticles),
		&this->pCurrentSimulationState,
		&this->pCurrentSimulationStateSRV,
		&this->pCurrentSimulationStateUAV,
		this->pCurrentSimulationData)
	);
	/*V_RETURN(this->GenerateAppendStructuredBuffer<Particle>(
		static_cast<uint>(this->numMaxParticles),
		&this->pCurrentSimulationState,
		&this->pCurrentSimulationStateSRV,
		&this->pCurrentSimulationStateUAV,
		this->pNextSimulationData)
	);*/
	V_RETURN(this->GenerateConstantBuffer<SimulationConstants>(&this->pSimulationBuffer));

	UINT bufferInit[4] = { (uint)this->numMaxParticles, 1, 0, 0 };
	V_RETURN(this->GenerateIndirectDrawIndirectBuffer<uint>(&pIndirectDrawBuffer, bufferInit));

	// Update simulation constants
	simulationData.numParticles = static_cast<uint>(this->numMaxParticles / THREAD_NUM_X);
	simulationData.lastTimestep = 1.0f;
	simulationData.timestep = 1.0f;
	simulationData.gravitySource = Math::Vec2{ 0.0f, 0.0f };
	simulationData.gravityStrength = 9.81f;
	simulationData.damping = 0.9948f;

	return hr;
}

void ParticleRenderer::UpdateParticles(float deltaTime)
{
	UINT UAVInitialCounts = 0;

	// Update simulation constants
	simulationData.lastTimestep = simulationData.timestep;
	/* simulationData.gravitySource = Math::Vec2{ (sin(accumulatedTime) * 0.1f), (cos(accumulatedTime) * 0.1f) }; */
	simulationData.timestep = deltaTime;

	this->pContext->UpdateSubresource(this->pSimulationBuffer, 0, NULL, &simulationData, 0, 0);

	// Setup of the compute shader
	this->pContext->CSSetShader(this->pParticleSimulationCS, NULL, 0);
	this->pContext->CSSetConstantBuffers(0, 1, &this->pSimulationBuffer);

	this->pContext->CSSetUnorderedAccessViews(0, 1, &this->pCurrentSimulationStateUAV, &UAVInitialCounts);
	// this->context->CSSetUnorderedAccessViews(0, 1, &this->pNextSimulationStateUAV, &UAVInitialCounts);

	// dispatch the compute command
	this->pContext->Dispatch(static_cast<uint>(this->numMaxParticles), 1, 1);
	// this->context->Dispatch(1, 1, 1);

	// Unset the views
	this->pContext->CSSetShader(nullptr, nullptr, 0);
	this->pContext->CSSetUnorderedAccessViews(0, 1, &gNullUAV, &UAVInitialCounts);
	// this->context->CSSetUnorderedAccessViews(1, 1, &g_nullUAV, &UAVInitialCounts);
}
void ParticleRenderer::RenderParticles(float deltaTime)
{
	// Set vertex and pixel shader
	this->pContext->VSSetShader(this->pParticleVS, NULL, 0);
	this->pContext->PSSetShader(this->pParticlePS, NULL, 0);

	// More pipeline settings
	this->pContext->VSSetShaderResources(0, 1, &this->pCurrentSimulationStateSRV);
	this->pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	// this->context->Draw(static_cast<uint>(this->numParticles), 0);
	this->pContext->DrawInstancedIndirect(pIndirectDrawBuffer, 0);

	// Unset the views
	//this->context->IASetVertexBuffers(0, 1, &g_nullBuffer, &g_nullUINT, 0);
	this->pContext->VSSetShaderResources(0, 1, &gNullSRV);
	this->pContext->VSSetShader(nullptr, NULL, 0);
	this->pContext->PSSetShader(nullptr, NULL, 0);
}

HRESULT ParticleRenderer::CompileShaders()
{
	HRESULT hr = S_OK;
	ID3DBlob* pBlob = NULL;

	// Compile Shaders from file
	V_RETURN(this->CompileShaderFromFile("ParticleRendering.hlsl", "ParticleVS", "vs_4_0", &pBlob));
	V_RETURN(this->pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &this->pParticleVS));
	SAFE_RELEASE(pBlob);

	V_RETURN(CompileShaderFromFile("ParticleRendering.hlsl", "ParticlePS", "ps_4_0", &pBlob));
	V_RETURN(this->pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &this->pParticlePS));
	SAFE_RELEASE(pBlob);

	V_RETURN(CompileShaderFromFile("ParticleSimulation.hlsl", "IntegrateCS", "cs_4_0", &pBlob));
	V_RETURN(this->pDevice->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &this->pParticleSimulationCS));
	SAFE_RELEASE(pBlob);

	/*V_RETURN(CompileShaderFromFile("ParticleCreation.hlsl", "CreateParticleCS", "cs_5_0", &pBlob));
	V_RETURN(this->device->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &this->pParticleCreation));
	SAFE_RELEASE(pBlob);*/

	return hr;
}
