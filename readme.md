The code looks pretty simple and nice.

# how to update offsets

https://github.com/t1f7/scp-internal-il2cpp/wiki

# friendly reminder

SL-AC.dll hooks next functions to detect this base

```
GetProcAddress
LoadLibraryA
LoadLibraryW
GetModuleHandleA
GetModuleHandleW
GetAsyncKeyState
NtOpenFile
CreateThread
Sleep
il2cpp_string_new
il2cpp_resolve_icall
UnityPlayer.dll + 0x937480 (gettable with il2cpp_resolve_icall("UnityEngine.GameObject::FindGameObjectsWithTag(System.String)")), in other words "il2cpp::unity_find_objects" is detected also.
```

# il2cpp

I have found time to learn megapatch2 changes in SCPSL game.

Old repo is read-only from now.

# Preview

[![v0.4](https://img.youtube.com/vi/f-emyZ0ZKdo/0.jpg)](https://www.youtube.com/watch?v=f-emyZ0ZKdo)

This project is a base for il2cpp game hack. You can make a couple changes to make it work with any other game, not SCPSL only.

# Updates

v0.4: hooking game thread to bypass GC detection.

v0.5: fetching component data example.

v0.6: engine rendering, removed old GUI

v0.7: scopophobia update, AC bypass

v0.8: auto screen resolution change support

v0.9: better transform handling & more interesting il2cpp class

# Credits

* [Foxye](https://github.com/EquiFox) for his public hack that gave me inspiration to continue.
* Allan for great w2s code and cool c++ snippets.
