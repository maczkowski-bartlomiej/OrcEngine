<#
.SYNOPSIS
    Lints C++ code against OrcEngine coding conventions defined in AGENTS.md.
.DESCRIPTION
    Checks:
    - Headers (.hpp, .inl) start with '#pragma once'.
    - Engine code in OrcEngine/ is wrapped in 'namespace orc'.
    - Preserves tab-based indentation (flags space-based indentation).
    - Trailing whitespace (flags whitespace at end of lines).
    - File extensions (.hpp, .cpp, .inl).
    - Optional -ChangedOnly to lint only modified/staged git files.
    - Optional -Fix to automatically prepend missing '#pragma once'.
.EXAMPLE
    .\scripts\lint-conventions.ps1
    .\scripts\lint-conventions.ps1 -ChangedOnly
    .\scripts\lint-conventions.ps1 -Fix
#>

[CmdletBinding()]
param(
    [switch]$ChangedOnly,
    [switch]$Fix,
    [string[]]$Paths = @("OrcEngine", "TestProject")
)

$ErrorActionPreference = "Continue"

$repoRoot = Resolve-Path "$PSScriptRoot\.."
Set-Location $repoRoot

$filesToCheck = @()

if ($ChangedOnly) {
    $gitOutput = (& git status --porcelain -uall 2>&1)
    foreach ($line in $gitOutput) {
        if ($line -match "^[ MADRCU?]{2}\s+(.+)$") {
            $relPath = $matches[1].Trim()
            if ($relPath -match " -> ") {
                $relPath = ($relPath -split " -> ")[1].Trim()
            }
            if ($relPath -match "\.(hpp|cpp|inl)$" -and (Test-Path $relPath)) {
                $filesToCheck += (Resolve-Path $relPath).Path
            }
        }
    }
    if ($filesToCheck.Count -eq 0) {
        Write-Host "No changed C++ files found in git working tree." -ForegroundColor Green
        exit 0
    }
} else {
    foreach ($p in $Paths) {
        if (Test-Path $p) {
            $filesToCheck += Get-ChildItem -Path $p -Include "*.hpp", "*.cpp", "*.inl" -Recurse | ForEach-Object { $_.FullName }
        }
    }
}

$filesToCheck = $filesToCheck | Sort-Object -Unique

Write-Host "`n=== OrcEngine C++ Conventions Linter ===" -ForegroundColor Cyan
Write-Host "Checking $($filesToCheck.Count) file(s)...`n" -ForegroundColor DarkGray

$totalErrors = 0
$totalWarnings = 0

