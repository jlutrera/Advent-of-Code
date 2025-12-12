# 🎄 Advent of Code

Este repositorio contiene mis soluciones a los retos de **[Advent of Code](https://adventofcode.com/)**, un evento anual de programación que se celebra cada diciembre.  
Cada día se publica un nuevo problema con dos partes, que ponen a prueba la lógica, la creatividad y la capacidad de optimización.

---

## 📂 Estructura del repositorio

        AdventOfCode/
        ├── 2024/
        │     ├── D01/
        │     │    ├── reto1.c
        │     │    ├── reto2.c
        │     │    ├── input.txt
        │     │    └── subject.txt
        │     ├── D02/
        │     │    ├── ...
        │     └── ... 
        ├── 2025/
        │     ├── D01/
        │     └── ...
        └── readme.md

Código

- Cada carpeta corresponde a un **año**.
- Dentro de cada año, hay subcarpetas por **día** (`D01`, `D02`, ...).
- En cada día encontrarás:
  - Código fuente de las soluciones (hechos en C, C++ y Python).
  - Un `subject.txt` con la descripción del problema y notas de implementación.
  - Un `input.txt`con la entrada propuesta en el problema.

---

## 🚀 Cómo ejecutar las soluciones

Ejemplo en C:
```bash
gcc reto1.c -o reto1
./reto1 < input.txt
```

Ejemplo en C++:
```bash
g++ reto1.cpp -o reto1
./reto1 < input.txt
```

Ejemplo en Python:
```bash
python3 reto1.py < input.txt
```
