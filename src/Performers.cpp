#include "Performers.hpp"

Performers::Performers(int id_performer, int id_type, std::string name, std::string image)
    : id_performer(id_performer),
    id_type(id_type),
    name(std::move(name)),
    image(std::move(image)) {

    std::string conn_str = "dbname=musicabasedatos user=postgres password=music_dev_password host=postgres port=5432";

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

Performers::~Performers() {
    conn.reset();
}

void Performers::Crear_Insertar() {
    if (!conn || !conn->is_open()) return;

    pqxx::work txn(*conn);

    txn.exec_params(
        "INSERT INTO Performers (id_performer, id_type, name, image) "
        "VALUES ($1, $2, $3, $4);",
        this->id_performer,
        this->id_type,
        this->name,
        this->image
        );

    txn.commit();
    std::cout << "Registro insertado y guardado con éxito.\n";
}

json Performers::Leer_Consultar(int id) {
    if (!conn || !conn->is_open()) return json{};

    pqxx::work txn(*conn);
    pqxx::result res = txn.exec_params(
        "SELECT id_performer, id_type, name, image "
        "FROM Performers WHERE id_performer = $1;",
        id
        );

    if (!res.empty()) {
        for (const auto& row : res) {
            if (id == row["id_performer"].as<int>(0)) {
                this->id_performer = row["id_performer"].as<int>(0);
                this->id_type = row["id_type"].as<int>(0);
                this->name = row["name"].as<std::string>("");
                this->image = row["image"].as<std::string>("");

                return json{
                    {"id_performer", this->id_performer},
                    {"id_type", this->id_type},
                    {"name", this->name},
                    {"image", this->image}
                };
            }
        }
    }
    return json{};
}

void Performers::Actualizar_Modificar(int elemento) {
    if (!conn || !conn->is_open()) return;

    pqxx::work txn(*conn);

    switch (elemento) {
    case 1:
        txn.exec_params("UPDATE Performers SET id_type = $1 WHERE id_performer = $2;", this->id_type, this->id_performer);
        break;
    case 2:
        txn.exec_params("UPDATE Performers SET name = $1 WHERE id_performer = $2;", this->name, this->id_performer);
        break;
    case 3:
        txn.exec_params("UPDATE Performers SET image = $1 WHERE id_performer = $2;", this->image, this->id_performer);
        break;
    default:
        std::cout << "Tu opcion no cabe dentro de los parametros." << std::endl;
        return;
    }

    txn.commit();
    std::cout << "Se actualizo tu parametro con exito." << std::endl;
}

void Performers::Eliminar_Borrar() {
    if (!conn || !conn->is_open()) return;

    pqxx::work txn(*conn);
    txn.exec_params("DELETE FROM Performers WHERE id_performer = $1;", this->id_performer);
    txn.commit();
    std::cout << "Se elimino la fila con exito." << std::endl;
}
