# Repository Guidelines

## Project Structure & Module Organization

- Chapters live at the repo root as `NN-topic-name/` (e.g., `04-hash-tables/`) and each has a `README.md`.
- Chapters may contain units as `NN-subtopic-name/` (e.g., `04-hash-tables/01-basic-hash-table/`).
- Units are organized by language: `python/`, `java/`, `javascript/`, `c/`, `cpp/`, `csharp/` (some units may implement only a subset).
- `tw-gradcs-data-structures.sln` is provided for C# projects.

## Build, Test, and Development Commands

Run commands from the unit/language directory you’re editing.

- Python: run demos with `python3 *.py`; tests: `python3 -m unittest -v` (repo prefers stdlib test runners; no network installs).
- JavaScript (Node): run demos with `node *.js`; tests: `node --test` (no `npm install` expected for most units).
- Java: compile with `javac *.java`; run demos/tests with `java MainClass` / `java *Test` (no Maven required).
- C: `make` (build), `make test`, `make run` (`c/Makefile`).
- C++: `cmake -S . -B build && cmake --build build && ctest --test-dir build`.
- C#: `dotnet run` / `dotnet run -- --test` in the `csharp/` project (optionally add `DOTNET_CLI_HOME=out` in restricted environments).

## Coding Style & Naming Conventions

- Indentation: 4 spaces; avoid tabs.
- Documentation is primarily Traditional Chinese; introduce terms as “中文（English）” on first use.
- Commenting policy: keep **English, line-level comments** on the vast majority of non-blank lines (target ≥90%) so the code is readable as a study note set.
- Naming:
  - Folders: `NN-...` kebab-case.
  - Tests: `test_*.py`, `*.test.js`, `*Test.java`, `test_*.c`, `test_*.cpp`, `*Test.cs`.

## Testing Guidelines

- Keep tests next to the implementation and runnable with the commands above.
- Add edge-case coverage (collisions, resizing/rehashing, null/empty keys where applicable).

## Commit & Pull Request Guidelines

- Git history is currently minimal (e.g., `first commit`); use short, imperative commits with an optional scope: `hash-tables: add open addressing tests`.
- PRs should describe the change, list affected units/languages, link any issues, and include the exact test commands you ran. Update the relevant unit `README.md` when behavior or commands change.

## Implementation Notes

- For each unit/language implementation, add a Traditional Chinese explainer in the same folder (e.g., `python/IMPLEMENTATION.md`) with short code excerpts and how to run tests.

## Agent-Specific Instructions

- Prefer unit-scoped changes; avoid unrelated refactors. Keep the per-line English comment style consistent within a file.
