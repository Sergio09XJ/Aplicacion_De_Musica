import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../componentes"


Item {
    id: raiz
    property var albumes: []

    function cargar() { albumes = biblioteca.obtenerAlbumes() }
    Component.onCompleted: cargar()

    DialogoEditarAlbum {
        id: dialogoAlbum
        onGuardado: raiz.cargar()
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 12

        RowLayout {
            Label { text: "Álbumes (" + albumes.length + ")"; font.bold: true; font.pixelSize: 16; Layout.fillWidth: true }
            Button { text: "Actualizar"; onClicked: raiz.cargar() }
        }

        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            model: albumes
            spacing: 4

            delegate: Frame {
                width: ListView.view.width
                RowLayout {
                    anchors.fill: parent
                    ColumnLayout {
                        Layout.fillWidth: true
                        Label { text: modelData.name + " (" + modelData.year + ")"; font.bold: true }
                        Label { text: "Intérprete: " + (modelData.artista || "Desconocido") + " | Ruta: " + modelData.path; opacity: 0.75 }
                    }
                    Button { text: "Editar"; onClicked: dialogoAlbum.abrir(modelData) }
                    Button {
                        text: "Borrar"
                        onClicked: { if (biblioteca.eliminarAlbum(modelData.id_album)) raiz.cargar() }
                    }
                }
            }
        }
    }
}
