# scripts/verify.ps1: Unified Verification Gate for OrcEngine on Windows
# Executes convention linting, build runner, and demo smoke test in strict sequence.

[CmdletBinding()]
param(
	[switch]$All,
	[switch]$Fix,
	[string]$Preset = "windows-debug",
	[int]$Timeout = 5
)

$ErrorActionPreference = "Stop"

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$repoRoot = Split-Path -Parent $scriptDir
Set-Location $repoRoot

Write-Host "`n====================================================" -ForegroundColor Cyan
Write-Host "       OrcEngine Unified Verification Gate          " -ForegroundColor Cyan
Write-Host "====================================================`n" -ForegroundColor Cyan

# Stage 1: Conventions Lint
Write-Host "[Stage 1/3] Running Conventions Linter..." -ForegroundColor Yellow
$lintParams = @{}
if (-not $All) {
	$lintParams["ChangedOnly"] = $true
}
if ($Fix) {
	$lintParams["Fix"] = $true
}

& "$scriptDir\lint-conventions.ps1" @lintParams
if ($LASTEXITCODE -ne 0) {
	Write-Host "`n[FAILED] Stage 1 (Conventions Lint) failed." -ForegroundColor Red
	exit 1
}
Write-Host "[PASSED] Stage 1: Conventions check clean.`n" -ForegroundColor Green

# Stage 2: Compilation & Linkage
Write-Host "[Stage 2/3] Building Engine & TestProject (Preset: $Preset)..." -ForegroundColor Yellow
& "$scriptDir\build.ps1" -Preset $Preset
if ($LASTEXITCODE -ne 0) {
	Write-Host "`n[FAILED] Stage 2 (Build) failed." -ForegroundColor Red
	exit 2
}
Write-Host "[PASSED] Stage 2: Build succeeded with zero errors.`n" -ForegroundColor Green

# Stage 3: Runtime Demo Smoke Test
Write-Host "[Stage 3/3] Running Demo Smoke Test (Timeout: ${Timeout}s)..." -ForegroundColor Yellow
& "$scriptDir\run-demo.ps1" -Preset $Preset -Timeout $Timeout -FailOnErrorLog
if ($LASTEXITCODE -ne 0) {
	Write-Host "`n[FAILED] Stage 3 (Demo Smoke Test) failed." -ForegroundColor Red
	exit 3
}
Write-Host "[PASSED] Stage 3: Demo app executed and exited cleanly.`n" -ForegroundColor Green

Write-Host "====================================================" -ForegroundColor Green
Write-Host "       ALL VERIFICATION GATES PASSED (100%)         " -ForegroundColor Green
Write-Host "====================================================`n" -ForegroundColor Green
