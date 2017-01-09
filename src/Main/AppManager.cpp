/*
 *  AppManager.cpp
 *  Led Vase
 *
 *  Created by Imanol Gomez on 05/10/15.
 *
 */

#include "ofMain.h"


#include "AppManager.h"

AppManager& AppManager::getInstance()
{
	// The only instance
	// Guaranteed to be lazy initialized
	// Guaranteed that it will be destroyed correctly
	static AppManager    m_instance;
    return m_instance;

}

AppManager::AppManager(): Manager(), m_debugMode(false)
{
   //Intentionally left empty
}


AppManager::~AppManager()
{
    ofLogNotice() <<"AppManager::Destructor";
}


void AppManager::setup()
{
	if(m_initialized)
		return;

    //ofSetDataPathRoot("../Resources/data/");
    
    ofLogNotice() << "AppManager::initialized";

	Manager::setup();
    
    this->setupOF();
	this->setupManagers();
    
    //setDebugMode(m_debugMode);
}

void AppManager::setupOF()
{
    ofSetLogLevel(OF_LOG_NOTICE);
    //ofSetFrameRate(24);
    ofSetVerticalSync(false);
    ofSetEscapeQuitsApp(false);
    
}


void AppManager::setupManagers()
{
    m_settingsManager.setup();
    m_resourceManager.setup();
    m_keyboardManager.setup();
    m_viewManager.setup();
    m_visualEffectsManager.setup();
    m_oscManager.setup();
    m_imageManager.setup();
    m_audioManager.setup();
    m_previewManager.setup();
    m_midiManager.setup();
    m_guiManager.setup();
   
    
    
}

void AppManager::update()
{
    m_oscManager.update();
    m_visualEffectsManager.update();
    m_guiManager.update();
    m_audioManager.update();
    m_viewManager.update();
    m_imageManager.update();
}


void AppManager::draw()
{
    ofClear(0);
    
    m_viewManager.draw();
    m_guiManager.draw();
    m_previewManager.draw();
    m_audioManager.draw();
}

void AppManager::toggleDebugMode()
{
    m_debugMode = !m_debugMode;
    setDebugMode(m_debugMode);
}


void AppManager::setDebugMode(bool showDebug)
{
    m_debugMode = showDebug;
    
    ofLogNotice()<<"AppManager::setDebugMode-> " << m_debugMode;
    
    if(m_debugMode){
        //ofSetLogLevel(OF_LOG_VERBOSE);
    }
    else{
        ofSetLogLevel(OF_LOG_NOTICE);
    }
    
    
}



