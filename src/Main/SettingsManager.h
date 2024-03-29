/*
 *  SettingsManager.h
 *  Bentley App
 *
 *  Created by Imanol Gomez on 06/02/19.
 *
 */
#pragma once

#include "Manager.h"


//========================== class SettingsManager ==============================
//============================================================================
/** \class SettingsManager SettingsManager.h
 *    \brief Class managing the whole settings of the application
 *    \details it reads from an xml settings file and provides access to the information
 */

typedef             map<string,string>               ResourcesPathMap;       ///< defines a map of path attached to the resources name

class SettingsManager: public Manager
{

public:
    
    static const string APPLICATION_SETTINGS_FILE_NAME;
    
    //! Destructor
    ~SettingsManager();
    
    //! Constructor
    SettingsManager();
    
    //! Compares two transition objects
    void setup();
    
    const ResourcesPathMap& getTextureResourcesPath() const {return m_texturesPath;}
    
    const ResourcesPathMap& getVideoResourcesPath() const {return m_videoResourcesPath;}
    
    const ofColor& getColor(const string& colorName);
    
    float getAppWidth() const {return m_appWidth;}
    
    float getAppHeight() const {return m_appHeight;}
    
    int getDmxPort() const {return m_dmxPort;}
    
    int getUdpPortSend() const {return m_portUdpSend;}
    
    int getUdpPortReceive() const {return m_portUdpReceive;}
    
    int getOscPortReceive() const {return m_portOscReceive;}

    int getOscPortSend() const {return m_portOscSend;}
    
    const string& getIpAddress() const {return m_ipAddress;}
    
   
private:
    
    //! Loads the settings file
    bool loadSettingsFile();
    
    //! Loads all the settings
    void loadAllSettings();
    
    //! Sets all the debug properties
    void setDebugProperties();
    
    //! Sets all the network properties
    void setNetworkProperties();
    
    //! Sets all the window properties
    void setWindowProperties();
    
    //! Loads all the app colors
    void loadColors();
    
    //! Loads all the textures settings
    void loadTextureSettings();
    

    
private:
    
    typedef             map< string, ofColor >    ColorMap;               ///< Defines a map of colors attached to a name
    
    
    ofXml                    m_xml;                  ///< instance of the xml parser
    ResourcesPathMap        m_texturesPath;         ///< stores the texture paths
    ResourcesPathMap        m_videoResourcesPath;   ///< stores the video paths
    ColorMap                m_colors;               ///< stores all the application's colors
    float                   m_appWidth;             ///< stores the applications width
    float                   m_appHeight;            ///< stores the applications height
    ofColor                 m_blankColor;           ///<return blank color
    int                     m_dmxPort;             ///< stores the settings dmx port
    int                     m_portUdpSend;          ///< stores the UDP port used for the OSC Sending communications
    int                     m_portUdpReceive;       ///< stores the UDP port used for
    string                  m_ipAddress;            ///< stores the Ip Address used for the Network communications
    int                     m_portOscReceive;       ///< stores the UDP port to receive OSC messages from
    int                     m_portOscSend;          ///< stores the UDP port to send OSC messages to

};


