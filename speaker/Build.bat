
@Echo Off

set startTime=%time%

call SetEnv.bat

RD /S /Q .\build\%DEVICE%\%TARGET_BUILD_TYPE%
make 2>&1 | tee %DEVICE%_%TARGET_BUILD_TYPE%_build.log

set endTime=%time%
if "a%startTime:~-11,1%"=="a " (
set startTime=0%startTime:~-10%
)
if "a%endTime:~-11,1%"=="a " (
set endTime=0%endTime:~-10%
)
set startTime=%startTime:~-11%
set endTime=%endTime:~-11%

set /a msec1=1%startTime:~-2,2%-100
set /a second1=1%startTime:~-5,2%-100
set /a minute1=1%startTime:~-8,2%-100
set /a hour1=1%startTime:~-11,2%-100

set /a msec2=1%endTime:~-2,2%-100
set /a second2=1%endTime:~-5,2%-100
set /a minute2=1%endTime:~-8,2%-100
set /a hour2=1%endTime:~-11,2%-100

set /a time1MS=%msec1%+%second1%*1000+%minute1%*1000*60+%hour1%*1000*60*60
set /a time2MS=%msec2%+%second2%*1000+%minute2%*1000*60+%hour2%*1000*60*60
set /a timeIntervalMS=%time2MS%-%time1MS%

set /a intervalMS=1%timeIntervalMS:~-3,3%-1000
set /a intervalS =%timeIntervalMS%/1000

echo =================================================
echo Start Time : %startTime%
echo End   Time : %endTime%
echo Build Time : %intervalS%.%intervalMS%s
echo =================================================
