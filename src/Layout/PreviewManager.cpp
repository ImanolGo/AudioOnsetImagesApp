/*
 *  PreviewManager.cpp
 *  AudioOnsetImagesApp
 *
 *  Created by Imanol Gomez on 01/03/16.
 *
 */

#include "ofMain.h"

#include "AppManager.h"
#include "PreviewManager.h"



PreviewManager::PreviewManager(): Manager()
{
	//Intentionally left empty
}


PreviewManager::~PreviewManager()
{
    ofLogNotice() <<"PreviewManager::Destructor";
}


void PreviewManager::setup()
{
	if(m_initialized)
		return;

	Manager::setup();

    this->setupFbos();
    this->setupText();
    this->setupRectangles();

    ofLogNotice() <<"PreviewManager::initialized";

}

void PreviewManager::setupFbos()
{
    //ofEnableAlphaBlending();
    WindowSettingsManager::WindowSettingsVector windowSettings = WindowSettingsManager::getInstance().getWindowsSettings();
    
    for (int i = 1; i < windowSettings.size(); i++) {
        ofPtr<ofFbo> fbo =  ofPtr<ofFbo>(new ofFbo());
        fbo->allocate(windowSettings[i].width, windowSettings[i].height, GL_RGBA);
        ofLogNotice() <<"PreviewManager::setupFbos -> width =  " << windowSettings[i].width << ", height =  " << windowSettings[i].height;
        
        fbo->begin(); ofClear(0); fbo->end();
        m_fbos.push_back(fbo);

    }
}

void PreviewManager::setupText()
{
    ofVec3f position;

    position.x = GuiManager::GUI_WIDTH + 2.5*GuiManager::MARGIN;
    position.y = 1.5*GuiManager::MARGIN;

    int width = 700;
    int fontSize = 12;
    int height = fontSize*3;

    string text = "IMAGES";
    ofPtr<TextVisual> textVisual = ofPtr<TextVisual>(new TextVisual(position, width, height));
    textVisual->setText(text, "fonts/open-sans/OpenSans-Semibold.ttf", fontSize);
    textVisual->setColor(ofColor::white);

    m_texts[text] = textVisual;
    AppManager::getInstance().getViewManager().addOverlay(textVisual);


}

void PreviewManager::setupRectangles()
{
    string key = "IMAGES";
    ofVec3f position = m_texts[key]->getPosition();
    position.x -= GuiManager::MARGIN*0.5;
    position.y -= GuiManager::MARGIN*0.5;
    float height = m_texts[key]->getHeight() + GuiManager::MARGIN;
    float width = m_texts[key]->getWidth() + GuiManager::MARGIN;
    ofPtr<RectangleVisual> rectangleVisual = ofPtr<RectangleVisual>(new RectangleVisual(position, width, height));
    ofColor color(60,60,60);
    rectangleVisual->setColor(color);

    AppManager::getInstance().getViewManager().addOverlay(rectangleVisual,2);


    int indexWindow = 0;
    if(indexWindow < m_fbos.size()){
        WindowSettingsManager::WindowSettingsVector windowSettings = WindowSettingsManager::getInstance().getWindowsSettings();

        position.y = 3*GuiManager::MARGIN;
        position.x = 2*GuiManager::MARGIN + GuiManager::GUI_WIDTH;

        if(m_fbos[indexWindow]->getWidth()> m_fbos[indexWindow]->getHeight()){
            width = (windowSettings[0].width - 4*GuiManager::MARGIN - GuiManager::GUI_WIDTH)*0.5;
            height = width *  m_fbos[indexWindow]->getHeight()/  m_fbos[indexWindow]->getWidth();
        }
        else{
            height = (windowSettings[0].width - 4*GuiManager::MARGIN - GuiManager::GUI_WIDTH)*0.5;
            width = height *  m_fbos[indexWindow]->getWidth()/  m_fbos[indexWindow]->getHeight();
        }
        //height = windowSettings.height * 0.4;
        //width = height * m_fbos[indexWindow]->getWidth() / m_fbos[indexWindow]->getHeight();

    }

    //ofLogNotice() <<"PreviewManager::rectangle -> x = "<< position.x << ", y =  " << width << ", width =  " << width << ", height =  " << height;

    rectangleVisual = ofPtr<RectangleVisual>(new RectangleVisual(position, width, height));
    rectangleVisual->setColor(color);

    m_rectangles[key] = rectangleVisual;
    AppManager::getInstance().getViewManager().addOverlay(rectangleVisual,2);


}

void PreviewManager::draw()
{
    this->drawPreview1();
}

void PreviewManager::drawPreview1()
{
    int indexWindow = 0;

    if(indexWindow<0 ||  indexWindow > (m_fbos.size()-1)){
        return;
    }

    string key = "IMAGES";

    float w = m_rectangles[key]->getWidth() - GuiManager::MARGIN;
    float h = m_rectangles[key]->getHeight() - GuiManager::MARGIN;


    ofVec3f position = m_rectangles[key]->getPosition();
    position.y += 0.5*GuiManager::MARGIN;
    position.x += 0.5*GuiManager::MARGIN;


    m_fbos[indexWindow]->draw(position.x, position.y, w, h);
}


void PreviewManager::draw(int windowIndex)
{
    if(windowIndex<0 ||  windowIndex > (m_fbos.size()-1)){
        return;
    }

    m_fbos[windowIndex]->draw(0,0);
}


void PreviewManager::begin(int windowIndex)
{
    if(windowIndex<0 ||  windowIndex > (m_fbos.size()-1)){
        return;
    }

    ofEnableAlphaBlending();
    m_fbos[windowIndex]->begin();
        ofClear(0,0,0);
}

void PreviewManager::end(int windowIndex)
{
    if(windowIndex<0 ||  windowIndex > (m_fbos.size()-1)){
        return;
    }

    m_fbos[windowIndex]->end();
    ofDisableAlphaBlending();
}
