
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "vistas"
import "componentes"


ApplicationWindow {
    id: ventanaPrincipal
    width: 1100
    height: 700
    visible: true
    title: "Biblioteca Musical"

    DialogoImportarMinero {
        id: dialogoMinero
        onImportacionTerminada: {
            vistaCanciones.cargar()
            vistaAlbumes.cargar()
            vistaArtistas.cargarTodo()
        }
    }

    header: ToolBar {
        RowLayout {
            anchors.fill: parent
            anchors.margins: 8
            Label {
                text: "Mi Biblioteca Musical"
                font.bold: true
                font.pixelSize: 18
                Layout.fillWidth: true
            }
            Button {
                text: "Importar Carpeta Musical"
                onClicked: dialogoMinero.abrirSelector()
            }
        }
    }

    RowLayout {
        anchors.fill: parent
        spacing: 0

        // Menú lateral
        Frame {
            Layout.preferredWidth: 180
            Layout.fillHeight: true
            ColumnLayout {
                anchors.fill: parent
                spacing: 8
                Button { text: "Canciones"; Layout.fillWidth: true; onClicked: stackVistas.currentIndex = 0 }
                Button { text: "Álbumes"; Layout.fillWidth: true; onClicked: stackVistas.currentIndex = 1 }
                Button { text: "Artistas y Grupos"; Layout.fillWidth: true; onClicked: stackVistas.currentIndex = 2 }
                Button { text: "Buscador SQL"; Layout.fillWidth: true; onClicked: stackVistas.currentIndex = 3 }
                Item { Layout.fillHeight: true }
            }
        }

        // Contenido dinámico
        StackLayout {
            id: stackVistas
            Layout.fillWidth: true
            Layout.fillHeight: true

            VistaCanciones {
                id: vistaCanciones
                onReproducirRola: (titulo, artista, ruta) => barraReproductor.cargarPista(titulo, artista, ruta)
            }
            VistaAlbumes { id: vistaAlbumes }
            VistaArtistas { id: vistaArtistas }
            VistaBuscadorAvanzado { id: vistaBuscador }
        }
    }

    footer: ColumnLayout {
        spacing: 0
        BarraReproductor {
            id: barraReproductor
            Layout.fillWidth: true
        }
        ToolBar {
            Layout.fillWidth: true
            Label {
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 10
                text: biblioteca.mensajeEstado
            }
        }
    }
}
