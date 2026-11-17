## Braille3D-Lang: Guía de Sintaxis para Escribir Archivos `.b3d`

Para redactar correctamente un archivo de diseño en `Braille3D-Lang`, se deben seguir las reglas sintácticas y léxicas del lenguaje.

* **Estructura de Placa (`PLATE`):** Define el bloque principal de la placa. Sus atributos internos deben terminar obligatoriamente con punto y coma (`;`).
* **Bloques Internos (`TEXT` y `BRAILLE`):** Permiten configurar el contenido visual y el relieve táctil en Braille mediante atributos como `CONTENT`, `POSITION`, `ORIENTATION` y `RELIEF`.
* **Reutilización (`TEMPLATE` y `USE`):** Permiten definir plantillas base con propiedades físicas comunes y heredarlas en diferentes placas.
* **Restricciones y Reglas:**
  * **Comentarios (`COMMENT`):** Comienzan con `#` y son omitidos por el compilador. Admiten texto, espacios, símbolos (como `|`), tildes (`á`, `é`, `í`, `ó`, `ú`) y eñes (`ñ`, `Ñ`).
  * **Dimensiones y Unidades:** Todas las dimensiones numéricas deben incluir de forma estricta la unidad **`mm`** (ej. `50mm`, `0.6mm`).
  * **Cadenas de Texto (`STRING`):** Soportan de forma nativa las tildes y la letra Ñ (ejemplo: `"Institución De Salud"`, `"FARMACIA PARA NIÑOS"`).
  * **Bloques y Llaves:** Cada bloque (`PLATE`, `TEXT`, `BRAILLE`, `TEMPLATE`) se abre con `{` y se cierra con `}`, y cada sentencia interna termina con punto y coma (`;`).

---

### Plantilla Base de Archivos `.b3d`

#### Placa individual con texto vertical y Braille

```text
# Base plate
PLATE {
    # Dimensions
    WIDTH = 50mm;
    HEIGHT = 120mm;
    THICKNESS = 4mm;
    MARGIN = 10mm;
    MATERIAL = PLA;

    # Visible text
    TEXT {
        CONTENT = "BASE";
        POSITION = CENTER;
        ORIENTATION = VERTICAL;
    }

    # Braille relief
    BRAILLE {
        CONTENT = "BASE";
        RELIEF = 0.6mm;
        POSITION = CENTER;
    }
}
```

Los programas completos están en la carpeta [`examples/`](../examples/README.md).

---

## Licencia

Para consultar los autores y los términos de uso académico del proyecto **Braille3D-Lang**, revisa [LICENSE.md](../LICENSE.md).
