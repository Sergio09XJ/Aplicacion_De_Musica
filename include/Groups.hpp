
#ifndef GROUPS_HPP
#define GROUPS_HPP

#include "InterFacadeTablas.hpp"

/**
 * @class Groups; esta clase es la representación de la tabla Groups
 * @brief Permite acceder, modificar, eliminar, actualizar valores de esta tabla.
 */
class Groups : public InterFacadeTablas{

private:

    int id_group;
    std::string name;
    std::string start_date;
    std::string end_date;
    int performers_id;
    std::unique_ptr<pqxx::connection> conn;

public:

    /**
    * @brief Constructor Groups
    * @param int para persons y performers, string para stage_name, real name, nacimiento y muerte.
    * @return Sin retorno
    */
    Groups(int id_group, std::string name, std::string start_date, std::string end_date, int performers_id);

    /**
    * @brief Destructo de Groups.
    * @param Sin parametros.
    * @return Sin retorno
    */
    ~Groups();

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

#endif //Groups