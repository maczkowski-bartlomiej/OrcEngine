# Issue 04: Encapsulate Camera Projection & Viewport Synchronization

Status: needs-triage

## Description

`Camera` exposes 14 shallow mutators that eagerly recompute view-projection matrices on every call. `Engine::onEvent` directly intercepts window resize events and mutates the active layer's camera viewport, leaking synchronization responsibilities across callers.

## Objective

1. Deepen `Camera` into a `Camera2D` module with lazy projection evaluation (dirty-flag caching).
2. Encapsulate viewport tracking and coordinate transformation helpers (`screenToWorld`, `worldToScreen`).
3. Eliminate redundant setter variations and eager recomputation.

See `.scratch/architecture-review/ideas.md#candidate-4-encapsulate-camera-projection--viewport-synchronization` for detailed analysis and grilling questions.

## Comments
