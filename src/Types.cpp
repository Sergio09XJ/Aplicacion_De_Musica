#include "Types.hpp"




Types::Types(int id_type, std::string description) : id_type(id_type), description(description){

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


Types::~Types(){
    conn.reset();
}


void Types::Crear_Insertar(){
}



std::string Types::Leer_Consultar(int id){
    pqxx::work txn(&conn);
    pqxx::result res = txn.exec(peticion)

     // Extraemos las columnas mapeándolas a sus tipos de C++
    this->id_type = row["id_type"].as<int>();
    this->description = row["description"].as<std::string>();

    std::cout << "ID: " << id << " | Nombre: " << name << "\n";
    return std::pair<int,std::string>(id_type, description);

}


void Types::Actualizar_Modificar(std::string peticion){


}


void Types::Eliminar_Borrar(){}