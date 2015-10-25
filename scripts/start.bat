@echo off
cd /d %~dp0
set workdir=%~dp0
set PATH=%PATH%;C:\Windows\System32;C:\Windows\SysWOW64
set cecho=.\tools\cecho
set choice=Choice.exe

Title Imgdata tool v1.0.0 for Nexus 5X

ver | findstr /i "5\.0\." > nul
IF %ERRORLEVEL% EQU 0  set choice=.\tools\Choice4XP.exe
ver | findstr /i "5\.1\." > nul
IF %ERRORLEVEL% EQU 0 set choice=.\tools\Choice4XP.exe
ver | findstr /i "5\.2\." > nul
IF %ERRORLEVEL% EQU 0 set choice=.\tools\Choice4XP.exe
ver | findstr /i "6\.2\." > nul

:menu
cls
%cecho% {white}
echo ==========================================
%cecho% {blue on white}Imgdata extractor and builder for Nexus 5X{#}{\n}
echo ==========================================
echo.
%cecho% {fuchisa}Select an action{\n}----------------{#}{\n}
echo.  1 - Extract imgdata
echo.  2 - Preview
echo.  3 - Build imgdata
echo.
%cecho% {aqua}Type an action number, or X to exit{#}
%choice% /c 123X /n /m ""
if errorlevel 4 exit 0
if errorlevel 3 goto :build
if errorlevel 2 goto :preview
if errorlevel 1 goto :extract

:preview
cls
%cecho% {white}
echo ============
%cecho% {blue on white}Preview menu{#}{\n}
echo ============
echo.
%cecho% {fuchisa}Select an action{\n}----------------{#}{\n}
echo.  0 - View locked boot screen
echo.  1 - View unlocked boot screen
echo.  2 - View charger screen
echo.  3 - View fastboot screen with Start
echo.  4 - View fastboot screen with Restart Bootloader
echo.  5 - View fastboot screeenwith Recovery mode
echo.  6 - View fastboot screen with Power Off
echo.  7 - View oem unlock screen with no
echo.  8 - View oem unlock screen with yes
echo.  9 - View download mode screen
echo.  A - View oem lock screen with no
echo.  B - View oem lock screen with yes
echo.  C - View yellow press power to continue screen
echo.  D - View yellow press power to pause boot screen
echo.  E - View orange press power to continue screen
echo.  F - view orange press power to pause boot screen
echo.  G - View red press power to continue screen
echo.  H - View red press power to pause boot screen
echo.  I - View all
echo.
%cecho% {aqua}Type an action number, or X to exit{#}
%choice% /c 0123456789ABCDEFGHIX /n /m ""
if errorlevel 20 goto :menu
if errorlevel 19 goto :all
if errorlevel 18 goto :red-sleep
if errorlevel 17 goto :red-power
if errorlevel 16 goto :orange-sleep
if errorlevel 15 goto :orange-power
if errorlevel 14 goto :yellow-sleep
if errorlevel 13 goto :yellow-power
if errorlevel 12 goto :oem-lock-no
if errorlevel 11 goto :oem-lock-yes
if errorlevel 10 goto :downloadmode
if errorlevel 9 goto :oem-unlock-no
if errorlevel 8 goto :oem-unlock-yes
if errorlevel 7 goto :fastboot-poweroff
if errorlevel 6 goto :fastboot-recovery
if errorlevel 5 goto :fastboot-bootloader
if errorlevel 4 goto :fastboot-start
if errorlevel 3 goto :charger
if errorlevel 2 goto :unlocked-boot
if errorlevel 1 goto :locked-boot

:all
cls
set option=all
del .\previews\* /q
.\tools\imgdata.exe preview %option% .\images\ .\previews\
set error=%errorlevel%
if %error% NEQ 0 goto :allerr
explorer.exe .\previews\
echo.
%cecho% {white}Again? (y/n):{#}
%choice% /c YN /n /m ""
if errorlevel 2 goto :menu
if errorlevel 1 goto :all

:allerr
%cecho% {red}{\n}Something went wrong. Check the lines above.{#}{\n}
%cecho% {white}Try again? (y/n):{#}
%choice% /c YN /n /m ""
if errorlevel 2 goto :menu
if errorlevel 1 goto :all

:red-sleep
cls
set option=red-sleep
.\tools\imgdata.exe preview %option% .\images\ .\previews\
set error=%errorlevel%
if %error% NEQ 0 goto :red-sleeperr
.\previews\%option%.bmp
echo.
%cecho% {white}Again? (y/n):{#}
%choice% /c YN /n /m ""
if errorlevel 2 goto :menu
if errorlevel 1 goto :red-sleep

:red-sleeperr
%cecho% {red}{\n}Something went wrong. Check the lines above.{#}{\n}
%cecho% {white}Try again? (y/n):{#}
%choice% /c YN /n /m ""
if errorlevel 2 goto :menu
if errorlevel 1 goto :red-sleep

:red-power
cls
set option=red-power
.\tools\imgdata.exe preview %option% .\images\ .\previews\
set error=%errorlevel%
if %error% NEQ 0 goto :red-powererr
.\previews\%option%.bmp
echo.
%cecho% {white}Again? (y/n):{#}
%choice% /c YN /n /m ""
if errorlevel 2 goto :menu
if errorlevel 1 goto :red-power

:red-powererr
%cecho% {red}{\n}Something went wrong. Check the lines above.{#}{\n}
%cecho% {white}Try again? (y/n):{#}
%choice% /c YN /n /m ""
if errorlevel 2 goto :menu
if errorlevel 1 goto :red-power

:orange-sleep
cls
set option=orange-sleep
.\tools\imgdata.exe preview %option% .\images\ .\previews\
set error=%errorlevel%
if %error% NEQ 0 goto :orange-sleeperr
.\previews\%option%.bmp
echo.
%cecho% {white}Again? (y/n):{#}
%choice% /c YN /n /m ""
if errorlevel 2 goto :menu
if errorlevel 1 goto :orange-sleep

:orange-sleeperr
%cecho% {red}{\n}Something went wrong. Check the lines above.{#}{\n}
%cecho% {white}Try again? (y/n):{#}
%choice% /c YN /n /m ""
if errorlevel 2 goto :menu
if errorlevel 1 goto :orange-sleep

:orange-power
cls
set option=orange-power
.\tools\imgdata.exe preview %option% .\images\ .\previews\
set error=%errorlevel%
if %error% NEQ 0 goto :orange-powererr
.\previews\%option%.bmp
echo.
%cecho% {white}Again? (y/n):{#}
%choice% /c YN /n /m ""
if errorlevel 2 goto :menu
if errorlevel 1 goto :orange-power

:orange-powererr
%cecho% {red}{\n}Something went wrong. Check the lines above.{#}{\n}
%cecho% {white}Try again? (y/n):{#}
%choice% /c YN /n /m ""
if errorlevel 2 goto :menu
if errorlevel 1 goto :orange-power

:yellow-sleep
cls
set option=yellow-sleep
.\tools\imgdata.exe preview %option% .\images\ .\previews\
set error=%errorlevel%
if %error% NEQ 0 goto :yellow-sleeperr
.\previews\%option%.bmp
echo.
%cecho% {white}Again? (y/n):{#}
%choice% /c YN /n /m ""
if errorlevel 2 goto :menu
if errorlevel 1 goto :yellow-sleep

:yellow-sleeperr
%cecho% {red}{\n}Something went wrong. Check the lines above.{#}{\n}
%cecho% {white}Try again? (y/n):{#}
%choice% /c YN /n /m ""
if errorlevel 2 goto :menu
if errorlevel 1 goto :yellow-sleep

:yellow-power
cls
set option=yellow-power
.\tools\imgdata.exe preview %option% .\images\ .\previews\
set error=%errorlevel%
if %error% NEQ 0 goto :yellow-powererr
.\previews\%option%.bmp
echo.
%cecho% {white}Again? (y/n):{#}
%choice% /c YN /n /m ""
if errorlevel 2 goto :menu
if errorlevel 1 goto :yellow-power

:yellow-powererr
%cecho% {red}{\n}Something went wrong. Check the lines above.{#}{\n}
%cecho% {white}Try again? (y/n):{#}
%choice% /c YN /n /m ""
if errorlevel 2 goto :menu
if errorlevel 1 goto :yellow-power

:oem-lock-no
cls
set option=oem-lock-no
.\tools\imgdata.exe preview %option% .\images\ .\previews\
set error=%errorlevel%
if %error% NEQ 0 goto :oem-lock-noerr
.\previews\%option%.bmp
echo.
%cecho% {white}Again? (y/n):{#}
%choice% /c YN /n /m ""
if errorlevel 2 goto :menu
if errorlevel 1 goto :oem-lock-no

:oem-lock-noerr
%cecho% {red}{\n}Something went wrong. Check the lines above.{#}{\n}
%cecho% {white}Try again? (y/n):{#}
%choice% /c YN /n /m ""
if errorlevel 2 goto :menu
if errorlevel 1 goto :oem-lock-no

:oem-lock-yes
cls
set option=oem-lock-yes
.\tools\imgdata.exe preview %option% .\images\ .\previews\
set error=%errorlevel%
if %error% NEQ 0 goto :oem-lock-yeserr
.\previews\%option%.bmp
echo.
%cecho% {white}Again? (y/n):{#}
%choice% /c YN /n /m ""
if errorlevel 2 goto :menu
if errorlevel 1 goto :oem-lock-yes

:oem-lock-yeserr
%cecho% {red}{\n}Something went wrong. Check the lines above.{#}{\n}
%cecho% {white}Try again? (y/n):{#}
%choice% /c YN /n /m ""
if errorlevel 2 goto :menu
if errorlevel 1 goto :oem-lock-yes

:downloadmode
cls
set option=downloadmode
.\tools\imgdata.exe preview %option% .\images\ .\previews\
set error=%errorlevel%
if %error% NEQ 0 goto :downloadmodeerr
.\previews\%option%.bmp
echo.
%cecho% {white}Again? (y/n):{#}
%choice% /c YN /n /m ""
if errorlevel 2 goto :menu
if errorlevel 1 goto :downloadmode

:downloadmodeerr
%cecho% {red}{\n}Something went wrong. Check the lines above.{#}{\n}
%cecho% {white}Try again? (y/n):{#}
%choice% /c YN /n /m ""
if errorlevel 2 goto :menu
if errorlevel 1 goto :downloadmode

:oem-unlock-yes
cls
set option=oem-unlock-yes
.\tools\imgdata.exe preview %option% .\images\ .\previews\
set error=%errorlevel%
if %error% NEQ 0 goto :oem-unlock-yeserr
.\previews\%option%.bmp
echo.
%cecho% {white}Again? (y/n):{#}
%choice% /c YN /n /m ""
if errorlevel 2 goto :menu
if errorlevel 1 goto :oem-unlock-yes

:oem-unlock-yeserr
%cecho% {red}{\n}Something went wrong. Check the lines above.{#}{\n}
%cecho% {white}Try again? (y/n):{#}
%choice% /c YN /n /m ""
if errorlevel 2 goto :menu
if errorlevel 1 goto :oem-unlock-yes

:oem-unlock-no
cls
set option=oem-unlock-no
.\tools\imgdata.exe preview %option% .\images\ .\previews\
set error=%errorlevel%
if %error% NEQ 0 goto :oem-unlock-noerr
.\previews\%option%.bmp
echo.
%cecho% {white}Again? (y/n):{#}
%choice% /c YN /n /m ""
if errorlevel 2 goto :menu
if errorlevel 1 goto :oem-unlock-no

:oem-unlock-noerr
%cecho% {red}{\n}Something went wrong. Check the lines above.{#}{\n}
%cecho% {white}Try again? (y/n):{#}
%choice% /c YN /n /m ""
if errorlevel 2 goto :menu
if errorlevel 1 goto :oem-unlock-no

:fastboot-poweroff
cls
set option=fastboot-poweroff
.\tools\imgdata.exe preview %option% .\images\ .\previews\
set error=%errorlevel%
if %error% NEQ 0 goto :fastboot-powerofferr
.\previews\%option%.bmp
echo.
%cecho% {white}Again? (y/n):{#}
%choice% /c YN /n /m ""
if errorlevel 2 goto :menu
if errorlevel 1 goto :fastboot-poweroff

:fastboot-powerofferr
%cecho% {red}{\n}Something went wrong. Check the lines above.{#}{\n}
%cecho% {white}Try again? (y/n):{#}
%choice% /c YN /n /m ""
if errorlevel 2 goto :menu
if errorlevel 1 goto :fastboot-poweroff

:fastboot-recovery
cls
set option=fastboot-recovery
.\tools\imgdata.exe preview %option% .\images\ .\previews\
set error=%errorlevel%
if %error% NEQ 0 goto :fastboot-recoveryerr
.\previews\%option%.bmp
echo.
%cecho% {white}Again? (y/n):{#}
%choice% /c YN /n /m ""
if errorlevel 2 goto :menu
if errorlevel 1 goto :fastboot-recovery

:fastboot-recoveryerr
%cecho% {red}{\n}Something went wrong. Check the lines above.{#}{\n}
%cecho% {white}Try again? (y/n):{#}
%choice% /c YN /n /m ""
if errorlevel 2 goto :menu
if errorlevel 1 goto :fastboot-recovery

:fastboot-bootloader
cls
set option=fastboot-bootloader
.\tools\imgdata.exe preview %option% .\images\ .\previews\
set error=%errorlevel%
if %error% NEQ 0 goto :fastboot-bootloadererr
.\previews\%option%.bmp
echo.
%cecho% {white}Again? (y/n):{#}
%choice% /c YN /n /m ""
if errorlevel 2 goto :menu
if errorlevel 1 goto :fastboot-bootloader

:fastboot-bootloadererr
%cecho% {red}{\n}Something went wrong. Check the lines above.{#}{\n}
%cecho% {white}Try again? (y/n):{#}
%choice% /c YN /n /m ""
if errorlevel 2 goto :menu
if errorlevel 1 goto :fastboot-bootloader

:fastboot-start
cls
set option=fastboot-start
.\tools\imgdata.exe preview %option% .\images\ .\previews\
set error=%errorlevel%
if %error% NEQ 0 goto :fastboot-starterr
.\previews\%option%.bmp
echo.
%cecho% {white}Again? (y/n):{#}
%choice% /c YN /n /m ""
if errorlevel 2 goto :menu
if errorlevel 1 goto :fastboot-start

:fastboot-starterr
%cecho% {red}{\n}Something went wrong. Check the lines above.{#}{\n}
%cecho% {white}Try again? (y/n):{#}
%choice% /c YN /n /m ""
if errorlevel 2 goto :menu
if errorlevel 1 goto :fastboot-start

:charger
cls
set option=charger
.\tools\imgdata.exe preview %option% .\images\ .\previews\
set error=%errorlevel%
if %error% NEQ 0 goto :chargererr
.\previews\%option%.bmp
echo.
%cecho% {white}Again? (y/n):{#}
%choice% /c YN /n /m ""
if errorlevel 2 goto :menu
if errorlevel 1 goto :charger

:chargererr
%cecho% {red}{\n}Something went wrong. Check the lines above.{#}{\n}
%cecho% {white}Try again? (y/n):{#}
%choice% /c YN /n /m ""
if errorlevel 2 goto :menu
if errorlevel 1 goto :charger

:unlocked-boot
cls
set option=unlocked-boot
.\tools\imgdata.exe preview %option% .\images\ .\previews\
set error=%errorlevel%
if %error% NEQ 0 goto :unlocked-booterr
.\previews\%option%.bmp
echo.
%cecho% {white}Again? (y/n):{#}
%choice% /c YN /n /m ""
if errorlevel 2 goto :menu
if errorlevel 1 goto :unlocked-boot

:unlocked-booterr
%cecho% {red}{\n}Something went wrong. Check the lines above.{#}{\n}
%cecho% {white}Try again? (y/n):{#}
%choice% /c YN /n /m ""
if errorlevel 2 goto :menu
if errorlevel 1 goto :unlocked-boot

:locked-boot
cls
set option=locked-boot
.\tools\imgdata.exe preview %option% .\images\ .\previews\
set error=%errorlevel%
if %error% NEQ 0 goto :locked-booterr
.\previews\%option%.bmp
echo.
%cecho% {white}Again? (y/n):{#}
%choice% /c YN /n /m ""
if errorlevel 2 goto :menu
if errorlevel 1 goto :locked-boot

:locked-booterr
%cecho% {red}{\n}Something went wrong. Check the lines above.{#}{\n}
%cecho% {white}Try again? (y/n):{#}
%choice% /c YN /n /m ""
if errorlevel 2 goto :menu
if errorlevel 1 goto :locked-boot

:extract
cls
%cecho% {aqua}Drag 'n Drop here the imgdata file or give its path and press [enter].{\n}Press x and [enter] to cancel: {white}
set /P file="")
if not defined file goto :extract
if /I %file%==x set x=1&goto :menu
echo.
del .\images\* /q
.\tools\imgdata.exe extract %file% .\images\
set error=%errorlevel%
if %error% NEQ 0 goto :eerr
echo.
%cecho% {lime}%file% is extracted.{#}
explorer.exe .\images\
pause > nul 
goto :menu

:eerr
%cecho% {red}{\n}Something went wrong. Check the lines above.{#}{\n}
%cecho% {white}Try again? (y/n):{#}
%choice% /c YN /n /m ""
if errorlevel 2 goto :menu
if errorlevel 1 goto :extract

:build
cls
:from16bit
.\tools\imgdata.exe build .\images\ .\custom\
set error=%errorlevel%
if %error% EQU 1 goto :bigfile
if %error% NEQ 0 goto :berr
echo.
copy .\tools\dummy.zip .\flash-me.zip /Y
echo.
.\tools\7za.exe a flash-me.zip .\custom\new-imgdata.img
echo.
%cecho% {lime}Building finished. Flash 
%cecho% {white}flash-me.zip 
%cecho% {lime}in the recovery.{#}
echo.
%cecho% {white}{\n}Do you want to flash it? (y/n): {#}
%choice% /c YN /n /m ""
if errorlevel 2 goto :menu
if errorlevel 1 goto :flash

:bigfile
echo.
%cecho% {red}{\n}Something went wrong. The new imgdata is too big{\n}
%cecho% {red}Resize the images or use the built in 16 bit conversion{#}{\n}
%cecho% {white}{\n}Do you want to start the 16 bit conversion? (y/n):
%choice% /c YN /n /m ""
if errorlevel 2 goto :menu
if errorlevel 1 goto :16bit

:berr
%cecho% {red}{\n}Something went wrong. Check the lines above.{#}{\n}
%cecho% {white}Try again? (y/n):{#}
%choice% /c YN /n /m ""
if errorlevel 2 goto :menu
if errorlevel 1 goto :build

:flash
cls
%cecho% {white}
echo ==========
%cecho% {blue on white}Flash menu{#}{\n}
echo ==========
echo.
%cecho% {fuchisa}Select an action{\n}----------------{#}{\n}
echo.  1 - Flash with adb sideload
echo.  2 - Flash with fastboot
echo.
%cecho% {aqua}Type an action number, or X to exit{#}
%choice% /c 12X /n /m ""
if errorlevel 3 goto :menu
if errorlevel 2 goto :fastboot
if errorlevel 1 goto :adb
:fastboot
cls
%cecho% {white}Restart your phone to fastboot mode then press enter.
pause > nul
%cecho% {\n}
echo.
.\tools\fastboot.exe flash imgdata .\custom\new-imgdata.img
set error=%errorlevel%
if %error% NEQ 0 goto :fastbooterr
%cecho% {\n}
%cecho% {lime}New-imgdata.img flashed to the device succesfuly{#}
pause > nul
goto :menu
:fastbooterr
%cecho% {red}{\n}Something went wrong. Check the lines above.{#}{\n}
%cecho% {white}Try again? (y/n):{#}
%choice% /c YN /n /m ""
if errorlevel 2 goto :menu
if errorlevel 1 goto :fastboot

:adb
cls
%cecho% {white}Restart your phone to recovery, enable adb sideload then press enter.
pause > nul
%cecho% {\n}
echo.
.\tools\adb.exe sideload flash-me.zip
set error=%errorlevel%
if %error% NEQ 0 goto :adberr
%cecho% {\n}
%cecho% {lime}Flash-me.zip sent to the device succesfuly.{#}
pause > nul
goto :menu

:adberr
%cecho% {red}{\n}Something went wrong. Check the lines above.{#}{\n}
%cecho% {white}Try again? (y/n):{#}
%choice% /c YN /n /m ""
if errorlevel 2 goto :menu
if errorlevel 1 goto :adb

:16bit
cls
%cecho% {white}
echo =================
%cecho% {blue on white}16 bit conversion{#}{\n}
echo =================
echo.
%cecho% {fuchisa}Select an action{\n}----------------{#}{\n}
echo.  0 - Convert boot.bmp
echo.  1 - Convert charger.bmp
echo.  2 - Convert unlocked.bmp
echo.  3 - Convert start.bmp
echo.  4 - Convert bootloader.bmp
echo.  5 - Convert recovery.bmp
echo.  6 - Convert poweroff.bmp
echo.  7 - Convert fastboot_op.bmp
echo.  8 - Convert oem_unlock.bmp
echo.  9 - Convert unlock_yes.bmp
echo.  A - Convert unlock_no.bmp
echo.  B - Convert downloadmode.bmp
echo.  C - Convert oem_lock.bmp
echo.  D - Convert lock_yes.bmp
echo.  E - Convert lock_no.bmp
echo.  F - Convert locked.bmp
echo.  G - Convert yellow_power.bmp
echo.  H - Convert yellow_sleep.bmp
echo.  I - Convert orange_power.bmp
echo.  J - Convert orange_sleep.bmp
echo.  K - Convert red_power.bmp
echo.  L - Convert red_sleep.bmp
echo.
%cecho% {aqua}Type an action number, or X to exit{#}
%choice% /c 0123456789ABCDEFGHIJKLX /n /m ""
set error=%errorlevel%
if %error% EQU 23 exit 0
if %error% EQU 1 set imgchoice=.\images\boot.bmp
if %error% EQU 2 set imgchoice=.\images\charger.bmp
if %error% EQU 3 set imgchoice=.\images\unlocked.bmp
if %error% EQU 4 set imgchoice=.\images\start.bmp
if %error% EQU 5 set imgchoice=.\images\bootloader.bmp
if %error% EQU 6 set imgchoice=.\images\recovery.bmp
if %error% EQU 7 set imgchoice=.\images\poweroff.bmp
if %error% EQU 8 set imgchoice=.\images\fastboot_op.bmp
if %error% EQU 9 set imgchoice=.\images\oem_unlock.bmp
if %error% EQU 10 set imgchoice=.\images\unlock-yes.bmp
if %error% EQU 11 set imgchoice=.\images\unlock-no.bmp
if %error% EQU 12 set imgchoice=.\images\downloadmode.bmp
if %error% EQU 13 set imgchoice=.\images\oem_lock.bmp
if %error% EQU 14 set imgchoice=.\images\lock_yes.bmp
if %error% EQU 15 set imgchoice=.\images\lock_no.bmp
if %error% EQU 16 set imgchoice=.\images\locked.bmp
if %error% EQU 17 set imgchoice=.\images\yellow_power.bmp
if %error% EQU 18 set imgchoice=.\images\yellow_sleep.bmp
if %error% EQU 19 set imgchoice=.\images\orange_power.bmp
if %error% EQU 20 set imgchoice=.\images\orange_sleep.bmp
if %error% EQU 21 set imgchoice=.\images\red_power.bmp
if %error% EQU 22 set imgchoice=.\images\red_sleep.bmp
cls
.\tools\imgdata.exe 16bit %imgchoice%
set error=%errorlevel%
if %error% NEQ 0 goto :16biter
%cecho% {\n}
%cecho% {lime}{\n}%imgchoice% converted to 16 bit{#}
%cecho% {\n}
echo.
goto :from16bit

:16biter
echo.
%cecho% {red}{\n}Something went wrong. Check the lines above.{#}{\n}
%cecho% {white}Try again? (y/n):{#}
%choice% /c YN /n /m ""
if errorlevel 2 goto :menu
if errorlevel 1 goto :16bit
exit 0
