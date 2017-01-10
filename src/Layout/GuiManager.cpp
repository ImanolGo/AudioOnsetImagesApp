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
const float GuiManager::GUI_WIDTH = 350;
const float GuiManager::MARGIN = 20;


GuiManager::GuiManager(): Manager(), m_showGui(true), m_currentPreset(-1), m_switchColor(0)
{
	//Intentionally left empty
}


GuiManager::~GuiManager()
{
    this->deleteTempPresets();
    ofLogNotice() <<"GuiManager::Destructor";
}


void GuiManager::setup()
{
	if(m_initialized)
		return;

    Manager::setup();


    this->setupGuiParameters();
    this->setupGuiPresets();
    this->setupImageGui();
    this->setupAudioGui();
    this->loadTempPreset();

    ofLogNotice() <<"GuiManager::initialized";

}

void GuiManager::setupGuiParameters()
{
    auto appManager = &AppManager::getInstance();
    
    ofColor backgroundColor = AppManager::getInstance().getSettingsManager().getColor("GUI_Background");
    ofColor fillColor = AppManager::getInstance().getSettingsManager().getColor("GUI_Fill");
    m_gui.setDefaultBackgroundColor(backgroundColor);
    m_gui.setDefaultFillColor(fillColor);
    m_colors.push_back(AppManager::getInstance().getSettingsManager().getColor("GUI1"));
    m_colors.push_back(AppManager::getInstance().getSettingsManager().getColor("GUI2"));
    
    m_gui.setDefaultHeaderBackgroundColor(m_colors[m_switchColor]);
    m_gui.setDefaultFillColor(m_colors[m_switchColor]);
    m_switchColor = 1 - m_switchColor;
    
    m_gui.setDefaultWidth(GUI_WIDTH);
    m_gui.setup(GUI_SETTINGS_NAME, GUI_SETTINGS_FILE_NAME);
    m_gui.setPosition(MARGIN, MARGIN);
    //m_gui.setPosition(20, 20);
    m_gui.add(m_guiFPS.set("FPS", 0, 0, 60));
    

    m_saveAllPresets.addListener(this, &GuiManager::saveAllPresets);
    m_gui.add(m_saveAllPresets.setup("Save All Presets"));
    
    m_saveCurrentPreset.addListener(this, &GuiManager::saveCurrentPreset);
    m_gui.add(m_saveCurrentPreset.setup("Save Current Preset"));
    
    m_stop.set("Stop", false);
    m_stop.addListener(appManager, &AppManager::onChangeStop);
    m_gui.add(m_stop);
    
    m_pause.set("Pause", false);
    m_pause.addListener(appManager, &AppManager::onChangePause);
    m_gui.add(m_pause);
    
    ofxGuiSetFont( "fonts/open-sans/OpenSans-Semibold.ttf", 11 );
}

void GuiManager::setupGuiPresets()
{
    ofColor backgroundColor = AppManager::getInstance().getSettingsManager().getColor("GUI_Background");
    ofColor fillColor = AppManager::getInstance().getSettingsManager().getColor("GUI_Fill");
    m_guiPresets.setDefaultBackgroundColor(backgroundColor);
    m_guiPresets.setDefaultFillColor(fillColor);
    m_guiPresets.setDefaultHeaderBackgroundColor(m_colors[m_switchColor]);
    m_guiPresets.setDefaultFillColor(m_colors[m_switchColor]);

    m_guiPresets.setDefaultWidth(GUI_WIDTH);
    m_guiPresets.setup("GuiPresets", "xmls/GuiPresets.xml");
    m_guiPresets.setPosition(ofGetWidth() - GUI_WIDTH*0.5 - MARGIN, MARGIN);
    //m_guiPresets.setPosition(200, 20);
    
    this->setupPresets();
}

