# PA04_Computaci-n-Gr-fica

# Producto Académico 4: Desarrollo de un Editor de Escenas 3D

Este proyecto corresponde a la implementación del Producto Académico 4 (Evaluación Final) de la asignatura Computación Gráfica. Consiste en el diseño e implementación de un Editor de Escenas Tridimensionales interactivo (*RoboLab 3D Scene Editor*) desarrollado en C++17 utilizando OpenGL 2.0 y FreeGLUT, estructurado modularmente mediante CMake. A diferencia de entregas previas, el sistema no presenta una escena preprogramada fija; la aplicación inicia con un espacio de trabajo vacío y una rejilla métrica de referencia, permitiendo al usuario construir, modificar y organizar entornos tridimensionales en tiempo de ejecución. El prototipo incorpora la creación dinámica de primitivas (cubo, esfera, cilindro, plano) y la importación de modelos Wavefront OBJ, clonación y eliminación de entidades, transformaciones afines independientes (TRS), modelado jerárquico padre-hijo mediante pila matricial recursiva, modelo de iluminación de Phong con control de posición y brillo especular, asignación e intercambio dinámico de texturas 2D (formato BMP), conmutación de proyecciones (perspectiva y ortogonal), modos de visualización (sólido y alambre), animación paramétrica gobernada por $\Delta t$ (LERP), detección de colisiones en tiempo real mediante cajas envolventes alineadas con los ejes (AABB), persistencia de archivos de escena (`.scene`) y una interfaz de inspección textual superpuesta en pantalla (HUD).

## Requisitos del Sistema

* Compilador de C++ compatible con C++17 (GCC/MinGW, Clang o MSVC).
* CMake (versión 3.10 o superior).
* Bibliotecas de desarrollo de OpenGL y FreeGLUT / GLUT instaladas en el sistema.

## Compilación y Ejecución

### Windows (MinGW Makefiles)

Para generar los archivos de compilación y compilar desde la raíz del proyecto (`PA4_SceneEditor/`):

```bash
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
cmake --build .

```

Para ejecutar el binario asegurando el enlace de los recursos (`assets/` y `scenes/`):

```bash
cd ..
.\build\PA4_SceneEditor.exe

```

## Controles de Interacción y Experimentación

### Creación y Gestión de Entidades

* **1 / 2 / 3**: Instanciar primitivas geométricas en el origen (Cubo, Esfera paramétrica o Cilindro).


* **4**: Instanciar un Plano horizontal de soporte.


* **5**: Importar y compilar la malla poligonal externa en formato OBJ (`sample_part.obj`).


* **D**: Duplicar el objeto seleccionado conservando sus propiedades y aplicando un desplazamiento lateral.


* **X**: Eliminar de la escena el objeto actualmente seleccionado y desvincular sus referencias.


* **TAB**: Alternar secuencialmente la selección del objeto activo en el árbol de escena.



### Transformaciones Espaciales (Modo TRS)

* **T**: Activar el modo de **Traslación**.


* **R**: Activar el modo de **Rotación**.


* **E**: Activar el modo de **Escalamiento**.


* **I / K**: Incrementar o decrementar el eje X según el modo de transformación activo.
* **O / L**: Incrementar o decrementar el eje Y según el modo de transformación activo.
* **U / J**: Incrementar o decrementar el eje Z según el modo de transformación activo.

### Jerarquías y Relaciones Dinámicas

* **H**: Emparentar el objeto seleccionado como hijo del nodo anterior en la lista de la escena.


* **N**: Desvincular (desemparentar) el objeto activo de su nodo padre para devolverlo a la raíz.

### Materiales, Texturas e Iluminación

* **M**: Conmutar cíclicamente la textura asignada al objeto activo (Textura 1, Textura 2 o Sin textura).


* **B**: Modificar la concentración del exponente de brillo especular (*shininess*) de la superficie.


* **G**: Desplazar dinámicamente la posición horizontal de la fuente de luz puntual (`GL_LIGHT0`).



### Navegación y Viewport

* **Flechas direccionales**: Orbitar la cámara libremente alrededor del centro de interés.


* **\+ / -**: Acercar o alejar la distancia orbital de la cámara (Zoom).


* **P**: Alternar el modo de proyección entre perspectiva cónica y paralela ortogonal.


* **W**: Alternar la rasterización entre polígonos sólidos (`GL_FILL`) y malla alámbrica (`GL_LINE`).



### Animación y Persistencia

* **Espacio**: Iniciar o pausar la reproducción de la animación configurada sobre el objeto activo.


* **Z**: Detener y reiniciar el ciclo de animación a sus valores iniciales.


* **C**: Alternar la propiedad de transformación asignada a la pista de animación (Rotación Y, Posición Y, Escala X).


* **F5**: Serializar y guardar la escena actual en disco (`scenes/saved_workspace.scene`).


* **F6**: Cargar, parsear y reconstruir el grafo de escena completo desde el archivo guardado.


* **ESC**: Finalizar la ejecución del programa.

## Elaboración

* **Asignatura:** Computación Gráfica


* **Institución:** Universidad Continental


* **Docente:** Mg. Percy Maldonado Quispe


* **Estudiante:** Erwin Alain Felix Tayro Mosqueira

## Licencia

Este proyecto se distribuye bajo la Licencia MIT. Para más detalles, consulte los términos estándar de código abierto.
