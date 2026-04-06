# dev-tools

`dev-tools` is a collection repository for small, language-specific utilities.

The contents are organized by language or platform area, and the items within a folder may share conventions, but they should not be assumed to be part of a single tightly coupled framework. Unless a subdirectory says otherwise, each utility should be treated as a focused, standalone building block.

## Types of utilities

Utilities in this repository generally fall into a few broad categories:

- **Helper includes** - small, focused macros or definitions intended to solve a narrow problem.
- **Import includes** - NASM include files that describe imported functions, symbols, or constants for external libraries.
- **Other small utilities** - scripts or support files that fit the same overall philosophy of being explicit, local, and easy to reuse.

These categories may differ in size and purpose, but they follow the same principles: explicit behavior, minimal assumptions, and no hidden cross-folder dependencies.


## Repository layout

Current contents:

```text
dev-tools/
  nasm/
    import/
      x-plat/
        freetype.inc
        openal32.inc
      win64/
        glew32.inc
        glfw3.inc
        kernel32.inc
        opengl32.inc
        user32.inc
    inc/
      win64/
        seh.inc
      x-plat/
        visibility.inc
```

`inc/` is used for reusable helper includes and macro utilities.

`import/` is used for include files whose main job is to expose external library imports or related declarations.

`x-plat` is the cross-platform folder. It is used for utilities that do not depend on a specific operating system.

## Current NASM utilities

The `nasm` folder currently contains two kinds of include files:

- Helper includes under `inc/`.
- Import includes under `import/`.

Current helper includes:

- `inc/win64/seh.inc`: helpers for emitting Windows x64 unwind and exception metadata.
- `inc/x-plat/visibility.inc`: macros for function visibility and common code section setup.

Current import includes:

- `import/win64/glew32.inc`: NASM import declarations for GLEW.
- `import/win64/glfw3.inc`: NASM import declarations for GLFW.
- `import/win64/kernel32.inc`: NASM import declarations for Kernel32.
- `import/win64/opengl32.inc`: NASM import declarations for OpenGL.
- `import/win64/user32.inc`: NASM import declarations for User32.
- `import/x-plat/freetype.inc`: NASM import declarations for FreeType.
- `import/x-plat/openal32.inc`: NASM import declarations for OpenAL.

## Project goals

- Keep utilities small, explicit, and easy to drop into other projects.
- Separate language-specific work by folder.    
- Separate helper includes from import-oriented includes when that distinction improves clarity.
- Prefer practical helpers over large abstractions.
- Document folder-local conventions close to the code when behavior is non-obvious.

## What to expect

- APIs are local to each utility or folder.
- Cross-folder compatibility is not implied.
- Examples, tests, and additional notes may live alongside the relevant utility instead of in a single central system.

## Examples

Examples of how to use the utilities in this repository can be found in [EXAMPLES.md](./EXAMPLES.md).

## Contributing

Contribution guidelines are in [CONTRIBUTING.md](./CONTRIBUTING.md).

When adding a new utility, keep the change focused and document what problem it solves, where it belongs, and any assumptions it makes about toolchains or platforms.

## License

This repository is dedicated to the public domain under CC0 1.0 Universal. See [LICENSE](./LICENSE.md).