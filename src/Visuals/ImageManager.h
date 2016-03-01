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
#include "WindowSettingsManager.h"


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
    
        void nextImage();
    
        void onChangeFadeTime(float& value);
    

    private:

        void loadImages();
    
        string getImageName(const string& path);
    
        void setAnimations();
    
        void loadNextImage();
    
        void nextImageIndex();


    private:
    
        typedef                 vector<string>          ImageNameVector;       ///< defines a vector of images names
        typedef                 vector<int>             IndexVector;       ///< defines a vector of indexes
    
        ImageNameVector         m_imageNames;       ///< image names sorted by name
        IndexVector             m_indexes;          ///< current available indexes
    
        int                     m_currentIndex;     ///< current image index
        float                   m_fadeTime;         ///< fade time for the animations
    
        ofPtr<ImageVisual>      m_currentImage;  ///< current image to be displayed

};
