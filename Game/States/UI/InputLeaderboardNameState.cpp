#include "InputLeaderboardNameState.hpp"

#include <fstream>

#include <nlohmann/json.hpp>

#include "Core/Renderer.hpp"
#include "Core/Scene.hpp"
#include "Managers/SceneManager.hpp"

#include "Base/CommonManagerVariables.hpp"
#include "Base/Events.hpp"
#include "Components/TextComponent.hpp"
#include "Managers/LevelManager.hpp"
#include "Managers/ResourceManager.hpp"
#include "States/UI/LeaderboardState.hpp"


using namespace Game::States;
using json = nlohmann::json;


InputLeaderboardNameState::InputLeaderboardNameState(bae::GameObject& selectionObject, const GameMode gameMode,
                                                     const int totalScore) :
    SceneState(selectionObject),
    m_GameMode{ gameMode },
    m_Score{ totalScore }
{
}

void InputLeaderboardNameState::OnEnter()
{
    ClearCommands();
    AddSelectionConfirmedCommands();
    AddSelectionDownUpCommands();
    AddSelectionLeftRightCommands();

    auto* inputLeaderboardNameScene  = bae::SceneManager::GetInstance().GetScene(g_ScenesManagerSceneName.data());
    const bae::WindowSize windowSize = bae::Renderer::GetInstance().GetSDLWindowSize();

    // Title
    const auto titleFont = bae::ResourceManager::GetInstance().LoadFont("Fonts/JoystixMonospace.otf", 49);
    auto titleObject     = std::make_unique<bae::GameObject>("Title");
    titleObject->SetWorldLocation({ windowSize.Width / 2.f, 50.f });

    titleObject->AddComponent<bae::TextComponent>(*titleObject, "Input Name", titleFont);
    titleObject->GetComponent<bae::TextComponent>()->m_bIsCenteredAtPosition = true;
    inputLeaderboardNameScene->Add(std::move(titleObject));


    const auto scoreFont = bae::ResourceManager::GetInstance().LoadFont("Fonts/JoystixMonospace.otf", 32);
    auto scoreObject     = std::make_unique<bae::GameObject>("Score");
    scoreObject->SetWorldLocation({ windowSize.Width / 2.f, 90.f });

    const std::string scoreText{ "Score: " + std::to_string(m_Score) };
    scoreObject->AddComponent<bae::TextComponent>(*scoreObject, scoreText, scoreFont);
    scoreObject->GetComponent<bae::TextComponent>()->m_bIsCenteredAtPosition = true;
    inputLeaderboardNameScene->Add(std::move(scoreObject));


    auto inputNameMenu = std::make_unique<bae::GameObject>("Input LeaderboardName Object");
    inputNameMenu->SetWorldLocation({
        static_cast<float>(windowSize.Width) / 2.f, static_cast<float>(windowSize.Height) / 2.f
    });


    auto letter0Object = std::make_unique<bae::GameObject>("Letter 0");
    auto letter1Object = std::make_unique<bae::GameObject>("Letter 1");
    auto letter2Object = std::make_unique<bae::GameObject>("Letter 2");
    auto letter3Object = std::make_unique<bae::GameObject>("Letter 3");
    auto letter4Object = std::make_unique<bae::GameObject>("Letter 4");

    constexpr float horizontalSpace = 800.f;

    letter0Object->SetLocalLocation({ -horizontalSpace / 2.f, 0 });
    letter1Object->SetLocalLocation({ -horizontalSpace / 4.f, 0 });
    letter2Object->SetLocalLocation({ 0.f, 0 });
    letter3Object->SetLocalLocation({ +horizontalSpace / 4.f, 0 });
    letter4Object->SetLocalLocation({ +horizontalSpace / 2.f, 0 });


    auto AddData = [&](const int index, bae::GameObject& gameObject)
    {
        // AttachChild, but don't freeze position
        inputNameMenu->AttachChild(gameObject, false);

        gameObject.SetWorldScale({ 4.f, 4.f });
        gameObject.AddComponent<bae::SpriteComponent>(gameObject, "Textures/Misc/Letters.png",
                                                      SDL_FRect(0.f, 0.f, 208.f, 8.f), 26, 26);

        m_LeaderboardName[index] = gameObject.GetComponent<bae::SpriteComponent>();
    };

    AddData(0, *letter0Object.get());
    AddData(1, *letter1Object.get());
    AddData(2, *letter2Object.get());
    AddData(3, *letter3Object.get());
    AddData(4, *letter4Object.get());


    auto selectorObject = std::make_unique<bae::GameObject>("Selector Object");
    m_SelectorObject    = selectorObject.get();

    auto AddSelectorData = [&](bae::GameObject& gameObject)
    {
        gameObject.AddComponent<bae::TextureComponent>(gameObject, "Textures/Misc/Selector.png");
        gameObject.SetWorldScale({ 2.f, 2.f });

        // AttachChild, but don't freeze position
        selectorObject->AttachChild(gameObject, true);
    };


    auto selectorDownObject  = std::make_unique<bae::GameObject>("Selector Down Object");
    auto selectorUpObject    = std::make_unique<bae::GameObject>("Selector Up Object");
    auto selectorLeftObject  = std::make_unique<bae::GameObject>("Selector Left Object");
    auto selectorRightObject = std::make_unique<bae::GameObject>("Selector Right Object");

    selectorDownObject->AddLocation({ -10.f, +30.f });
    selectorUpObject->AddLocation({ -10.f, -50.f });
    selectorLeftObject->AddLocation({ -50.f, -5.f });
    selectorRightObject->AddLocation({ +30.f, -5.f });

    selectorDownObject->SetLocalRotation(90.f);
    selectorUpObject->SetLocalRotation(-90.f);
    selectorLeftObject->SetLocalRotation(180.f);
    selectorRightObject->SetLocalRotation(0.f);

    AddSelectorData(*selectorDownObject);
    AddSelectorData(*selectorUpObject);
    AddSelectorData(*selectorLeftObject);
    AddSelectorData(*selectorRightObject);

    inputNameMenu->AttachChild(*selectorObject, false);

    m_SelectorObject = selectorObject.get();


    inputLeaderboardNameScene->Add(std::move(inputNameMenu));

    inputLeaderboardNameScene->Add(std::move(letter0Object));
    inputLeaderboardNameScene->Add(std::move(letter1Object));
    inputLeaderboardNameScene->Add(std::move(letter2Object));
    inputLeaderboardNameScene->Add(std::move(letter3Object));
    inputLeaderboardNameScene->Add(std::move(letter4Object));

    inputLeaderboardNameScene->Add(std::move(selectorObject));

    inputLeaderboardNameScene->Add(std::move(selectorDownObject));
    inputLeaderboardNameScene->Add(std::move(selectorUpObject));
    inputLeaderboardNameScene->Add(std::move(selectorLeftObject));
    inputLeaderboardNameScene->Add(std::move(selectorRightObject));


    UpdateSelector();
}

