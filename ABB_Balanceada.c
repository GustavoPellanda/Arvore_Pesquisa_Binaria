#include <stdio.h>
#include <stdlib.h>

typedef struct node {
   char chave;
   int altura;
   struct node* esq;
   struct node* dir;
} node, Arvore;

int maior (int esq, int dir);
int altura (Arvore* a);
int atualizar_altura (Arvore *a);
int balanceamento (Arvore *a);
Arvore* rotacao_simples_esq (Arvore* a);
Arvore* rotacao_simples_dir (Arvore* a);
Arvore* rotacao_dupla_esq (Arvore* a);
Arvore* rotacao_dupla_dir (Arvore* a);
Arvore* atualizar_fb_dir (Arvore *a);
Arvore* atualizar_fb_esq (Arvore *a);
Arvore* inserir (Arvore *a, char chave);
Arvore* remover (Arvore *a, char chave);
void imprimir_in_order (Arvore* a, int nivel);

/* Devolve o maior entre dois números inteiros */
int maior (int esq, int dir) {
   return (esq > dir ? esq : dir);
}

/* Retorna a altura da árvore */
int altura (Arvore* a) {
    return (a == NULL ? -1 : a->altura);
}

/* Altera a informação sobre a atual altura da árvore */
int atualizar_altura (Arvore *a) {
    return (maior(altura(a->esq), altura(a->dir)) + 1);
}

/* Retorna um inteiro equivalente ao atual fator de blanceamento da árvore */
int balanceamento (Arvore *a) {
   return(altura(a->dir) - altura(a->esq));
}

/* Realiza rotação simples à esquerda */
Arvore* rotacao_simples_esq (Arvore* a) {
   node* nova_raiz = a->dir; // Aponta a nova raiz para o nó que está à direita da antiga raiz
   a->dir = nova_raiz->esq; // O nó à direita da antiga raiz recebe o nó que está à esquerda da nova raiz   
   nova_raiz->esq = a; // A nova árvore constiruída por a é posicionada à esquerda da nova raiz
   a->altura = atualizar_altura(a);
   nova_raiz->altura = atualizar_altura(nova_raiz);
   return nova_raiz;
}

/* Realiza rotação simples à direita */
Arvore* rotacao_simples_dir (Arvore* a) {
   node* nova_raiz = a->esq;
   a->esq = nova_raiz->dir;
   nova_raiz->dir = a;
   a->altura = atualizar_altura(a);
   nova_raiz->altura = atualizar_altura(nova_raiz);
   return nova_raiz;
}

/* Realiza rotação dupla à esquerda */
Arvore* rotacao_dupla_esq (Arvore* a) {
   a->dir = rotacao_simples_dir(a->dir); // Realiza uma rotação à direita
   return rotacao_simples_esq(a); // Seguida de uma rotação à esquerda
}

/* Realiza uma rotação simples à direita */
Arvore* rotacao_dupla_dir (Arvore* a) {
   a->esq = rotacao_simples_esq(a->esq);
   return rotacao_simples_dir(a);
}

// À esquerda - balanceamento negativo, à direita - balanceamento positivo

/* Verifica se o fator de balanceamento à esquerda está correto */
Arvore* atualizar_fb_esq (Arvore *a) {
   a->altura = atualizar_altura(a);
   if(balanceamento(a) == -2){ // Verifica o se o fator de balanceamento da raiz ultrapassou -2
      if(balanceamento(a->esq) <= 0) a = rotacao_simples_dir(a); // Verifica se o sinal de balanceamento do nó filho é o mesmo
      else a = rotacao_dupla_dir(a);
   }
   return a;
}

/* Verifica se o fator de balanceamento à direita está correto */
Arvore* atualizar_fb_dir (Arvore *a) {
   a->altura = atualizar_altura(a);
   if(balanceamento(a) == 2){ // Verifica o se o fator de balanceamento da raiz ultrapassou +2
      if(balanceamento(a->dir) >= 0) a = rotacao_simples_esq(a); // Verifica se o sinal de balanceamento do nó filho é o mesmo
      else a = rotacao_dupla_esq(a);
   }
   return a;
}

