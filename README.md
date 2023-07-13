# PF-ImageProcessingServer
Repositório para o projeto final da disciplina de Sistemas Operacionais, envolvendo um servidor que pré-processa imagens de clientes.

## Biblioteca Opencv:
[OpenCV](https://opencv.org/) É uma biblioteca de visão computacional de código aberto, sob licença Apache, e multiplataforma (as interfaces C++, Python e Java suportam Linux, MacOS, Windows, iOS e Android). Além disso ela é uma biblioteca otimizada com focos em aplicações de tempo real. Neste Projeto a utilizamos para ler, codificar/decodificar, processar e salvar imagens arbitrárias.
Utilizamos a [Documentação oficial](https://docs.opencv.org/4.8.0/) com a interface C++ para o projeto rodando em um sistema Linux.

Tutorial de instalação seguido: [How to Install opencv in C++ on Linux?](https://www.geeksforgeeks.org/how-to-install-opencv-in-c-on-linux/)

## Compilação:
A hirarquia de arquivos é a seguinte:
  ```
    ├── .
    ├── PF-SO                    # Diretório que armazena arquivos do Cliente e Servidor
    │   ├── Server               # Sub-diretório para a lógica do servidor
    |        ├── CMakeLists.txt  # Arquivo necessário para o processo de compilação
    |        └── serverPF.cpp    # Arquivo C++ do servidor
    │   └── Client               # Sub-diretório para a lógica do cliente
    |        ├── CMakeLists.txt  
    |        ├── image2.jpg      # Imagem de teste
    └──      └── clientPF.cpp    # Arquivo C++ do servidor
  ```

Para compilar os arquivos C++ é necessário ter instalado também o cmake. Dentro de cada subdiretório de ```PF-SO``` rodar ```cmake .``` e em seguida ```make```. Essa etapa consta no tutorial de instalação da opencv informado acima.
