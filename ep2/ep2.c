#define PI              3.14159
#define RAIO_AP         200
#define RAIO_ZA         2000
#define DELTA_ALARME    60
#define EPS_COS         0.000001
#define EPS             0.01

#include <stdio.h>

/*prototipos das funções*/
int iguais (double x, double y);
double cosseno(double theta, double epsilon);
int localiza(double xi, double yi, double div, double xj, double yj, double djv, double xk, double yk, double dkv, double *xv, double *yv);
double velocidade(double x0, double y0, double x1, double y1, double deltaT);
int intercepta(double x0, double y0, double x1, double y1, double *x, double *y);
double raiz(double x, double epsilon);
double distancia(double H, double theta);
double deltaS(double x0, double x1, double y0, double y1);
double tempo(double s, double v);

/*MAIN*/
int main(){
    FILE *entrada;
    int n, i, id, antena1, antena2, antena3;
    double xini, yini, x1, y1, h1, d1, theta1, x2, y2, h2, d2, theta2, x3, y3, h3, d3, theta3, t;
    double xfim, yfim, dp, v, dorigem, interceptou, x, y, dap, intert, encontrou, lixo, qtdlixo = 0;

    entrada = fopen("entrada", "r"); /*usar um arquivo chamado "entrada" para enviar os dados*/

    printf("Programa-demo para o Sistema de Monitoramento\n");

    fscanf(entrada, "%d", &n); /*numero de casos*/
    printf("\nNumero de casos a serem analisados: %d\n\n", n);

    for (i = 0; i < n; i++)
    {
        fscanf(entrada, "%d", &id);
        printf("\n\n\nIDENTIFICACAO: veiculo %d\n\n", id);

        /*PREVIO*/
        printf("Antenas na posicao previa\n");
        /*salva os dados da antena 1*/
        fscanf(entrada, "%d", &antena1);
        fscanf(entrada, "%lf", &x1);
        fscanf(entrada, "%lf", &y1);
        fscanf(entrada, "%lf", &h1);
        fscanf(entrada, "%lf", &theta1);
        d1 = distancia(h1, theta1);

        /*salva os dados da antena 2*/
        fscanf(entrada, "%d", &antena2);
        fscanf(entrada, "%lf", &x2);
        fscanf(entrada, "%lf", &y2);
        fscanf(entrada, "%lf", &h2);
        fscanf(entrada, "%lf", &theta2);
        d2 = distancia(h2, theta2);

        /*salva os dados da antena 3*/
        fscanf(entrada, "%d", &antena3);
        fscanf(entrada, "%lf", &x3);
        fscanf(entrada, "%lf", &y3);
        fscanf(entrada, "%lf", &h3);
        fscanf(entrada, "%lf", &theta3);
        d3 = distancia(h3, theta3);

        printf("\nid | posicao | H (m) | theta (graus) | distancia (m)\n");
        printf("%2d | (%8.2f, %8.2f) | %8.2f | %8.2f | %8.2f\n", antena1, x1, y1, h1, theta1, d1);
        printf("%2d | (%8.2f, %8.2f) | %8.2f | %8.2f | %8.2f\n", antena1, x2, y2, h2, theta2, d2);
        printf("%2d | (%8.2f, %8.2f) | %8.2f | %8.2f | %8.2f\n", antena1, x3, y3, h3, theta3, d3);

        encontrou = localiza(x1, y1, d1, x2, y2, d2, x3, y3, d3, &xini, &yini); /*define as coordenadas do veiculo*/
        if (!encontrou)
        {
            printf("\nNao foi possivel calcular a localizacao atual do veiculo %d\n", id);
            printf("\nNao foi possivel determinar a situacao do veiculo %d\n\n\n", id);
            
            while (qtdlixo < 16)
            {
                fscanf (entrada, "%lf", &lixo);/*salva os dados posteriores para nao interferir na proxima leitura*/
                qtdlixo++;
            }
        }
        else{
            printf("\nLocalização previa: ( %.2f, %.2f)\n", xini, yini);

            fscanf(entrada, "%lf", &t);
            printf("\nIntervalo de tempo: %.2f segundos", t);
        
            /*ATUAL*/
            printf("\n\nAntenas na posicao atual\n");
            /*salva os dados da antena 1*/
            fscanf(entrada, "%d", &antena1);
            fscanf(entrada, "%lf", &x1);
            fscanf(entrada, "%lf", &y1);
            fscanf(entrada, "%lf", &h1);
            fscanf(entrada, "%lf", &theta1);
            d1 = distancia(h1, theta1);

            /*salva os dados da antena 2*/
            fscanf(entrada, "%d", &antena2);
            fscanf(entrada, "%lf", &x2);
            fscanf(entrada, "%lf", &y2);
            fscanf(entrada, "%lf", &h2);
            fscanf(entrada, "%lf", &theta2);
            d2 = distancia(h2, theta2);

            /*salva os dados da antena 3*/
            fscanf(entrada, "%d", &antena3);
            fscanf(entrada, "%lf", &x3);
            fscanf(entrada, "%lf", &y3);
            fscanf(entrada, "%lf", &h3);
            fscanf(entrada, "%lf", &theta3);
            d3 = distancia(h3, theta3);

            printf("\nid | posicao | H (m) | theta (graus) | distancia (m)\n");
            printf("%2d | (%8.2f, %8.2f) | %8.2f | %8.2f | %8.2f\n", antena1, x1, y1, h1, theta1, d1);
            printf("%2d | (%8.2f, %8.2f) | %8.2f | %8.2f | %8.2f\n", antena1, x2, y2, h2, theta2, d2);
            printf("%2d | (%8.2f, %8.2f) | %8.2f | %8.2f | %8.2f\n", antena1, x3, y3, h3, theta3, d3);

            encontrou = localiza(x1, y1, d1, x2, y2, d2, x3, y3, d3, &xfim, &yfim); /*define as coordenadas do veiculo*/
            if (!encontrou)
            {
                printf("\nNao foi possível determinar a localizacao atual do veiculo %d\n\n", id);
                printf("Nao foi possivel determinar a situacao do veiculo %d\n\n\n", id);
            }
            else{    
                printf("\nLocalização atual: ( %.2f, %.2f)\n", xfim, yfim);

                dp = deltaS(xini, xfim, yini, yfim);
                printf("\nDistancia percorrida: %.2f m", dp);
                v = velocidade(xini, yini, xfim, yfim, t);
                printf("\nVelocidade: %.2f m/s\n", v);

                dorigem = deltaS(xfim, 0, yfim, 0);
                printf("\nDistancia da origem: %.2f m", dorigem);

                if (dorigem <= RAIO_AP)
                {
                    if (v != 0)
                    {
                        printf("\nVeículo em movimento na AREA DE PERIGO!\n");
                    }
                    else{
                        printf("\nVeículo estacionado na AREA DE PERIGO!\n");
                        printf("\n\n***********************************");
                        printf("\n ALARME, ALARME, ALARME, ALARME !!\n         Veiculo na AP !");
                        printf("\n***********************************\n\n\n");
                        
                    }
                }
                else if(dorigem > RAIO_AP && dorigem <= RAIO_ZA){
                    if (v != 0)
                    {
                        printf("\nVeículo em movimento na ZONA DE ALERTA!\n");
                    }
                    else{
                        printf("\nVeículo estacionado na ZONA DE ALERTA!\n");
                    }
                }
                else{
                    if (v != 0)
                    {
                        printf("\nVeículo em movimento fora da ZONA DE ALERTA!\n");
                    }
                    else{
                        printf("\nVeículo estacionado fora da ZONA DE ALERTA!\n ");
                    }
                }

                interceptou = intercepta(xini, yini, xfim, yfim, &x, &y);

                if (interceptou)
                {
                    dap = deltaS(xfim, x, yfim, y);
                    if (v != 0)
                    {    
                        printf("\n\nTrajetoria INTERCEPTARA AP");

                        printf("\nDistancia atual a AP: %.2f", dap);
                        
                        intert = tempo(dap, v);
                        printf("\nInterseccao ocorrera em %.2f segundos, na coordenada (%.2f, %.2f)", intert, x, y);

                        if (intert <= 60)
                        {
                            printf("\n\n***********************************");
                            printf("\n ALARME, ALARME, ALARME, ALARME !!\n Invasão Iminente !");
                            printf("\n***********************************\n\n\n");
                        }
                    }
                }
                else{
                    printf("\nTrajetoria NAO INTERCEPTARA AP\n\n");
                }
            }  
        }      
    }
    printf("\n\n");
    fclose(entrada);
    return 0;
}

