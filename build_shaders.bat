@echo off

bin\sokol-shdc.exe -l glsl330:glsl100:hlsl4 -b -i src\shd_default.glsl -o src\shd_default.h
bin\sokol-shdc.exe -l glsl330:glsl100:hlsl4 -b -i src\shd_water.glsl -o src\shd_water.h
