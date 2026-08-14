#include "FireComponent.hpp"

#include "TemporaryWallComponent.hpp"
#include "Base/CommonManagerVariables.hpp"
#include "Components/SpriteComponent.hpp"
#include "Core/Subject.hpp"

#include "Base/Events.hpp"
#include "Components/LifeComponent.hpp"
#include "Core/Scene.hpp"
#include "Managers/LevelManager.hpp"
#include "Singletons/GameTime.hpp"


using namespace Game;


FireComponent::FireComponent(bae::GameObject& owner, bae::GameObject& instigatorObject) :
    Component(owner),
    m_Instigator{ &instigatorObject }
{
    // Add Sprite
    m_Owner->AddComponent<bae::SpriteComponent>(*m_Owner, m_FireCenterTexturePath, SDL_FRect(0, 0, 32, 16),
                                                m_SpriteNrColumns, m_SpriteNrSprites);

    m_SpriteComponent = m_Owner->GetComponent<bae::SpriteComponent>();

    // Add Hitbox
    const glm::vec2 offset = -m_HitboxDimension / 2.f;
    m_Owner->AddComponent<HitboxComponent>(*m_Owner, m_HitboxDimension, offset);
    m_Owner->GetComponent<HitboxComponent>()->SetVisibility(true);
    m_Owner->GetComponent<HitboxComponent>()->AddObserver(this);

    AddFireChildren();
}

void FireComponent::Update()
{
    m_ElapsedTime += bae::GameTime::GetInstance().GetDeltaTime();
    if(m_ElapsedTime >= m_FireDuration)
    {
        const auto hitboxComp               = m_Owner->GetComponent<HitboxComponent>();
        hitboxComp->m_bAreCollisionsEnabled = false;

        GetOwner()->Destroy();
    }
}

void FireComponent::Notify(const unsigned eventHash, bae::Subject*, const std::any& eventData)
{
    if(GetEvent(eventHash) != Events::CollisionEvent)
    {
        return;
    }

    if(!eventData.has_value())
    {
        throw std::runtime_error(FUNCTION_NAME + std::string(" Failed to Get EventData"));
    }

    const auto otherHitbox = std::any_cast<HitboxComponent*>(eventData);
    if(!otherHitbox)
    {
        throw std::runtime_error(FUNCTION_NAME + std::string(" Failed! Invalid EventData GameObject!"));
    }

    HandleCollision(*otherHitbox);
}

void FireComponent::HandleCollision(const HitboxComponent& otherHitboxComponent)
{
    // Checks and handles it, if it's player
    HandleIfPlayerCollision(otherHitboxComponent);
    HandleIfTemporaryWallCollision(otherHitboxComponent);
}

void FireComponent::HandleIfPlayerCollision(const HitboxComponent& otherHitboxComponent)
{
    const bae::GameObject* otherGameObject = otherHitboxComponent.GetGameObject();

    std::vector<std::pair<bae::GameObject*, PlayerType>> playersData =
            LevelManager::GetInstance().GetPlayers();

    auto GetPlayerObject = [&]() -> bae::GameObject*
    {
        for(bae::GameObject* playerObject : playersData | std::views::keys)
        {
            if(playerObject == otherGameObject)
            {
                return playerObject;
            }
        }

        return nullptr;
    };

    bae::GameObject* playerObject = GetPlayerObject();
    if(!playerObject)
    {
        return;
    }

    std::cout << FUNCTION_NAME << " Fire" << '\n';
    playerObject->GetComponent<LifeComponent>()->RemoveLife();
}

void FireComponent::HandleIfTemporaryWallCollision(const HitboxComponent& otherHitboxComponent)
{
    bae::GameObject* otherGameObject             = otherHitboxComponent.GetGameObject();
    const auto gridComponent                     = LevelManager::GetInstance().GetGridComponent();
    const bae::Graphs::GridPosition gridPosition = gridComponent->GetGridPosition(otherGameObject->GetWorldLocation());


    if(otherGameObject->GetComponent<TemporaryWallComponent>())
    {
        LevelManager::GetInstance().GetGridComponent()->SetNodeType(gridPosition, LevelNodeType::Nothing);

        otherGameObject->Destroy();
    }
}

void FireComponent::AddFireChild(const Direction directionFire, const int childFireRange)
{
    if(childFireRange == 0)
    {
        std::cout << FUNCTION_NAME << " Failed, child Fire Range shouldn't be 0" << '\n';
        return;
    }

    const auto childFire = std::make_shared<bae::GameObject>("Child Fire");

    // Add Sprite
    childFire->AddComponent<bae::SpriteComponent>(*childFire, m_FireTexturePath, SDL_FRect(0, 0, 32, 16),
                                                  m_SpriteNrColumns, m_SpriteNrSprites);

    // Add Hitbox
    const glm::vec2 offset = -m_HitboxDimension / 2.f;
    childFire->AddComponent<HitboxComponent>(*childFire, m_HitboxDimension, offset);
    childFire->GetComponent<HitboxComponent>()->AddObserver(this);

    switch(directionFire)
    {
        case Direction::Right:
            childFire->SetLocalLocation({ childFireRange * m_GridSize, 0.f });
            break;
        case Direction::Left:
            childFire->SetLocalLocation({ -childFireRange * m_GridSize, 0.f });
            break;
        case Direction::Up:
            childFire->SetLocalRotation(90.f);
            childFire->SetLocalLocation({ 0.f, -childFireRange * m_GridSize });
            break;
        case Direction::Down:
            childFire->SetLocalRotation(90.f);
            childFire->SetLocalLocation({ 0.f, childFireRange * m_GridSize });
            break;
    }

    m_Owner->AttachChild(childFire.get(), false, false, false);

    bae::Scene* const scene = bae::SceneManager::GetInstance().GetScene(g_LevelBackgroundName.data());
    scene->Add(childFire);
}

void FireComponent::AddFireChildren()
{
    GridComponent* gridComp                        = LevelManager::GetInstance().GetGridComponent();
    const bae::Graphs::GridPosition centerPosition = gridComp->GetClosestValidNodePosition(m_Owner->GetWorldLocation());
    const int fireRange                            = LevelManager::GetInstance().GetBombFireRange();


    for(const bae::Graphs::GridPosition directionGridPos : m_Directions)
    {
        const Direction currentDirection = GetDirectionFromGridPos(directionGridPos);

        bae::Graphs::GridPosition childGridPos = centerPosition;
        for(int i = 1; i <= fireRange; ++i)
        {
            childGridPos.Column += directionGridPos.Column;
            childGridPos.Row    += directionGridPos.Row;

            // If Node doesn't exist, aka permanent wall, skip
            if(!gridComp->IsValidGridPosition(childGridPos))
            {
                break;
            }

            AddFireChild(currentDirection, i);
        }
    }
}

Direction FireComponent::GetDirectionFromGridPos(const bae::Graphs::GridPosition& gridPos)
{
    if(gridPos.Column == 1 && gridPos.Row == 0)
    {
        return Direction::Right;
    }

    if(gridPos.Column == -1 && gridPos.Row == 0)
    {
        return Direction::Left;
    }

    if(gridPos.Column == 0 && gridPos.Row == -1)
    {
        return Direction::Up;
    }

    if(gridPos.Column == 0 && gridPos.Row == 1)
    {
        return Direction::Down;
    }


    throw std::runtime_error(FUNCTION_NAME + std::string(" Failed, Invalid grid direction!"));
}

