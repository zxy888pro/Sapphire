#ifndef __SAPPHIRE_GRAPHICS__H__
#define __SAPPHIRE_GRAPHICS__H__

#include "Sapphire.h"
#include <handle.h>
#include <singleton.h>
#include <stringHelper.h>
#include <logUtil.h>
#include <BaseException.h>
#include <BaseResource.h>
#include <path.h>

#ifndef GLEW_STATIC
#define  GLEW_STATIC
#endif
#include <GL/glew.h>
#include <GLFW/glfw3.h>

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

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#ifndef STB_IMAGE_RESIZE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#endif

#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#endif

typedef  byte* PRAWIMAGE;


#include <GraphicsDefs.h>

#endif // !__SAPPHIRE_GRAPHICS__H__
