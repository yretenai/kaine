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
    due to it raising error C2338 in winnt.h when compiling on Windows 10. 
- `#define NPLUGIN_API __declspec(dllexport)` has `extern "C"` added.
