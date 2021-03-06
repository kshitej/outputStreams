
/*
 * This file is part of outputStream
 * Licensed under the MIT License 
 * Copyright (c) 2022 Kshitej Jadhav
 */

#include <helper_tools.hpp>
#include <csmanip.hpp>
#include <progress_bar.hpp>

#include <iostream>
#include <string>
#include <chrono>
#include <set>
#include <map>
#include <cmath>
#include <mutex>
#include <stdexcept>

namespace outStream
{
    //====================================================
    //     DEFINITION OF THE CONSTRUCTORS AND DESTRUCTOR
    //====================================================
    template <typename bar_type>
    ProgressBar<bar_type>::ProgressBar():
     time_count_( duration::zero().count() ),
     ticks_occurred( 0 ),
     max_( 0 ),
     min_( 0 ),
     style_( "" ),
     type_( "" ),
     message_( "" ),
     brackets_open_( "" ),
     brackets_close_( "" ),
     color_( feat( t_ops::rst_color ) ),
     time_flag_( "off" ),
     begin_timer( steady_clock::now() )
    {
    }

    template <typename bar_type>
    ProgressBar<bar_type>::~ProgressBar() {}

    //====================================================
    //     DEFINITION OF THE STATIC ATTRIBUTES
    //====================================================
    template <typename bar_type>
    string_set_map ProgressBar<bar_type>::styles_map_ {
        { "indicator", { "%", "/100" } },
        { "loader", { "#", "■" } },
        { "spinner", { "/-\\|" } },
    };

    template <typename bar_type>
    std::vector<bar_type> ProgressBar<bar_type>::counter_( 2 );

    template <typename bar_type>
    const std::string ProgressBar<bar_type>::null_str = "";

    template <typename bar_type>
    const std::string ProgressBar<bar_type>::empty_space = " ";

    template <typename bar_type>
    std::mutex ProgressBar<bar_type>::mutex_;

    //====================================================
    //     DEFINITION OF THE SETTERS
    //====================================================
    template <typename bar_type>
    void ProgressBar<bar_type>::setMax( const bar_type & max )
    {
        max_ = max;
    }

    template <typename bar_type>
    void ProgressBar<bar_type>::setMin( const bar_type & min )
    {
        min_ = min;
    }

    // First setStyle overload, to set style of loader or indicator:
    template <typename bar_type>
    void ProgressBar<bar_type>::setStyle( const std::string & type, const std::string & style )
    {
        try
        {
            if( styles_map_.at( type ).find( style ) != styles_map_.at( type ).end() )
            {
                style_ = style;
                type_ = type;
            }
            else if( styles_map_.at( type ).find( style ) == styles_map_.at( type ).end() )
            {
                throw runtime_error_func( "Inserted ProgressBar style", style, "is not supported for this type!" );
            }
            else
            {
                throw runtime_error_func( "Inserted ProgressBar type", type, "is not supported!" );
            }
        }
        catch( std::out_of_range const & exception )
        {
            throw runtime_error_func( "Inserted ProgressBar type", type, "is not supported!" );
        }
    }

    // Second setStyle overload, to set style of complete bar:
    template <typename bar_type>
    void ProgressBar<bar_type>::setStyle( const std::string & type, const std::string & style_p, const std::string & style_l )
    {
        if( styles_map_.at( "indicator" ).find( style_p ) != styles_map_.at( "indicator" ).end() &&
            styles_map_.at( "loader" ).find( style_l ) != styles_map_.at( "loader" ).end() &&
            type == "complete" )
        {
            style_ = style_p + style_l;
            style_p_ = style_p;
            style_l_ = style_l;
            type_ = type;
        }
        else if( styles_map_.at( "indicator" ).find( style_p ) == styles_map_.at( "indicator" ).end() )
        {
            throw runtime_error_func( "Inserted indicator style", style_p, "is not supported for this type!" );
        }
        else if( styles_map_.at( "loader" ).find( style_l ) == styles_map_.at( "loader" ).end() )
        {
            throw runtime_error_func( "Inserted loader style", style_l, "is not supported for this type!" );
        }
        else
        {
            throw runtime_error_func( "Inserted ProgressBar type", type, "is not supported!" );
        }
    }

