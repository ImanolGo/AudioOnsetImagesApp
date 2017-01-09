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
    
        void previousImage();
    
        void onChangeFadeTime(float& value);
    
        void onChangeMinFadeTime(float& value);
    
        void onChangeMaxFadeTime(float& value);
    
        void onChangeRandomImages(bool& value);
    
        void onChangeRandomFade(bool& value);
    
        void onChangeNoFade(bool& value);
    
        void onChangeStop(bool& value);
    
        void setImageGroup(int index);
    
        const vector<string>& getFoldersNames() {return m_folderNames;}
    

    private:

        void loadImages();
    
        bool loadSubfolder(ofDirectory& dir, int index);
    
        string getImageName(const string& path);
    
        float getFadeTime() const;
    
        void setAnimations();
    
        void loadNextImage();
    
        void nextImageIndex();
    
        void previousImageIndex();
    
        void nextRandomImageIndex();
    
        void nextOrderedImageIndex();
    
        void previousImageOrderedIndex();


    private:
    
        typedef                 vector<string>                  FolderNameVector;      ///< defines a vector of folder names
        typedef                 vector<string>                  ImageNameVector;       ///< defines a vector of images names
        typedef                 map<int,ImageNameVector>        ImageNameVectorMap;    ///< defines a map of image name vector
        typedef                 vector<int>                     IndexVector;           ///< defines a vector of indexes
       
    
        FolderNameVector        m_folderNames;          ///< folder names
        ImageNameVectorMap      m_imageNames;           ///< groups of image names
        ImageNameVector         m_currentImageNames;    ///< image names sorted by name
        IndexVector             m_indexes;              ///< current available indexes
    
        int                     m_currentIndex;     ///< current image index
        float                   m_fadeTime;         ///< fade time for the animations
        float                   m_fadeTimeMin;      ///< minimum fade time of random selection
        float                   m_fadeTimeMax;      ///< maximum fade time of random selection
        bool                    m_random;           ///< defines wether the images are selected randomly or in order
        bool                    m_randomFade;       ///< defines wether the images fade time are random or not
        bool                    m_noFade;           ///< defines if the image needs to fade or not
        bool                    m_stop;             ///< defines if the images will be shown or not
    
        ofPtr<ImageVisual>      m_currentImage;  ///< current image to be displayed

};
