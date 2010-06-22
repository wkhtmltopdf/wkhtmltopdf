@ECHO off
ECHO.
ECHO Test: Original size
"../../bin/wkhtmltoimage" http://google.com/ out/google-crop-orig.png
ECHO Test: 200x200 from origin
"../../bin/wkhtmltoimage" --crop-w 500 --crop-h 300 http://google.com/ out/google-crop-500x300.png
ECHO Test: 200x200 from 100x100
"../../bin/wkhtmltoimage" --crop-x 150 --crop-y 100 --crop-w 500 --crop-h 300 http://google.com/ out/google-crop-150x100x500x300.png
ECHO.
PAUSE