/*VERIFICA SE DOIS NÚMEROS SÃO IGUAIS*/
int iguais (double x, double y){
    if (x-y < EPS && y-x < EPS)
        return 1;
    else
        return 0;
}

/*CALCULA O TEMPO LEVADO DE UM PONTO ATÉ OUTRO*/
double tempo(double s, double v){
    double t;
    t = s/v;

    return t;
}

/*CALCULA O QUANTO O CARRO ANDOU DE UM PONTO A OUTRO*/
double deltaS(double x0, double x1, double y0, double y1){
    double x, y, res;
    double S;

    x = x1-x0;
    x = x*x;
    y = y1-y0;
    y = y*y;
    res = x + y;

    S = raiz(res, EPS);
    return S;
}

/*DISTANCIA BASE DA ANTENA AO VEICULO*/
double distancia(double H, double theta){
    double d;
    double cos;
    cos = cosseno(theta, EPS_COS);
    d = H * cos;
    return d;
}

/* FUNCIONA */
double cosseno(double theta, double epsilon){
    /*adotar EPS_COS = 0.000001*/
    double cos, x;
    double num; /*numerador*/
    int den, sinal; /*denominador*/
    
    x = (theta*PI)/180;
    num = x*x;
    den = 2;
    cos = 1;
    sinal = 1;
    
    do
    {
        cos = cos - sinal*(num/den);
        sinal = -sinal;
        num = num * x * x;
        den = den * (den+1) * (den+2);
    } while (num / den >= epsilon);
    
    return cos; 
}

