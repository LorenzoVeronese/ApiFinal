/*
INIZIO: 16/07/21
PSEUDOCODICE
1---Salvo n e k.
2---Costruisco bst*1 di k elementi con valori ad inf.
3---Per ciascun AggiungiGrafo
    3.1   costruisci lista di adiacenza
    3.2   fai n-1 ricerche dei percorsi più brevi con Dijkstra, sommando di volta in volta
		3.2.1 costruisci min-heap di n elementi
		3.2.2 manipola min-heap con sottoprogrammi
    3.3   inserisci somma in bst
4---Stampa con ricerca InOrder i nodi del bst

*1 per adesso array
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAXKEY 14



//Lista di adiacenza
typedef struct Node{
	int key;
    int weight;
	struct Node *next;
} node;
typedef node *Ptr_node;


//Coda con priorità
typedef struct Queue{
	int key;
	//int prev;
	int dist;
} queue;
typedef queue *Ptr_queue;

Ptr_queue* build_ranking(int k);
void add_ranked(Ptr_queue* ranking, int graph_num, int distance, int k);

Ptr_node* build_adjacency_list(int n);
void print_adjacency_list(Ptr_node* ptr_arr, int n);

Ptr_queue* build_priority_queue(int n);
void print_priority_queue(Ptr_queue* ptr_queue, int n);
void clear_priority_queue(Ptr_queue* ptr_queue, int n);
void min_heapify(Ptr_queue* ptr_queue, int to_move, int n);
int search_in_priority_queue(Ptr_queue* ptr_queue, int to_search, int n);
void delete_element_priority_queue(Ptr_queue* ptr_queue, int to_remove, int* n);

int dijkstra(Ptr_node* ptr_arr, Ptr_queue* ptr_queue, int dest, int n);



int main(){
    int n, //numero nodi
        k; //lunghezza classifica
    Ptr_queue* ranking; //array*1 con k migliori
    char key_word[MAXKEY]; //parola di stacco
	Ptr_node* ptr_arr; //array di puntatori alla liste
	Ptr_queue* ptr_queue; //array (coda) di struct
	int somma; //somma cammini minimi
	int i, j;

    //1. LETTURA INPUT n e k
    if(scanf("%d %d\n", &n, &k)){}


    //2. COSTRUZIONE ARRAY*1
    ranking = build_ranking(k);



	//3.2 Costruisco queue (basta una volta sola) per algoritmo di Dijkstra
	ptr_queue = build_priority_queue(n);
	//D print_priority_queue(ptr_queue, n);
	j = 0;
    //3. AGGIUNGI GRAFO O TOPK
    //AggiungiGrafo
    while(1){
		somma = 0;
		//Leggo key word
        if(fgets(key_word, 14, stdin)){}

		//caso AggiungiGrafo
        if (key_word[0] == 'A' && j==19){
			//Leggo grafo
            ptr_arr = build_adjacency_list(n);
			//D print_adjacency_list(ptr_arr, n);

			printf("----------INIZIA DIJKSTRA _ %d-esimo----------\n", j);
			//Calcolo somma cammini minimi
			if(ptr_arr[0] != NULL){
				for(i = 1; i < n; i++){
					somma = somma + dijkstra(ptr_arr, ptr_queue, i, n);
					//ripulisco la coda
					clear_priority_queue(ptr_queue, n);
					//D printf("---nodo fatto\n");
				}
			}
			//printf("%d\n", somma);
			//Inserisco somma in array*1
			add_ranked(ranking, j, somma, k);

			ptr_arr = NULL;
			j++;

        }
        //caso TopK
        else{
            break;
        }
    }

	//D print_priority_queue(ranking, k);
	for(i = 0; i < k - 1; i++){
		printf("%d, ", ranking[i] -> key);
	}
	printf("%d", ranking[k - 1] -> key);

    return 0;
}



Ptr_queue* build_ranking(int k){
	Ptr_queue* ranking;
	int i;


	ranking = (Ptr_queue*)malloc(k * sizeof(Ptr_queue));

	//imposto gli altri nodi a infinito
	for(i = 0; i < k; i++){
		ranking[i] = (Ptr_queue)malloc(sizeof(Ptr_queue));
		ranking[i] -> key = (int)INFINITY;
		ranking[i] -> dist = (int)INFINITY;
	}


	return ranking;
}


void add_ranked(Ptr_queue* ranking, int graph_num, int distance, int k){
	int i;

	if(ranking[k - 1] -> key != (int)INFINITY){
		for(i = 0; i < k; i++){
			if(distance < ranking[i] -> dist){
				ranking[i] -> key = graph_num;
				ranking[i] -> dist = distance;
				break;
			}
		}
	}
	else{
		for(i = 0; ranking[i] -> key != (int)INFINITY; i++){}
		ranking[i] -> key = graph_num;
		ranking[i] -> dist = distance;
	}
}


Ptr_node* build_adjacency_list(int n){
    Ptr_node* ptr_arr; //array di puntatori alla liste
    Ptr_node curr_node; //per creazione lista
    int i, j, num; //supporto

    //Creo array che conterrà puntatori a ciascuna lista
    ptr_arr = (Ptr_node*)malloc(n * sizeof(Ptr_node));

    //Setto i puntatori a NULL
    for(i = 0; i < n; i++){
        ptr_arr[i] = NULL;

        break;
    }

	//Per prevenire [-Werror=unused-but-set-variable] inizializzo curr_node a NULL
	curr_node = NULL;
	//Creo liste per ciascun nodo: nodo i-esimo collegato al j-esimo con costo num
	for(i = 0; i < n; i++){
		for(j = 0; j < n-1; j++){
			if(scanf("%d,", &num)){}
			//Nodi con costo zero non li considero
			//Non considero nemmeno gli autoanelli
			//Non mi interessano i percorsi diretti verso zero (cicli)
			if(num != 0 && i != j && j != 0){
				//Primo nodo direttamente collegato all'i-esimo
				if(ptr_arr[i] == NULL){
					curr_node = (Ptr_node)malloc(sizeof(node));
					ptr_arr[i] = curr_node;
					curr_node -> key = j;
					curr_node -> weight = num;
					curr_node -> next = NULL;
				}
				//Non è il primo
				else{
					//vediamo poi se serve prev_node = curr_node;
					curr_node -> next = (Ptr_node)malloc(sizeof(node));
					curr_node = curr_node -> next;
					curr_node -> key = j;
					curr_node -> weight = num;
					curr_node -> next = NULL;
					//vediamo poi se serve prev_node -> next = curr_node;
				}
			}
		}
		//L'ultimo numero lo leggo a parte
		if(scanf("%d\n", &num)){}
		//Nodi con costo zero non li considero
		//Non considero nemmeno gli autoanelli
		//Non mi interessano i percorsi diretti verso zero (cicli)
		if(num != 0 && i != j && j != 0){
			//primo nodo direttamente collegato all'i-esimo
			if(ptr_arr[i] == NULL){
				curr_node = (Ptr_node)malloc(sizeof(node));
				ptr_arr[i] = curr_node;
				curr_node -> key = j;
				curr_node -> weight = num;
				curr_node -> next = NULL;
			}
			//non è il primo
			else{
				//prev_node = curr_node;
				curr_node -> next = (Ptr_node)malloc(sizeof(node));
				curr_node = curr_node -> next;
				curr_node -> key = j;
				curr_node -> weight = num;
				curr_node -> next = NULL;
				//prev_node -> next = curr_node;
			}
		}
	}

	return ptr_arr;
}



//D
void print_adjacency_list(Ptr_node* ptr_arr, int n){
	Ptr_node curr_node;
	int i;

	printf("Adjacency List:\n");
	for(i = 0; i < n; i++){
		printf("-----%d collegato a:\n", i);
		if(ptr_arr[i] != NULL){
			curr_node = ptr_arr[i];
			printf("%d con costo %d\n", curr_node -> key, curr_node -> weight);
			while(curr_node -> next != NULL){
				curr_node = curr_node -> next;
				printf("%d con costo %d\n", curr_node -> key, curr_node -> weight);
			}
		}
	}
}


Ptr_queue* build_priority_queue(int n){
	Ptr_queue* ptr_queue;
	int i;


	ptr_queue = (Ptr_queue*)malloc(n * sizeof(Ptr_queue));

	//parto sempre dal nodo zero
	ptr_queue[0] = (Ptr_queue)malloc(sizeof(Ptr_queue));
	ptr_queue[0] -> key = 0;
	ptr_queue[0] -> dist = 0;

	//imposto gli altri nodi a infinito
	for(i = 1; i < n; i++){
		ptr_queue[i] = (Ptr_queue)malloc(sizeof(Ptr_queue));
		ptr_queue[i] -> key = i;
		ptr_queue[i] -> dist = (int)INFINITY;
	}


	return ptr_queue;
}


void delete_element_priority_queue(Ptr_queue* ptr_queue, int to_remove, int* n_queue){
	//Ptr_queue temp;
	//temp = ptr_queue[to_remove];
	ptr_queue[to_remove] = ptr_queue[*n_queue - 1];
	ptr_queue[*n_queue - 1] = NULL;
	//ptr_queue[*n_queue - 1] = NULL;

	*n_queue = *n_queue - 1;

}


void print_priority_queue(Ptr_queue* ptr_queue, int n){
	int i;

	printf("Priority Queue:\n");
	for(i = 0; i < n; i++){
		if(ptr_queue[i] != NULL){
			printf("%d, %d\n", ptr_queue[i] -> key, ptr_queue[i] -> dist);
		}
		else{
			printf("deleted\n");
		}
	}
}

/*
//Vedi slide 6 "data_structures_3"
void min_heapify(Ptr_queue* ptr_queue, int to_move, int n){
	int left, right, posmin;
	Ptr_queue temp;

	left = 2 * to_move;
	right = 2 * to_move + 1;

	if(left >= n){
		return;
	}

	if(ptr_queue[left] -> dist < ptr_queue[to_move] -> dist){
		posmin = left;
	}
	else{
		posmin = to_move;
	}
	if(ptr_queue[right] -> dist < ptr_queue[posmin] -> dist){
		posmin = right;
	}

	if(posmin != to_move){
		temp = ptr_queue[to_move];
		ptr_queue[to_move] = ptr_queue[posmin];
		ptr_queue[posmin] = temp;

		min_heapify(ptr_queue, posmin, n);
	}
}
*/
void min_heapify(Ptr_queue* ptr_queue, int to_move, int n){
	Ptr_queue temp;
	int i;


	while(to_move - 1 >= 0){
		if(ptr_queue[to_move - 1] == NULL){
			break;
		}
		if(ptr_queue[to_move - 1] -> dist > ptr_queue[to_move] -> dist){
			temp = ptr_queue[to_move - 1];
			ptr_queue[to_move - 1] = ptr_queue[to_move];
			ptr_queue[to_move] = temp;

			to_move = to_move - 1;
		}
		else{
			break;
		}
	}
	i = 0;
	while(to_move + 1 < n){
		if(ptr_queue[to_move - 1] == NULL){
			break;
		}
		//D printf("%d-esimo\n", i);
		//D print_priority_queue(ptr_queue, n);
		if(ptr_queue[to_move + 1] -> dist < ptr_queue[to_move] -> dist){
			temp = ptr_queue[to_move + 1];
			ptr_queue[to_move + 1] = ptr_queue[to_move];
			ptr_queue[to_move] = temp;

			to_move = to_move + 1;
		}
		else{
			break;
		}
		i++;
	}
}




