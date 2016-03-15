echo off
del list1.txt >nul
del list.txt >nul
for /f "tokens=*" %%i in ('dir /b') do echo %%i >> list1.txt
findstr ".*\.jpg" list1.txt >> list.txt
findstr ".*\.jpeg" list1.txt >> list.txt
findstr ".*\.png" list1.txt >> list.txt
del list1.txt >nul
