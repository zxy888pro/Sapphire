#ifndef __SAPPHIRE_GRAPHICS__H__
#define __SAPPHIRE_GRAPHICS__H__

#include "Sapphire.h"

//SAPPHIRE_WIN
#ifdef SAPPHIRE_WIN
#ifdef SAPPHIRE_GRAPHICS_EXPORT
//export
#ifdef SAPPHIRE_GRAPHICS_STATIC_LIB
#define SAPPHIRE_GRAPHICS_CLASS 
#define SAPPHIRE_GRAPHICS_API  extern "C"
#elif SAPPHIRE_GRAPHICS_DYNAMIC_LIB
#  define SAPPHIRE_GRAPHICS_CLASS  __declspec(dllexport)
#  define SAPPHIRE_GRAPHICS_API    extern "C" __declspec(dllexport)
#else
#define SAPPHIRE_GRAPHICS_CLASS 
#define SAPPHIRE_GRAPHICS_API 
#endif
#else
//import
#ifdef SAPPHIRE_GRAPHICS_STATIC_LIB
#define SAPPHIRE_GRAPHICS_CLASS 
#define SAPPHIRE_GRAPHICS_API  extern "C"
#elif SAPPHIRE_GRAPHICS_DYNAMIC_LIB
#  define SAPPHIRE_GRAPHICS_CLASS  __declspec(dllimport)
#  define SAPPHIRE_GRAPHICS_API    extern "C" __declspec(dllimport)
#else
#define SAPPHIRE_GRAPHICS_CLASS 
#define SAPPHIRE_GRAPHICS_API 

#endif
#endif

#else // SAPPHIRE_LINUX
#ifdef SAPPHIRE_GRAPHICS_EXPORT
//export
#ifdef SAPPHIRE_GRAPHICS_STATIC_LIB
#define SAPPHIRE_GRAPHICS_CLASS 
#define SAPPHIRE_GRAPHICS_API  
#elif SAPPHIRE_GRAPHICS_DYNAMIC_LIB
#  define SAPPHIRE_GRAPHICS_CLASS  
#  define SAPPHIRE_GRAPHICS_API   
#else
#define SAPPHIRE_GRAPHICS_CLASS 
#define SAPPHIRE_GRAPHICS_API 
#endif
#else
//import
#ifdef SAPPHIRE_GRAPHICS_STATIC_LIB
#define SAPPHIRE_GRAPHICS_CLASS 
#define SAPPHIRE_GRAPHICS_API  
#elif SAPPHIRE_GRAPHICS_DYNAMIC_LIB
#  define SAPPHIRE_GRAPHICS_CLASS  
#  define SAPPHIRE_GRAPHICS_API   
#else
#define SAPPHIRE_GRAPHICS_CLASS 
#define SAPPHIRE_GRAPHICS_API 
#endif
#endif

#endif




#endif // !__SAPPHIRE_GRAPHICS__H__
