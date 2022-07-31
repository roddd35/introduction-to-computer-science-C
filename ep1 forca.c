#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
    int numjogos = 0;
    int depurador = 0; /* desligado */
    int jogar = 1;
    int opc = 0;
    int numvitorias = 0;

    printf("Bem-vindo ao JOGO da FORCA!\n\n");

    while(jogar == 1){
        if(depurador == 0){
            printf("\n\nDepuração = DESLIGADO / Jogos Jogados = %d\n\n", numjogos);
        }
        else{
            printf("\n\nDepuração = LIGADO / Jogos Jogados = %d\n\n", numjogos);
        }
        
        printf("1. Continuar a jogar\n2. Alternar modo depuração e continuar a jogar\n3. Parar\nEscolha uma opção: ");
        scanf("%d", &opc);

        while(opc != 1 && opc != 2 && opc !=3){
            printf("Digite uma opção válida!\nEscolha uma opção: ");
            scanf("%d", &opc);
        }

        if(opc == 1){ /* iniciar o jogo */
            numjogos++;
            int numtentativas = 6;
            int palpite;
            int dig1 = -1;
            int dig2 = -1;
            int dig3 = -1;
            int numsort;

            /*sorteia um número*/
            srand(time(NULL));
            numsort = rand() % 1000;

            if (depurador == 1){
                printf("\nNúmero Sorteado: %d", numsort);
            }

            while (numtentativas > 0) {
                printf("\n\nRestam %d tentativas!", numtentativas);
                printf("\nSua próxima tentativa (dígito de 0 a 9): ");
                scanf("%d", &palpite);

                /*sequência de condicionais para verificar se o dígito está contido no número sorteado*/
                if (numsort % 10 == palpite && dig3 < 0){
                    dig3 = palpite;
                    printf("\nVocê acertou o dígito: %d\n", dig3);
                    numtentativas--;
                }
                else if((numsort/10) % 10 == palpite && dig2 < 0){
                    dig2 = palpite;
                    printf("\nVocê acertou o dígito: %d\n", dig2);
                    numtentativas--;
                }
                else if((numsort/100) % 10 == palpite && dig1 < 0){ 
                    dig1 = palpite;
                    printf("\nVocê acertou o dígito: %d\n", dig1);
                    numtentativas--;
                }
                else{
                    numtentativas--;
                    printf("Você não acertou nenhum dígito nessa rodada!");
                }

                if (dig1 >= 0 && dig2 >= 0 && dig3 >= 0) { /*verifica se os 3 dígitos foram acertados*/
                    printf("Fim de Jogo! Você Ganhou!");
                    printf("\nO número sorteado era: %d", numsort);
                    numvitorias++;
                    numtentativas = 0;
                }
            }
            if (dig1 < 0 || dig2 < 0 || dig3 < 0){ /*verifica se o usuário perdeu*/
                printf("\n\nFim de Jogo! Você Perdeu!");
                printf("\nO número sorteado era: %d", numsort);
            }
        }

        else if(opc == 2){ /* verificar a situação do depurador */
            if (depurador == 0){
                depurador = 1;
            }
            else {
                depurador = 0;
            }
        }

        else { /* sair do jogo */
            printf("Número de Jogos: %d\nNúmero de Vitórias: %d\n\n", numjogos, numvitorias);
            jogar = 0;
        }
    }
}