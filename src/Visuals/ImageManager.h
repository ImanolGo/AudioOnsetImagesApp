/*
 *  ImageManager.h
 *  AudioOnsetImagesApp
 *
 *  Created by Imanol Gomez on 01/03/16.
 *
 */


#pragma once

#include "Manager.h"
#include "ImageVisual.h"


//========================== class ImageManager ==============================
//============================================================================
/** \class ImageManager ImageManager.h
 *	\brief Class managing the images to be exported
 *	\details it reads from the Led visuals and saves it on a file on demand
 */


class ImageManager: public Manager
{

    public:

        //! Constructor
        ImageManager();

        //! Destructor
        ~ImageManager();

        //! Setup the Image Manager
        void setup();

        //! Update the Image Manager
        void update();

        //! Draw the Image Manager
        void draw();
    
        void loadCurrentImage();

    private:

        void loadImages();
    
        string getImageName(const string& path);


    private:
    
        typedef                 vector<string>          ImageNameVector;       ///< defines a vector of images names
    
        ImageNameVector         m_imageNames;       ///< image names sorted by name
    
        ImageVisual             m_currentImage;  ///< current image to be displayed

};
