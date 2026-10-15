<#
.SYNOPSIS
    Checks the development and agent environment for OrcEngine.
.DESCRIPTION
    Verifies that CMake, Ninja, a C++20 compiler, VCPKG_ROOT, FMOD_ROOT, and Git
    are properly installed and configured as required by AGENTS.md.
.EXAMPLE
    .\scripts\check-env.ps1
#>

[CmdletBinding()]
param(
    [switch]$Detailed
)

$ErrorActionPreference = "Continue"

$repoRoot = Resolve-Path "$PSScriptRoot\.."
Set-Location $repoRoot

$allOk = $true
$warnings = 0

function Write-CheckResult {
    param(
        [string]$Name,
        [string]$Status, # "PASS", "WARN", "FAIL"
        [string]$Message,
        [string]$Remediation = ""
    )
    switch ($Status) {
        "PASS" {
            Write-Host "  [ PASS ] " -ForegroundColor Green -NoNewline
            Write-Host "$Name`: " -ForegroundColor White -NoNewline
            Write-Host $Message -ForegroundColor Gray
        }
        "WARN" {
            Write-Host "  [ WARN ] " -ForegroundColor Yellow -NoNewline
            Write-Host "$Name`: " -ForegroundColor White -NoNewline
            Write-Host $Message -ForegroundColor Yellow
            if ($Remediation) {
                Write-Host "           Remediation: $Remediation" -ForegroundColor DarkYellow
            }
            $script:warnings++
        }
        "FAIL" {
            Write-Host "  [ FAIL ] " -ForegroundColor Red -NoNewline
            Write-Host "$Name`: " -ForegroundColor White -NoNewline
            Write-Host $Message -ForegroundColor Red
            if ($Remediation) {
                Write-Host "           Remediation: $Remediation" -ForegroundColor DarkRed
            }
            $script:allOk = $false
        }
    }
}

Write-Host "`n=== OrcEngine Agent Environment Diagnostic ===" -ForegroundColor Cyan
Write-Host "Repository root: $repoRoot`n" -ForegroundColor DarkGray

# 1. CMake Check
$cmakeCmd = Get-Command cmake.exe -ErrorAction SilentlyContinue
$cmakePath = if ($cmakeCmd) { $cmakeCmd.Source } else { $null }

if (-not $cmakePath) {
    # Search common locations
    $candidates = @(
        "$env:ProgramFiles\Microsoft Visual Studio\*\*\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe",
        "${env:ProgramFiles(x86)}\Microsoft Visual Studio\*\*\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe",
        "$env:LOCALAPPDATA\Programs\CLion\bin\cmake\win\x64\bin\cmake.exe"
    )
    foreach ($cand in $candidates) {
        $found = Resolve-Path $cand -ErrorAction SilentlyContinue | Select-Object -First 1
        if ($found) {
            $cmakePath = $found.Path
            break
        }
    }
}

if ($cmakePath) {
    $cmakeVerOutput = & $cmakePath --version 2>&1 | Select-Object -First 1
    if ($cmakeVerOutput -match "version (\d+\.\d+\.\d+)") {
        $ver = [version]$matches[1]
        $minVer = [version]"3.25.0"
        if ($ver -ge $minVer) {
            Write-CheckResult "CMake" "PASS" "$cmakeVerOutput ($cmakePath)"
        } else {
            Write-CheckResult "CMake" "FAIL" "Found version $ver at $cmakePath, but >= 3.25.0 required." "Upgrade CMake or install Visual Studio CMake tools."
        }
    } else {
        Write-CheckResult "CMake" "PASS" "$cmakeVerOutput ($cmakePath)"
    }
} else {
    Write-CheckResult "CMake" "FAIL" "cmake.exe not found in PATH or standard Visual Studio/CLion paths." "Install CMake >= 3.25 or install the C++ CMake tools in Visual Studio."
}

# 2. Ninja Check
$ninjaCmd = Get-Command ninja.exe -ErrorAction SilentlyContinue
$ninjaPath = if ($ninjaCmd) { $ninjaCmd.Source } else { $null }

if (-not $ninjaPath) {
    $candidates = @(
        "$env:ProgramFiles\Microsoft Visual Studio\*\*\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe",
        "${env:ProgramFiles(x86)}\Microsoft Visual Studio\*\*\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe",
        "$env:LOCALAPPDATA\Programs\CLion\bin\ninja\win\x64\ninja.exe"
    )
    foreach ($cand in $candidates) {
        $found = Resolve-Path $cand -ErrorAction SilentlyContinue | Select-Object -First 1
        if ($found) {
            $ninjaPath = $found.Path
            break
        }
    }
}

if ($ninjaPath) {
    $ninjaVer = (& $ninjaPath --version 2>&1).Trim()
    Write-CheckResult "Ninja" "PASS" "Version $ninjaVer ($ninjaPath)"
} else {
    Write-CheckResult "Ninja" "FAIL" "ninja.exe not found in PATH or Visual Studio/CLion directories." "Install Ninja or Visual Studio C++ CMake tools."
}

