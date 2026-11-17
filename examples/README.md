# Ejemplos

Programas `.b3d` de prueba para el analizador léxico. La sintaxis del lenguaje está descrita en [`docs/syntax.md`](../docs/syntax.md).

| Archivo | Qué muestra |
| --- | --- |
| `health-center.b3d` | una placa individual: dimensiones con decimales, material `PLA`, orientación vertical y contenido en braille |
| `room-template.b3d` | una plantilla (`TEMPLATE`) reutilizada con `USE` por dos placas horizontales |
| `accessible-sign.b3d` | el material `PETG`, las posiciones `LEFT`, `RIGHT`, `TOP` y `BOTTOM`, y cadenas con tildes y `ñ` |

Para ejecutar uno:

```bash
make build
./build/braille3d examples/health-center.b3d
```

Los comentarios dentro de los archivos están en inglés, igual que el código. Los textos entre comillas están en español porque ejercitan las vocales acentuadas y la `ñ`.