    template <typename bar_type>
    void ProgressBar<bar_type>::setMessage( const std::string & message )
    {
        message_ = message;
    }

    template <typename bar_type>
    void ProgressBar<bar_type>::setBegin()
    {
        begin = steady_clock::now();
    }

    template <typename bar_type>
    void ProgressBar<bar_type>::setEnd()
    {
        end = steady_clock::now();
        time_count_ += std::chrono::duration_cast<std::chrono::milliseconds>( end - begin ).count();
    }

    template <typename bar_type>
    void ProgressBar<bar_type>::setBrackets( const std::string & brackets_open, const std::string & brackets_close )
    {
        brackets_open_ = brackets_open,
        brackets_close_ = brackets_close;
    }

    template <typename bar_type>
    void ProgressBar<bar_type>::setColor( const t_ops& color )
    {
        color_ = feat( color );
    }

    template <typename bar_type>
    void ProgressBar<bar_type>::setRemainingTimeFlag( const std::string & time_flag )
    {
        time_flag_ = time_flag;
    }

    //====================================================
    //     DEFINITION OF THE RESETTERS
    //====================================================
    template <typename bar_type>
    void ProgressBar<bar_type>::resetAll()
    {
        max_ = static_cast<bar_type>( NULL ),
        min_ = static_cast<bar_type>( NULL ),
        style_ = "",
        type_ = "",
        message_ = "",
        time_count_ = 0,
        ticks_occurred = 0,
        begin_timer = steady_clock::now(),
        brackets_open_ = "",
        brackets_close_ = "",
        color_ = feat( t_ops::rst_color );
        time_flag_ = "off";
    }

    template <typename bar_type>
    void ProgressBar<bar_type>::resetMax()
    {
        max_ = static_cast<bar_type>( NULL );
    }

    template <typename bar_type>
    void ProgressBar<bar_type>::resetMin()
    {
        min_ = static_cast<bar_type>( NULL );
    }

    template <typename bar_type>
    void ProgressBar<bar_type>::resetStyle()
    {
        style_.clear();
        type_.clear();
    }

    template <typename bar_type>
    void ProgressBar<bar_type>::resetMessage()
    {
        message_.clear();
    }

    template <typename bar_type>
    void ProgressBar<bar_type>::resetTime()
    {
        time_count_ = duration::zero().count();
    }

    template <typename bar_type>
    void ProgressBar<bar_type>::resetRemainingTime()
    {
        ticks_occurred = 0;
        begin_timer = steady_clock::now();
    }

    template <typename bar_type>
    void ProgressBar<bar_type>::resetBrackets()
    {
        brackets_open_.clear(),
           brackets_close_.clear();
    }

    template <typename bar_type>
    void ProgressBar<bar_type>::resetColor()
    {
        color_ = feat( t_ops::rst_color );
    }

    //====================================================
    //     DEFINITION OF THE GETTERS
    //====================================================
    template <typename bar_type>
    bar_type ProgressBar<bar_type>::getMax() const
    {
        return max_;
    }

    template <typename bar_type>
    bar_type ProgressBar<bar_type>::getMin() const
    {
        return min_;
    }

    template <typename bar_type>
    long long ProgressBar<bar_type>::getTime() const
    {
        return time_count_;
    }

    template <typename bar_type>
    bar_type ProgressBar<bar_type>::getIteratingVar() const
    {
        return iterating_var_;
    }

    template <typename bar_type>
    std::string ProgressBar<bar_type>::getStyle() const
    {
        return style_;
    }

    template <typename bar_type>
    std::string ProgressBar<bar_type>::getStyleComplete() const
    {
        if( type_ == "complete" )
        {
            return "Percentage: \"" + style_p_ + "\"\n" +
               "Loader: \"" + style_l_ + "\"\n";
        }
        else
        {
            return style_;
        }
    }

