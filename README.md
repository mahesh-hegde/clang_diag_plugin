## Description
This is a Clang-11 plugin which adds a diagnostic warning when variable or function names start with underscore `_`.

This is intended as an example to demonstrate building diagnostic plugins to clang.

## Requirements and building

Requires clang-11 with development files (headers) installed on your linux distribution. (Usually something like libclang-11-dev)

```
git clone https://github.com/mahesh-hegde/clang_diag_plugin.git
cd clang_diag_plugin/NoUnderscore
cmake . && cmake --build .
```

## Running:

```
cmake --build .
# This command is like running GCC
# It must be run after any changes in source file

clang++ -flugin=./NoUnderscore.so [source_files]
# Our plugin is a shared library which exports symbols
# to main Clang executable.
```

If you change the name of the project:

* Edit CMakeLists.txt to reflect the changes in filenames.

* remove all cmake related temporary files _except CMakeLists.txt_

* run `cmake .` before next `cmake --build .`

## sample output:

![Sample Output](Screenshot_1.png)

## Appendix

### LSP support
LSP (such as clang based C++ plugins for Vim / VS Code) are quite helpful when experimenting with clang plugins, because documentation is sparse.

If you have an LSP implementation installed and want it to work properly on C++ file, you have to install `bear`. Prefix it the first time you run a cmake build, like this:

```
bear -- cmake --build .
```

