#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define MAX_REQUEST_SIZE 2048

void send_html(int client_socket) {
    char *html_content = "<html><head><title>Servidor Web Simples</title></head><body><h1>Olá, Mundo!</h1></body></html>";
    char response[1024];
    sprintf(response, "HTTP/1.1 200 OK\r\nContent-Length: %ld\r\nContent-Type: text/html\r\n\r\n%s", strlen(html_content), html_content);
    send(client_socket, response, strlen(response), 0);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    char buffer[MAX_REQUEST_SIZE] = {0};

    // Criação do socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Falha ao criar o socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Associa o socket a uma porta
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("Falha ao associar o socket à porta");
        exit(EXIT_FAILURE);
    }

    // Marca o socket para aguardar conexões
    if (listen(server_fd, 3) < 0) {
        perror("Erro na escuta de conexões");
        exit(EXIT_FAILURE);
    }

    printf("Servidor aguardando conexões na porta %d\n", PORT);

    while (1) {
        // Aceita a conexão
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
            perror("Erro na aceitação da conexão");
            exit(EXIT_FAILURE);
        }

        // Recebe a requisição do cliente
        read(new_socket, buffer, MAX_REQUEST_SIZE);
        printf("Requisição:\n%s\n", buffer);

        // Envia a página HTML para o cliente
        send_html(new_socket);
        printf("Página HTML enviada ao cliente\n");

        // Fecha o socket do cliente
        close(new_socket);
    }
    return 0;
}
