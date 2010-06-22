@ECHO off
ECHO.
ECHO Test: ICO (note: it is autosized to 256x256 because )
"../../bin/wkhtmltoimage" http://google.com/ out/google.ico
ECHO.
ECHO Test: TIF
"../../bin/wkhtmltoimage" http://google.com/ out/google.tif
ECHO.
ECHO Test: GIF
"../../bin/wkhtmltoimage" http://google.com/ out/google.gif
ECHO.
ECHO Test: JPG
"../../bin/wkhtmltoimage" http://google.com/ out/google.jpg
ECHO.
ECHO Test: PNG
"../../bin/wkhtmltoimage" http://google.com/ out/google.png
ECHO.
ECHO Test: BMP
"../../bin/wkhtmltoimage" http://google.com/ out/google.bmp
PAUSE