//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2013 Glenn De Jonghe
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 3
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

#include "states_screens/dialogs/registration_dialog.hpp"

#include <IGUIEnvironment.h>

#include "audio/sfx_manager.hpp"
#include "config/player.hpp"
#include "guiengine/engine.hpp"
#include "states_screens/state_manager.hpp"
#include "utils/translation.hpp"
#include "utils/string_utils.hpp"
#include "online/messages.hpp"


using namespace GUIEngine;
using namespace irr;
using namespace irr::gui;
using namespace Online;

// -----------------------------------------------------------------------------

RegistrationDialog::RegistrationDialog(const Phase phase) :
        ModalDialog(0.8f,0.9f)
{
    m_sign_up_request = NULL;
    m_activation_request = NULL;
    m_self_destroy = false;
    m_show_registration_info = false;
    m_show_registration_terms = false;
    m_show_registration_activation = false;
    m_username = "";
    m_email = "";
    m_email_confirm = "";
    m_password = "";
    m_password_confirm = "";
    m_agreement = false;
    //If not asked for the Activation phase, default to the Info phase.
    if (phase == Activation)
        m_show_registration_activation = true;
    else
        m_show_registration_info = true;
}

// -----------------------------------------------------------------------------

RegistrationDialog::~RegistrationDialog()
{
    delete m_sign_up_request;
    delete m_activation_request;
}

// -----------------------------------------------------------------------------

void RegistrationDialog::showRegistrationInfo(){
    m_show_registration_info = false;
    clearWindow();
    m_phase = Info;
    loadFromFile("online/registration_info.stkgui");

    //Password should always be reentered if previous has been clicked, or an error occurred.
    m_password = "";
    m_password_confirm = "";

    TextBoxWidget* textBox = getWidget<TextBoxWidget>("username");
    assert(textBox != NULL);
    textBox->setText(m_username);
    textBox->setFocusForPlayer(PLAYER_ID_GAME_MASTER);

    textBox = getWidget<TextBoxWidget>("password");
    assert(textBox != NULL);
    textBox->setPasswordBox(true,L'*');

    textBox = getWidget<TextBoxWidget>("password_confirm");
    assert(textBox != NULL);
    textBox->setPasswordBox(true,L'*');

    textBox = getWidget<TextBoxWidget>("email");
    assert(textBox != NULL);
    textBox->setText(m_email);

    textBox = getWidget<TextBoxWidget>("email_confirm");
    assert(textBox != NULL);
    textBox->setText(m_email_confirm);

    LabelWidget* label = getWidget<LabelWidget>("info");
    assert(label != NULL);
    label->setText(m_registration_error, false);

    ButtonWidget * button = getWidget<ButtonWidget>("next");
    assert(button != NULL);

    button = getWidget<ButtonWidget>("cancel");
    assert(button != NULL);

}

// -----------------------------------------------------------------------------

void RegistrationDialog::showRegistrationTerms(){
    m_show_registration_terms = false;
    clearWindow();
    m_phase = Terms;
    loadFromFile("online/registration_terms.stkgui");
    CheckBoxWidget * checkbox = getWidget<CheckBoxWidget>("accepted");
    assert(checkbox != NULL);
    checkbox->setState(false);
    checkbox->setFocusForPlayer(PLAYER_ID_GAME_MASTER); //FIXME set focus on the terms
    ButtonWidget * submitButton = getWidget<ButtonWidget>("next");
    assert(submitButton != NULL);
    submitButton->setDeactivated();
}

// -----------------------------------------------------------------------------

void RegistrationDialog::showRegistrationActivation(){
    m_show_registration_activation = false;
    clearWindow();
    m_phase = Activation;
    loadFromFile("online/registration_activation.stkgui");
}

// -----------------------------------------------------------------------------

