@ECHO OFF

echo;
echo --------------------------
echo ----- SCRIPT BEGIN -------
echo --------------------------

set ALLDESTDIR=%~dp0\..\..\ComputreeInstall

for /d %%X in (%~dp0\..\*) do (
	call:launch %%X %%~nX
)

for /d %%X in (%~dp0\..\..\plugin*) do (

	for /d %%Y in (%%X\*) do (
		call:launch %%Y %%~nY
	)
)

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

IF EXIST %~1\shaders (

	echo;
	echo Copy shaders of %~2
	echo;
	
	IF NOT EXIST %ALLDESTDIR%\shaders mkdir %ALLDESTDIR%\shaders
	
	set /a nFiles=0
	
	for %%j in (%~1\shaders\*.*) do (
		echo %%~nxj
		copy %~1\shaders\*.* %ALLDESTDIR%\shaders >nul
		set /a nFiles+=1
	)
	
	echo;
		
	for /f "tokens=2 delims==" %%k in ('set nFiles') do (
		echo %%k files copied
	)
)

for %%Y in (%~1\scripts\*.bat) do (
	REM call %%Y
)	