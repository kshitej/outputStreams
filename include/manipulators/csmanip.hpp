#ifndef CSMANIP_HPP
#define CSMANIP_HPP

// STD headers
#include <iostream>
#include <string>
#include <map>
#include <set>

namespace osm
{
    //====================================================
    //     USING ALIASES
    //====================================================
    using string_pair_map = std::map<std::string, std::pair<std::string, std::string>>;
    using string_set_map = std::map<std::string, std::set<std::string>>;

    //====================================================
    //     ENUM CLASSES
    //====================================================
    enum class CURSOR
    {
        ON,
        OFF
    };

    //====================================================
    //     VARIABLES DECLARATION
    //====================================================
    extern struct color_pair;
    extern color_pair col[];
    extern const std::map<std::string, std::string> sty, rst, tcs;
    extern const string_pair_map crs, tcsc;

    //====================================================
    //     FUNCTIONS DECLARATION
    //====================================================
    extern std::string feat( const std::map<std::string, std::string> & generic_map, const std::string & feat_string );
    extern std::string feat( const string_pair_map & generic_map, const std::string & feat_string, const int & feat_int );
    extern std::string reset( const std::string & reset_string );
    extern std::string go_to( const int & x, const int & y );
    extern std::string RGB( const int & r, const int & g, const int & b );

    //====================================================
    //     TEMPLATE FUNCTIONS DECLARATION
    //====================================================
    template <typename T>
    extern void OPTION( const T & opt );

    //====================================================
    //     TEMPLATE "printf" FUNCTIONS DEFINITION
    //====================================================
    template <typename... Args>
    inline void print( std::ostream & os, const Args &... args )
    {
        if( &os == &std::cerr )
            os << feat( col, "red" ) << feat( sty, "bold" );
        else if( &os == &std::clog )
            os << feat( col, "lt blue" ) << feat( sty, "bold" );

        ( os << ... << args ) << "\n";

        if( &os == &std::cerr || &os == &std::clog || &os == &std::cout ) os << reset( "all" );
    }

    template <typename... Args>
    inline void print( Args &... args )
    {
        ( std::cout << ... << args ) << "\n"
                                     << reset( "all" );
    }
}      // namespace osm

#endif