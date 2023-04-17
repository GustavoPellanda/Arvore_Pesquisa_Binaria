#include <stdio.h>
#include <stdlib.h>

struct tree{
	int num;
	struct tree *left;
	struct tree *right;
};

struct tree *root; // Nó raíz, o primeiro da árvore

struct tree *stree(struct tree *root, struct tree *leaf, int num);
void print_tree(struct tree *leaf, int leafCount);
int total_nodes(struct tree *root);
int find_max(struct tree *root);
void inorder(struct tree *root);
void preorder(struct tree *root);
void postorder(struct tree *root);

int main(){

	printf("\n\nDigite uma lista para organiza-la em um arvore binaria\n");
	printf("digite 00000 quando tiver terminado de inserir os numeros e pressione enter\n\n");

	root = NULL; // Aponta a raíz para o nulo e inicializa a árvore
	int entry; // Fará a leitura dos números de entrada
	scanf("%d", &entry);
	fflush(stdin);

	// Insere números em um novo nó enquanto a entrada for diferente de 00000
	while(entry != 000000){
		if(!root) root = stree(root, root, entry); // Insere a informação no nó raíz quando ele ainda não existir
		else stree(root, root, entry); // Cria um nó
		scanf("%d", &entry); // Lê o próximo número
		fflush(stdin);
	}

	print_tree(root, 0);
	printf("\n\nNumero total de nos: %d\n", total_nodes(root));
	printf("\n\nMaior numero encontrado: %d\n", find_max(root));
  	printf("Resultado ordenado: ");
  	
  	printf("\nEm ordem: ");
  	inorder(root);
  	printf("\nPre ordem: ");
  	preorder(root);
  	printf("\nPos ordem: ");
  	postorder(root);

  	printf("\n\n");

	system("PAUSE");
	return 0;
}

// Cria um novo nó, ou uma nova folha na árvore:
struct tree *stree(struct tree *root, struct tree *leaf, int num){

	if(!leaf){

	/* if(!leaf) Inicia o processo de criação da folha quando ele ainda não tiver acontecido.
 	 * Isso acontecerá quando o "fundo" da árvore for atingido,
 	 * a partir de onde a árvora será reconstruída de cima para baixo.
 	 */
		leaf = (struct tree *) malloc(sizeof(struct tree)); // Alocação dinâmica da folha
		if(!leaf) exit(0);

		leaf->left = NULL; // Inicializa os ponteiros do nó apontanto para NULL ("fundo" da árvore)
		leaf->right = NULL;
		leaf->num = num; // Insere o número em sua respectiva variável

		if(!root) return leaf; // Quando é a primeira entrada, portanto o nó raíz não existe

		if(num < root->num) root->left = leaf; // Quando a informação é menor que o nó raiz, a folha é criada à esquerda
		else root->right = leaf; // Quando a informação é maior que o nó raiz, a folha é criada à direita
		return leaf;
	}

	// Movimenta pelas folhas já existentes o número que será inserido, de cima para baixo:
	if(num < leaf->num) stree(leaf, leaf->left, num); // Para a esquerda quando um número maior é encontrado
	else stree(leaf, leaf->right, num); // Para a direita quando um número menor é encontrado
}

// Imprime a árvore:
void print_tree(struct tree *leaf, int leafCount){ 
	printf("\n\n");
	if(leaf == NULL) return; // Finaliza as chamadas de função quando a última folha é encontrada
	print_tree(leaf->left, leafCount++); // Move uma casa para a esquerda
	for(int i = 0; i < leafCount; i++) printf("    "); // Imprime espaços entre os números
	printf("%d\n", leaf->num); //Imprime o número
	print_tree(leaf->right, leafCount++); // Movimenta uma casa para a esquerda
}

// Retorna o total de nós:
int total_nodes(struct tree *root){
	if(root == NULL) return 0;
	int alt_left = total_nodes(root->left); 
	int alt_right = total_nodes(root->right);
	return(alt_left + alt_right + 1);
}
/* Conta os nós à direita de cima a baixo, 
 * à esquerda de cima a baixo e então soma-os ao nó central
 */

// Encontra o maior número:
int find_max(struct tree *root){
    if (root == NULL) return 0;
    int res = root->num;
    int left_res = find_max(root->left);
    int right_res = find_max(root->right);
    if (left_res > res) res = left_res;
    if (right_res > res) res = right_res;
    return res;
}
/* Busca o maior número no caminho pela direita, o maior número no
 * caminho pela esquerda e então compara os dois
 */


void inorder(struct tree *root){
  if (!root) return;
  inorder(root->left);
  printf("%d ", root->num);
  inorder(root->right);
}

void preorder(struct tree *root){
  if (!root) return;
  printf("%d ", root->num);
  preorder(root->left);
  preorder(root->right);
}

void postorder(struct tree *root){
  if(!root) return;
  postorder(root->left);
  postorder(root->right);
  printf("%d ", root->num);
}