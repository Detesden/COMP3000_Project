#pragma once


using CreateInterface_t = void* (__cdecl*)(const char* interfaceName, int* returnCode);
inline CreateInterface_t CreateInterface = nullptr; // The function for creating interfaces


// The interfaces
class IClientEntityList;
inline IClientEntityList* IClientEntityList_ = nullptr;
inline void* IBaseClientDLL = nullptr;
inline void* IClientMode = nullptr;