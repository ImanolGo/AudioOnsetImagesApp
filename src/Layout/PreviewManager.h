/*
 *  PreviewManager.cpp
 *  AudioOnsetImagesApp
 *
 *  Created by Imanol Gomez on 01/03/16.
 *
 */

#pragma once

#include "Manager.h"
#include "RectangleVisual.h"
#include "TextVisual.h"

//========================== class PreviewManager ==============================
//============================================================================
/** \class PreviewManager PreviewManager.h
 *	\brief Class managing the preview of the application
 *	\details It reads the render visuals in the other windows and previews them in the main window
 */

class PreviewManager: public Manager
{
public:

    //! Constructor
    PreviewManager();

    //! Destructor
    ~PreviewManager();

    //! Set-up the layout
    void setup();

    //! Draw Layer
    void draw();

    //! Draw a specific fbo
    void draw(int windowIndex);

    //! Begin fbo for a specific window
    void begin(int windowIndex);

    //! End fbo for a specific window
    void end(int windowIndex);


private:

    //! Set-up the fbos
    void setupFbos();

    //! Set-up the rectangle visuals
    void setupRectangles();

    //! Set-up the text visuals
    void setupText();

    void drawPreview1();


private:

    typedef  vector<ofPtr<ofFbo> >                  FboVector;        ///< defines a vector of FBOs
    typedef  map< string, ofPtr<RectangleVisual> >  RectanglesMap;    ///< defines a map of Rectangle Visuals sorted by name
    typedef  map<string, ofPtr<TextVisual> >        TextMap;         ///< defines a map of Text Visuals sorted by name

    FboVector          m_fbos;
    RectanglesMap      m_rectangles;
    TextMap            m_texts;

};

//==========================================================================
