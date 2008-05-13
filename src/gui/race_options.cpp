//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2006 SuperTuxKart-Team
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include "race_manager.hpp"
#include "race_options.hpp"
#include "widget_manager.hpp"
#include "user_config.hpp"
#include "menu_manager.hpp"
#include "material_manager.hpp"
#include "unlock_manager.hpp"
#include "translation.hpp"
#if defined(WIN32) && !defined(__CYGWIN__)
#  define snprintf _snprintf
#endif

enum WidgetTokens
{
    WTOK_TITLE,

    WTOK_DIFFICULTY_TITLE,
    WTOK_DIFFICULTY_UP,
    WTOK_DIFFICULTY,
    WTOK_DIFFICULTY_DOWN,

    WTOK_KARTS_TITLE,
    WTOK_KARTS_UP,
    WTOK_KARTS,
    WTOK_KARTS_DOWN,

    WTOK_LAPS_TITLE,
    WTOK_LAPS_UP,
    WTOK_LAPS,
    WTOK_LAPS_DOWN,

    WTOK_START,

    WTOK_QUIT
};

RaceOptions::RaceOptions() :
             m_difficulty(race_manager->getDifficulty()),
             m_num_karts(race_manager->getNumKarts()),
             m_num_laps(race_manager->getNumLaps())
{
    // Difficulty
    // ==========
    widget_manager->switchOrder();
    widget_manager->addTextWgt( WTOK_DIFFICULTY_TITLE, 38, 7, _("Difficulty") );
    widget_manager->switchOrder();
    widget_manager->addTextButtonWgt( WTOK_DIFFICULTY_DOWN, 3, 7, _("<") );

    widget_manager->addTextWgt( WTOK_DIFFICULTY, 32, 7, getDifficultyString(m_difficulty));
    widget_manager->setWgtBorderPercentage( WTOK_DIFFICULTY, 10 );
    widget_manager->showWgtBorder( WTOK_DIFFICULTY );
    widget_manager->hideWgtRect( WTOK_DIFFICULTY );

    widget_manager->addTextButtonWgt( WTOK_DIFFICULTY_UP, 3, 7, ">" );

    widget_manager->breakLine();
    widget_manager->breakLine();
    widget_manager->addEmptyWgt( WidgetManager::WGT_NONE, 2, 1);

    // Number of karts
    // ===============
    widget_manager->switchOrder();
    widget_manager->addTextWgt( WTOK_KARTS_TITLE, 38, 7, _("Number of karts") );
    widget_manager->switchOrder();
    widget_manager->addTextButtonWgt( WTOK_KARTS_DOWN, 3, 7, "<" );

    char string_num_karts[MAX_MESSAGE_LENGTH];
    snprintf(string_num_karts, MAX_MESSAGE_LENGTH, "%d", m_num_karts);
    widget_manager->addTextWgt( WTOK_KARTS, 32, 7, string_num_karts );
    widget_manager->setWgtBorderPercentage( WTOK_KARTS, 10 );
    widget_manager->showWgtBorder( WTOK_KARTS );
    widget_manager->hideWgtRect( WTOK_KARTS );

    widget_manager->addTextButtonWgt( WTOK_KARTS_UP, 3, 7, ">" );

    widget_manager->breakLine();
    widget_manager->breakLine();
    widget_manager->breakLine();

    widget_manager->addEmptyWgt( WidgetManager::WGT_NONE, 2, 1);
    widget_manager->breakLine();

    // Number of laps
    // ==============
    if( race_manager->getRaceMode() != RaceManager::RM_GRAND_PRIX   &&
        race_manager->getRaceMode() != RaceManager::RM_FOLLOW_LEADER   )
    {
        widget_manager->switchOrder();
        widget_manager->addTextWgt( WTOK_LAPS_TITLE, 38, 7, _("Number of laps") );
        widget_manager->switchOrder();
        widget_manager->addTextButtonWgt( WTOK_LAPS_DOWN, 3, 7, "<" );

        char string_num_laps[MAX_MESSAGE_LENGTH];
        snprintf(string_num_laps, MAX_MESSAGE_LENGTH, "%d", m_num_laps);
        widget_manager->addTextWgt( WTOK_LAPS, 32, 7, string_num_laps );
        widget_manager->setWgtBorderPercentage( WTOK_LAPS, 10 );
        widget_manager->showWgtBorder( WTOK_LAPS );
        widget_manager->hideWgtRect( WTOK_LAPS );

        widget_manager->addTextButtonWgt( WTOK_LAPS_UP, 3, 7, ">" );
        widget_manager->breakLine();
        widget_manager->breakLine();
    }

    widget_manager->breakLine();

    //Lower buttons
    //=============
    widget_manager->switchOrder();
    widget_manager->addEmptyWgt( WidgetManager::WGT_NONE, 1, 10);

    widget_manager->addTextButtonWgt( WTOK_START, 60, 7, _("Start race") );
    widget_manager->setWgtColor( WTOK_START, WGT_TRANS_RED );

    widget_manager->addEmptyWgt( WidgetManager::WGT_NONE, 1, 10);

    widget_manager->addTextButtonWgt( WTOK_QUIT, 60, 7, _("Press <ESC> to go back") );

    widget_manager->layout(WGT_AREA_ALL);
}   // RaceOptions

