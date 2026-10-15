#!/usr/bin/env bash
# scripts/ticket.sh: Local Markdown Issue Tracker and Wayfinder CLI for Linux/macOS

set -eo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
SCRATCH_DIR="$REPO_ROOT/.scratch"

mkdir -p "$SCRATCH_DIR"

COMMAND="${1:-}"
TARGET="${2:-}"
SECONDARY="${3:-}"

usage() {
    cat << EOF
Usage: ./scripts/ticket.sh <command> [args...]

Commands:
  list [effort]                                  List features and tickets
  frontier [effort]                              Find next unblocked, unclaimed ticket
  claim <ticket-path>                            Set ticket status to 'claimed'
  resolve <ticket-path> [answer-gist]            Set ticket status to 'resolved' and append answer
  new-feature <effort-slug> [title]              Scaffold .scratch/<effort-slug>/ (spec + map)
  new-issue <effort-slug> <slug> [title] [type]  Scaffold .scratch/<effort-slug>/issues/<NN>-<slug>.md
  triage                                         Group all tickets by triage status
EOF
    exit 1
}

if [ -z "$COMMAND" ]; then
    usage
fi

case "$COMMAND" in
    list)
        EFFORT_FILTER="$TARGET"
        printf "\n\033[36m=== Local Tickets in .scratch/ ===\033[0m\n\n"
        FOUND_ANY=0
        for feature_dir in "$SCRATCH_DIR"/*/; do
            [ -d "$feature_dir" ] || continue
            feature_name="$(basename "$feature_dir")"
            if [ -n "$EFFORT_FILTER" ] && [[ "$feature_name" != *"$EFFORT_FILTER"* ]]; then
                continue
            fi
            FOUND_ANY=1
            SPEC_STATUS="\033[33m(spec: missing)\033[0m"
            if [ -f "$feature_dir/spec.md" ]; then
                SPEC_STATUS="\033[90m(spec: present)\033[0m"
            fi
            printf "\033[32mFeature: %s\033[0m %b\n" "$feature_name" "$SPEC_STATUS"

            ISSUES_DIR="$feature_dir/issues"
            if [ -d "$ISSUES_DIR" ]; then
                for issue_file in "$ISSUES_DIR"/*.md; do
                    [ -f "$issue_file" ] || continue
                    fname="$(basename "$issue_file")"
                    num="${fname%%-*}"
                    status="$(grep -iE '^Status:' "$issue_file" | head -n1 | sed -E 's/^Status:[[:space:]]*//I' | tr -d '\r')"
                    status="${status:-needs-triage}"
                    type="$(grep -iE '^Type:' "$issue_file" | head -n1 | sed -E 's/^Type:[[:space:]]*//I' | tr -d '\r')"
                    type="${type:-task}"
                    title="$(grep -iE '^# ' "$issue_file" | head -n1 | sed -E 's/^#[[:space:]]*//' | tr -d '\r')"
                    title="${title:-$fname}"
                    blockers="$(grep -iE '^Blocked by:' "$issue_file" | head -n1 | sed -E 's/^Blocked by:[[:space:]]*//I' | tr -d '\r')"
                    
                    color="\033[37m"
                    case "$status" in
                        ready-for-agent) color="\033[32m" ;;
                        claimed) color="\033[36m" ;;
                        resolved) color="\033[90m" ;;
                        needs-triage) color="\033[33m" ;;
                        needs-info) color="\033[35m" ;;
                        ready-for-human) color="\033[34m" ;;
                        wontfix) color="\033[31m" ;;
                    esac
                    
                    blocker_str=""
                    if [ -n "$blockers" ] && [ "$blockers" != "none" ]; then
                        blocker_str=" [blocked by: $blockers]"
                    fi

                    printf "  [%s] %b[%s]\033[0m (%s) %s%s\n" "$num" "$color" "$(echo "$status" | tr '[:lower:]' '[:upper:]')" "$type" "$title" "$blocker_str"
                    printf "       Path: .scratch/%s/issues/%s\n" "$feature_name" "$fname"
                done
            fi
            printf "\n"
        done
        if [ "$FOUND_ANY" -eq 0 ]; then
            printf "\033[33mNo tickets found in .scratch/.\033[0m\n"
        fi
        ;;

    frontier)
        EFFORT_FILTER="$TARGET"
        RESOLVED_NUMS=()
        ALL_TICKETS=()

        for feature_dir in "$SCRATCH_DIR"/*/; do
            [ -d "$feature_dir" ] || continue
            feature_name="$(basename "$feature_dir")"
            if [ -n "$EFFORT_FILTER" ] && [[ "$feature_name" != *"$EFFORT_FILTER"* ]]; then
                continue
            fi
            ISSUES_DIR="$feature_dir/issues"
            if [ -d "$ISSUES_DIR" ]; then
                for issue_file in "$ISSUES_DIR"/*.md; do
                    [ -f "$issue_file" ] || continue
                    status="$(grep -iE '^Status:' "$issue_file" | head -n1 | sed -E 's/^Status:[[:space:]]*//I' | tr -d '\r')"
                    fname="$(basename "$issue_file")"
                    num="${fname%%-*}"
                    if [ "$status" = "resolved" ]; then
                        RESOLVED_NUMS+=("$num")
                    fi
                done
            fi
        done

        FOUND_TICKET=""
        for feature_dir in "$SCRATCH_DIR"/*/; do
            [ -d "$feature_dir" ] || continue
            feature_name="$(basename "$feature_dir")"
            if [ -n "$EFFORT_FILTER" ] && [[ "$feature_name" != *"$EFFORT_FILTER"* ]]; then
                continue
            fi
            ISSUES_DIR="$feature_dir/issues"
            if [ -d "$ISSUES_DIR" ]; then
                for issue_file in $(ls "$ISSUES_DIR"/*.md 2>/dev/null | sort); do
                    [ -f "$issue_file" ] || continue
                    status="$(grep -iE '^Status:' "$issue_file" | head -n1 | sed -E 's/^Status:[[:space:]]*//I' | tr -d '\r')"
                    if [ "$status" = "resolved" ] || [ "$status" = "wontfix" ] || [ "$status" = "claimed" ] || [ "$status" = "ready-for-human" ]; then
                        continue
                    fi

                    blockers="$(grep -iE '^Blocked by:' "$issue_file" | head -n1 | sed -E 's/^Blocked by:[[:space:]]*//I' | tr -d '\r')"
                    is_blocked=0
                    if [ -n "$blockers" ] && [ "$blockers" != "none" ]; then
                        IFS=',' read -ra B_LIST <<< "$blockers"
                        for b in "${B_LIST[@]}"; do
                            b_clean=$(echo "$b" | tr -d ' ')
                            b_padded=$(printf "%02d" "$((10#$b_clean))" 2>/dev/null || echo "$b_clean")
                            b_found=0
                            for r in "${RESOLVED_NUMS[@]}"; do
                                if [ "$r" = "$b_padded" ]; then
                                    b_found=1
                                    break
                                fi
                            done
                            if [ "$b_found" -eq 0 ]; then
                                is_blocked=1
                                break
                            fi
                        done
                    fi

                    if [ "$is_blocked" -eq 0 ]; then
                        FOUND_TICKET="$issue_file"
                        break 2
                    fi
                done
            fi
        done

        if [ -n "$FOUND_TICKET" ]; then
            fname="$(basename "$FOUND_TICKET")"
            num="${fname%%-*}"
            title="$(grep -iE '^# ' "$FOUND_TICKET" | head -n1 | sed -E 's/^#[[:space:]]*//' | tr -d '\r')"
            status="$(grep -iE '^Status:' "$FOUND_TICKET" | head -n1 | sed -E 's/^Status:[[:space:]]*//I' | tr -d '\r')"
            type="$(grep -iE '^Type:' "$FOUND_TICKET" | head -n1 | sed -E 's/^Type:[[:space:]]*//I' | tr -d '\r')"
            rel_path="${FOUND_TICKET#$REPO_ROOT/}"

            printf "\n\033[36m=== Next Frontier Ticket for Agent ===\033[0m\n"
            printf "\033[32mTicket: %s - %s\033[0m\n" "$num" "$title"
            printf "Path:   %s\n" "$rel_path"
            printf "Status: \033[33m%s\033[0m\n" "$status"
            printf "Type:   %s\n" "$type"
            printf "\n\033[90mTo claim this ticket, run:\033[0m\n"
            printf "  \033[36m./scripts/ticket.sh claim %s\033[0m\n\n" "$rel_path"
        else
            printf "\033[33mFrontier is empty. No open, unblocked, unclaimed tickets available.\033[0m\n"
        fi
        ;;

    claim)
        [ -n "$TARGET" ] || usage
        TICKET_FILE="$TARGET"
        [ -f "$TICKET_FILE" ] || TICKET_FILE="$REPO_ROOT/$TARGET"
        [ -f "$TICKET_FILE" ] || { echo "File not found: $TARGET"; exit 1; }

        if grep -qE '^Status:' "$TICKET_FILE"; then
            sed -i -E 's/^Status:.*$/Status: claimed/' "$TICKET_FILE"
        else
            printf "Status: claimed\n%s" "$(cat "$TICKET_FILE")" > "$TICKET_FILE"
        fi
        printf "\033[32mClaimed ticket: %s\033[0m\n" "$TARGET"
        ;;

    resolve)
        [ -n "$TARGET" ] || usage
        TICKET_FILE="$TARGET"
        [ -f "$TICKET_FILE" ] || TICKET_FILE="$REPO_ROOT/$TARGET"
        [ -f "$TICKET_FILE" ] || { echo "File not found: $TARGET"; exit 1; }

        ANSWER="${SECONDARY:-}"
        if grep -qE '^Status:' "$TICKET_FILE"; then
            sed -i -E 's/^Status:.*$/Status: resolved/' "$TICKET_FILE"
        else
            printf "Status: resolved\n%s" "$(cat "$TICKET_FILE")" > "$TICKET_FILE"
        fi

        if [ -n "$ANSWER" ]; then
            if ! grep -qE '^##[[:space:]]+Answer' "$TICKET_FILE"; then
                printf "\n## Answer\n\n%s\n" "$ANSWER" >> "$TICKET_FILE"
            else
                printf "\n%s\n" "$ANSWER" >> "$TICKET_FILE"
            fi
        fi
        printf "\033[32mResolved ticket: %s\033[0m\n" "$TARGET"

        # Update parent map.md if present
        FEATURE_DIR="$(dirname "$(dirname "$TICKET_FILE")")"
        MAP_FILE="$FEATURE_DIR/map.md"
        if [ -f "$MAP_FILE" ]; then
            fname="$(basename "$TICKET_FILE")"
            num="${fname%%-*}"
            slug="${fname#*-}"
            slug="${slug%.md}"
            gist="${ANSWER:-Resolved}"
            entry="- [$num-$slug](issues/$fname): $gist"
            if grep -qE '^##[[:space:]]+Decisions so far' "$MAP_FILE"; then
                sed -i -E "/^##[[:space:]]+Decisions so far/a\\
