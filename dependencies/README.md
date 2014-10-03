= Dependencies for LUHOS =

The CMakeLists.txt-script within this directory automatically downloads and compiles most of LUGHOS' dependencies. The deps are packaged so they can be installed by a package manager (UNIXES) or with the help of a installer-application (WIN).

On the commandline type:

cmake ./

If you like to crosscompile for another system type:

cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/<Name of appropriate toolchain-file>

After completion of the compilation process all packages are in the created, OS-specific subdir.

