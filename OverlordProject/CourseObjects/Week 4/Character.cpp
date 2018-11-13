
#include "stdafx.h"
#include "Character.h"
#include "Components.h"
#include "Prefabs.h"
#include "GameScene.h"
#include "PhysxManager.h"
#include "PhysxProxy.h"


Character::Character(float radius, float height, float moveSpeed) : 
	m_Radius(radius),
	m_Height(height),
	m_MoveSpeed(moveSpeed),
	m_pCamera(nullptr),
	m_pController(nullptr),
	m_TotalPitch(0), 
	m_TotalYaw(0),
	m_RotationSpeed(90.f),
	//Running
	m_MaxRunVelocity(50.0f), 
	m_TerminalVelocity(20),
	m_Gravity(9.81f), 
	m_RunAccelerationTime(0.3f), 
	m_JumpAccelerationTime(0.8f), 
	m_RunAcceleration(m_MaxRunVelocity/m_RunAccelerationTime), 
	m_JumpAcceleration(m_Gravity/m_JumpAccelerationTime), 
	m_RunVelocity(0), 
	m_JumpVelocity(0),
	m_Velocity(0,0,0)
{
}


Character::~Character(void)
{
}

void Character::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	// Create controller
	auto material = PhysxManager::GetInstance()->GetPhysics()->createMaterial(0.5f, 0.5f, 0.1f);
	m_pController = new ControllerComponent(material, m_Radius, m_Height );
	AddComponent(m_pController);

	//auto pCube = new CubePrefab(m_Radius, m_Height, m_Radius);
	//AddChild(pCube);

	// Add a fixed camera as child
	auto camera = new FixedCamera();
	AddChild(camera);

	// Register all Input Actions
	auto inputAction = InputAction(CharacterMovement::LEFT, InputTriggerState::Down, 'A');
	gameContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterMovement::RIGHT, InputTriggerState::Down, 'D');
	gameContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterMovement::FORWARD, InputTriggerState::Down, 'W');
	gameContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterMovement::BACKWARD, InputTriggerState::Down, 'S');
	gameContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterMovement::JUMP, InputTriggerState::Pressed, VK_SPACE, -1, XINPUT_GAMEPAD_A);
	gameContext.pInput->AddInputAction(inputAction);
}

void Character::PostInitialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	// Set the camera as active
	// We need to do this in the PostInitialize because child game objects only get initialized after the Initialize of the current object finishes
	m_pCamera = GetChild<FixedCamera>()->GetComponent<CameraComponent>();
	m_pCamera->SetActive();
}


void Character::Update(const GameContext& gameContext)
{
	// see https://stackoverflow.com/questions/21688529/binary-directxxmvector-does-not-define-this-operator-or-a-conversion
	using namespace DirectX;

	if(m_pCamera->IsActive())
	{
		//***************
		//HANDLE INPUT
	 DirectX::XMFLOAT2 move = DirectX::XMFLOAT2(0,0);
		move.y = gameContext.pInput->IsActionTriggered(CharacterMovement::FORWARD)?1.0f:0.0f;
		move.y -= gameContext.pInput->IsActionTriggered(CharacterMovement::BACKWARD)?1.0f:0.0f;
		if(move.y == 0) move.y = gameContext.pInput->GetThumbstickPosition().y;

		move.x = gameContext.pInput->IsActionTriggered(CharacterMovement::RIGHT)?1.0f:0.0f;
		move.x -= gameContext.pInput->IsActionTriggered(CharacterMovement::LEFT)?1.0f:0.0f;
		if(move.x == 0) move.x = gameContext.pInput->GetThumbstickPosition().x;

	 DirectX::XMFLOAT2 look = DirectX::XMFLOAT2(0,0);
		if(gameContext.pInput->IsMouseButtonDown(VK_LBUTTON))
		{
			auto mouseMove = gameContext.pInput->GetMouseMovement();
			look = DirectX::XMFLOAT2(static_cast<float>(mouseMove.x),static_cast<float>(mouseMove.y));
		}

		if(look.x == 0 && look.y == 0)
		{
			look = gameContext.pInput->GetThumbstickPosition(false);
		}

		//***************
		//GATHERING TRANSFORM INFO
		auto transform = GetTransform();

	 DirectX::XMVECTOR forward = XMLoadFloat3(&transform->GetForward());
	 DirectX::XMVECTOR right = XMLoadFloat3(&transform->GetRight());

		//***************
		//CAMERA ROTATION
		m_TotalYaw += look.x * m_RotationSpeed * gameContext.pGameTime->GetElapsed();
		m_TotalPitch += look.y * m_RotationSpeed * gameContext.pGameTime->GetElapsed();

		transform->Rotate(m_TotalPitch, m_TotalYaw, 0);

		//************
		//TRANSLATION

	 DirectX::XMVECTOR currDirection = (forward * move.y)+(right * move.x);
		currDirection = DirectX::XMVector3Normalize(currDirection);

		if(move.x != 0 || move.y != 0)
		{
			m_RunVelocity += m_RunAcceleration * gameContext.pGameTime->GetElapsed();		
			if(m_RunVelocity > m_MaxRunVelocity) m_RunVelocity = m_MaxRunVelocity;

			float jumpVelocity = m_Velocity.y;
			DirectX::XMStoreFloat3(&m_Velocity, currDirection * m_RunVelocity);
			m_Velocity.y = jumpVelocity;
		}
		else
		{
			m_Velocity.x = 0;
			m_Velocity.z = 0;
		}

		// handle jump
		if(!m_pController->GetCollisionFlags().isSet(physx::PxControllerCollisionFlag::eCOLLISION_DOWN))
		{
			m_JumpVelocity -= m_JumpAcceleration*gameContext.pGameTime->GetElapsed();
			if(m_JumpVelocity < -m_TerminalVelocity) m_JumpVelocity = -m_TerminalVelocity;
		}
		else if(gameContext.pInput->IsActionTriggered(CharacterMovement::JUMP))
		{
			m_JumpVelocity = 0;
			m_Velocity.y = 200;
		}
		else
		{
			m_Velocity.y = 0;
		}

		m_Velocity.y += m_JumpVelocity;
		//m_Velocity.y = 0;
		auto vecVelocity = DirectX::XMLoadFloat3(&m_Velocity);
	 DirectX::XMFLOAT3 moveAmount;

		XMStoreFloat3(&moveAmount, (vecVelocity * gameContext.pGameTime->GetElapsed()));
		
		m_pController->Move(moveAmount);
	}
}