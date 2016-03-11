/*
 *  OscManager.cpp
 *  AudioOnsetImagesApp
 *
 *  Created by Imanol Gomez on 11/03/16.
 *
 */


#include "OscManager.h"
#include "SettingsManager.h"
#include "AppManager.h"


OscManager::OscManager(): Manager()
{
    //Intentionally left empty
}

OscManager::~OscManager()
{
   ofLogNotice() << "OscManager::destructor";
}


//--------------------------------------------------------------

void OscManager::setup()
{
    if(m_initialized)
        return;

    Manager::setup();

    this->setupOscReceiver();
    this->setupOscSender();

    ofLogNotice() <<"OscManager::initialized" ;
}

void OscManager::setupOscReceiver()
{
    int portReceive = AppManager::getInstance().getSettingsManager().getOscPortReceive();
    ofLogNotice() <<"OscManager::setupOscReceiver -> listening for osc messages on port  " << portReceive;
    m_oscReceiver.setup(portReceive);


}

void OscManager::setupOscSender()
{
    int portSend = AppManager::getInstance().getSettingsManager().getOscPortSend();
    string host = AppManager::getInstance().getSettingsManager().getIpAddress();

    ofLogNotice() <<"OscManager::setupOscSender -> open osc connection " << host << ":" << portSend;

    m_oscSender.setup(host, portSend);

}


void OscManager::update()
{
    // check for waiting messages
    while(m_oscReceiver.hasWaitingMessages())
    {
        // get the next message
        ofxOscMessage m;
        m_oscReceiver.getNextMessage(m);

        m_latestOscMessage = m;

        if(m.getAddress() == "/FadeTime")
        {
            float value = m.getArgAsFloat(0);
            AppManager::getInstance().getGuiManager().setFadeTime(value);
        }

        else if(m.getAddress() == "/FadeTimeMin")
        {
            float value = m.getArgAsFloat(0);
            AppManager::getInstance().getGuiManager().setFadeTimeMin(value);
        }
        
        else if(m.getAddress() == "/FadeTimeMax")
        {
            float value = m.getArgAsFloat(0);
            AppManager::getInstance().getGuiManager().setFadeTimeMax(value);
        }
        
        else if(m.getAddress() == "/RandomImages")
        {
            bool value = m.getArgAsInt(0) > 0;
            AppManager::getInstance().getGuiManager().setRandomImages(value);
        }
        
        else if(m.getAddress() == "/RandomFadeTime")
        {
            bool value = m.getArgAsInt(0) > 0;
            AppManager::getInstance().getGuiManager().setRandomFadeTime(value);
        }
        
        else if(m.getAddress() == "/Stop")
        {
            bool value = m.getArgAsInt(0) > 0;
            AppManager::getInstance().getGuiManager().setStop(value);
        }
        
        else if(m.getAddress() == "/NoFade")
        {
            bool value = m.getArgAsInt(0) > 0;
            AppManager::getInstance().getGuiManager().setNoFade(value);
        }
        
        else if(m.getAddress() == "/Threshold")
        {
            float value = m.getArgAsFloat(0);
            AppManager::getInstance().getGuiManager().setThreshold(value);
        }
        
        else if(m.getAddress() == "/DecayTime")
        {
            float value = m.getArgAsFloat(0);
            AppManager::getInstance().getGuiManager().setDecayTime(value);
        }
        
        else if(m.getAddress() == "/SetNote")
        {
            ofVec2f value;
            value.x = m.getArgAsInt(0);
            value.y = m.getArgAsInt(1);
            AppManager::getInstance().getGuiManager().setNote(value);
        }
        
        else
        {
            string message = this->getMessageAsString(m);
            ofLogNotice() <<"OscManager::update -> received:  " << message;
        }

    }
}

void OscManager::sendMessage(ofxOscMessage& message)
{
    m_latestOscMessage = message;
    m_oscSender.sendMessage(message);
}


string OscManager::getMessageAsString(const ofxOscMessage& m) const
{
    string msg_string;
    msg_string = m.getAddress();
    for(int i = 0; i < m.getNumArgs(); i++){
        // get the argument type
        msg_string += " ";
        // display the argument - make sure we get the right type
        if(m.getArgType(i) == OFXOSC_TYPE_INT32){
            msg_string += ofToString(m.getArgAsInt32(i));
        }
        else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
            msg_string += ofToString(m.getArgAsFloat(i));
        }
        else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
            msg_string += m.getArgAsString(i);
        }
        else{
            msg_string += "unknown";
        }
    }

    return msg_string;
}


