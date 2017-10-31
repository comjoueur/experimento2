#include<io.h>
#include<stdio.h>
#include<winsock2.h>
#include<iostream>
#include <thread>
using namespace std;
class mensaje{
    public:
    char oracion[1000];
    mensaje(){
        memset(oracion,0,999);//limpia el mensaje
    }
    void imprimir_mensaje_recep(){cout<<"receptado: "<<oracion<<endl;}
    void imprimir_mensaje_envi(){cout<<"enviar: "<<oracion<<endl;}
    void ingresar_mensaje(){cin.getline(oracion,1000);}
};
class Socket{
    public:
    SOCKET s;//crea un Socket
    void constructor (){
        s=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    }
    void constructor2(Socket d,sockaddr_in &client,int &c){
        s = accept(d.s , (struct sockaddr *)&client, &c);
    }
    bool revision_Socket(){
        if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
        {
            printf("Could not create socket : %d" , WSAGetLastError());
        }
    }
    bool revision_Socket2(){
        if (s == INVALID_SOCKET){
            printf("accept failed with error code : %d" , WSAGetLastError());
        }
    }

};
class Sockaddrs{
    public:
    struct sockaddr_in r;
    void constructor(){
        r.sin_family = AF_INET;
        r.sin_addr.s_addr = INADDR_ANY;
        r.sin_port = htons( 8888 );
    }
};
int receive_msg(SOCKET &Socket){
        mensaje m;
        int inDataLength=recv(Socket,m.oracion,1000,0);
        m.imprimir_mensaje_recep();
        int error=WSAGetLastError();
        if(error!=WSAEWOULDBLOCK && error!=0){
            cout<<"codigo de error Winsock: "<<error<<endl;
            cout<<"Servidor desconectado!"<<endl;
            return 0;
            // Apaga el socket
            //shutdown(Socket,SD_SEND);
            // Cierra el socket
            //closesocket(Socket);
            //break;
        }
}
int send_msg(SOCKET &s){
    mensaje m;
        cout<<"SEND: ";
        m.ingresar_mensaje();
        send(s,m.oracion,strlen(m.oracion),0);
        memset(m.oracion,0,255);
}
bool revision_Winsock(){
    WSADATA WsaDat;
    if(WSAStartup(MAKEWORD(2,2),&WsaDat)!=0){
        cout<<"Error en Winsock- Inicializacion fallida"<<endl;
        WSACleanup();
        system("PAUSE");
        return 1;
    }
    return 0;
}

int main(){
    revision_Winsock();
    Socket s , new_socket;
    Sockaddrs server , client;
    int c;
    //Create a socket
    s.revision_Socket();
    //Prepare the sockaddr_in structure
    server.constructor();
    //Bind
    if( bind(s.s ,(struct sockaddr *)&server.r , sizeof(server.r)) == SOCKET_ERROR)
    {
        cout<<"falla en bind "<<WSAGetLastError()<<endl;
        return 0;
    }
    //Listen to incoming connections
    listen(s.s , 3);

    //Accept and incoming connection
    puts("Esperando conexiones...");

    c = sizeof(struct sockaddr_in);
    new_socket.constructor2(s,client.r,c);
    new_socket.revision_Socket2();

    cout<<"conexion aceptada"<<endl;
    receive_msg(new_socket.s);
    while(true){
        receive_msg(new_socket.s);
        Sleep(4000);
    }
    closesocket(s.s);
    WSACleanup();
    return 0;
}
