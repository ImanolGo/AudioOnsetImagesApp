#include "ofMain.h"
#include "AudioOnsetImagesApp.h"
#include "WindowSettingsManager.h"

//========================================================================
int main( ){
    

    WindowSettingsManager::WindowSettingsVector windowSettings = WindowSettingsManager::getInstance().getWindowsSettings();

    
    shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(windowSettings[0]);
    
    windowSettings[1].shareContextWith = mainWindow;
    shared_ptr<ofAppBaseWindow> secondWindow = ofCreateWindow(windowSettings[1]);

    //secondWindow->setVerticalSync(false);
    
    shared_ptr<AudioOnsetImagesApp> mainApp(new AudioOnsetImagesApp);
    ofAddListener(secondWindow->events().draw,mainApp.get(),&AudioOnsetImagesApp::drawScreen2);
    ofAddListener(secondWindow->events().keyPressed,mainApp.get(),&AudioOnsetImagesApp::keyPressed2);
    
    ofRunApp(mainWindow, mainApp);
    mainWindow->setWindowShape(windowSettings[0].width, windowSettings[0].height);
    ofRunMainLoop();

}
