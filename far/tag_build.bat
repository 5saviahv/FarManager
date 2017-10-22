@echo off
setlocal

for /f "tokens=1,2,4 delims=," %%i in ('tools\m4 -P farversion.inc.m4') do (
	set major=%%i
	set minor=%%j
	set build=%%k
)

set tag=%major%.%minor%.%build%

echo --------------------------------------------------------------------
echo Continue only if you are sure that you have set the correct
echo build and commited the changes.
echo This command will tag the current state of the repository as:
echo builds/%tag%
echo --------------------------------------------------------------------
echo If you're not sure press CtrlC.
echo --------------------------------------------------------------------
echo --------------------------------------------------------------------
echo �த������ ⮫쪮 �᫨ �� 㢥७�, �� �� ���⠢��� �ࠢ����
echo ����� ����� � �������⨫� ���������.
echo �� ������� ������ ⥪�饥 ���ﭨ� ९������ ���:
echo builds/%tag%
echo --------------------------------------------------------------------
echo �᫨ �� �� 㢥७�, � ������ CtrlC
echo --------------------------------------------------------------------
pause
echo.

for /f "tokens=3" %%f in ('svn info 2^>nul ^| find "Root:"') do set repo=%%f
if [%repo%] == [] goto git

set tag_path=%repo%/tags/builds/%tag%

svn info %tag_path% > nul 2>&1 & (
	if not errorlevel 1 (
		echo Error: tag %tag% already exists
	) else (
		svn copy %repo%/trunk %tag_path% -m "tag build %build%"
	)
)
goto finish

:git
echo Sorry, git version is not yet implemented - you have to do it manually.
goto finish

:finish
endlocal
