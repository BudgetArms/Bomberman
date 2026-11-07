#include "ForceDamageCommand.hpp"

#include "Core/Logger.hpp"

#include "Components/LifeComponent.hpp"


using namespace Game;


ForceDamageCommand::ForceDamageCommand(bae::GameObject& actor) :
    GameActorCommand(actor)
{
}

void ForceDamageCommand::Execute()
{
    if(!GetActor())
    {
        return;
    }

    const auto lifeComponent = GetActor()->GetComponent<LifeComponent>();
    lifeComponent->RemoveLife();
    bae::Logger::LogNormal("Removed one life, lives left: " + std::to_string(lifeComponent->GetLives()));
}