/*
devolve em *xv e *yv as coordenadas absolutas do veículo
FUNCIONA
*/
int localiza(double xi, double yi, double div, double xj, double yj, double djv, double xk, double yk, double dkv, double *xv, double *yv){
    double pij, pik, qij, qik;
    
    if(iguais(xj, xk) && iguais(xj, xi))
    {
        return 0; /*não foi possível calcular a localização inicial, pular pro proximo veículo*/
    }
    else{    
        if (iguais(xj, xk)) /*usa os pares ij e ik*/
        {
            pij = ((xi*xi) - (xj*xj) + (yi*yi) - (yj*yj) - (div*div) + (djv*djv))/(2*(xi - xj));
            qij = (yj - yi)/(xi - xj);

            pik = ((xi*xi) - (xk*xk) + (yi*yi) - (yk*yk) - (div*div) + (dkv*dkv))/(2*(xi - xk));
            qik = (yk - yi)/(xi - xk);

            *yv = (pij - pik)/(qik - qij);
            *xv = pij + qij * *yv;
        }
        else if(iguais(xi, xk)) /*usa os pares ji e jk*/
        {
            pij = ((xj*xj) - (xi*xi) + (yj*yj) - (yi*yi) - (djv*djv) + (div*div))/(2*(xj - xi));
            qij = (yi - yj)/(xj - xi);

            pik = ((xj*xj) - (xk*xk) + (yj*yj) - (yk*yk) - (djv*djv) + (dkv*dkv))/(2*(xj - xk));
            qik = (yk - yj)/(xj - xk);

            *yv = (pij - pik)/(qik - qij);
            *xv = pij + qij * *yv;
        }
        else if(iguais(xi, xj)) /*usa os pares ki e kj*/
        {
            pij = ((xk*xk) - (xi*xi) + (yk*yk) - (yi*yi) - (dkv*dkv) + (div*div))/(2*(xk - xi));
            qij = (yk - yi)/(xi - xk);

            pik = ((xk*xk) - (xj*xj) + (yk*yk) - (yj*yj) - (dkv*dkv) + (djv*djv))/(2*(xk - xj));
            qik = (yj - yk)/(xk - xj);

            *yv = (pij - pik)/(qik - qij);
            *xv = pij + qij * *yv;
        }
        else{ /*usa qualquer par pois a divisão não será por 0*/
            pij = ((xi*xi) - (xj*xj) + (yi*yi) - (yj*yj) - (div*div) + (djv*djv))/(2*(xi - xj));
            qij = (yj - yi)/(xi - xj);

            pik = ((xi*xi) - (xk*xk) + (yi*yi) - (yk*yk) - (div*div) + (dkv*dkv))/(2*(xi - xk));
            qik = (yk - yi)/(xi - xk);

            *yv = (pij - pik)/(qik - qij);
            *xv = pij + qij * *yv;
        }
        return 1;
    }
}

