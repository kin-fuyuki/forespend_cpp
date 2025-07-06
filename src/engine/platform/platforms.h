/*
likely support roadmap:

major desktop platforms
 software	| hardware
sdl2 -> fb0 -> uefi -> bios -> gl -> vulkan -> metal -> misc
mobile platforms
android opengl -> android vulkan -> metal -> misc
console platforms
xbox -> ps1 -> ps2 -> xbox360 -> ps3 -> ps4 -> xboxone -> xbos series ->
nintendo wii -> nintendo wiiu -> nintendo 64 -> nintendo 3ds -> nintendo switch ->
sega saturn -> sega dreamcast
embedded systems
raspberry pi

other systems will be added by the community with a basic guide on how to port the engine, its gonna be easy since
the code is "plug and play"

*/

#define RENDERER_NULL 0b00000001 // no graphics, custom from user
#define RENDERER_FB0 0b00000011 //linux framebuffer (without X11/wayland/gui interface, kinda like DOS mode)
#define RENDERER_SDL2 0b00000100
#define RENDERER_GL 0b00001001
#define RENDERER_VULKAN 0b00010001


#define HARDWARE_DESKTOP 0b00000001
#define HARDWARE_MOBILE 0b00000010
#define HARDWARE_CONSOLE 0b00000100
#define HARDWARE_EMBEDDED 0b00001000

// desktop systems
//boot
#define SYSTEM_BIOS 0b00000010
#define SYSTEM_UEFI 0b00000011
//windows
#define SYSTEM_WIN32 0b00001000
#define SYSTEM_NT32 0b00001010
#define SYSTEM_NT64 0b00001011
#define SYSTEM_NT7_32 0b00001100
#define SYSTEM_NT7_64 0b00001101
#define SYSTEM_NT1X_32 0b00001110
#define SYSTEM_NT1X_64 0b00001111
//linux
/* NOT YET SUPPORTED
#define SYSTEM_LINUX_2_32 0b00010000
#define SYSTEM_LINUX_2_64 0b00010001*/
#define SYSTEM_LINUX_5_32 0b00010010
#define SYSTEM_LINUX_5_64 0b00010011
/* NOT YET SUPPORTED
//mac
#define SYSTEM_MACOS_8 0b00010100
#define SYSTEM_MACOS_10_5 0b00010100
#define SYSTEM_MACOS_11 0b00010101
#define SYSTEM_MACOS_15 0b00010110
//bsd
#define SYSTEM_F_BSD_ARM 0b00011000
#define SYSTEM_O_BSD 0b00011001 // please compile per release, there wont be precompiled binaries
#define SYSTEM_F_BSD_386 0b00011010
#define SYSTEM_F_BSD_64 0b00011011
#define SYSTEM_N_BSD_1 0b00011100
#define SYSTEM_N_BSD_5 0b00011101
#define SYSTEM_D_BSD_1 0b00011110
#define SYSTEM_D_BSD_5 0b00011111
//misc
#define SYSTEM_SOLARIS_X86 0b10000000 //only 2.5.1+
#define SYSTEM_SOLARIS_SPARC 0b10000001
#define SYSTEM_MINIX_X86 0b10000010
#define SYSTEM_MINIX_ARM 0b10000011
#define SYSTEM_SERENITY 0b10000100
#define SYSTEM_HAIKU 0b10000101
#define SYSTEM_WEB 0b10000110
//mobile systems
//android
#define SYSTEM_ANDROID_2 0b00000000
#define SYSTEM_ANDROID_4 0b00000001
#define SYSTEM_ANDROID_7 0b00000010
#define SYSTEM_ANDROID_9 0b00000011
#define SYSTEM_ANDROID_10 0b00000100
#define SYSTEM_ANDROID_12 0b00000101
#define SYSTEM_ANDROID_14 0b00000110
//#define SYSTEM_ANDROID_16 0b00000111 future release
//ios
#define SYSTEM_IOS_4 0b00001000
#define SYSTEM_IOS_11 0b00001001
#define SYSTEM_KAIOS 0b00001010
//console systems
#define SYSTEM_XBOX 0b00000000
#define SYSTEM_XBOX360 0b00000001
#define SYSTEM_XBOXONE 0b00000010
#define SYSTEM_XBOXSERIES 0b00000011
#define SYSTEM_PLAYSTATION 0b00000100
#define SYSTEM_PLAYSTATION2 0b00000101
#define SYSTEM_PLAYSTATION3 0b00000110
#define SYSTEM_PLAYSTATION4 0b00000111
#define SYSTEM_SEGA_SATURN 0b00001000
#define SYSTEM_SEGA_DREAMCAST 0b00001001
#define SYSTEM_NINTENDO_64 0b00001010
#define SYSTEM_NINTENDO_WII 0b00001011
#define SYSTEM_NINTENDO_WIIU 0b00001100
#define SYSTEM_NINTENDO_SWITCH 0b00001101
#define SYSTEM_NINTENDO_3DS 0b00001110
//embedded systems
#define SYSTEM_RASPBERRY_PI 0b00000000
*/


