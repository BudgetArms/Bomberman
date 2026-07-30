// VLD include
#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

// STD includes
#include <cassert>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <memory>


// External libraries includes
#if WIN32
#include <Windows.h>
#endif

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "Core/ServiceLocator.hpp"
#include "Sounds/LoggingSoundSystem.hpp"

#ifndef __EMSCRIPTEN__
#include "Sounds/MixerSoundSystem.hpp"
#endif
#include "Base/CommonManagerVariables.hpp"
#include "Base/SoundAssets.hpp"
#include "Commands/ToggleMuteSoundsCommand.hpp"
#include "Components/ManagersComponent.hpp"
#include "Managers/InputManager.hpp"
#include "Sounds/SoLoudSoundSystem.hpp"
#include "Wrappers/Keyboard.hpp"


#ifdef STEAMWORKS_ENABLED
#pragma warning (push)
#pragma warning (disable: 4996)
#include <steam_api.h>
#pragma warning (pop)
#endif

// BudgetArmsEngine includes
#include "Core/BudgetEngine.hpp"
#include "Core/GameObject.hpp"
#include "Core/Renderer.hpp"
#include "Core/Scene.hpp"

#include "Components/FpsCounterComponent.hpp"
#include "Components/TextComponent.hpp"
#include "Components/TextureComponent.hpp"

#include "Managers/ResourceManager.hpp"
#include "Managers/SceneManager.hpp"

#include "Wrappers/Texture2D.hpp"


#ifdef STEAMWORKS_ENABLED
#include "Core/Achievement.hpp"
#include "Managers/AchievementManager.hpp"
#include "Managers/SteamManager.hpp"
#endif


// Game Includes
#include "Managers/LevelManager.hpp"


namespace fs = std::filesystem;

void Start();

void LoadSounds();
void LoadSoundCommands();

void CreateAllScenes();

void LoadStartMenu();

void LoadManagers();

void LoadDAEBackground();
void LoadFpsCounterScene();
void LoadGameNameScene();

void EnableLogMousePosition();


int main(int, char*[])
{
    #if _DEBUG && __has_include(<vld.h>)
    std::cout << "VLD enabled" << '\n';
    #else
    std::cout << "VLD disabled" << '\n';
    #endif

    bae::Utils::Window window{ "Bomberman", "./Resources/", 992, 416, false };


    #if __EMSCRIPTEN__
    window.ResourceFolder = "";
    #else

    if(!fs::exists(window.ResourceFolder))
    {
        window.ResourceFolder = "../Resources/";
    }

    if(!fs::exists(window.ResourceFolder))
    {
        std::cout << "Resources Folder Not Found" << '\n';
        assert("Resources Folder Not Found");
        exit(-1);
    }

    #ifdef STEAMWORKS_ENABLED
    bae::SteamManager::GetInstance().Initialize();
    #endif

    #endif


    bae::BudgetEngine engine(window);
    engine.Run(Start);


    #ifdef STEAMWORKS_ENABLED
    bae::SteamManager::GetInstance().Shutdown();
    #endif

    std::cout << "\n\n";
    return 0;
}


void Start()
{
    LoadSounds();
    LoadSoundCommands();

    CreateAllScenes();

    // LoadStartMenu();
    LoadManagers();

    // LoadDAEBackground();
    LoadFpsCounterScene();
    LoadGameNameScene();

    EnableLogMousePosition();
}


void LoadSounds()
{
    namespace gs = Game::Sounds;

    #if __EMSCRIPTEN__

    bae::ServiceLocator::RegisterSoundSystem(std::make_unique<bae::LoggingSoundSystem>(
        std::make_unique<bae::SoLoudSoundSystem>()));

    #else

    bae::ServiceLocator::RegisterSoundSystem(
        // std::make_unique<bae::LoggingSoundSystem>(std::make_unique<bae::MixerSoundSystem>()));
        std::make_unique<bae::MixerSoundSystem>());

    #endif

    const auto soundSystem = &bae::ServiceLocator::GetSoundSystem();
    gs::g_sSoundEvents     =
    {
        { gs::SoundAssets::BombermanKilled, soundSystem->LoadSound("Sounds/BombermanKilled.wav") },
        { gs::SoundAssets::BombExplosion, soundSystem->LoadSound("Sounds/BombExplosion.wav") },
        { gs::SoundAssets::BombLay, soundSystem->LoadSound("Sounds/BombLay.wav") },
        { gs::SoundAssets::Pause, soundSystem->LoadSound("Sounds/Pause.wav") },
        { gs::SoundAssets::PowerUp, soundSystem->LoadSound("Sounds/PowerUp.wav") },
        { gs::SoundAssets::StepHorizontal, soundSystem->LoadSound("Sounds/StepHorizontal.wav") },
        { gs::SoundAssets::StepVertical, soundSystem->LoadSound("Sounds/StepVertical.wav") },
    };
}

