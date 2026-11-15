#include "Groups.hpp"


Groups::Groups(int id_group, std::string name, std::string start_date, std::string end_date, int performers_id)
    : id_group(id_group), name(name), start_date(start_date), end_date(end_date), performers_id(performers_id){}


Groups::~Groups(){
    conn.reset();
}


void Groups::Crear_Insertar(){}


json Groups::Leer_Consultar(int id){
    pqxx::work txn(*conn);
    pqxx::result res = txn.exec("SELECT id_type, descripcion FROM Types;");

    // Extraemos las columnas mapeándolas a sus tipos de C++
    for(const auto& row : res){
        if (id == row["id_group"].as<int>()){
            this->id_group = row["id_group"].as<int>();
            this->name = row["name"].as<std::string>();
            this->start_date = row["start_date"].as<std::string>();
            this->end_date = row["end_date"].as<std::string>();
            this->performers_id = row["performers_id"].as<int>();



            return json{
                        {"id_group" , this->id_group},
                        {"name", this->name},
                        {"start_date" , this->start_date},
                        {"end_date", this->end_date},
                        {"performers_id", this->performers_id},
                        };

        }
    }
    return json{};
}


void Groups::Actualizar_Modificar(std::string peticion){}


void Groups::Eliminar_Borrar(){}