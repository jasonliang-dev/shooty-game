@echo off

bin\sokol-shdc.exe -i src\shd_default.glsl -o src\shd_default.h -l glsl330:glsl100:hlsl4 -b -f sokol_impl