void LoadSoundCommands()
{
    const bae::Keyboard& keyboard = bae::InputManager::GetInstance().GetKeyboard();

    auto toggleMuteSoundsCommand = std::make_unique<Game::ToggleMuteSoundsCommand>();
    keyboard.AddKeyboardCommands(std::move(toggleMuteSoundsCommand), SDLK_F2, bae::InputManager::ButtonState::Down);
}

void CreateAllScenes()
{
    // in reverse order, so that the start menu scene is displayed before anything else
    bae::SceneManager::GetInstance().CreateScene(Game::g_LevelBackgroundName.data());
    bae::SceneManager::GetInstance().CreateScene(Game::g_LevelSceneName.data());
    bae::SceneManager::GetInstance().CreateScene(Game::g_LevelGameOverSceneName.data());
    bae::SceneManager::GetInstance().CreateScene(Game::g_StartMenuSceneName.data());
}

void LoadStartMenu()
{
    auto* startMenuScene = bae::SceneManager::GetInstance().GetScene(Game::g_StartMenuSceneName.data());

    bae::Renderer::GetInstance().SetBackgroundColor(bae::Utils::Color::Black);

    const bae::WindowSize windowSize = bae::Renderer::GetInstance().GetSDLWindowSize();

    const auto startMenuObject = std::make_shared<bae::GameObject>("StartMenu");
    startMenuObject->SetWorldLocation({
        static_cast<float>(windowSize.Width) / 2.f, static_cast<float>(windowSize.Height) / 2.f
    });

    const auto singlePlayerObject = std::make_shared<bae::GameObject>("SinglePlayer");
    const auto coOpObject         = std::make_shared<bae::GameObject>("CoOp");
    const auto versusObject       = std::make_shared<bae::GameObject>("Versus");

    constexpr float verticalPadding = 150.f;


    singlePlayerObject->AddLocation({ 0, -verticalPadding });
    coOpObject->AddLocation({ 0, 0 });
    versusObject->AddLocation({ 0, verticalPadding });

    // AttachChild, but don't freeze position
    startMenuObject->AttachChild(singlePlayerObject.get(), false);
    startMenuObject->AttachChild(coOpObject.get(), false);
    startMenuObject->AttachChild(versusObject.get(), false);

    singlePlayerObject->AddComponent<bae::TextComponent>(*singlePlayerObject, "SinglePlayer");
    coOpObject->AddComponent<bae::TextComponent>(*coOpObject, "Co-Op");
    versusObject->AddComponent<bae::TextComponent>(*versusObject, "Versus");

    // Enable center text
    singlePlayerObject->GetComponent<bae::TextComponent>()->m_bIsCenteredAtPosition = true;
    coOpObject->GetComponent<bae::TextComponent>()->m_bIsCenteredAtPosition         = true;
    versusObject->GetComponent<bae::TextComponent>()->m_bIsCenteredAtPosition       = true;

    // Add Objects to Scene
    startMenuScene->Add(startMenuObject);
    startMenuScene->Add(singlePlayerObject);
    startMenuScene->Add(coOpObject);
    startMenuScene->Add(versusObject);
}


