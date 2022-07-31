#include <stdio.h>
#include <stdlib.h>
#define MAX 256

int *load_image_from_file(char *filename, int *w, int *h);
void save_image_to_file(char *filename, int *i, int w, int h);
int *invert_image(int *I, int w, int h);
int *binarize(int *I, int w, int h, int t);
int *filter_image(int *I, int w, int h, int d, int tipo);
int *contorno(int *I, int w, int h, int *F);
int *label_components(int *I, int w, int h, int *ncc);
void add_border(int *I, int w, int h, int *bordered_img, int d);
void sort(int V[MAX], int n);
int apply_filter(int V[MAX], int qtd, int tipo);
void replace_with_label(int *I, int *R, int i, int j, int ncc, int w, int h);

/*arrumar a liberação de memoria e o MANTER*/

int main(){
    char opc, fileName[MAX];
    int *img = NULL, *img_transformada = NULL, *filtered_img = NULL, *backup = NULL;
    int w = 0, h = 0;
    int flag = 0, t, d = 0, tipo, ncc = 0;

    while (flag == 0)
    {
        printf("----------------------------------------------\n");
        printf("Escolha uma opção: ");
        printf("\n  Q. Quit\n  L. Ler um arquivo\n  S. Salvar imagem em arquivo\n  M. Manter entrada anterior\n  B. Binarizar a imagem\n  C. Contorno da imagem\n  I. Inverter a imagem\n  F. Filtrar a imagem\n  R. Rotular os componentes conexos");
        printf("\n==> ");
        scanf(" %c", &opc);
        printf("\n");

        /*sair*/
        if (opc == 'Q' || opc == 'q'){
            printf("\n");
            if (img != NULL)
            {
                free(img);
                img = NULL;
            }
            
            return 0;
        }
        /*ler*/
        else if (opc == 'L' || opc == 'l'){
            free(img);
            img = NULL; img_transformada = NULL; backup = NULL;
            printf("\nDigite o nome do arquivo: ");
            scanf("%s", fileName);
            img = load_image_from_file(fileName, &w, &h);
            printf("\n ** Tamanho da imagem: %d x %d\n\n", w, h);
            backup = malloc(sizeof(int)*w*h);
        }

        else if (img != NULL){
            /*salvar*/
            if (opc == 'S' || opc == 's'){
                printf("\nDigite o nome do arquivo de saída: ");
                scanf("%s", fileName);
                if (img_transformada == NULL)
                {
                    img_transformada = img;
                }
                
                save_image_to_file(fileName, img_transformada, w, h);
                printf("\n ** Imagem gravada no arquivo %s \n\n", fileName);
            }
            /*manter*/
            else if (opc == 'M' || opc == 'm'){
                if (img_transformada == NULL)
                    img = img;
                else {
                    img = backup;
                    img_transformada = NULL;
                }
            }
            /*binarizar*/
            else if(opc == 'B' || opc == 'b'){
                printf("\nDigite o Limiar: ");
                scanf("%d", &t);
                
                if (img_transformada == NULL)
                {
                    img_transformada = img;
                }

                img_transformada = binarize(img_transformada, w, h, t);
                backup = img;
                img = img_transformada;
                printf("\n ** Imagem foi binarizada\n\n");
            }
            /*contorno*/
            else if(opc == 'C' || opc == 'c'){
                d = 3; tipo = 1;
                if (img_transformada == NULL)
                {
                    img_transformada = img;
                }
                filtered_img = filter_image(img_transformada, w, h, d, tipo);

                img_transformada = contorno(img_transformada, w, h, filtered_img);
                
                backup = img;
                img = img_transformada;
                free(filtered_img);
                filtered_img = NULL;
                printf(" ** Contorno foi calculado\n\n");
            }
            /*inverter*/
            else if(opc == 'I' || opc == 'i'){
                if (img_transformada == NULL){
                    img_transformada = img;
                }

                img_transformada = invert_image(img_transformada, w, h);
                backup = img;
                img = img_transformada;
                printf("\n ** Imagem foi invertida\n\n");
            }
            /*filtrar*/
            else if(opc == 'F' || opc == 'f'){
                printf("\n==> Digite o tipo de filtro: ");
                scanf("%d", &tipo);
                printf("\n==> Digite o tamanho do filtro: ");
                scanf("%d", &d);
                if (img_transformada == NULL)
                {
                    img_transformada = img;
                }

                img_transformada = filter_image(img_transformada, w, h, d, tipo);
                backup = img;
                img = img_transformada;
                printf("\n ** Imagem foi filtrada\n\n");
            }
            /*rotular*/
            else if(opc == 'R' || opc == 'r'){
                if (img_transformada == NULL)
                {
                    printf("\nPor favor, binarize a imagem antes!\n");
                }
                else{
                    img_transformada = label_components(img_transformada, w,  h, &ncc);
                    backup = img;
                    img = img_transformada;
                    printf(" ** Imagem foi rotulada\n");
                    printf(" ** Número de componentes conexos: %d\n\n", ncc);
                }
            }
        }
        else{
            printf("\nPor favor, insira uma opção válida!\n\n");
        }
    }
    return 0;
}

