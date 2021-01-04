# DepthAI-Core Documentation

The site is powered by [Sphinx](https://github.com/sphinx-doc/sphinx).

## Setup

```
./install_dependencies.sh
```

## Dependencies

- CMake v3.18.2
- Sphinx v4

## Running the local web server

```
make html
cd build/html
python3 -m http.server
```

The server runs at http://127.0.0.1:8000.
