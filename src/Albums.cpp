#include "Albums.hpp"

Albums::Albums(int id_album, std::string path, std::string name, int year, std::string cover_image)
    : id_album(id_album), path(std::move(path)), name(std::move(name)), year(year), cover_image(std::move(cover_image)) {

    std::string conn_str = "dbname=musicabasedatos user=postgres password=music_dev_password host=postgres port=5432";

    try {
        this->conn = std::make_unique<pqxx::connection>(conn_str);
        if (conn->is_open()) {
            std::cout << "La conexión con la base de datos fue Exitosa. " << std::endl;
        } else {
            std::cout << "La conexión con la base de datos No fue Exitosa. " << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error crítico en la base de datos: " << e.what() << std::endl;
    }
}

Albums::~Albums() {
    conn.reset();
}

void Albums::Crear_Insertar() {
    if (!conn || !conn->is_open()) return;

    pqxx::work txn(*conn);
    txn.exec_params(
        "INSERT INTO Albums (id_album, path, name, year, cover_image) VALUES ($1, $2, $3, $4, $5);",
        this->id_album,
        this->path,
        this->name,
        this->year,
        this->cover_image
        );
    txn.commit();
    std::cout << "Registro insertado y guardado con éxito.\n";
}

json Albums::Leer_Consultar(int id) {
    if (!conn || !conn->is_open()) return json{};

    pqxx::work txn(*conn);
    pqxx::result res = txn.exec_params(
        "SELECT id_album, path, name, year, cover_image FROM Albums WHERE id_album = $1;", id
        );

    if (!res.empty()) {
        for (const auto& row : res) {
            if (id == row["id_album"].as<int>(0)) {
                this->id_album = row["id_album"].as<int>(0);
                this->path = row["path"].as<std::string>("");
                this->name = row["name"].as<std::string>("");
                this->year = row["year"].as<int>(0);
                this->cover_image = row["cover_image"].as<std::string>("");

                return json{
                    {"id_album", this->id_album},
                    {"path", this->path},
                    {"name", this->name},
                    {"year", this->year},
                    {"coverImage", this->cover_image}
                };
            }
        }
    }
    return json{};
}

void Albums::Actualizar_Modificar(int elemento) {
    if (!conn || !conn->is_open()) return;

    pqxx::work txn(*conn);
    switch (elemento) {
    case 1:
        txn.exec_params("UPDATE Albums SET path = $1 WHERE id_album = $2;", this->path, this->id_album);
        break;
    case 2:
        txn.exec_params("UPDATE Albums SET name = $1 WHERE id_album = $2;", this->name, this->id_album);
        break;
    case 3:
        txn.exec_params("UPDATE Albums SET year = $1 WHERE id_album = $2;", this->year, this->id_album);
        break;
    case 4:
        txn.exec_params("UPDATE Albums SET cover_image = $1 WHERE id_album = $2;", this->cover_image, this->id_album);
        break;
    default:
        std::cout << "Tu opcion no cabe dentro de los parametros." << std::endl;
        return;
    }
    txn.commit();
    std::cout << "Se actualizo tu parametro con exito. " << std::endl;
}

void Albums::Eliminar_Borrar() {
    if (!conn || !conn->is_open()) return;

    pqxx::work txn(*conn);
    txn.exec_params("DELETE FROM Albums WHERE id_album = $1;", this->id_album);
    txn.commit();
    std::cout << "Se elimino la fila con exito. " << std::endl;
}