void GuiManager::setupImageGui()
{
    m_gui.setDefaultHeaderBackgroundColor(m_colors[m_switchColor]);
    m_gui.setDefaultFillColor(m_colors[m_switchColor]);
    m_switchColor = 1 - m_switchColor;
    
    auto imageManager = &AppManager::getInstance().getImageManager();

    m_parametersImage.setName("Images");
    
    m_brightness.set("Brightness", 255, 0, 255);
    m_brightness.addListener(imageManager, &ImageManager::onChangeBrightness);
    m_parametersImage.add(m_brightness);
    
    m_crossFadeImgs.set("Crossfading", false);
    m_crossFadeImgs.addListener(imageManager, &ImageManager::onChangeCrossFade);
    m_parametersImage.add(m_crossFadeImgs);
    
    m_fadeTime.set("Fade Time", 2.0, 0.1, 30.0);
    m_fadeTime.addListener(imageManager, &ImageManager::onChangeFadeTime);
    m_parametersImage.add(m_fadeTime);
    
    m_fadeTimeMin.set("Fade Time Min", 0.1, 0.1, 30.0);
    m_fadeTimeMin.addListener(imageManager, &ImageManager::onChangeMinFadeTime);
    m_parametersImage.add(m_fadeTimeMin);
    
    m_fadeTimeMax.set("Fade Time Max", 5, 0.1, 30.0);
    m_fadeTimeMax.addListener(imageManager, &ImageManager::onChangeMaxFadeTime);
    m_parametersImage.add(m_fadeTimeMax);
    
    m_noFade.set("No fading", false);
    m_noFade.addListener(imageManager, &ImageManager::onChangeNoFade);
    m_parametersImage.add(m_noFade);
    
    m_randomFadeTime.set("Random Fade Time", false);
    m_randomFadeTime.addListener(imageManager, &ImageManager::onChangeRandomFade);
    m_parametersImage.add(m_randomFadeTime);
    
    m_randomImages.set("Random Images", false);
    m_randomImages.addListener(imageManager, &ImageManager::onChangeRandomImages);
    m_parametersImage.add(m_randomImages);

    m_gui.add(m_parametersImage);
    
    ofxButton * button = new ofxButton();
    button->setup("Next Image");
    button->addListener(imageManager, &ImageManager::nextImage);
    m_gui.add(button);
    
    button = new ofxButton();
    button->setup("Previous Image");
    button->addListener(imageManager, &ImageManager::previousImage);
    m_gui.add(button);

}

void GuiManager::setupAudioGui()
{
    m_gui.setDefaultHeaderBackgroundColor(m_colors[m_switchColor]);
    m_gui.setDefaultFillColor(m_colors[m_switchColor]);
    m_switchColor = 1 - m_switchColor;
    
    auto audioManager = &AppManager::getInstance().getAudioManager();

    m_parametersAudio.setName("Audio");
    
    
    m_lowFreqCut.set("Low Freq Cut", 0, 0, 511);
    m_lowFreqCut.addListener(audioManager, &AudioManager::onChangeLowFreqCut);
    m_parametersAudio.add(m_lowFreqCut);
    
    m_highFreqCut.set("High Freq Cut", 511, 0, 511);
    m_highFreqCut.addListener(audioManager, &AudioManager::onChangeHighFreqCut);
    m_parametersAudio.add(m_highFreqCut);
    
    m_threshold.set("Threshold", 0.1, 0.0, 1.0);
    m_threshold.addListener(audioManager, &AudioManager::onChangeThreshold);
    m_parametersAudio.add(m_threshold);
    
    m_decayTime.set("Decay Time", 0.5, 0.01, 5.0);
    m_decayTime.addListener(audioManager, &AudioManager::onChangeDecayTime);
    m_parametersAudio.add(m_decayTime);
    
    auto midiManager = &AppManager::getInstance().getMidiManager();
    
    m_midiChannel.set("Midi Channel", 1, 0, 20);
    m_midiChannel.addListener(midiManager, &MidiManager::onChangeChannel);
    m_parametersAudio.add(m_midiChannel);
    
    
    m_notes_params.push_back(ofParameter<bool>("C-1",false));
    m_notes_params.push_back(ofParameter<bool>("G-1",false));
    m_notes_params.push_back(ofParameter<bool>("C0",false));
    m_notes_params.push_back(ofParameter<bool>("G0",false));
    m_notes_params.push_back(ofParameter<bool>("C1",false));
    m_notes_params.push_back(ofParameter<bool>("G1",false));
    m_notes_params.push_back(ofParameter<bool>("C2",false));
    m_notes_params.push_back(ofParameter<bool>("G2",false));
    m_notes_params.push_back(ofParameter<bool>("C3",false));
    m_notes_params.push_back(ofParameter<bool>("G3",false));
    m_notes_params.push_back(ofParameter<bool>("C4",false));
    m_notes_params.push_back(ofParameter<bool>("G4",false));
    m_matrixNotes.setup("Notes",3);
    for(unsigned int i = 0; i < m_notes_params.size(); i++) {
        m_notes_params.at(i).addListener(this, &GuiManager::onNoteChange);
        m_matrixNotes.add(new ofxMinimalToggle(m_notes_params.at(i)));
    }
    //m_matrixNotes.setBorderColor(ofColor::aquamarine);
    m_matrixNotes.setElementHeight(26);
    //m_matrixNotes.allowMultipleActiveToggles(false);
    

    m_gui.add(m_parametersAudio);
    m_gui.add(&m_matrixNotes);
}

