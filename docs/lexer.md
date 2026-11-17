# Braille3D-Lang — Analizador Léxico (Implementación 1/3)

## Descripción del Analizador Léxico

Esta primera entrega corresponde a la fase de **Análisis Léxico** del compilador, estableciendo las **Palabras Reservadas (keywords)**, mediante la identificación de `KW_NAME`, y las **Reglas léxicas**.

El lenguaje distingue entre mayúsculas y minúsculas. Las **Palabras Reservadas** se definen en inglés (mayúsculas), mientras que las cadenas de texto (`STRING`) admiten caracteres propios del español (incluyendo la letra `ñ`/`Ñ` y las vocales acentuadas).

### 1. Estructura de la Clase `lexer.hpp`

El diseño del analizador léxico está encapsulado dentro del espacio de nombres `Braille3D`, ofreciendo una interfaz limpia y orientada a objetos en C++17. La declaración está en `include/braille3d/lexer.hpp`. La cabecera vive dentro de `braille3d/` para que nunca se confunda con las cabeceras de DeSiGNAR que tienen el mismo nombre (`lexer.hpp`, `parser.hpp`, `grammar.hpp`).

1. **Métodos Públicos:**
    * `LexerB3D()`: Constructor encargado de invocar internamente a `setup_rules()` para inicializar todas las reglas del lenguaje.
    * `void setup_rules()`: Método responsable de registrar las expresiones regulares de palabras reservadas, identificadores, números y operadores.
    * `tokenize(const std::string& source_code)`: Procesa el código fuente, eliminando los espacios en blanco (`WS`) y los comentarios (`COMMENT`), para entregar un flujo limpio destinado al Parser y optimizar la construcción del **Árbol de Análisis Sintáctico (AST)**.
    * `tokenize_raw(const std::string& source_code)`: Devuelve la totalidad de los tokens (incluyendo espacios y comentarios), facilitando las tareas de depuración y permitiendo comprobar que cada carácter del archivo fuente es reconocido por las expresiones regulares.

2. **Atributo Privado:**
    * `Designar::Lexer lexer_`: Objeto central de la biblioteca DeSiGNAR que gestiona el motor de autómatas.

---

### 2. Definición de `lexer.cpp`

El analizador léxico procesa el código fuente de los archivos `.b3d` y lo desglosa en los siguientes componentes reconocidos por el autómata:

1. **Auxiliares y Expresiones Regulares Base:**
Para mantener la modularidad y la limpieza en el código fuente, se definieron constantes de expresiones regulares auxiliares antes de registrar los tokens:

| Constante   | Expresión                | Descripción                                                                  |
| :---------- | :----------------------- | :--------------------------------------------------------------------------- |
| **`LOWER`** | `(a\|b\|c\|...\|z)`      | Conjunto de letras minúsculas del alfabeto.                                  |
| **`UPPER`** | `(A\|B\|C\|...\|Z)`      | Conjunto de letras mayúsculas del alfabeto.                                  |
| **`DIGIT`** | `(0\|1\|2\|...\|9)`      | Dígitos numéricos individuales.                                              |
| **`MIX`**   | `(LOWER \| UPPER \| _)`  | Agrupa letras (mayúsculas/minúsculas) y el guion bajo. Base para los `ID`.   |
| **`ALNUM`** | `(MIX \| DIGIT)`         | Caracteres alfanuméricos combinados con guion bajo.                          |

2. **Palabras Reservadas y Unidades:**

