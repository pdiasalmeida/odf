#ifndef CASCADECLASSIFIERDETECTORPLUGIN_CONFIG_H
#define CASCADECLASSIFIERDETECTORPLUGIN_CONFIG_H

// ------------------------------------------------------------------------- //

// Decides whether symbols are imported from a dll (client app) or exported to a
// dll (FileDataStorePlugin library). The FILEDATASTOREPLUGIN_SOURCE symbol is
// defined by all source files of the library, so you don't have to worry about a thing.
#if defined(_MSC_VER)

  #if defined(CASCADECLASSIFIERDETECTORPLUGIN_STATICLIB)
    #define CASCADECLASSIFIERDETECTORPLUGIN_API
  #else
    #if defined(CASCADECLASSIFIERDETECTORPLUGIN_SOURCE)
      // If we are building the DLL, export the symbols tagged like this
      #define CASCADECLASSIFIERDETECTORPLUGIN_API __declspec(dllexport)
    #else
      // If we are consuming the DLL, import the symbols tagged like this
      #define CASCADECLASSIFIERDETECTORPLUGIN_API __declspec(dllimport)
    #endif
  #endif

#elif defined(__GNUC__)

  #if defined(CASCADECLASSIFIERDETECTORPLUGIN_STATICLIB)
    #define CASCADECLASSIFIERDETECTORPLUGIN_API __attribute__ ((visibility ("default")))
  #else
    #if defined(CASCADECLASSIFIERDETECTORPLUGIN_SOURCE)
      #define CASCADECLASSIFIERDETECTORPLUGIN_API __attribute__ ((visibility ("default")))
    #else
      // If you use -fvisibility=hidden in GCC, exception handling and RTTI
      // would break if visibility wasn't set during export _and_ import
      // because GCC would immediately forget all type infos encountered.
      // See http://gcc.gnu.org/wiki/Visibility
      #define CASCADECLASSIFIERDETECTORPLUGIN_API __attribute__ ((visibility ("default")))
    #endif
  #endif

#else

  #error Unknown compiler, please implement shared library macros

#endif

// ------------------------------------------------------------------------- //

#endif // CASCADECLASSIFIERDETECTORPLUGIN_CONFIG_H
