/************************************************************
 *
 *      File : video_gen.cpp
 *      Author: T. Graba
 *      Credits: A. Polti
 *      Telecom ParisTECH
 *
 ************************************************************/


#ifndef VIDEO_GEN_H
#define VIDEO_GEN_H

#include <systemc>
#include "utils.h"      // Image dimensions included.

// Une image est un tableau de pixels (unsigned char, pour une image en niveaux de gris)
// On ne d�finit pas sa taille � l'avance, cela permettra de faire des 
// simulations avec des images plus petites...

typedef struct Image {
    int   width, height;  
    unsigned char  *pixel; 
} Image;

using namespace sc_core;
using namespace std;

namespace soclib { namespace caba {

    class VideoGen
        :sc_module {

            public:
                // IO PORTS

                sc_in_clk           clk;
                sc_in<bool>         reset_n;

                sc_out<bool>        line_valid;
                sc_out<bool>        frame_valid;

                sc_out<unsigned char> pixel_out;


                ////////////////////////////////////////////////////
                //	constructor
                ////////////////////////////////////////////////////

                VideoGen(sc_module_name insname,
//                        const char *bn = "wallace",         // nom de base par defaut
                        const char *bn = "img/gfourmi",     // nom de base par defaut
                        const int w = WIDTH,                // largeur d'image par defaut
                        const int h = HEIGHT,               // hauteur par defaut
                        const int lsync = LINE_SYNC,        // synchro ligne par defaut
                        const int fsync = ENDFRAME_SYNC     // synchro trame par defaut
                        );

                ////////////////////////////////////////////////////
                //	methods and structural parameters
                ////////////////////////////////////////////////////

                void                gen_sorties();    

            private:
                void                read_image();
                const char *        base_name;              // nom de base des images d'entree
                int                 current_image_number;   // numero de l'image courante

                // param�tres de l'image
                const uint32_t  p_WIDTH      ;
                const uint32_t  p_HEIGHT     ;
                const uint32_t  p_LINE_SYNC  ;
                const uint32_t  p_FRAME_SYNC ;

                Image               image;

            protected:
                SC_HAS_PROCESS(VideoGen);

        };
}}
#endif //VIDEO_GEN_H

