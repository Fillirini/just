# Project structure

**src** - module with C-headers. This is where the main API of the language is declared. 

**impl** - module with implementation of headers from the **src** module.

Platform-independent logic, such as *strings*, resides in the "general" directory.

The logic that depends on a specific platform is located in the remaining directories, named after the OS or API of this OS.

Platform:
1. **win32** (Windows)

---

The development of a standard API for other platforms is not yet underway, but is planned for Unix.