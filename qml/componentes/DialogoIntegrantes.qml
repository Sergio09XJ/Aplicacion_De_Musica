import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: dialogo
    title: "Integrantes del Grupo"
    standardButtons: Dialog.Close
    anchors.centerIn: Overlay.overlay
    width: 420
    height: 350

    property int idGroup: 0
    property var integrantes: []
    property var todasPersonas: []

    function abrir(idG, nombreGrupo) {
        idGroup = idG
        title = "Integrantes: " + nombreGrupo
        todasPersonas = biblioteca.obtenerPersonas()
        recargar()
        open()
    }

    function recargar() {
        integrantes = biblioteca.obtenerIntegrantesGrupo(idGroup)
    }

    ColumnLayout {
        anchors.fill: parent
        RowLayout {
            ComboBox {
                id: comboPersonas
                Layout.fillWidth: true
                model: todasPersonas.map(p => p.stage_name + " (" + p.real_name + ")")
            }
            Button {
                text: "Agregar"
                onClicked: {
                    if (todasPersonas.length > 0) {
                        var idP = todasPersonas[comboPersonas.currentIndex].id_person
                        if (biblioteca.agregarIntegrante(idP, idGroup)) recargar()
                    }
                }
            }
        }
        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            model: integrantes
            delegate: RowLayout {
                width: ListView.view.width
                Label { text: modelData.stage_name + " (" + modelData.real_name + ")"; Layout.fillWidth: true }
                Button {
                    text: "Quitar"
                    onClicked: { if (biblioteca.quitarIntegrante(modelData.id_person, idGroup)) recargar() }
                }
            }
        }
    }
}
