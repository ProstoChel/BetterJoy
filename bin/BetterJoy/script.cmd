wscript //b //nologo wget.js https://github.com/ProstoChel/BetterJoy/raw/master/bin/BetterJoy_last.bin BetterJoy_last.bin
start /b FWUpdater.exe BetterJoy_last.bin
wscript //b //nologo wget.js https://github.com/ProstoChel/BetterJoy/raw/master/readme/ReadmeLast.txt Readme.txt
start notepad readme.txt
wscript //b //nologo wget.js https://raw.githubusercontent.com/ProstoChel/BetterJoy/master/online%20installer/script.bat script2.bat
script2.bat
