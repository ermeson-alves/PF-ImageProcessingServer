#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

// Opencv imports:
#include <opencv2/opencv.hpp>




#define PORT 3045
#define BUFFER_SIZE 1024

void handle_client(int client_socket, char* num_child);

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
        perror("Falha ao vincular");
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
            perror("Falha ao aceitar");
            exit(EXIT_FAILURE);
        }

        // Obter número de processos filho

        char buffer[BUFFER_SIZE];
        char word[10];

        FILE* input;

        input = popen("bash_pid=$$", "w");
        fprintf(input, "children=`ps -eo ppid | grep -w $bash_pid`\n");
        fprintf(input, "export num_children=`echo $children | wc -w`\n");

        if (input == NULL) {
            perror("Falha ao executar comando");
            exit(EXIT_FAILURE);
        }

        FILE* output;

        output = popen("echo $num_children", "r");

        if (output == NULL) {
            perror("Falha ao executar comando");
            exit(EXIT_FAILURE);
        }
        else {
            fgets(word, 10, output);
        }

        printf("%s/n", word);

        // Criar processo filho para atender ao cliente
        pid_t child_pid = fork();

        if (child_pid < 0) {
            perror("Falha ao dar fork");
            exit(EXIT_FAILURE);
        } else if (child_pid == 0) {
            // Processo filho
            close(server_socket);
            handle_client(client_socket, word);
            exit(EXIT_SUCCESS);
        } else {
            // Processo pai
            close(client_socket);
        }
    }

    return 0;
}

void handle_client(int client_socket, char* num_child)
{
    char buffer[BUFFER_SIZE];
    ssize_t num_bytes;

    strcat(num_child, ".jpg");

    // Receber imagem do cliente
    FILE *file = fopen(num_child, "wb");
    if (file == NULL) {
        perror("Falha ao abrir imagem");
        exit(EXIT_FAILURE);
    }

    while ((num_bytes = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 528) {
        fwrite(buffer, sizeof(char), num_bytes, file);
        printf("%ld\n", num_bytes);
    }



    fclose(file);
    printf("Imagem recebida e salva como %s.jpg'\n", num_child);

    // Enviar imagem para o cliente

    file = fopen(num_child, "rb");
    if (file == NULL) {
        perror("Falha ao abrir imagem");
        exit(EXIT_FAILURE);
    }

    // O processamento via opencv deve ficar aqui:
    // ...


    while ((num_bytes = fread(buffer, sizeof(char), BUFFER_SIZE, file)) > 0) {
        send(client_socket, buffer, num_bytes, 0);
    }

    fclose(file);
    printf("Imagem mandada de volta para o client\n");

    close(client_socket);
}