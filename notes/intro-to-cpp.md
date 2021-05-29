# Introduction to C++

- [C++ Core Guidelines](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines) are a set of best practices, recommendations, and rules for coding in C++
- Use the [Standard Library](https://www.cplusplus.com/reference/) whenever possible
- Compiler compiles the source code into object (.o) and executable (.exe) files, and automatically links standard library object files

![Image of complier](https://video.udacity-data.com/topher/2019/July/5d2a71f4_c-compilation-process/c-compilation-process.svg)

- Build tools automate the process of compiling multiple source files, linking object files, and what changed that need to be recompiled
    + [GNU Make](https://www.gnu.org/software/make/manual/html_node/index.html#Top) relies on `Makefile`, which includes one ore more
    "_targets_"
        * `build` compile all of the source code into an executable file
        * `clear` delete all object files and other artifacts of the build process, return to a clean, unbuilt project state
    + [CMake](https://cmake.org/) facilitates cross-platform builds.
        * relies on `CMakeLists.txt`, which configures approciate targets
- [C++ Core Guidelines](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#nl-naming-and-layout-rules) and [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)