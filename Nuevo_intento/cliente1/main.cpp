#include <iostream>
#include <winsock2.h>
using namespace std;

class mensaje{
    public:
    char oracion[1000];
    mensaje(){
        memset(oracion,0,999);//limpia el mensaje
    }
    void imprimir_mensaje_recep(){cout<<"receptado: "<<oracion<<endl;}
    void imprimir_mensaje_envi(){cout<<"enviar: "<<oracion<<endl;}
    void ingresar_mensaje(){cin.getline(oracion,256);}
};
class Socket{
    public:
    SOCKET s;//crea un Socket
    Socket(){
        s=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    }
    bool revision_Socket(){
        if(s==INVALID_SOCKET){
            cout<<"Error en Winsock - Error en Creacion de Socket"<<endl;;
            WSACleanup();
            system("PAUSE");
            return 1;
        }
        return 0;
    }

};
class Hosts{
    public:
    struct hostent *host;
    Hosts(){
        host=gethostbyname("192.168.1.13");
    }
    bool revision_hosts(){
        if(host==NULL){
            cout<<"Fallo conectandose al host"<<endl;;
            WSACleanup();
            system("PAUSE");
            return 1;
        }
        return 0;
    }
};
class Sockaddrs{
    public:
    SOCKADDR_IN SockAddr;
    Sockaddrs(Hosts &h){
        SockAddr.sin_port=htons(9999);
        SockAddr.sin_family=AF_INET;
        SockAddr.sin_addr.s_addr=*((unsigned long*)h.host->h_addr);
    }
    bool conectar(Socket s){
        if(connect(s.s,(SOCKADDR*)(&SockAddr),sizeof(SockAddr))!=0){
            cout<<"Failed to establish connection with server"<<endl;
            WSACleanup();
            system("PAUSE");
            return 1;
        }
        return 0;
    }
};
int receive_msg(SOCKET &Socket){
        mensaje m;
        int inDataLength=recv(Socket,m.oracion,1000,0);
        m.imprimir_mensaje_recep();
        int error=WSAGetLastError();
        if(error!=WSAEWOULDBLOCK && error!=0){;
            cout<<"codigo de error Winsock: "<<error<<endl;
            cout<<"Servidor desconectado!"<<endl;
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
        memset(m.oracion,0,999);
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
    if(revision_Winsock()) return 0;
    Socket s;
    if(s.revision_Socket()) return 0;
    Hosts h;
    if(h.revision_hosts()) return 0;
    // Setup our socket address structure
    Sockaddrs r(h);
    // Attempt to connect to server
    if(r.conectar(s)) return 0;
    // If iMode!=0, non-blocking mode is enabled.
    u_long iMode=1;
    ioctlsocket(s.s,FIONBIO,&iMode);
    // Enciar Mensaje
    while(true){
        send_msg(s.s);
    }
    WSACleanup();
    system("PAUSE");
    return 0;
}
