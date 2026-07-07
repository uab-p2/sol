# Plantilla de solución de quest

Esta plantilla permite generar soluciones a un quest del proyecto SOL.

Esta descripción puede usarse como ayuda para entender la solución mostrada.

* `main.cpp` puede usarse para mostrar un ejemplo de solución.

* `test.cpp` puede contener tests Catch2 que generan resultados interpretables por gradescope.

  ```cpp
  // Test weight between brackets (interpreted outside catch2)
  TEST_CASE("Test name", "[1]") {
      REQUIRE(2 + 2 == 4);
  }

  // Test with partial valgrind weight: [4][vg0.75] means 75% of the [4] score depends on valgrind
  // If regular test passes but valgrind fails: score = 4 * (1 - 0.75) = 1
  // If both pass: score = 4
  TEST_CASE("Partial valgrind scoring", "[4][vg0.75]") {
      // Allocate but don't delete - will fail valgrind but test passes
      char* buffer = new char[1024];
      REQUIRE(buffer != nullptr);
      // Intentional: memory not freed, so valgrind will report a leak
  }
  ```

* La herramienta `gradescope/pack_coderunner.py` permite (re)generar el
  coderunner para corrección automática con gradescope.
  **Importante**: siempre configurar el assignment the gradescope con un máximo de 10 puntos
  (el sistema pondera según los pesos de cada test en test.cpp).

---
- Campaña principal: [&leftarrow;](/auto/campaign_solucion)
- [Código](https://github.com/uab-p2/sol/tree/main//quest/solution_template)
