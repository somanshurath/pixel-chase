@echo off

pushd ..
external\Walnut\vendor\bin\premake\Windows\premake5.exe --file=build-client.lua vs2022
external\Walnut\vendor\bin\premake\Windows\premake5.exe --file=build-server.lua vs2022
popd
pause
