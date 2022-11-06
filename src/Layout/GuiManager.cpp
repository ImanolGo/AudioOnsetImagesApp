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
#include "GuiTheme.h"


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
    this->loadTempPreset(m_currentPreset);

    ofLogNotice() <<"GuiManager::initialized";

}

void GuiManager::setupGuiParameters()
{
    //create an actual ImGui context before setting up the addon
    ImGui::CreateContext();
    
    //specify a font to use
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF(&ofToDataPath("fonts/roboto/Roboto-Regular.ttf")[0], 16.f);
    
    //ImGui::GetIO().FontGlobalScale = 1.0 / 2.0;
    
    //finally setup the addon ofxImGui::Gui setup;
    m_gui.setup(new GuiTheme());
    ofxImGui::Settings().windowPos  = ofVec2f(0,0);
    ofxImGui::Settings().windowSize = ofVec2f(GUI_WIDTH,ofGetHeight());
    
    m_width = 0;
    m_height = 0;

    
    auto appManager = &AppManager::getInstance();
    
    m_stop.set("Stop", false);
    m_stop.addListener(appManager, &AppManager::onChangeStop);
    m_parameters.add(m_stop);
    
    m_pause.set("Pause", false);
    m_pause.addListener(appManager, &AppManager::onChangePause);
    m_parameters.add(m_pause);
    
    m_blackout.set("Blackout", false);
    m_blackout.addListener(appManager, &AppManager::onChangeBlackout);
    m_parameters.add(m_blackout);
    

}

void GuiManager::setupGuiPresets()
{
    this->setupPresets();
}

void GuiManager::setupImageGui()
{
    
    auto imageManager = &AppManager::getInstance().getImageManager();

    m_parametersImage.setName("Images");
    
    m_brightness.set("Brightness", 255, 0, 255);
    m_brightness.addListener(imageManager, &ImageManager::onChangeBrightness);
    m_parametersImage.add(m_brightness);
    m_parameters.add(m_brightness);
    
    m_crossFadeImgs.set("Crossfading", false);
    m_crossFadeImgs.addListener(imageManager, &ImageManager::onChangeCrossFade);
    m_parametersImage.add(m_crossFadeImgs);
    m_parameters.add(m_crossFadeImgs);
    
    m_fadeTime.set("Fade Time", 2.0, 0.1, 30.0);
    m_fadeTime.addListener(imageManager, &ImageManager::onChangeFadeTime);
    m_parametersImage.add(m_fadeTime);
    m_parameters.add(m_fadeTime);
    
    m_fadeTimeMin.set("Fade Time Min", 0.1, 0.1, 30.0);
    m_fadeTimeMin.addListener(imageManager, &ImageManager::onChangeMinFadeTime);
    m_parametersImage.add(m_fadeTimeMin);
    m_parameters.add(m_fadeTimeMin);
    
    m_fadeTimeMax.set("Fade Time Max", 5, 0.1, 30.0);
    m_fadeTimeMax.addListener(imageManager, &ImageManager::onChangeMaxFadeTime);
    m_parametersImage.add(m_fadeTimeMax);
    m_parameters.add(m_fadeTimeMax);
    
    m_noFade.set("No fading", false);
    m_noFade.addListener(imageManager, &ImageManager::onChangeNoFade);
    m_parametersImage.add(m_noFade);
    m_parameters.add(m_noFade);
    
    m_randomFadeTime.set("Random Fade Time", false);
    m_randomFadeTime.addListener(imageManager, &ImageManager::onChangeRandomFade);
    m_parametersImage.add(m_randomFadeTime);
    m_parameters.add(m_randomFadeTime);
    
    m_randomImages.set("Random Images", false);
    m_randomImages.addListener(imageManager, &ImageManager::onChangeRandomImages);
    m_parametersImage.add(m_randomImages);
    m_parameters.add(m_randomImages);

}

void GuiManager::setupAudioGui()
{
    
    auto audioManager = &AppManager::getInstance().getAudioManager();

    m_parametersAudio.setName("Audio");
    
    
    m_lowFreqCut.set("Low Freq Cut", 0, 0, 511);
    m_lowFreqCut.addListener(audioManager, &AudioManager::onChangeLowFreqCut);
    m_parametersAudio.add(m_lowFreqCut);
    m_parameters.add(m_lowFreqCut);
    
    m_highFreqCut.set("High Freq Cut", 511, 0, 511);
    m_highFreqCut.addListener(audioManager, &AudioManager::onChangeHighFreqCut);
    m_parametersAudio.add(m_highFreqCut);
    m_parameters.add(m_highFreqCut);
    
    m_threshold.set("Threshold", 0.1, 0.0, 1.0);
    m_threshold.addListener(audioManager, &AudioManager::onChangeThreshold);
    m_parametersAudio.add(m_threshold);
    m_parameters.add(m_threshold);
    
    m_decayTime.set("Decay Time", 0.5, 0.01, 30.0);
    m_decayTime.addListener(audioManager, &AudioManager::onChangeDecayTime);
    m_parametersAudio.add(m_decayTime);
    m_parameters.add(m_decayTime);
    
    auto midiManager = &AppManager::getInstance().getMidiManager();
    
    m_midiChannel.set("Midi Channel", 1, 0, 20);
    m_midiChannel.addListener(midiManager, &MidiManager::onChangeChannel);
    m_parametersAudio.add(m_midiChannel);
    m_parameters.add(m_midiChannel);
    
    m_matrixNotes.setName("Notes");
    std::vector<std::string> notes = {"C-1","G-1","C0","G0","C1","G1","C2","G2","C3","G3","C4","G4"};
    for(auto note: notes)
    {
        m_notes_params.push_back(ofParameter<bool>(note,false));
        m_matrixNotes.add(m_notes_params.back());
        m_parameters.add(m_notes_params.back());
    }
}

