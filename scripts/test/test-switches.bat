@ECHO off
ECHO.
ECHO Test: Show version
"../../bin/wkhtmltoimage" --version
ECHO.
ECHO Test: Show help
"../../bin/wkhtmltoimage" --help
ECHO.
ECHO Test: Show more help
"../../bin/wkhtmltoimage" --extended-help
ECHO.
ECHO Test: Show readme
"../../bin/wkhtmltoimage" --readme
PAUSE