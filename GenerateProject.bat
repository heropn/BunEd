@echo off
call premake5 vs2022
ping -n 2 localhost >nul