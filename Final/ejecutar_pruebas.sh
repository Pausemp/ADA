#!/bin/bash
# Script para compilar y ejecutar todas las pruebas de maze_bb

echo "=== Compilando el código ==="
# Intentamos compilar con g++ (o g++-13 si g++ por defecto no soporta C++11)
if command -v g++-13 &> /dev/null; then
    make CXX=g++-13 clean
    make CXX=g++-13
else
    make clean
    make
fi

if [ ! -f maze_bb ]; then
    echo "Error: No se pudo generar el ejecutable maze_bb"
    exit 1
fi

echo ""
echo "=== Ejecutando pruebas ==="
tests=("00" "01" "02" "03" "04" "05" "06" "07" "08" "09" "10" "11" "12" "13")
failed=0

for t in "${tests[@]}"; do
    maze_file="maze-BB-testfiles/${t}-bb.maze"
    sol_file="maze-BB-testfiles/${t}-bb.maze.sol_bb"
    
    if [ ! -f "$maze_file" ] || [ ! -f "$sol_file" ]; then
        echo "Prueba ${t}: FALTAN ARCHIVOS DE PRUEBA"
        continue
    fi
    
    # Obtener la longitud esperada (primera línea del archivo .sol_bb)
    expected_len=$(head -n 1 "$sol_file" | tr -d '\r' | xargs)
    
    # Ejecutar nuestro solver
    actual_out=($(./maze_bb -f "$maze_file"))
    actual_len=$(echo "${actual_out[0]}" | tr -d '\r' | xargs)
    actual_time=$(echo "${actual_out[2]}" | tr -d '\r' | xargs)
    
    if [ "$expected_len" == "$actual_len" ]; then
        echo "Prueba ${t}: OK (Longitud: $actual_len, Tiempo: $actual_time ms)"
    else
        echo "Prueba ${t}: ERROR (Esperado: $expected_len, Obtenido: $actual_len)"
        failed=$((failed + 1))
    fi
done

echo ""
if [ $failed -eq 0 ]; then
    echo "¡Todas las pruebas estándar pasaron con éxito!"
else
    echo "Hubo $failed pruebas fallidas."
fi
