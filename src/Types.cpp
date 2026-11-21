#include "Types.hpp"




Types::Types(int id_type, std::string description) : id_type(id_type), description(std::move(description)){

    std::string conn_str = "dbname=musicabasedatos user=postgres password=music_dev_password host=postgres port=5432";

    try{
       this->conn = std::make_unique<pqxx::connection>(conn_str);
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
    pqxx::work txn(*conn);

    txn.exec_params("INSERT INTO Types (id_type, description) VALUES ($1, $2);",
                    this->id_type,
                    this->description
                    );


    txn.commit();
    std::cout << "Registro insertado y guardado con éxito.\n";
}



json Types::Leer_Consultar(int id) {
    if (!conn || !conn->is_open()) return json{};

    pqxx::work txn(*conn);
    pqxx::result res = txn.exec_params(
        "SELECT id_type, description FROM Types WHERE id_type = $1;", id
        );

    if (!res.empty()) {
        for (const auto& row : res) {
            if (id == row["id_type"].as<int>(0)) {
                this->id_type = row["id_type"].as<int>(0);
                this->description = row["description"].as<std::string>("");
                return json{
                    {"id_type", this->id_type},
                    {"description", this->description}
                };
            }
        }
    }
    return json{};
}


void Types::Actualizar_Modificar(int elemento){
    pqxx::work txn(*conn);

    switch (elemento) {
    case 1:
        txn.exec_params("UPDATE Types SET description = $1 WHERE id_type = $2;", this->description, this->id_type);
        break;
    default:
        std::cout << "Tu opcion no cabe dentro de los parametros." << std::endl;
        return;
    }

    txn.commit();
    std::cout << "Se actualizo tu parametro con exito. " << std::endl;

}


void Types::Eliminar_Borrar(){
    pqxx::work txn(*conn);
    txn.exec_params("DELETE FROM Types WHERE id_type = $1;", this->id_type);
    txn.commit();
    std::cout << "Se elimino la fila con exito. " << std::endl;
}






