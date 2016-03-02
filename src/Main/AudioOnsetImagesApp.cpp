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

void AudioOnsetImagesApp::audioReceived(float* input, int bufferSize, int nChannels) {
    AppManager::getInstance().getAudioManager().audioReceived(input, bufferSize);
}

