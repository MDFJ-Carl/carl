/* SPDX-License-Identifier: GPL-2.0-only 
* Carl-OS/.sh/build-electron.sh
* /* Copyright (C) MDFJ 2026
* MDFJ Project: CarlOS (Part 3 of Carl)
* License: GNU General Public License v2.0. */

# !/bin/bash

# ============================================================
# Carl OS v12.02 LTS — Script de construcción Electron
# Ejecuta este script en tu computadora local (no en Replit).
#
# Requisitos previos:
#   - Node.js 18+  (https://nodejs.org)
#   - GCC          (Linux: sudo apt install gcc | Mac: xcode-select --install | Windows: mingw-w64)
#
# Uso:
#   chmod +x build-electron.sh
#   ./build-electron.sh
# ============================================================

set -e

echo "=================================================="
echo "  Carl OS v12.02 LTS - Electron Builder"
echo "=================================================="

# 1. Instalar dependencias
echo ""
echo "[1/4] Instalando dependencias npm..."
npm install

# 2. Verificar GCC
echo ""
echo "[2/4] Verificando GCC..."
if ! command -v gcc &> /dev/null; then
  echo "ERROR: GCC no encontrado."
  echo "  Linux:   sudo apt install gcc"
  echo "  Mac:     xcode-select --install"
  echo "  Windows: instalar MinGW-w64"
  exit 1
fi
echo "OK - $(gcc --version | head -1)"

# 3. Compilar main.c
echo ""
echo "[3/4] Compilando main.c..."
gcc main.c -o main
echo "OK - binario 'main' generado"

# 4. Empaquetar con electron-builder
echo ""
echo "[4/4] Empaquetando aplicación Electron..."
echo "  Esto puede tardar varios minutos la primera vez."
npx electron-builder --config electron-builder.json

echo ""
echo "=================================================="
echo "  BUILD COMPLETO"
echo "  Los instaladores están en la carpeta: dist/"
echo "  - Windows: dist/Carl-OS-Setup-12.02.exe"
echo "  - Linux:   dist/Carl-OS-12.02.AppImage"
echo "  - Mac:     dist/Carl-OS-12.02.dmg"
echo "=================================================="