    template <typename bar_type>
    std::string ProgressBar<bar_type>::getType() const
    {
        return type_;
    }

    template <typename bar_type>
    std::string ProgressBar<bar_type>::getMessage() const
    {
        return message_;
    }

    template <typename bar_type>
    std::string ProgressBar<bar_type>::getBrackets_open() const
    {
        return brackets_open_;
    }

    template <typename bar_type>
    std::string ProgressBar<bar_type>::getBrackets_close() const
    {
        return brackets_close_;
    }

    template <typename bar_type>
    std::string ProgressBar<bar_type>::getColor() const
    {
        return color_;
    }

    template <typename bar_type>
    std::string ProgressBar<bar_type>::getRemainingTimeFlag() const
    {
        return time_flag_;
    }

    //====================================================
    //     DEFINITION OF THE "one" METHOD
    //====================================================
    template <typename bar_type>
    bar_type ProgressBar<bar_type>::one( const bar_type & iterating_var )
    {
        if( isFloatingPoint( iterating_var ) )
        {
            if( counter_.size() < 2 )
            {
                counter_.push_back( iterating_var );
            }
            if( counter_.size() == 2 )
            {
                return abs( abs( counter_.front() ) - abs( counter_.back() ) );
            }
            return static_cast<bar_type>( NULL );
        }
        return 1;
    }

    //====================================================
    //     DEFINITION OF THE "remaining_time" METHOD
    //====================================================
    template <typename bar_type>
    void ProgressBar<bar_type>::remaining_time()
    {
        max_spin_ = check_condition(
           [ &max_ = max_ ]
           { return isFloatingPoint( max_ ); },
           roundoff( max_ - min_, 1 ) * 10 + 1,
           max_ - min_ + 1 );

        time_taken = steady_clock::now() - begin_timer;
        percentage_done = static_cast<float>( ticks_occurred ) / ( max_spin_ );
        time_left = time_taken * ( 1 / percentage_done - 1 );
        minutes_left = std::chrono::duration_cast<std::chrono::minutes>( time_left );
        seconds_left = std::chrono::duration_cast<std::chrono::seconds>( time_left - minutes_left );

        std::cout << "["
                  << feat( t_ops::italics ) + "Estimated time left: " + feat( t_ops::rst_italics )
                  << feat( t_ops::green ) << minutes_left.count() << feat( t_ops::rst_color ) << "m "
                  << feat( t_ops::green ) << seconds_left.count() << feat( t_ops::rst_color ) << "s"
                  << "]"
                  << feat( t_ops::clear_line );
    }

    //====================================================
    //     DEFINITION OF THE "update_output" METHOD
    //====================================================
    template <typename bar_type>
    void ProgressBar<bar_type>::update_output( const std::string & output )
    {
        std::cout << output
                  << getColor()
                  << check_condition(
                        [ &message_ = message_ ]
                        { return ( message_ != null_str ); },
                        empty_space + message_ + empty_space,
                        empty_space )
                  << feat( t_ops::rst_color );

        if( time_flag_ == "on" )
        {
            ticks_occurred++;
            remaining_time();
        }

        std::cout << std::flush;
    }

