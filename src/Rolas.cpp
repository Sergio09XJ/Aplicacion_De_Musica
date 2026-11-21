#include "Rolas.hpp"

Rolas::Rolas(int id_rola, int id_performer, int id_album, int id_composer, int id_original_artist,
             std::string path, std::string title, std::string track, int disk_number, int year,
             std::string genre, std::string comment)
    : id_rola(id_rola),
    id_performer(id_performer),
    id_album(id_album),
    id_composer(id_composer),
    id_original_artist(id_original_artist),
    path(std::move(path)),
    title(std::move(title)),
    track(std::move(track)),
    disk_number(disk_number),
    year(year),
    genre(std::move(genre)),
    comment(std::move(comment)) {

#ifdef __APPLE__
    const std::string conn_str = "dbname=musicabasedatos user=postgres password=music_dev_password host=localhost port=5432";
#else
    const std::string conn_str = "dbname=musicabasedatos user=postgres password=music_dev_password host=postgres port=5432";
#endif

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

Rolas::~Rolas() {
    conn.reset();
}

void Rolas::Crear_Insertar() {
    if (!conn || !conn->is_open()) return;

    pqxx::work txn(*conn);

    txn.exec_params(
        "INSERT INTO Rolas (id_rola, id_performer, id_album, id_composer, id_original_artist, "
        "path, title, track, disc_number, year, genre, comment) "
        "VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12);",
        this->id_rola,
        this->id_performer,
        this->id_album,
        this->id_composer,
        this->id_original_artist,
        this->path,
        this->title,
        this->track,
        this->disk_number,
        this->year,
        this->genre,
        this->comment
        );

    txn.commit();
    std::cout << "Registro insertado y guardado con éxito.\n";
}

json Rolas::Leer_Consultar(int id) {
    if (!conn || !conn->is_open()) return json{};

    pqxx::work txn(*conn);
    pqxx::result res = txn.exec_params(
        "SELECT id_rola, id_performer, id_album, id_composer, id_original_artist, "
        "path, title, track, disc_number, year, genre, comment "
        "FROM Rolas WHERE id_rola = $1;",
        id
        );

    if (!res.empty()) {
        for (const auto& row : res) {
            if (id == row["id_rola"].as<int>(0)) {
                this->id_rola = row["id_rola"].as<int>(0);
                this->id_performer = row["id_performer"].as<int>(0);
                this->id_album = row["id_album"].as<int>(0);
                this->id_composer = row["id_composer"].as<int>(0);
                this->id_original_artist = row["id_original_artist"].as<int>(0);
                this->path = row["path"].as<std::string>("");
                this->title = row["title"].as<std::string>("");
                this->track = row["track"].as<std::string>("");
                this->disk_number = row["disc_number"].as<int>(0);
                this->year = row["year"].as<int>(0);
                this->genre = row["genre"].as<std::string>("");
                this->comment = row["comment"].as<std::string>("");

                return json{
                    {"id_rola", this->id_rola},
                    {"id_performer", this->id_performer},
                    {"id_album", this->id_album},
                    {"id_composer", this->id_composer},
                    {"id_original_artist", this->id_original_artist},
                    {"path", this->path},
                    {"title", this->title},
                    {"track", this->track},
                    {"disk_number", this->disk_number},
                    {"year", this->year},
                    {"genre", this->genre},
                    {"comment", this->comment}
                };
            }
        }
    }
    return json{};
}

void Rolas::Actualizar_Modificar(int elemento) {
    if (!conn || !conn->is_open()) return;

    pqxx::work txn(*conn);

    switch (elemento) {
    case 1:
        txn.exec_params("UPDATE Rolas SET id_performer = $1 WHERE id_rola = $2;", this->id_performer, this->id_rola);
        break;
    case 2:
        txn.exec_params("UPDATE Rolas SET id_album = $1 WHERE id_rola = $2;", this->id_album, this->id_rola);
        break;
    case 3:
        txn.exec_params("UPDATE Rolas SET id_composer = $1 WHERE id_rola = $2;", this->id_composer, this->id_rola);
        break;
    case 4:
        txn.exec_params("UPDATE Rolas SET id_original_artist = $1 WHERE id_rola = $2;", this->id_original_artist, this->id_rola);
        break;
    case 5:
        txn.exec_params("UPDATE Rolas SET path = $1 WHERE id_rola = $2;", this->path, this->id_rola);
        break;
    case 6:
        txn.exec_params("UPDATE Rolas SET title = $1 WHERE id_rola = $2;", this->title, this->id_rola);
        break;
    case 7:
        txn.exec_params("UPDATE Rolas SET track = $1 WHERE id_rola = $2;", this->track, this->id_rola);
        break;
    case 8:
        txn.exec_params("UPDATE Rolas SET disc_number = $1 WHERE id_rola = $2;", this->disk_number, this->id_rola);
        break;
    case 9:
        txn.exec_params("UPDATE Rolas SET year = $1 WHERE id_rola = $2;", this->year, this->id_rola);
        break;
    case 10:
        txn.exec_params("UPDATE Rolas SET genre = $1 WHERE id_rola = $2;", this->genre, this->id_rola);
        break;
    case 11:
        txn.exec_params("UPDATE Rolas SET comment = $1 WHERE id_rola = $2;", this->comment, this->id_rola);
        break;
    default:
        std::cout << "Tu opcion no cabe dentro de los parametros." << std::endl;
        return;
    }

    txn.commit();
    std::cout << "Se actualizo tu parametro con exito." << std::endl;
}

void Rolas::Eliminar_Borrar() {
    if (!conn || !conn->is_open()) return;

    pqxx::work txn(*conn);
    txn.exec_params("DELETE FROM Rolas WHERE id_rola = $1;", this->id_rola);
    txn.commit();
    std::cout << "Se elimino la fila con exito." << std::endl;
}
