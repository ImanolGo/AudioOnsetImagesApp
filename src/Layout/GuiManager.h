/*
 *  GuiManager.h
 *  AudioOnsetImagesApp
 *
 *  Created by Imanol Gomez on 01/03/16.
 *
 */


#pragma once

#include "Manager.h"
#include "ofxImGui.h"
//========================== class GuiManager ==============================
//============================================================================
/** \class GuiManager GuiManager.h
 *	\brief Class managing the application´s grapical user interface (GUI)
 *	\details It creates the gui and the callback functions that will be affected
 *    by the gui
 */

class GuiManager: public Manager
{
    static const string GUI_SETTINGS_FILE_NAME;
    static const string GUI_SETTINGS_NAME;
    
    
public:
    
    static const float GUI_WIDTH;
    static const float MARGIN;

    //! Constructor
    GuiManager();

    //! Destructor
    ~GuiManager();

    //! Set-up the gui
    void setup();
    
    //! update the gui
    void update();
    
    //! Draw the gui
    void draw();
    
    void saveAllPresets();
    
    void saveCurrentPreset();
    
    void saveTempPreset(int index);
    
    void loadTempPreset(int index);
    
    void toggleGui();
    
    void showGui(bool show){m_showGui=show;}
    
    int getWidth() {return m_width;}
    
    int getHeight() {return m_height;}
    
    ofPoint  getPosition() {return m_position;}
    
    void  onPresetChange(int& value);
    
    
    vector<int> getNoteIndexes();
    
    void setFadeTime(float value) {m_fadeTime = value;}
    
    void setFadeTimeMin(float value) {m_fadeTimeMin = value;}
    
    void setFadeTimeMax(float value) {m_fadeTimeMax = value;}
    
    void setRandomImages(bool value) {m_randomImages = value;}
    
    void setRandomFadeTime(bool value) {m_randomFadeTime = value;}
    
    void setStop(bool value) {m_stop = value;}
    
    void setPause(bool value) {m_pause = value;}

    void setNoFade(bool value) {m_noFade = value;}
    
    void setThreshold(float value) {m_threshold = value;}
    
    void setLowFreqCut(float value) {m_lowFreqCut = value;}
    
    void setHighFreqCut(float value) {m_highFreqCut = value;}
    
    void setDecayTime(float value) {m_decayTime = value;}
    
    void setBrightness(float value) {m_brightness = value;}
    
    void setCrossfading(bool value) {m_crossFadeImgs = value;}
    
    void setNote(const ofVec2f& value);
    
    void setPreset(int value);
    
private:
    
    void drawGui();
    
    void setupGuiParameters();
    
    void setupGuiPresets();
    
    void setupImageGui();
    
    void setupAudioGui();
    
    void setupPresets();
    
    void drawRectangle();
    
    void updatePresets();
    
    void deleteTempPresets();
    
    void updateSize(const ofxImGui::Settings& settings);

private:
    
    ofxImGui::Gui			m_gui;
    ofxImGui::Gui			m_guiPresets;


    ofParameterGroup    m_parameters;
    ofParameter<bool>   m_stop;
    ofParameter<bool>   m_pause;
    ofParameter<bool>   m_blackout;
    
    
    ofParameterGroup    m_parametersImage;
    ofParameterGroup    m_parametersAudio;
    ofParameterGroup    m_parametersPresets;
    
    ofParameterGroup        m_matrixNotes;
    
    ofParameterGroup        m_matrixPresets;
    
    	
    ofParameter<float>    m_fadeTime;
    ofParameter<float>    m_fadeTimeMin;
    ofParameter<float>    m_fadeTimeMax;
    ofParameter<bool>     m_randomImages;
    ofParameter<bool>     m_randomFadeTime;
    ofParameter<bool>     m_noFade;
    ofParameter<bool>     m_crossFadeImgs;
    ofParameter<int>      m_brightness;
   
    
    ofParameter<int>            m_lowFreqCut;
    ofParameter<int>            m_highFreqCut;
    ofParameter<float>          m_threshold;
    ofParameter<float>          m_decayTime;
    ofParameter<int>            m_midiChannel;
    vector<ofParameter<bool>>   m_notes_params;
    vector<int>                 m_noteIndexes;
    
    ofParameter<int>            m_currentPreset;
    int                         m_lastPreset;
    std::vector<std::string>    m_presetNames;
   
    bool        m_showGui;  //It defines the whether the gui should be shown or not
    
    vector<ofColor>         m_colors;
    int                     m_switchColor;
    
    float m_width, m_height;
    ofPoint   m_position;
};

//==========================================================================


