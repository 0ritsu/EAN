# EAN – Elementy Analizy Numerycznej (Qt Widgets)

## Wymagania
- Qt 6 (lub Qt 5)
- CMake >= 3.20
- Kompilator C++17
- GMP/MPFR (dla mpreal/Interval)

## Struktura
- `src/gui` – GUI (Qt Widgets)
- `src/core` – logika parsowania, solver, log iteracji
- `src/interval` – **tu wklej Interval.h i mpreal.h**

## Budowanie
```bash
cmake -S . -B build
cmake --build build
```
