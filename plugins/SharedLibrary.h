#ifndef PLUGINENGINE_SHAREDLIBRARY_H
#define PLUGINENGINE_SHAREDLIBRARY_H

#include "Config.h"

#include <string>
#include <stdexcept>

#if defined(PLUGINENGINE_WIN32)

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <windows.h>

#elif defined(PLUGINENGINE_LINUX)

#include <dlfcn.h>

#else

#error Please implement the shared library functions for your system

#endif

namespace PluginEngine {

  // ----------------------------------------------------------------------- //

#if defined(PLUGINENGINE_WIN32)

  // Shared library loading and access on windows
  class SharedLibrary {

    // Handle by which DLLs are referenced
    public: typedef HMODULE HandleType;

    // Loads the DLL from the specified path
    public: PLUGINENGINE_API static HandleType Load(const std::string &path) {
      std::string pathWithExtension = path + ".dll";

      HMODULE moduleHandle = ::LoadLibraryA(pathWithExtension.c_str());
      if(moduleHandle == NULL) {
        throw std::runtime_error("Could not load DLL");
      }

      return moduleHandle;
    }

    // Unloads the DLL with the specified handle
    public: PLUGINENGINE_API static void Unload(HandleType sharedLibraryHandle) {
      BOOL result = ::FreeLibrary(sharedLibraryHandle);
      if(result == FALSE) {
        throw std::runtime_error("Could not unload DLL");
      }
    }

    // Looks up a function exported by the DLL
    public: template<typename TSignature>
    static TSignature *GetFunctionPointer(
      HandleType sharedLibraryHandle, const std::string &functionName
    ) {
      FARPROC functionAddress = ::GetProcAddress(
        sharedLibraryHandle, functionName.c_str()
      );
      if(functionAddress == NULL) {
        throw std::runtime_error("Could not find exported function");
      }

      return reinterpret_cast<TSignature *>(functionAddress);
    }

  };

#endif

  // ----------------------------------------------------------------------- //

#if defined(PLUGINENGINE_LINUX)

  // Shared library loading and access on windows
  class SharedLibrary {

	  // Handle by which shared objects are referenced
  	  public: typedef void * HandleType;

  	  // Loads the shared object from the specified path
  	  public: PLUGINENGINE_API static HandleType Load(const std::string &path)
  	  {
  		  // std::string pathWithExtension = std::string("./lib") + path + ".so";
  		  std::string pathWithExtension = path;

  		  void *sharedObject = ::dlopen(pathWithExtension.c_str(), RTLD_NOW);
  		  if (sharedObject == NULL) {
  			  throw std::runtime_error(
  					  std::string("Could not load '") + pathWithExtension + "'"
  			  );
  		  }

  		  return sharedObject;
  	  }

  	  // Unloads the shared object with the specified handle
  	  public: PLUGINENGINE_API static void Unload(HandleType sharedLibraryHandle)
  	  {
  		  int result = ::dlclose(sharedLibraryHandle);
  		  if (result != 0) {
  			  throw std::runtime_error("Could not unload shared object");
  		  }
  	  }

  	  // Looks up a function exported by the shared object
  	  public: template<typename TSignature>
  	  static TSignature *GetFunctionPointer(
  			  HandleType sharedLibraryHandle, const std::string &functionName
  	  ) {
  		  ::dlerror(); // clear error value

  		  void *functionAddress = ::dlsym(
  				  sharedLibraryHandle, functionName.c_str()
  		  );

  		  const char *error = ::dlerror(); // check for error
  		  if (error != NULL) {
  			  // throw std::runtime_error("Could not find exported function '"
  					  // + functionName + "'" + error);
  			  throw std::runtime_error(error);
  		  }

  		  return reinterpret_cast<TSignature *>(functionAddress);
  	  }

  };

#endif

  // ----------------------------------------------------------------------- //

} // namespace PluginEngine

#endif // PLUGINENGINE_SHAREDLIBRARY_H
