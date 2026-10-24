# Resizing Window Does Not Resize Viewport

Status: resolved
Type: grilling

## Raw Proposal

Currently resizing window does not resize viewport

## Context

- **Source:** Standalone
- **Captured:** 2026-10-20

## Frontier Seeds

- ❓ Should `glViewport` be updated automatically within `Window` resize handling, `Renderer`, or by `GameLayer` on
  `WindowResizedEvent`?
- ❓ Should camera projection maintain a fixed virtual resolution / aspect ratio or expand visible world bounds with the
  new window dimensions?

## Answer

Specified and transferred into feature .scratch/window-viewport-synchronization/
