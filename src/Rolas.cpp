#include "Rolas.hpp"


Rolas::Rolas(int id_rola, int id_performer, int id_album,  int id_original_artist, std::string path, std::string title,
             std::string track,std::string disk_number, int year, std::string genre, std::string comment)
    : id_rola(id_rola), id_performer(id_performer), id_album(id_album), id_original_artist(id_original_artist), path(path),
    title(title), track(track), disk_number(disk_number), year(year), genre(genre), comment(comment){}


Rolas::~Rolas(){}


void Rolas::Crear_Insertar(){}


std::string Rolas::Leer_Consultar(int id){
    return " espera ";
}


void Rolas::Actualizar_Modificar(){}


void Rolas::Eliminar_Borrar(){}