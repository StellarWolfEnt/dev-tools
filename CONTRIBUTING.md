# Contributing

This repository is a collection of largely independent utilities grouped by language, platform, or toolchain.

That has two implications:

1. Contributions should be narrowly scoped.
2. New additions should clearly explain why they belong in this repository and in their chosen folder.

## General expectations

- Keep each pull request focused on one utility, one folder, or one well-defined improvement.
- Prefer small, reusable building blocks over broad frameworks.
- Preserve existing style and conventions within the folder you are editing.
- Add or update documentation whenever behavior, assumptions, or usage are not obvious from the code.
- Avoid unrelated cleanup in the same change.

## Adding a new utility

When adding a new utility:

- Put it under the language or platform folder where it naturally belongs.
- Keep it self-contained unless there is a strong reason to share code with neighboring utilities.
- Document its purpose, intended environment, and any external assumptions.
- Include a short usage example when practical.

If a new utility introduces a new folder, prefer a structure that makes the language or environment obvious from the path.

## Requesting a new import include

If you want a new NASM import include added for a library, a request is usually enough.

- Open an issue or pull request that names the library.
- Include any naming or platform assumptions that matter for the generated include.

These import includes are generated with local tooling, so contributors generally do not need to hand-author the `.inc` file unless there is a specific reason to do so.

## Editing existing utilities

When changing an existing utility:

- Maintain backward compatibility unless the change is intentionally breaking and clearly documented.
- Update comments or adjacent documentation when parameter contracts or behavior change.
- Keep naming, guard macros, and file organization consistent with the surrounding code.

## NASM-specific notes

The current `nasm` includes follow a few clear patterns:

- Use include guards.
- Validate macro arguments aggressively with `%error` for incorrect usage.
- Keep platform-specific helpers separated from cross-platform helpers.
- Prefer descriptive macro names and explicit parameter behavior.

New NASM helpers should follow the same approach.

## Review standard

Before submitting a change, check that:

- the utility belongs in this repository,
- the placement and naming are clear,
- the documentation matches the code,
- the change is isolated to the intended scope.

## License

By contributing to this repository, you agree that your contributions will be dedicated to the public domain under CC0 1.0 Universal.