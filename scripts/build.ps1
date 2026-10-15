<#
.SYNOPSIS
    Build and test runner for OrcEngine and TestProject demo.
.DESCRIPTION
    Wraps CMake preset configuration, compilation, and optional demo execution.
    Automatically detects and activates Visual Studio / Ninja / CMake toolchains
    on Windows if they are not already present in PATH.
.EXAMPLE
    .\scripts\build.ps1
    .\scripts\build.ps1 -Preset windows-release
    .\scripts\build.ps1 -Target OrcEngine
    .\scripts\build.ps1 -Clean
    .\scripts\build.ps1 -Run
    .\scripts\build.ps1 -Run -TimeoutSeconds 5
#>

[CmdletBinding()]
param(
    [string]$Preset = "windows-debug",
    [string]$Target,
    [switch]$Clean,
    [switch]$ConfigureOnly,
    [switch]$BuildOnly,
    [switch]$Run,
    [int]$TimeoutSeconds = 0,
    [string[]]$ExtraCMakeArgs = @()
)

$ErrorActionPreference = "Stop"

$repoRoot = Resolve-Path "$PSScriptRoot\.."
Set-Location $repoRoot

Write-Host "`n=== OrcEngine Build Runner ===" -ForegroundColor Cyan
Write-Host "Preset: $Preset" -ForegroundColor White

# 1. Toolchain Activation (Windows MSVC / Ninja / CMake)
$isWindows = [System.Environment]::OSVersion.Platform -match "Win"
if ($isWindows) {
    $hasCl = (Get-Command cl.exe -ErrorAction SilentlyContinue) -ne $null
    $hasCmake = (Get-Command cmake.exe -ErrorAction SilentlyContinue) -ne $null
    $hasNinja = (Get-Command ninja.exe -ErrorAction SilentlyContinue) -ne $null

    if (-not $hasCl -or -not $hasCmake -or -not $hasNinja) {
        Write-Host "Configuring build toolchain environment..." -ForegroundColor DarkGray
        
        $vsDevShell = Resolve-Path "$env:ProgramFiles\Microsoft Visual Studio\*\*\Common7\Tools\Launch-VsDevShell.ps1" -ErrorAction SilentlyContinue | Select-Object -First 1
        if (-not $vsDevShell) {
            $vsDevShell = Resolve-Path "${env:ProgramFiles(x86)}\Microsoft Visual Studio\*\*\Common7\Tools\Launch-VsDevShell.ps1" -ErrorAction SilentlyContinue | Select-Object -First 1
        }

        if ($vsDevShell) {
            Write-Host "Activating Visual Studio Developer Shell ($($vsDevShell.Path))..." -ForegroundColor DarkGray
            & $vsDevShell.Path -Arch amd64 | Out-Null
        }

        # Double check cmake
        if (-not (Get-Command cmake.exe -ErrorAction SilentlyContinue)) {
            $cmakeCandidates = @(
                "$env:ProgramFiles\Microsoft Visual Studio\*\*\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin",
                "$env:LOCALAPPDATA\Programs\CLion\bin\cmake\win\x64\bin"
            )
            foreach ($cand in $cmakeCandidates) {
                $found = Resolve-Path $cand -ErrorAction SilentlyContinue | Select-Object -First 1
                if ($found) {
                    $env:PATH = "$($found.Path);$env:PATH"
                    break
                }
            }
        }

        # Double check ninja
        if (-not (Get-Command ninja.exe -ErrorAction SilentlyContinue)) {
            $ninjaCandidates = @(
                "$env:ProgramFiles\Microsoft Visual Studio\*\*\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja",
                "$env:LOCALAPPDATA\Programs\CLion\bin\ninja\win\x64"
            )
            foreach ($cand in $ninjaCandidates) {
                $found = Resolve-Path $cand -ErrorAction SilentlyContinue | Select-Object -First 1
                if ($found) {
                    $env:PATH = "$($found.Path);$env:PATH"
                    break
                }
            }
        }
    }
}