| Token (identificador en C++) | Palabra Reservada / Lexema | Descripción                                                  |
| :--------------------------- | :------------------------- | :----------------------------------------------------------- |
| **`KW_PLATE`**       | `"PLATE"`       | Define el bloque o elemento principal de una placa braille.          |
| **`KW_BRAILLE`**     | `"BRAILLE"`     | Define una sección o contenido de celdas en sistema braille.         |
| **`KW_TEXT`**        | `"TEXT"`        | Especifica una línea de texto plano o traducible.                    |
| **`KW_USE`**         | `"USE"`         | Importa o referencia plantillas o componentes externos.              |
| **`KW_TEMPLATE`**    | `"TEMPLATE"`    | Define una plantilla base reutilizable para los diseños.             |
| **`KW_WIDTH`**       | `"WIDTH"`       | Propiedad para configurar el ancho de la placa o elemento.           |
| **`KW_HEIGHT`**      | `"HEIGHT"`      | Propiedad para configurar el alto de la placa o elemento.            |
| **`KW_THICKNESS`**   | `"THICKNESS"`   | Especifica el grosor o profundidad del modelo 3D.                    |
| **`KW_MARGIN`**      | `"MARGIN"`      | Define el margen de separación en los bordes.                        |
| **`KW_RELIEF`**      | `"RELIEF"`      | Configura la altura del relieve de los puntos braille.               |
| **`KW_MATERIAL`**    | `"MATERIAL"`    | Especifica el material de fabricación para la impresión 3D.          |
| **`KW_POSITION`**    | `"POSITION"`    | Establece las coordenadas o la posición de los elementos.            |
| **`KW_ORIENTATION`** | `"ORIENTATION"` | Define la orientación espacial del diseño.                           |
| **`KW_CONTENT`**     | `"CONTENT"`     | Define el contenido (texto) de un bloque `TEXT` o `BRAILLE`.         |
| **`KW_PLA`**         | `"PLA"`         | Valor del material termoplástico PLA.                                |
| **`KW_PETG`**        | `"PETG"`        | Valor del material termoplástico PETG.                               |
| **`KW_CENTER`**      | `"CENTER"`      | Valor de alineación o posición central.                              |
| **`KW_LEFT`**        | `"LEFT"`        | Valor de alineación hacia la izquierda.                              |
| **`KW_RIGHT`**       | `"RIGHT"`       | Valor de alineación hacia la derecha.                                |
| **`KW_TOP`**         | `"TOP"`         | Valor de alineación o posición superior.                             |
| **`KW_BOTTOM`**      | `"BOTTOM"`      | Valor de alineación o posición inferior.                             |
| **`KW_HORIZONTAL`**  | `"HORIZONTAL"`  | Especifica orientación horizontal.                                   |
| **`KW_VERTICAL`**    | `"VERTICAL"`    | Especifica orientación vertical.                                     |
| **`UNIT_MM`**        | `"mm"`          | Token de unidad de medida en milímetros para las dimensiones.        |

3. **Delimitadores, Asignación y Símbolos de Puntuación:**

| Token (identificador en C++) | Lexema / Patrón | Descripción                                                |
| :--------------------------- | :-------------- | :--------------------------------------------------------- |
| **`ASSIGN`**    | `"="`   | Operador de asignación para propiedades o atributos.            |
| **`LPAREN`**    | `"\\("` | Paréntesis de apertura para parámetros o agrupaciones.          |
| **`RPAREN`**    | `"\\)"` | Paréntesis de cierre.                                           |
| **`LBRACE`**    | `"\\{"` | Llave de apertura para bloques de código o estructuras.         |
| **`RBRACE`**    | `"\\}"` | Llave de cierre de bloques.                                     |
| **`COMMA`**     | `","`   | Coma para separar elementos en listas o parámetros.             |
| **`SEMICOLON`** | `";"`   | Delimitador de fin de sentencia o instrucción.                  |

4. **Elementos Omitidos:**

| Token (identificador en C++) | Expresión Regular / Patrón | Descripción |
| :--------------------------- | :------------------------- | :---------- |
| **`COMMENT`** | `"#" + comment_char + "*"` | Los comentarios de una línea comienzan con `#` y admiten los mismos caracteres que `STRING` (texto, espacios, símbolos como `\|` y tildes `á`, `é`, `í`, `ó`, `ú`, también `ñ` y `Ñ`), sin las comillas dobles: una `"` termina el comentario. Se omiten en el análisis estándar, pero son accesibles mediante `tokenize_raw()`. |
| **`WS`**      | `"( \|\t\|\n\|\r)( \|\t\|\n\|\r)*"` | Espacios en blanco, tabulaciones y saltos de línea, ignorados por el lexer en el método `tokenize()`. |

5. **Tokens de Datos y Literales:**

