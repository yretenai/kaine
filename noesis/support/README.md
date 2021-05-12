# noesis

This directory holds Noesis Plugin support files.

Specifically these:

- `noesisplugin.cpp`
- `NoeSRShared.h`
- `pluginbasetypes.h`
- `pluginclasses.h`
- `pluginshare.h`
- `pluginsupport.cpp`

## pluginshare.h has two edits!!

- `#pragma pack(push, 1)` has been moved to after the include references 
    due to it raising error C2338 in winnt.h when compiling with Windows 10. 
- `#define NPLUGIN_API __declspec(dllexport)` has `extern "C"` added.

## pluginclasses.h has one major edit!

- Depending on your Windows SDK you might have to move the CArrayList class 
    implementations to the lines after where `noeRapi_t` is defined in 
    pluginshare.h. CArrayList depends on `noeRapi_t g_nfn`, but the point at 
    which pluginclasses.h is imported, g_nfn is not defined.
