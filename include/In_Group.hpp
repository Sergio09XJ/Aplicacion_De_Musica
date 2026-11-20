
#ifndef IN_GROUP_HPP
#define IN_GROUP_HPP

#include "InterFacadeTablas.hpp"

/**
 * @class In_Group; esta clase es la representación de la tabla In_Group
 * @brief Permite acceder, modificar, eliminar, actualizar valores de esta tabla.
 */
class In_Group : public InterFacadeTablas{

private:

    int id_person;
    int id_group;
    std::unique_ptr<pqxx::connection> conn;


public:

    /**
    * @brief Constructor In_Group
    * @param int para id_person y group.
    * @return Sin retorno
    */
    In_Group(int id_person, int id_group);

    /**
    * @brief Destructo de In_Group.
    * @param Sin parametros.
    * @return Sin retorno
    */
    ~In_Group();

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
    void Actualizar_Modificar(int elemento);

    /**
    * @brief Método concreto para eliminar la base de datos.
    * @param Sin parametros.
    * @return Sin retorno.
    */
    void Eliminar_Borrar();


};

#endif //IN_GROUP