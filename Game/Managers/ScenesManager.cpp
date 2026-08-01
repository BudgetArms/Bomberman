#include "ScenesManager.hpp"

#include "Base/CommonManagerVariables.hpp"
#include "Core/HelperFunctions.hpp"
#include "Core/Scene.hpp"
#include "Managers/SceneManager.hpp"
#include "States/UI/MainMenuState.hpp"


using namespace Game;


void ScenesManager::Initialize()
{
    if(m_bIsInitialized)
    {
        std::cout << FUNCTION_NAME << " Failed! Already is initialized" << '\n';
        return;
    }

    m_bIsInitialized = true;

    // Create SelectionObject
    const auto selectionObject = std::make_shared<bae::GameObject>("Selection Object");
    m_SelectionObject          = selectionObject.get();

    bae::Scene* scene = bae::SceneManager::GetInstance().GetScene(g_ScenesManagerSceneName.data());
    scene->Add(selectionObject);

    m_SceneState = std::make_unique<States::MainMenuState>(*m_SelectionObject);
    m_SceneState->OnEnter();
}

void ScenesManager::Update()
{
    if(!m_bIsInitialized)
    {
        return;
    }

    UpdateToNewState(m_SceneState->Update());
}

void ScenesManager::Notify(const unsigned, bae::Subject*, const std::any&)
{
}

bae::GameObject* ScenesManager::GetSelectionObject() const
{
    return m_SelectionObject;
}


void ScenesManager::UpdateToNewState(std::unique_ptr<States::SceneState> newState)
{
    if(!newState)
    {
        return;
    }

    m_SceneState->OnExit();
    m_SceneState = std::move(newState);
    m_SceneState->OnEnter();
}

