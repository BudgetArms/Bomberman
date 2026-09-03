#include "MainMenuState.hpp"

#include "Components/TextComponent.hpp"
#include "Components/TextureComponent.hpp"
#include "Core/Renderer.hpp"
#include "Core/Scene.hpp"
#include "Managers/ResourceManager.hpp"
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

    // Title
    const auto titleFont = bae::ResourceManager::GetInstance().LoadFont("Fonts/JoystixMonospace.otf", 56);
    auto titleObject     = std::make_unique<bae::GameObject>("Title");
    titleObject->SetWorldLocation({ windowSize.Width / 2.f, 50.f });

    titleObject->AddComponent<bae::TextComponent>(*titleObject, "Bomberman", titleFont);
    titleObject->GetComponent<bae::TextComponent>()->m_bIsCenteredAtPosition = true;
    gameModeSelectionScene->Add(std::move(titleObject));


    auto mainMenu = std::make_unique<bae::GameObject>("MainMenu");
    mainMenu->SetWorldLocation({
        static_cast<float>(windowSize.Width) / 2.f, static_cast<float>(windowSize.Height) / 2.f
    });
    mainMenu->AddLocation({ 0, 50 });

    auto playObject        = std::make_unique<bae::GameObject>("Play");
    auto leaderboardObject = std::make_unique<bae::GameObject>("Leaderboard");
    auto quitObject        = std::make_unique<bae::GameObject>("Quit");

    constexpr float verticalPadding = 120.f;

    playObject->AddLocation({ 0, -verticalPadding });
    leaderboardObject->AddLocation({ 0, 0 });
    quitObject->AddLocation({ 0, verticalPadding });

    // AttachChild, but don't freeze position
    mainMenu->AttachChild(*playObject, false);
    mainMenu->AttachChild(*leaderboardObject, false);
    mainMenu->AttachChild(*quitObject, false);

    const auto textFont = bae::ResourceManager::GetInstance().LoadFont("Fonts/JoystixMonospace.otf", 32);
    playObject->AddComponent<bae::TextComponent>(*playObject, "Play", textFont);
    leaderboardObject->AddComponent<bae::TextComponent>(*leaderboardObject, "Leaderboard", textFont);
    quitObject->AddComponent<bae::TextComponent>(*quitObject, "Quit", textFont);

    // Enable center text
    playObject->GetComponent<bae::TextComponent>()->m_bIsCenteredAtPosition        = true;
    leaderboardObject->GetComponent<bae::TextComponent>()->m_bIsCenteredAtPosition = true;
    quitObject->GetComponent<bae::TextComponent>()->m_bIsCenteredAtPosition        = true;


    // Selector Positions
    m_SelectorPositions[0] = { m_SelectorXPosition, playObject->GetWorldLocation().y + m_SelectorYOffset };
    m_SelectorPositions[1] = { m_SelectorXPosition, leaderboardObject->GetWorldLocation().y + m_SelectorYOffset };
    m_SelectorPositions[2] = { m_SelectorXPosition, quitObject->GetWorldLocation().y + m_SelectorYOffset };

    auto selectorObject = std::make_unique<bae::GameObject>("Selector Object");
    selectorObject->AddComponent<bae::TextureComponent>(*selectorObject, "Textures/Misc/Selector.png");
    selectorObject->SetWorldScale({ 4.f, 4.f });

    const glm::vec2 selectorPosition = m_SelectorPositions[static_cast<int>(m_Selection)];

    m_SelectorObject = selectorObject.get();
    m_SelectorObject->SetWorldLocation(selectorPosition);

    // Add Objects to Scene
    gameModeSelectionScene->Add(std::move(mainMenu));
    gameModeSelectionScene->Add(std::move(playObject));
    gameModeSelectionScene->Add(std::move(leaderboardObject));
    gameModeSelectionScene->Add(std::move(quitObject));

    gameModeSelectionScene->Add(std::move(selectorObject));
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
            return std::make_unique<GameModeSelectionState>(*m_GameObject);
        case StartMenuSelections::Leaderboard:
            return std::make_unique<LeaderboardState>(*m_GameObject);
        case StartMenuSelections::Quit:
        {
            // Request Quit
            SDL_Event event{};
            event.type = SDL_EVENT_QUIT;
            SDL_PushEvent(&event);

            return nullptr;
        }
    }

    return nullptr;
}

void MainMenuState::Notify(const bae::EventData& eventData, bae::Subject*)
{
    const Events event = GetEvent(eventData.Hash);

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

