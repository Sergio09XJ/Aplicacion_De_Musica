import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: raiz
    property var camposDisponibles: []
    property string sqlGenerado: ""
    property var resultados: []

    ListModel { id: modeloCriterios }

    function actualizarCampos() {
        camposDisponibles = biblioteca.obtenerCamposDeEntidad(comboEntidad.currentText)
    }

    Component.onCompleted: actualizarCampos()

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 8

        RowLayout {
            Label { text: "Entidad base:" }
            ComboBox {
                id: comboEntidad
                model: biblioteca.tablasDisponibles
                onCurrentTextChanged: {
                    modeloCriterios.clear()
                    raiz.actualizarCampos()
                }
            }
            Label { text: "Combinar con:" }
            ComboBox { id: comboConector; model: ["AND", "OR"] }
            Button {
                text: "+ Criterio"
                onClicked: {
                    if (camposDisponibles.length > 0) {
                        modeloCriterios.append({
                            columna: camposDisponibles[0].nombre,
                            operador: "CONTIENE",
                            valor: ""
                        })
                    }
                }
            }
            Button {
                text: "Ejecutar Búsqueda"
                onClicked: {
                    var listaCrit = []
                    for (var i = 0; i < modeloCriterios.count; i++) {
                        var item = modeloCriterios.get(i)
                        listaCrit.push({
                            entidad: comboEntidad.currentText,
                            columna: item.columna,
                            operador: item.operador,
                            valor: item.valor
                        })
                    }
                    var resp = biblioteca.ejecutarBusquedaAvanzada({
                        entidadBase: comboEntidad.currentText,
                        campos: [],
                        criterios: listaCrit,
                        conector: comboConector.currentText
                    })
                    sqlGenerado = resp.sql || resp.error || ""
                    resultados = resp.filas || []
                }
            }
        }

        // Lista dinámica de criterios
        ListView {
            Layout.fillWidth: true
            Layout.preferredHeight: 140
            clip: true
            model: modeloCriterios
            delegate: RowLayout {
                width: ListView.view.width
                ComboBox {
                    model: camposDisponibles.map(c => c.nombre)
                    onCurrentTextChanged: modeloCriterios.setProperty(index, "columna", currentText)
                }
                ComboBox {
                    model: ["CONTIENE", "=", "!=", "<", ">", "<=", ">=", "EMPIEZA_CON", "TERMINA_CON"]
                    onCurrentTextChanged: modeloCriterios.setProperty(index, "operador", currentText)
                }
                TextField {
                    placeholderText: "Valor a buscar..."
                    Layout.fillWidth: true
                    onTextChanged: modeloCriterios.setProperty(index, "valor", text)
                }
                Button { text: "X"; onClicked: modeloCriterios.remove(index) }
            }
        }

        // SQL Parametrizado generado
        TextArea {
            Layout.fillWidth: true
            Layout.preferredHeight: 60
            readOnly: true
            text: sqlGenerado
            placeholderText: "Aquí aparecerá la consulta SQL parametrizada generada..."
        }

        // Resultados
        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            model: resultados
            delegate: Frame {
                width: ListView.view.width
                Label {
                    text: JSON.stringify(modelData)
                    wrapMode: Text.Wrap
                    width: parent.width
                }
            }
        }
    }
}
