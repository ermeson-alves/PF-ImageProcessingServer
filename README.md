# PF-ImageProcessingServer
Repositório para o projeto final da disciplina de Sistemas Operacionais, envolvendo uma aplicação cliente-servidor que pré-processa imagens de clientes e as envia de volta.

## Grupo
Carlos Victor Gonçalves Moura - 515016

Antônio Ermeson Pereira Alves - 511473

Weatherly Moura Albuquerque - 511792

## Funcionamento

![fluxograma3](https://github.com/ermeson-alves/PF-ImageProcessingServer/assets/109031995/63d845fc-3de2-4b3d-8491-ab7884985bf4)

A aplicação permite que múltiplos clientes enviem imagens simultaneamente ao servidor por meio do uso da chamada de sistema Fork(). Quando um cliente envia uma solicitação de conexão ao servidor, o servidor aceitará a conexão com o seu processo principal e então esse processo criará um filho para lidar com o cliente. Após a criação do filho, o pai volta a receber conexões e criar mais filhos. Já cada processo filho criado receberá uma imagem enviada pelo cliente, pré-processará ela usando o próprio espaço de memória, e então enviará a imagem pré-processada para o cliente. O cliente salva essa imagem no próprio diretório e então o processo filho encerra a conexão e a si mesmo.

## Biblioteca Opencv:
[OpenCV](https://opencv.org/) É uma biblioteca de visão computacional de código aberto, sob licença Apache, e multiplataforma (as interfaces C++, Python e Java suportam Linux, MacOS, Windows, iOS e Android). Além disso ela é uma biblioteca otimizada com focos em aplicações de tempo real. Neste Projeto a utilizamos para ler, codificar/decodificar, processar e salvar imagens arbitrárias.
Utilizamos a [Documentação oficial](https://docs.opencv.org/4.8.0/) com a interface C++ para o projeto rodando em um sistema Linux.

Tutorial de instalação seguido: [How to Install opencv in C++ on Linux?](https://www.geeksforgeeks.org/how-to-install-opencv-in-c-on-linux/)

## Compilação
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
