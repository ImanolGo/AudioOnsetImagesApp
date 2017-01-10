/*
 *  AudioManager.cpp
 *  AudioOnsetImagesApp
 *
 *  Created by Imanol Gomez on 01/03/16.
 *
 */


#include "AppManager.h"
#include "AudioManager.h"


AudioManager::AudioManager(): m_volume(0.5), m_threshold(2.0), m_minimumThreshold(0.0), m_decayRate(0.1), m_decayTime(0.5), m_maxThreshold(1.0), m_lowFreqCut(0), m_highFreqCut(511), m_stopped(false)
{
    //Intentionaly left empty
}


AudioManager::~AudioManager()
{
    ofLogNotice() <<"AudioManager::Destructor" ;
}


void AudioManager::setup()
{

    if (m_initialized) {
        return;
    }
    
    ofLogNotice() <<"AudioManager::initialized" ;
    this->setupFft();
    this->setupText();
    this->setupMidiNotes();
   
    Manager::setup();
}


void AudioManager::setupFft()
{
   
    m_animationVisual =  ofPtr<BasicVisual>(new BasicVisual());
    m_animationVisual->setValue(m_minimumThreshold);
    
    m_fftLive.setMirrorData(false);
    m_fftLive.setup();
}

void AudioManager::setupMidiNotes()
{
    m_notes.clear();
    int offsetNote = 12;
    int numberOfNotes = 12;
    for (int i = 0; i<numberOfNotes; i++) {
        int note = offsetNote + (i/2*5 + (i+1)/2*7);
         ofLogNotice() <<"AudioManager::setupMidiNotes -> Index: " << i << ". note: " << note ;
        m_notes.push_back(note);
    }
}

void AudioManager::setupText()
{
    WindowSettingsManager::WindowSettingsVector windowSettings = WindowSettingsManager::getInstance().getWindowsSettings();

    
    ofVec3f position;
    
    position.x = GuiManager::GUI_WIDTH + 4*GuiManager::MARGIN;
    float yOffset = AppManager::getInstance().getPreviewManager().getPreviewRect()->getPosition().y + AppManager::getInstance().getPreviewManager().getPreviewRect()->getHeight();
    position.y = 1.5*GuiManager::MARGIN + yOffset;
    
    
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
    if(m_stopped){
        return;
    }
    
    //ofLogNotice() <<"AudioManager::update" ;
    m_fftLive.update();
    this->updateOnsetDetector();
    
    //ofLogNotice() <<"AudioManager::peak -> " << m_fftLive.getAveragePeak() ;

}

void AudioManager::updateOnsetDetector()
{
    //m_threshold = ofLerp(m_threshold, m_minimumThreshold, m_decayRate);
    m_threshold = m_animationVisual->getValue();
    m_fftLive.setThreshold(m_threshold);
    
    // if(m_fftLive.getAveragePeak() > m_threshold)
    
    float peak = this->getFilteredAveragePeak();
    if(peak > m_threshold)
     {
        //ofLogNotice() <<"AudioManager::setupMidiNotes -> AvgPeak " << peak << ", threshold = " << m_threshold;
         
        AppManager::getInstance().getImageManager().nextImage();
        m_threshold = m_maxThreshold;
        this->setAnimations();
        this->sendAllNotesOff();
        this->sendMidiNotesOn();
    }
}

void AudioManager::draw()
{
    this->drawFFT();
    this->drawFFTFilter();
}

void AudioManager::drawFFT()
{
    
    float xOffset = GuiManager::GUI_WIDTH + 3.5*GuiManager::MARGIN;
    float yOffset = GuiManager::MARGIN + m_rect->getPosition().y + m_rect->getHeight();
    
    
    m_fftLive.draw(xOffset, yOffset, OFX_FFT_WIDTH, OFX_FFT_HEIGHT);

}

void AudioManager::drawFFTFilter()
{
    
    float xOffset = GuiManager::GUI_WIDTH + 3.5*GuiManager::MARGIN;
    float yOffset = GuiManager::MARGIN + m_rect->getPosition().y + m_rect->getHeight();
    
    
    float x  = xOffset + m_lowFreqCut + 1;
    float w  = m_highFreqCut - m_lowFreqCut;
    float y = yOffset+1;
    float h = OFX_FFT_HEIGHT;
    
    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetColor(200,0,0,50);
    ofDrawRectangle(x, y, w, h);
    ofEnableAlphaBlending();
    ofPopStyle();
    
}

void AudioManager::setAnimations()
{
    AppManager::getInstance().getVisualEffectsManager().removeAllVisualEffects(m_animationVisual);
    AppManager::getInstance().getVisualEffectsManager().createValueEffect(m_animationVisual, m_maxThreshold, m_minimumThreshold, 0.0, m_decayTime);
}

void AudioManager::sendAllNotesOff()
{
    int velocity = 0;
    for (auto note : m_notes) {
        AppManager::getInstance().getMidiManager().sendNote(note, velocity);
    }
}

void AudioManager::sendMidiNotesOn()
{
    int velocity = 127;
    //int index =  AppManager::getInstance().getGuiManager().getCurrentNoteIndex();
    //int note = m_notes[index];
    //AppManager::getInstance().getMidiManager().sendNote(note, velocity);
    
    vector<int> indexes =  AppManager::getInstance().getGuiManager().getNoteIndexes();
    for(auto index: indexes)
    {
        int note = m_notes[index];
        AppManager::getInstance().getMidiManager().sendNote(note, velocity);
    }
}


void AudioManager::onChangeVolume(float& value)
{
    m_volume = value;
 //   m_fft.setVolume(m_volume);
}

void AudioManager::onChangeThreshold(float& value)
{
    m_minimumThreshold = value;
    m_animationVisual->setValue(m_minimumThreshold);
    //   m_fft.setVolume(m_volume);
}

void AudioManager::onChangeDecayTime(float& value)
{
    //m_decayRate = value;
    m_decayTime = value;
}

void AudioManager::onChangeLowFreqCut(int& value)
{
    if(value<m_highFreqCut && value < m_fftLive.getBufferSize()){
        m_lowFreqCut = value;
    }
}

void AudioManager::onChangeHighFreqCut(int& value){
    if(value>m_lowFreqCut && value < m_fftLive.getBufferSize()){
        m_highFreqCut = value;
    }
}


float AudioManager::getAudioMax()
{
    return m_audioMax;
}


void AudioManager::audioIn(float * input, int bufferSize, int nChannels)
{
    m_fftLive.audioIn(input, bufferSize, nChannels);
}

float AudioManager::getFilteredAveragePeak()
{
    
    float avrPeak = 0;
    //vector<float> peakData = m_fftLive.getFftPeakData();
    vector<float> peakData = m_fftLive.getFftRawData();
    
    
    
    for(int i=m_lowFreqCut; i<= m_highFreqCut; i++)
    {
        if(peakData[i]>0 && peakData[i]<=1){
            avrPeak+=peakData[i];
        }
        
    }
    
    
    int numPeaks = m_highFreqCut - m_lowFreqCut;
    
   // ofLogNotice() <<"AudioManager::setupMidiNotes -> SumPeaks " <<avrPeak << ", w = " << m_fftLive.getBufferSize();
    
    //avrPeak/=numPeaks;
    //avrPeak = ofClamp(avrPeak, 0.0, 1.0);
    
    avrPeak = ofMap(avrPeak, 0.0, numPeaks, 0.01, 1.0, true);
    
    return avrPeak;
    
}

void AudioManager::stop(bool value)
{
    m_stopped = value;
}