void InputLeaderboardNameState::OnExit()
{
    ClearCommands();
}

std::unique_ptr<SceneState> InputLeaderboardNameState::Update()
{
    if(!m_bIsSelected)
    {
        return nullptr;
    }

    SaveScore();

    return std::make_unique<LeaderboardState>(*m_GameObject);
}

void InputLeaderboardNameState::Notify(const bae::EventData& eventData, bae::Subject*)
{
    const Events event = GetEvent(eventData.Hash);

    if(event == Events::SelectionConfirmed)
    {
        m_bIsSelected = true;
        return;
    }

    if(event == Events::SelectionLeft)
    {
        if(m_LetterIndex == 0)
        {
            return;
        }

        --m_LetterIndex;
    }

    if(event == Events::SelectionRight)
    {
        if(m_LetterIndex >= m_NrLetters - 1)
        {
            return;
        }

        ++m_LetterIndex;
    }

    if(event == Events::SelectionUp)
    {
        if(m_LeaderboardName[m_LetterIndex]->m_Index == 0)
        {
            m_LeaderboardName[m_LetterIndex]->m_Index = 25;
            return;
        }

        --m_LeaderboardName[m_LetterIndex]->m_Index;
    }

    if(event == Events::SelectionDown)
    {
        if(m_LeaderboardName[m_LetterIndex]->m_Index == 25)
        {
            m_LeaderboardName[m_LetterIndex]->m_Index = 0;
            return;
        }

        ++m_LeaderboardName[m_LetterIndex]->m_Index;
    }

    UpdateSelector();
}

void InputLeaderboardNameState::UpdateSelector() const
{
    const glm::vec2 position = m_LeaderboardName[m_LetterIndex]->GetOwner()->GetWorldLocation();
    m_SelectorObject->SetWorldLocation(position);
}

void InputLeaderboardNameState::SaveScore() const
{
    std::ifstream file(LevelManager::m_SaveFileName.data());

    json data{};
    if(!file)
    {
        data[LevelManager::m_SinglePlayerName.data()] = json::array();
        data[LevelManager::m_CoOpName.data()]         = json::array();
        data[LevelManager::m_VersusName.data()]       = json::array();
    }
    else
    {
        file >> data;
    }

    file.close();


    std::string gameMode{};
    switch(m_GameMode)
    {
        case GameMode::Singleplayer:
            gameMode = LevelManager::m_SinglePlayerName;
            break;
        case GameMode::CoOp:
            gameMode = LevelManager::m_CoOpName;
            break;
        case GameMode::Versus:
            gameMode = LevelManager::m_VersusName;
            break;
    }

    const std::string leaderboardName = GetLeaderboardName();
    data[gameMode].push_back(
        {
            { LevelManager::m_SaveEntryName, leaderboardName },
            { LevelManager::m_SaveEntryScore, m_Score },
        }
    );

    std::ofstream outputFile(LevelManager::m_SaveFileName.data());
    outputFile << data.dump(4);
}

std::string InputLeaderboardNameState::GetLetter(const int letterIndex)
{
    return std::basic_string(1, static_cast<char>('A' + letterIndex));
}


std::string InputLeaderboardNameState::GetLeaderboardName() const
{
    std::string leaderboardName{};
    for(const auto& leaderboardSpriteSheet : m_LeaderboardName)
    {
        leaderboardName.append(GetLetter(leaderboardSpriteSheet->m_Index));
    }

    std::cout << FUNCTION_NAME << " " << leaderboardName << '\n';
    return leaderboardName;
}