/* Insere um novo nó na árvore */
Arvore* inserir (Arvore *a, char chave) {
   // Cria um novo nó
   if(a == NULL){
      a = (node*)malloc(sizeof(node));
      a->chave = chave;
      a->altura = 0;
      a->esq = NULL;
      a->dir = NULL;
   }
   // Busca o local adequado para alocar o novo nó:
   else if(chave < a->chave){
      a->esq = inserir(a->esq, chave);
      a = atualizar_fb_esq(a);
   } 
   else if(chave > a->chave){
      a->dir = inserir(a->dir, chave);
      a = atualizar_fb_dir(a);
   }
   return a;
} 

/* Função para remover um nó de uma árvore binária de busca balanceada */
Arvore* remover (Arvore *a, char chave) {
   if (a == NULL) {
      return NULL;
   }
   else {
      if (chave < a->chave) {
         a->esq = remover (a->esq, chave);
         a = atualizar_fb_dir (a);
      }
      else if (chave > a->chave) {
         a->dir = remover (a->dir, chave);
         a = atualizar_fb_esq (a);
      }
      else {
         if ((a->esq == NULL) && (a->dir == NULL)) {
            free (a);
            a = NULL;
         }
         else if (a->esq == NULL) {
            node *tmp = a;
            a = a->dir;
            free (tmp);
         }
         else if (a->dir == NULL) {
            node *tmp = a;
            a = a->esq;
            free (tmp);
         }
         else {
            node *tmp = a->esq;
            while (tmp->dir != NULL) {
               tmp = tmp->dir;
            }
            a->chave = tmp->chave; 
            tmp->chave = chave;
            a->esq = remover (a->esq, chave); 
            a = atualizar_fb_dir (a);
         }
      }
      return a; 
   }   
}

/*Função para imprimir os nós de uma árvore binária de acordo com um percurso in-ordem!*/
void imprimir_in_order (Arvore* a, int nivel) {
   if (a != NULL) {
      int i;
      for (i = 0; i < nivel; i++) {
         printf("      ");
      }
      printf("Chave: %c (altura: %d, fb: %+d) no nivel: %d\n", a->chave, a->altura, balanceamento(a), nivel);
      imprimir_in_order (a->esq, nivel + 1);
      imprimir_in_order (a->dir, nivel + 1);
   }
}

int main () {

   Arvore *AVL = NULL;

   /* Q, Z, B, Y, T, M, E, W, X, S, F, G, A, H, N, O, P, R */
   AVL = inserir (AVL, 'Q');
   AVL = inserir (AVL, 'Z');
   AVL = inserir (AVL, 'B');
   AVL = inserir (AVL, 'Y');
   AVL = inserir (AVL, 'T');
   AVL = inserir (AVL, 'M');
   AVL = inserir (AVL, 'E');
   AVL = inserir (AVL, 'W');
   AVL = inserir (AVL, 'X');
   AVL = inserir (AVL, 'S');
   AVL = inserir (AVL, 'F');
   AVL = inserir (AVL, 'G');
   AVL = inserir (AVL, 'A');
   AVL = inserir (AVL, 'H');
   AVL = inserir (AVL, 'N');
   AVL = inserir (AVL, 'O');
   AVL = inserir (AVL, 'P');
   AVL = inserir (AVL, 'R');

   //AVL = remover (AVL, 'A');
   //AVL = remover (AVL, 'H');
   //AVL = remover (AVL, 'E');
   //AVL = remover (AVL, 'W');
   //AVL = remover (AVL, 'G');
   //AVL = remover (AVL, 'N');
   //AVL = remover (AVL, 'P');
   //AVL = remover (AVL, 'O');

   imprimir_in_order (AVL, 0);

   return 0;
}