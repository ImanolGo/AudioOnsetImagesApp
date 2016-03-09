/*
 *  GuiManager.h
 *  AudioOnsetImagesApp
 *
 *  Created by Imanol Gomez on 01/03/16.
 *
 */


#pragma once

#include "Manager.h"
#include "ofxGuiExtended.h"
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
    
    static const int GUI_WIDTH;
    static const int MARGIN;

    //! Constructor
    GuiManager();

    //! Destructor
    ~GuiManager();

    //! Set-up the gui
    void setup();
    
    //! Draw the gui
    void draw();
    
    void saveGuiValues();
    
    void loadGuiValues();
    
    void toggleGui();
    
    void showGui(bool show){m_showGui=show;}
    
    int getWidth() {return GUI_WIDTH;}
    
    int getHeight() {return m_gui.getHeight();}
    
    ofPoint  getPosition() {return m_gui.getPosition();}
    
    void  onNoteChange(bool& value);
    
    int  getCurrentNoteIndex();
    
    vector<int> getNoteIndexes();
    
private:
    
    void setupGuiParameters();
    
    void setupImageGui();
    
    void setupAudioGui();
    
    void drawRectangle();

private:
    
    // Fluid GUI
    ofxPanel			m_gui;
    
    ofParameter<float>	m_guiFPS;
    
    
    ofParameterGroup    m_parametersImage;
    ofParameterGroup    m_parametersAudio;
    
    ofxGuiMatrix        m_matrixNotes;
    
    
    ofParameter<float>    m_fadeTime;
    ofParameter<float>    m_fadeTimeMin;
    ofParameter<float>    m_fadeTimeMax;
    ofParameter<bool>     m_randomImages;
    ofParameter<bool>     m_randomFadeTime;
    
    ofParameter<float>          m_threshold;
    ofParameter<float>          m_decayTime;
    vector<ofParameter<bool>>   m_notes_params;
    vector<int>                 m_noteIndexes;
    
   
    bool        m_showGui;  //It defines the whether the gui should be shown or not
};

//==========================================================================


