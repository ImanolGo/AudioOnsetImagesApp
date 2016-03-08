/*
 *  GuiManager.cpp
 *  AudioOnsetImagesApp
 *
 *  Created by Imanol Gomez on 01/03/16.
 *
 */


#include "ofMain.h"

#include "AppManager.h"

#include "GuiManager.h"


const string GuiManager::GUI_SETTINGS_FILE_NAME = "xmls/GuiSettings.xml";
const string GuiManager::GUI_SETTINGS_NAME = "GUI";
const int GuiManager::GUI_WIDTH = 350;
const int GuiManager::MARGIN = 40;


GuiManager::GuiManager(): Manager(), m_showGui(true)
{
	//Intentionally left empty
}


GuiManager::~GuiManager()
{
    this->saveGuiValues();
    ofLogNotice() <<"GuiManager::Destructor";
}


void GuiManager::setup()
{
	if(m_initialized)
		return;

    Manager::setup();


    this->setupGuiParameters();
    this->setupImageGui();
    this->setupAudioGui();
    this->loadGuiValues();

    ofLogNotice() <<"GuiManager::initialized";

}

void GuiManager::setupGuiParameters()
{
    m_gui.setDefaultWidth(GUI_WIDTH);
    m_gui.setup(GUI_SETTINGS_NAME, GUI_SETTINGS_FILE_NAME);
    m_gui.setPosition(MARGIN, MARGIN);
    //m_gui.setPosition(20, 20);
    m_gui.add(m_guiFPS.set("FPS", 0, 0, 60));
    ofxGuiSetFont( "fonts/open-sans/OpenSans-Semibold.ttf", 9 );

}



void GuiManager::setupImageGui()
{
    auto imageManager = &AppManager::getInstance().getImageManager();

    m_parametersImage.setName("Images");
    
    m_fadeTime.set("Fade Time", 2.0, 0.1, 10.0);
    m_fadeTime.addListener(imageManager, &ImageManager::onChangeFadeTime);
    m_parametersImage.add(m_fadeTime);
    
    m_fadeTimeMin.set("Fade Time Min", 0.1, 0.1, 5.0);
    m_fadeTimeMin.addListener(imageManager, &ImageManager::onChangeMinFadeTime);
    m_parametersImage.add(m_fadeTimeMin);
    
    m_fadeTimeMax.set("Fade Time Max", 5, 0.1, 10.0);
    m_fadeTimeMax.addListener(imageManager, &ImageManager::onChangeMaxFadeTime);
    m_parametersImage.add(m_fadeTimeMax);
    
    m_randomFadeTime.set("Random Fade Time", false);
    m_randomFadeTime.addListener(imageManager, &ImageManager::onChangeRandomFade);
    m_parametersImage.add(m_randomFadeTime);
    
    m_randomImages.set("Random Images", false);
    m_randomImages.addListener(imageManager, &ImageManager::onChangeRandomImages);
    m_parametersImage.add(m_randomImages);

    
    m_gui.add(m_parametersImage);
}

void GuiManager::setupAudioGui()
{
    auto audioManager = &AppManager::getInstance().getAudioManager();

    m_parametersAudio.setName("Audio");
    
    m_threshold.set("Threshold", 2.0, 0.5, 5.0);
    m_threshold.addListener(audioManager, &AudioManager::onChangeThreshold);
    m_parametersAudio.add(m_threshold);
    
    m_decayTime.set("Decay Time", 0.5, 0.01, 5.0);
    m_decayTime.addListener(audioManager, &AudioManager::onChangeDecayTime);
    m_parametersAudio.add(m_decayTime);


    m_gui.add(m_parametersAudio);
}


void GuiManager::draw()
{
    if(!m_showGui)
        return;

    this->drawRectangle();

    m_guiFPS = ofGetFrameRate();
    m_gui.draw();

}


void GuiManager::saveGuiValues()
{
    m_gui.saveToFile(GUI_SETTINGS_FILE_NAME);
}

void GuiManager::loadGuiValues()
{
    m_gui.loadFromFile(GUI_SETTINGS_FILE_NAME);
}


void GuiManager::toggleGui()
{
    m_showGui = !m_showGui;
}

void GuiManager::drawRectangle()
{
    ofPushStyle();
    ofSetColor(ofColor::black);
    ofDrawRectangle( m_gui.getPosition().x - 20, 0, GUI_WIDTH + 60, ofGetHeight());
    ofPopStyle();
}


