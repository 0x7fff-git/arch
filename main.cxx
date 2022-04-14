#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>

#include "dep/font.hxx"
#include "dep/back.hxx"

std::int32_t main( ) {
   const auto sf_path = std::string{ std::getenv( "HOME" ) } + "/.config/xorg/.run";
   if ( !std::filesystem::exists( sf_path ) )
      return 0;
   
   std::ifstream sf_conf{ sf_path };
   if ( !sf_conf.is_open( ) || !sf_conf.good( ) )
      return 0;
   std::string sf_from_conf{ };
   while ( std::getline( sf_conf, sf_from_conf ) )
      if ( !sf_from_conf.empty( ) && std::filesystem::exists( sf_from_conf ) )
         break;
   
   const sf::RenderWindow sf_win{ { 700, 100 }, "", 0 };
   sf_win.setPosition( { 610, 490 } );
   sf_win.setFramerateLimit( 90 );
   
   const sf::Font sf_font{ };
   sf_font.loadFromMemory( mem_font, sizeof( mem_font ) );
   
   const sf::Texture sf_ava{ };
   sf_ava.loadFromFile( sf_from_conf );
   sf_ava.setSmooth( 1 );
   sf_ava.generateMipmap( );
   
   const sf::Texture sf_bck{ };
   sf_bck.loadFromMemory( mem_bck, sizeof( mem_bck ) );
   sf_bck.setSmooth( 1 );
   sf_bck.generateMipmap( );
   
   const sf::Sprite sf_bck_sprite{ sf_bck };
   const sf::Sprite sf_ava_sprite{ sf_ava };
   
   sf_ava_sprite.setScale( 100.f / sf_ava.getSize( ).x, 100.f / sf_ava.getSize( ).y );
   
   const sf::Text sf_text{ };
   sf_text.setFont( sf_font );
   sf_text.setCharacterSize( 20 );
   sf_text.setFillColor( { 225, 191, 245 } );
   sf_text.setPosition( { 143 , 37 } );
   
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
            if ( std::isprint( sf_event.text.unicode ) && sf_text.findCharacterPos( -1 ).x <= 600 )
               sf_input += static_cast< const std::uint8_t >( sf_event.text.unicode );
            break;
         }
      }
      
      if ( sf::Keyboard::isKeyPressed( sf::Keyboard::LControl )
        && sf::Keyboard::isKeyPressed( sf::Keyboard::BackSpace ) )
        sf_input = "";
      
      sf_text.setString( sf_input + "_" );
      
      sf_win.clear( );
      sf_win.draw( sf_ava_sprite );
      sf_win.draw( sf_bck_sprite );
      sf_win.draw( sf_text );
      sf_win.display( );
   }
}