// for testing
#define TARGET_SYSTEM SYSTEM_NT1X_64
#define TARGET_HARDWARE HARDWARE_DESKTOP
#define TARGET_RENDERER RENDERER_SDL2

#if TARGET_HARDWARE == HARDWARE_DESKTOP

	#if TARGET_SYSTEM == SYSTEM_BIOS
		#include "bios.h"
	#elif TARGET_SYSTEM == SYSTEM_UEFI
		#include "uefi.h"
	#elif TARGET_SYSTEM == SYSTEM_WIN32
		#include "sdl.h"
	#elif TARGET_SYSTEM == SYSTEM_NT32
		#if TARGET_RENDERER == RENDERER_SDL2
			#include "sdl2.h"
		#elif TARGET_RENDERER == RENDERER_GL
			#include "opengl.h"
		#else
		#include "null.h"
		#endif
	#elif TARGET_SYSTEM == SYSTEM_NT64
		#if TARGET_RENDERER == RENDERER_SDL2
			#include "sdl2.h"
		#elif TARGET_RENDERER == RENDERER_GL
			#include "opengl.h"
		#else
		#include "null.h"
		#endif
	#elif TARGET_SYSTEM == SYSTEM_NT7_32
		#if TARGET_RENDERER == RENDERER_SDL2
			#include "sdl2.h"
		#elif TARGET_RENDERER == RENDERER_GL
			#include "opengl.h"
		#else
		#include "null.h"
		#endif
	#elif TARGET_SYSTEM == SYSTEM_NT7_64
		#if TARGET_RENDERER == RENDERER_SDL2
			#include "sdl2.h"
		#elif TARGET_RENDERER == RENDERER_GL
			#include "opengl.h"
		#else
		#include "null.h"
		#endif
	#elif TARGET_SYSTEM == SYSTEM_NT1X_32
		#if TARGET_RENDERER == RENDERER_SDL2
			#include "sdl2.h"
		#elif TARGET_RENDERER == RENDERER_GL
			#include "opengl.h"
		#elif TARGET_RENDERER == RENDERER_VULKAN
			#include "vulkan.h"
		#else
		#include "null.h"
		#endif
	#elif TARGET_SYSTEM == SYSTEM_NT1X_64
		#if TARGET_RENDERER == RENDERER_SDL2
			#include "sdl2.h"
		#elif TARGET_RENDERER == RENDERER_GL
			#include "opengl.h"
		#elif TARGET_RENDERER == RENDERER_VULKAN
			#include "vulkan.h"
		#else
		#include "null.h"
		#endif
	#elif TARGET_SYSTEM == SYSTEM_LINUX_5_32
		#if TARGET_RENDERER == RENDERER_SDL2
			#include "sdl2.h"
		#elif TARGET_RENDERER == RENDERER_GL
			#include "opengl.h"
		#elif TARGET_RENDERER == RENDERER_VULKAN
			#include "vulkan.h"
		#else
		#include "null.h"
		#endif
	#elif TARGET_SYSTEM == SYSTEM_LINUX_5_64
		#if TARGET_RENDERER == RENDERER_SDL2
			#include "sdl2.h"
		#elif TARGET_RENDERER == RENDERER_GL
			#include "opengl.h"
		#elif TARGET_RENDERER == RENDERER_VULKAN
			#include "vulkan.h"
		#elif TARGET_RENDERER == RENDERER_FB0
			#include "fb0.h"
		#else
		#include "null.h"
		#endif
	/* NOT YET SUPPORTED
	#elif TARGET_SYSTEM == SYSTEM_LINUX_2_32
	#elif TARGET_SYSTEM == SYSTEM_LINUX_2_64
	#elif TARGET_SYSTEM == SYSTEM_MACOS_8
	#elif TARGET_SYSTEM == SYSTEM_MACOS_10_5
	#elif TARGET_SYSTEM == SYSTEM_MACOS_11
	#elif TARGET_SYSTEM == SYSTEM_MACOS_15
	#elif TARGET_SYSTEM == SYSTEM_F_BSD_ARM
	#elif TARGET_SYSTEM == SYSTEM_O_BSD
	#elif TARGET_SYSTEM == SYSTEM_F_BSD_386
	#elif TARGET_SYSTEM == SYSTEM_F_BSD_64
	#elif TARGET_SYSTEM == SYSTEM_N_BSD_1
	#elif TARGET_SYSTEM == SYSTEM_N_BSD_5
	#elif TARGET_SYSTEM == SYSTEM_D_BSD_1
	#elif TARGET_SYSTEM == SYSTEM_D_BSD_5
	#elif TARGET_SYSTEM == SYSTEM_SOLARIS_X86
	#elif TARGET_SYSTEM == SYSTEM_SOLARIS_SPARC
	#elif TARGET_SYSTEM == SYSTEM_MINIX_X86
	#elif TARGET_SYSTEM == SYSTEM_MINIX_ARM
	#elif TARGET_SYSTEM == SYSTEM_SERENITY
	#elif TARGET_SYSTEM == SYSTEM_HAIKU
	#elif TARGET_SYSTEM == SYSTEM_WEB*/
	#else
	#include "null.h"
	#endif