# 2. Check Prerequisites
if (-not $env:VCPKG_ROOT) {
    Write-Warning "VCPKG_ROOT environment variable is not set. Configure may fail."
}
if (-not $env:FMOD_ROOT) {
    Write-Warning "FMOD_ROOT environment variable is not set. FMOD discovery may fail."
}

# 3. Clean if requested
$buildDir = Join-Path $repoRoot "build\$Preset"
if ($Clean) {
    if (Test-Path $buildDir) {
        Write-Host "Cleaning build directory: $buildDir" -ForegroundColor Yellow
        Remove-Item -Path $buildDir -Recurse -Force
    }
}

# 4. Configure Phase
$cmakeCache = Join-Path $buildDir "CMakeCache.txt"
$needsConfigure = -not (Test-Path $cmakeCache) -or $Clean

if (-not $BuildOnly -and ($needsConfigure -or -not (Test-Path $buildDir))) {
    Write-Host "`n>> Configuring preset '$Preset'..." -ForegroundColor Cyan
    $configArgs = @("--preset", $Preset) + $ExtraCMakeArgs
    Write-Host "cmake $($configArgs -join ' ')" -ForegroundColor DarkGray

    & cmake @configArgs
    if ($LASTEXITCODE -ne 0) {
        Write-Error "CMake configure failed with exit code $LASTEXITCODE"
    }
}

if ($ConfigureOnly) {
    Write-Host "`nConfigure completed successfully." -ForegroundColor Green
    exit 0
}

# 5. Build Phase
Write-Host "`n>> Building preset '$Preset'..." -ForegroundColor Cyan
$buildArgs = @("--build", "--preset", $Preset)
if ($Target) {
    $buildArgs += @("--target", $Target)
}
Write-Host "cmake $($buildArgs -join ' ')" -ForegroundColor DarkGray

& cmake @buildArgs
if ($LASTEXITCODE -ne 0) {
    Write-Error "CMake build failed with exit code $LASTEXITCODE"
}

Write-Host "`nBuild succeeded!" -ForegroundColor Green

# 6. Run Phase
if ($Run) {
    $exeName = if ($isWindows) { "TestProject.exe" } else { "TestProject" }
    $exePath = Join-Path $buildDir $exeName

    if (-not (Test-Path $exePath)) {
        Write-Error "Executable not found at '$exePath'. Did TestProject build successfully?"
    }

    Write-Host "`n>> Launching $exeName from $buildDir..." -ForegroundColor Cyan
    
    $procInfo = New-Object System.Diagnostics.ProcessStartInfo
    $procInfo.FileName = $exePath
    $procInfo.WorkingDirectory = $buildDir
    $procInfo.UseShellExecute = $false

    $proc = [System.Diagnostics.Process]::Start($procInfo)

    if ($TimeoutSeconds -gt 0) {
        Write-Host "Running with timeout of $TimeoutSeconds second(s)..." -ForegroundColor DarkGray
        $finished = $proc.WaitForExit($TimeoutSeconds * 1000)
        if (-not $finished) {
            Write-Host "Timeout reached ($TimeoutSeconds s). Closing process..." -ForegroundColor Yellow
            $proc.Kill()
            $proc.WaitForExit()
        }
    } else {
        $proc.WaitForExit()
    }

    Write-Host "Process exited with code: $($proc.ExitCode)" -ForegroundColor White

    # Check log file
    $logFile = Join-Path $buildDir "logs\TestProject.log"
    if (-not (Test-Path $logFile)) {
        $logFile = Join-Path $repoRoot "logs\TestProject.log"
    }

    if (Test-Path $logFile) {
        Write-Host "`nRecent entries from ${logFile}:" -ForegroundColor DarkGray
        Get-Content -Path $logFile -Tail 10 | ForEach-Object { Write-Host "  $_" -ForegroundColor Gray }
    }
}
