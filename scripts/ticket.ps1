<#
.SYNOPSIS
    CLI manager for local markdown issue tracker in .scratch/.
.DESCRIPTION
    Implements issue tracking, triage, and wayfinding frontier operations as documented in
    docs/agents/issue-tracker.md and docs/agents/triage-labels.md.
.EXAMPLE
    .\scripts\ticket.ps1 list
    .\scripts\ticket.ps1 frontier
    .\scripts\ticket.ps1 claim .scratch/renderer/issues/01-fix-pipeline.md
    .\scripts\ticket.ps1 resolve .scratch/renderer/issues/01-fix-pipeline.md -Answer "Fixed pipeline layout"
    .\scripts\ticket.ps1 new-feature audio-system -Title "FMOD Audio Integration"
    .\scripts\ticket.ps1 new-issue audio-system bus-management -Title "Implement audio bus routing" -Type task
#>

[CmdletBinding()]
param(
    [Parameter(Position = 0, Mandatory = $true)]
    [ValidateSet("list", "frontier", "claim", "resolve", "new-feature", "new-issue", "triage")]
    [string]$Command,

    [Parameter(Position = 1)]
    [string]$Target,

    [Parameter(Position = 2)]
    [string]$Secondary,

    [string]$Title,
    [string]$Type = "task",
    [string]$Status,
    [string]$BlockedBy = "",
    [string]$Answer = ""
)

$ErrorActionPreference = "Stop"

$repoRoot = Resolve-Path "$PSScriptRoot\.."
$scratchDir = Join-Path $repoRoot ".scratch"

function Ensure-ScratchDir {
    if (-not (Test-Path $scratchDir)) {
        New-Item -ItemType Directory -Path $scratchDir -Force | Out-Null
    }
}

function Parse-TicketFile {
    param([string]$FilePath)
    $content = Get-Content -Path $FilePath -Raw
    $lines = Get-Content -Path $FilePath
    
    $ticket = [PSCustomObject]@{
        Path = $FilePath
        RelativePath = (Resolve-Path $FilePath -Relative)
        FileName = Split-Path $FilePath -Leaf
        Number = ""
        Slug = ""
        Title = ""
        Status = "needs-triage"
        Type = "task"
        BlockedBy = @()
        Content = $content
    }

    if ($ticket.FileName -match "^(\d+)-(.*)\.md$") {
        $ticket.Number = $matches[1]
        $ticket.Slug = $matches[2]
    }

    foreach ($line in $lines) {
        if ($line -match "^#\s+(.+)$" -and [string]::IsNullOrEmpty($ticket.Title)) {
            $ticket.Title = $matches[1].Trim()
        } elseif ($line -match "^Status:\s*(.+)$") {
            $ticket.Status = $matches[1].Trim().ToLowerInvariant()
        } elseif ($line -match "^Type:\s*(.+)$") {
            $ticket.Type = $matches[1].Trim().ToLowerInvariant()
        } elseif ($line -match "^Blocked\s*by:\s*(.+)$") {
            $rawBlockers = $matches[1].Trim()
            if ($rawBlockers -and $rawBlockers -ne "none") {
                $ticket.BlockedBy = $rawBlockers.Split(",") | ForEach-Object { $_.Trim().PadLeft(2, '0') }
            }
        }
    }

    if ([string]::IsNullOrEmpty($ticket.Title)) {
        $ticket.Title = $ticket.Slug
    }

    return $ticket
}

function Get-AllTickets {
    param([string]$EffortFilter = "")
    $tickets = @()
    if (-not (Test-Path $scratchDir)) { return $tickets }

    $effortDirs = Get-ChildItem -Path $scratchDir -Directory
    if ($EffortFilter) {
        $effortDirs = $effortDirs | Where-Object { $_.Name -like "*$EffortFilter*" }
    }

    foreach ($effort in $effortDirs) {
        $issuesDir = Join-Path $effort.FullName "issues"
        if (Test-Path $issuesDir) {
            $files = Get-ChildItem -Path $issuesDir -Filter "*.md" | Sort-Object Name
            foreach ($file in $files) {
                $tickets += Parse-TicketFile -FilePath $file.FullName
            }
        }
    }
    return $tickets
}

