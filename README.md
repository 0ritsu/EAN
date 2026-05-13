# EAN – Elementy Analizy Numerycznej (Qt Widgets)

## Wymagania
- Qt 6 (lub Qt 5)
- CMake >= 3.20
- Kompilator C++17
- GMP/MPFR (dla mpreal/Interval) – do dołączenia w kolejnym kroku

## Struktura
- `src/gui` – GUI (Qt Widgets)
- `src/core` – logika parsowania, solver, log iteracji
- `src/interval` – **tu wklej Interval.h i mpreal.h**

## Budowanie
```bash
cmake -S . -B build
cmake --build build
```

## Następne kroki
1. Wklej pełne treści `Interval.h` i `mpreal.h` do `src/interval/`.
2. Uzupełnij parser wejścia (macierz + wektor w jednym polu).
3. Uzupełnij solver Choleskiego (arytmetyka zwykła i przedziałowa).
4. Dokończ dokumentację i przykłady wejścia.
