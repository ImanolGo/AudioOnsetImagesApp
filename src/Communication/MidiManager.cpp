/*
 *  MidiManager.cpp
 *  AudioOnsetImagesApp
 *
 *  Created by Imanol Gomez on 08/03/16.
 *
 */


#include "MidiManager.h"
#include "AppManager.h"


MidiManager::MidiManager(): Manager(), m_channel(1)
{
    //Intentionally left empty
}

MidiManager::~MidiManager()
{
   ofLogNotice() << "MidiManager::destructor";
}


//--------------------------------------------------------------

void MidiManager::setup()
{
    if(m_initialized)
        return;

    Manager::setup();

    ofLogNotice() <<"MidiManager::initialized" ;

    this->setupMidiPorts();

}

void MidiManager::setupMidiPorts()
{
    // print the available output ports to the console
    m_midiOut.listPorts(); // via instance
    //ofxMidiOut::listPorts(); // via static too
    
    // connect
    m_midiOut.openPort(0); // by number
    //midiOut.openPort("IAC Driver Pure Data In"); // by name
    //midiOut.openVirtualPort("ofxMidiOut"); // open a virtual port
    
    
    ofLogNotice() <<"MidiManager::setupMidiPorts-> Connected to port: " << m_midiOut.getPort() << " \"" <<  m_midiOut.getName() <<  "\"" ;
    
}


//--------------------------------------------------------------
void MidiManager::sendNote(int note, int velocity)
{

    m_midiOut.sendNoteOn(m_channel, note,  velocity);
    
    if(velocity>0){
        // print out both the midi note and the frequency
        ofLogNotice() << "MidiManager::sendNote->note: " << note << ", velocity: " << velocity << ", freq: " << ofxMidi::mtof(note) << " Hz";
    }
}

//--------------------------------------------------------------

