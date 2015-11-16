@ECHO OFF

echo;
echo --------------------------------------------
echo ----- SCRIPT BEGIN FOR DEBUG VERSION -------
echo --------------------------------------------

set ALLDESTDIR=%~dp0\..\..\ComputreeInstallDebug

for /d %%X in (%~dp0\..\*) do (
	call:launch %%X %%~nX
)

for /d %%X in (%~dp0\..\..\plugin*) do (

	for /d %%Y in (%%X\*) do (
		call:launch %%Y %%~nY
	)
)

echo;
echo;
echo "////// !!!!!! \\\\\\\\\\\\"
echo;
echo "If you have problem with lupdate or lrelease (not found) please call this script in a QT preconfigured console"
echo;
echo "////// !!!!!! \\\\\\\\\\\\"
echo;
echo;

echo;
echo --------------------------------------------
echo ----- SCRIPT BEGIN FOR RELEASE VERSION -------
echo --------------------------------------------

set ALLDESTDIR=%~dp0\..\..\ComputreeInstallRelease

for /d %%X in (%~dp0\..\*) do (
	call:launch %%X %%~nX
)

for /d %%X in (%~dp0\..\..\plugin*) do (

	for /d %%Y in (%%X\*) do (
		call:launch %%Y %%~nY
	)
)

echo;
echo;
echo "////// !!!!!! \\\\\\\\\\\\"
echo;
echo "If you have problem with lupdate or lrelease (not found) please call this script in a QT preconfigured console"
echo;
echo "////// !!!!!! \\\\\\\\\\\\"
echo;
echo;

PAUSE

:launch

for %%f in (%~1\*.pro) do (

	findstr /c:"TRANSLATIONS" %%f >nul && (

		echo;
		echo Generate and copy translations of %%~nf
		echo;
		
		lupdate %%f >nul
		lrelease %%f >nul
		
		IF NOT EXIST %ALLDESTDIR%\languages mkdir %ALLDESTDIR%\languages 
		
		set /a nFiles=0
		
		for /d %%i in (%~1\*) do (			
			for %%j in (%%i\*.qm) do (
				echo %%~nxj
				copy %%j %ALLDESTDIR%\languages >nul
				set /a nFiles+=1
			)
		)
		
		for %%j in (%~1\*.qm) do (
			echo %%~nxj
			copy %%j %ALLDESTDIR%\languages >nul
			set /a nFiles+=1
		)
		
		echo;
		
		for /f "tokens=2 delims==" %%k in ('set nFiles') do (
			echo %%k files copied
		)
	)
)