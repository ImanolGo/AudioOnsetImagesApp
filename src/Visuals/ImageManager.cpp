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
    this->loadCurrentImage();
    ofLogNotice() <<"ImageManager::initialized" ;

}



void ImageManager::loadImages()
{
    //some path, may be absolute or relative to bin/data
    string path = "images/performance";
    ofDirectory dir(path);
    //only show png files
    dir.allowExt("png");
    dir.allowExt("jpeg");
    dir.allowExt("jpg");
    //populate the directory object
    dir.listDir();
    
    ofLogNotice()<< "ImageManager::loadImages-> Path: " << dir.getAbsolutePath();
    ofLogNotice()<< "ImageManager::loadImages-> Size: " << dir.size();
    
    //go through and print out all the paths
    for(int i = 0; i < dir.size(); i++){
        string name = this->getImageName(dir.getPath(i));
        ofLogNotice()<< "ImageManager::loadImages-> Loaded: " << name;
        AppManager::getInstance().getResourceManager().addTexture(name, dir.getPath(i));
        m_imageNames.push_back(name);
        
    }
}


void ImageManager::loadCurrentImage()
{
    if(m_imageNames.empty()){
        return;
    }
    
    int index = ofRandom(m_imageNames.size());
    float width = 1920;
    float height = 1200;
    
    m_currentImage.setResource(m_imageNames[index]);
    m_currentImage.setPosition(ofPoint(width*0.5,height*0.5));
    m_currentImage.setCentred(true);
    
    
    ofLogNotice()<< "ImageManager::loadImages-> Loading current image : " <<m_imageNames[index];
    if(m_currentImage.getOriginalWidth() > m_currentImage.getOriginalHeight()){
        m_currentImage.setWidth(width,true);
    }
    else{
        m_currentImage.setWidth(height,true);
    }
    
}

string ImageManager::getImageName(const string& path)
{
    std::vector<std::string> strs = ofSplitString(path, "/");
    //strs = ofSplitString(strs.back(), ".");
    return strs.back();
}

void ImageManager::update()
{

}

void ImageManager::draw()
{
    ofClear(0);
    m_currentImage.draw();
}
