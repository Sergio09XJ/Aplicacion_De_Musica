
#include "Albums.hpp"


Albums::Albums(int id_album, std::string path, std::string name,  int year, std::string coverImage)
       : id_album(id_album), path(path), name(name), year(year), coverImage(coverImage){}


Albums::~Albums(){}


void Albums::Crear_Insertar(){}


std::string Albums::Leer_Consultar(int id){
    return " espera ";
}


void Albums::Actualizar_Modificar(){}


void Albums::Eliminar_Borrar(){}