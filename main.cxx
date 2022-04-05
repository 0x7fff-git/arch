#include <SFML/Graphics.hpp>
#include "image.hxx"
#include "font.hxx"

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if ( !pipe )
      return { };
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

std::int32_t main( ) noexcept {
   sf::RenderWindow window{ { 700, 100 }, "run", 0 };
   window.setFramerateLimit( 60 );
   window.setVerticalSyncEnabled( 1 );
   window.setPosition( { ( sf::VideoMode::getDesktopMode( ).width * 0.5f ) - 350,
                         ( sf::VideoMode::getDesktopMode( ).height * 0.5f ) - 50 } );
   sf::Font font{ };
   font.loadFromMemory( font_bytes, font_byte_length );
   sf::Text text{ "", font, 20 };
   text.setPosition( { 148, 37 } );
   std::string input_text{ };
   while ( window.isOpen( ) ) {
      if ( !window.hasFocus( ) )
         window.close( );
      sf::Event event{ };
      while ( window.pollEvent( event ) ) {
         switch ( event.type ) {
         case sf::Event::TextEntered:
            if ( !std::isprint( event.text.unicode ) )
               break;
            { sf::Vector2f position{ }; 
               if ( !input_text.empty( ) )
                  position = text.findCharacterPos( input_text.length( ) );
               if ( position.x <= 590 )
                  input_text += event.text.unicode;
            }
            break;
         case sf::Event::KeyPressed:
            switch ( event.key.code ) {
            case sf::Keyboard::Escape:
               window.close( );
               break;
            case sf::Keyboard::BackSpace:
               if ( !input_text.empty( ) )
                  input_text.pop_back( );
               break;
            case sf::Keyboard::Return:
               if ( input_text.length( ) > 0 )
                  std::system( std::string{ input_text + " &" }.c_str( ) );
               window.close( );
               break;
            }
            break;
         }
      }
      if ( sf::Keyboard::isKeyPressed( sf::Keyboard::LControl ) ) {
         if ( sf::Keyboard::isKeyPressed( sf::Keyboard::BackSpace ) )
            input_text = "";
         else if ( sf::Keyboard::isKeyPressed( sf::Keyboard::V ) ) {
            auto input = exec( "echo -n $( xclip -o )" );
            if ( input.length( ) < 35 )
               input_text = input;
         }
         else if ( sf::Keyboard::isKeyPressed( sf::Keyboard::C ) ) {
            std::system( std::string{ "echo -n '" + input_text + "' | xclip -i " }.c_str( ) );
         }
      }
      sf::Image image{ };
      image.loadFromMemory( image_bytes, image_byte_length );
      sf::Texture texture{ };
      texture.loadFromImage( image );
      sf::Sprite sprite{ };
      sprite.setTexture( texture, 1 );
      text.setString( input_text + "_" );
      text.setFillColor( { 225, 191, 245 } );
      window.clear( );
      window.draw( sprite );
      window.draw( text );
      window.display( );
   }
}
