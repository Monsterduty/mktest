#include "libsResolver.hpp"
#include "qt6Templates.hpp"

void resolveLibsFlags( std::string &buffer, std::vector<std::string> &results )
{
	using namespace std;
	//for compiler autocompletion flags
	#if defined(__WIN32)
		if ( buffer.find("SDL.h") != string::npos ) results.push_back("-lmingw32 -lSDL2main -lSDL2");
	#else
		if ( buffer.find("SDL.h") != string::npos ) results.push_back("-lSDL2");
	#endif
	if ( buffer.find("filesystem") != string::npos ) results.push_back("-std=c++17");
	if ( buffer.find("SDL_image.h") != string::npos ) results.push_back("-lSDL2_image");
	if ( buffer.find("Xlib.h") != string::npos ) results.push_back("-lX11");
	if ( buffer.find("curl.h") != string::npos ) results.push_back("-lcurl");
	if ( buffer.find("Imlib2.h") != string::npos ) results.push_back("-lImlib2");
	if ( buffer.find("SDL_mixer.h") != string::npos) results.push_back("-lSDL2_mixer");
	if ( buffer.find("SDL2_gfxPrimitives.h") != string::npos ) results.push_back("-lSDL2_gfx");
	if ( buffer.find("SDL_ttf.h") != string::npos  ) results.push_back("-lSDL2_ttf");
	
	if ( buffer.find( "Q" ) != string::npos || buffer.find("q") != string::npos )
	{
		results.push_back("-lQt6Core");
		results.push_back("-std=c++17");
		for ( int i = 0; i < 322; i++ )
		{
			if ( buffer.find( Qt6::widgetsHeaders[i] ) != string::npos )
			{
				results.push_back("-lQt6Widgets");
				break;
			}
		}
		for ( int i = 0; i < 24; i++ )
		{
			if ( buffer.find(Qt6::concurrentHeaders[i]) != string::npos )
			{
				results.push_back("-lQt6Concurrent");	
				break;
			}
		}
		for ( int i = 0; i < 341; i++ )
		{
			if ( buffer.find( Qt6::guiHeaders[i] ) != string::npos )
			{
				results.push_back("-lQt6Gui");
				break;
			}
		}
	}
}
