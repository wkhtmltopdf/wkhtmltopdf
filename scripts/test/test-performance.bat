@ECHO off
ECHO.
ECHO Test: Performance/load testing.
ECHO Run through cvpldbg
SET iter=0
:loop
ECHO Iteration #%iter%
"../../bin/wkhtmltoimage" http://google.com/ tmp/google%iter%.png
SET /A iter+=1
GOTO :loop