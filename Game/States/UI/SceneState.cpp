#include "SceneState.hpp"

#include "Wrappers/Controller.hpp"
#include "Wrappers/Keyboard.hpp"

#include "Base/Events.hpp"
#include "Commands/SelectionCommands.hpp"
#include "Commands/ToggleMuteSoundsCommand.hpp"
#include "Managers/LevelManager.hpp"
#include "Managers/ScenesManager.hpp"


using namespace Game::States;


SceneState::SceneState(bae::GameObject& selectionObject) :
    m_GameObject{ &selectionObject }
{
}

void SceneState::ClearCommands()
{
    const bae::Keyboard& keyboard     = bae::InputManager::GetInstance().GetKeyboard();
    const bae::Controller& controller = *bae::InputManager::GetInstance().GetController(0);

    keyboard.ClearCommands();
    controller.ClearCommands();
}

void SceneState::AddSelectionConfirmedCommands()
{
    constexpr auto buttonState  = bae::InputManager::ButtonState::Pressed;
    bae::GameObject* gameObject = ScenesManager::GetInstance().GetSelectionObject();


    const bae::Keyboard& keyboard = bae::InputManager::GetInstance().GetKeyboard();

    auto selectionConfirmationCommand = std::make_unique<SelectionConfirmedCommand>(*gameObject);
    keyboard.AddKeyboardCommands(std::move(selectionConfirmationCommand), m_KeySelectionConfirmation, buttonState);

    #if WIN32
    const bae::Controller& controller = *bae::InputManager::GetInstance().GetController(0);

    selectionConfirmationCommand = std::make_unique<SelectionConfirmedCommand>(*gameObject);
    controller.AddControllerCommands(std::move(selectionConfirmationCommand), m_ControllerSelectionConfirmation,
                                     buttonState);
    #endif
}

void SceneState::AddSelectionDownUpCommands()
{
    constexpr auto buttonState  = bae::InputManager::ButtonState::Pressed;
    bae::GameObject* gameObject = ScenesManager::GetInstance().GetSelectionObject();


    const bae::Keyboard& keyboard = bae::InputManager::GetInstance().GetKeyboard();

    auto selectionDownCommand = std::make_unique<SelectionDownCommand>(*gameObject);
    auto selectionUpCommand   = std::make_unique<SelectionUpCommand>(*gameObject);
    keyboard.AddKeyboardCommands(std::move(selectionDownCommand), m_KeySelectionDown, buttonState);
    keyboard.AddKeyboardCommands(std::move(selectionUpCommand), m_KeySelectionUp, buttonState);

    #if WIN32
    const bae::Controller& controller = *bae::InputManager::GetInstance().GetController(0);

    selectionDownCommand = std::make_unique<SelectionDownCommand>(*gameObject);
    selectionUpCommand   = std::make_unique<SelectionUpCommand>(*gameObject);
    controller.AddControllerCommands(std::move(selectionDownCommand), m_ControllerSelectionDown, buttonState);
    controller.AddControllerCommands(std::move(selectionUpCommand), m_ControllerSelectionUp, buttonState);
    #endif
}

void SceneState::AddSelectionLeftRightCommands()
{
    constexpr auto buttonState  = bae::InputManager::ButtonState::Pressed;
    bae::GameObject* gameObject = ScenesManager::GetInstance().GetSelectionObject();


    const bae::Keyboard& keyboard = bae::InputManager::GetInstance().GetKeyboard();

    auto selectionLeftCommand  = std::make_unique<SelectionLeftCommand>(*gameObject);
    auto selectionRightCommand = std::make_unique<SelectionRightCommand>(*gameObject);
    keyboard.AddKeyboardCommands(std::move(selectionLeftCommand), m_KeySelectionLeft, buttonState);
    keyboard.AddKeyboardCommands(std::move(selectionRightCommand), m_KeySelectionRight, buttonState);

    #if WIN32
    const bae::Controller& controller = *bae::InputManager::GetInstance().GetController(0);

    selectionLeftCommand  = std::make_unique<SelectionLeftCommand>(*gameObject);
    selectionRightCommand = std::make_unique<SelectionRightCommand>(*gameObject);
    controller.AddControllerCommands(std::move(selectionLeftCommand), m_ControllerSelectionLeft, buttonState);
    controller.AddControllerCommands(std::move(selectionRightCommand), m_ControllerSelectionRight, buttonState);
    #endif
}


// Main Menu
MainMenuState::MainMenuState(bae::GameObject& selectionObject) :
    SceneState(selectionObject)
{
}

void MainMenuState::OnEnter()
{
    ClearCommands();
    AddSelectionConfirmedCommands();
    AddSelectionDownUpCommands();
}

void MainMenuState::OnExit()
{
    ClearCommands();
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
                return;
            case StartMenuSelections::Leaderboard:
                m_Selection = StartMenuSelections::Play;
                return;
            case StartMenuSelections::Quit:
                m_Selection = StartMenuSelections::Leaderboard;
                return;
        }
    }

    if(event == Events::SelectionDown)
    {
        switch(m_Selection)
        {
            case StartMenuSelections::Play:
                m_Selection = StartMenuSelections::Leaderboard;
                return;
            case StartMenuSelections::Leaderboard:
                m_Selection = StartMenuSelections::Quit;
                return;
            case StartMenuSelections::Quit:
                return;
        }
    }
}


// Game Mode Selection
GameModeSelectionState::GameModeSelectionState(bae::GameObject& selectionObject) :
    SceneState(selectionObject)
{
}

void GameModeSelectionState::OnEnter()
{
    ClearCommands();
    AddSelectionConfirmedCommands();
    AddSelectionDownUpCommands();
}

