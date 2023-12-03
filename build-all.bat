REM Build Everything

ECHO "Building everything..."

REM Engine
mingw32-make -f "Makefile.engine.mak" all
IF %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit)

REM application
mingw32-make -f "Makefile.application.mak" all
IF %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit)


ECHO "All assemblies built successfully."