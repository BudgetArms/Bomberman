#include "ScenesManager.hpp"

#include "Base/CommonManagerVariables.hpp"
#include "Core/HelperFunctions.hpp"
#include "Core/Scene.hpp"
#include "Managers/SceneManager.hpp"


using namespace Game;


void ScenesManager::Initialize()
{
    if(m_bIsInitialized)
    {
        std::cout << FUNCTION_NAME << " Failed! Already is initialized" << '\n';
        return;
    }

    m_bIsInitialized = true;
    m_SceneMode      = SceneMode::MainMenu;

    ClearScene();
    LoadMainMenuScene();
}

void ScenesManager::Notify(unsigned eventHash, bae::Subject* subject, const std::any& eventData)
{
}


void ScenesManager::ClearScene()
{
    const bae::Scene* scene = bae::SceneManager::GetInstance().GetScene(g_ScenesManagerSceneName.data());
    scene->RemoveAll();
}

void ScenesManager::LoadControls()
{
}

void ScenesManager::LoadMainMenuScene()
{
}

void ScenesManager::LoadGameModeSelectionScene() {
}

void ScenesManager::LoadLeaderboardScene() {
}

void ScenesManager::LoadGameScene() {
}

void ScenesManager::LoadInputLeaderboardNameScene() {
}
