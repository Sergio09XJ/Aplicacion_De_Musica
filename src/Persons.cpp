#include "Persons.hpp"


Persons::Persons(int id_persons, std::string stage_name, std::string real_name, std::string birth_date,  std::string death_date,  int performers_id)
   : id_persons(id_persons), stage_name(stage_name), real_name(real_name), birth_date(birth_date), death_date(death_date), performers_id(performers_id){}


Persons::~Persons(){}


void Persons::Crear_Insertar(){}


std::string Persons::Leer_Consultar(int id){
    return " espera ";
}


void Persons::Actualizar_Modificar(){}


void Persons::Eliminar_Borrar(){}