# --- Command Implementations ---

switch ($Command) {
    "list" {
        Ensure-ScratchDir
        $tickets = Get-AllTickets -EffortFilter $Target
        if ($tickets.Count -eq 0) {
            Write-Host "No tickets found in .scratch/." -ForegroundColor Yellow
            return
        }

        Write-Host "`n=== Local Tickets in .scratch/ ===`n" -ForegroundColor Cyan
        $grouped = $tickets | Group-Object { Split-Path (Split-Path (Split-Path $_.Path -Parent) -Parent) -Leaf }

        foreach ($group in $grouped) {
            Write-Host "Feature: $($group.Name)" -ForegroundColor Green -NoNewline
            $specPath = Join-Path $scratchDir "$($group.Name)\spec.md"
            if (Test-Path $specPath) {
                Write-Host " (spec: present)" -ForegroundColor DarkGray
            } else {
                Write-Host " (spec: missing)" -ForegroundColor DarkYellow
            }

            foreach ($t in $group.Group) {
                $statusColor = switch ($t.Status) {
                    "ready-for-agent" { "Green" }
                    "claimed" { "Cyan" }
                    "resolved" { "DarkGray" }
                    "needs-triage" { "Yellow" }
                    "needs-info" { "Magenta" }
                    "ready-for-human" { "Blue" }
                    "wontfix" { "Red" }
                    default { "White" }
                }
                $blockerStr = if ($t.BlockedBy.Count -gt 0) { " [blocked by: $($t.BlockedBy -join ', ')]" } else { "" }
                Write-Host "  [$($t.Number)] " -ForegroundColor White -NoNewline
                Write-Host "[$($t.Status.ToUpper())]" -ForegroundColor $statusColor -NoNewline
                Write-Host " ($($t.Type)) $($t.Title)$blockerStr" -ForegroundColor Gray
                Write-Host "       Path: $($t.RelativePath)" -ForegroundColor DarkGray
            }
            Write-Host ""
        }
    }

    "frontier" {
        # Wayfinding Frontier:
        # Scan .scratch/<effort>/issues/ for files that are open, unblocked, and unclaimed. First by number wins.
        Ensure-ScratchDir
        $tickets = Get-AllTickets -EffortFilter $Target
        if ($tickets.Count -eq 0) {
            Write-Host "No tickets found to search." -ForegroundColor Yellow
            return
        }

        # Build set of resolved numbers
        $resolvedSet = @{}
        foreach ($t in $tickets) {
            if ($t.Status -eq "resolved") {
                $resolvedSet[$t.Number] = $true
            }
        }

        # Candidate must be open, unclaimed, and all blockers resolved
        $candidates = @()
        foreach ($t in $tickets) {
            if ($t.Status -in @("resolved", "wontfix", "claimed", "ready-for-human")) {
                continue
            }
            $allBlockersResolved = $true
            foreach ($b in $t.BlockedBy) {
                if (-not $resolvedSet.ContainsKey($b)) {
                    $allBlockersResolved = $false
                    break
                }
            }
            if ($allBlockersResolved) {
                $candidates += $t
            }
        }

        if ($candidates.Count -eq 0) {
            Write-Host "Frontier is empty. No open, unblocked, unclaimed tickets available." -ForegroundColor Yellow
            return
        }

        # Sort candidate by number
        $candidate = $candidates | Sort-Object { [int]$_.Number } | Select-Object -First 1

        Write-Host "`n=== Next Frontier Ticket for Agent ===" -ForegroundColor Cyan
        Write-Host "Ticket: $($candidate.Number) - $($candidate.Title)" -ForegroundColor Green
        Write-Host "Path:   $($candidate.RelativePath)" -ForegroundColor White
        Write-Host "Status: $($candidate.Status)" -ForegroundColor Yellow
        Write-Host "Type:   $($candidate.Type)" -ForegroundColor Gray
        Write-Host "`nTo claim this ticket, run:" -ForegroundColor DarkGray
        Write-Host "  .\scripts\ticket.ps1 claim $($candidate.RelativePath)`n" -ForegroundColor Cyan
    }

    "claim" {
        if (-not $Target) {
            Write-Error "Please specify the ticket file path. Example: .\scripts\ticket.ps1 claim .scratch/feat/issues/01-slug.md"
        }
        $resolvedPath = Resolve-Path $Target
        if (-not (Test-Path $resolvedPath)) {
            Write-Error "File not found: $Target"
        }

        $lines = Get-Content -Path $resolvedPath
        $statusFound = $false
        $newLines = @()
        foreach ($line in $lines) {
            if ($line -match "^Status:\s*") {
                $newLines += "Status: claimed"
                $statusFound = $true
            } else {
                $newLines += $line
            }
        }
        if (-not $statusFound) {
            # Insert after the title or at beginning
            $newLines = @("Status: claimed") + $lines
        }

        Set-Content -Path $resolvedPath -Value $newLines
        Write-Host "Claimed ticket: $Target" -ForegroundColor Green
        Write-Host "Status set to 'claimed'." -ForegroundColor Cyan
    }

    "resolve" {
        if (-not $Target) {
            Write-Error "Please specify the ticket file path. Example: .\scripts\ticket.ps1 resolve .scratch/feat/issues/01-slug.md -Answer 'Summary of changes'"
        }
        $resolvedPath = Resolve-Path $Target
        if (-not (Test-Path $resolvedPath)) {
            Write-Error "File not found: $Target"
        }

        $ticket = Parse-TicketFile -FilePath $resolvedPath
        $lines = Get-Content -Path $resolvedPath
        $newLines = @()
        $statusFound = $false

        foreach ($line in $lines) {
            if ($line -match "^Status:\s*") {
                $newLines += "Status: resolved"
                $statusFound = $true
            } else {
                $newLines += $line
            }
        }
        if (-not $statusFound) {
            $newLines = @("Status: resolved") + $newLines
        }

        # Append Answer section if provided
        if ($Answer) {
            $hasAnswerHeading = ($newLines | Select-String -Pattern "^##\s+Answer").Length -gt 0
            if (-not $hasAnswerHeading) {
                $newLines += ""
                $newLines += "## Answer"
                $newLines += ""
                $newLines += $Answer
            } else {
                $newLines += ""
                $newLines += $Answer
            }
        }

        Set-Content -Path $resolvedPath -Value $newLines
        Write-Host "Resolved ticket: $Target" -ForegroundColor Green

        # Update map.md Decisions-so-far if present
        $effortDir = Split-Path (Split-Path $resolvedPath -Parent) -Parent
        $mapFile = Join-Path $effortDir "map.md"
        if (Test-Path $mapFile) {
            $decisionGist = if ($Answer) { $Answer } else { $ticket.Title }
            $ticketRel = "issues/$($ticket.FileName)"
            $decisionEntry = "- [$($ticket.Number)-$($ticket.Slug)]($ticketRel): $decisionGist"

            $mapContent = Get-Content -Path $mapFile
            $mapNewLines = @()
            $inDecisions = $false
            $decisionsAppended = $false

            foreach ($mLine in $mapContent) {
                $mapNewLines += $mLine
                if ($mLine -match "^##\s+Decisions\s+so\s+far") {
                    $inDecisions = $true
                    $mapNewLines += $decisionEntry
                    $decisionsAppended = $true
                }
            }

            if (-not $decisionsAppended) {
                $mapNewLines += ""
                $mapNewLines += "## Decisions so far"
                $mapNewLines += $decisionEntry
            }

            Set-Content -Path $mapFile -Value $mapNewLines
            Write-Host "Updated Decisions-so-far in $($mapFile)" -ForegroundColor Cyan
        }
    }

    "new-feature" {
        if (-not $Target) {
            Write-Error "Please specify feature slug. Example: .\scripts\ticket.ps1 new-feature audio-system -Title 'Audio System'"
        }
        Ensure-ScratchDir
        $featureSlug = $Target.ToLowerInvariant() -replace "[^a-z0-9\-]", "-"
        $featureDir = Join-Path $scratchDir $featureSlug
        $issuesDir = Join-Path $featureDir "issues"
        New-Item -ItemType Directory -Path $issuesDir -Force | Out-Null

        $featureTitle = if ($Title) { $Title } else { $featureSlug }

        # spec.md
        $specPath = Join-Path $featureDir "spec.md"
        if (-not (Test-Path $specPath)) {
            $specContent = @"
# Spec: $featureTitle

## Overview
Brief description of the feature or goal.

## Requirements
- Requirement 1
- Requirement 2

## Constraints & Standards
Follow OrcEngine guidelines in AGENTS.md.
"@
            Set-Content -Path $specPath -Value $specContent
            Write-Host "Created spec: $specPath" -ForegroundColor Green
        }

        # map.md
        $mapPath = Join-Path $featureDir "map.md"
        if (-not (Test-Path $mapPath)) {
            $mapContent = @"
# $featureTitle Map

## Notes
Initial exploratory notes.

## Decisions so far

## Fog
Unknowns and risks to explore.
"@
            Set-Content -Path $mapPath -Value $mapContent
            Write-Host "Created map: $mapPath" -ForegroundColor Green
        }

        Write-Host "Feature scaffold created at: .scratch/$featureSlug" -ForegroundColor Cyan
    }

    "new-issue" {
        if (-not $Target -or -not $Secondary) {
            Write-Error "Usage: .\scripts\ticket.ps1 new-issue <feature-slug> <ticket-slug> [-Title '...'] [-Type task|research|prototype|grilling] [-BlockedBy NN]"
        }
        Ensure-ScratchDir
        $featureSlug = $Target.ToLowerInvariant()
        $ticketSlug = $Secondary.ToLowerInvariant() -replace "[^a-z0-9\-]", "-"
        $featureDir = Join-Path $scratchDir $featureSlug
        $issuesDir = Join-Path $featureDir "issues"

        if (-not (Test-Path $issuesDir)) {
            New-Item -ItemType Directory -Path $issuesDir -Force | Out-Null
        }

        # Calculate next NN
        $existing = Get-ChildItem -Path $issuesDir -Filter "*.md"
        $maxNum = 0
        foreach ($f in $existing) {
            if ($f.Name -match "^(\d+)-") {
                $num = [int]$matches[1]
                if ($num -gt $maxNum) { $maxNum = $num }
            }
        }
        $nextNum = ($maxNum + 1).ToString("D2")
        $fileName = "$nextNum-$ticketSlug.md"
        $filePath = Join-Path $issuesDir $fileName

        $issueTitle = if ($Title) { $Title } else { "$ticketSlug" }
        $initialStatus = if ($Status) { $Status } else { "ready-for-agent" }
        $blockerLine = if ($BlockedBy) { "Blocked by: $BlockedBy`n" } else { "" }

        $content = @"
# $issueTitle

Status: $initialStatus
Type: $Type
${blockerLine}
## Description
Detailed explanation of this issue and completion criteria.

## Comments
"@
        Set-Content -Path $filePath -Value $content
        Write-Host "Created issue ticket: $filePath" -ForegroundColor Green
        Write-Host "Relative path: .scratch/$featureSlug/issues/$fileName" -ForegroundColor Cyan
    }

    "triage" {
        Ensure-ScratchDir
        $tickets = Get-AllTickets
        if ($tickets.Count -eq 0) {
            Write-Host "No tickets found to triage." -ForegroundColor Yellow
            return
        }

        $canonicalRoles = @(
            "needs-triage",
            "needs-info",
            "ready-for-agent",
            "ready-for-human",
            "claimed",
            "resolved",
            "wontfix"
        )

        Write-Host "`n=== Triage Summary (docs/agents/triage-labels.md) ===`n" -ForegroundColor Cyan
        foreach ($role in $canonicalRoles) {
            $matching = @($tickets | Where-Object { $_.Status -eq $role })
            $roleColor = switch ($role) {
                "ready-for-agent" { "Green" }
                "claimed" { "Cyan" }
                "resolved" { "DarkGray" }
                "needs-triage" { "Yellow" }
                "needs-info" { "Magenta" }
                "ready-for-human" { "Blue" }
                "wontfix" { "Red" }
                default { "White" }
            }
            Write-Host "[$($role.ToUpper())] ($($matching.Count))" -ForegroundColor $roleColor
            foreach ($t in $matching) {
                Write-Host "  - $($t.RelativePath) - $($t.Title)" -ForegroundColor Gray
            }
        }
        Write-Host ""
    }
}
