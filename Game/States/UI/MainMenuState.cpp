#include "MainMenuState.hpp"

#include "Components/TextComponent.hpp"
#include "Core/Renderer.hpp"
#include "Core/Scene.hpp"
#include "Managers/SceneManager.hpp"

#include "Base/CommonManagerVariables.hpp"
#include "Base/Events.hpp"
#include "States/UI/GameModeSelectionState.hpp"
#include "States/UI/LeaderboardState.hpp"


using namespace Game::States;


MainMenuState::MainMenuState(bae::GameObject& selectionObject) :
    SceneState(selectionObject)
{
}

void MainMenuState::OnEnter()
{
    ClearScene();
    ClearCommands();
    AddSelectionConfirmedCommands();
    AddSelectionDownUpCommands();


    auto* gameModeSelectionScene = bae::SceneManager::GetInstance().GetScene(g_ScenesManagerSceneName.data());

    bae::Renderer::GetInstance().SetBackgroundColor(bae::Utils::Color::Black);

    const bae::WindowSize windowSize = bae::Renderer::GetInstance().GetSDLWindowSize();

    const auto mainMenu = std::make_shared<bae::GameObject>("MainMenu");
    mainMenu->SetWorldLocation({
        static_cast<float>(windowSize.Width) / 2.f, static_cast<float>(windowSize.Height) / 2.f
    });

    const auto playObject        = std::make_shared<bae::GameObject>("Play");
    const auto leaderboardObject = std::make_shared<bae::GameObject>("Leaderboard");
    const auto quitObject        = std::make_shared<bae::GameObject>("Quit");

    constexpr float verticalPadding = 150.f;

    playObject->AddLocation({ 0, -verticalPadding });
    leaderboardObject->AddLocation({ 0, 0 });
    quitObject->AddLocation({ 0, verticalPadding });

    // AttachChild, but don't freeze position
    mainMenu->AttachChild(playObject.get(), false);
    mainMenu->AttachChild(leaderboardObject.get(), false);
    mainMenu->AttachChild(quitObject.get(), false);

    playObject->AddComponent<bae::TextComponent>(*playObject, "Play");
    leaderboardObject->AddComponent<bae::TextComponent>(*leaderboardObject, "Leaderboard");
    quitObject->AddComponent<bae::TextComponent>(*quitObject, "Quit");

    // Enable center text
    playObject->GetComponent<bae::TextComponent>()->m_bIsCenteredAtPosition        = true;
    leaderboardObject->GetComponent<bae::TextComponent>()->m_bIsCenteredAtPosition = true;
    quitObject->GetComponent<bae::TextComponent>()->m_bIsCenteredAtPosition        = true;


    // Selector Positions
    m_SelectorPositions[0] = { m_SelectorXPosition, playObject->GetWorldLocation().y + m_SelectorYOffset };
    m_SelectorPositions[1] = { m_SelectorXPosition, leaderboardObject->GetWorldLocation().y + m_SelectorYOffset };
    m_SelectorPositions[2] = { m_SelectorXPosition, quitObject->GetWorldLocation().y + m_SelectorYOffset };

    const auto selectorObject = std::make_shared<bae::GameObject>("Selector Object");
    selectorObject->AddComponent<bae::TextureComponent>(*selectorObject, "Textures/Misc/Selector.png");
    selectorObject->SetWorldScale({ 4.f, 4.f });

    const glm::vec2 selectorPosition = m_SelectorPositions[static_cast<int>(m_Selection)];

    m_SelectorObject = selectorObject.get();
    m_SelectorObject->SetWorldLocation(selectorPosition);

    // Add Objects to Scene
    gameModeSelectionScene->Add(mainMenu);
    gameModeSelectionScene->Add(playObject);
    gameModeSelectionScene->Add(leaderboardObject);
    gameModeSelectionScene->Add(quitObject);

    gameModeSelectionScene->Add(selectorObject);
}

void MainMenuState::OnExit()
{
    ClearCommands();
    ClearScene();
}

std::unique_ptr<SceneState> MainMenuState::Update()
{
    if(!m_bIsSelected)
    {
        return nullptr;
    }

    switch(m_Selection)
    {
        case StartMenuSelections::Play:
            std::cout << "GameMode Selection\n";
            return std::make_unique<GameModeSelectionState>(*m_GameObject);
        case StartMenuSelections::Leaderboard:
            std::cout << "Leaderboard\n";
            return std::make_unique<LeaderboardState>(*m_GameObject);
        case StartMenuSelections::Quit:
            std::cout << "Quit\n";
            return nullptr;
    }

    return nullptr;
}

void MainMenuState::Notify(const unsigned eventHash, bae::Subject*, const std::any&)
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
            case StartMenuSelections::Play:
                break;
            case StartMenuSelections::Leaderboard:
                m_Selection = StartMenuSelections::Play;
                break;
            case StartMenuSelections::Quit:
                m_Selection = StartMenuSelections::Leaderboard;
                break;
        }
    }

    if(event == Events::SelectionDown)
    {
        switch(m_Selection)
        {
            case StartMenuSelections::Play:
                m_Selection = StartMenuSelections::Leaderboard;
                break;
            case StartMenuSelections::Leaderboard:
                m_Selection = StartMenuSelections::Quit;
                break;
            case StartMenuSelections::Quit:
                break;
        }
    }

    UpdateSelectorPosition();
}

void MainMenuState::UpdateSelectorPosition()
{
    const glm::vec2 selectorPosition = m_SelectorPositions[static_cast<int>(m_Selection)];
    m_SelectorObject->SetWorldLocation(selectorPosition);
}

