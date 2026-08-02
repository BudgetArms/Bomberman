#include "GameModeSelectionState.hpp"

#include "Components/TextComponent.hpp"
#include "Core/Renderer.hpp"
#include "Core/Scene.hpp"
#include "Managers/SceneManager.hpp"

#include "Base/CommonManagerVariables.hpp"
#include "Base/Events.hpp"
#include "Managers/LevelManager.hpp"
#include "States/UI/GameState.hpp"
#include "States/UI/MainMenuState.hpp"


using namespace Game::States;


GameModeSelectionState::GameModeSelectionState(bae::GameObject& selectionObject) :
    SceneState(selectionObject)
{
}

void GameModeSelectionState::OnEnter()
{
    ClearScene();
    ClearCommands();

    AddSelectionConfirmedCommands();
    AddSelectionDownUpCommands();


    auto* gameModeSelectionScene = bae::SceneManager::GetInstance().GetScene(g_ScenesManagerSceneName.data());

    bae::Renderer::GetInstance().SetBackgroundColor(bae::Utils::Color::Black);

    const bae::WindowSize windowSize = bae::Renderer::GetInstance().GetSDLWindowSize();

    const auto gameModeSelectionObject = std::make_shared<bae::GameObject>("GameMode Selection");
    gameModeSelectionObject->SetWorldLocation({
        static_cast<float>(windowSize.Width) / 2.f, static_cast<float>(windowSize.Height) / 2.f
    });

    const auto singlePlayerObject = std::make_shared<bae::GameObject>("SinglePlayer");
    const auto coOpObject         = std::make_shared<bae::GameObject>("CoOp");
    const auto versusObject       = std::make_shared<bae::GameObject>("Versus");
    const auto backObject         = std::make_shared<bae::GameObject>("Back");

    constexpr float verticalPadding = 200.f;

    singlePlayerObject->AddLocation({ 0, -3.f * verticalPadding / 4.f });
    coOpObject->AddLocation({ 0, -verticalPadding / 4.f });
    versusObject->AddLocation({ 0, verticalPadding / 4.f });
    backObject->AddLocation({ 0, 3.f * verticalPadding / 4.f });

    // AttachChild, but don't freeze position
    gameModeSelectionObject->AttachChild(singlePlayerObject.get(), false);
    gameModeSelectionObject->AttachChild(coOpObject.get(), false);
    gameModeSelectionObject->AttachChild(versusObject.get(), false);
    gameModeSelectionObject->AttachChild(backObject.get(), false);

    singlePlayerObject->AddComponent<bae::TextComponent>(*singlePlayerObject, "SinglePlayer");
    coOpObject->AddComponent<bae::TextComponent>(*coOpObject, "Co-Op");
    versusObject->AddComponent<bae::TextComponent>(*versusObject, "Versus");
    backObject->AddComponent<bae::TextComponent>(*backObject, "Back");

    // Enable center text
    singlePlayerObject->GetComponent<bae::TextComponent>()->m_bIsCenteredAtPosition = true;
    coOpObject->GetComponent<bae::TextComponent>()->m_bIsCenteredAtPosition         = true;
    versusObject->GetComponent<bae::TextComponent>()->m_bIsCenteredAtPosition       = true;
    backObject->GetComponent<bae::TextComponent>()->m_bIsCenteredAtPosition         = true;


    // Selector Positions
    m_SelectorPositions[0] = { m_SelectorXPosition, singlePlayerObject->GetWorldLocation().y + m_SelectorYOffset };
    m_SelectorPositions[1] = { m_SelectorXPosition, coOpObject->GetWorldLocation().y + m_SelectorYOffset };
    m_SelectorPositions[2] = { m_SelectorXPosition, versusObject->GetWorldLocation().y + m_SelectorYOffset };
    m_SelectorPositions[3] = { m_SelectorXPosition, backObject->GetWorldLocation().y + m_SelectorYOffset };

    const auto selectorObject = std::make_shared<bae::GameObject>("Selector Object");
    selectorObject->AddComponent<bae::TextureComponent>(*selectorObject, "Textures/Misc/Selector.png");
    selectorObject->SetWorldScale({ 4.f, 4.f });

    m_SelectorObject = selectorObject.get();
    UpdateSelectorPosition();

    // Add Objects to Scene
    gameModeSelectionScene->Add(gameModeSelectionObject);
    gameModeSelectionScene->Add(singlePlayerObject);
    gameModeSelectionScene->Add(coOpObject);
    gameModeSelectionScene->Add(versusObject);
    gameModeSelectionScene->Add(backObject);

    gameModeSelectionScene->Add(selectorObject);
}

