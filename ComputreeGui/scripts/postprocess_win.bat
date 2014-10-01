@ECHO OFF

cd > PWD
set /p PWD= < PWD

set DESTDIR=%PWD%\..\..\ComputreeInstall

echo "Copying img"

mkdir %DESTDIR%\img
copy img_to_copy\*.png %DESTDIR%\img

echo "Generating Translations"

for %%f in (*.pro) do (
	lrelease %%~nf.pro
)

mkdir %DESTDIR%\languages 
copy languages\*.qm %DESTDIR%\languages

cd %PWD%