//-----------------------------------------------------------------------------
RaceOptions::~RaceOptions()
{
    widget_manager->reset();
}   // ~RaceOptions

//-----------------------------------------------------------------------------
void RaceOptions::select()
{
    switch ( widget_manager->getSelectedWgt() )
    {
        case WTOK_DIFFICULTY_UP:
            if( m_difficulty == RaceManager::RD_HARD && !unlock_manager->isLocked("skidding"))
            {
                m_difficulty = RaceManager::RD_SKIDDING;
            }
            else if( m_difficulty == RaceManager::RD_MEDIUM )
            {
                m_difficulty = RaceManager::RD_HARD;
            }
            else if( m_difficulty == RaceManager::RD_EASY )
            {
//TEMP: done just for the release after 0.4 because of AI problems
#undef ENABLE_MEDIUM_AI
#if ENABLE_MEDIUM_AI
                m_difficulty = RaceManager::RD_MEDIUM;
#else
                m_difficulty = RaceManager::RD_HARD;
#endif
            }
            widget_manager->setWgtText( WTOK_DIFFICULTY, getDifficultyString(m_difficulty) );
            break;

        case WTOK_DIFFICULTY_DOWN:
            if( m_difficulty == RaceManager::RD_SKIDDING )
            {
                m_difficulty = RaceManager::RD_HARD;
            }

            else if( m_difficulty == RaceManager::RD_HARD )
            {
//TEMP: done just for the release after 0.4 because of AI problems
#if ENABLE_MEDIUM_AI
                m_difficulty = RaceManager::RD_MEDIUM;
#else
                m_difficulty = RaceManager::RD_EASY;
#endif
            }
            else if( m_difficulty == RaceManager::RD_MEDIUM )
            {
                m_difficulty = RaceManager::RD_EASY;
            }
            widget_manager->setWgtText( WTOK_DIFFICULTY, getDifficultyString(m_difficulty) );

            break;

        case WTOK_KARTS_UP:
            {
                m_num_karts = std::min( stk_config->m_max_karts, m_num_karts + 1 );

                char label[ MAX_MESSAGE_LENGTH ];
                snprintf( label, MAX_MESSAGE_LENGTH, "%d", m_num_karts );

                widget_manager->setWgtText( WTOK_KARTS, label );
            }
            break;

        case WTOK_KARTS_DOWN:
            {
                // Follow the leader needs at least three karts
                if(race_manager->getRaceMode() == RaceManager::RM_FOLLOW_LEADER)
                {
                    m_num_karts = std::max( 3, m_num_karts - 1 );
                }
                else
                {
                    m_num_karts = std::max( (int)race_manager->getNumPlayers(),
                        m_num_karts - 1 );
                }

                char label[ MAX_MESSAGE_LENGTH ];
                snprintf( label, MAX_MESSAGE_LENGTH, "%d", m_num_karts );

                widget_manager->setWgtText( WTOK_KARTS, label );
            }
            break;

        case WTOK_LAPS_UP:
            {
                m_num_laps = std::min( 10, m_num_laps + 1 );

                char label[ MAX_MESSAGE_LENGTH ];
                snprintf( label, MAX_MESSAGE_LENGTH, "%d", m_num_laps );

                widget_manager->setWgtText( WTOK_LAPS, label);
            }
            break;

        case WTOK_LAPS_DOWN:
            {
                m_num_laps = std::max( 1, m_num_laps - 1 );

                char label[ MAX_MESSAGE_LENGTH ];
                snprintf( label, MAX_MESSAGE_LENGTH, "%d", m_num_laps );

                widget_manager->setWgtText( WTOK_LAPS, label);
            }
            break;

    case WTOK_START:
        if( m_difficulty >= RaceManager::RD_EASY &&
            m_difficulty <= RaceManager::RD_SKIDDING)
        {
            race_manager->setDifficulty((RaceManager::Difficulty)m_difficulty);
        }
        else // invalid difficulty
        {
            race_manager->setDifficulty( RaceManager::RD_EASY );
        }

        race_manager->setNumKarts(m_num_karts);

        if( race_manager->getRaceMode() != RaceManager::RM_GRAND_PRIX    &&
            race_manager->getRaceMode() != RaceManager::RM_FOLLOW_LEADER    )
        {
            race_manager->setNumLaps( m_num_laps );
        }

        menu_manager->pushMenu(MENUID_START_RACE_FEEDBACK);
        break;
    case WTOK_QUIT:
        menu_manager->popMenu();
        break;
    default: break;
    }   // switch
}   // select

// ----------------------------------------------------------------------------
const char *RaceOptions::getDifficultyString(int difficulty) const
{
    switch(difficulty)
    {
    case RaceManager::RD_EASY:     return _("Novice");
    case RaceManager::RD_MEDIUM:   return _("Driver");
    case RaceManager::RD_HARD:     return _("Racer" );
    case RaceManager::RD_SKIDDING: return _("Skidding Preview");
    default:                       return _("Novice");

    }   // switch
}   // getDifficultyString
