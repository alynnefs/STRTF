#include <stdio.h>
#include <pthread.h>

int a=1, b=0, c=0, d=0; //variáveis globais
int canal[8] = {-1,-1,-1,-1,-1,-1,-1,-1};


/*Função de recebimento*/
void receive_mes(int *buff, int c){
    while(canal[c] == -1){} // faz 'nada' enquanto a posição c do vetor canal for igual a -1 (vazio)
    *buff = canal[c]; // valor existente na posição c do vetor canal é copiado para o buffer
    canal[c] = -1; // posição c do vetor canal recebe -1, liberando para novo recebimento
    return;
}

/*Função de envio síncrono*/
void send_mes_sync(int *buff, int c){
    canal[c] = *buff; // valor do buffer é copiado na posição c do vetor canal
    while(canal[c] != -1){} // faz 'nada' enquanto a posição c do vetor canal for diferente de -1 (ocupado)
    return;
}

int converte_valor(unsigned char bytex2, unsigned char bytex1){
    return bytex2*256 + bytex1;
}

/* Extrai os dois bytes da word passada, o MSB e o LSB */ 
void encontra_bytes(int valor, unsigned char * byte2, unsigned char * byte1){
    *byte2 = ((valor >> 8) & 255);
    *byte1 = (valor & 255);
    return;
}    

void * threadA() { // Envia 10 para E
    unsigned char IDprocess = 4;
    unsigned char mensagem = 10;
    int valor;
    int i = 0;
    while(i <= 6) {
        while(a == 0){}
        valor = converte_valor(IDprocess, mensagem);
        //printf("\nValor apos mascaramento:%d", valor);
        send_mes_sync(&valor,0);
        a = 0;
        b = 1;
        i++;
    }
}

void * threadB() { // Envia 20 para F
    unsigned char IDprocess = 5;
    unsigned char mensagem = 20;
    int valor;
    int i = 0;
    while(i <= 6) {
        while(b == 0){}
        valor = converte_valor(IDprocess, mensagem);
        //printf("\nValor apos mascaramento:%d", valor);
        send_mes_sync(&valor,1);
        b = 0;
        c = 1;
        i++;
    }
}

void * threadC() { // Envia 30 para G
    unsigned char IDprocess = 6;
    unsigned char mensagem = 30;
    int valor;
    int i = 0;
    while(i <= 6) {
        while(c == 0){}
        valor = converte_valor(IDprocess, mensagem);
        //printf("\nValor apos mascaramento:%d", valor);
        send_mes_sync(&valor,2);
        c = 0;
        d = 1;
        i++;
    }
}

void * threadD() { // Envia 40 para H
    unsigned char IDprocess = 7;
    unsigned char mensagem = 40;
    int valor;
    int i = 0;
    while(i <= 9) {
        while(d == 0){}
        valor = converte_valor(IDprocess, mensagem);
        //printf("\nValor apos mascaramento:%d", valor);
        send_mes_sync(&valor,3);
        d = 0;
        a = 1;
        i++;
    }
}

void * threadE() { // Recebe 10 de A
    int valor;
    while(1){
        receive_mes(&valor,4);
        printf("\nTHREAD E: %d\n",valor);
    }
}

void * threadF() { // Recebe 20 de B
    int valor;
    while(1){
        receive_mes(&valor,5);
        printf("\nTHREAD F: %d\n",valor);
    }
}

void * threadG() { // Recebe 30 de C
    int valor;
    while(1){
        receive_mes(&valor,6);
        printf("\nTHREAD G: %d\n",valor);
    }
}

void * threadH() { // Recebe 40 de D
    int valor;
    while(1){
        receive_mes(&valor,7);
        printf("\nTHREAD H: %d\n",valor);
    }
}

void * t_mailbox(){
    int buffer[10] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}; // 10 elementos
    int i, valor, mensagemEncaminhada;
    unsigned char IDprocessDestino;
    unsigned char mensagemRecebida;
    int cont = 0;
    int can = 0;
    while(cont < 24){
        for(i=0; i<10; i++){
            if(cont == 24){
                buffer[i] = -1;
            } else {
                if(can>3){
                  can = 0;
                }
                receive_mes(&buffer[i],can); //recebe valor dos canais
                
                //printf("\nID do processo destino:%d", IDprocessDestino);
                //printf("\nMensagem recebida:%d\n", mensagemRecebida);
                cont++;
                can++;
            }
        }

        i = 0;
        for(i=0; i<10; i++){
            if(buffer[i] != -1){
                encontra_bytes(buffer[i], &IDprocessDestino, &mensagemRecebida);
                mensagemEncaminhada = (unsigned int)mensagemRecebida;
                send_mes_sync(&mensagemEncaminhada,IDprocessDestino);
                buffer[i] = -1;
            }
        }
        i = 0;
    }
}



/*função principal*/
int main() {
    pthread_t tid0,tid1,tid2,tid3,tid4,tid5,tid6,tid7,tid8; // id das threads.
    // criação das threads
    pthread_create(&tid0,NULL,threadA,NULL); // Thread A com id tid1
    pthread_create(&tid1,NULL,threadB,NULL); // Thread A com id tid1
    pthread_create(&tid2,NULL,threadC,NULL); // Thread B com id tid2
    pthread_create(&tid3,NULL,threadD,NULL); // Thread C com id tid3
    pthread_create(&tid4,NULL,threadE,NULL); // Thread D com id tid4
    pthread_create(&tid5,NULL,threadF,NULL); // Thread E com id tid5
    pthread_create(&tid6,NULL,threadG,NULL); // Thread F com id tid6
    pthread_create(&tid7,NULL,threadH,NULL); // Thread G com id tid7
    pthread_create(&tid8,NULL,t_mailbox,NULL); // Thread t_mailbox com id tid9

    // finaliza as threads caso não estejam mais executando nada

    pthread_join(tid0,NULL);    
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    pthread_join(tid3,NULL);
    pthread_join(tid4,NULL);
    pthread_join(tid5,NULL);
    pthread_join(tid6,NULL);
    pthread_join(tid7,NULL);
    pthread_join(tid8,NULL);

    return 0;
}
