#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <map>

std::map< std::string, std::string >config{
   { "font_path", "/usr/share/fonts/TTF/Comfortaa-Bold.ttf" },
   { "font_size", "20" },
   { "font_color_r", "225" },
   { "font_color_g", "191" },
   { "font_color_b", "245" },
   { "font_pos_x", "143" },
   { "font_pos_y", "37" },
   
   { "input_delimiter", "600" },
   { "input_ctrl_back", "1" },
   { "input_cursor", "1" },
   
   { "window_size_x", "700" },
   { "window_size_y", "100" },
   { "window_pos_x", "610" },
   { "window_pos_y", "490" },
   { "window_vsync", "0" },
   { "window_rate", "60" },
   
   { "avatar_path", "/.config/cxx-run/avatar.png" },
   { "avatar_size_x", "100" },
   { "avatar_size_y", "100" },
   { "avatar_pos_x", "0" },
   { "avatar_pos_y", "0" },
   { "avatar_smooth", "1" },
   { "avatar_mipmap", "1" },
   
   { "background_path", "/.config/cxx-run/background.png" },
   { "background_size_x", "700" },
   { "background_size_y", "100" },
   { "background_pos_x", "0" },
   { "background_pos_y", "0" },
   { "background_smooth", "1" },
   { "background_mipmap", "1" }
};

[[ nodiscard ]]
const std::int8_t parse_config(
   std::map< std::string, std::string >&config,
   const std::filesystem::path config_path
) {
   if ( config.empty( ) || config_path.empty( ) ) {
      std::cerr << "* parse_config: config or path was empty" << std::endl;
      return 0;
   }
   
   const auto abs_path = std::string{ std::getenv( "HOME" ) } + config_path.c_str( );  
   if ( !std::filesystem::exists( abs_path ) ) {
      std::cerr << "* parse_config: path does not point to file" << std::endl;
      return 0;
   }
   
   if ( !std::filesystem::file_size( abs_path ) ) {
      std::cerr << "* parse_config: empty config file" << std::endl;
      return 0;
   }
   
   std::ifstream file{ abs_path, std::ios::in };
   if ( !file.is_open( ) ) {
      std::cerr << "* parse_config: failed to open file at path" << std::endl;
      return 0;
   }
   
   std::string line{ };
   while ( std::getline( file, line ) ) {
      if ( line.empty( ) || line[ 0 ] == '#' )
         continue;
      std::remove( line.begin( ), line.end( ), ' ' );
      
      const std::string key{ line.substr( 0, line.find( '=' ) ) };
      if ( key.empty( ) || !config.count( key ) ) {
         std::cerr << "* parse_config: skipping unrecognised key" << std::endl;
         continue;
      }
      
      const std::string value{ line.substr( line.find( '=' ) + 1, line.find( '\0' ) ) };
      if ( value.empty( ) ) {
         std::cerr << "* parse_config: skipping unrecognised value" << std::endl;
         continue;
      }
      config[ key ] = value;
   }
   
   return 1;
}

[[ nodiscard ]]
const std::int32_t hash(
   const std::string_view string
) {
   if ( string.empty( ) )
      return 0;
   std::int32_t hash{ };
   for ( std::size_t i{ 0 }; i < string.length( ); i++ )
      hash += std::tolower( string.at( i ) ) ^ i;
   return hash;
}

