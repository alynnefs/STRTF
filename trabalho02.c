#include <stdio.h>
#include <pthread.h>

int canal[6] = {-1,-1,-1,-1,-1,-1}; // 0, 1 e 2 para voto. 3, 4 e 5 para status. É global.

/*Função de envio assíncrono*/
void send_mes_async(int *buff, int c){
    canal[c] = *buff; // valor do buffer é copiado na posição c do vetor canal
    return;
}

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

void * threadA() { 
	int voto = 0;
	int status;
	send_mes_async(&voto,0); // primeiro canal pra send
	receive_mes(&status,3); // primeiro canal pra receive
	if (status == 0) { // statusFail
		printf("A versão \'A\' falhou. Finalizando\n"); // mostra e depois finaliza
	} else {
		printf("A versão \'A\' continua executando\n");
		while(1); // executa 'nada'
	}
}

void * threadB() { 
	int voto = 2 * 5;
	int status;
	send_mes_async(&voto,1); // segundo canal pra send
	receive_mes(&status,4); // segundo canal pra receive
	if (status == 0) { // statusFail
		printf("A versão \'B\' falhou. Finalizando\n"); // mostra e depois finaliza
	} else {
		printf("A versão \'B\' continua executando\n");
		while(1); // executa 'nada'
	}
}

void * threadC() { 
	int voto = 3 + 7;
	int status;
	send_mes_async(&voto,2); // terceiro canal pra send
	receive_mes(&status,5); // terceiro canal pra receive
	if (status == 0) { // statusFail
		printf("A versão \'C\' falhou. Finalizando\n"); // mostra e depois finaliza
	} else {
		printf("A versão \'C\' continua executando\n");
		while(1); // executa 'nada'
	}
}

/*Função de comparação dos votos*/
void compara(int comparacao[], int *versao){
	if((comparacao [0] == comparacao[1]) && (comparacao[1] == comparacao[2])){ // a = b = c
		printf("Nenhuma versão falhou\nVoto majoritário: %d\n",comparacao[1]);
		*versao = -1; // todos são iguais
	} else {
		if(comparacao[0] == comparacao[1]){ // somente a = b
			printf("Voto majoritário: %d\n",comparacao[1]);
			printf("Voto minoritário: %d\n",comparacao[2]);
			*versao = 2; // c é diferente
		} else {
			if(comparacao [1] == comparacao[2]){ // somente b = c
				printf("Voto majoritário: %d\n",comparacao[1]);
				printf("Voto minoritário: %d\n",comparacao[0]);
				*versao = 0; // a é diferente
			} else { // somente a = c
    			printf("Voto majoritário: %d\n",comparacao[0]);
    			printf("Voto minoritário: %d\n",comparacao[1]);
    			*versao = 1; // b é diferente
			}
		}
	}
}



void * threadDriver(){
	int statusOk = 1, statusFail = 0;
	pthread_t tid1,tid2,tid3; // id das threads.
	int comparacao[3] = {-1,-1,-1}; // vetor inicializado com -1
	int versao; // versão errada
	
	// criação das threads
	pthread_create(&tid1,NULL,threadA,NULL); // Thread A com id tid1
	pthread_create(&tid2,NULL,threadB,NULL); // Thread B com id tid2
	pthread_create(&tid3,NULL,threadC,NULL); // Thread C com id tid3

	receive_mes(&comparacao[0],0); // recebe da threadA
	receive_mes(&comparacao[1],1); // recebe da threadB
	receive_mes(&comparacao[2],2); // recebe da threadC

	compara(comparacao,&versao); // chama a função compara, passando o vetor de comparação e a versão, pois precisará ser modificada
	
	// envia mensagem de status
	if(versao == -1){ // todos são iguais
		send_mes_sync(&statusOk,3);
		send_mes_sync(&statusOk,4);
		send_mes_sync(&statusOk,5);
	} else {
		if(versao == 0){ // a é diferente
			send_mes_sync(&statusFail,3); // a falhou
			send_mes_sync(&statusOk,4);
			send_mes_sync(&statusOk,5);
		} else {
			if(versao == 1){ // b é diferente
				send_mes_sync(&statusOk,3);
				send_mes_sync(&statusFail,4); // b falhou
				send_mes_sync(&statusOk,5);
			} else { // c é diferente
				send_mes_sync(&statusOk,3);
				send_mes_sync(&statusOk,4);
				send_mes_sync(&statusFail,5); // c falhou
			}
		}
	}

	// finaliza as threads caso não estejam mais executando nada
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	pthread_join(tid3,NULL);
}



/*função principal*/
int main() {
    pthread_t tidDriver; // id de driver
    pthread_create(&tidDriver,NULL,threadDriver,NULL); // cria thread driver
    pthread_join(tidDriver, NULL); // finaliza a thread caso não esteja mais executando nada
    return 0;
}

