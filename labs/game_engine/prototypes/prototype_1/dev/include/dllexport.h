#pragma once

#ifdef	DLL_EXPORTS
#define EXPORT_CLASS	__declspec(dllexport)
#define EXPORT_API		extern "C" __declspec(dllexport) 
#else
#define EXPORT_CLASS	__declspec(dllimport)
#define EXPORT_API		extern "C" 
#endif