foreach ($filePath in $filesToCheck) {
    $relPath = $filePath.Replace("$repoRoot\", "").Replace("\", "/")
    $ext = [System.IO.Path]::GetExtension($filePath).ToLowerInvariant()
    $lines = Get-Content -Path $filePath
    $content = $lines -join "`n"
    $fileIssues = @()

    # 1. Check #pragma once in headers and inlines
    if ($ext -in @(".hpp", ".inl")) {
        $hasPragmaOnce = $false
        for ($i = 0; $i -lt [Math]::Min(10, $lines.Count); $i++) {
            if ($lines[$i] -match "^\s*#pragma\s+once\b") {
                $hasPragmaOnce = $true
                break
            }
        }

        if (-not $hasPragmaOnce) {
            if ($Fix) {
                $newContent = "#pragma once`n`n" + (Get-Content -Path $filePath -Raw)
                Set-Content -Path $filePath -Value $newContent
                $fileIssues += [PSCustomObject]@{
                    Level = "FIXED"
                    Message = "Added missing '#pragma once'"
                    Line = 1
                }
            } else {
                $fileIssues += [PSCustomObject]@{
                    Level = "ERROR"
                    Message = "Missing '#pragma once' near top of header"
                    Line = 1
                }
                $totalErrors++
            }
        }
    }

    # 2. Check namespace orc in OrcEngine/ files
    if ($relPath -like "OrcEngine/*" -and $relPath -notlike "*OrcPch.hpp" -and $relPath -notlike "*EntryPoint.cpp" -and $relPath -notlike "*StbImage.cpp" -and $relPath -notlike "*Debug.hpp" -and $relPath -notlike "*Orc.hpp" -and $relPath -notlike "*Platform.hpp" -and $relPath -notlike "*Core.hpp" -and $relPath -notlike "*AudioUtility.hpp" -and $relPath -notlike "*FontUtility.hpp") {
        if ($content -notmatch "\bnamespace\s+orc\b") {
            $fileIssues += [PSCustomObject]@{
                Level = "ERROR"
                Message = "Engine code must be enclosed in 'namespace orc'"
                Line = 1
            }
            $totalErrors++
        }
    }

    # 3. Check tab-based indentation (OrcEngine standard: preserve tab-based indentation)
    $spaceIndentCount = 0
    $firstSpaceLine = 0
    for ($i = 0; $i -lt $lines.Count; $i++) {
        $line = $lines[$i]
        # Ignore comments and multiline comment continuations
        if ($line -match "^\s*\*" -or $line -match "^\s*//") { continue }
        # Match 4 or more leading spaces
        if ($line -match "^ {4,}\S") {
            $spaceIndentCount++
            if ($firstSpaceLine -eq 0) {
                $firstSpaceLine = $i + 1
            }
        }
    }

    if ($spaceIndentCount -gt 0) {
        if ($Fix) {
            $fixedContent = foreach ($line in $lines) {
                $cur = $line
                while ($cur -match "^(\t*)    ") {
                    $cur = $cur -replace "^(\t*)    ", "`$1`t"
                }
                $cur = $cur -replace "\s+$", ""
                $cur
            }
            Set-Content -Path $filePath -Value $fixedContent
            $lines = Get-Content -Path $filePath
            $fileIssues += [PSCustomObject]@{
                Level = "FIXED"
                Message = "Converted space-based indentation to tabs and trimmed trailing whitespace"
                Line = $firstSpaceLine
            }
        } else {
            $fileIssues += [PSCustomObject]@{
                Level = "WARN"
                Message = "Space-based indentation detected on $spaceIndentCount line(s). OrcEngine standard is tab indentation."
                Line = $firstSpaceLine
            }
            $totalWarnings++
        }
    }

    # 4. Check trailing whitespace
    $trailingWhitespaceCount = 0
    $firstTrailingLine = 0
    for ($i = 0; $i -lt $lines.Count; $i++) {
        if ($lines[$i] -match "\s+$") {
            $trailingWhitespaceCount++
            if ($firstTrailingLine -eq 0) {
                $firstTrailingLine = $i + 1
            }
        }
    }

    if ($trailingWhitespaceCount -gt 0) {
        if ($Fix) {
            $fixedContent = foreach ($line in $lines) {
                $line -replace "\s+$", ""
            }
            Set-Content -Path $filePath -Value $fixedContent
            $fileIssues += [PSCustomObject]@{
                Level = "FIXED"
                Message = "Stripped trailing whitespace on $trailingWhitespaceCount line(s)"
                Line = $firstTrailingLine
            }
        } else {
            $fileIssues += [PSCustomObject]@{
                Level = "WARN"
                Message = "Trailing whitespace detected on $trailingWhitespaceCount line(s)."
                Line = $firstTrailingLine
            }
            $totalWarnings++
        }
    }

    # Report if issues exist
    if ($fileIssues.Count -gt 0) {
        Write-Host "$relPath" -ForegroundColor White
        foreach ($issue in $fileIssues) {
            $color = switch ($issue.Level) {
                "ERROR" { "Red" }
                "WARN" { "Yellow" }
                "FIXED" { "Green" }
                default { "Gray" }
            }
            Write-Host "  Line $($issue.Line) [$($issue.Level)]: $($issue.Message)" -ForegroundColor $color
        }
    }
}

Write-Host ""
if ($totalErrors -eq 0 -and $totalWarnings -eq 0) {
    Write-Host "All files passed conventions check!" -ForegroundColor Green
    exit 0
} elseif ($totalErrors -eq 0) {
    Write-Host "Passed with $totalWarnings warning(s)." -ForegroundColor Yellow
    exit 0
} else {
    Write-Host "Failed with $totalErrors error(s) and $totalWarnings warning(s)." -ForegroundColor Red
    exit 1
}
