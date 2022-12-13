#include <jogo.h>
#include <input.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

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
    cobra->segmentos[0].x = 40;
    cobra->segmentos[0].y = 0;   
    cobra->segmentos[1].x = 30;
    cobra->segmentos[1].y = 0;    
    cobra->segmentos[2].x = 20;
    cobra->segmentos[2].y = 0;    
    cobra->segmentos[3].x = 10;
    cobra->segmentos[3].y = 0;    
    cobra->direcao = DIREITA;
    cobra->tamanho = 4;
    return cobra;
}

void surgir_fruta (Fruta *fruta){
    if(fruta->ativo==0)
    {
        srand(time(NULL));
        fruta->segmento.x = rand() % 640;
        fruta->segmento.y = rand() % 480;
        fruta->ativo = 1;
    }

}


void limite_de_tela(Cobra *cobra){
    if (cobra->segmentos[0].x<0 || cobra->segmentos[0].x>640){}
    if (cobra->segmentos[0].y<0 || cobra->segmentos[0].y>480){}
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
        cor(LARANJA_ESCURO);
        desenha_retangulo(x, y, TAM_SEGMENTO, TAM_SEGMENTO);
    }
}

void desenha_fruta(Fruta *fruta){
    cor(ROXO);
    desenha_retangulo(fruta->segmento.x, fruta->segmento.y, TAM_SEGMENTO, TAM_SEGMENTO);
}

void colisao_fruta(Fruta *fruta, Cobra *cobra){
    if(cobra->segmentos[0].x<fruta->segmento.x<cobra->segmentos[0].x+10 && 
    cobra->segmentos[0].y<fruta->segmento.y+1<cobra->segmentos[0].y+10) {
        fruta->ativo=0;
    }
}

int main() {

    // char *apelido = malloc(sizeof (char)*1000);

    // printf("Digite seu apelido: ");
    // fgets(apelido, 30, stdin);

    // FILE *arq_apelidos = fopen("arq_apelidos.txt", "W");
    // fwrite(apelido, 1, sizeof (apelido), arq_apelidos);


    abre_janela(640, 480);
    Cobra *cobra = cria_cobra();
    Fruta *fruta = (Fruta*)malloc(sizeof(Fruta));
    fruta->ativo = 0;

    while (janela_esta_aberta()) {

         if (tecla_pressionada(ESC))
            janela_deve_fechar();


        atualiza_cobra(cobra);
        desenha_cobra(cobra);
        colisao_fruta(fruta, cobra);
        surgir_fruta(fruta);
        desenha_fruta(fruta);

        


        if (tecla_pressionada(CIMA)) 
            cobra->direcao = CIMA;
        if (tecla_pressionada(BAIXO)) 
            cobra->direcao = BAIXO;
        if (tecla_pressionada(ESQUERDA)) 
            cobra->direcao = ESQUERDA;
        if (tecla_pressionada(DIREITA)) 
            cobra->direcao = DIREITA;
        int i=0;
        while(i < 100000000){ //50000000 10000000
            i++;
        }
    }
    fecha_janela();
}