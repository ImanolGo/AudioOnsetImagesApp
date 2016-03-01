/*
 *  ImageManager.cpp
 *  AudioOnsetImagesApp
 *
 *  Created by Imanol Gomez on 01/03/16.
 *
 */


#include "ofMain.h"

#include <time.h>

#include "ImageManager.h"
#include "AppManager.h"


ImageManager::ImageManager(): Manager()
{
	//Intentionally left empty
}


ImageManager::~ImageManager()
{
    ofLogNotice() <<"ImageManager::Destructor" ;
}


void ImageManager::setup()
{
	if(m_initialized)
		return;


	Manager::setup();

    this->loadImages();
    ofLogNotice() <<"ImageManager::initialized" ;

}



void ImageManager::loadImages()
{
    
}


void ImageManager::update()
{

}

void ImageManager::draw()
{

}
