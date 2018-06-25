struct Particle
{
	float2 position;
	float2 prevPosition;
	float2 nextPosition;
}; 

AppendStructuredBuffer<Particle> CurrentSimulationState : register(u0);

cbuffer ParticleEmitter
{
	float4 EmitterLocation;
};

// This method is the particle generator
// TODO:	implement the functionality for append and consume
//			of particles. This method is currently unused
[numthreads(8, 1, 1)]
void CreateParticleCS(uint3 GroupThreadID : SV_GroupThreadID)
{
	Particle p;

	// Initialize positions to the current emitter location
	p.position = EmitterLocation.xyz;
	p.prevPosition = EmitterLocation.xyz;
	p.nextPosition = EmitterLocation.xyz;

	// Append the new particle to the output buffer
	CurrentSimulationState.Append(p);
}