#pragma once
#include "BaseComponent.h"

struct ParticleEmitterSettings
{
	ParticleEmitterSettings():
		MinSize(0.1f),
		MaxSize(2.0f),
		MinEnergy(1.0f),
		MaxEnergy(2.0f),
		Velocity(DirectX::XMFLOAT3(0, 0, 0)),
		MaxEmitterRange(10.0f),
		MinEmitterRange(9.0f),
		MinSizeGrow(1.0f),
		MaxSizeGrow(1.0f),
		Color(static_cast<DirectX::XMFLOAT4>(DirectX::Colors::White))
	{
	}

	//The minimum size each particle can be at the time when it is spawned
	float MinSize;
	//The maximum size each particle can be at the time when it is spawned
	float MaxSize;
	//The minimum lifetime of each particle, measured in seconds
	float MinEnergy;
	//The maximum lifetime of each particle, measured in seconds
	float MaxEnergy;
	//The starting speed of particles along X, Y and Z, measured in the object's orientation
	DirectX::XMFLOAT3 Velocity;
	//The maximum radius that the particles are spawned in
	float MaxEmitterRange;
	//The minimum radius that the particles are spawned in
	float MinEmitterRange;
	//The percentual maximum change in size during the particle's lifetime
	float MinSizeGrow;
	//The percentual minimum change in size during the particle's lifetime
	float MaxSizeGrow;
	//The color of a particle
	DirectX::XMFLOAT4 Color;
};

struct ParticleVertex
{
	ParticleVertex(const DirectX::XMFLOAT3 pos = DirectX::XMFLOAT3(0, 0, 0),
		const DirectX::XMFLOAT4 col = static_cast<DirectX::XMFLOAT4>(DirectX::Colors::White), 
		const float size = 5.0f, const float rotation = 0) :
		Position(pos),
		Color(col),
		Size(size),
		Rotation(rotation)
	{
	}

	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT4 Color;
	float Size;
	float Rotation;
};

class Particle;
class TextureData;

class ParticleEmitterComponent : public BaseComponent
{
	//The actual vertex buffer, containing ParticleVertex information for each Particle
	ID3D11Buffer* m_pVertexBuffer;

	//The effect we'll use to render the particles
	ID3DX11Effect* m_pEffect;
	//The default technique
	ID3DX11EffectTechnique* m_pDefaultTechnique{};
	//Shader matrix variables
	ID3DX11EffectMatrixVariable *m_pWvpVariable{}, *m_pViewInverseVariable{};
	//Shader texture variable
	ID3DX11EffectShaderResourceVariable* m_pTextureVariable{};
	//ShaderResourceView, containing the particle texture
	TextureData* m_pParticleTexture;

	//Vertex input layout variables
	ID3D11InputLayout* m_pInputLayout;
	UINT m_pInputLayoutSize;

	//Vector of particle pointers
	std::vector<Particle*> m_Particles;
	//The settings for this particle system
	ParticleEmitterSettings m_Settings;
	//The total amount of particles (m_Particles.size() == m_ParticleCount)
	int m_ParticleCount;
	//The active particles for a certain frame
	int m_ActiveParticles;
	//Total seconds since the last particle initialisation
	float m_LastParticleInit;
	//string containing the path to the particle texture
	std::wstring m_AssetFile;
	TextureStruct* m_TextureStruct = nullptr;


	//Method to load effect-related stuff
	void LoadEffect(const GameContext& gameContext);
	//Method to create the vertex buffer
	void CreateVertexBuffer(const GameContext& gameContext);

	void WriteToFile(BinaryWriter*) override;
public:
	ParticleEmitterComponent(const ParticleEmitterComponent& other) = delete;
	ParticleEmitterComponent(ParticleEmitterComponent&& other) noexcept = delete;
	ParticleEmitterComponent& operator=(const ParticleEmitterComponent& other) = delete;
	ParticleEmitterComponent& operator=(ParticleEmitterComponent&& other) noexcept = delete;
	//Constructor, particle texture path and max particles
	ParticleEmitterComponent(std::wstring assetFile, int particleCount = 50);
	ParticleEmitterComponent(TextureStruct* texture, int particleCount = 50);

	~ParticleEmitterComponent();

	//EmitterSettings SETTERS
	void SetTextureStruct(TextureStruct* texture);
	TextureStruct* GetTextureStruct() { return m_TextureStruct; }

	void SetMinSize(float minSize) { m_Settings.MinSize = minSize; }
	float GetMinSize(){return m_Settings.MinSize;}

	void SetMaxSize(float maxSize) { m_Settings.MaxSize = maxSize; }
	float GetMaxSize() {return m_Settings.MaxSize; }

	void SetMinEnergy(float minEnergy) { m_Settings.MinEnergy = minEnergy; }
	float GetMinEngergy() { return m_Settings.MinEnergy; }

	void SetMaxEnergy(float maxEnergy) { m_Settings.MaxEnergy = maxEnergy; }
	float GetMaxEngergy() { return m_Settings.MaxEnergy; }


	void SetVelocity(DirectX::XMFLOAT3 velocity) { m_Settings.Velocity = velocity; }
	DirectX::XMFLOAT3 GetVelocityEngergy() { return m_Settings.Velocity; }


	void SetMinSizeGrow(float minSizeGrow) { m_Settings.MinSizeGrow = minSizeGrow; }
	float GetminSizeGrow() { return m_Settings.MinSizeGrow; }


	void SetMaxSizeGrow(float maxSizeGrow) { m_Settings.MaxSizeGrow = maxSizeGrow; }
	float GetMaxSizeGrow() { return m_Settings.MaxSizeGrow; }


	void SetMinEmitterRange(float minEmitterRange) { m_Settings.MinEmitterRange = minEmitterRange; }
	float GetMinEmmiterRange() { return m_Settings.MinEmitterRange; }


	void SetMaxEmitterRange(float maxEmitterRange) { m_Settings.MaxEmitterRange = maxEmitterRange; }
	float GetMaxEmmiterRange() { return m_Settings.MaxEmitterRange; }


	void SetColor(DirectX::XMFLOAT4 color) { m_Settings.Color = color; }
	DirectX::XMFLOAT4 GetColor() { return m_Settings.Color; }

	void SetParticleCount(int count);
	int GetParticleCount() { return m_ParticleCount; }
protected:
	void Initialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;
	void Draw(const GameContext& gameContext) override;
	void PostDraw(const GameContext& gameContext) override;

	bool m_ParticleCountChanged = false;
};
