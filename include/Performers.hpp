
#ifndef PERFORMERS_HPP
#define PERFORMERS_HPP

#include "InterFacadeTablas.hpp"

/**
 * @class Performers; esta clase es la representación de la tabla Performers
 * @brief Permite acceder, modificar, eliminar, actualizar valores de esta tabla.
 */
class Performers : public InterFacadeTablas{

private:

    int id_performers;
    int id_type;
    std::string name;
    std::string image;

public:

    /**
    * @brief Constructor Performers
    * @param int de el id para performers y type y string de name e image.
    * @return Sin retorno
    */
    Performers(int id_performers, int id_type, std::string name, std::string image);

    /**
    * @brief Destructor de Performers.
    * @param Sin parametros.
    * @return Sin retorno
    */
    ~Performers();

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

#endif //Performers