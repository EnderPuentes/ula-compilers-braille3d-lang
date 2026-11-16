
# Braille3D-Lang — Analizador Léxico (Implementación 1/3):

## Descripción del Analizador Léxico:

Esta primera entrega corresponde a la fase de **Análisis Léxico** del compilador estableciendo las **Palabras Reservadas (keywords)**,mediante la identificación de `KW_NAME` y las **Reglas léxicas**. 

El lenguaje distingue entre mayúsculas y minúsculas, las **Palabras reservadas** se definen en inglés (mayúsculas), mientras que las cadenas de texto (`STRING`) admiten caracteres propios del español (incluyendo la letra `ñ`/`Ñ` y vocales acentuadas).

### 1. Estructura de la Clase `lexer.h`:

El diseño del analizador léxico está encapsulado dentro del espacio de nombres `Braille3D`, ofreciendo una interfaz limpia y orientada a objetos en C++17:

* 1. **Métodos Públicos:**
    * `LexerB3D()`: Constructor encargado de invocar internamente a `setup_rules()` para inicializar todas las reglas del lenguaje.
    * `void setup_rules()`: Método interno responsable de registrar las expresiones regulares de palabras reservadas, identificadores, números y operadores.
    * `tokenize(const std::string& source_code)`: Procesa el código fuente, eliminando los espacios en blanco (`WS`) y comentarios (`COMMENT`) para entregar un flujo limpio destinado al Parser optimizando la construcción del **Árbol de Análisis Sintáctico (AST)**.
    * `tokenize_raw(const std::string& source_code)`: Devuelve la totalidad de los tokens (incluyendo espacios y comentarios), facilitando las tareas de depuración, permitiendo comprobar que cada carácter del archivo fuente está siendo reconocido por las expresiones regulares.

* 2. **Atributo Privado:**
    * `Designar::Lexer lexer_`: Objeto central de la biblioteca DeSiGNAR que gestiona el motor de autómatas. 
---
### 2. Definición de `lexer.cpp`:

El analizador léxico procesa el código fuente de los archivos `.b3d` y lo desglosa en los siguientes componentes reconocidos por el autómata:
* 1. **Auxiliares y Expresiones Regulares Base:**
Para mantener la modularidad y limpieza en el código fuente, se definieron constantes de expresiones regulares auxiliares antes de registrar los tokens:

|  Constante  |     Expresión   |             Descripción                    |
| :-----------| :-------------- | :----------------------------------------- |
| **`LOWER`** | `(a|b|c|...|z)` | Conjunto de letras minúsculas del alfabeto.|
| **`UPPER`** | `(A|B|C|...|Z)` | Conjunto de letras mayúsculas del alfabeto.|
| **`DIGIT`** | `(0|1|2|3|4|5|6|7|8|9)`| Dígitos numéricos individuales.     |
| **`MIX`** | `(LOWER \| UPPER \| _)`| Agrupa letras (mayúsculas/minúsculas) y el guion bajo. Base para los `ID`.|
| **`ALNUM`** | `(MIX \| DIGIT)` | Caracteres alfanuméricos combinados con guion bajo.|

* 2. **Palabras Reservadas y Unidades:**

| Token(Identificador en C++) | Palabra Reservada / Lexema | Descripción |
| :--------------------------- | :------------------------- | :-----------|
| **`KW_PLATE`** | `"PLATE"` | Define el bloque o elemento principal de una placa braille.|
| **`KW_BRAILLE`** | `"BRAILLE"` | Define una sección o contenido de celdas en sistema braille.|
| **`KW_TEXT`** | `"TEXT"` | Especifica una línea de texto plano o traducible.|
| **`KW_USE`** | `"USE"` | Importa o referencia plantillas o componentes externos.|
| **`KW_TEMPLATE`** | `"TEMPLATE"` | Define una plantilla base reutilizable para los diseños.|
| **`KW_WIDTH`** | `"WIDTH"` | Propiedad para configurar el ancho de la placa o elemento.|
| **`KW_HEIGHT`** | `"HEIGHT"` | Propiedad para configurar el alto de la placa o elemento.|
| **`KW_THICKNESS`** | `"THICKNESS"` | Especifica el grosor o profundidad del modelo 3D.|
| **`KW_MARGIN`** | `"MARGIN"` | Define el margen de separación en los bordes.|
| **`KW_RELIEF`** | `"RELIEF"` | Configura la altura del relieve de los puntos braille.|
| **`KW_MATERIAL`** | `"MATERIAL"` | Especifica el material de fabricación para la impresión 3D.|
| **`KW_POSITION`** | `"POSITION"` | Establece las coordenadas o posición de los elementos.|
| **`KW_ORIENTATION`** | `"ORIENTATION"` | Define la orientación espacial del diseño.|
| **`KW_PLA`** | `"PLA"` | Valor del material termoplástico PLA.|
| **`KW_PETG`** | `"PETG"` | Valor del material termoplástico PETG.|
| **`KW_CENTER`** | `"CENTER"` | Valor de alineación o posición central.|
| **`KW_LEFT`** | `"LEFT"` | Valor de alineación hacia la izquierda.|
| **`KW_RIGHT`** | `"RIGHT"` | Valor de alineación hacia la derecha.|
| **`KW_TOP`** | `"TOP"` | Valor de alineación o posición superior.|
| **`KW_BOTTOM`** | `"BOTTOM"` | Valor de alineación o posición inferior.|
| **`KW_HORIZONTAL`** | `"HORIZONTAL"` | Especifica orientación horizontal.|
| **`KW_VERTICAL`** | `"VERTICAL"` | Especifica orientación vertical.|
| **`UNIT_MM`** | `"mm"` | Token de unidad de medida en milímetros para las dimensiones.|

