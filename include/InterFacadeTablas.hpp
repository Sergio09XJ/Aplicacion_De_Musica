
#ifndef INTERFACADETABLAS_HPP
#define INTERFACADETABLAS_HPP


#include <string>
#include <iostream>
#include <memory>
#include <pqxx/pqxx>


/**
 * @class Clase InterFacadeTablas; es la interface para CRUD en la base de datos
 * @brief Permite crear, acceder, actualizar y eliminar en la base de datos.
 */
class InterFacadeTablas{

  public:

    /**
    * @brief Método virtual para crear e insertar en la base de datos.
    * @param Sin parametros.
    * @return Sin retorno
    */
    virtual void Crear_Insertar() = 0;

    /**
    * @brief Método virtual para consutar la base de datos.
    * @param id para acceder a la fila.
    * @return retorno auto en base a tipo de dato que se retorno
    */
    virtual std::string Leer_Consultar(int id) = 0;

    /**
    * @brief Método virtual para modificar la base de datos.
    * @param Sin parametros.
    * @return Sin retorno.
   …. */
    virtual void Actualizar_Modificar() = 0;

    /**
    * @brief Método virtual para eliminar la base de datos.
    * @param Sin parametros.
    * @return Sin retorno.
    */
    virtual void Eliminar_Borrar() = 0;





};

#endif //INTERFACADETABLAS_HPP