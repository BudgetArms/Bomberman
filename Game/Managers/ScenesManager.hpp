#pragma once

#include "Core/GameObject.hpp"
#include "Core/Observer.hpp"
#include "Singletons/Singleton.hpp"

#include "States/UI/SceneState.hpp"


namespace Game
{
    class ScenesManager final : public bae::Singleton<ScenesManager>, public bae::Observer
    {
    public:
        void Initialize();

        void Update();

        void Notify(unsigned eventHash, bae::Subject* subject, const std::any& eventData) override;

        bae::GameObject* GetSelectionObject() const;

    private:
        void UpdateToNewState(std::unique_ptr<States::SceneState> newState);


        bae::GameObject* m_SelectionObject{};

        std::unique_ptr<States::SceneState> m_SceneState{};

        bool m_bIsInitialized{};
    };
}

