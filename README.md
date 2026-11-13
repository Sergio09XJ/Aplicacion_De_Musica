# Aplicación de Música

Este proyecto se puede definir como una base de datos musical con interfaz gráfica. 

> **Nota importante:** Los cambios que se realizan a través de la aplicación ocurren exclusivamente en la base de datos, no en los archivos MP3 originales.

## Componentes del Proyecto

* **Base de datos:** Almacena toda la información organizada de la biblioteca musical.
* **Minero:** Recorre el directorio del sistema y analiza los archivos MP3 en base a sus etiquetas ID3v2.4. Esta información extraída se agrega automáticamente a la base de datos.
* **Interfaz gráfica:** Su función es permitir la reproducción de los archivos y modificar ciertos datos dentro de la base de datos (por ejemplo, cambiar el título de la canción, el año de un álbum o el intérprete).
* **Compilador:** Su objetivo es permitir la búsqueda de distintos campos en la base de datos de forma intuitiva, sin que el usuario deba escribir consultas SQL.

## Gestión en la Base de Datos

El sistema permite estructurar y modificar la información musical con las siguientes reglas:

* Los intérpretes de una canción pueden ser clasificados como individuos o como una banda.
* Se pueden editar los detalles específicos de cualquier banda o individuo.
* Es posible registrar en el sistema individuos que actualmente no interpreten ninguna pista.
* Se pueden agregar individuos para que formen parte de las bandas existentes.

## Acceso a las Canciones

El acceso a los archivos físicos es un trabajo gestionado por el minero de la siguiente manera:

* **Pistas:** Cada pista tiene un *path* (ruta) en el sistema de archivos que apunta directamente al archivo MP3 que representa.
* **Álbumes:** Cada álbum cuenta con una ruta que apunta al directorio específico donde están alojadas las canciones de ese álbum.

## Etiquetas Analizadas (ID3v2.4)

El minero lee las siguientes etiquetas de los archivos MP3 para poblar la base de datos:

| Atributo | Etiqueta | Descripción / Formato |
| :--- | :---: | :--- |
| **Artista** | `TPE1` | Intérprete principal de la pista. |
| **Título** | `TIT2` | Nombre de la canción. |
| **Álbum** | `TALB` | Nombre del álbum. |
| **Banda del álbum** | `TPE2` | También conocida como "artista del álbum". |
| **Año** | `TDRC` | Año de grabación o publicación. |
| **Pista** | `TRCK` | Cadena con la pista y el total de pistas (ej. `04/13` para la pista 04 de 13 en total). |
| **Número de discos** | `TPOS` | Posición del medio en un set de múltiples volúmenes. |
| **Género** | `TCON` | Categoría o género musical. |
| **Comentario** | `COMM` | Notas o descripciones adicionales. |
| **Compositor** | `TCOM` | Creador o compositor de la pista. |
| **Artista original** | `TOPE` | Intérprete original de la obra (útil para identificar *covers*). |
| **Imagen** | `APIC` | Carátula del álbum y fotografía del artista. |
