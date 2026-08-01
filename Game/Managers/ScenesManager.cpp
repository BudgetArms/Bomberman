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


    const auto selectionObject = std::make_shared<bae::GameObject>("Selection Object");
    m_SelectionObject          = selectionObject.get();

    bae::Scene* scene = bae::SceneManager::GetInstance().GetScene(g_ScenesManagerSceneName.data());
    scene->Add(selectionObject);
}

void ScenesManager::Notify(const unsigned, bae::Subject*, const std::any&)
{
}

bae::GameObject* ScenesManager::GetSelectionObject() const
{
    return m_SelectionObject;
}