| Token (identificador) | Patrón / Expresión Regular | Descripción | Ejemplo |
| :-------------------- | :------------------------- | :---------- | :------ |
| **`ID`**     | `[a-zA-Z_][a-zA-Z0-9_]*` | Identificadores para nombres de componentes o referencias. | `centro_salud` |
| **`NUMBER`** | `[0-9]+(\.[0-9]+)?`      | Parte entera (`[0-9]+`) y parte decimal opcional (`(\.[0-9]+)?`). Valores numéricos enteros o decimales (con soporte para *Maximal Munch*). | `50`, `3.3` |
| **`STRING`** | `"[^"]*"`                | Literales de texto encerrados entre comillas dobles (incluye tildes, `ñ` y caracteres especiales del español). El patrón se muestra de forma conceptual: en el código los caracteres permitidos se enumeran explícitamente (ver la sección 3). | `"Institución para Niños y Niñas"` |

6. **Flujo Limpio:**

Para evitar conflictos entre tokens con prefijos similares o patrones solapados, el analizador aplica dos principios fundamentales:

* **Prioridad por orden de registro:** En `Designar::Lexer`, cuando un lexema cumple con varias reglas de igual longitud, prevalece la regla registrada primero. Por ello, todas las **Palabras Reservadas** y la unidad de medida `mm` se registran antes que la regla general de identificadores (`ID`), evitando que lexemas como `PLATE`, `PLA` o `mm` sean clasificados erróneamente como variables.

* **Gestión de tokens contiguos por medio de DeSiGNAR:** El análisis de expresiones como `50mm` se resuelve automáticamente: el lexer utiliza la estrategia de *Maximal Munch* (coincidencia más larga) para consumir todo el número posible antes de llegar a la unidad, delegando en la biblioteca la correcta delimitación entre `NUMBER` y `UNIT_MM`.

---

### 3. Adaptación a DeSiGNAR

La construcción de los autómatas finitos para el reconocimiento de patrones se realiza mediante la clase `Designar::Lexer`. **No se modificó la gramática léxica ni el conjunto de palabras reservadas** de la especificación formal planteada en los documentos de diseño previos. Únicamente se realizaron tres adaptaciones técnicas en la escritura interna de las expresiones regulares, para garantizar la compatibilidad total con el motor de autómatas de **DeSiGNAR**:

1. **Expansión explícita de clases de caracteres:** El motor de expresiones regulares de DeSiGNAR construye los autómatas mediante operadores básicos de concatenación, alternancia (`|`) y cerradura de Kleene (`*`). Los rangos numéricos y alfabéticos se expandieron explícitamente a `(0|1|2|3|4|5|6|7|8|9)`, etc.

2. **Soporte explícito de UTF-8 en cadenas:** Dado que DeSiGNAR no admite clases negadas del tipo `[^"]`, el patrón de `STRING` enumera explícitamente los caracteres permitidos, asegurando la integridad del texto: letras y dígitos, el espacio, los símbolos `- _ . , : ; ! ? ( ) / + * = # |` y los caracteres UTF-8 del español (`á é í ó ú Á É Í Ó Ú ñ Ñ ü Ü`). Una cadena contiene solo esos caracteres; cualquier otro dentro de las comillas es un error léxico.