void LoadManagers()
{
    // Setting up level manager data
    Game::LevelManager& levelManager = Game::LevelManager::GetInstance();

    levelManager.SpawnBomberman();


    const auto managersObject = std::make_shared<bae::GameObject>("Managers Object");
    managersObject->AddComponent<Game::ManagersComponent>(*managersObject);

    bae::Scene* managersScene = bae::SceneManager::GetInstance().GetScene(Game::g_LevelBackgroundName.data());
    managersScene->Add(managersObject);


    /*
    const bae::WindowSize windowSize = bae::Renderer::GetInstance().GetSDLWindowSize();
    levelManager.SetSpriteSheetWorldLocation({
        static_cast<float>(windowSize.Width) / 2.f, static_cast<float>(windowSize.Height) / 2.f
    });
    levelManager.SetSpriteSheetWorldScale({ 2.f, 2.f });

    levelManager.LoadLevelFromFile(0, "Levels/Level_1.json");
    levelManager.LoadLevelFromFile(1, "Levels/Level_2.json");

    auto skipLevelCommand = std::make_unique<Game::SkipLevelCommand>();

    const bae::Keyboard& keyboard = bae::InputManager::GetInstance().GetKeyboard();
    keyboard.AddKeyboardCommands(std::move(skipLevelCommand), SDLK_F1, bae::InputManager::ButtonState::Down);

    bae::EventQueue::GetInstance().SendEvent(Game::GetEventHash(Game::Events::BeginLevel));

    auto* backgroundScene = bae::SceneManager::GetInstance().GetScene(Game::g_LevelBackgroundName.data());

    const auto managersObject = std::make_shared<bae::GameObject>("ManagersObject");
    managersObject->AddComponent<Game::ManagersComponent>(*managersObject);

    backgroundScene->Add(managersObject);
    */
}

void LoadDAEBackground()
{
    auto& backgroundScene = bae::SceneManager::GetInstance().CreateScene("Background Scene");

    const auto backgroundTexture = std::make_shared<bae::GameObject>("BackgroundTexture");
    backgroundTexture->AddComponent<bae::TextureComponent>(*backgroundTexture, "Textures/background.png");
    backgroundScene.Add(backgroundTexture);


    const auto backgroundLogoTexture = std::make_shared<bae::GameObject>("Background Logo Texture");
    backgroundLogoTexture->AddComponent<bae::TextureComponent>(*backgroundLogoTexture, "Textures/logo.png");
    const auto backgroundLogoTextureComp = backgroundLogoTexture->GetComponent<bae::TextureComponent>();

    const bae::WindowSize windowSize = bae::Renderer::GetInstance().GetSDLWindowSize();

    backgroundLogoTextureComp->m_bIsCenteredAtPosition = true;
    backgroundLogoTexture->SetWorldLocation(
        {
            static_cast<float>(windowSize.Width) / 2,
            static_cast<float>(windowSize.Height) / 2
        }
    );

    backgroundScene.Add(backgroundLogoTexture);

    bae::Utils::DrawCircle({ 0, 0 }, 1000, bae::Utils::Color::Blue);
}


void LoadFpsCounterScene()
{
    auto& fpsScene = bae::SceneManager::GetInstance().CreateScene("FpsCounterScene");

    auto fontSmall = bae::ResourceManager::GetInstance().LoadFont("Fonts/Lingua.otf", 18);

    const auto fpsCounter = std::make_shared<bae::GameObject>("Fps Counter");
    fpsCounter->AddComponent<bae::FpsTextComponent>(*fpsCounter, fontSmall, bae::Utils::Color::White);

    const bae::WindowSize windowSize = bae::Renderer::GetInstance().GetSDLWindowSize();

    fpsCounter->SetWorldLocation({ windowSize.Width, 0.f });
    fpsCounter->AddLocation({ -75.f, 5.f });

    fpsScene.Add(fpsCounter);
}

void LoadGameNameScene()
{
    auto& gameNameScene = bae::SceneManager::GetInstance().CreateScene("Game Name Scene");
    auto font           = bae::ResourceManager::GetInstance().LoadFont("Fonts/Lingua.otf", 48);

    const auto gameName = std::make_shared<bae::GameObject>("Game Name");
    gameName->AddComponent<bae::TextComponent>(*gameName, "Bomberman", font, bae::Utils::Color::Green);

    gameNameScene.Add(gameName);
}

void EnableLogMousePosition()
{
    /*
    const bae::Mouse& mouse = bae::InputManager::GetInstance().GetMouse();

    auto mousePressedCommand = std::make_unique<Game::TestMousePositionCommand>();
    mouse.AddMouseCommands(std::move(mousePressedCommand), SDL_BUTTON_RIGHT, bae::InputManager::ButtonState::Pressed);
    */
}

