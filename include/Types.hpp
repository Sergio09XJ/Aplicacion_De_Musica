
#ifndef TYPES_HPP
#define TYPES_HPP

#include "InterFacadeTablas.hpp"

/**
 * @class Types; esta clase es la representación de la tabla Types
 * @brief Permite acceder, modificar, eliminar, actualizar valores de esta tabla.
 */
class Types : public InterFacadeTablas{

  private:

    int id_type;
    std::string description;
    std::unique_ptr<pqxx::connection> conn;

  public:


    /**
    * @brief Constructor Types
    * @param int de el id y string de la descricción.
    * @return Sin retorno
    */
    Types(int id_type, std::string description);

      /**
    * @brief Destructo de Types.
    * @param Sin parametros.
    * @return Sin retorno
    */
    ~Types();

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
    json Leer_Consultar(int id);

    /**
    * @brief Método concreto para modificar la base de datos.
    * @param Sin parametros.
    * @return Sin retorno.
    */
    void Actualizar_Modificar(std::string peticion);

    /**
    * @brief Método concreto para eliminar la base de datos.
    * @param Sin parametros.
    * @return Sin retorno.
    */
    void Eliminar_Borrar();


};

#endif //Types