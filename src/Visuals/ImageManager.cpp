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




ImageManager::ImageManager(): Manager(), m_currentIndex(0), m_prevIndex(0), m_fadeTime(2.0), m_random(false), m_randomFade(false),m_fadeTimeMin(0), m_fadeTimeMax(10), m_noFade(false), m_stop(false), m_crossFadeImages(false)
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

    this->setupRectangle();
    this->loadImages();
    this->setImageGroup(0);
    this->loadNextImage();
    
    ofLogNotice() <<"ImageManager::initialized" ;

}

void ImageManager::setupRectangle()
{
    WindowSettingsManager::WindowSettingsVector windowSettings = WindowSettingsManager::getInstance().getWindowsSettings();
    
    float width = windowSettings[1].getWidth();
    float height = windowSettings[1].getHeight();
  
    m_darknessRect =  ofPtr<RectangleVisual> (new RectangleVisual(ofPoint(0,0,0), width, height));
    m_darknessRect->setColor(ofColor::black);
    m_darknessRect->setAlpha(0.0);
}

void ImageManager::loadImages()
{
    m_currentImage =  ofPtr<ImageVisual>(new ImageVisual());
    m_previousImage =  ofPtr<ImageVisual>(new ImageVisual());

    m_currentImage->setAlpha(0);
    m_previousImage->setAlpha(0);
    
    ofLogNotice() <<"ImageManager::loading images ..." ;
    //some path, may be absolute or relative to bin/data
    string path = "performance/";
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
        
        if(m_currentImageNames.size()<1){
            return;
        }
        
        m_currentIndex = m_currentImageNames.size() - 1;
        m_prevIndex = m_currentIndex - 1;
        if(m_prevIndex<0){
            m_prevIndex = m_currentImageNames.size()-1;
        }
        
        ofLogNotice()<< "ImageManager::setImageGroup-> group : " << index;
        
         //ofLogNotice()<< "ImageManager::setImageGroup-> m_imageNames size : " << m_currentImageNames.size();
        
        
        this->loadNextImage();
        AppManager::getInstance().getVisualEffectsManager().removeAllVisualEffects(m_currentImage);
        m_currentImage->setAlpha(0);
        m_previousImage->setAlpha(0);
        
    }
    

}

void ImageManager::nextImage()
{
    if(m_currentImageNames.empty()){
        return;
    }
    
    if(m_crossFadeImages && m_previousImage->getAlpha()>0){
        return;
    }
        
    this->nextImageIndex();
    this->loadNextImage();
    
    if(m_noFade){
        AppManager::getInstance().getVisualEffectsManager().removeAllVisualEffects(m_currentImage);
        AppManager::getInstance().getVisualEffectsManager().removeAllVisualEffects(m_previousImage);
        m_currentImage->setAlpha(255);
        m_previousImage->setAlpha(0);
       
    }
    else{
         this->setAnimations();
    }
    
     ofLogNotice()<< "ImageManager::nextImage> currentIndex : " << m_currentIndex << ",  prevIndex -> " << m_prevIndex;
 
}

