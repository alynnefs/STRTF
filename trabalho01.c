/*
    Exemplo de Busy Wait Loop
    Alynne Ferreira Sousa
    Sistemas de Tempo Real e Tolerantes à Falha
    
    Procedimento:
    Elabore um programa que crie 3 threads: thread A, thread B e thread C.
    Cada thread deve acessar um array do tipo char com 30 elementos.
    Esse array deve ser preenchido na seguinte ordem:
    o elemento 0 do array recebe o caractere “A”, da thread A;
    o elemento 1 do array recebe o caractere “B”, da thread B;
    o elemento 2 do array recebe o caractere “C”, da thread C;
    e assim sucessivamente até todo o array ser preenchido.
    Ao final, a thread C deverá imprimir na tela a seqüência dos caracteres armazenados.
    
*/

#include <stdio.h>
#include <pthread.h>

int a=1, b=0, c=0, i=0; //variáveis globais
char vetor[30]; // vetor de char

/*função que mostra o vetor*/
void printVetor(char *vetor) {
    int j = 0;
    while(j < 30){ // percorre o vetor
        printf("%c\n",vetor[j]); // imprime o caractere apontado pelo índice
        j++; // incrementa o índice
    }
}

/*Thread A, responsável por adicionar o caracter 'a' no vetor. */
void * threadA() {
    while(i <= 27) { // preenche até o índice 27, já que os dois últimos são 'b' e 'c'
        while(a == 0){} // se a flag 'a' for zero, ele fica no laço. Por mais que não altere nada, o laço está sendo executado.
        vetor[i] = 'a'; // quando a flag 'a' for 1, ele preenche a posição 'i' do vetor,
        a = 0; // seta a flag 'a' pra 0
        b = 1; // e a flag 'b' para 1, de forma que a Thread B começa a executar
        i++; // incrementa o índice
    }
}

/*Thread B, responsável por adicionar o caracter 'b' no vetor. */
void * threadB() {
    while(i <= 28) { // preenche até o índice 28, já que o último é 'c'
        while(b == 0){} // se a flag 'b' for zero, ele fica no laço. Por mais que não altere nada, o laço está sendo executado.
        vetor[i] = 'b'; // quando a flag 'b' for 1, ele preenche a posição 'i' do vetor,
        b = 0; // seta a flag 'b' pra 0
        c = 1; // e a flag 'c' para 1, de forma que a Thread C começa a executar
        i++; // incrementa o índice
    }
}

/*Thread C, responsável por adicionar o caracter 'c' no vetor. */
void * threadC() {
    while(i <= 29) {  // preenche até o índice 29, que é o último
        while(c == 0){} // se a flag 'c' for zero, ele fica no laço. Por mais que não altere nada, o laço está sendo executado.
        vetor[i] = 'c'; // quando a flag 'c' for 1, ele preenche a posição 'i' do vetor,
        c = 0; // seta a flag 'c' pra 0
        a = 1; // e a flag 'a' para 1, de forma que a Thread A começa a executar
        i++; // incrementa o índice
    }
    printVetor(vetor); // quando a Thread C acaba, a função de mostrar o vetor é chamada
}


/*função principal*/
int main() {
    pthread_t tid1,tid2,tid3; // id das threads.

    // criação das threads
    pthread_create(&tid1,NULL,threadA,NULL); // Thread A com id tid1
    pthread_create(&tid2,NULL,threadB,NULL); // Thread B com id tid2
    pthread_create(&tid3,NULL,threadC,NULL); // Thread C com id tid3

    // mantém as threads 'vivas' enquanto estão executando. Quando terminarem, elas são finalizadas.
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    pthread_join(tid3,NULL);

    return 0;
}
