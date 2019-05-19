@setlocal
@set throw=@goto :eof
@set exit=@goto :eof

call :%COMPILER%

@endlocal
@goto :eof

:MSVC_PROJ
set PATH=C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\MSBuild\15.0\Bin;%PATH%

cd far
MSBuild.exe /property:Configuration=%CONFIG% /property:Platform=%PLATFORM% far.vcxproj || %throw%

%exit%


:MSVC_NMAKE
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\VC\Auxiliary\Build\vcvars%PLATFORM:~-2%.bat" || %throw%
if %CONFIG%==Debug set DEBUG=1

cd far
nmake /f makefile_vc %ADD_MAKE% || %throw%
cd ..

cd plugins
nmake /f makefile_all_vc %ADD_MAKE% || %throw%
cd ..

%exit%


:CLANG_NMAKE
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\VC\Auxiliary\Build\vcvars%PLATFORM:~-2%.bat" || %throw%
set PATH=C:\Program Files\LLVM\bin;%PATH%
set CLANG=1
if %CONFIG%==Debug set DEBUG=1

cd far
nmake /f makefile_vc %ADD_MAKE% || %throw%
cd ..

%exit%


:GCC_MAKE
set PATH=C:\ProgramData\chocolatey\lib\mingw\tools\install\mingw%PLATFORM:~-2%\bin;%PATH%
if %CONFIG%==Debug set DEBUG=1

cd far
mingw32-make -j4 -f makefile_gcc %ADD_MAKE% || %throw%
cd ..

cd plugins
mingw32-make -j4 -f makefile_all_gcc %ADD_MAKE% || %throw%
cd ..

%exit%
