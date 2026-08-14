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
#include "Commands/LogMousePositionCommand.hpp"
#include "Commands/ToggleMuteSoundsCommand.hpp"
#include "Components/ManagersComponent.hpp"
#include "Managers/InputManager.hpp"
#include "Managers/ScenesManager.hpp"
#include "Sounds/SoLoudSoundSystem.hpp"
#include "Wrappers/Keyboard.hpp"
#include "Wrappers/Mouse.hpp"


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
void LoadManagers();

void LoadFpsCounterScene();
void LoadGameNameScene();


int main(int, char*[])
{
    #if _DEBUG && __has_include(<vld.h>)
    std::cout << "VLD enabled" << '\n';
    #else
    std::cout << "VLD disabled" << '\n';
    #endif

    bae::Utils::Window window{ "Bomberman", "./Resources/", 992, 450, false };


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

    LoadManagers();

    // LoadFpsCounterScene();
    // LoadGameNameScene();
}

#ifndef __EMSCRIPTEN__
void CreateConsole()
{
    if(AllocConsole())
    {
        FILE* fp;

        freopen_s(&fp, "CONOUT$", "w", stdout);
        freopen_s(&fp, "CONOUT$", "w", stderr);
        freopen_s(&fp, "CONIN$", "r", stdin);

        std::cout << "Release Mode: Console created\n";
    }
}
#endif

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
        { gs::SoundAssets::ManOutOfMe, soundSystem->LoadSound("Sounds/StepVertical.wav") },
    };


    const bool bAreAllSoundLoaded = Game::Sounds::g_sSoundEvents.size()
            == static_cast<std::size_t>(Game::Sounds::SoundAssets::Count);

    if(!bAreAllSoundLoaded)
    {
        const std::string errorMessage = std::string(FUNCTION_NAME) + " Failed Not All SoundEvents are Loaded \n";

        #ifndef __EMSCRIPTEN__
        #ifdef NDEBUG
        CreateConsole();
        #endif
        #endif

        std::cout << errorMessage;
        assert(false && errorMessage.c_str());
    }
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
    bae::SceneManager::GetInstance().CreateScene(Game::g_NeverDestroySceneName.data());
    bae::SceneManager::GetInstance().CreateScene(Game::g_LevelBackgroundName.data());
    bae::SceneManager::GetInstance().CreateScene(Game::g_LevelSceneName.data());
    bae::SceneManager::GetInstance().CreateScene(Game::g_ScenesManagerSceneName.data());
    bae::SceneManager::GetInstance().CreateScene(Game::g_SelectionObjectSceneName.data());
}

void LoadManagers()
{
    Game::ScenesManager& scenesManager = Game::ScenesManager::GetInstance();
    scenesManager.Initialize();

    const auto managersObject = std::make_shared<bae::GameObject>("Managers Object");
    managersObject->AddComponent<Game::ManagersComponent>(*managersObject);

    bae::Scene* managersScene = bae::SceneManager::GetInstance().GetScene(Game::g_NeverDestroySceneName.data());
    managersScene->Add(managersObject);

    auto& levelManager = Game::LevelManager::GetInstance();

    levelManager.LoadLevelInfo("Levels/Level_0.json");
    levelManager.LoadLevelInfo("Levels/Level_1.json");
    levelManager.LoadLevelInfo("Levels/Level_Testing.json");
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
