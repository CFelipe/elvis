#include "documento.h"

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

    viewport = Ponto(0, 0);

    op = DOIS_CRIACAO;

    gradeSep = 50;

    espessuraLinha = 1;
    preenchimento = true;
    linha = true;
    grade = false;
}

/*
void Documento::salvarElv() {
    ofstream myfile;
    myfile.open ("example.txt");
    myfile << "Writing this to a file.\n";
    myfile.close();
}
*/

/*
void Documento::exportarSVG() {
    <?xml version="1.0" standalone="no"?>
    <!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN"  "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">
    <svg width="5cm" height="4cm" version="1.1" xmlns="http://www.w3.org/2000/svg">
    <rect x="0.5cm" y="0.5cm" width="2cm" height="1cm"/>
    <rect x="0.5cm" y="2cm" width="1cm" height="1.5cm"/>
    <rect x="3cm" y="0.5cm" width="1.5cm" height="2cm"/>
    <rect x="3.5cm" y="3cm" width="1cm" height="0.5cm"/>
    </svg>
}
*/
