Rem Set Build Envirement

set PLATFORM_NAME=MercuryPlat
set COMPILER=GCC
set COMPILER_DIR=%CD%

set CPP_COMPILER="%COMPILER_DIR%\prebuilts\win32\gcc-arm-none-eabi\bin\arm-none-eabi-gcc.exe"
set ASM_COMPILER="%COMPILER_DIR%\prebuilts\win32\gcc-arm-none-eabi\bin\arm-none-eabi-as.exe"
set LINKER="%COMPILER_DIR%\prebuilts\win32\gcc-arm-none-eabi\bin\arm-none-eabi-ld.exe"
set LIB_PACKER="%COMPILER_DIR%\prebuilts\win32\gcc-arm-none-eabi\bin\arm-none-eabi-ar.exe"
set OBJCOPY="%COMPILER_DIR%\prebuilts\win32\gcc-arm-none-eabi\bin\arm-none-eabi-objcopy.exe"
set LDPP="%COMPILER_DIR%\prebuilts\win32\gcc-arm-none-eabi\bin\arm-none-eabi-cpp.exe"
set OBJDUMP="%COMPILER_DIR%\prebuilts\win32\gcc-arm-none-eabi\bin\arm-none-eabi-objdump.exe"
set PYTHON="%COMPILER_DIR%\prebuilts\win32\python3\python3.exe"

set PATH=%PATH%;.\make\make_cmd

