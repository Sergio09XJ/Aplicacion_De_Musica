
#include "Albums.hpp"


Albums::Albums(int id_album, std::string path, std::string name,  int year, std::string coverImage)
       : id_album(id_album), path(path), name(name), year(year), coverImage(coverImage){}


Albums::~Albums(){}


void Albums::Crear_Insertar(){}


json Albums::Leer_Consultar(int id){
    pqxx::work txn(*conn);
    pqxx::result res = txn.exec("SELECT id_type, descripcion FROM Types;");

    // Extraemos las columnas mapeándolas a sus tipos de C++
    for(const auto& row : res){
        if (id == row["id_album"].as<int>()){
            this->id_album = row["id_album"].as<int>();
            this->path = row["path"].as<std::string>();
            this->name = row["name"].as<std::string>();
            this->year = row["year"].as<int>();
            this->coverImage = row["coverImage"].as<std::string>();



            return json{
                        {"id_album" , this->id_album},
                        {"path", this->path},
                        {"name" , this->name},
                        {"year", this->year},
                        {"coverImage", this->coverImage},
                        };

        }
    }
    return json{};
}


void Albums::Actualizar_Modificar(std::string peticion){}


void Albums::Eliminar_Borrar(){}