3. **Secuencias de escape:** En las expresiones regulares de DeSiGNAR, la barra invertida (`\`) solo convierte en literal el carácter que la sigue (por ejemplo `\(`, `\{` o `\.`). El motor no define las secuencias `\n`, `\t` ni `\r`: `\n` es la letra `n`. Por eso la regla `WS` se escribe en C++ con una sola barra, `"\n"`, de modo que el compilador inserte el carácter real de salto de línea. Escrita como `"\\n"`, la regla recibiría la letra `n` y no reconocería los saltos de línea ni los tabuladores.

---

### 4. Pruebas del Lexer con `main.cpp`

En la implementación del programa principal (`src/main.cpp`), para mostrar el funcionamiento del Lexer:

1. **Carga del archivo fuente (`.b3d`):** El programa recibe la ruta del archivo como argumento por consola (utiliza por defecto `examples/health-center.b3d` si no se especifica uno) y lee su contenido completo mediante `std::stringstream`.

2. **Inicialización automática del Lexer:** Se instancia el objeto `const Braille3D::LexerB3D lexer;`, el cual ejecuta automáticamente `setup_rules()` en su constructor para cargar todas las reglas y prioridades léxicas.

3. **Generación e impresión del flujo de tokens:** Se invoca `lexer.tokenize(code)` para escanear el contenido aplicando *Maximal Munch* y filtrando `WS` y `COMMENT`. Luego se recorre el `Designar::DynArray<Designar::Token>` resultante para mostrar en consola el índice, el nombre del `TOKEN` y el `LEXEMA` de cada unidad encontrada.

4. **Validación y notificación léxica:** Mediante un bloque `try-catch`, el programa evalúa el escaneo. Si no se detectan anomalías, se notifica que el archivo no posee errores léxicos y que el flujo es válido para el Parser; de lo contrario, captura y reporta el error léxico o de lectura de archivo.

### 5. Pruebas automáticas

El lexer tiene sus pruebas en `test/lexer.test.cpp`. Comprueban las palabras reservadas, los números y la unidad, los identificadores, las cadenas con tildes y `ñ`, los delimitadores, los espacios y comentarios, los errores léxicos y que los programas de `examples/` se analicen sin error. Se ejecutan con `make test`. La guía completa, con cómo ejecutar una sola prueba y cómo añadir otra, está en [`testing.md`](testing.md).


---

## Ejecución de los ejemplos de prueba

Los archivos están en la carpeta `examples/` (ver su [README](../examples/README.md)).

1. **`health-center.b3d`:** Ejemplo de una placa individual que configura dimensiones físicas con decimales, material termoplástico, orientación vertical y contenido en sistema braille.
    * **Objetivo en el lexer:** Valida el reconocimiento de identificadores (`ID`), números enteros y decimales (`NUMBER`), la unidad de medida (`UNIT_MM`), palabras reservadas de posicionamiento (`VERTICAL`, `CENTER`) y literales de cadena (`STRING`).

2. **`room-template.b3d`:** Ejemplo avanzado que demuestra la modularidad del lenguaje mediante la definición y reutilización de plantillas base.
    * **Propósito en el lexer:** Verifica el correcto procesamiento de las palabras clave de estructuración y herencia (`TEMPLATE`, `USE`), además de los bloques delimitados por llaves (`LBRACE`, `RBRACE`) y el operador de asignación (`ASSIGN`), y de igual manera los objetivos del ejemplo anterior.

3. **`accessible-sign.b3d`:** Ejemplo con el material `PETG`, las posiciones `LEFT`, `RIGHT`, `TOP` y `BOTTOM`, y cadenas con vocales acentuadas y `ñ`.
    * **Propósito en el lexer:** Valida las palabras reservadas que los otros ejemplos no usan y el soporte de los caracteres del español en `STRING`.

### Compilar y ejecutar

* **Requisitos previos:**
    * Un compilador de C++ compatible con **C++17** (`g++`).
    * **GNU `make`** y la biblioteca **DeSiGNAR** instalada, según la guía del [README](../README.md).

* **Pasos:** Abre una terminal en la carpeta raíz del repositorio y ejecuta:

```bash
make build
./build/braille3d examples/health-center.b3d
```

---

## Estructura del Directorio

```text
ula-compilers-braille3d-lang/
├── Makefile
├── config.mk
├── README.md
├── include/
│   └── braille3d/
│       └── lexer.hpp
├── src/
│   ├── lexer.cpp
│   └── main.cpp
├── test/
│   ├── prerequisites.test.cpp
│   └── lexer.test.cpp
├── examples/
│   ├── health-center.b3d
│   ├── room-template.b3d
│   ├── accessible-sign.b3d
│   └── README.md
└── docs/
    ├── lexer.md
    ├── syntax.md
    └── testing.md
```

| Directorio   | Descripción |
| :----------- | :---------- |
| **include**  | Archivos de cabecera (`.hpp`). Declara la clase `LexerB3D` en `include/braille3d/lexer.hpp`, encargada de procesar el código fuente. |
| **src**      | Contiene todas las expresiones regulares configuradas con la sintaxis de DeSiGNAR para reconocer palabras reservadas, identificadores, números, la unidad, cadenas de texto y comentarios, además de la lógica para filtrar los espacios en blanco (`WS`), mediante `lexer.cpp`. |
| **examples** | Programas de ejemplo y pruebas de funcionamiento del Analizador Léxico. |

**Makefile:** Configura el proyecto en C++17 y compila el ejecutable del lexer. DeSiGNAR no se incluye en el repositorio: es un requisito del sistema.

---

## Licencia

Consulta los autores y los términos de uso académico del proyecto en [LICENSE.md](../LICENSE.md).