$entry" "$MAP_FILE"
            else
                printf "\n## Decisions so far\n%s\n" "$entry" >> "$MAP_FILE"
            fi
            printf "\033[36mUpdated Decisions so far in %s\033[0m\n" "$MAP_FILE"
        fi
        ;;

    new-feature)
        [ -n "$TARGET" ] || usage
        FEAT_SLUG="$(echo "$TARGET" | tr '[:upper:]' '[:lower:]' | tr -c 'a-z0-9\n' '-' | sed 's/-*$//')"
        FEAT_TITLE="${SECONDARY:-$FEAT_SLUG}"
        FEAT_DIR="$SCRATCH_DIR/$FEAT_SLUG"
        mkdir -p "$FEAT_DIR/issues"

        if [ ! -f "$FEAT_DIR/spec.md" ]; then
            cat << EOF > "$FEAT_DIR/spec.md"
# Spec: $FEAT_TITLE

## Overview
Brief description of the feature or goal.

## Requirements
- Requirement 1
- Requirement 2

## Constraints & Standards
Follow OrcEngine guidelines in AGENTS.md.
EOF
        fi

        if [ ! -f "$FEAT_DIR/map.md" ]; then
            cat << EOF > "$FEAT_DIR/map.md"
# $FEAT_TITLE Map

