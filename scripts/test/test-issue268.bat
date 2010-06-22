@ECHO off
ECHO.
ECHO Test: fix for Issue 268
"../../bin/wkhtmltoimage" --scale-w 200 issue268.html out/issue268.png
ECHO.
PAUSE