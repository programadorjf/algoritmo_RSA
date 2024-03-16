/*
Autore: João Fabricio
Disiplina de Criptografia e segurança de Dados
		
		TRABALHO 4 - IMPLEMENTAÇÃO DO ALGORITMO RSA
4 – Implemente o algoritmo RSA, esquematizado na imagem abaixo:
a) Este algoritmo necessita da determinacao de numeros primos grandes, p e q, que devera
ser feito utilizando o algoritmo de Miller-Rabin anteriormente desenvolvido. Como o
algoritmo e probabilistico, ele devera ser executado no minimo seis vezes para cada
numero a ser testado.
b) As operacoes de exponenciacao deverao ser executadas com o programa desenvolvido
no item 1
c) A determinacao de um numero d e e que sao inversos multiplicativos modulo phi(n)
podera ser feita atraves do algoritmo estendido de euclides.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include <stdbool.h>
long expo_rapida(long long a, long long e, long long mod){//Exponenciação modular rápida
	long long A=a, P=1, E=e;	
	while(1){		
		if(E == 0){//Quando acabar o while, retorna P
			return P;
		}		
		else if(E%2 != 0){//Expoente ímpar
			P=(A * P)%mod;//Redução do módulo n para cada multpilicação
			E=(E-1)/2;
		}
		else{//Expoente par
			E = E/2;
		}
		A=(A*A)%mod;//Calcula a sequência de potências
	}
}
bool Teste_Miller(long long valor){//Função para teste de números primos Miller-Rabin
	bool primeira_rodada=false;
	long long q, val, a, exp;
	int k=0;
	q = valor - 1;
	while ((q%2)==0){
		q = q/2;
		k = k + 1;
	}
	a=(rand() % (valor-4)) + 2;//Gera números aleatórios entre os valores da primeira e da penúltima posição
	while(a==valor){
		a=(rand() % (valor-4)) + 2;//Gera números aleatórios entre os valores da primeira e da penúltima posição
	}
	if(primeira_rodada==false){
		val = expo_rapida(a,q,valor);
		exp = q;
		primeira_rodada=true;
	}
	if(primeira_rodada==true){
		 val = expo_rapida(a,exp,valor);
	}
	if (val==1 || val==(valor-1)){
		 return true;
	}
	for (int j=0; j<(k-1); j++){//Laço de repetição
		val = (val*val)%valor;
		exp *= 2;		
		if (val==(valor-1)){
			return true;
		}
	}
	return false;
}
bool verificaPrimo(long long valor, int rodadas){//Função para verificar número Primo
	long long number;
	number = valor;
	if (valor<=3){
		 return true;
	}
	if((valor%2)==0){
	  	 return false;
	}
    for (int i=0; i<rodadas; i++)//Laço de repetição
         if (!Teste_Miller(number))
              return false;
    return true;
}
long long escolheE(long long phi, long long p, long long p2){//Função para escolher o menor num primo que divide o coef de euler, que seja diferente de p e p2
	long long i, e;
	for(i=(phi-1); i>=2; i--){//Laço de repetição
		if(phi%i != 0 && verificaPrimo(i,10) && i != p && i != p2){
			e = i;
			break;
		}
	}
	return e;
}
int MDC(long long m, long b){//Função MDC, algoritmo recursivo de Euclides
    if(b == 0){
		return m;
	}
    else{
		return MDC(b, m % b);
	}
} 
long long modInverso(long long a, long long m){//Função modInverso, algoritmo inverso de Euclides
    long m0 = m;
    long long y = 0, x = 1; 
    if (MDC(a,m)!=1 || m==1){
         return 0;
    } 
    while (a > 1) {
        long long quociente = a / m;
        long long t = m; 
    	m = a % m, //Atualiza m
        a = t;
        t = y;        
        y = x - quociente * y;//Atualiza y e x
        x = t;
    }
    if(x<0){
    	x+=m0;	
	}
    return x;
}
int *codifica(char *mensagem, long long e, long long n){//Encripta os caracteres usando o resto da divisão de a^e por n para cada caractere, a tabela ASCII
	long long i;
	int *mensagemC;
	mensagemC = (int*)malloc(100 * sizeof(long long));
	for(i = 0; i < 100; i++){
		mensagemC[i] = expo_rapida(mensagem[i], e, n);
	}
	return mensagemC;
}
char *decodifica(int *mensagemC, long long d, long long n){//Encripta os inteiros em uma string de caracteres usando o resto da divisão de a^d por n para cada inteiro
	long long i;
	char *mensagem;
	mensagem = (char*)malloc(100 * sizeof(char));	
	for(i = 0; i < 100; i++){
		mensagem[i] = expo_rapida(mensagemC[i], d, n);
	}
	return mensagem;
}
int main(void){//Função principal
	long long i;
	long long p, q, n, phi_n, e, d;
	char mensagem[100];//O texto para enciptação é limitado em 100 caracteres
    int *mensagemC;
    char *mensagemD;
    printf("CIRPTOGRAFIA E SEGURANCA DE DADOS\nALGORITMO RSA - JOAO FABRICIO\n\n");
	printf("\nDIGITE O PRIMEIRO NUMERO PRIMO: ");
	scanf("%llu", &p);
	if(verificaPrimo(p,10)==false){//Testa se é primo
		printf("O NUMERO DIGITADO NAO EH PRIMO\n");
		return 0;
	}
	printf("\nDIGITE O SEGUNDO NUMERO PRIMO: ");
	scanf("%llu", &q);	
	if(verificaPrimo(q,10)==false || p == q){//Testa se é primo
		printf("\nO NUMERO DIGITADO NAO EH PRIMO\n");
		return 0;
	}
	printf("\nESCREVA A MENSAGEM (LIMITADA A 100 CARACTERES): \n\n");
	scanf("\n");
	fgets(mensagem, 100, stdin);
	n = p * q;
	phi_n = (p - 1)*(q - 1);//Define o quociente de euler
	e = escolheE(phi_n, p, q);//Define o expoente para a chave pública
	d = modInverso(e, phi_n);//Define o d, para a chave privada
	printf("\nCHAVE PUBLICA: (%llu, %llu)\n", e, n);
	printf("\nCHAVE PRIVADA: (%llu, %llu)\n", d, n);
	mensagemC = codifica(mensagem, e, n);//Encripta a mensagem
	printf("\nMENSAGEM CRIPTOGRAFADA: \n\n");//Imprime mensagem criptografada
	for(i = 0; i < strlen(mensagem); i++){
		printf("%c", mensagemC[i]);
	}
	printf("\n");
	mensagemD = decodifica(mensagemC, d, n);//Decripta mensagem
	printf("\n\nMENSAGEM DESCRIPTOGRAFADA: \n\n%s", mensagemD);//Imprime mensagem descriptografada
	free(mensagemC);//Liberando a memória de programa alocada
	free(mensagemD);
	printf("\n\n\nPROGRAMA EXECUTADO COM SUCESSO!\n\n\n");
	system("pause");
	return 0;
}
