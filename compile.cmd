..\bjam toolset=msvc preserve-test-targets=on %*
@echo off
call ..\boost-version.cmd
IF NOT ERRORLEVEL 1 (
    for /r %BUILD_DIRECTORY% %%f in (*.pdb) do xcopy /D /Y %%f ..\dist\bin
)
