@echo off
rem Abre o Xadrez 3D no Windows a partir da raiz do projeto,
rem onde estao shaders\, res\ e glfw3.dll.
cd /d "%~dp0"

if not exist "bin\Windows-Release\Xadrez3D.exe" (
    echo Executavel nao encontrado: bin\Windows-Release\Xadrez3D.exe
    echo.
    echo Abra Xadrez3D.cbp no Code::Blocks, escolha o target "Windows Release"
    echo na caixa "Build target" e use Build ^(Ctrl+F9^). Depois rode este arquivo de novo.
    pause
    exit /b 1
)

"bin\Windows-Release\Xadrez3D.exe"
if errorlevel 1 pause