void GuiManager::setupPresets()
{
    this->deleteTempPresets();
    
    auto foldersVector = AppManager::getInstance().getImageManager().getFoldersNames();
    
    for(auto folder: foldersVector)
    {
        m_presetParameters.push_back(ofParameter<bool>(folder,false));
    }
    
    m_matrixPresets.setup("Presets",1);
    for(unsigned int i = 0; i < m_presetParameters.size(); i++) {
        m_presetParameters.at(i).addListener(this, &GuiManager::onPresetChange);
        m_matrixPresets.add(new ofxMinimalToggle(m_presetParameters.at(i)));
    }
    //m_matrixNotes.setBorderColor(ofColor::aquamarine);
   // m_matrixNotes.setElementHeight(26);
    m_matrixPresets.allowMultipleActiveToggles(false);
    
    m_guiPresets.add(&m_matrixPresets);
}

void GuiManager::update()
{
    this->updatePresets();
}

void GuiManager::updatePresets()
{
    if(m_currentPreset != m_matrixPresets.getActiveToggleIndex()){
        this->saveTempPreset();
        m_currentPreset = m_matrixPresets.getActiveToggleIndex();
        ofLogNotice() <<"GuiManager::updatePresetGroup -> Current Preset: " << m_currentPreset;
        this->loadTempPreset();
        AppManager::getInstance().getImageManager().setImageGroup(m_currentPreset);
        m_matrixPresets.setActiveToggle(m_currentPreset);
        
    }
}

void GuiManager::draw()
{
    if(!m_showGui)
        return;

    this->drawRectangle();

    m_guiFPS = ofGetFrameRate();
    m_gui.draw();
    m_guiPresets.draw();
}


void GuiManager::saveTempPreset()
{
    //m_gui.saveToFile(GUI_SETTINGS_FILE_NAME);
    
    auto foldersVector = AppManager::getInstance().getImageManager().getFoldersNames();

    if(m_currentPreset>=0){
        string presetFileName = "xmls/GuiSettings_" + foldersVector[m_currentPreset] + "_temp.xml";
        m_gui.saveToFile(presetFileName);
    }
    
}

void GuiManager::loadTempPreset()
{
    //m_gui.loadFromFile(GUI_SETTINGS_FILE_NAME);
    
    auto foldersVector = AppManager::getInstance().getImageManager().getFoldersNames();
    
    if(m_currentPreset>=0 && foldersVector.size()>m_currentPreset){
        //Try permanent preset
        string presetFileName = "xmls/GuiSettings_" + foldersVector[m_currentPreset] + ".xml";
        m_gui.loadFromFile(presetFileName);
        
        //Try temporal preset
        string temporalPresetFileName  = "xmls/GuiSettings_" + foldersVector[m_currentPreset] + "_temp.xml";
        m_gui.loadFromFile(temporalPresetFileName);
    }
}


