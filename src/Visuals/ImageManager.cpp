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
#include "WindowSettingsManager.h"




ImageManager::ImageManager(): Manager(), m_currentIndex(0), m_fadeTime(2.0), m_random(false), m_randomFade(false),m_fadeTimeMin(0), m_fadeTimeMax(10), m_noFade(false), m_stop(false)
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

    m_currentImage =  ofPtr<ImageVisual>(new ImageVisual());

    this->loadImages();
    this->setImageGroup(0);
    this->loadNextImage();
    m_currentImage->setAlpha(0);
    
    ofLogNotice() <<"ImageManager::initialized" ;

}



void ImageManager::loadImages()
{
    ofLogNotice() <<"ImageManager::loading images ..." ;
    //some path, may be absolute or relative to bin/data
    string path = "images/performance/";
    ofDirectory dir(path);
    dir.listDir();
    
    ofLogNotice() <<"ImageManager::directory size: " << dir.size() ;
    
    for(int i = 0; i < dir.size(); i++)
    {
        string pathAux =  dir.getPath(i);
        ofDirectory dirAux(pathAux);
        if(this->loadSubfolder(dirAux, i))
        {
            m_folderNames.push_back(dir.getName(i));
            ofLogNotice()<< "ImageManager::loadImages-> Added folder: " << dir.getName(i);
        }
        
    }
   

    
}

bool ImageManager::loadSubfolder(ofDirectory& dir, int index)
{
    ofLogNotice() <<"ImageManager::loading subfolders ..." ;
    //only show png files
    dir.allowExt("png");
    dir.allowExt("jpeg");
    dir.allowExt("jpg");
    //populate the directory object
    
    
    if(dir.listDir()==0){
        ofLogNotice()<< "ImageManager::loadSubfolder-> No image files found in: " << dir.getAbsolutePath();
        return false;
    }
    
        
    ofLogNotice()<< "ImageManager::loadSubfolder-> Path: " << dir.getAbsolutePath();
    ofLogNotice()<< "ImageManager::loadSubfolder-> Size: " << dir.size();
    
    ImageNameVector images;
    
    //go through and print out all the paths
    for(int i = 0; i < dir.size(); i++){
        string name = this->getImageName(dir.getPath(i));
        ofLogNotice()<< "ImageManager::loadSubfolder-> Loaded: " << name;
        AppManager::getInstance().getResourceManager().addTexture(name, dir.getPath(i));
        images.push_back(name);
    }
    
    m_imageNames[index] = images;
    
    return true;
    
}

void ImageManager::setImageGroup(int index)
{
    if(m_imageNames.find(index)!=m_imageNames.end()){
        m_currentImageNames = m_imageNames[index];
        m_indexes.empty();
        m_currentIndex = 0;
        //this->nextImage();
    }
}

void ImageManager::nextImage()
{
    if(m_currentImageNames.empty()){
        return;
    }

    this->nextImageIndex();
    this->loadNextImage();
    
    if(!m_noFade){
        this->setAnimations();
    }
}

void ImageManager::previousImage()
{
    if(m_currentImageNames.empty()){
        return;
    }
    
    this->previousImageIndex();
    this->loadNextImage();
    
    if(!m_noFade){
        this->setAnimations();
    }
}

void ImageManager::previousImageIndex()
{
    if (m_stop) {
        return;
    }
    
    if(m_random){
        this->nextRandomImageIndex();
    }
    else{
        this->previousImageOrderedIndex();
    }
    
    //ofLogNotice()<< "ImageManager::nextImageIndex-> current index : " << m_currentIndex;
}

void ImageManager::nextImageIndex()
{
    if (m_stop) {
        return;
    }
    
    if(m_random){
        this->nextRandomImageIndex();
    }
    else{
        this->nextOrderedImageIndex();
    }
    
    //ofLogNotice()<< "ImageManager::nextImageIndex-> current index : " << m_currentIndex;
}

void ImageManager::nextOrderedImageIndex()
{
    
    m_currentIndex = (m_currentIndex+1)%m_currentImageNames.size();
}

void ImageManager::previousImageOrderedIndex()
{
    m_currentIndex--;
    if(m_currentIndex<0){
        m_currentIndex = m_currentImageNames.size()-1;
    }
    
}

void ImageManager::nextRandomImageIndex()
{
    if(m_indexes.empty()){
        for(int i = 0; i < m_currentImageNames.size(); i++){
            m_indexes.push_back(i);
        }
    }
    
    int n = (int) ceil( ofRandom(m_indexes.size()-1));
    
    
    m_currentIndex = m_indexes[n];
    m_indexes.erase(m_indexes.begin() + n);
    
    //ofLogNotice()<< "ImageManager::nextImageIndex-> current index : " << m_currentIndex;
}


void ImageManager::loadNextImage()
{

    WindowSettingsManager::WindowSettingsVector windowSettings = WindowSettingsManager::getInstance().getWindowsSettings();

    float width = windowSettings[1].width;
    float height = windowSettings[1].height;


    m_currentImage->setResource(m_currentImageNames[m_currentIndex]);
    m_currentImage->setPosition(ofPoint(width*0.5,height*0.5));
    m_currentImage->setCentred(true);
    m_currentImage->setAlpha(255);


    //ofLogNotice()<< "ImageManager::loadNextImage-> Loading current image : " <<m_currentImageNames[m_currentIndex];
    if(m_currentImage->getOriginalWidth() > m_currentImage->getOriginalHeight()){
        m_currentImage->setWidth(width,true);
    }
    else{
        m_currentImage->setWidth(height,true);
    }
}

void ImageManager::setAnimations()
{
    float fadeTime = this->getFadeTime();
    AppManager::getInstance().getVisualEffectsManager().removeAllVisualEffects(m_currentImage);
    AppManager::getInstance().getVisualEffectsManager().createFadeEffect(m_currentImage, 255.0, 0.0, 0.0, fadeTime);
}

float ImageManager::getFadeTime() const
{
    if(m_randomFade){
        return ofRandom(m_fadeTimeMin, m_fadeTimeMax);
    }
    
    return m_fadeTime;
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
    if (m_stop) {
        return;
    }
    
    ofClear(0);
    if(m_currentImage){
        m_currentImage->draw();
    }

}


void ImageManager::onChangeFadeTime(float& value)
{
    m_fadeTime = value;
}

void ImageManager::onChangeMinFadeTime(float& value)
{
    m_fadeTimeMin = value;
}

void ImageManager::onChangeMaxFadeTime(float& value)
{
    m_fadeTimeMax = value;
}


void ImageManager::onChangeRandomImages(bool& value)
{
    m_random = value;
    if(!m_random){
        m_currentIndex = m_currentImageNames.size() - 1;
        //ofLogNotice()<< "ImageManager::loadNextImage-> Set to image: " << m_currentIndex;
    }
}

void ImageManager::onChangeRandomFade(bool& value)
{
    m_randomFade = value;
}

void ImageManager::onChangeNoFade(bool& value)
{
    m_noFade = value;
}

void ImageManager::onChangeStop(bool &value)
{
    m_stop = value;
    
    if(m_stop){
        AppManager::getInstance().getVisualEffectsManager().removeAllVisualEffects(m_currentImage);
        m_currentImage->setAlpha(0);
    }
}

