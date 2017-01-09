/*
 *  AudioOnsetImagesApp.cpp
 *  AudioOnsetImagesApp
 *
 *  Created by Imanol Gomez on 27/02/16.
 *
 */

#include "AppManager.h"

#include "AudioOnsetImagesApp.h"

//--------------------------------------------------------------
void AudioOnsetImagesApp::setup(){
    
    // Output->0 Input->mono  sampleRate->44100  bufSize->1024
    ofSoundStreamSetup(0, 1, this, 44100, 1024, 4);
    
    AppManager::getInstance().setup();
}

//--------------------------------------------------------------
void AudioOnsetImagesApp::update(){
    AppManager::getInstance().update();
}

//--------------------------------------------------------------
void AudioOnsetImagesApp::draw(){
    ofBackground(0);
    AppManager::getInstance().draw();
}

void AudioOnsetImagesApp::exit()
{
    ofLogNotice() <<"AudioOnsetImagesApp::Exit";

}


void AudioOnsetImagesApp::drawScreen2(ofEventArgs & args){
    ofBackground(255, 0, 0);
    
    int wIndex = 0;
    AppManager::getInstance().getPreviewManager().begin(wIndex);
        AppManager::getInstance().getImageManager().draw();
    AppManager::getInstance().getPreviewManager().end(wIndex);
    
    AppManager::getInstance().getPreviewManager().draw(wIndex);
    
    //AppManager::getInstance().getImageManager().draw();
}

void AudioOnsetImagesApp::keyPressed2(ofKeyEventArgs &e){
    int key = e.key;
    
    ofLogNotice() <<"KeyboardManager::keyPressed-> " + ofToString(key);
    
    
    
    if(key == ' ') {
        AppManager::getInstance().toggleDebugMode();
    }
    
    if(key == 'm') {
        int note  = ofRandom(48, 122);
        AppManager::getInstance().getMidiManager().sendNote(note, 127);
    }
    
    
    if(key == OF_KEY_RIGHT) {
        AppManager::getInstance().getImageManager().nextImage();
    }
    
    if(key == OF_KEY_LEFT) {
        AppManager::getInstance().getImageManager().previousImage();
    }

}




void AudioOnsetImagesApp::audioIn(float * input, int bufferSize, int nChannels)
{
    AppManager::getInstance().getAudioManager().audioIn(input, bufferSize, nChannels);
}


