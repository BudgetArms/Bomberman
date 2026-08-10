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
    auto AddNameAndScore = [&](const ScoreData& data, const int index, bae::GameObject& gameObject)
    {
        std::string text{ data.Name };

        constexpr size_t textToBeOccupied = 11;
        const size_t textOccupied         = data.Name.size() + std::to_string(data.Score).size();
        if(textOccupied < textToBeOccupied)
        {
            for(int i{}; i < textToBeOccupied - textOccupied; ++i)
            {
                text += " ";
            }
        }
        text += std::to_string(data.Score);

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


    LoadSaveFileData();

    for(int i{}; i < m_SingleplayerData.size(); ++i)
    {
        AddNameAndScore(m_SingleplayerData[i], i, *singleplayerSection);
    }

    for(int i{}; i < m_CoOpData.size(); ++i)
    {
        AddNameAndScore(m_CoOpData[i], i, *coOpSection);
    }

    for(int i{}; i < m_VersusData.size(); ++i)
    {
        AddNameAndScore(m_VersusData[i], i, *versusSection);
    }


    leaderboardSceneName->Add(singleplayerSection);
    leaderboardSceneName->Add(coOpSection);
    leaderboardSceneName->Add(versusSection);


    const auto backFont = bae::ResourceManager::GetInstance().LoadFont("Fonts/JoystixMonospace.otf", 20);

    const auto backButton = std::make_shared<bae::GameObject>("Back Button");
    backButton->SetWorldLocation({ centerScreen.x, 2.f * centerScreen.y - 50.f });

    backButton->AddComponent<bae::TextComponent>(*backButton, "Back", backFont);
    backButton->GetComponent<bae::TextComponent>()->m_bIsCenteredAtPosition = true;

    leaderboardSceneName->Add(backButton);
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

