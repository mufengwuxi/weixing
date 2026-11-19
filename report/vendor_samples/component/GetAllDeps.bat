@echo off
setlocal EnableDelayedExpansion

cd ..
cd ..
set "DEP_QUEUE=IAtkObject.dll;AtGfxEngine.dll;"
set "Finded_QUEUE="

call :GetAllDeps "IAtkObject.dll"
call :GetAllDeps "AtGfxEngine.dll"
echo !DEP_QUEUE!

set "TARGET_DIR=IntegratingWithATK\component\bin"
if "%~1"=="Java" (
    set "TARGET_DIR=IntegratingWithATK\component\Java"
)
if "%~1"=="Python" (
    set "TARGET_DIR=IntegratingWithATK\component\Python"
)
if not exist "%TARGET_DIR%" (
    md "%TARGET_DIR%"
)
for  %%I in (!DEP_QUEUE!) do (
    copy /Y "%%I" "%TARGET_DIR%\" >nul
)
copy /Y "gdal204.dll" "%TARGET_DIR%\" >nul
copy /Y "geos.dll" "%TARGET_DIR%\" >nul
copy /Y "geos_c.dll" "%TARGET_DIR%\" >nul
copy /Y "osg161-osg.dll" "%TARGET_DIR%\" >nul
copy /Y "osg161-osgDB.dll" "%TARGET_DIR%\" >nul
copy /Y "osg161-osgGA.dll" "%TARGET_DIR%\" >nul
copy /Y "osg161-osgManipulator.dll" "%TARGET_DIR%\" >nul
copy /Y "osg161-osgShadow.dll" "%TARGET_DIR%\" >nul
copy /Y "osg161-osgSim.dll" "%TARGET_DIR%\" >nul
copy /Y "osg161-osgText.dll" "%TARGET_DIR%\" >nul
copy /Y "osg161-osgUtil.dll" "%TARGET_DIR%\" >nul
copy /Y "osg161-osgViewer.dll" "%TARGET_DIR%\" >nul
copy /Y "osgEarth.dll" "%TARGET_DIR%\" >nul
copy /Y "osgSimBaseObject.dll" "%TARGET_DIR%\" >nul
copy /Y "ot21-OpenThreads.dll" "%TARGET_DIR%\" >nul
copy /Y "proj_4_9.dll" "%TARGET_DIR%\" >nul
copy /Y "sqlite3.dll" "%TARGET_DIR%\" >nul
copy /Y "zlib.dll" "%TARGET_DIR%\" >nul
copy /Y "libcurl.dll" "%TARGET_DIR%\" >nul


pause
endlocal
exit /b 0


:GetAllDeps
set "CUR_DLL=%~1"
echo !Finded_QUEUE! | FINDSTR /i /c:"!CUR_DLL!" >nul && exit /b 0
set "Finded_QUEUE=!Finded_QUEUE!!CUR_DLL!;"
echo -------------------- find dep !CUR_DLL!
for /f "skip=1 tokens=*" %%I in ('.\IntegratingWithATK\component\dumpbin /dependents "!CUR_DLL!" 2^>nul ^| findstr /i "\.dll$"') do (
    set "DEP_DLL=%%I"
    if exist "!DEP_DLL!" (
        echo !DEP_QUEUE! | findstr /i /c:"!DEP_DLL!;" >nul
        if errorlevel 1 (
            echo     ++++++++++++++++++++ add to DEP_QUEUE !DEP_DLL!
            set "DEP_QUEUE=!DEP_QUEUE!!DEP_DLL!;"
        ) else (
            echo     ==================== in the DEP_QUEUE !DEP_DLL!
        )
    )
)
for  %%I in (!DEP_QUEUE!) do (
    call :GetAllDeps %%I
)
exit /b 0