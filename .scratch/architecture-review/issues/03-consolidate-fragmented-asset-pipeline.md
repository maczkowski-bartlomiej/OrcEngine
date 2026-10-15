# Issue 03: Consolidate the Fragmented Asset Pipeline

Status: needs-triage

## Description

Asset management is split into 4 separate template specializations (`Resources<Font>`, `Resources<Texture>`, `Resources<Shader>`, `Resources<Animation>`). Each re-implements XML parsing loops with `tinyxml2`, and parsing failures cause hard engine termination via `ORC_FATAL_CHECK`.

## Objective

1. Merge the resource holders into a single deep `AssetManager` module presenting a unified interface (`get<T>`, `loadManifest`).
2. Centralize XML manifest traversal into one parser.
3. Replace fatal aborts with graceful fallback assets (e.g. 1x1 magenta texture, default font) and non-fatal logging.

See `.scratch/architecture-review/ideas.md#candidate-3-consolidate-the-fragmented-asset-pipeline` for detailed analysis and grilling questions.

## Comments
