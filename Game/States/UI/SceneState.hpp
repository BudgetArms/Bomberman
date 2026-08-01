#pragma once

#if WIN32
#include <Windows.h>
#include <XInput.h>
#endif

#include <array>
#include <memory>


#include <SDL3/SDL_keycode.h>

#include "Core/EventListener.hpp"
#include "Core/GameObject.hpp"
#include "Core/Observer.hpp"
#include "Core/SpriteSheet.hpp"


namespace Game::States
{
    class SceneState
    {
    public:
        explicit SceneState(bae::GameObject& selectionObject);
        virtual ~SceneState() = default;

        virtual void OnEnter() = 0;
        virtual void OnExit() = 0;

        virtual std::unique_ptr<SceneState> Update() = 0;

    protected:
        static void ClearCommands();

        static void AddSelectionConfirmedCommands();
        static void AddSelectionDownUpCommands();
        static void AddSelectionLeftRightCommands();


        bae::GameObject* m_GameObject;

        static constexpr SDL_Keycode m_KeySelectionConfirmation = SDLK_J;
        static constexpr SDL_Keycode m_KeySelectionDown         = SDLK_S;
        static constexpr SDL_Keycode m_KeySelectionUp           = SDLK_W;
        static constexpr SDL_Keycode m_KeySelectionLeft         = SDLK_A;
        static constexpr SDL_Keycode m_KeySelectionRight        = SDLK_D;

        #if WIN32
        static constexpr unsigned int m_ControllerSelectionConfirmation = XINPUT_GAMEPAD_A;
        static constexpr unsigned int m_ControllerSelectionDown         = XINPUT_GAMEPAD_DPAD_DOWN;
        static constexpr unsigned int m_ControllerSelectionUp           = XINPUT_GAMEPAD_DPAD_UP;
        static constexpr unsigned int m_ControllerSelectionLeft         = XINPUT_GAMEPAD_DPAD_LEFT;
        static constexpr unsigned int m_ControllerSelectionRight        = XINPUT_GAMEPAD_DPAD_RIGHT;
        #endif
    };
}
