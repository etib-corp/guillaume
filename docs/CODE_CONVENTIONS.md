# Code Conventions

This document defines coding standards for Guillaume. The goal is consistent,
readable, and maintainable code.

## General Rules

- Use clear and descriptive names.
- Formatting is defined by `.clang-format` (tabs, 80 columns) and is not a
  matter of taste: run `./scripts/run-clang-format.sh` before committing.
- Prefer small, focused functions over large monolithic blocks.
- Avoid duplicated logic (DRY principle).
- Document every public declaration (see "Documenting Code"); inside
  functions, add comments only when intent is not obvious.
- Validate inputs and handle errors explicitly.

## C++ Conventions

- Use `PascalCase` for classes and `camelCase` for functions and variables.
- Prefix member fields with `_`.
- Use `nullptr` instead of `NULL` or `0`.
- Prefer RAII and smart pointers (`std::unique_ptr`, `std::shared_ptr`).
- Apply `const` correctness whenever possible.
- Prefer `enum class` over unscoped enums.

## Class Layout

In class declarations, keep sections in this order:

1. `public`
2. `protected`
3. `private`

Within each section, keep this order:

1. Types (`struct`, `enum`, aliases)
2. Constructors and special members
3. Methods
4. Fields

## Documenting Code

Every declaration in a public header has a Doxygen comment. Write it for a
developer who opens the file for the first time and knows C++ but not this
project. `headers/guillaume/namespaces.hpp` is the map of the modules; start
there.

### What a class comment answers

In this order, in plain sentences:

1. **What it is**, in one line (`@brief`). Say what it represents or does,
   not that it is "a class" or "a component for X".
2. **Its role in the frame**: for a component, who writes it and who reads
   it; for a system, what it reads, what it writes and in which phase; for a
   widget class, which component holds its state and which system holds its
   rules.
3. **How to use it**, when that is not obvious: a short `@code` example, or
   the one call a newcomer needs.
4. **What to watch out for**: a unit, an ownership rule, an ordering
   constraint, a value that is overwritten every frame.
5. `@see` the two or three types to read next.

Skip a point that has nothing to say. A three-line comment is fine for a
small type.

### Members

- `@brief` says what the function does or what the value means, with its
  unit when it has one (`in dp`, `in seconds`, `0 to 1`).
- Document every parameter and the return value with a real description:
  `@param width The new width, in dp.`, not `@param width The width.` and
  never a placeholder such as `It.` or `The value.`.
- Say what a setter triggers when it does more than store: marking the
  component changed, restyling at once, building a child.
- Say when a function can throw and what an "empty" result looks like
  (`nullptr`, `InvalidIdentifier`, `std::nullopt`).
- A trivial getter/setter pair may use one-line comments.

### Tone

- Describe the code as it is. No history: not "now", "no longer", "used
  to", "legacy", "migrated", "kept for compatibility". History belongs in
  `CHANGELOG.md` and `docs/MIGRATION_GUIDE.md`.
- Use the project's words consistently: *component* (state), *system*
  (rules run every frame), *entity* (an identifier plus its components),
  *widget* (an entity class a user creates, such as `Button`), *facade*
  only when explaining that a widget class holds no state.
- Prefer a concrete example to an abstract sentence.
- Comments in `.cpp` files explain why, not what.

## Checklist Before Opening A PR

- Naming follows conventions.
- Class and method order is consistent.
- No obvious duplication.
- Error handling is present where needed.
- Public declarations are documented as described in "Documenting Code".
- `./scripts/run-clang-format.sh` was run.
