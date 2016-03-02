/*
 *  AudioManager.cpp
 *  AudioOnsetImagesApp
 *
 *  Created by Imanol Gomez on 01/03/16.
 *
 */


#include "AppManager.h"
#include "AudioManager.h"


AudioManager::AudioManager(): m_volume(0.5), m_threshold(2.0), m_minimumThreshold(2.0), m_decayRate(0.1)
{
    //Intentionaly left empty
}


AudioManager::~AudioManager()
{
    ofLogNotice() <<"AudioManager::Destructor" ;
}


void AudioManager::setup()
{
    Manager::setup();
    
    ofLogNotice() <<"AudioManager::initialized" ;
    this->setupBeatTracker();
    this->setupText();
   
   
}

void AudioManager::audioReceived(float* input, int bufferSize) {
    m_beatTracker.audioReceived(input, bufferSize);
}

void AudioManager::setupBeatTracker()
{
   

}

void AudioManager::setupText()
{
    WindowSettingsManager::WindowSettingsVector windowSettings = WindowSettingsManager::getInstance().getWindowsSettings();

    
    ofVec3f position;
    
    position.x = GuiManager::GUI_WIDTH + 2.5*GuiManager::MARGIN;
    position.y = 1.5*GuiManager::MARGIN + windowSettings[0].height*0.5;
    
    int width = 700;
    int fontSize = 12;
    int height = fontSize*3;
    
    string text = "SOUND";
    m_text = ofPtr<TextVisual>(new TextVisual(position, width, height));
    m_text->setText(text, "fonts/open-sans/OpenSans-Semibold.ttf", fontSize);
    m_text->setColor(ofColor::white);
    
    AppManager::getInstance().getViewManager().addOverlay(m_text);
    
    position.x -= GuiManager::MARGIN*0.5;
    position.y -= GuiManager::MARGIN*0.5;
    height = m_text->getHeight() + GuiManager::MARGIN;
    width = m_text->getWidth() + GuiManager::MARGIN;
    m_rect = ofPtr<RectangleVisual>(new RectangleVisual(position, width, height));
    ofColor color(60,60,60);
    m_rect->setColor(color);
    
    AppManager::getInstance().getViewManager().addOverlay(m_rect,2);
}



void AudioManager::update()
{
     m_beatTracker.updateFFT();
}

void AudioManager::draw()
{
    this->drawFFT();
}

void AudioManager::drawFFT()
{
    ofPushStyle();
    ofEnableAlphaBlending();
    int xOffset = GuiManager::GUI_WIDTH + 2*GuiManager::MARGIN;
    int yOffset = 100 + GuiManager::MARGIN + m_rect->getPosition().y + m_rect->getHeight();
    for (int i = 1; i < FFT_BINS/2; i++){
        if(i % 16 == 0) {
            ofSetColor(200,0,0);
        } else {
            ofSetColor(250,250,250);
        }
        ofDrawLine(xOffset+(i*3),yOffset,  xOffset+(i*3),yOffset- m_beatTracker.magnitude[i]*10.0f);
        //printf("%f \n", magnitude_average[i]);
    }
    
    
    ofSetColor(255,255,255,70);
    
     m_threshold = ofLerp(m_threshold, m_minimumThreshold, m_decayRate);
    
    if(m_beatTracker.isBeatRange(0,2,m_threshold)){
        ofSetColor(255,255,255,150);
        AppManager::getInstance().getImageManager().nextImage();
        m_threshold = 10;
    }
    
    
    ofDrawRectangle(xOffset, yOffset,(FFT_BINS/2*3), - m_threshold*10);
    //ofDrawLine(xOffset,yOffset- m_threshold,  xOffset+(FFT_BINS/2*3),yOffset- m_threshold);
    ofDisableAlphaBlending();
    ofPopStyle();
}

void AudioManager::onChangeVolume(float& value)
{
    m_volume = value;
 //   m_fft.setVolume(m_volume);
}

void AudioManager::onChangeThreshold(float& value)
{
    m_minimumThreshold = value;
    //   m_fft.setVolume(m_volume);
}



float AudioManager::getAudioMax()
{
    return m_audioMax;
}

