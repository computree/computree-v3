@ECHO OFF

call win_updateLanguage.bat
call win_createInclude.bat

PAUSE

REM for %%Y in (%~1\scripts\*.bat) do (
	REM call %%Y
REM )	