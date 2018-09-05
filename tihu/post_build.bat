:: tihu post build commands
::
@ECHO OFF


set Platform=%1%
set Configuration=%2%

copy ".\bin\%Platform%\%Configuration%\tihu.dll"   "..\build\tihu.dll"
copy ".\bin\%Platform%\%Configuration%\tihu.lib"   "..\build\tihu.lib"