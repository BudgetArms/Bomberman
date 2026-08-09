#include "LeaderboardState.hpp"

#include "Base/CommonManagerVariables.hpp"
#include "Base/Events.hpp"
#include "Components/TextComponent.hpp"
#include "Core/Renderer.hpp"
#include "Core/Scene.hpp"
#include "Managers/ResourceManager.hpp"
#include "Managers/SceneManager.hpp"
#include "States/UI/MainMenuState.hpp"


using namespace Game::States;


LeaderboardState::LeaderboardState(bae::GameObject& selectionObject) :
    SceneState(selectionObject)
{
}

void LeaderboardState::OnEnter()
{
    ClearCommands();
    AddSelectionConfirmedCommands();

    const bae::WindowSize windowSize = bae::Renderer::GetInstance().GetSDLWindowSize();

    const auto singleplayerSection = std::make_shared<bae::GameObject>("Singleplayer Section");
    const auto coOpSection         = std::make_shared<bae::GameObject>("Co-Op Section");
    const auto versusSection       = std::make_shared<bae::GameObject>("Versus Section");

    constexpr float horizontalOffset{ 50.f };
    const glm::vec2 centerScreen
    {
        static_cast<float>(windowSize.Width) / 2.,
        static_cast<float>(windowSize.Height) / 2.f
    };
    singleplayerSection->SetWorldLocation({ 1 / 2.f * centerScreen.x - horizontalOffset, centerScreen.y });
    coOpSection->SetWorldLocation({ 2 / 2.f * centerScreen.x, centerScreen.y });
    versusSection->SetWorldLocation({ 3 / 2.f * centerScreen.x + horizontalOffset, centerScreen.y });


    auto* leaderboardSceneName = bae::SceneManager::GetInstance().GetScene(g_ScenesManagerSceneName.data());
    const auto titleFont       = bae::ResourceManager::GetInstance().LoadFont("Fonts/JoystixMonospace.otf", 30);

    auto AddTitle = [&](const std::string& text, bae::GameObject& gameObject)
    {
        const auto titleGameObject = std::make_shared<bae::GameObject>(text);
        gameObject.AttachChild(titleGameObject.get(), false, false, false);
        titleGameObject->SetLocalLocation({ 0.f, -175.f });

        titleGameObject->AddComponent<bae::TextComponent>(*titleGameObject, text, titleFont);
        titleGameObject->GetComponent<bae::TextComponent>()->m_bIsCenteredAtPosition = true;

        leaderboardSceneName->Add(titleGameObject);
    };


    const auto entryFont = bae::ResourceManager::GetInstance().LoadFont("Fonts/JoystixMonospace.otf", 28);
    auto AddNameAndScore = [&](const std::pair<std::string, int>& data, const int index, bae::GameObject& gameObject)
    {
        std::string text{ data.first };

        constexpr size_t textToBeOccupied = 11;
        const size_t textOccupied         = data.first.size() + std::to_string(data.second).size();
        if(textOccupied < textToBeOccupied)
        {
            for(int i{}; i < textToBeOccupied - textOccupied; ++i)
            {
                text += " ";
            }
        }
        text += std::to_string(data.second);

        const auto leaderboardEntryGameObject = std::make_shared<bae::GameObject>(text);
        gameObject.AttachChild(leaderboardEntryGameObject.get(), false, false, false);
        leaderboardEntryGameObject->SetLocalLocation({ 0.f, -125.f });
        leaderboardEntryGameObject->AddLocation({ 0.f, index * 30.f });

        leaderboardEntryGameObject->AddComponent<bae::TextComponent>(*leaderboardEntryGameObject, text, entryFont);
        leaderboardEntryGameObject->GetComponent<bae::TextComponent>()->m_bIsCenteredAtPosition = true;

        leaderboardSceneName->Add(leaderboardEntryGameObject);
    };

    AddTitle("Singleplayer", *singleplayerSection);
    AddTitle("Co-Op", *coOpSection);
    AddTitle("Versus", *versusSection);

    std::vector<std::pair<std::string, int>> data
    {
        { "ABCDE", 012345 },
        { "FGHIJ", 67890 },
        { "FORTN", 80085 },
        { "EPICG", 1991 },
        { "NICKI", 24 },
        { "BUDGT", 23 },
        { "MATLD", 22 },
        { "MHAEL", 20 },
        { "SMILE", 1 },
        { "NAAAT", 0 },
    };

    for(int i{}; i < data.size(); ++i)
    {
        AddNameAndScore({ data[i].first, data[i].second }, i, *singleplayerSection);
        AddNameAndScore({ data[i].first, data[i].second }, i, *coOpSection);
        AddNameAndScore({ data[i].first, data[i].second }, i, *versusSection);
    }

    leaderboardSceneName->Add(singleplayerSection);
    leaderboardSceneName->Add(coOpSection);
    leaderboardSceneName->Add(versusSection);
}

void LeaderboardState::OnExit()
{
    ClearCommands();
}

std::unique_ptr<SceneState> LeaderboardState::Update()
{
    if(!m_bIsSelected)
    {
        return nullptr;
    }

    return std::make_unique<MainMenuState>(*m_GameObject);
}

void LeaderboardState::Notify(const unsigned eventHash, bae::Subject*, const std::any&)
{
    const Events event = GetEvent(eventHash);
    if(event == Events::SelectionConfirmed)
    {
        m_bIsSelected = true;
    }
}

