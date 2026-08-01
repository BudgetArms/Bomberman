#pragma once

#include <algorithm>
#include <iostream>

#include "Core/HelperFunctions.hpp"


namespace Game
{
    enum class Events
    {
        PlayerDied,
        DirectionChanged,
        GameWon,
        GameOver,
        LevelWon,
        LevelLost,
        BalloomDied,
        OnealDied,
        DollDied,
        MinvoDied,
        BeginLevel,
        RestartLevel,
        ScoreChanged,
        LivesChanged,
        LifeLost,
        CollisionEvent,
        SelectionUp,
        SelectionDown,
        SelectionLeft,
        SelectionRight,
        SelectionConfirmed,
        NoEvent,
    };


    inline const std::vector<std::pair<Events, unsigned int>> g_EventsMap
    {
        { Events::PlayerDied, bae::HashSDBM("PlayerDied") },
        { Events::DirectionChanged, bae::HashSDBM("DirectionChanged") },
        { Events::GameWon, bae::HashSDBM("GameWon") },
        { Events::GameOver, bae::HashSDBM("GameOver") },
        { Events::LevelWon, bae::HashSDBM("LevelWon") },
        { Events::LevelLost, bae::HashSDBM("LevelLost") },
        { Events::BalloomDied, bae::HashSDBM("BalloomDied") },
        { Events::OnealDied, bae::HashSDBM("OnealDied") },
        { Events::DollDied, bae::HashSDBM("DollDied") },
        { Events::MinvoDied, bae::HashSDBM("MinvoDied") },
        { Events::BeginLevel, bae::HashSDBM("BeginLevel") },
        { Events::RestartLevel, bae::HashSDBM("RestartLevel") },
        { Events::ScoreChanged, bae::HashSDBM("ScoreChanged") },
        { Events::LivesChanged, bae::HashSDBM("LivesChanged") },
        { Events::LifeLost, bae::HashSDBM("LifeLost") },
        { Events::CollisionEvent, bae::HashSDBM("CollisionEvent") },
        { Events::SelectionUp, bae::HashSDBM("SelectionUp") },
        { Events::SelectionDown, bae::HashSDBM("SelectionDown") },
        { Events::SelectionConfirmed, bae::HashSDBM("SelectionConfirmed") },
        { Events::NoEvent, bae::HashSDBM("NoEvent") },
    };


    constexpr unsigned int GetEventHash(const Events event)
    {
        const auto eventPairIt = std::ranges::find_if(g_EventsMap,
                                                      [&](auto pair)
                                                      {
                                                          const auto& eventPair = pair.first;
                                                          return event == eventPair;
                                                      });

        if(eventPairIt == g_EventsMap.end())
        {
            std::cout << FUNCTION_NAME << " Failed! Not Implemented" << '\n';
            return bae::HashSDBM("NoEvent");
        }

        return eventPairIt->second;
    }


    constexpr Events GetEvent(unsigned int eventHash)
    {
        const auto eventPairIt = std::ranges::find_if(g_EventsMap,
                                                      [&](auto pair)
                                                      {
                                                          const auto& eventHashPair = pair.second;
                                                          return eventHash == eventHashPair;
                                                      });

        if(eventPairIt == g_EventsMap.end())
        {
            std::cout << FUNCTION_NAME << " Failed! Not Implemented" << '\n';
            return Events::NoEvent;
        }

        return eventPairIt->first;
    }
}