int search_in_priority_queue(Ptr_queue* ptr_queue, int to_search, int n){
	int i;


	for(i = 0; i < n; i++){
		if(ptr_queue[i] -> key == to_search){
			return i;
		}
	}

	//non lo trovo
	return (int)INFINITY;
}


int dijkstra(Ptr_node* ptr_arr, Ptr_queue* ptr_queue, int dest, int n){
	int index_reached, cost, curr;
	int n_queue;
	Ptr_node curr_node;

	n_queue = n;

	while(1){
		curr = ptr_queue[0] -> key;
		printf("ora la sorgente è %d (dest %d)\n", curr, dest);
		//in cima all'heap c'è un percorso con dest come destinazione: è già
		// il minimo.
		if(curr == dest){
			printf("%d\n", ptr_queue[0] -> dist);
			return ptr_queue[0] -> dist;
		}

		curr_node = ptr_arr[curr];
		//NON vicolo cieco
		if(curr_node != NULL){
			//Analizzo i collegati
			while(curr_node != NULL){
				printf("collegamento %d\n", curr_node -> key);
				index_reached = search_in_priority_queue(ptr_queue, curr_node -> key, n_queue);

				//se trovo un nodo
				if(index_reached != (int)INFINITY){
					//caso in cui ho distanza minore raggiungendo quel nodo per questa via
					cost = ptr_queue[0] -> dist + curr_node -> weight;
					if(ptr_queue[index_reached] -> dist > cost){
						ptr_queue[index_reached] -> dist = cost;
						min_heapify(ptr_queue, index_reached, n_queue);
						print_priority_queue(ptr_queue, n);
					}
				}
				curr_node = curr_node -> next;
			}

		}/*
		//Vicolo cieco
		else{
			delete_element_priority_queue(ptr_queue, 0, &n_queue);
			printf("E' un vicolo cieco, quindi elimino\n");
			print_priority_queue(ptr_queue, n);
		}
		*/
		if(n_queue != 1){
			printf("secondo min_heapify\n");
			//Setto distanza e nome del nodo appena studiato a infinito
			delete_element_priority_queue(ptr_queue, 0, &n_queue);
			printf("Tolgo il primo\n");
			print_priority_queue(ptr_queue, n);
			min_heapify(ptr_queue, 0, n_queue);
			print_priority_queue(ptr_queue, n);
		}
		/*
		else{
			if(curr == dest){
				//Dprintf("%d\n", ptr_queue[0] -> dist);
				return ptr_queue[0] -> dist;
			}
			else{
				break;
			}
		}
		*/

	}

	return 0;
}


void clear_priority_queue(Ptr_queue* ptr_queue, int n){
	int i;

	i = 1;
	if(ptr_queue[0] != NULL){
		ptr_queue[0] -> key = 0;
		ptr_queue[0] -> dist = 0;
	}
	else{
		i = 0;
	}

	while(ptr_queue[i] != NULL){
		ptr_queue[i] -> key = i;
		ptr_queue[i] -> dist = (int)INFINITY;

		i++;
	}
	while(i < n){
		ptr_queue[i] = (Ptr_queue)malloc(sizeof(Ptr_queue));
		ptr_queue[i] -> key = i;
		ptr_queue[i] -> dist = (int)INFINITY;

		i++;
	}

}