/* NOT YET SUPPORTED
#elif TARGET_HARDWARE == HARDWARE_CONSOLE
	#if TARGET_SYSTEM == SYSTEM_XBOX
	#elif TARGET_SYSTEM == SYSTEM_XBOX360
	#elif TARGET_SYSTEM == SYSTEM_XBOXONE
	#elif TARGET_SYSTEM == SYSTEM_XBOXSERIES
	#elif TARGET_SYSTEM == SYSTEM_PLAYSTATION
	#elif TARGET_SYSTEM == SYSTEM_PLAYSTATION2
	#elif TARGET_SYSTEM == SYSTEM_PLAYSTATION3
	#elif TARGET_SYSTEM == SYSTEM_PLAYSTATION4
	#elif TARGET_SYSTEM == SYSTEM_SEGA_SATURN
	#elif TARGET_SYSTEM == SYSTEM_SEGA_DREAMCAST
	#elif TARGET_SYSTEM == SYSTEM_NINTENDO_64
	#elif TARGET_SYSTEM == SYSTEM_NINTENDO_WII
	#elif TARGET_SYSTEM == SYSTEM_NINTENDO_WIIU
	#elif TARGET_SYSTEM == SYSTEM_NINTENDO_SWITCH
	#elif TARGET_SYSTEM == SYSTEM_NINTENDO_3DS
	#else
	#include "null.h"
#endif

#elif TARGET_HARDWARE == HARDWARE_EMBEDDED
	#if TARGET_SYSTEM == SYSTEM_RASPBERRY_PI
	#else
	#include "null.h"
#endif

#elif TARGET_HARDWARE == HARDWARE_MOBILE
	#if TARGET_SYSTEM == SYSTEM_ANDROID_2
	#elif TARGET_SYSTEM == SYSTEM_ANDROID_4
	#elif TARGET_SYSTEM == SYSTEM_ANDROID_7
	#elif TARGET_SYSTEM == SYSTEM_ANDROID_9
	#elif TARGET_SYSTEM == SYSTEM_ANDROID_10
	#elif TARGET_SYSTEM == SYSTEM_ANDROID_12
	#elif TARGET_SYSTEM == SYSTEM_ANDROID_14
	#elif TARGET_SYSTEM == SYSTEM_IOS_4
	#elif TARGET_SYSTEM == SYSTEM_IOS_11
	#elif TARGET_SYSTEM == SYSTEM_KAIOS
	#else
	#include "null.h"
	#endif*/
#else
#include "null.h"
#endif