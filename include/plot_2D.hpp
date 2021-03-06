
/*
 * This file is part of outputStream
 * Licensed under the MIT License 
 * Copyright (c) 2022 Kshitej Jadhav
 */
#ifndef PLOT2DCANVAS
#define PLOT2DCANVAS

// My headers
#include <canvas.hpp>

// STD headers
#include <functional>
#include <cmath>
#include <string>

namespace outStream
{
    //====================================================
    //     PLOT2DCANVAS CLASS
    //====================================================
    class Plot2DCanvas : public Canvas
    {
   public:
        //====================================================
        //     CONSTRUCTORS AND DESTRUCTORS DECLARATION
        //====================================================
        Plot2DCanvas( unsigned int w, unsigned int h );

        //====================================================
        //     SETTERS DECLARATION
        //====================================================
        void setOffset( float xOff, float yOff );
        void setScale( float xScale, float yScale );

        //====================================================
        //     GETTERS DECLARATION
        //====================================================
        float getOffsetX() const;
        float getOffsetY() const;
        float getScaleX() const;
        float getScaleY() const;

        //====================================================
        //     DRAW METHOD DEFINITION
        //====================================================
        template <typename Y, typename X>
        constexpr void draw( std::function<Y( X )> function, char c, const std::string & feat = "" )
        {
            for( int x = 0; x < static_cast<int>(width_); x++ )
            {
                float real_x = offset_x_ + x * scale_x_;
                Y real_y = function( real_x );
                unsigned int y = std::round( ( real_y - offset_y_ ) / scale_y_ );
                if( y >= 0 && y < height_ ) put( x, y, c, feat );
            }
        }

   private:
        //====================================================
        //     PRIVATE ATTRIBUTES DECLARATION
        //====================================================
        float offset_x_, offset_y_, scale_x_, scale_y_;
    };
}      // namespace outStream

#endif