/*FUNCIONA*/
int *load_image_from_file(char *filename, int *w, int *h){
    char tipo[MAX];
    int *img, i, grad;
    FILE *file;

    file = fopen(filename, "r"); /*abre o arquivo de leitura*/
    fscanf(file, "%[^\n]\n", tipo);
    fscanf(file, "%d", &*w);
    fscanf(file, "%d", &*h);
    fscanf(file, "%d", &grad);

    img = malloc((sizeof(int)) * (*w) * (*h));

    for (i = 0; i < *w * *h; i++)
    {
        fscanf(file, "%d", &img[i]);
    }

    fclose(file);
    return img;
}

/*FUNCIONA*/
void save_image_to_file(char *filename, int *i, int w, int h){
    int j;
    FILE *file;
    file = fopen(filename, "w");/*escrever no arquivo*/

    fprintf(file, "P2\n");
    fprintf(file, "%d %d", w, h);
    fprintf(file, "\n255\n");

    /*envia a imagem para o arquivo*/
    for(j = 0; j < w * h; j++){
        fprintf(file, "%d ", i[j]);
        if (j % w == 0 && j != 0)/*pular a linha após o número certo de colunas*/
        {
            fprintf(file, "\n");
        }
    }
    fclose(file);
}

/*FUNCIONA*/
int *invert_image(int *I, int w, int h){
    int i;
    int *inv_img = NULL;

    inv_img = malloc (sizeof(int) * w * h);

    for(i = 0; i < w * h; i++){
        inv_img[i] = 255 - I[i];
    }

    return inv_img; 
}

/*FUNCIONA*/
int *binarize(int *I, int w, int h, int t){
    int i, j; 
    int *bin_img;

    bin_img = malloc(sizeof(int) * w * h);

    for (i = 0; i < h; i++){
        for(j = 0; j < w; j++){
            if (I[i * w + j] >= t)
            {
                bin_img[i * w + j] = 255;    
            }
            else{
                bin_img[i * w + j] = 0;
            }
        }
    }
    return bin_img;
}

/*FUNCIONA*/
int *filter_image(int *I, int w, int h, int d, int tipo){
    int *bordered_img = NULL, *F = NULL, V[MAX];/*V é usado para fazer o ordenamento dos elementos*/
    int i, j, k, m;
    int qtd = d*d;

    F = malloc(sizeof(int) * w * h);
    bordered_img = malloc(sizeof(int) * (w + d - 1) * (h + d - 1));

    add_border(I, w, h, bordered_img, d);/*adiciona a borda na matriz*/

    free(malloc(sizeof(int) * w * h));
    for(i = 0; i < h; i++){
        for(j = 0; j < w; j++){
            for(k = 0; k < d; k++){
                for(m = 0; m < d; m++){
                    V[k * d + m] = bordered_img[(i + k) * (w + d - 1) + j + m];
                }
            }
            F[i * w + j] = apply_filter(V, qtd, tipo);/*aplicar o filtro na matriz*/
        }
    }
    free(malloc(sizeof(int) * w * h));
    /*free(bordered_img);
    bordered_img = NULL;
    obs: com esse free tava dando um erro de "Double free" no terminal*/
    return F;
}

/*FUNCIONA*/
int *contorno(int *I, int w, int h, int *F){
    int i, j;
    int *img_contornada = NULL;

    img_contornada = malloc(sizeof(int) * w * h);

    for(i = 0; i < h; i++){
        for(j = 0; j < w; j++){
            img_contornada[i * w + j] = I[i * w + j] - F[i * w + j];
        }
    }
    return img_contornada;
}

