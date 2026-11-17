# Pruebas

Este documento explica qué comprueban las pruebas automáticas del proyecto, cómo ejecutarlas y cómo añadir una nueva.

## Cómo ejecutarlas

Desde la carpeta raíz del repositorio:

```bash
make test
```

Compila y ejecuta todos los archivos `test/*.test.cpp`. Cada comprobación imprime su resultado, agrupado por tema, y al final hay un resumen:

```text
== keywords
  [ OK ] the word PLATE is a keyword
  [ OK ] the word BRAILLE is a keyword
  ...

== summary
  keywords                23/23
  numbers and unit        5/5
  ...
  total                   62/62
test: all tests passed
```

Cada línea `[ OK ]` o `[FAIL]` dice qué comportamiento se comprobó. Si todo pasa, el último mensaje es `test: all tests passed`.

Si DeSiGNAR no está en `/usr/local`, indica su prefijo, o defínelo una vez en `config.local.mk` (ver el [README](../README.md#configurar-la-ruta-de-designar)):

```bash
make test DESIGNAR_PREFIX=$HOME/.local
```

Para ejecutar una sola prueba:

```bash
make build/lexer.test
./build/lexer.test
```

Hay que ejecutarlas desde la raíz del proyecto, porque las pruebas leen los programas de `examples/` con rutas relativas.

### Cómo se ve un fallo

Una comprobación que falla se marca `[FAIL]` y la prueba continúa, de modo que una sola ejecución lista todos los problemas. En el resumen, el grupo muestra cuántas pasaron, y al final aparece la lista de las que fallaron. Por ejemplo, si la regla de espacios en blanco dejara de reconocer los saltos de línea:

```text
== whitespace and comments
  [FAIL] a line break is whitespace
  [FAIL] a tab is whitespace
  [FAIL] the letters n, t and r are identifiers, not whitespace
  ...

== summary
  whitespace and comments 1/8
  total                   52/62

failed checks:
  - whitespace and comments: a line break is whitespace
  - whitespace and comments: a tab is whitespace
  ...
test: build/lexer.test failed.
```

`make test` termina con error si alguna prueba falla, y el hook `pre-push` detiene el push en ese caso.

## Qué pruebas hay

| Archivo | Qué comprueba |
| --- | --- |
| `test/prerequisites.test.cpp` | que DeSiGNAR compila, enlaza y funciona con tu compilador y tus rutas: inserta y busca un valor en un `AVLTree`. No prueba código del proyecto |
| `test/lexer.test.cpp` | las reglas del analizador léxico, descritas abajo |

### Qué cubre `lexer.test.cpp`

| Grupo | Qué garantiza |
| --- | --- |
| Palabras reservadas | las 23 palabras reservadas se reconocen como `KW_<PALABRA>` y no como identificadores |
| Números y unidad | `50` y `0.6` son `NUMBER`; `50mm`, `0.6mm` y `50 mm` son un `NUMBER` seguido de `UNIT_MM` |
| Identificadores | `room` y `_x1` son `ID`; `PLATEX` es un solo `ID` (gana la coincidencia más larga); `plate` y `Content` son `ID` porque las palabras reservadas distinguen mayúsculas |
| Cadenas | el texto entre comillas es un `STRING`, también con tildes y `ñ`, y el lexema conserva todos los bytes |
| Delimitadores | `=`, `{`, `}`, `(`, `)`, `,` y `;` tienen su propio token, incluso sin espacios |
| Espacios y comentarios | los saltos de línea, tabuladores y retornos de carro son espacios; las letras `n`, `t` y `r` no lo son; `tokenize()` descarta espacios y comentarios, y `tokenize_raw()` los conserva |
| Caracteres de comentarios | un comentario acepta `'`, `$`, `%`, `&`, `@`, `<`, `>`, `[`, `]`, `^`, `~`, `` ` ``, `\`, `¿` y `¡`; una `"` o una `{` lo terminan; una cadena sigue rechazando esos símbolos |
| Errores léxicos | un carácter suelto (`@`), una cadena sin cerrar y un número terminado en punto lanzan `std::runtime_error` |
| Programas de ejemplo | cada archivo de `examples/` se analiza sin error y produce tokens |

Los casos de espacios en blanco existen por un error real: la regla `WS` llegó a escribirse con `\\n`, `\\t` y `\\r`, que DeSiGNAR interpreta como las letras `n`, `t` y `r`. Con ese error, cualquier archivo de varias líneas fallaba. Ver [`lexer.md`](lexer.md), sección 3.

## Cómo está hecha una prueba

Cada archivo `test/<nombre>.test.cpp` es un programa independiente con su propio `main()`. Devuelve `0` si todo pasa y `1` si algo falla, y eso es lo que `make test` comprueba. No se usa ninguna biblioteca de pruebas: `begin_group(título)` abre un grupo y imprime su encabezado, `check(condición, descripción)` imprime `[ OK ]` o `[FAIL]` con la descripción, y `print_summary()` imprime el resumen y devuelve cuántas fallaron.

Las pruebas del lexer siguen siempre la misma forma: se escanea un texto pequeño y se compara la lista de nombres de token con la esperada.

```cpp
check(scan(lexer, "PLATE {") == "KW_PLATE LBRACE",
      "a plate opens a block");
```

El `Makefile` enlaza cada prueba con los objetos del proyecto, salvo `main.o`, así que una prueba puede usar cualquier clase de `src/`.

## Cómo añadir una prueba

1. Para un caso nuevo del lexer, añade un `check(...)` a un grupo existente de `test/lexer.test.cpp`, o crea una función nueva que empiece con `begin_group("título")` y llámala desde `main()`.
2. La descripción dice **qué debe funcionar**, porque es lo que se imprime junto al resultado.
3. Para otro módulo, crea `test/<modulo>.test.cpp` con su propio `main()`. El `Makefile` lo encuentra solo, sin tocar nada más.
4. Ejecuta `make test`.

## Comprobar que una prueba detecta el error

Una prueba que nunca falla no protege nada. Para asegurarte de que detecta el problema que dice cubrir, rompe a propósito la regla en una **copia** del proyecto (nunca en tu rama de trabajo), ejecuta `make test` y comprueba que aparecen los fallos esperados. Por ejemplo, en la copia, escribe la regla `WS` de `src/lexer.cpp` con doble barra (`"\\n"`) y deben fallar las pruebas de saltos de línea y tabuladores.
