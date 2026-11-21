#ifndef METADATOS_MP3_HPP
#define METADATOS_MP3_HPP

#include <string>

/**
 * @struct struct MetadatosMP3; es la instancia que nos permite acceder a los componenetes del MP3
 * @brief Permite acceder y modificar los metadatos del mp3. 
 */
struct MetadatosMP3 {
    std::string path;
    std::string title;
    std::string artist;
    std::string album;
    std::string track;
    int discNumber = 1;
    int year = 0;
    std::string genre;
    std::string comment;
};

/**
 * @struct ResumenMinado
   @brief nos indica los cambios que se hicieron en las canciones. 
 */
struct ResumenMinado {
    int archivosEscaneados = 0;
    int cancionesInsertadas = 0;
    int cancionesOmitidas = 0;
    int errores = 0;
};

#endif // METADATOS_MP3_HPP
