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
}
