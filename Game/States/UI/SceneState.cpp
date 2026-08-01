#include "SceneState.hpp"

#include "Wrappers/Controller.hpp"
#include "Wrappers/Keyboard.hpp"

#include "Base/Events.hpp"
#include "Commands/SelectionCommands.hpp"
#include "Commands/ToggleMuteSoundsCommand.hpp"
#include "Managers/LevelManager.hpp"
#include "Managers/ScenesManager.hpp"
#include "States/UI/MainMenuState.hpp"


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

