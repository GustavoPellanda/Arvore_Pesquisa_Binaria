#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Árvore B

#define FOUND true
#define NOT_FOUND false

#define T 2                     // Ordem da árvore

typedef struct node {
    bool leaf;                  // Flag indicando se é uma folha
    int n;                      // Número de chaves atualmente no nó
    int keys[2 * T - 1];        // Chaves armazenadas no nó
    struct node *child[2 * T];  // Ponteiros para os filhos
} Node;

Node *root = NULL;              // Inicializa o nó raíz

Node *createNode();
void splitChild(Node *x, int i, Node *y);
void insert(int key);
void insertNonFull(Node *x, int key);
void printTree(Node *x);
bool search(Node *x, int key);

// Função para criar um novo nó:
Node *createNode(){
    Node *newNode = (Node *)malloc(sizeof(Node));
    if(!newNode){
        perror("Erro ao alocar memória para novo nó");
        exit(EXIT_FAILURE);
    }
    newNode->n = 0;
    newNode->leaf = true;  // Inicialmente, assume-se que é uma folha
    return newNode;
}

// Função para dividir o nó filho de um nó pai:
void splitChild(Node *x, int i, Node *y){
    Node *z = createNode();             // Cria um novo nó para armazenar as chaves extras
    z->leaf = y->leaf;                  // Mantém a mesma condição de folha
    z->n = T - 1;                       // O novo nó terá T-1 chaves

    // Copia as chaves extras e os filhos para o novo nó
    for(int j = 0; j < T - 1; j++) z->keys[j] = y->keys[j + T];
    if(!y->leaf){
        for (int j = 0; j < T; j++) z->child[j] = y->child[j + T];
    }

    y->n = T - 1;                       // Atualiza o número de chaves no nó original
    
    // Desloca os ponteiros dos filhos de x para abrir espaço para o novo filho
    for(int j = x->n; j >= i + 1; j--) x->child[j + 1] = x->child[j];
    
    x->child[i + 1] = z;                // Conecta o novo filho ao nó pai

    // Move uma chave do meio de y para x
    for(int j = x->n - 1; j >= i; j--) 
        x->keys[j + 1] = x->keys[j];
    x->keys[i] = y->keys[T - 1];        // Chave do meio de y é promovida para x
    x->n++;
}

// Função para inserir uma chave na árvore:
void insert(int key){
    if(!root){                                  // Se a árvore estiver vazia
        root = createNode();
        root->keys[0] = key;
        root->n = 1;
    } 
    else {
        if(root->n == 2 * T - 1){               // Se a raiz estiver cheia
            Node *s = createNode();
            s->leaf = false;
            s->child[0] = root;
            splitChild(s, 0, root);             // Divide a raiz cheia
            int i = 0;
            if (s->keys[0] < key)
                i++;
            insertNonFull(s->child[i], key);    // Insere a chave no nó adequado
            root = s;                           // Atualiza a raiz
        } 
        else {
            insertNonFull(root, key);           // Insere a chave em uma raiz não cheia
        }
    }
}

// Função para inserir em um nó não cheio:
void insertNonFull(Node *x, int key){
    int i = x->n - 1;
    if(x->leaf){
        // Move as chaves maiores para frente para abrir espaço para a nova chave
        while(i >= 0 && key < x->keys[i]){
            x->keys[i + 1] = x->keys[i];
            i--;
        }
        x->keys[i + 1] = key;                   // Insere a nova chave
        x->n++;
    }
    else {
        while(i >= 0 && key < x->keys[i]) i--;  // Encontra o filho adequado para inserção
        
        i++;                                    // Incrementa para encontrar o filho correto

        if (x->child[i]->n == 2 * T - 1) {      // Se o filho estiver cheio
            splitChild(x, i, x->child[i]);      // Divide o filho
            if (key > x->keys[i]) i++;          // Decide qual dos dois filhos deve ser usado
        }
        
        insertNonFull(x->child[i], key);        // Insere a chave no filho adequado
    }
}

// Função para imprimir a árvore B:
void printTree(Node *x){
    int i = 0;
    for(i = 0; i < x->n; i++){
        if (!x->leaf)                // Se não for uma folha, imprime os filhos primeiro
            printTree(x->child[i]);
        printf(" %d", x->keys[i]);  // Imprime a chave
    }

    if(!x->leaf)                   // Se não for uma folha, imprime o último filho
        printTree(x->child[i]);
}

// Função para buscar uma chave:
bool search(Node *x, int key){
    int i = 0;
    while ((i < x->n) && (key > x->keys[i])) i = i + 1;  // Encontra o índice da chave ou do próximo filho
    if ((i < x->n) && (key == x->keys[i])) return FOUND; // Chave foi encontrada neste nó
    else if (x->leaf) return NOT_FOUND;                 // Não é uma folha e a chave não foi encontrada neste nó
    else return search(x->child[i], key);
}

int main(){
    insert(10);
    insert(20);
    insert(5);
    insert(6);
    insert(12);
    insert(30);
    insert(7);

    printf("Arvore B:\n");
    printTree(root);
    printf("\n");

    if (search(root, 12)) printf("Chave encontrada\n");

    return 0;
}