    //====================================================
    //     DEFINITION OF THE "update" METHOD
    //====================================================
    template <typename bar_type>
    void ProgressBar<bar_type>::update( const bar_type & iterating_var )
    {
        std::lock_guard<std::mutex> lock { mutex_ };

        iterating_var_ = 100 * ( iterating_var - min_ ) / ( max_ - min_ - one( iterating_var ) ),
        iterating_var_spin_ = check_condition(
           [ &iterating_var = iterating_var ]
           { return isFloatingPoint( iterating_var ); },
           roundoff( iterating_var, 1 ) * 10,
           iterating_var ),

        width_ = ( iterating_var_ + 1 ) / 4;

        // Update of the progress indicator only:
        if( styles_map_.at( "indicator" ).find( style_ ) != styles_map_.at( "indicator" ).end() )
        {
            output_ = feat( t_ops::cursor_backward, 100 ) +
               getColor() +
               std::to_string( static_cast<int>( round( iterating_var_++ ) ) ) +
               feat( t_ops::rst_color ) +
               getStyle();

            update_output( output_ );
        }

        // Update of the loader indicator only:
        else if( styles_map_.at( "loader" ).find( style_ ) != styles_map_.at( "loader" ).end() )
        {
            output_ = feat( t_ops::cursor_backward, 100 ) +
               getBrackets_open() +
               getColor() +
               getStyle() * width_ +
               empty_space * ( check_condition( [ &iterating_var = iterating_var ]
                                                { return isFloatingPoint( iterating_var ); },
                                                26, 25 ) -
                               width_ ) +
               feat( t_ops::rst_color ) +
               getBrackets_close();

            update_output( output_ );
        }

        // Update of the whole progress bar:
        else if( style_.find( style_p_ ) != std::string::npos && style_.find( style_l_ ) != std::string::npos && type_ == "complete" )
        {
            output_ = feat( t_ops::cursor_backward, 100 ) +
               getBrackets_open() +
               getColor() +
               style_l_ * width_ +
               empty_space * ( check_condition( [ &iterating_var = iterating_var ]
                                                { return isFloatingPoint( iterating_var ); },
                                                26, 25 ) -
                               width_ ) +
               feat( t_ops::rst_color ) +
               getBrackets_close() +
               getColor() +
               empty_space +
               std::to_string( static_cast<int>( round( iterating_var_++ ) ) ) +
               feat( t_ops::rst_color ) +
               style_p_;

            update_output( output_ );
        }

        // Update of the progress spinner:
        else if( styles_map_.at( "spinner" ).find( style_ ) != styles_map_.at( "spinner" ).end() )
        {
            output_ = feat( t_ops::cursor_backward, 100 ) +
               getColor() +
               getStyle()[ static_cast<unsigned long>( iterating_var_spin_ ) & 3 ] +
               feat( t_ops::green ) +
               check_condition(
                         [ = ]
                         { return roundoff( iterating_var, 1 ) == roundoff( max_, 1 ) - one( iterating_var ); },
                         static_cast<std::string>( feat( t_ops::cursor_backward, 100 ) + "0" ),
                         static_cast<std::string>( "" ) ) +
               feat( t_ops::rst_color );

            update_output( output_ );
        }

        else
        {
            throw std::runtime_error( "ProgressBar style has not been set!" );
        }
    }

    //====================================================
    //     DEFINITION OF THE "print" METHOD
    //====================================================
    template <typename bar_type>
    void ProgressBar<bar_type>::print() const
    {
        std::cout << "Max: " << max_ << std::endl
                  << "Min: " << min_ << std::endl
                  << "Time counter: " << time_count_ << std::endl
                  << "Style: " << style_ << std::endl
                  << "Type: " << type_ << std::endl
                  << "Message: " << message_ << std::endl
                  << "Brackets style: " << brackets_open_ << brackets_close_ << std::endl
                  << "Color: " << color_ << std::endl
                  << "Show remaining time: " << time_flag_ << std::endl;
    }

    //====================================================
    //     DEFINITION OF THE "addStyle" METHOD
    //====================================================
    template <typename bar_type>
    void ProgressBar<bar_type>::addStyle( const std::string & type, const std::string & style )
    {
        if( styles_map_.at( type ).find( style ) == styles_map_.at( type ).end() )
        {
            styles_map_.at( type ).insert( style );
        }
        else if( styles_map_.at( type ).find( style ) != styles_map_.at( type ).end() )
        {
            throw runtime_error_func( "Inserted ProgressBar style", style, "is already available!" );
        }
        else
        {
            throw runtime_error_func( "Inserted ProgressBar type", type, "is already available!" );
        }
    }

    //====================================================
    //     EXPLICIT INSTANTIATIONS
    //====================================================
    template class ProgressBar<int>;
    template class ProgressBar<long>;
    template class ProgressBar<long long>;
    template class ProgressBar<double>;
    template class ProgressBar<long double>;
    template class ProgressBar<float>;
}      // namespace outStream