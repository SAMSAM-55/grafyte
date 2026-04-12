## Contributing to grafyte

Contributions to grafyte are welcome! By contributing to this project, you agree to follow the guidelines below.

### How to contribute

To contribute, you first have to fork this repository and create a Pull Request.

> [!IMPORTANT]
> PRs must target the `dev` branch and not the `main` branch. Any PR on the `main` branch will be rejected immediately
> without further review.

### Coding style

One of grafyte’s goals is to provide an ergonomic and modern game engine for Python. When making changes to the Python
API, please follow these principles to keep the API consistent and user-friendly:

- Prefer Pythonic interfaces
- Expose object attributes (e.g., position, size, color, etc.) as properties
- Prefer clear and explicit error messages, unless doing so would harm usability

For the C++ engine:

- Write self-documenting code whenever possible
- Add comments only when they provide meaningful clarification
- Follow the provided clang-format and clang-tidy configuration to maintain a consistent code style across the project

## Project philosophy

Here is a summary of the project's philosophy:

- clarity over cleverness
- consistency over micro-optimizations
- ergonomics over verbosity
