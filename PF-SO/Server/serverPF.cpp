#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

// Opencv imports:
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>


#define PORT 3045
#define BUFFER_SIZE 4096

void handle_client(int client_socket);

int main()
{

    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_length = sizeof(client_address);

    // Criar socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Falha ao criar socket");
        exit(EXIT_FAILURE);
    }

    // Endereço do servidor
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    // Vincular socket ao IP e porta
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Falha ao vincular socket");
        exit(EXIT_FAILURE);
    }

    // Esperar por conexões
    if (listen(server_socket, 5) < 0) {
        perror("Falha ao se conectar");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    while (1) {
        // Aceitar conexão
        if ((client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_length)) < 0) {
            perror("Falha ao aceitar conexao");
            exit(EXIT_FAILURE);
        }
    
        // Criar processo filho para atender ao cliente
        pid_t child_pid = fork();

        if (child_pid < 0) {
            perror("Falha ao dar fork");
            exit(EXIT_FAILURE);
        } else if (child_pid == 0) {
            // Processo filho
            close(server_socket);
            handle_client(client_socket);
            exit(EXIT_SUCCESS);
        } else {
            // Processo pai
            close(client_socket);
        }
    }

    return 0;
}

void handle_client(int client_socket)
{
    char buffer[BUFFER_SIZE];
    ssize_t num_bytes;

    //RECEBER IMAGEM DO CLIENTE: 

    // Receba o tamanho do buffer de bytes
    size_t image_size;
    recv(client_socket, &image_size, sizeof(size_t), 0);

    // Receba o buffer de bytes contendo a imagem
    std::vector<uchar> received_image(image_size);
    recv(client_socket, received_image.data(), image_size, 0);

    // Decodifique o buffer de bytes em uma imagem usando OpenCV
    cv::Mat image = cv::imdecode(received_image, cv::IMREAD_COLOR);
    if (image.empty()) {
        perror("Falha ao decodificar imagem");
        exit(EXIT_FAILURE);
    }


    // ENVIAR IMAGEM PARA O CLIENTE

    // Pré-processamento:
    // First we declare the variables we are going to use
    cv::Mat grad, src, src_gray;
    const std::string window_name = "Resultado: Filtro da média e Filtro Sobel";
    int ksize = 3;
    int scale = 0.5;
    int delta = 1;
    int ddepth = CV_16S;

    // Remoçao de ruido
    cv::GaussianBlur(image, src, cv::Size(3,3), 0, 0, cv::BORDER_DEFAULT);
    // Conversão para escala de cinza
    cv::cvtColor(src, src_gray, cv::COLOR_BGR2GRAY);

    cv::Mat grad_x, grad_y;
    cv::Mat abs_grad_x, abs_grad_y;
    cv::Sobel(src_gray, grad_x, ddepth, 1, 0, ksize);
    cv::Sobel(src_gray, grad_y, ddepth, 0, 1, ksize);

    // Convertendo de volta para CV_8U
    cv::convertScaleAbs(grad_x, abs_grad_x);
    cv::convertScaleAbs(grad_y, abs_grad_y);
    cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
    // cv::imshow(window_name, grad);
    // cv::waitKey(0); 

    // Codificação em buffer de bytes:
    std::vector<uchar> encode_image;
    cv::imencode(".jpg", grad, encode_image);
    
    //Enviar tamanho do buffer de bytes para o servidor:

    size_t image_size2 = encode_image.size();
    send(client_socket, &image_size2, sizeof(size_t), 0);

    // Envie o buffer de bytes para o servidor
    send(client_socket, encode_image.data(), encode_image.size(), 0);



    // file = fopen(num_child, "rb");
    // if (file == NULL) {
    //     perror("Falha ao abrir imagem");
    //     exit(EXIT_FAILURE);
    // }

    
    // while ((num_bytes = fread(buffer, sizeof(char), BUFFER_SIZE, file)) > 0) {
    //     send(client_socket, buffer, num_bytes, 0);
    // }

    // fclose(file);

    printf("Imagem mandada de volta para o client\n");

    close(client_socket);
}