* 3. **Delimitadores, Asignación y Símbolos de Puntuación:**
| Token(Identificador en C++) | Lexema / Patrón | Descripción |
| :-------------------------- | :-------------- | :---------- |
| **`ASSIGN`** | `"="`   | Operador de asignación para propiedades o atributos.|
| **`LPAREN`** | `"\\("` | Paréntesis de apertura para parámetros o agrupaciones.|
| **`RPAREN`** | `"\\)"` | Paréntesis de cierre.|
| **`LBRACE`** | `"\\{"` | Llave de apertura para bloques de código o estructuras.|
| **`RBRACE`** | `"\\}"` | Llave de cierre de bloques.|
| **`COMMA`** | `","` | Coma para separar elementos en listas o parámetros.|
| **`SEMICOLON`** | `";"` | Delimitador de fin de sentencia o instrucción.|

* 4. **Elementos Omitidos:**
| Token (Identificador en C++) | Expresión Regular/ Patrón | Descripción |
| :--------------------------- | :------------------------ | :---------- |
| **`COMMENT`** | `"#" + comment_char + "*"`|Comentarios de una línea comienzan con `#` y admiten texto, espacios, símbolos como `\|`, tildes (`á`, `é`, `í`, `ó`, `ú`), tambien (`ñ`, `Ñ`), omitidos en el análisis estándar, pero accesibles mediante `tokenize_raw()`.|
| **`WS`** | `"( |\\t|\\n|\\r)( |\\t|\\n|\\r)*"` | Espacios en blanco, tabulaciones y saltos de línea ignorados por el lexer por el método `tokenize()`.|

* 5. **Tokens de Datos y Literales:**
| Token (Identificador) | Patrón / Expresión Regular | Descripción | Ejemplo |
| :---------------------| :--------------------------| :---------- | :------ |
| **`ID`** | `[a-zA-Z_][a-zA-Z0-9_]*`|Identificadores para nombres de componentes o referencias.| `centro_salud`|
| **`NUMBER`** |`[0-9]+(\.[0-9]+)?`| Parte Entera (`[0-9]+`) y Parte Decimal Opcional (`(\.[0-9]+)?`),Valores numéricos enteros (con soporte para *Maximal Munch*).| `50`, `3.3`|
| **`STRING`** | `"[^"]*"` | Literales de texto encerrados entre comillas dobles (incluye tildes, ñ y caracteres especiales en español).| `"Institución para Niños y Niñas"`|

* 6. **Flujo Limpio:**

Para evitar conflictos entre tokens con prefijos similares o patrones solapados, el analizador aplica dos principios fundamentales:

   * **Prioridad por Orden de Registro:** En `Designar::Lexer`, cuando un lexema cumple con múltiples reglas de igual longitud, prevalece la regla registrada primero. Por ello, todas las **Palabras Reservadas** y la unidad de medida `mm` se registran antes que la regla general de identificadores (`ID`), evitando que lexemas como `PLATE`, `PLA` o `mm` sean clasificados erróneamente como variables.

   * **Gestión de tokens contiguos por medio de (DeSiGNAR):** El análisis de expresiones como `50mm` se resuelve automáticamente, el lexer utiliza la estrategia de *Maximal Munch* (coincidencia más larga) para consumir todo el número posible antes de llegar con la unidad, delegando en la biblioteca la correcta delimitación entre `NUMBER` y `UNIT_MM`.
---
### 3. Adaptación a DeSiGNAR: 

La construcción de los autómatas finitos para el reconocimiento de patrones se realiza mediante la clase `Designar::Lexer`. La especificación formal planteada en los documentos de diseño previos, **no se modificó la gramática léxica ni el conjunto de palabras reservadas del lenguaje**. Únicamente se realizaron dos adaptaciones técnicas en la escritura interna de las expresiones regulares para garantizar compatibilidad total con el motor de autómatas de **DeSiGNAR**:

  * 1. **Expansión explícita de clases de caracteres:** El motor de expresiones regulares de DeSiGNAR construye los autómatas mediante operadores básicos de concatenación, alternancia (`|`) y cerradura de Kleene (`*`),los rangos numéricos y alfabéticos se expandieron explícitamente a (`(0|1|2|3|4|5|6|7|8|9)`,etc).

  * 2. **Soporte explícito de UTF-8 en cadenas:** Dado que DeSiGNAR no admite clases negadas tipo `[^"]`, el patrón de `STRING` enumera explícitamente los caracteres imprimibles, símbolos de puntuación y caracteres UTF-8 del español (`á, é, í, ó, ú, ü, ñ, Ñ`), asegurando la integridad del texto.
