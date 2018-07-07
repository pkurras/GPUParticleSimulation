#define THREAD_NUM_X 64

// particle stucture
struct Particle
{
	float2 position;
	float2 prevPosition;
	float2 nextPosition;
};

// ParticlesRW as UnorderedAccessView register 0
RWStructuredBuffer<Particle> CurrentSimulationState : register(u0);
//AppendStructuredBuffer<Particle> NextSimulationState : register(u1);

// SimulationConstants as ConstantBuffer register 0
cbuffer SimulationConstants : register(b0)
{
	uint numParticles;
	float2 gravitySource;
	float gravityStrength;
	float lastTimestep;
	float timestep;
	float damping;
};

// This is the verlet integration compute shader
// TODO:	use AppendStructuredBuffer and ConsumeStructuredBuffer
//			to dynamically control the creation and destruction of particles
//			I tried it already but it didn't work and I had to revert, unfortunately. :(
[numthreads(THREAD_NUM_X, 1, 1)]
void IntegrateCS(uint3 DispatchThreadID : SV_DispatchThreadID)
{
	// uint myID = DispatchThreadID.x + DispatchThreadID.y * 512 + DispatchThreadID.z * 512 * 512;

	// Particle ID to operate on
	const unsigned int P_ID = DispatchThreadID.x;
	// Particle p = CurrentSimulationState.Consume();

	// retrieve positions from particle
	float2 prevPosition = CurrentSimulationState[P_ID].position;
	float2 position = CurrentSimulationState[P_ID].nextPosition;
	// float2 prevPosition = p.position;
	// float2 position = p.nextPosition;

	// precalculate the distance vector to the gravity source
	float2 vecDist = (gravitySource - position);
	// precalculate the delta to the last position
	float2 deltaPos = (position - prevPosition);
	// precalculate the "distance influence drop" to the gravity source
	float vecDist2 = (vecDist.x * vecDist.x + vecDist.y * vecDist.y);
	// calculate the acceleration
	float2 acceleration = normalize(vecDist) * step(0.000001, vecDist2) * gravityStrength;

	// Set the former "nextPosition" to be the current position in the next iteration
	CurrentSimulationState[P_ID].position = position;
	// p.position = position;

	// Verlet integration in one line .. for the reading fun :)
	// This implementation uses the verlet integration for non-constant time differences
	// It is the time corrected formula using taylor series in place of the
	// Störmer–Verlet method see: https://en.wikipedia.org/wiki/Verlet_integration#Non-constant_time_differences
	CurrentSimulationState[P_ID].nextPosition = position + (deltaPos * timestep / lastTimestep + (acceleration * (timestep + lastTimestep) * timestep * 0.5)) * damping;
	// p.nextPosition = position + (deltaPos * timestep / lastTimestep + (acceleration * (timestep + lastTimestep) * timestep * 0.5)) * damping;

	//NextSimulationState.Append(p);
}
