/*
 *  AudioManager.h
 *  AudioOnsetImagesApp
 *
 *  Created by Imanol Gomez on 01/03/16.
 *
 */


#pragma once

#include "Manager.h"

#include "ofxBeatTracking.h"
#include "TextVisual.h"

#include "RectangleVisual.h"
#include "BasicVisual.h"


//========================== class AudioManager ==============================
//============================================================================
/** \class AudioManager AudioManager.h
 *	\brief Class managing the audio input
 *	\details It reads from the input microphone and calculates the energy.
 */


class AudioManager: public Manager
{

public:

    //! Constructor
    AudioManager();

    //! Destructor
    ~AudioManager();

    //! Setup the Audio Manager
    void setup();

    //! Update the Audio Manager
    void update();

    //! Draw the Audio Manager
    void draw();

    float getAudioMax();

    void onChangeVolume(float& value);
    
    void onChangeThreshold(float& value);
    
    void onChangeDecayTime(float& value);
    
    void audioReceived(float* input, int bufferSize);


private:

    void setupBeatTracker();
    
    void setupText();
    
    void setupMidiNotes();

    void drawFFT();
    
    void setAnimations();

    void sendAllNotesOff();
    
    void sendMidiNotesOn();
    
private:
    
    typedef     vector<int>  MidiNoteVector;     ///<Vector of Midi Notes

    float           m_volume;
    ofxBeatTracking m_beatTracker;
    float           m_audioMax;
    float           m_threshold;
    float           m_minimumThreshold;
    float           m_maxThreshold;
    float           m_decayRate;
    float           m_decayTime;
    
    
    ofPtr<RectangleVisual>  m_rect;
    ofPtr<TextVisual>       m_text;
    ofPtr<BasicVisual>      m_animationVisual;
    MidiNoteVector          m_notes;

};
