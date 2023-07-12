#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 3045
#define BUFFER_SIZE 1024

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
    if (inet_pton(AF_INET, "192.168.0.225", &(server_address.sin_addr)) <= 0) {
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

    // Enviar imagem pro servidor
    FILE *file = fopen("image.jpg", "rb");

    if (file == NULL) {
        perror("Falha ao abrir imagem");
        exit(EXIT_FAILURE);
    }

    while ((num_bytes = fread(buffer, sizeof(char), BUFFER_SIZE, file)) > 0) {
        send(client_socket, buffer, num_bytes, 0);
    }

    fclose(file);
    printf("Imagem enviada para o servidor\n");

    // Receber imagem do servidor
    file = fopen("received_image.jpg", "wb");
    if (file == NULL) {
        perror("Falha ao abrir imagem");
        exit(EXIT_FAILURE);
    }

    while ((num_bytes = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        fwrite(buffer, sizeof(char), num_bytes, file);
    }

    fclose(file);
    printf("Imagem recebida do servidor e salva como 'received_image.jpg'\n");
}
