#include "Rolas.hpp"






Rolas::Rolas(int id_rola, int id_performer, int id_album, int id_composer, int id_original_artist,
             std::string path, std::string title, int track, int disk_number, int year,
             std::string genre, std::string comment)
    : id_rola(id_rola),
    id_performer(id_performer),
    id_album(id_album),
    id_composer(id_composer),
    id_original_artist(id_original_artist),
    path(std::move(path)),
    title(std::move(title)),
    track(track),
    disk_number(disk_number),
    year(year),
    genre(std::move(genre)),
    comment(std::move(comment)) {

    std::string conn_str = "dbname=musica-pgadmin user=postgres password=music_dev_password host=postgres port=5432";

    try {
        this->conn = std::make_unique<pqxx::connection>(conn_str);
        if (conn->is_open()) {
            std::cout << "La conexión con la base de datos fue Exitosa." << std::endl;
        } else {
            std::cout << "La conexión con la base de datos No fue Exitosa." << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error crítico en la base de datos: " << e.what() << std::endl;
    }
}



Rolas::~Rolas(){
    conn.reset();
}


void Rolas::Crear_Insertar(){}


json Rolas::Leer_Consultar(int id){
    pqxx::work txn(*conn);
    pqxx::result res = txn.exec("SELECT id_type, descripcion FROM Types;");

    // Extraemos las columnas mapeándolas a sus tipos de C++
    for(const auto& row : res){
        if (id == row["id_rola"].as<int>()){
            this->id_rola = row["id_rola"].as<int>();
            this->id_performer = row["id_performer"].as<int>();
            this->id_album = row["id_album"].as<int>();
            this->id_composer = row["id_composer"].as<int>();
            this->id_original_artist = row["id_original_artist"].as<int>();
            this->path = row["path"].as<std::string>();
            this->title = row["title"].as<std::string>();
            this->track = row["track"].as<int>();
            this->disk_number = row["disk_number"].as<int>();
            this->genre = row["genre"].as<std::string>();
            this->comment = row["comment"].as<std::string>();


            return json{
                {"id_rola" , this->id_rola},
                {"id_performer", this->id_performer},
                {"id_album" , this->id_album},
                {"id_composer", this->id_composer},
                {"id_original_artist" , this->id_original_artist},
                {"path", this->path},
                {"title" , this->title},
                {"track", this->track},
                {"disk_number" , this->disk_number},
                {"genre" , this->genre},
                {"comment", this->comment},
            };

        }
    }
    return json{};
}


void Rolas::Actualizar_Modificar(std::string peticion){
}


void Rolas::Eliminar_Borrar(){}