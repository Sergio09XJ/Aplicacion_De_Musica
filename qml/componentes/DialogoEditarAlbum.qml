import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: dialogo
    title: "Editar Álbum"
    standardButtons: Dialog.Save | Dialog.Cancel
    anchors.centerIn: Overlay.overlay
    width: 400

    property int idAlbum: 0
    property var listaPerformers: []
    signal guardado()

    function abrir(album) {
        idAlbum = album.id_album
        campoName.text = album.name || ""
        campoYear.text = album.year || 0
        campoPath.text = album.path || ""

        listaPerformers = biblioteca.obtenerPerformers()
        var idx = 0
        for (var i = 0; i < listaPerformers.length; i++) {
            if (listaPerformers[i].id_performer === album.id_performer) { idx = i; break }
        }
        comboPerf.currentIndex = idx
        open()
    }

    onAccepted: {
        var idPerf = listaPerformers.length > 0 ? listaPerformers[comboPerf.currentIndex].id_performer : 1
        if (biblioteca.editarAlbum(idAlbum, campoName.text, parseInt(campoYear.text) || 0, campoPath.text, idPerf)) {
            guardado()
        }
    }

    GridLayout {
        columns: 2
        anchors.fill: parent
        Label { text: "Nombre:" }     ; TextField { id: campoName; Layout.fillWidth: true }
        Label { text: "Intérprete:" } ; ComboBox  { id: comboPerf; model: listaPerformers.map(p => p.name); Layout.fillWidth: true }
        Label { text: "Año:" }        ; TextField { id: campoYear; Layout.fillWidth: true }
        Label { text: "Ruta:" }       ; TextField { id: campoPath; Layout.fillWidth: true }
    }
}