bool RegistrationDialog::processInfoEvent(const std::string& eventSource){
    if (m_phase == Info)
    {
        if (eventSource == "next")
        {
            m_username = getWidget<TextBoxWidget>("username")->getText().trim();
            m_password = getWidget<TextBoxWidget>("password")->getText().trim();
            m_password_confirm =  getWidget<TextBoxWidget>("password_confirm")->getText().trim();
            m_email = getWidget<TextBoxWidget>("email")->getText().trim();
            m_email_confirm = getWidget<TextBoxWidget>("email_confirm")->getText().trim();
            LabelWidget * info_widget = getWidget<LabelWidget>("info");
            //FIXME More validation of registration information
            info_widget->setErrorColor();
            if (m_password != m_password_confirm)
            {
                info_widget->setText(_("Passwords don't match!"), false);
            }
            else if (m_email != m_email_confirm)
            {
                info_widget->setText(_("Emails don't match!"), false);
            }
            else if (m_username.size() < 4 || m_username.size() > 30)
            {
                info_widget->setText(_("Username has to be between 4 and 30 characters long!"), false);
            }
            else if (m_password.size() < 8 || m_password.size() > 30)
            {
                info_widget->setText(_("Password has to be between 8 and 30 characters long!"), false);
            }
            else if (m_email.size() < 4 || m_email.size() > 50)
            {
                info_widget->setText(_("Email has to be between 4 and 50 characters long!"), false);
            }
            else
            {
                m_show_registration_terms = true;
                return true;
            }
            sfx_manager->quickSound( "anvil" );
            return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------

bool RegistrationDialog::processTermsEvent(const std::string& eventSource){
    if (m_phase == Terms)
    {
        if (eventSource == "next")
        {
            assert(getWidget<CheckBoxWidget>("accepted")->getState());
            m_sign_up_request = CurrentUser::acquire()->requestSignUp(m_username, m_password, m_password_confirm, m_email, true);
            CurrentUser::release();
            return true;
        }
        else if (eventSource == "accepted")
        {
            CheckBoxWidget * checkbox = getWidget<CheckBoxWidget>("accepted");
            bool new_state = !checkbox->getState();
            checkbox->setState(new_state);
            ButtonWidget * submitButton = getWidget<ButtonWidget>("next");
            if(new_state)
                submitButton->setActivated();
            else
                submitButton->setDeactivated();
            return true;
        }
        else if (eventSource == "previous")
        {
            m_show_registration_info = true;
            return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------

bool RegistrationDialog::processActivationEvent(const std::string& eventSource){
    if (m_phase == Activation)
    {
        if (eventSource == "next")
        {
            //FIXME : activate
            m_self_destroy = true;
            return true;
        }
    }
    return false;
}


// -----------------------------------------------------------------------------

GUIEngine::EventPropagation RegistrationDialog::processEvent(const std::string& eventSource)
{
    if (eventSource == "cancel")
    {
        m_self_destroy = true;
        return GUIEngine::EVENT_BLOCK;
    }
    else if (processInfoEvent(eventSource) || processTermsEvent(eventSource) || processActivationEvent(eventSource))
    {
        return GUIEngine::EVENT_BLOCK;
    }
    return GUIEngine::EVENT_LET;
}

// -----------------------------------------------------------------------------

void RegistrationDialog::onEnterPressedInternal()
{
    //If enter was pressed while no button was focused, then interpret as "next" press.
    const int playerID = PLAYER_ID_GAME_MASTER;
    bool interpret_as_next = true;
    ButtonWidget * cancel_widget = getWidget<ButtonWidget>("cancel");
    ButtonWidget * next_widget = getWidget<ButtonWidget>("next");
    interpret_as_next = interpret_as_next &&
                        !GUIEngine::isFocusedForPlayer(next_widget, playerID) &&
                        !GUIEngine::isFocusedForPlayer(cancel_widget, playerID);
    if (interpret_as_next && m_phase == Terms)
    {
        ButtonWidget * previous_widget = getWidget<ButtonWidget>("previous");
        interpret_as_next = interpret_as_next && !GUIEngine::isFocusedForPlayer(previous_widget, playerID);
    }
    if (interpret_as_next)
    {
        processEvent("next");
    }
}

// -----------------------------------------------------------------------------

void RegistrationDialog::onUpdate(float dt)
{
    if (m_phase == Terms)
    {
        if(m_sign_up_request  != NULL)
        {
            if(m_sign_up_request->isDone())
            {
                if(m_sign_up_request->isSuccess())
                {
                    m_show_registration_activation = true;
                }
                else
                {
                    sfx_manager->quickSound( "anvil" );
                    m_show_registration_info = true;
                    m_registration_error = m_sign_up_request->getInfo();
                }
                delete m_sign_up_request;
                m_sign_up_request = NULL;
                //FIXME m_options_widget->setActivated();
            }
            else
            {
                LabelWidget* label = getWidget<LabelWidget>("info");
                assert(label != NULL);
                label->setDefaultColor();
                label->setText(Messages::signingUp(), false);
            }
        }
    }
    else if (m_phase == Activation)
    {
        /*
        XMLRequest * sign_up_request = HTTPManager::get()->getXMLResponse(Request::RT_SIGN_UP);
        if(sign_up_request != NULL)
        {
            if(sign_up_request->isSuccess())
            {
                m_show_registration_activation = true;
            }
            else
            {
                sfx_manager->quickSound( "anvil" );
                m_show_registration_info = true;
                m_registration_error = sign_up_request->getInfo();
            }
            delete sign_up_request;
            m_signing_up = false;
            //FIXME m_options_widget->setActivated();
        }*/
    }
    // It's unsafe to delete from inside the event handler so we do it here
    if (m_self_destroy)
        ModalDialog::dismiss();
    else if (m_show_registration_info)
        showRegistrationInfo();
    else if (m_show_registration_terms)
        showRegistrationTerms();
    else if (m_show_registration_activation)
        showRegistrationActivation();
}
