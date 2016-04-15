/*
 *  MidiManager.h
 *  AudioOnsetImagesApp
 *
 *  Created by Imanol Gomez on 08/03/16.
 *
 */


#pragma once

#include "ofxMidi.h"
#include "Manager.h"

//========================== class MidiManager =======================================
//==============================================================================
/** \class MidiManager MidiManager.h
 *	\brief class for managing midi unputs
 *	\details It wraps the ofxMidi addon
 */


class MidiManager: public Manager
{

public:
    //! Constructor
    MidiManager();

    //! Destructor
    virtual ~MidiManager();

    //! setup
    void setup();
    
    void sendNote(int note, int velocity);
    
    void onChangeChannel(int & channel) {m_channel = channel;}


private:

    //! setups the midi ports
    void setupMidiPorts();

private:


    ofxMidiOut           m_midiOut;    ///< Hanles Midi output
    float                m_channel;    ///< midi channel

};
