
#ifndef PERSONS_HPP
#define PERSONS_HPP

#include "InterFacadeTablas.hpp"

/**
 * @class Persons; esta clase es la representación de la tabla Persons
 * @brief Permite acceder, modificar, eliminar, actualizar valores de esta tabla.
 */
class Persons : public InterFacadeTablas{

private:

    int id_persons;
    std::string stage_name;
    std::string real_name;
    std::string birth_date;
    std::string death_date;
    int performers_id;

public:

    /**
    * @brief Constructor Persons
    * @param int para persons y performers, string para stage_name, real name, nacimiento y muerte.
    * @return Sin retorno
    */
    Persons(int id_persons, std::string stage_name, std::string real_name, std::string birth_date,  std::string death_date,  int performers_id);

    /**
    * @brief Destructo de Persons.
    * @param Sin parametros.
    * @return Sin retorno
    */
    ~Persons();

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

#endif //Persons