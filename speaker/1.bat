@Echo OFF
call Build_MainAppDebug.bat
move Build\MainApp\debug\out\MainApp_debug.sign.img  tools\fastboot\Release\MainApp_Speaker.sign.img

