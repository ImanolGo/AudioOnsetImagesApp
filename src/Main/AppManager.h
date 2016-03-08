/*
 *  AppManager.h
 *  AudioOnsetImagesApp
 *
 *  Created by Imanol Gomez on 01/03/16.
 *
 */

#pragma once

#include "ofMain.h"

#include "Manager.h"

#include "SettingsManager.h"
#include "ResourceManager.h"
#include "GuiManager.h"
#include "VisualEffectsManager.h"
#include "ViewManager.h"
#include "ImageManager.h"
#include "KeyboardManager.h"
#include "PreviewManager.h"
#include "AudioManager.h"
#include "MidiManager.h"


//========================== class AppManager ==============================
//============================================================================
/** \class AppManager AppManager.h
 *	\brief Class managing the whole application
 *	\details it set-ups, updates and renders all the different managers used for the application
 */

class AppManager: public Manager
{
public:

    //! Destructor
    ~AppManager();

    //! Returns the singleton instance.
    static AppManager& getInstance();

    //! Compares two transition objects
    void setup();

    //! updates the logic
    void update();

    //! calls the view manager to draw
    void draw();

    //==========================================================================

    //! Returns the settings manager
    SettingsManager& getSettingsManager() { return m_settingsManager; }
    
    //! Returns the resource manager
    ResourceManager& getResourceManager() { return m_resourceManager; }

    //! Returns the GUI manager
    GuiManager& getGuiManager() { return m_guiManager; }

    //! Returns the view manager
    ViewManager&  getViewManager() { return m_viewManager; }

    //! Returns the visual effects manager
    VisualEffectsManager&  getVisualEffectsManager() { return m_visualEffectsManager; }

    //! Returns the Image manager
    ImageManager&   getImageManager() { return m_imageManager; }
    
    //! Returns the Preview Manager
    PreviewManager&  getPreviewManager() { return m_previewManager; }
    
    //! Returns the Audio Manager
    AudioManager&  getAudioManager() { return m_audioManager; }
    
    //! Returns the MIDI Manager
    MidiManager&  getMidiManager() { return m_midiManager; }
    

    //==========================================================================

    void toggleDebugMode();

    void setDebugMode(bool showDebug);


private:

     //! Constructor
     AppManager();

	//! Stop the compiler generating methods of copy the object
	 AppManager(AppManager const& copy);              // Don't Implement

    //! Prevent operator= being generated.
     AppManager& operator=(AppManager const& copy);     // Don't implement

    //==========================================================================

    //! Set-up all the managers
    void setupManagers();

    //! Set-up openFrameworks
    void setupOF();

    //! update all the managers
    void updateManagers();

private:

    SettingsManager          m_settingsManager;          ///< Manages the application's settings
    ResourceManager          m_resourceManager;          ///< Manages the application's resources
    
    KeyboardManager          m_keyboardManager;          ///< Manages the application's keyboard
    GuiManager               m_guiManager;               ///< Manages the application's settings
    ViewManager              m_viewManager;              ///< Manages the visuals
    VisualEffectsManager     m_visualEffectsManager;     ///< Manages the visual effects
    ImageManager             m_imageManager;             ///< Manages the images to be exported
    PreviewManager           m_previewManager;           ///< Manages the preview
    AudioManager             m_audioManager;             ///< Manages the audio
    MidiManager              m_midiManager;              ///< Manages the MIDI

    bool                     m_debugMode;
};

//==========================================================================
