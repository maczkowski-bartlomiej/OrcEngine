<#
.SYNOPSIS
    Runs TestProject demo from its build directory and inspects logs.
.DESCRIPTION
    Ensures TestProject is executed with its working directory set to build/<preset>,
    guaranteeing that relative shader and asset paths resolve properly as required by AGENTS.md.
    Inspects logs/TestProject.log for errors and crashes.
.EXAMPLE
    .\scripts\run-demo.ps1
    .\scripts\run-demo.ps1 -Preset windows-release
    .\scripts\run-demo.ps1 -TimeoutSeconds 10 -FailOnErrorLog
#>

[CmdletBinding()]
param(
    [string]$Preset = "windows-debug",
    [int]$TimeoutSeconds = 0,
    [switch]$FailOnErrorLog
)

$ErrorActionPreference = "Stop"

$repoRoot = Resolve-Path "$PSScriptRoot\.."
$buildDir = Join-Path $repoRoot "build\$Preset"
$isWindows = [System.Environment]::OSVersion.Platform -match "Win"
$exeName = if ($isWindows) { "TestProject.exe" } else { "TestProject" }
$exePath = Join-Path $buildDir $exeName

Write-Host "`n=== OrcEngine Demo Runner ===" -ForegroundColor Cyan
Write-Host "Working Directory: $buildDir" -ForegroundColor DarkGray

if (-not (Test-Path $exePath)) {
    Write-Host "Executable '$exeName' not found in $buildDir." -ForegroundColor Red
    Write-Host "Please build first using: .\scripts\build.ps1 -Preset $Preset" -ForegroundColor Yellow
    exit 1
}

# Record log size or position before starting
$logPath = Join-Path $buildDir "logs\TestProject.log"
if (-not (Test-Path $logPath)) {
    $logPath = Join-Path $repoRoot "logs\TestProject.log"
}

$initialLogSize = 0
if (Test-Path $logPath) {
    $initialLogSize = (Get-Item $logPath).Length
}

Write-Host "Launching $exeName..." -ForegroundColor Green
$procInfo = New-Object System.Diagnostics.ProcessStartInfo
$procInfo.FileName = $exePath
$procInfo.WorkingDirectory = $buildDir
$procInfo.UseShellExecute = $false

$proc = [System.Diagnostics.Process]::Start($procInfo)

$timedOut = $false
if ($TimeoutSeconds -gt 0) {
    Write-Host "Running with $TimeoutSeconds second(s) timeout..." -ForegroundColor DarkGray
    $finished = $proc.WaitForExit($TimeoutSeconds * 1000)
    if (-not $finished) {
        Write-Host "Timeout reached ($TimeoutSeconds s). Closing demo process..." -ForegroundColor Yellow
        $proc.Kill()
        $proc.WaitForExit()
        $timedOut = $true
    }
} else {
    $proc.WaitForExit()
}

$exitCode = $proc.ExitCode
Write-Host "Process exited with code: $exitCode" -ForegroundColor White

# Log analysis
$hasLogErrors = $false
if (Test-Path $logPath) {
    Write-Host "`n=== TestProject Log Output ===" -ForegroundColor Cyan
    $newContent = Get-Content -Path $logPath
    $tail = $newContent | Select-Object -Last 20
    foreach ($line in $tail) {
        if ($line -match "\[(error|critical)\]|fatal|crash|assert") {
            Write-Host "  $line" -ForegroundColor Red
            $hasLogErrors = $true
        } else {
            Write-Host "  $line" -ForegroundColor Gray
        }
    }
}

if ($exitCode -ne 0 -and -not $timedOut) {
    Write-Host "`nDemo crashed or exited with non-zero exit code: $exitCode" -ForegroundColor Red
    exit $exitCode
}

if ($FailOnErrorLog -and $hasLogErrors) {
    Write-Host "`nErrors found in TestProject.log!" -ForegroundColor Red
    exit 1
}

Write-Host "`nDemo run completed successfully." -ForegroundColor Green
exit 0
