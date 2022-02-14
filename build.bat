@echo off

cl /std:c++20 /W3 /Zi /EHsc /nologo /DSOKOL_D3D11 src/*.cpp /link /out:main.exe
