#include "Game.h"

int go()
{
    LOG_INFO( "Game starting..." );

	Game game;
	game.run();

    LOG_INFO( "Game ending..." );
    return 0 ;
}

int main( int argc, char* argv [] )
{
    return go() ;
}

#ifdef _WIN32
#include <Windows.h>
// ReSharper disable CppInconsistentNaming
int WINAPI WinMain( _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd )
{
    return go() ;
}
#endif