void ImageManager::previousImage()
{
    
    if(m_currentImageNames.empty()){
        return;
    }
    
    if(m_crossFadeImages && m_previousImage->getAlpha()>0){
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
    m_prevIndex = m_currentIndex;
    m_currentIndex = (m_currentIndex+1)%m_currentImageNames.size();
}

void ImageManager::previousImageOrderedIndex()
{
    m_prevIndex = m_currentIndex;
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
    
    m_prevIndex = m_currentIndex;
    m_currentIndex = m_indexes[n];
    m_indexes.erase(m_indexes.begin() + n);
    
    //ofLogNotice()<< "ImageManager::nextImageIndex-> current index : " << m_currentIndex;
}


void ImageManager::loadNextImage()
{
    this->loadImage(m_currentImage, m_currentIndex);
    this->loadImage(m_previousImage, m_prevIndex);
}

void ImageManager::loadImage(ofPtr<ImageVisual> image, int index)
{
    if(index<0 || index>= m_currentImageNames.size()){
        ofLogNotice()<< "ImageManager::loadImage-> Non valid index : " << index;
    }
    
    ofLogNotice()<< "ImageManager::loadImage-> index : " << index;

    WindowSettingsManager::WindowSettingsVector windowSettings = WindowSettingsManager::getInstance().getWindowsSettings();
    
    float width = windowSettings[1].getWidth();
    float height = windowSettings[1].getHeight();
    
    
    image->setResource(m_currentImageNames[index]);
    image->setPosition(ofPoint(width*0.5,height*0.5));
    image->setCentred(true);
    
    
    //ofLogNotice()<< "ImageManager::loadNextImage-> Loading current image : " <<m_currentImageNames[m_currentIndex];
    if(image->getOriginalWidth() > image->getOriginalHeight()){
        image->setWidth(width,true);
    }
    else{
        image->setHeight(height,true);
    }
    
}

void ImageManager::setAnimations()
{
    AppManager::getInstance().getVisualEffectsManager().removeAllVisualEffects(m_currentImage);
    AppManager::getInstance().getVisualEffectsManager().removeAllVisualEffects(m_previousImage);

    EffectSettings settings;
    settings.animationTime =  this->getFadeTime();
    
    if(m_crossFadeImages){
        float currAlpha = m_currentImage->getAlpha();
        float prevAlpha = m_previousImage->getAlpha();
        m_previousImage->setAlpha(currAlpha);
        m_currentImage->setAlpha(0.0);
        
//        void createFadeEffect(ofPtr<BasicVisual> visual, double startAlpha,double endAlpha, EffectSettings& settings);
//
//        struct EffectSettings {
//            EasingFunction function = LINEAR;
//            EasingType type = EASE_IN;
//            double startAnimation = 0.0;
//            double animationTime = 0.0;
//        };
        
        
        AppManager::getInstance().getVisualEffectsManager().createFadeEffect(m_currentImage, 255.0, 0.0, settings);
        AppManager::getInstance().getVisualEffectsManager().createFadeEffect(m_previousImage, 0.0, 0.0, settings);
        //m_currentImage->setAlpha(prevAlpha);
        //m_previousImage->setAlpha(currAlpha);
        
    }
    else{
        AppManager::getInstance().getVisualEffectsManager().createFadeEffect(m_currentImage, 255.0, 0.0, settings);
        m_currentImage->setAlpha(255);
        m_previousImage->setAlpha(0);
    }
   
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
    
    ofPushStyle();
    ofEnableAlphaBlending();
    ofClear(0);
    if(m_currentImage){
        m_currentImage->draw();
    }
    
    if(m_crossFadeImages && m_previousImage){
         m_previousImage->draw();
    }
    
    m_darknessRect->draw();
    ofDisableAlphaBlending();
    ofPopStyle();
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
    m_prevIndex = m_currentIndex;
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
    
//    if(m_noFade){
//        AppManager::getInstance().getVisualEffectsManager().removeAllVisualEffects(m_currentImage);
//        AppManager::getInstance().getVisualEffectsManager().removeAllVisualEffects(m_previousImage);
//        
//        m_currentImage->setAlpha(255);
//        m_previousImage->setAlpha(0);
//    }
}

void ImageManager::stop(bool value)
{
    m_stop = value;
    
    if(m_stop){
        AppManager::getInstance().getVisualEffectsManager().removeAllVisualEffects(m_currentImage);
        AppManager::getInstance().getVisualEffectsManager().removeAllVisualEffects(m_previousImage);
        m_currentImage->setAlpha(0);
    }
}

void ImageManager::pause(bool value)
{
    m_stop = value;
    
     ofLogNotice()<< "ImageManager::pause-> Pause: " << m_stop;
    
    if(m_stop){
        AppManager::getInstance().getVisualEffectsManager().stopAllVisualEffects(m_currentImage);
        AppManager::getInstance().getVisualEffectsManager().stopAllVisualEffects(m_previousImage);
    }
    else{
        AppManager::getInstance().getVisualEffectsManager().continueAllVisualEffects(m_currentImage);
        AppManager::getInstance().getVisualEffectsManager().continueAllVisualEffects(m_previousImage);
    }
    
}

void ImageManager::blackout(bool value)
{
    m_stop = value;
    EffectSettings settings;
    settings.animationTime =  1;
    
    ofLogNotice()<< "ImageManager::blackout-> Blackout: " << m_stop;
    
    if(m_stop){
        AppManager::getInstance().getVisualEffectsManager().stopAllVisualEffects(m_currentImage);
        AppManager::getInstance().getVisualEffectsManager().stopAllVisualEffects(m_previousImage);
        
        AppManager::getInstance().getVisualEffectsManager().createFadeEffect(m_currentImage, 0.0, 0.0, settings);
        AppManager::getInstance().getVisualEffectsManager().createFadeEffect(m_previousImage, 0.0, 0.0, settings);
    }
}

void ImageManager::onChangeBrightness(int& value)
{
    int alpha = ofMap(value,0,255,255,0, true);
    m_darknessRect->setAlpha(alpha);
}

void ImageManager::onChangeCrossFade(bool& value)
{
    m_crossFadeImages = value;
}

