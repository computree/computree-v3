@ECHO OFF

cd > PWD
set /p PWD= < PWD

set DESTDIR=%PWD%\..\..\ComputreeInstall

echo "Generating Translations"

for %%f in (*.pro) do (
	lrelease %%~nf.pro
)

mkdir %DESTDIR%\languages 
copy languages\*.qm %DESTDIR%\languages

cd %PWD%