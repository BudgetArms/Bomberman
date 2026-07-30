#include "ToggleMuteSoundsCommand.hpp"

#include "Commands/Command.hpp"
#include "Core/ServiceLocator.hpp"


using namespace Game;


ToggleMuteSoundsCommand::ToggleMuteSoundsCommand() :
    Command()
{
}

void ToggleMuteSoundsCommand::Execute()
{
    bae::SoundSystem& soundSystem = bae::ServiceLocator::GetSoundSystem();

    if(m_bIsMuted)
    {
        soundSystem.UnMuteAllSounds();
    }
    else
    {
        soundSystem.MuteAllSounds();
    }

    m_bIsMuted = !m_bIsMuted;
}

