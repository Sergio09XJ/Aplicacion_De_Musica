#include "In_Group.hpp"


In_Group::In_Group(int id_person, int id_group)
    : id_person(id_person), id_group(id_group){}


In_Group::~In_Group(){
    conn.reset();
}


void In_Group::Crear_Insertar(){}


json In_Group::Leer_Consultar(int id){
    pqxx::work txn(*conn);
    pqxx::result res = txn.exec("SELECT id_type, descripcion FROM Types;");

    // Extraemos las columnas mapeándolas a sus tipos de C++
    for(const auto& row : res){
        if (id == row["id_person"].as<int>()){
            this->id_person = row["id_person"].as<int>();
            this->id_group = row["id_group"].as<int>();


            return json{
                        {"id_person" , this->id_person},
                        {"id_group", this->id_group},
                        };

        }
    }
    return json{};
}


void In_Group::Actualizar_Modificar(std::string peticion){}


void In_Group::Eliminar_Borrar(){}