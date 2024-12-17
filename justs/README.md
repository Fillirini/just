# Project structure

**src** - module with C-headers. This is where the main API of the language is declared. 

**impl** - module with implementation of headers from the **src** module.
Platform-independent logic, such as *strings*, resides in the "general" directory.
Platform-specific logic, such as the *file system* and *console API*, resides in the remaining directories, named after the OS or that OS's API.

Platform:
1. **win32** (Windows)

---

The development of a standard API for other platforms is not yet underway, but is planned for Unix.

# Environment structure

The interpreter application must be located in the directory for app, for example `C:\Program Files` on Windows.
The hierarchy itself looks like this:

```
Just
    | justc
        | settings.config
        | justc.exe
        | libraries needed only for JustC
    | justs
        | settings.config
        | justs.exe
        | libraries needed only for JustS
    | std
        | uchar
          | about.config
          | uchar.dll
          | uchar.dll.lib
          | uchar.h
        | guilib
          | about.config
          | guilib.dll
          | guilib.dll.lib
          | guilib.h
```

The format of the `settings` and `about` configuration files has not yet been finalized, most likely it will be JSON or TOML
