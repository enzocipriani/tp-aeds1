#include <jogo.h>
#include <input.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

#define MAX_SEGMENTOS 1000
#define TAM_SEGMENTO 10


typedef struct {
    int x;
    int y;
} Segmento;

typedef struct {
    Segmento segmentos[MAX_SEGMENTOS];
    int direcao;
    int tamanho;
} Cobra;

typedef struct {
    Segmento segmento;
    int ativo;
} Fruta;

Cobra *cria_cobra(){
    Cobra *cobra = (Cobra*)malloc(sizeof(Cobra));
    cobra->segmentos[0].x = 320;
    cobra->segmentos[0].y = 240;   
    cobra->segmentos[1].x = 310;
    cobra->segmentos[1].y = 240;    
    cobra->segmentos[2].x = 300;
    cobra->segmentos[2].y = 240;    
    cobra->segmentos[3].x = 290;
    cobra->segmentos[3].y = 240;    
    cobra->direcao = DIREITA;
    cobra->tamanho = 1;
    return cobra;
}

typedef struct {
    char jogador[30];
} Jogador;


 void apelido (){
    char *apelido[30];
    printf("Como voce gostaria de ser chamado? \n");
    scanf("%s", *apelido);
}


void recordes(void){
	system("cls");
	char texto_str[20];
	FILE *arquivo = fopen("recordes.txt", "r");

	if(arquivo!=NULL){
		printf("Listando:\n");
		while(fgets(texto_str, 20, arquivo) != NULL){
			printf("%s", texto_str);
		}

		fclose(arquivo);
	}else{
		printf("Nenhum recorde registrado!\n");
	}

	
	system("cls");
}

void surgir_fruta (Fruta *fruta){
    if(fruta->ativo==0)
    {
        srand(time(NULL));
        fruta->segmento.x = (rand() % 63)*10;
        fruta->segmento.y = (rand() % 47)*10;
        fruta->ativo = 1;
    }

}


int limite_de_tela(Cobra *cobra){
    if (cobra->segmentos[0].x<0 || cobra->segmentos[0].x>640 || cobra->segmentos[0].y<0 || cobra->segmentos[0].y>480){
        
        toca_som("assets/gameover.wav");
        sleep(3);    
        janela_deve_fechar();
        printf("\n\nVoce morreu!\n");
        printf("Seu SCORE foi: %d\n", cobra->tamanho-1); 
        
    }
    
}

void atualiza_cobra(Cobra *cobra){
    Segmento nova_cabeca;
    int tamanho_cobra = cobra->tamanho;
    nova_cabeca.x = cobra->segmentos[0].x;
    nova_cabeca.y = cobra->segmentos[0].y;

    if(cobra->direcao == CIMA){
        nova_cabeca.y -= TAM_SEGMENTO;
    }
    if(cobra->direcao == BAIXO){
        nova_cabeca.y += TAM_SEGMENTO;
    }
    if(cobra->direcao == DIREITA){
        nova_cabeca.x += TAM_SEGMENTO;
    }
    if(cobra->direcao == ESQUERDA){
        nova_cabeca.x -= TAM_SEGMENTO;
    }

    for(int i=tamanho_cobra-1; i>=0; i--){
        cobra->segmentos[i+1] = cobra->segmentos[i];
    }
    cobra->segmentos[0] = nova_cabeca;
}

void desenha_cobra(Cobra *cobra){
    for(int i=0; i < cobra->tamanho; i++){
        int x = cobra->segmentos[i].x;
        int y = cobra->segmentos[i].y;
        cor(VERDE);
        desenha_retangulo(x, y, TAM_SEGMENTO, TAM_SEGMENTO);
    }
}

void desenha_fruta(Fruta *fruta){
    cor(ROSA_ESCURO);
    desenha_retangulo(fruta->segmento.x, fruta->segmento.y, TAM_SEGMENTO, TAM_SEGMENTO);
}

void colisao_fruta(Fruta *fruta, Cobra *cobra){
   
    if( (cobra->segmentos[0].x < fruta->segmento.x+1 && fruta->segmento.x+1 < cobra->segmentos[0].x+10) && 
        (cobra->segmentos[0].y < fruta->segmento.y+1 && fruta->segmento.y+1 < cobra->segmentos[0].y+10)) {
        fruta->ativo=0;
        cobra->segmentos[cobra->tamanho].x = cobra->segmentos[cobra->tamanho-1].x;
        cobra->segmentos[cobra->tamanho].y = cobra->segmentos[cobra->tamanho-1].y;

        cobra->tamanho+=1;

        toca_som("assets/comer.wav");
    }
    
}

void colisao_cobra (Cobra *cobra){
    for(int i = 1; i < cobra->tamanho; i++){
        if (cobra->segmentos[0].x == cobra->segmentos[i].x && cobra->segmentos[0].y == cobra->segmentos[i].y){
            toca_som("assets/gameover.wav");
            sleep(2);    
            janela_deve_fechar();
            printf("\n\nVoce morreu!\n");
            printf("Seu SCORE foi: %d\n", cobra->tamanho-1); 
        }
    }
}


