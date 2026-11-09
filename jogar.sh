#!/usr/bin/env bash
set -euo pipefail

project_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
cd -- "$project_dir"

binary="bin/Release/Xadrez3D"
sources=(main.cpp src/*.cpp)
inputs=("${sources[@]}" include/*.h jogar.sh Xadrez3D.cbp)
needs_build=false

if [[ ! -x "$binary" ]]; then
    needs_build=true
else
    for input in "${inputs[@]}"; do
        if [[ "$input" -nt "$binary" ]]; then
            needs_build=true
            break
        fi
    done
fi

flags=(-std=c++14 -Wall -Wextra -Wpedantic -O2 -DNDEBUG -Iinclude -Ilibs/glm)
if [[ "$(uname)" == Darwin ]]; then
    # macOS: bibliotecas do Homebrew (brew install glfw glew glm).
    compiler=clang++
    prefix="$(brew --prefix 2>/dev/null || echo /opt/homebrew)"
    flags+=(-DGL_SILENCE_DEPRECATION -I"$prefix/include")
    libs=(-L"$prefix/lib" -lglfw -lGLEW -framework OpenGL)
    missing='Instale o Xcode Command Line Tools: xcode-select --install'
else
    compiler=g++
    libs=(-lglfw -lGLEW -lGL)
    missing='Instale o pacote base-devel.'
fi

if [[ "$needs_build" == true ]]; then
    if ! command -v "$compiler" >/dev/null 2>&1; then
        printf 'Compilador %s não encontrado. %s\n' "$compiler" "$missing" >&2
        exit 1
    fi

    printf 'Compilando Xadrez 3D...\n'
    mkdir -p bin/Release
    "$compiler" "${flags[@]}" "${sources[@]}" -o "$binary" "${libs[@]}"
fi

exec "$project_dir/$binary"
