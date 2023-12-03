@@ -0,0 +1,14 @@
@ECHO OFF
REM Clean Everything

ECHO "Cleaning everything..."

REM Engine
mingw32-make -f "Makefile.engine.mak" clean
IF %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit)

REM application
mingw32-make -f "Makefile.application.mak" clean
IF %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit)

ECHO "All assemblies cleaned successfully."