/* FUNCIONA */
double velocidade(double x0, double y0, double x1, double y1, double deltaT){
    double vel;
    double deltaS, x, y, res;

    /*calcular deltaS*/
    x = x1-x0;
    x = x*x;
    y = y1-y0;
    y = y*y;
    res = x + y;

    deltaS = raiz(res, EPS);

    vel = deltaS/deltaT;
    /*fazer print se a velocidade for nula ou nao*/
    return vel;
}

/* 
tratamento: x1 = x0 e y1 = y0 (nao intercepta, fazer no main)
prints de estacionado ou movimento (zona de alerta ou area de perigo)
verificar o tempo até interceptar (usar função tempo), fazer no main
verificar em que regiao está (no main, usa a função deltaS e x1/y1) 
FUNCIONA
*/
int intercepta(double x0, double y0, double x1, double y1, double *x, double *y){
    int intercepta = 0;
    double a, b, c, r, xpos, xneg, ypos, yneg, dpos, dneg;
    double delta;

    r = RAIO_AP;

    /*verifica se x1 = x0*/
    if (iguais(x1, x0))
    {   
        a = y0 - y1;
        b = 0;
        c = (x0*y1) - (x1*y0);
        delta = (c*a)*(c*a) - (b*b + a*a) * (-b*b * (r*r) + (c*c));
        
        if (x1 < -200 || x1 > 200)
        {
            intercepta = 0;
        }
        else{
            *x = x1;
            *y = (-c - a * *x)/b;
            
            intercepta = 1;
        }
        
    }
    /* se x1 =/= x0 */
    else
    {
        if (iguais(y0, y1))
        {
            a = 0;
        }
        else{
            a = y0-y1;
        }

        b = x1 - x0;
        c = (x0*y1) - (x1*y0);
        delta = (c*c)*(a*a) - (b*b + a*a) * (-(b*b) * (r*r) + (c*c));

        if (delta < 0)
        {
            intercepta = 0;
        }
        else{
            /*extrai as duas raízes do bhaskara*/
            xpos = ( -(c*a) + raiz(delta, EPS) ) / (b*b + a*a);
            xneg = ( -(c*a) - raiz(delta, EPS) ) / (b*b + a*a);

            ypos = (-c - a * xpos)/b;
            yneg = (-c - a * xneg)/b;

            dpos = deltaS(x1, xpos, y1, ypos);
            dneg = deltaS(x1, xneg, y1, yneg);

            if (dpos < dneg){
                *x = xpos;
                *y = ypos;
            }
                
            else {
                *x = xneg;
                *y = yneg;
            }

            if (deltaS(x0, 0, y0, 0) < deltaS(x1, 0, y0, 0))/*nessas condições, o carro se afasta de AP*/
                intercepta = 0;  
            else
                intercepta = 1;
        }
    }
    return intercepta;
}

/* FUNCIONA*/
double raiz(double x, double epsilon){
    /* ADOTAR EPS = 0.01*/
    /* NAO USAR X = 0 */
    /* ADICIONAR TRATAMENTO PRA NUMERO NEGATIVO */
    double r, ri; /*raiz*/
    
    if (x == 0)
    {
        return x;
    }

    r = x;
    ri = 0.5*(r + x/r);
    while (r - ri > epsilon)
    {
        r = ri;
        ri = 0.5*(r + x/r);
    }
    
    return ri;        
}