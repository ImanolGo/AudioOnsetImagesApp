/*
 *  AudioOnsetImagesApp.h
 *  AudioOnsetImagesApp
 *
 *  Created by Imanol Gomez on 27/02/16.
 *
 */

#pragma once

#include "ofMain.h"

class AudioOnsetImagesApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void exit();
    
        void drawScreen2(ofEventArgs & args);
    
        void keyPressed2(ofKeyEventArgs &e);
    
        void audioIn(float * input, int bufferSize, int nChannels);
    
};
