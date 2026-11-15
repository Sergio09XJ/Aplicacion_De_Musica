#include "Performers.hpp"


Performers::Performers(int id_performers, int id_type, std::string name, std::string image)
    : id_performers(id_performers), id_type(id_type), name(name), image(image){}


Performers::~Performers(){
    conn.reset();
}


void Performers::Crear_Insertar(){}


json Performers::Leer_Consultar(int id){
    pqxx::work txn(*conn);
    pqxx::result res = txn.exec("SELECT id_type, descripcion FROM Types;");

    // Extraemos las columnas mapeándolas a sus tipos de C++
    for(const auto& row : res){
        if (id == row["id_performers"].as<int>()){
            this->id_performers = row["id_performers"].as<int>();
            this->id_type = row["id_type"].as<int>();
            this->name = row["name"].as<std::string>();
            this->image = row["image"].as<std::string>();



            return json{
                        {"id_performers" , this->id_performers},
                        {"id_type", this->id_type},
                        {"name" , this->name},
                        {"image", this->image},
                        };

        }
    }
    return json{};
}


void Performers::Actualizar_Modificar(std::string peticion){}


void Performers::Eliminar_Borrar(){}