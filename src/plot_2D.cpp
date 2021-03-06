
/*
 * This file is part of outputStream
 * Licensed under the MIT License 
 * Copyright (c) 2022 Kshitej Jadhav
 */
// My headers
#include "plot_2D.hpp"

namespace outStream
{
    //====================================================
    //     CONSTRUCTORS AND DESTRUCTORS DEFINITION
    //====================================================
    Plot2DCanvas::Plot2DCanvas( unsigned int w, unsigned int h ):
     Canvas( w, h )
    {
        offset_x_ = 0;
        offset_y_ = 0;
        scale_x_ = 1;
        scale_y_ = 1;
    }

    //====================================================
    //     SETTERS DEFINITION
    //====================================================
    void Plot2DCanvas::setOffset( float xOff, float yOff )
    {
        offset_x_ = xOff;
        offset_y_ = yOff;
    }

    void Plot2DCanvas::setScale( float xScale, float yScale )
    {
        scale_x_ = xScale;
        scale_y_ = yScale;
    }

    //====================================================
    //     GETTERS DEFINITION
    //====================================================
    float Plot2DCanvas::getOffsetX() const
    {
        return offset_x_;
    }

    float Plot2DCanvas::getOffsetY() const
    {
        return offset_y_;
    }

    float Plot2DCanvas::getScaleX() const
    {
        return scale_x_;
    }

    float Plot2DCanvas::getScaleY() const
    {
        return scale_y_;
    }
}      // namespace outStream