## Notes
Initial exploratory notes.

## Decisions so far

## Fog
Unknowns and risks to explore.
EOF
        fi
        printf "\033[32mCreated feature scaffold at: .scratch/%s\033[0m\n" "$FEAT_SLUG"
        ;;

    new-issue)
        [ -n "$TARGET" ] && [ -n "$SECONDARY" ] || usage
        FEAT_SLUG="$(echo "$TARGET" | tr '[:upper:]' '[:lower:]')"
        TICKET_SLUG="$(echo "$SECONDARY" | tr '[:upper:]' '[:lower:]' | tr -c 'a-z0-9\n' '-' | sed 's/-*$//')"
        FEAT_DIR="$SCRATCH_DIR/$FEAT_SLUG"
        ISSUES_DIR="$FEAT_DIR/issues"
        mkdir -p "$ISSUES_DIR"

        MAX_NUM=0
        for f in "$ISSUES_DIR"/*.md; do
            [ -f "$f" ] || continue
            bname="$(basename "$f")"
            n="${bname%%-*}"
            if [ "$n" -gt "$MAX_NUM" ] 2>/dev/null; then
                MAX_NUM=$n
            fi
        done
        NEXT_NUM=$(printf "%02d" $((MAX_NUM + 1)))
        OUT_FILE="$ISSUES_DIR/$NEXT_NUM-$TICKET_SLUG.md"

        cat << EOF > "$OUT_FILE"
# $TICKET_SLUG

Status: ready-for-agent
Type: task

## Description
Detailed explanation of this issue and completion criteria.

## Comments
EOF
        printf "\033[32mCreated issue ticket: .scratch/%s/issues/%s-%s.md\033[0m\n" "$FEAT_SLUG" "$NEXT_NUM" "$TICKET_SLUG"
        ;;

    triage)
        printf "\n\033[36m=== Triage Summary (docs/agents/triage-labels.md) ===\033[0m\n\n"
        ROLES=("needs-triage" "needs-info" "ready-for-agent" "ready-for-human" "claimed" "resolved" "wontfix")
        for r in "${ROLES[@]}"; do
            case "$r" in
                ready-for-agent) color="\033[32m" ;;
                claimed) color="\033[36m" ;;
                resolved) color="\033[90m" ;;
                needs-triage) color="\033[33m" ;;
                needs-info) color="\033[35m" ;;
                ready-for-human) color="\033[34m" ;;
                wontfix) color="\033[31m" ;;
            esac

            MATCHES=()
            for f in "$SCRATCH_DIR"/*/issues/*.md; do
                [ -f "$f" ] || continue
                st="$(grep -iE '^Status:' "$f" | head -n1 | sed -E 's/^Status:[[:space:]]*//I' | tr -d '\r')"
                st="${st:-needs-triage}"
                if [ "$st" = "$r" ]; then
                    MATCHES+=("$f")
                fi
            done
            printf "%b[%s]\033[0m (%d)\n" "$color" "$(echo "$r" | tr '[:lower:]' '[:upper:]')" "${#MATCHES[@]}"
            for m in "${MATCHES[@]}"; do
                rel_path="${m#$REPO_ROOT/}"
                title="$(grep -iE '^# ' "$m" | head -n1 | sed -E 's/^#[[:space:]]*//' | tr -d '\r')"
                printf "  - %s - %s\n" "$rel_path" "$title"
            done
        done
        printf "\n"
        ;;

    *)
        usage
        ;;
esac
