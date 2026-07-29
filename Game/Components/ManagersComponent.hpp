#pragma once

#include "Components/Component.hpp"


namespace Game
{
    class ManagersComponent final : public bae::Component
    {
    public:
        explicit ManagersComponent(bae::GameObject& owner);

        void Update() override;
        void FixedUpdate() override;
        void LateUpdate() override;
        void Render() const override;

    private:
    };
}
