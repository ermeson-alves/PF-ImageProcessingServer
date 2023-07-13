#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// Opencv imports:
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>

#define PORT 3045
#define BUFFER_SIZE 4096

void send_image(int client_socket);

int main()
{
    int client_socket;
    struct sockaddr_in server_address;

    // Criar socket
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Falha ao criar socket");
        exit(EXIT_FAILURE);
    }

    // Endereço do servidor
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    // Converter IP para forma binaria
    if (inet_pton(AF_INET, "192.168.1.108", &(server_address.sin_addr)) <= 0) {
        perror("Endereço invalido");
        exit(EXIT_FAILURE);
    }

    // Conectar ao servidor
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Falha ao se conectar");
        exit(EXIT_FAILURE);
    }

    send_image(client_socket);

    close(client_socket);

    return 0;
}

void send_image(int client_socket)
{
    char buffer[BUFFER_SIZE];
    ssize_t num_bytes;

    // Declara um objeto do tipo "cv2.Mat"
    cv::Mat image;
    image = cv::imread("image2.jpg", 1);

    if (image.empty()) {
        perror("Falha ao abrir imagem");
        exit(EXIT_FAILURE);
    }


    // ENVIAR IMAGEM PARA O SERVIDOR:

    // cv::resize(image, image, cv::Size(480, 320));

    // Codificação em buffer de bytes:
    std::vector<uchar> encode_image;
    cv::imencode(".jpg", image, encode_image);
    //Enviar tamanho do buffer de bytes para o servidor:
    size_t image_size = encode_image.size();
    printf("Tamanho da imagem enviada para o servidor: %ld\n", image_size);
    send(client_socket, &image_size, sizeof(size_t), 0);

    // Envie o buffer de bytes para o servidor
    send(client_socket, encode_image.data(), encode_image.size(), 0);
    printf("Imagem enviada para o servidor\n");


    // RECEBER IMAGEM DO SERVIDOR:

    // Receba o tamanho do buffer de bytes
    size_t image_size2;
    recv(client_socket, &image_size2, sizeof(size_t), 0);

    // Receba o buffer de bytes contendo a imagem
    std::vector<uchar> received_image(image_size2);
    recv(client_socket, received_image.data(), image_size2, 0);

    // Decodifique o buffer de bytes em uma imagem usando OpenCV
    cv::Mat image_recv = cv::imdecode(received_image, cv::IMREAD_COLOR);
    if (image_recv.empty()) {
        perror("Falha ao decodificar imagem");
        exit(EXIT_FAILURE);
    }




    // SALVAR IMAGEM RECEBIDA:

    cv::imwrite("received_image.jpg", image_recv);


    printf("Imagem recebida do servidor e salva como 'received_image.jpg'\n");
}