/*FUNCIONA*/
int *label_components(int *I, int w, int h, int *ncc){
    int i, j;
    int *rotulada;
    *ncc = 1;

    /*alocar espaços na memória*/
    rotulada = malloc(sizeof(int)*w*h);

    /*percorrer a imagem procurando os componentes conexos*/
    for(i = 0; i < h; i++){
        for(j = 0; j < w; j++){
            if (I[i * w + j] == 255){
                replace_with_label(I, rotulada, i, j, *ncc, w, h);
                *ncc = *ncc + 1;
            }
        }
    }

    /*tratamento para poucos componentes conexos*/
    for(i = 0; i < h; i++){
        for(j = 0; j < w; j++){
            if(*ncc < 128 && rotulada[i * w + j] != 0)
                rotulada[i * w + j] = 255 / rotulada[i * w + j];
        }
    }
    *ncc = *ncc - 1;/*ajustar o valor, visto que como sempre começa em ncc=1, sempre soma 1 a mais ao fim*/
    return rotulada;
}

/*FUNCIONA*/
void add_border(int *I, int w, int h, int *bordered_img, int d){
    int i, j;
    int bigW, bigH;

    bigW = w + d - 1;
    bigH = h + d - 1;

    /*copiar a matriz original na matriz com borda*/
    for(i = 0; i < h; i++){
        for(j = 0; j < w; j++){
            bordered_img[(i + (d/2)) * bigW + (j + (d/2))] = I[i * w + j];
        }
    }

    /*linhas superiores e inferiores*/
    for(i = 0; i < d/2; i++){
        for(j = 0; j < w; j++){
            bordered_img[i * bigW + j + (d/2)] = I[j];
            bordered_img[(i + h + (d/2)) * bigW + j + (d/2)] = I[h * w - w + j];
        }
    }

    /*colunas à esquerda e à direita*/
    for(i = 0; i < d/2; i++){
        for(j = 0; j < h; j++){
            bordered_img[(j + (d/2)) * bigW + i] = I[j*w]; 
            bordered_img[(j + (d/2)) * bigW + i + (d / 2) + w] = I[j * w + w - 1];
        }
    }

    /*cantos superiores*/
    for(i = 0; i < d/2; i++){
        for(j = 0; j < d/2; j++){
            bordered_img[i * bigW + j] = I[0];/*esquerda*/
        }
        for (j = bigW - (d/2); j < bigW; j++){
            bordered_img[i * bigW + j] = I[w-1];/*direita*/
        }
    }

    /*cantos inferiores*/
    for(i = bigH - (d/2); i < bigH + (d/2); i++){
        for(j = 0; j < d/2; j++){
            bordered_img[i * (bigW-1) + j] = I[(h-1)*w];
        }
        for(j = bigW - (d/2); j < bigW; j++){
            bordered_img[i * (bigW-1) + j] = I[(h-1) * w + (w - 1)];
        }
    }
}

void sort(int V[MAX], int n){
    int j, aux, troca = 1;
    while (troca == 1)
    {
        troca = 0;        
        for (j = 0; j <= n; j++)
        {
            if (V[j] > V[j + 1])
            {
                troca = 1;
                aux = V[j];
                V[j] = V[j + 1];
                V[j + 1] = aux;
            }
        }
    }
}

int apply_filter(int V[MAX], int qtd, int tipo){
    int max, min, med;
    sort(V, qtd);

    /*definir os valores dos filtros*/
    min = V[0];
    max = V[qtd-1];    
    med = V[qtd/2];

    /*verificar qual filtro retornar*/
    if (tipo == 1)/*mínimo*/
    {
        return min;
    }
    else if(tipo == 2){
        return med;
    }
    else{
        return max;
    }
}

void replace_with_label(int *I, int *R, int i, int j, int ncc, int w, int h){
    int k, m;
    int label;
    
    if (i-1 >= 0 && i < h-1 && j-1 >= 0 && j < w-1)
    {    
        for(k = i-1; k <= i+1; k++){
            for(m = j-1; m <= j+1; m++){
                label = k * w + m;/*valor da próxima casa da verificação*/
                if (I[label] == 255){
                    I[label] = 0;
                    R[label] = ncc;
                    replace_with_label(I, R, k, m, ncc, w, h);/*aplicar esse rótulo até encontrar algum elemento que não pertença ao componente*/
                }
            }
        }
    }
    /*rotular as bordas*/
    else if(i-1 < 0){
        for(k = i; k <= i+1; k++){
            for(m = j-1; m <= j+1; m++){
                label = k * w + m;
                if(I[label] == 255){
                    I[label] = 0;
                    R[label] = ncc;
                    replace_with_label(I, R, k, m, ncc, w, h);
                }
            }
        }
    }
    else if(j-1 < 0){
        for(k = i-1; k <= i+1; k++){
            for(m = j; m <= j+1; m++){
                label = k * w + m;
                if(I[label] == 255){
                    I[label] = 0;
                    R[label] = ncc;
                    replace_with_label(I, R, k, m, ncc, w, h);
                }
            }
        }
    }
}