---
## 4. Pruebas del Lexer `main.cpp`:

En la implementación del programa principal (src/main.cpp) para mostrar el funcionamiento del Lexer tenemos:
   * 1. **Carga del Archivo Fuente (.b3d):** El programa recibe la ruta del archivo como argumento por consola (utiliza por defecto `examples/centro-salud.b3d`, si no se especifica uno) y lee su contenido completo mediante `std::stringstream`. 

   * 2. **Inicialización Automática del Lexer:** Se instancia el objeto `Braille3D::LexerB3D lexer;` el cual ejecuta automáticamente `setup_rules()` en su constructor para cargar todas las reglas y prioridades léxicas.

   * 3. **Generación e Impresión del Flujo de Tokens:** Se invoca `lexer.tokenize(code)` para escanear el contenido aplicando `Maximal Munch` y filtrando `WS` y `COMMENT`. Luego, se recorre el `Designar::DynArray<Designar::Token>` resultante para mostrar en consola el índice, nombre del (`TOKEN`) y (`LEXEMA`) de cada unidad encontrada.   
   * 4. **Validación y Notificación Léxica:** Mediante un bloque try-catch, el programa evalúa el escaneo. Si no se detectan anomalías, se notifica que el archivo no posee errores léxicos y el flujo es válido para el Parser; de lo contrario, captura y reporta el error léxico o de lectura de archivo. 
---

## 5. Ejecucion de los ejemplos de prueba:
**Nota: En caso de querer modificarlos leer el README correspondiente a la carpeta(examples):**
   * 1. **centro-salud.b3d:**Ejemplo de una placa individual que configura dimensiones físicas con decimales, material termoplástico, orientación vertical y contenido en sistema braille.
      * **Objetivo en el lexer:**Valida el reconocimiento de identificadores (`ID`), números enteros y decimales (`NUMBER`), la unidad de medida (`UNIT_MM`), palabras reservadas de posicionamiento (`VERTICAL`, `CENTER`) y literales de cadena con acentos en español (`STRING`).
    * 2. **plantilla-sala.b3d:**Ejemplo avanzado que demuestra la modularidad del lenguaje mediante la definición y reutilización de plantillas base.
      * **Propósito en el Lexer:** Verifica el correcto procesamiento de las palabras clave de estructuración y herencia (`TEMPLATE`, `USE`), además de bloques delimitados por llaves (`LBRACE`, `RBRACE`) y el operador de asignación (`ASSIGN`) y de igual manera los objetivos del ejemplo anterior.

Compilar el proyecto y probar el analizador léxico con el archivo de ejemplo (`examples/centro-salud.b3d`).

* **1. Requisitos Previos:**
   * Un compilador de C++ compatible con **C++17** (como GCC / MinGW, Clang o MSVC).
   * **CMake** (versión 3.14 o superior).

* **2. Pasos para Compilar:**
Abre una terminal en la carpeta raíz del repositorio (`Braille3D-Lang/`) y ejecuta los siguientes comandos según tu sistema operativo:

* **En Windows (Usando MinGW / MSYS2):**
  
  * **1. Configurar el proyecto especificando el generador de MinGW**
  cmake -B build -G "MinGW Makefiles"
  * **2. Compilar el proyecto de forma limpia**
  cmake --build build
  .\build\Braille3D.exe examples/centro-salud.b3d

* **Linux / macOS:**
  
  * **1. Configurar el proyecto**
  cmake -B build
  * **2. Compilar el proyecto**
  cmake --build build
  ./build/Braille3D examples/centro-salud.b3d



## 6. Estructura del Directorio :

Braille3D-Lang/
├── CMakeLists.txt          
├── README.md               
├── DeSiGNAR-main           
├── include/
│   └── LexerB3D.hpp        
├── src/
│   ├── LexerB3D.cpp        
│   └── main.cpp           
└── examples/
    ├── centro-salud.b3d    
    └── plantilla-sala.b3d  
    └── README.md 

|    Directorio    |       Descripción     |
| :--------------- | :---------------------|
| **DeSiGNAR-main**| Directorio que contiene el código fuente, autómatas y utilidades de la biblioteca DeSiGNAR utilizada para el soporte del análisis léxico.|
| **include** | Archivos de cabecera (**.hpp**), Declara la clase `LexerB3D` en `LexerB3D.hpp` encargada de procesar el código fuente.|
| **src**    | Contiene todas las expresiones regulares configuradas con la sintaxis de DeSiGNAR para reconocer palabras reservadas, identificadores, números, la unidad, cadenas de texto y comentarios, además de la lógica para filtrar los espacios en blanco (`WS`), mediante `LexerB3D.cpp`.|
| **examples** | Programas de ejemplo y pruebas de funcionamiento del Analizador Léxico.|

**CMakeLists.txt**: Configura el proyecto en C++17, vincula los fuentes de DeSiGNAR y compila el ejecutable del lexer.

---
## Licencia
Consulta los autores y términos de uso académico del proyecto en [LICENSE.md](../../LICENSE.md).