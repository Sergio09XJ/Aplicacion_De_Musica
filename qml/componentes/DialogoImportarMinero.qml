import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

Item {
    id: raiz
    signal importacionTerminada()

    function abrirSelector() { folderDialog.open() }

    FolderDialog {
        id: folderDialog
        title: "Selecciona la carpeta con archivos MP3"
        onAccepted: {
            var res = biblioteca.importarDirectorio(selectedFolder)
            textoResumen.text = res.error
                ? "Error: " + res.error
                : "Escaneados: " + res.escaneados +
                  "\nInsertadas: " + res.insertadas +
                  "\nOmitidas (duplicadas): " + res.omitidas +
                  "\nErrores: " + res.errores
            dialogoResultado.open()
            raiz.importacionTerminada()
        }
    }

    Dialog {
        id: dialogoResultado
        title: "Resultado del Minero"
        standardButtons: Dialog.Ok
        anchors.centerIn: Overlay.overlay
        Label { id: textoResumen }
    }
}
