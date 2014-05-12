#include "documento.h"
#include "polilinha.h"

Documento::Documento() {
    Camada* camada1 = new Camada("Camada 1");
    camadaSelecionada = camada1;
    camadas.append(camada1);

    linhaColorSelecionada[0] = 0;
    linhaColorSelecionada[1] = 0;
    linhaColorSelecionada[2] = 0;
    linhaColorSelecionada[3] = 0;

    fillColorSelecionada[0] = 1;
    fillColorSelecionada[1] = 0;
    fillColorSelecionada[2] = 0;
    fillColorSelecionada[3] = 0;

    estiloLinha = "-";

    viewport = Ponto(0, 0);

    canvasW = 800;
    canvasH = 600;

    op = CRIACAO;
    forma = ELIPSE;

    gradeSep = 50;

    desenha = true;
    espessuraLinha = 1;
    preenchimento = true;
    linha = true;
    grade = false;

    selecaoMultipla = false;
}

void Documento::deletarSelecionados() {
    int i;
    for(i = 0; i < camadas.size(); i++) {
        QList<Objeto*>::iterator j = camadas.at(i)->objetos->begin();
        while (j != camadas.at(i)->objetos->end()) {
            if ((*j)->selecionado) {
                j = camadas.at(i)->objetos->erase(j);
            } else {
                ++j;
            }
        }
    }
}

/*
void Documento::salvarElv() {
}
*/

bool Documento::exportarSVG(const QString &fileName) {
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        //QMessageBox::warning(this, QString("Elvis"), QString("Não foi possível exportar o arquivo"));
        return false;
    }

    QTextStream out(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    out << "<?xml version=\"1.0\" standalone=\"no\"?>\n";
    out << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"  \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n";
    out << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"355\" height=\"155\">\n";

    QString estiloLinha;
    QString corLinha;
    QString corFill;
    Objeto* aux;
    int i, j, k;
    for(i = 0; i < camadas.size(); i++) {
        GLint sizeJ = camadas.at(i)->objetos->size();
        for(j = 0; j < sizeJ; j++) {
            aux = camadas.at(i)->objetos->at(j);

            if(aux->linha) {
                corLinha = QColor::fromRgbF(aux->colorLine[0],
                                            aux->colorLine[1],
                                            aux->colorLine[2]).name();

                if(estiloLinha != "-") {
                    estiloLinha = dasharrayToSVG(aux->estiloLinha);
                }
            } else {
                corLinha = "none";
            }

            if(aux->preenchido) {
                corFill = QColor::fromRgbF(aux->colorfill[0],
                                           aux->colorfill[1],
                                           aux->colorfill[2]).name();
            } else {
                corFill = "none";
            }

            if(aux->tipo == CIRCULO) {
                Circulo *c = dynamic_cast <Circulo *>(aux);
                out << QString("<circle cx=\"%1\" cy=\"%2\" r=\"%3\" ")
                       .arg(QString::number(c->getXc()),
                            QString::number(c->getYc()),
                            QString::number(c->getRaio()));

            } else if(aux->tipo == RETANGULO) {
                /* <rect x="0.5cm" y="0.5cm" width="2cm" height="1cm"/> */

                Retangulo *r = dynamic_cast <Retangulo *>(aux);
                out << QString("<rect x=\"%1\" y=\"%2\" width=\"%3\" height=\"%4\" \n")
                       .arg(QString::number(r->boundsMin().x),
                            QString::number(r->boundsMax().y),
                            QString::number(r->w()),
                            QString::number(r->h()));
            } else if(aux->tipo == POLILINHA) {
                Polilinha *p = dynamic_cast <Polilinha *>(aux);
                out << QString("<polyline points=\"");

                for(k = 0; k < p->vertices.size(); k++) {
                    out << QString("%1,%2").arg(QString::number(p->vertices.at(k)->p.x),
                                                QString::number(p->vertices.at(k)->p.y));
;
                    if(k != p->vertices.size() - 1) {
                        out << QString(" ");
                    }
                }

                out << "\" ";

            }

            out << QString("fill=\"%1\" stroke=\"%2\" stroke-width=\"%3\" />\n")
                   .arg(corFill,
                        corLinha,
                        QString::number(aux->espessuraLinha));
        }
    }

    out << "</svg>";

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    return true;
}

QString Documento::dasharrayToSVG(QString dash) {
    QString out = "";
    int i, count = 0;
    for(i = 0; i < dash.size(); i++) {
        if(i > 1 && dash[i] != dash[i - 1]) {
            out += QString::number(count);
            out += ",";
            count = 0;
        }
        count++;
    }

    out += QString::number(count);

    return out;
}
