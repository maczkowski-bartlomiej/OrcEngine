# scripts/setup-git-hooks.ps1: Install OrcEngine Git Pre-Commit Hook on Windows

$ErrorActionPreference = "Stop"

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$repoRoot = Split-Path -Parent $scriptDir
$hooksDir = Join-Path $repoRoot ".git\hooks"

if (-not (Test-Path $hooksDir)) {
	Write-Host "Error: .git/hooks directory not found at '$hooksDir'." -ForegroundColor Red
	exit 1
}

$preCommitPath = Join-Path $hooksDir "pre-commit"

$hookContent = @'
#!/bin/sh
# OrcEngine Pre-Commit Hook: Validates conventions on changed files

ROOT_DIR="$(git rev-parse --show-toplevel)"

if command -v pwsh >/dev/null 2>&1; then
	pwsh -NoProfile -ExecutionPolicy Bypass -File "$ROOT_DIR/scripts/lint-conventions.ps1" -ChangedOnly
	exit $?
elif command -v powershell >/dev/null 2>&1; then
	powershell -NoProfile -ExecutionPolicy Bypass -File "$ROOT_DIR/scripts/lint-conventions.ps1" -ChangedOnly
	exit $?
elif [ -f "$ROOT_DIR/scripts/lint-conventions.sh" ]; then
	"$ROOT_DIR/scripts/lint-conventions.sh" --changed-only
	exit $?
fi
'@

Set-Content -Path $preCommitPath -Value $hookContent -NoNewline
Write-Host "Successfully installed OrcEngine pre-commit hook to '$preCommitPath'!" -ForegroundColor Green
