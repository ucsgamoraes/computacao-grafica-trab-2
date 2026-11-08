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

if [[ "$needs_build" == true ]]; then
    if ! command -v g++ >/dev/null 2>&1; then
        printf 'Compilador g++ não encontrado. Instale o pacote base-devel.\n' >&2
        exit 1
    fi

    printf 'Compilando Xadrez 3D...\n'
    mkdir -p bin/Release
    g++ -std=c++14 -Wall -Wextra -Wpedantic -O2 -DNDEBUG -Iinclude \
        "${sources[@]}" -o "$binary" -lglfw -lGLEW -lGL
fi

exec "$project_dir/$binary"
