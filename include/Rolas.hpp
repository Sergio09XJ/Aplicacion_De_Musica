#ifndef ROLAS_HPP
#define ROLAS_HPP

#include "InterFacadeTablas.hpp"

/**
 * @class Rolas; esta clase es la representación de la tabla Rolas
 * @brief Permite acceder, modificar, eliminar, actualizar valores de esta tabla.
 */
class Rolas : public InterFacadeTablas{

private:

    int id_rola;
    int id_performer;
    int id_album;
    int id_composer;
    int id_original_artist;
    std::string path;
    std::string title;
    std::string track;
    std::string disk_number;
    int year;
    std::string genre;
    std::string comment;


public:

    /**
    * @brief Constructor Rolas
    * @param int para id_album y year string para  path, name, coverImage
    * @return Sin retorno
    */
    Rolas(int id_rola, int id_performer, int id_album,  int id_original_artist, std::string path, std::string title,
           std::string track,std::string disk_number, int year, std::string genre, std::string comment);

    /**
    * @brief Destructor de Rolas.
    * @param Sin parametros.
    * @return Sin retorno
    */
    ~Rolas();

    /**
    * @brief Método concreto para crear e insertar en la base de datos.
    * @param Sin parametros.
    * @return Sin retorno
    */
    void Crear_Insertar();

    /**
    * @brief Método contreto para consutar la base de datos.
    * @param id para acceder a la fila.
    * @return retorno auto en base a tipo de dato que se retorno
    */
    std::string Leer_Consultar(int id);

    /**
    * @brief Método concreto para modificar la base de datos.
    * @param Sin parametros.
    * @return Sin retorno.
    */
    void Actualizar_Modificar();

    /**
    * @brief Método concreto para eliminar la base de datos.
    * @param Sin parametros.
    * @return Sin retorno.
    */
    void Eliminar_Borrar();


};

#endif //Rolas