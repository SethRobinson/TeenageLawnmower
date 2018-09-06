for /r script %%a in (*.log) do del %%a

set C_RESOURCE_NAME=data\resource.cfl
del %C_RESOURCE_NAME%.ini
@echo off
:for /r script %%a in (*.c  *.pss) do makecfl -i %C_RESOURCE_NAME%.ini %%a -xzlib
:for /r scriptg %%a in (*.c  *.pss) do makecfl -i %C_RESOURCE_NAME%.ini %%a -xzlib
@echo on
//makecfl -i %C_RESOURCE_NAME%.ini script\*.*
//makecfl -i %C_RESOURCE_NAME%.ini scriptg\*.*
:makecfl -b %C_RESOURCE_NAME%.ini
del %C_RESOURCE_NAME%.ini
:copy data\resource.cfl ..\demo\data


