#include "Persons.hpp"


Persons::Persons(int id_persons, std::string stage_name, std::string real_name, std::string birth_date,  std::string death_date,  int performers_id)
   : id_persons(id_persons), stage_name(stage_name), real_name(real_name), birth_date(birth_date), death_date(death_date), performers_id(performers_id){

    std::string conn_str = "dbname=musica-pgadmin user=postgres password=music_dev_password host=postgres port=5432";
    this->conn = std::make_unique<pqxx::connection>(conn_str);

    try{
        if(conn->is_open()){
            std::cout << "La conexión con la base de datos fue Exitosa. " << std::endl;
        }else{
            std::cout << "La conexión con la base de datos No fue Exitosa. " << std::endl;
        }
    }catch(const std::exception &e) {
        // Capturamos cualquier error de conexión o sintaxis SQL
        std::cerr << "Error crítico en la base de datos: " << e.what() << std::endl;
    }
}


Persons::~Persons(){
    conn.reset();
}


void Persons::Crear_Insertar(){}


json Persons::Leer_Consultar(int id){
    pqxx::work txn(*conn);
    pqxx::result res = txn.exec("SELECT id_type, descripcion FROM Types;");

    // Extraemos las columnas mapeándolas a sus tipos de C++
    for(const auto& row : res){
        if (id == row["id_persons"].as<int>()){
            this->id_persons = row["id_rola"].as<int>();
            this->stage_name = row["id_performer"].as<std::string>();
            this->real_name = row["id_album"].as<std::string>();
            this->birth_date = row["id_composer"].as<std::string>();
            this->death_date = row["id_original_artist"].as<std::string>();
            this->performers_id = row["path"].as<int>();



            return json{
                        {"id_persons" , this->id_persons},
                        {"stage_name", this->stage_name},
                        {"real_name" , this->real_name},
                        {"birth_date", this->birth_date},
                        {"death_date" , this->death_date},
                        {"performers_id", this->performers_id},

                        };

        }
    }
    return json{};
}


void Persons::Actualizar_Modificar(std::string peticion){}


void Persons::Eliminar_Borrar(){}
