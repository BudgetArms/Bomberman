#include "InputLeaderboardNameState.hpp"

#include "Base/CommonManagerVariables.hpp"
#include "Core/Renderer.hpp"

#include "Base/Events.hpp"
#include "Core/Scene.hpp"
#include "Managers/SceneManager.hpp"
#include "States/UI/MainMenuState.hpp"


using namespace Game::States;


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


    const bae::WindowSize windowSize = bae::Renderer::GetInstance().GetSDLWindowSize();

    const auto inputNameMenu = std::make_shared<bae::GameObject>("Input LeaderboardName Object");
    inputNameMenu->SetWorldLocation({
        static_cast<float>(windowSize.Width) / 2.f, static_cast<float>(windowSize.Height) / 2.f
    });

    const auto letter0Object = std::make_shared<bae::GameObject>("Letter 0");
    const auto letter1Object = std::make_shared<bae::GameObject>("Letter 1");
    const auto letter2Object = std::make_shared<bae::GameObject>("Letter 2");
    const auto letter3Object = std::make_shared<bae::GameObject>("Letter 3");
    const auto letter4Object = std::make_shared<bae::GameObject>("Letter 4");

    constexpr float horizontalSpace = 800.f;

    letter0Object->SetLocalLocation({ -horizontalSpace / 2.f, 0 });
    letter1Object->SetLocalLocation({ -horizontalSpace / 4.f, 0 });
    letter2Object->SetLocalLocation({ 0.f, 0 });
    letter3Object->SetLocalLocation({ +horizontalSpace / 4.f, 0 });
    letter4Object->SetLocalLocation({ +horizontalSpace / 2.f, 0 });


    auto AddData = [&](const int index, bae::GameObject& gameObject)
    {
        // AttachChild, but don't freeze position
        inputNameMenu->AttachChild(&gameObject, false);

        gameObject.SetWorldScale({ 4.f, 4.f });
        gameObject.AddComponent<bae::SpriteComponent>(gameObject, "Textures/Misc/Letters.png",
                                                      SDL_FRect(0.f, 0.f, 208.f, 8.f), 26, 26);

        m_LeaderboardName[index] = gameObject.GetComponent<bae::SpriteComponent>();
        std::cout << "Position Letter " << index << ", x:" << gameObject.GetWorldLocation().x
                << ", y:" << gameObject.GetWorldLocation().y << '\n';
    };

    AddData(0, *letter0Object.get());
    AddData(1, *letter1Object.get());
    AddData(2, *letter2Object.get());
    AddData(3, *letter3Object.get());
    AddData(4, *letter4Object.get());


    const auto selectorObject = std::make_shared<bae::GameObject>("Selector Object");
    m_SelectorObject          = selectorObject.get();

    auto AddSelectorData = [&](bae::GameObject& gameObject)
    {
        gameObject.AddComponent<bae::TextureComponent>(gameObject, "Textures/Misc/Selector.png");
        gameObject.SetWorldScale({ 2.f, 2.f });

        // AttachChild, but don't freeze position
        selectorObject->AttachChild(&gameObject, true);
    };

    const auto selectorDownObject  = std::make_shared<bae::GameObject>("Selector Down Object");
    const auto selectorUpObject    = std::make_shared<bae::GameObject>("Selector Up Object");
    const auto selectorLeftObject  = std::make_shared<bae::GameObject>("Selector Left Object");
    const auto selectorRightObject = std::make_shared<bae::GameObject>("Selector Right Object");

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

    inputNameMenu->AttachChild(selectorObject.get(), false);

    auto* inputLeaderboardNameScene = bae::SceneManager::GetInstance().GetScene(g_ScenesManagerSceneName.data());

    inputLeaderboardNameScene->Add(inputNameMenu);

    inputLeaderboardNameScene->Add(letter0Object);
    inputLeaderboardNameScene->Add(letter1Object);
    inputLeaderboardNameScene->Add(letter2Object);
    inputLeaderboardNameScene->Add(letter3Object);
    inputLeaderboardNameScene->Add(letter4Object);

    inputLeaderboardNameScene->Add(selectorObject);

    inputLeaderboardNameScene->Add(selectorDownObject);
    inputLeaderboardNameScene->Add(selectorUpObject);
    inputLeaderboardNameScene->Add(selectorLeftObject);
    inputLeaderboardNameScene->Add(selectorRightObject);

    m_SelectorObject = selectorObject.get();

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

    // todo: add the score to the score save file
    [[maybe_unused]] const std::string leaderboardName = GetLeaderboardName();
    std::cout << leaderboardName << '\n';


    return std::make_unique<MainMenuState>(*m_GameObject);
}

void InputLeaderboardNameState::Notify(const unsigned eventHash, bae::Subject*, const std::any&)
{
    const Events event = GetEvent(eventHash);

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