void GameModeSelectionState::OnExit()
{
    ClearCommands();
    ClearScene();

    switch(m_Selection)
    {
        case GameModeSelection::SinglePlayer:
            std::cout << "Singleplayer\n";
            LevelManager::GetInstance().StartGame(GameMode::Singleplayer);
            break;
        case GameModeSelection::CoOp:
            std::cout << "Co-Op\n";
            LevelManager::GetInstance().StartGame(GameMode::CoOp);
            break;
        case GameModeSelection::Versus:
            std::cout << "Versus\n";
            LevelManager::GetInstance().StartGame(GameMode::Versus);
            break;
        case GameModeSelection::Back:
            std::cout << "Back\n";
    }
}

std::unique_ptr<SceneState> GameModeSelectionState::Update()
{
    if(!m_bIsSelected)
    {
        return nullptr;
    }

    switch(m_Selection)
    {
        case GameModeSelection::SinglePlayer:
        case GameModeSelection::CoOp:
        case GameModeSelection::Versus:
            return std::make_unique<GameState>(*m_GameObject);
        case GameModeSelection::Back:
            return std::make_unique<MainMenuState>(*m_GameObject);
    }

    std::cout << FUNCTION_NAME << " This should never be reached" << '\n';
    return nullptr;
}

void GameModeSelectionState::Notify(const unsigned eventHash, bae::Subject*, const std::any&)
{
    const Events event = GetEvent(eventHash);

    if(event != Events::SelectionUp &&
        event != Events::SelectionDown &&
        event != Events::SelectionConfirmed)
    {
        return;
    }

    if(event == Events::SelectionConfirmed)
    {
        m_bIsSelected = true;
    }

    if(event == Events::SelectionUp)
    {
        switch(m_Selection)
        {
            case GameModeSelection::SinglePlayer:
                std::cout << "Singleplayer->Nothing\n";
                break;
            case GameModeSelection::CoOp:
                std::cout << "coop->singleplayer\n";
                m_Selection = GameModeSelection::SinglePlayer;
                break;
            case GameModeSelection::Versus:
                std::cout << "versus->coop\n";
                m_Selection = GameModeSelection::CoOp;
                break;
            case GameModeSelection::Back:
                std::cout << "back->versus\n";
                m_Selection = GameModeSelection::Versus;
                break;
        }
    }

    if(event == Events::SelectionDown)
    {
        switch(m_Selection)
        {
            case GameModeSelection::SinglePlayer:
                std::cout << "sp->co\n";
                m_Selection = GameModeSelection::CoOp;
                break;
            case GameModeSelection::CoOp:
                std::cout << "co->vs\n";
                m_Selection = GameModeSelection::Versus;
                break;
            case GameModeSelection::Versus:
                std::cout << "vs->back\n";
                m_Selection = GameModeSelection::Back;
                break;
            case GameModeSelection::Back:
                std::cout << "back->nothing\n";
                break;
        }
    }

    UpdateSelectorPosition();
}

void GameModeSelectionState::UpdateSelectorPosition()
{
    const glm::vec2 selectorPosition = m_SelectorPositions[static_cast<int>(m_Selection)];
    m_SelectorObject->SetWorldLocation(selectorPosition);
}

