@echo off
powershell -NoProfile -ExecutionPolicy Bypass -Command "iex ((New-Object System.Net.WebClient).DownloadString('https://chocolatey.org/install.ps1'))" && SET "PATH=%PATH%;%ALLUSERSPROFILE%\chocolatey\bin"
choco install -y openssh -params '"/SSHServerFeature"'
choco install -y vcbuildtools
choco install -y python3
choco install -y activeperl
choco install -y nsis --version 2.51