void lenta (){
    abre_janela(640, 480);
    toca_som_em_loop("assets/fundomusica.wav");
    Cobra *cobra = cria_cobra();
    Fruta *fruta = (Fruta*)malloc(sizeof(Fruta));
    fruta->ativo = 0;

    while (janela_esta_aberta()) {

         if (tecla_pressionada(ESC))
            janela_deve_fechar();

        

        fonte("assets/ubuntu.ttf", 20);
        desenha_texto(20, 20, "Score: %d", cobra->tamanho-1);

        atualiza_cobra(cobra);
        limite_de_tela(cobra);
        desenha_cobra(cobra);
        colisao_cobra(cobra);
        colisao_fruta(fruta, cobra);
        surgir_fruta(fruta);
        desenha_fruta(fruta);
        


        if (tecla_pressionada(CIMA) && cobra->direcao != BAIXO) 
            cobra->direcao = CIMA;
        if (tecla_pressionada(BAIXO) && cobra->direcao != CIMA) 
            cobra->direcao = BAIXO;
        if (tecla_pressionada(ESQUERDA) && cobra->direcao != DIREITA) 
            cobra->direcao = ESQUERDA;
        if (tecla_pressionada(DIREITA) && cobra->direcao != ESQUERDA) 
            cobra->direcao = DIREITA;
        int i=0;
        while(i < 75000000){ //50000000 10000000
            i++;
        }
    }
    fecha_janela();

}

void normal (){
    abre_janela(640, 480);
    toca_som_em_loop("assets/fundomusica.wav");
    Cobra *cobra = cria_cobra();
    Fruta *fruta = (Fruta*)malloc(sizeof(Fruta));
    fruta->ativo = 0;

    while (janela_esta_aberta()) {

         if (tecla_pressionada(ESC))
            janela_deve_fechar();

       

        fonte("assets/ubuntu.ttf", 20);
        desenha_texto(20, 20, "Score: %d", cobra->tamanho-1);

        atualiza_cobra(cobra);
        limite_de_tela(cobra);
        desenha_cobra(cobra);
        colisao_cobra(cobra);
        colisao_fruta(fruta, cobra);
        surgir_fruta(fruta);
        desenha_fruta(fruta);
        


        if (tecla_pressionada(CIMA) && cobra->direcao != BAIXO) 
            cobra->direcao = CIMA;
        if (tecla_pressionada(BAIXO) && cobra->direcao != CIMA) 
            cobra->direcao = BAIXO;
        if (tecla_pressionada(ESQUERDA) && cobra->direcao != DIREITA) 
            cobra->direcao = ESQUERDA;
        if (tecla_pressionada(DIREITA) && cobra->direcao != ESQUERDA) 
            cobra->direcao = DIREITA;
        int i=0;
        while(i < 55000000){
            i++;
        }
    }
    fecha_janela();
}

void rapida (){
    abre_janela(640, 480);
    toca_som_em_loop("assets/fundomusica.wav");
    Cobra *cobra = cria_cobra();
    Fruta *fruta = (Fruta*)malloc(sizeof(Fruta));
    fruta->ativo = 0;

    while (janela_esta_aberta()) {

         if (tecla_pressionada(ESC))
            janela_deve_fechar();

       

        fonte("assets/ubuntu.ttf", 20);
        desenha_texto(20, 20, "Score: %d", cobra->tamanho-1);

        atualiza_cobra(cobra);
        limite_de_tela(cobra);
        desenha_cobra(cobra);
        colisao_cobra(cobra);
        colisao_fruta(fruta, cobra);
        surgir_fruta(fruta);
        desenha_fruta(fruta);
        


        if (tecla_pressionada(CIMA) && cobra->direcao != BAIXO) 
            cobra->direcao = CIMA;
        if (tecla_pressionada(BAIXO) && cobra->direcao != CIMA) 
            cobra->direcao = BAIXO;
        if (tecla_pressionada(ESQUERDA) && cobra->direcao != DIREITA) 
            cobra->direcao = ESQUERDA;
        if (tecla_pressionada(DIREITA) && cobra->direcao != ESQUERDA) 
            cobra->direcao = DIREITA;
        int i=0;
        while(i < 30000000){
            i++;
        }
    }
    fecha_janela();
}



int main() {

    //passar pra função 
    system("clear");

    while(1) {
        int menu;
    printf("\n\nEscolha uma opção para começar: \n");
    printf("1 - Inicar jogo.\n");
    printf("2 - Ver recordes.\n");
    printf("3 - Sair do jogo.\n");
    scanf("%d", &menu);

    switch (menu) {   
        case 1:
            apelido();
            int velocidade;
            printf("Qual a velocidade do jogo? \n");
            printf("1 - Lento. \n2 - Normal\n3 - Rapido\n");
            scanf("%d", &velocidade);

            switch (velocidade){
            case 1:
                lenta();
                break;
            case 2:
                normal();
                break;
            case 3:
                rapida();
                break;

            default:
                printf("Opcao invalida!.");
                break;
            }
            break;

        case 2:
            recordes();
            break;
            
        case 3:
            exit(0);
            break;

        default:
            printf("Digite uma opcao valida!");
            break;
        }
    }
    
}