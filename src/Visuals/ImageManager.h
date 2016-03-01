/*
 *  ImageManager.h
 *  AudioOnsetImagesApp
 *
 *  Created by Imanol Gomez on 01/03/16.
 *
 */


#pragma once

#include "Manager.h"


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

    private:

        void loadImages();


    private:

};