void GuiManager::setupPresets()
{
    this->deleteTempPresets();
    
    m_presetNames = AppManager::getInstance().getImageManager().getFoldersNames();
    
    m_matrixPresets.setName("Presets");
    
    m_lastPreset = 0;
    m_currentPreset.set("Preset", m_lastPreset);
    m_currentPreset.addListener(this, &GuiManager::onPresetChange);
    m_matrixPresets.add(m_currentPreset);

}

void GuiManager::update()
{
    //Left empty
}

void GuiManager::draw()
{
    if(!m_showGui)
        return;

    this->drawRectangle();
    this->drawGui();

}

void GuiManager::drawGui()
{
    ofEnableAlphaBlending();
    
    m_gui.begin();
        
    auto mainSettings  = ofxImGui::Settings();

    
    if (ofxImGui::BeginWindow(GUI_SETTINGS_NAME, mainSettings, false))
    {
        ImGui::Text("%.1f FPS (%.3f ms/frame)", ofGetFrameRate(), 1000.0f / ImGui::GetIO().Framerate);
                
        if (ImGui::Button("Save All Presets"))
        {
            this->saveAllPresets();
        }
        
        if (ImGui::Button("Save Current Preset"))
        {
            this->saveCurrentPreset();
        }
                          
        if (ofxImGui::BeginTree(m_parameters, mainSettings))
         {
             ofxImGui::AddParameter(m_stop);
             ofxImGui::AddParameter(m_pause);
             ofxImGui::AddParameter(m_blackout);
             ofxImGui::EndTree(mainSettings);
        }
        
        if (ofxImGui::BeginTree(m_parametersImage, mainSettings))
         {
             ofxImGui::AddParameter(m_brightness);
             ofxImGui::AddParameter(m_crossFadeImgs);
             ofxImGui::AddParameter(m_fadeTime);
             ofxImGui::AddParameter(m_fadeTimeMin);
             ofxImGui::AddParameter(m_fadeTimeMax);
             ofxImGui::AddParameter(m_noFade);
             ofxImGui::AddParameter(m_randomFadeTime);
             ofxImGui::AddParameter(m_randomImages);
             
             
             if (ImGui::Button("Next Image"))
             {
                 AppManager::getInstance().getImageManager().nextImage();
             }
             
             if (ImGui::Button("Previous Image"))
             {
                 AppManager::getInstance().getImageManager().previousImage();
             }
             
             ofxImGui::EndTree(mainSettings);

        }
        
        if (ofxImGui::BeginTree(m_parametersAudio, mainSettings))
         {
             ofxImGui::AddParameter(m_lowFreqCut);
             ofxImGui::AddParameter(m_highFreqCut);
             ofxImGui::AddParameter(m_threshold);
             ofxImGui::AddParameter(m_decayTime);
             ofxImGui::AddParameter(m_midiChannel);
             ofxImGui::EndTree(mainSettings);
        }
        
        if (ofxImGui::BeginTree(m_matrixNotes, mainSettings))
         {
             for(unsigned int i = 0; i < m_notes_params.size(); i++) {
                 ofxImGui::AddParameter(m_notes_params.at(i));
        
             }
             
             ofxImGui::EndTree(mainSettings);
        }
        
        if (ofxImGui::BeginTree(m_matrixPresets, mainSettings))
         {
             ofxImGui::AddCombo(m_currentPreset, m_presetNames);
             
             ofxImGui::EndTree(mainSettings);
        }
        
        
                
    }
    
    
    ofxImGui::EndWindow(mainSettings);
    m_gui.end();
    
    //ofDisableAlphaBlending();
        
    this->updateSize(mainSettings);
   
}

void GuiManager::updateSize(const ofxImGui::Settings& settings)
{
    if(m_width!= settings.windowSize.x){
         m_width = settings.windowSize.x;
    }
   
    m_height = settings.windowSize.y;
    m_position = ofPoint(settings.windowPos.x, settings.windowPos.y);
}
void GuiManager::saveTempPreset(int index)
{
    
    auto foldersVector = AppManager::getInstance().getImageManager().getFoldersNames();
    
    if(index>=0 && index < foldersVector.size()){
        
        ofXml xml;
        ofSerialize(xml, m_parameters);
        
        string presetFileName = "xmls/GuiSettings_" + foldersVector[index] + "_temp.xml";
        xml.save(presetFileName);
    }
    
}

void GuiManager::loadTempPreset(int index)
{
    
    auto foldersVector = AppManager::getInstance().getImageManager().getFoldersNames();
    
    if(index>=0 && index < foldersVector.size()){
        //Try permanent preset
        string presetFileName = "xmls/GuiSettings_" + foldersVector[m_currentPreset] + ".xml";
        
        ofXml xml;
        xml.load(presetFileName);
        ofDeserialize(xml, m_parameters);
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
    this->saveTempPreset(m_currentPreset);

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
    this->saveTempPreset(m_currentPreset);
    
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
    ofSetColor(15);
    ofDrawRectangle( 0, 0, this->getWidth(), ofGetHeight());
    ofPopStyle();
}

void GuiManager::onPresetChange(int& value)
{
    this->saveTempPreset(m_lastPreset);
    ofLogNotice() <<"GuiManager::updatePresetGroup -> Current Preset: " << value;
    this->loadTempPreset(value);
    AppManager::getInstance().getImageManager().setImageGroup(value);
    m_lastPreset = value;
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
    if(value < 0 || value >= m_presetNames.size()){
        return;
    }
    
    m_currentPreset.set(value);
}