void GameModeSelectionState::OnExit()
{
    ClearCommands();
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
            LevelManager::GetInstance().StartGame(GameMode::Singleplayer);
            return std::make_unique<GameState>(*m_GameObject);
        case GameModeSelection::CoOp:
            LevelManager::GetInstance().StartGame(GameMode::CoOp);
            return std::make_unique<GameState>(*m_GameObject);
        case GameModeSelection::Versus:
            LevelManager::GetInstance().StartGame(GameMode::Versus);
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
                return;
            case GameModeSelection::CoOp:
                m_Selection = GameModeSelection::SinglePlayer;
                return;
            case GameModeSelection::Versus:
                m_Selection = GameModeSelection::CoOp;
                return;
            case GameModeSelection::Back:
                m_Selection = GameModeSelection::Versus;
                return;
        }
    }

    if(event == Events::SelectionDown)
    {
        switch(m_Selection)
        {
            case GameModeSelection::SinglePlayer:
                m_Selection = GameModeSelection::CoOp;
                return;
            case GameModeSelection::CoOp:
                m_Selection = GameModeSelection::Versus;
                return;
            case GameModeSelection::Versus:
                m_Selection = GameModeSelection::Back;
                return;
            case GameModeSelection::Back:
                return;
        }
    }
}


// GameState
GameState::GameState(bae::GameObject& selectionObject) :
    SceneState(selectionObject)
{
}

void GameState::OnEnter()
{
    ClearCommands();

    // Added Sound Toggle
    const bae::Keyboard& keyboard = bae::InputManager::GetInstance().GetKeyboard();

    auto toggleMuteSoundsCommand = std::make_unique<ToggleMuteSoundsCommand>();
    keyboard.AddKeyboardCommands(std::move(toggleMuteSoundsCommand), SDLK_F2, bae::InputManager::ButtonState::Down);
}

void GameState::OnExit()
{
    ClearCommands();
}

std::unique_ptr<SceneState> GameState::Update()
{
    if(!m_bIsGameFinished)
    {
        return nullptr;
    }

    return std::make_unique<InputLeaderboardNameState>(*m_GameObject);
}

void GameState::HandleEvent(const unsigned eventHash)
{
    if(GetEvent(eventHash) == Events::GameWon ||
        GetEvent(eventHash) == Events::GameOver)
    {
        m_bIsGameFinished = true;
    }
}


// Leaderboard State
LeaderboardState::LeaderboardState(bae::GameObject& selectionObject) :
    SceneState(selectionObject)
{
}

void LeaderboardState::OnEnter()
{
    ClearCommands();
    AddSelectionConfirmedCommands();
}

void LeaderboardState::OnExit()
{
    ClearCommands();
}

std::unique_ptr<SceneState> LeaderboardState::Update()
{
    if(!m_bIsSelected)
    {
        return nullptr;
    }

    return std::make_unique<MainMenuState>(*m_GameObject);
}

void LeaderboardState::Notify(const unsigned eventHash, bae::Subject*, const std::any&)
{
    const Events event = GetEvent(eventHash);
    if(event == Events::SelectionConfirmed)
    {
        m_bIsSelected = true;
    }
}


// Input Leaderboard Name
InputLeaderboardNameState::InputLeaderboardNameState(bae::GameObject& selectionObject) :
    SceneState(selectionObject)
{
}

void InputLeaderboardNameState::OnEnter()
{
    ClearCommands();
    AddSelectionConfirmedCommands();
    AddSelectionDownUpCommands();
    AddSelectionLeftRightCommands();
}

void InputLeaderboardNameState::OnExit()
{
    ClearCommands();
}

std::unique_ptr<SceneState> InputLeaderboardNameState::Update()
{
    if(!m_bIsSelected)
    {
        return nullptr;
    }

    [[maybe_unused]] const std::string leaderboardName = GetLeaderboardName();
    //

    return std::make_unique<MainMenuState>(*m_GameObject);
}

void InputLeaderboardNameState::Notify(const unsigned eventHash, bae::Subject*, const std::any&)
{
    const Events event = GetEvent(eventHash);

    if(event == Events::SelectionConfirmed)
    {
        m_bIsSelected = true;
        return;
    }

    if(event == Events::SelectionLeft)
    {
        if(m_LetterIndex == 0)
        {
            return;
        }

        --m_LetterIndex;
    }

    if(event == Events::SelectionRight)
    {
        if(m_LetterIndex >= m_NrLetters - 1)
        {
            return;
        }

        ++m_LetterIndex;
    }

    if(event == Events::SelectionUp)
    {
        if(m_LeaderboardName[m_LetterIndex]->m_Index == 0)
        {
            m_LeaderboardName[m_LetterIndex]->m_Index = 25;
            return;
        }

        --m_LeaderboardName[m_LetterIndex]->m_Index;
    }

    if(event == Events::SelectionDown)
    {
        if(m_LeaderboardName[m_LetterIndex]->m_Index == 25)
        {
            m_LeaderboardName[m_LetterIndex]->m_Index = 0;
            return;
        }

        ++m_LeaderboardName[m_LetterIndex]->m_Index;
    }
}

std::string InputLeaderboardNameState::GetLetter(const int letterIndex)
{
    return std::basic_string<char>{ 1, static_cast<char>('a' + letterIndex) };
}


std::string InputLeaderboardNameState::GetLeaderboardName() const
{
    std::string leaderboardName{};
    for(const auto& leaderboardSpriteSheet : m_LeaderboardName)
    {
        leaderboardName.append(GetLetter(leaderboardSpriteSheet->m_Index));
    }

    std::cout << FUNCTION_NAME << " " << leaderboardName << '\n';
    return leaderboardName;
}
