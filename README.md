# Journey
This repository aim is a step by step following after The Cherno "Game Engine" series with some files difference.

## Project loading
> git clone https://github.com/toradziro/Journey.git --recurse-submodules

## Project building
For Windows run

> generate_project.bat

This command will generate .sln file (vs2022 is required). In VS just tap F5 and Sanbox app will be built and run since it's set as a Startup Project

## Engine modules and short notice of what 3d parties are used

- Entry point (dll main creates app via func provided by client app)
- Application Layer (TBD)
- Window Layer (TBD)
    - Input
    - Events
- Renderer (TBD)
- Renderer API (TBD)
- Debugging support
    - Logging System (spdlog for logging & fmt::format for strings)
- Scripting language (TBD)
- Memory System (TBD)
- ECS (TBD)
- Physics (TBD)
- File I/O, Virtual FS (TBD)
- Build System (for resources) (TBD)
- Build System (for lib and application - premake)
- Asset System (TBD)