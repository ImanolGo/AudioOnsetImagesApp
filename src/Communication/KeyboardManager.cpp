/*
 *  KeyboardManager.cpp
 *
 *  Created by Imanol Gomez on 23/02/15.
 *
 */


#include "KeyboardManager.h"
#include "AppManager.h"


KeyboardManager::KeyboardManager(): Manager()
{
    //Intentionally left empty
}

KeyboardManager::~KeyboardManager()
{
   ofLogNotice() << "KeyboardManager::destructor";
}


//--------------------------------------------------------------

void KeyboardManager::setup()
{
    if(m_initialized)
        return;
    
    Manager::setup();
    
    ofLogNotice() <<"KeyboardManager::initialized" ;
    this->addListeners();
}

void KeyboardManager::addListeners()
{
    ofRegisterKeyEvents(this); // this will enable the InteractiveVisual class to listen to the key events.
}

void KeyboardManager::keyPressed(ofKeyEventArgs &e)
{
    int key = e.key;
    
    ofLogNotice() <<"KeyboardManager::keyPressed-> " + ofToString(key);
    
   
    
    if(key == ' ') {
        AppManager::getInstance().toggleDebugMode();
    }
    
    if(key == 'm') {
        int note  = ofRandom(48, 122);
        AppManager::getInstance().getMidiManager().sendNote(note, 127);
    }
    
    
    if(key == OF_KEY_RIGHT) {
        AppManager::getInstance().getImageManager().nextImage();
    }


}

void KeyboardManager::keyReleased(ofKeyEventArgs &e)
{
     //Intentionally left empty
}










