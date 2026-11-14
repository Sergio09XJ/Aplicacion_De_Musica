#ifndef ALBUMS_HPP
#define ALBUMS_HPP

#include "InterFacadeTablas.hpp"

/**
 * @class Albums; esta clase es la representación de la tabla Albums
 * @brief Permite acceder, modificar, eliminar, actualizar valores de esta tabla.
 */
class Albums : public InterFacadeTablas{

private:

    int id_album;
    int id_performer;
    std::string path;
    std::string name;
    int year;
    std::string coverImage;


public:

    /**
    * @brief Constructor Albums
    * @param int para id_album y year string para  path, name, coverImage
    * @return Sin retorno
    */
    Albums(int id_album, std::string path, std::string name,  int year, std::string coverImage);

    /**
    * @brief Destructo de Albums.
    * @param Sin parametros.
    * @return Sin retorno
    */
    ~Albums();

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

#endif //ALBUMS