void GuiManager::deleteTempPresets()
{
    auto foldersVector = AppManager::getInstance().getImageManager().getFoldersNames();
    
    for(auto folder: foldersVector){
        string temporalPresetFileName  = "xmls/GuiSettings_" + folder + "_temp.xml";
        ofFile file(ofToDataPath(temporalPresetFileName));
        file.remove();
    }
    
}


void GuiManager::saveAllPresets()
{
    this->saveTempPreset();

    auto foldersVector = AppManager::getInstance().getImageManager().getFoldersNames();
    
    for(auto folder: foldersVector){
        string temporalPresetFileName  = "xmls/GuiSettings_" + folder + "_temp.xml";
        ofFile file(ofToDataPath(temporalPresetFileName));
        string permanentPresetFileName  = "xmls/GuiSettings_" + folder + ".xml";
        file.moveTo(permanentPresetFileName, true, true);
    }
    
}

void GuiManager::saveCurrentPreset()
{
    this->saveTempPreset();
    
    auto foldersVector = AppManager::getInstance().getImageManager().getFoldersNames();
    
    if(m_currentPreset>=0){
        
        string temporalPresetFileName = "xmls/GuiSettings_" + foldersVector[m_currentPreset] + "_temp.xml";
        
        ofFile file(ofToDataPath(temporalPresetFileName));
       
        
        string permanentPresetFileName = "xmls/GuiSettings_" + foldersVector[m_currentPreset] + ".xml";
        
        file.moveTo(permanentPresetFileName, true, true);
    }
    
}



void GuiManager::toggleGui()
{
    m_showGui = !m_showGui;
}

int  GuiManager::getCurrentNoteIndex() 
{
    return m_matrixNotes.getActiveToggleIndex();
}

vector<int>  GuiManager::getNoteIndexes()
{
    vector<int> noteIndexes;
    
    for(unsigned int i = 0; i < m_notes_params.size(); i++)
    {
        if( m_notes_params.at(i).get())
        {
            noteIndexes.push_back(i);
        }
    }
            
    return noteIndexes;
}


void GuiManager::drawRectangle()
{
    ofPushStyle();
    ofSetColor(ofColor::black);
    ofDrawRectangle( m_gui.getPosition().x - 20, 0, GUI_WIDTH + 60, ofGetHeight());
    ofPopStyle();
}

void GuiManager::onNoteChange(bool& value)
{
    for(unsigned int i = 0; i < m_notes_params.size(); i++) {
        if( value && (m_notes_params.at(i).get() == true)){
            int index = m_matrixNotes.getActiveToggleIndex();
            ofLogNotice() <<"GuiManager::onNoteChange -> Note: " << i << ", index = " << index;
            
            //ofLogNotice() <<"GuiManager::onNoteChange -> Note: " << i << ", value = " << value;
        }
    }
}

void GuiManager::onPresetChange(bool& value)
{
    for(unsigned int i = 0; i < m_presetParameters.size(); i++) {
        if( value && (m_presetParameters.at(i).get() == true)){
            int index = m_matrixPresets.getActiveToggleIndex();
            ofLogNotice() <<"GuiManager::onPresetChange -> Preset: " << i << ", index = " << index;
            //AppManager::getInstance().getImageManager().setImageGroup(i);
            
            //ofLogNotice() <<"GuiManager::onNoteChange -> Note: " << i << ", value = " << value;
        }
    }
}

void GuiManager::setNote(const ofVec2f& value)
{
    int index = (int) value.x;
    
    if(index < 0 || index >= m_notes_params.size()){
        return;
    }
    
    bool isOn = value.y>0;
    m_notes_params.at(index) = isOn;
}


 void GuiManager::setPreset(int value)
{
    if(value < 0 || value >= m_presetParameters.size()){
        return;
    }
    
    m_matrixPresets.setActiveToggle(value);

}


