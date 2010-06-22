@ECHO off
ECHO.
ECHO Test: Original size
"../../bin/wkhtmltoimage" http://google.com/ out/google-scale-orig.png
ECHO Test: scale to 300x200
"../../bin/wkhtmltoimage" --scale-w 300 --scale-h 200 http://google.com/ out/google-scale-300x200.png
ECHO Test: crop to 500x300 and scale to 300x200
"../../bin/wkhtmltoimage" --crop-w 500 --crop-h 300 --scale-w 300 --scale-h 200 http://google.com/ out/google-scale-500x300x300x200.png
ECHO.
PAUSE