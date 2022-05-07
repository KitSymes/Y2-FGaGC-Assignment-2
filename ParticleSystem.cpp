#include "ParticleSystem.h"
#include "Application.h"

ParticleSystem::ParticleSystem(Geometry geometry, Material material)
{
	_particlePool = vector<Particle*>();
	_forces = Vector3();
	_enabled = false;

	_geometry = geometry;
	_material = material;
}

ParticleSystem::~ParticleSystem()
{
	for (Particle* particle : _particlePool)
		delete particle;
	_particlePool.clear();
}

bool ParticleSystem::IsEnabled()
{
	return _enabled;
}

void ParticleSystem::Start()
{
	_enabled = true;
}

void ParticleSystem::Stop()
{
	_enabled = false;
}

Particle* ParticleSystem::GetFirstInactive()
{
	for (Particle* particle : _particlePool)
		if (!particle->IsActive())
			return particle;
	return nullptr;
}

void ParticleSystem::Update(float deltaTime)
{
	_forces = Vector3();
}

void ParticleSystem::Draw(ID3D11DeviceContext* pImmediateContext, ConstantBuffer* constantBuffer, ID3D11Buffer* pConstantBuffer)
{
	for (Particle* particle : _particlePool)
	{
		// Get render material
		Material material = particle->GetGameObject()->GetAppearance()->GetMaterial();

		// Copy material to shader
		constantBuffer->surface.AmbientMtrl = material.ambient;
		constantBuffer->surface.DiffuseMtrl = material.diffuse;
		constantBuffer->surface.SpecularMtrl = material.specular;

		// Set world matrix
		constantBuffer->World = XMMatrixTranspose(particle->GetGameObject()->GetTransform()->GetWorldMatrix());

		// Set texture
		if (particle->GetGameObject()->GetAppearance()->HasTexture())
		{
			ID3D11ShaderResourceView* textureRV = particle->GetGameObject()->GetAppearance()->GetTextureRV();
			pImmediateContext->PSSetShaderResources(0, 1, &textureRV);
			constantBuffer->HasTexture = 1.0f;
		}
		else
		{
			constantBuffer->HasTexture = 0.0f;
		}

		// Update constant buffer
		pImmediateContext->UpdateSubresource(pConstantBuffer, 0, nullptr, constantBuffer, 0, 0);

		// Draw object
		particle->Draw(pImmediateContext);
	}
}

Particle* ParticleSystem::CreateParticle()
{
	Particle* particle = new Particle(_geometry, _material);
	return particle;
}