# 3. Compiler Check
$clCmd = Get-Command cl.exe -ErrorAction SilentlyContinue
$gccCmd = Get-Command g++.exe -ErrorAction SilentlyContinue
$clangCmd = Get-Command clang++.exe -ErrorAction SilentlyContinue

$vsDevShell = Resolve-Path "$env:ProgramFiles\Microsoft Visual Studio\*\*\Common7\Tools\Launch-VsDevShell.ps1" -ErrorAction SilentlyContinue | Select-Object -First 1

if ($clCmd) {
    $clVer = (& cl.exe 2>&1 | Select-Object -First 1).Trim()
    Write-CheckResult "Compiler" "PASS" "MSVC detected in PATH ($clVer)"
} elseif ($vsDevShell) {
    Write-CheckResult "Compiler" "PASS" "Visual Studio DevShell available at $($vsDevShell.Path)"
} elseif ($clangCmd) {
    $clangVer = (& clang++.exe --version 2>&1 | Select-Object -First 1).Trim()
    Write-CheckResult "Compiler" "PASS" "Clang detected ($clangVer)"
} elseif ($gccCmd) {
    $gccVer = (& g++.exe --version 2>&1 | Select-Object -First 1).Trim()
    Write-CheckResult "Compiler" "PASS" "GCC detected ($gccVer)"
} else {
    Write-CheckResult "Compiler" "FAIL" "No C++20 compiler (MSVC / Clang / GCC) found in PATH or Visual Studio install." "Install Visual Studio C++ desktop development tools or MinGW-w64."
}

# 4. VCPKG_ROOT Check
$vcpkgRoot = $env:VCPKG_ROOT
if ($vcpkgRoot) {
    $toolchainFile = Join-Path $vcpkgRoot "scripts\buildsystems\vcpkg.cmake"
    if (Test-Path $toolchainFile) {
        Write-CheckResult "VCPKG_ROOT" "PASS" "$vcpkgRoot (toolchain verified: $toolchainFile)"
    } else {
        Write-CheckResult "VCPKG_ROOT" "FAIL" "VCPKG_ROOT is set to '$vcpkgRoot', but scripts\buildsystems\vcpkg.cmake was not found." "Check VCPKG_ROOT path or clone vcpkg."
    }
} else {
    Write-CheckResult "VCPKG_ROOT" "FAIL" "Environment variable VCPKG_ROOT is not set." "Set `$env:VCPKG_ROOT to your vcpkg checkout directory."
}

# 5. FMOD_ROOT Check
$fmodRoot = $env:FMOD_ROOT
if ($fmodRoot) {
    $coreInc = Join-Path $fmodRoot "api\core\inc\fmod.hpp"
    $studioInc = Join-Path $fmodRoot "api\studio\inc\fmod_studio.hpp"
    if ((Test-Path $coreInc) -and (Test-Path $studioInc)) {
        Write-CheckResult "FMOD_ROOT" "PASS" "$fmodRoot (Core & Studio headers verified)"
    } else {
        Write-CheckResult "FMOD_ROOT" "WARN" "FMOD_ROOT is set to '$fmodRoot', but api\core\inc\fmod.hpp or api\studio\inc\fmod_studio.hpp is missing." "Verify FMOD SDK layout according to README.md."
    }
} else {
    Write-CheckResult "FMOD_ROOT" "FAIL" "Environment variable FMOD_ROOT is not set." "Set `$env:FMOD_ROOT to your manually installed FMOD SDK."
}

# 6. Git Status Check
$gitCmd = Get-Command git.exe -ErrorAction SilentlyContinue
if ($gitCmd) {
    $branch = (& git rev-parse --abbrev-ref HEAD 2>&1).Trim()
    $statusSummary = (& git status --porcelain 2>&1)
    $modifiedCount = if ($statusSummary) { ($statusSummary | Measure-Object).Count } else { 0 }
    Write-CheckResult "Git" "PASS" "Branch '$branch', $modifiedCount modified/untracked files"
} else {
    Write-CheckResult "Git" "WARN" "git.exe not found in PATH." "Install Git for Windows."
}

# 7. CMakePresets.json Check
if (Test-Path "CMakePresets.json") {
    Write-CheckResult "Presets" "PASS" "CMakePresets.json is present"
} else {
    Write-CheckResult "Presets" "FAIL" "CMakePresets.json not found in repository root." "Ensure you are running the script from the OrcEngine root."
}

Write-Host ""
if ($allOk) {
    if ($warnings -gt 0) {
        Write-Host "Diagnostic status: OK WITH WARNINGS ($warnings warning(s)). Environment is mostly ready." -ForegroundColor Yellow
    } else {
        Write-Host "Diagnostic status: ALL CHECKS PASSED. Environment is fully ready for building." -ForegroundColor Green
    }
    exit 0
} else {
    Write-Host "Diagnostic status: FAILED. Please resolve the critical items above before building." -ForegroundColor Red
    exit 1
}
