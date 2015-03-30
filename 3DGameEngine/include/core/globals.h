#ifndef GLOBALS_H
#define GLOBALS_H


// Global Defines
#define BITMASK unsigned int					 // Bitmask is defined as an uint (min 16 bits) - change to unsigned long if more than 16 component types
#define BITMASK_SIZE  sizeof(BITMASK) * CHAR_BIT // size in bits of a bitmask (can change in different architecture)


//const std::string ASSETS_PATH = "assets/";    // path to assets. Change this if release seems to require weird folder structure
#define ASSETS_PATH std::string("assets/")
#define DEMO_SCENE_PATH "demo.xml"
#define AUTO_SCENE_PATH "gameStart.xml"				// auto load this scene if defined


namespace GLOBALS
{
	extern int SCREENHEIGHT;
	extern int SCREENWIDTH;
};

#endif