std::int32_t main(
   const std::int32_t argc,
   const std::int8_t* argv[ ]
) {
   for ( std::size_t i{ 1 }; i < argc; i++ ) {
      auto ctx = std::string_view{ reinterpret_cast< const char* >( argv[ i ] ) };
      if ( ctx.empty( ) )
         continue;
      
      switch ( hash( ctx ) ) {
      default:
         std::cerr << "* main: unrecognised option" << std::endl;
         [[ fallthrough ]]
      case 0x96:
      case 0x206:
         std::cout << std::string{
            "usage: cxx-run [option]\n"
               "\t-h, --help\t\t: displays this page\n"
               "\t-u, --use-config\t: use custom config\n"
               "\t-v, --version\t\t: print version\n\n"
            "aur: https://aur.archlinux.org/packages/cxx-run\n"
            "git: https://github.com/3llena-arch/cxx-run\n\n"
            "t. 3llena-arch <3llena-arch@protonmail.com>"
         } << std::endl;
         break;
      case 0xa4:
      case 0x35c:
         std::cout << "version: 16-04-2022 (build)" << std::endl;
         break;
      case 0xa1:
      case 0x43a:
         if ( !parse_config( config, "/.config/cxx-run/config" ) )
            std::cerr << "* main: error parsing config" << std::endl;
         break;
      }
   }
   
   const sf::RenderWindow sf_win{ { std::stoi( config[ "window_size_x" ] ), 
         std::stoi( config[ "window_size_y" ] ) }, "", 0 };
   sf_win.setPosition( { std::stoi( config[ "window_pos_x" ] ), std::stoi( config[ "window_pos_x" ] ) } );
   sf_win.setFramerateLimit( std::stoi( config[ "window_rate" ] ) );
   sf_win.setVerticalSyncEnabled( std::stoi( config[ "window_vsync" ] ) );
   
   const sf::Font sf_font{ };
   if ( !sf_font.loadFromFile( config[ "font_path" ] ) ) {
      std::cerr << "* main: failed to load font from path" << std::endl;
      return 0;
   }
   
   const sf::Texture sf_avatar{ };
   if ( !sf_avatar.loadFromFile( std::string{ std::getenv( "HOME" ) } + "/" + config[ "avatar_path" ] ) ) {
      std::cerr << "* main: failed to load avatar from path" << std::endl;
      return 0;
   }
   sf_avatar.setSmooth( std::stoi( config[ "avatar_smooth" ] ) );
   if ( std::stoi( config[ "avatar_mipmap" ] ) )
      sf_avatar.generateMipmap( );
   
   const sf::Texture sf_background{ };
   if ( !sf_background.loadFromFile( std::string{ std::getenv( "HOME" ) } + "/" + config[ "background_path" ] ) ) {
      std::cerr << "* main: failed to load background from path" << std::endl;
      return 0;
   }
   sf_background.setSmooth( std::stoi( config[ "background_smooth" ] ) );
   if ( std::stoi( config[ "background_mipmap" ] ) )
      sf_background.generateMipmap( );
   
   const sf::Sprite sf_background_sprite{ sf_background };
   const sf::Sprite sf_avatar_sprite{ sf_avatar };
   
   sf_background_sprite.setScale( std::float_t{ std::stoi( config[ "background_size_x" ] ) } / sf_background.getSize( ).x,
      std::float_t{ std::stoi( config[ "background_size_y" ] ) } / sf_background.getSize( ).y );
   sf_avatar_sprite.setScale( std::float_t{ std::stoi( config[ "avatar_size_x" ] ) } / sf_avatar.getSize( ).x,
      std::float_t{ std::stoi( config[ "avatar_size_y" ] ) } / sf_avatar.getSize( ).y );
   
   const sf::Text sf_text{ };
   sf_text.setFont( sf_font );
   sf_text.setCharacterSize( std::stoi( config[ "font_size" ] ) );
   sf_text.setFillColor( { std::stoi( config[ "font_color_r" ] ), 
      std::stoi( config[ "font_color_g" ] ), std::stoi( config[ "font_color_b" ] ) } );
   sf_text.setPosition( { std::stoi( config[ "font_pos_x" ] ), std::stoi( config[ "font_pos_y" ] ) } );
   
   sf::Event sf_event{ };
   std::string sf_input{ };
   
   while ( sf_win.isOpen( ) ) {
      if ( sf_win.pollEvent( sf_event ) ) {
         switch ( sf_event.type ) {
         case sf::Event::KeyPressed:
            if ( !sf_input.empty( ) ) {
               if ( sf_event.key.code == sf::Keyboard::BackSpace )
                  sf_input.pop_back( );
                  
               if ( sf_event.key.code == sf::Keyboard::Return ) {
                  std::system( std::string( sf_input + " & > /dev/null" ).c_str( ) );
                  sf_win.close( );
               }
            }
            
            if ( sf_event.key.code == sf::Keyboard::Escape
              || sf_event.key.code == sf::Keyboard::LAlt
              || sf_event.key.code == sf::Keyboard::LSystem )
               [[ fallthrough ]]
               
         case sf::Event::Closed:
         case sf::Event::LostFocus:
            sf_win.close( );
            break;
            
         case sf::Event::TextEntered:
            if ( std::isprint( sf_event.text.unicode ) && sf_text.findCharacterPos( -1 ).x <= std::stoi( config[ "input_delimiter" ] ) )
               sf_input += static_cast< const std::uint8_t >( sf_event.text.unicode );
            break;
         }
      }
      
      if ( sf::Keyboard::isKeyPressed( sf::Keyboard::LControl )
        && sf::Keyboard::isKeyPressed( sf::Keyboard::BackSpace )
        && std::stoi( config[ "input_ctrl_back" ] ) )
        sf_input = "";
      
      sf_text.setString( sf_input + ( std::stoi( config[ "input_cursor" ] ) ? "_" : "" ) );
      
      sf_win.clear( );
      sf_win.draw( sf_avatar_sprite );
      sf_win.draw( sf_background_sprite );
      sf_win.draw( sf_text );
      sf_win.display( );
   }
}
