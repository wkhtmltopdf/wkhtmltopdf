@ECHO off
ECHO.
ECHO Test: fix for Issue 267
"../../bin/wkhtmltoimage" --crop-w 640 --crop-h 480 issue267.html out/issue267.png
ECHO.
PAUSE