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
        bae::GameObject* m_GameObject;

        static constexpr SDL_Keycode m_KeySelectionConfirmation = SDLK_J;
        static constexpr SDL_Keycode m_keySelectionDown         = SDLK_S;
        static constexpr SDL_Keycode m_keySelectionUp           = SDLK_W;
        static constexpr SDL_Keycode m_keySelectionLeft         = SDLK_A;
        static constexpr SDL_Keycode m_keySelectionRight        = SDLK_D;

        #if WIN32
        static constexpr unsigned int m_ControllerSelectionConfirmation = XINPUT_GAMEPAD_A;
        static constexpr unsigned int m_ControllerSelectionDown         = XINPUT_GAMEPAD_DPAD_DOWN;
        static constexpr unsigned int m_ControllerSelectionUp           = XINPUT_GAMEPAD_DPAD_UP;
        static constexpr unsigned int m_ControllerSelectionLeft         = XINPUT_GAMEPAD_DPAD_LEFT;
        static constexpr unsigned int m_ControllerSelectionRight        = XINPUT_GAMEPAD_DPAD_RIGHT;
        #endif
    };


    class MainMenuState final : public SceneState, public bae::Observer
    {
    public:
        explicit MainMenuState(bae::GameObject& selectionObject);

        void OnEnter() override;
        void OnExit() override;

        std::unique_ptr<SceneState> Update() override;

        void Notify(unsigned eventHash, bae::Subject* subject, const std::any& eventData) override;

    private:
        enum class StartMenuSelections
        {
            Play,
            Leaderboard,
            Quit
        };

        StartMenuSelections m_Selection{ StartMenuSelections::Play };
        bool m_bIsSelected{};
    };


    class GameModeSelectionState final : public SceneState, public bae::Observer
    {
    public:
        explicit GameModeSelectionState(bae::GameObject& selectionObject);

        void OnEnter() override;
        void OnExit() override;

        std::unique_ptr<SceneState> Update() override;

        void Notify(unsigned eventHash, bae::Subject* subject, const std::any& eventData) override;

    private:
        enum class GameModeSelection
        {
            SinglePlayer,
            CoOp,
            Versus,
            Back
        };

        GameModeSelection m_Selection{ GameModeSelection::SinglePlayer };
        bool m_bIsSelected{};
    };


    class GameState final : public SceneState, public bae::EventListener
    {
    public:
        explicit GameState(bae::GameObject& selectionObject);

        void OnEnter() override;
        void OnExit() override;

        std::unique_ptr<SceneState> Update() override;

        void HandleEvent(unsigned eventHash) override;

    private:
        bool m_bIsGameFinished{};
    };


    class LeaderboardState final : public SceneState, public bae::Observer
    {
    public:
        explicit LeaderboardState(bae::GameObject& selectionObject);

        void OnEnter() override;
        void OnExit() override;

        std::unique_ptr<SceneState> Update() override;

        void Notify(unsigned eventHash, bae::Subject* subject, const std::any& eventData) override;

    private:
        bool m_bIsSelected{};
    };


    class InputLeaderboardNameState final : public SceneState, public bae::Observer
    {
    public:
        explicit InputLeaderboardNameState(bae::GameObject& selectionObject);

        void OnEnter() override;
        void OnExit() override;

        std::unique_ptr<SceneState> Update() override;

        void Notify(unsigned eventHash, bae::Subject* subject, const std::any& eventData) override;

    private:
        static std::string GetLetter(int letterIndex);
        std::string GetLeaderboardName() const;

        std::array<std::unique_ptr<bae::SpriteSheet>, 5> m_LeaderboardName{};


        bool m_bIsSelected{};

        int m_LetterIndex{};
        const int m_NrLetters{ 5 };
    };
}
