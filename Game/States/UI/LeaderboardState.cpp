#include "LeaderboardState.hpp"

#include <fstream>

#include "Base/CommonManagerVariables.hpp"
#include "Base/Events.hpp"
#include "Components/TextComponent.hpp"
#include "Core/Renderer.hpp"
#include "Core/Scene.hpp"
#include "Managers/LevelManager.hpp"
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
    ClearScene();
    ClearCommands();
    AddSelectionConfirmedCommands();

    const bae::WindowSize windowSize = bae::Renderer::GetInstance().GetSDLWindowSize();

    auto singleplayerSection = std::make_unique<bae::GameObject>("Singleplayer Section");
    auto coOpSection         = std::make_unique<bae::GameObject>("Co-Op Section");
    auto versusSection       = std::make_unique<bae::GameObject>("Versus Section");

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
        auto titleGameObject = std::make_unique<bae::GameObject>(text);
        gameObject.AttachChild(*titleGameObject, false, false, false);
        titleGameObject->SetLocalLocation({ 0.f, -175.f });

        titleGameObject->AddComponent<bae::TextComponent>(*titleGameObject, text, titleFont);
        titleGameObject->GetComponent<bae::TextComponent>()->m_bIsCenteredAtPosition = true;

        leaderboardSceneName->Add(std::move(titleGameObject));
    };


    const auto entryFont = bae::ResourceManager::GetInstance().LoadFont("Fonts/JoystixMonospace.otf", 28);
    auto AddNameAndScore = [&](const ScoreData& data, const size_t index, bae::GameObject& gameObject)
    {
        std::string text{ data.Name };

        constexpr size_t textToBeOccupied = 11;
        const size_t textOccupied         = data.Name.size() + std::to_string(data.Score).size();
        if(textOccupied < textToBeOccupied)
        {
            for(size_t i{}; i < textToBeOccupied - textOccupied; ++i)
            {
                text += " ";
            }
        }
        text += std::to_string(data.Score);

        auto leaderboardEntryGameObject = std::make_unique<bae::GameObject>(text);
        gameObject.AttachChild(*leaderboardEntryGameObject, false, false, false);
        leaderboardEntryGameObject->SetLocalLocation({ 0.f, -125.f });
        leaderboardEntryGameObject->AddLocation({ 0.f, index * 30.f });

        leaderboardEntryGameObject->AddComponent<bae::TextComponent>(*leaderboardEntryGameObject, text, entryFont);
        leaderboardEntryGameObject->GetComponent<bae::TextComponent>()->m_bIsCenteredAtPosition = true;

        leaderboardSceneName->Add(std::move(leaderboardEntryGameObject));
    };

    AddTitle("Singleplayer", *singleplayerSection);
    AddTitle("Co-Op", *coOpSection);
    AddTitle("Versus", *versusSection);


    LoadSaveFileData();

    for(size_t i{}; i < m_SingleplayerData.size(); ++i)
    {
        AddNameAndScore(m_SingleplayerData[i], i, *singleplayerSection);
    }

    for(size_t i{}; i < m_CoOpData.size(); ++i)
    {
        AddNameAndScore(m_CoOpData[i], i, *coOpSection);
    }

    for(size_t i{}; i < m_VersusData.size(); ++i)
    {
        AddNameAndScore(m_VersusData[i], i, *versusSection);
    }


    leaderboardSceneName->Add(std::move(singleplayerSection));
    leaderboardSceneName->Add(std::move(coOpSection));
    leaderboardSceneName->Add(std::move(versusSection));


    const auto backFont = bae::ResourceManager::GetInstance().LoadFont("Fonts/JoystixMonospace.otf", 20);

    auto backButton = std::make_unique<bae::GameObject>("Back Button");
    backButton->SetWorldLocation({ centerScreen.x, 2.f * centerScreen.y - 50.f });

    backButton->AddComponent<bae::TextComponent>(*backButton, "Back", backFont);
    backButton->GetComponent<bae::TextComponent>()->m_bIsCenteredAtPosition = true;

    leaderboardSceneName->Add(std::move(backButton));
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

void LeaderboardState::Notify(const bae::EventData& eventData, bae::Subject*)
{
    const Events event = GetEvent(eventData.Hash);
    if(event == Events::SelectionConfirmed)
    {
        m_bIsSelected = true;
    }
}

void LeaderboardState::LoadSaveFileData()
{
    std::ifstream file(LevelManager::m_SaveFileName.data());
    if(!file)
    {
        return;
    }


    using json = nlohmann::json;
    json data{};
    file >> data;
    file.close();

    auto singleplayerData = data.at(LevelManager::m_SinglePlayerName.data());
    auto coOpData         = data.at(LevelManager::m_CoOpName.data());
    auto versusData       = data.at(LevelManager::m_VersusName.data());

    auto AddData = [&](const GameMode gameMode, std::vector<ScoreData>& dataVector)
    {
        std::string gameModeName{};
        switch(gameMode)
        {
            case GameMode::Singleplayer:
                gameModeName = LevelManager::m_SinglePlayerName.data();
                break;
            case GameMode::CoOp:
                gameModeName = LevelManager::m_CoOpName.data();
                break;
            case GameMode::Versus:
                gameModeName = LevelManager::m_VersusName.data();
                break;
        }

        for(const auto& scoreInfo : data.at(gameModeName))
        {
            const ScoreData entryData
            {
                scoreInfo[LevelManager::m_SaveEntryName.data()],
                scoreInfo[LevelManager::m_SaveEntryScore.data()]
            };

            dataVector.emplace_back(entryData);
        }

        std::ranges::sort(dataVector, [](const ScoreData& scoreOne, const ScoreData& scoreTwo)
        {
            return scoreOne.Score > scoreTwo.Score;
        });
    };

    AddData(GameMode::Singleplayer, m_SingleplayerData);
    AddData(GameMode::CoOp, m_CoOpData);
    AddData(GameMode::Versus, m_VersusData);
}
