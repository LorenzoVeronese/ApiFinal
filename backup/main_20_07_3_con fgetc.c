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
#include <string.h>
#define MAXKEY 16



//Lista di adiacenza
typedef struct Node{
	uint key;
    uint weight;
	struct Node *next;
} node;
typedef node *Ptr_node;


//Coda con priorità
typedef struct Queue{
	uint key;
	//int prev;
	uint dist;
} queue;
typedef queue *Ptr_queue;

Ptr_queue* build_ranking(int k);
void add_ranked(Ptr_queue* ranking, int graph_num, int distance, int k);

Ptr_node* build_adjacency_arr(int n);
void build_adjacency_list(Ptr_node* ptr_arr, int n);
void print_adjacency_list(Ptr_node* ptr_arr, int n);
void clear_adjacency_list(Ptr_node* ptr_arr, int n);


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
	int i, j, flag;

	flag = 0;

    //1. LETTURA INPUT n e k
    if(scanf("%d %d\n", &n, &k)){}


    //2. COSTRUZIONE ARRAY*1
    ranking = build_ranking(k);


	ptr_arr = build_adjacency_arr(n);
	//3.2 Costruisco queue (basta una volta sola) per algoritmo di Dijkstra
	ptr_queue = build_priority_queue(n);
	//D print_priority_queue(ptr_queue, n);
	j = 0;
    //3. AGGIUNGI GRAFO O TOPK
    //AggiungiGrafo
    while(1){
		somma = 0;
		//Leggo key word
        if(fgets(key_word, MAXKEY, stdin) == NULL){
			break;
		}
		else if(flag == 1){
			printf("\n");
			flag = 0;
		}


		//caso AggiungiGrafo
        if (key_word[0] == 'A'){
			//Leggo grafo
            build_adjacency_list(ptr_arr, n);
			//D print_adjacency_list(ptr_arr, n);
			//D printf("----------INIZIA DIJKSTRA _ %d-esimo----------\n", j);
			//Calcolo somma cammini minimi
			if(ptr_arr[0] != NULL){
				for(i = 1; i < n; i++){
					somma = somma + dijkstra(ptr_arr, ptr_queue, i, n);
					//ripulisco la coda
					clear_priority_queue(ptr_queue, n);
					//D printf("---nodo fatto\n");
				}
			}
			//D printf("%d\n", somma);
			//Inserisco somma in array*1
			add_ranked(ranking, j, somma, k);

			clear_adjacency_list(ptr_arr, n);
        }
        //caso TopK
        else{
			//D print_priority_queue(ranking, k);
			if(ranking[0] -> key != (uint)INFINITY){
				printf("%d", ranking[0] -> key);
			}
			for(i = 1; i < k; i++){
				if(ranking[i] -> key != (uint)INFINITY){
					printf(" %d", ranking[i] -> key);
				}
			}
			flag = 1;
			/*
			if(!feof(stdin)){
				printf("\n");
			}
			if(feof(stdin)){
				break;
			}*/
        }

		j++;
    }

    return 0;
}



Ptr_queue* build_ranking(int k){
	Ptr_queue* ranking;
	int i;


	ranking = (Ptr_queue*)malloc(k * sizeof(Ptr_queue));

	//imposto gli altri nodi a infinito
	for(i = 0; i < k; i++){
		ranking[i] = (Ptr_queue)malloc(sizeof(Ptr_queue));
		ranking[i] -> key = (uint)INFINITY;
		ranking[i] -> dist = (uint)INFINITY;
	}


	return ranking;
}


void add_ranked(Ptr_queue* ranking, int graph_num, int distance, int k){
	int i, max, max_index;

	if(ranking[k - 1] -> key != (uint)INFINITY){
		//Cerco il max
		max = ranking[0] -> dist;
		max_index = 0;
		for(i = 1; i < k; i++){
			if(max < ranking[i] -> dist){
				max = ranking[i] -> dist;
				max_index = i;
			}
		}
		if(distance < ranking[max_index] -> dist){
			ranking[max_index] -> key = graph_num;
			ranking[max_index] -> dist = distance;
		}

	}
	else{
		for(i = 0; ranking[i] -> key != (uint)INFINITY; i++){}
		ranking[i] -> key = graph_num;
		ranking[i] -> dist = distance;
	}
}


Ptr_node* build_adjacency_arr(int n){
	int i;
	Ptr_node* ptr_arr; //array di puntatori alla liste

	ptr_arr = (Ptr_node*)malloc(n * sizeof(Ptr_node));

	//Setto i puntatori a NULL
    for(i = 0; i < n; i++){
        ptr_arr[i] = NULL;

        break;
    }

	return ptr_arr;
}


void build_adjacency_list(Ptr_node* ptr_arr, int n){
    Ptr_node curr_node; //per creazione lista
    uint i, j, h, num; //supporto
	char c;
	char number[10];
	uint len;

	//Per prevenire [-Werror=unused-but-set-variable] inizializzo curr_node a NULL
	curr_node = NULL;
	//Creo liste per ciascun nodo: nodo i-esimo collegato al j-esimo con costo num
	for(i = 0; i < n; i++){
		//D printf("riga matrice: %s\n", line);
		for(j = 0; j < n-1; j++){
			h = 0;
			c = fgetc(stdin);
			while(c != ','){
				//D printf("%c\n", c);
				number[h] = c;
				c = fgetc(stdin);
				h++;
			}

			//if(scanf("%d,", &num)){}
			num = atoi(number);
			//D printf("%s\n", number);
			//D printf("%d\n", num);
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
			len = sizeof(number);
			for(h = 0; h < len; h++){
				number[h] = 0;
			}
		}

		//L'ultimo numero lo leggo a parte
		//if(scanf("%d\n", &num)){}
		h = 0;
		c = fgetc(stdin);
		while(c != '\n'){
			//D printf("%c\n", c);
			number[h] = c;
			c = fgetc(stdin);
			h++;
		}
		num = atoi(number);
		// D printf("%d\n", num);
		//Nodi con costo zero non li considero
		//Non considero nemmeno gli autoanelli
		//Non mi interessano i percorsi diretti verso zero (cicli)
		if(num != 0 && i != j && j != 0){
			//if(scanf("%d,", &num)){}
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
		len = sizeof(number);
		for(h = 0; h < len; h++){
			number[h] = 0;
		}
	}
}


void clear_adjacency_list(Ptr_node* ptr_arr, int n){
	int i;
	Ptr_node prev_node, curr_node;

	for(i = 0; i < n; i++){
		if(ptr_arr[i] != NULL){
			curr_node = ptr_arr[i];
			while(curr_node -> next != NULL){
				prev_node = curr_node;
				curr_node = curr_node -> next;
				free(prev_node);
			}
			free(curr_node);
		}
		//free(ptr_arr[i]);
		ptr_arr[i] = NULL;
	}
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
		ptr_queue[i] -> dist = (uint)INFINITY;
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
	return (uint)INFINITY;
}


int dijkstra(Ptr_node* ptr_arr, Ptr_queue* ptr_queue, int dest, int n){
	int index_reached, cost, curr;
	int n_queue;
	Ptr_node curr_node;

	n_queue = n;

	while(1){
		curr = ptr_queue[0] -> key;
		//D printf("ora la sorgente è %d (dest %d)\n", curr, dest);
		//in cima all'heap c'è un percorso con dest come destinazione: è già
		// il minimo.
		if(curr == dest){
			if(ptr_queue[0] -> dist == (uint)INFINITY){
				return 0;
			}
			//D printf("%d\n", ptr_queue[0] -> dist);
			return ptr_queue[0] -> dist;
		}

		curr_node = ptr_arr[curr];
		//NON vicolo cieco
		if(curr_node != NULL){
			//Analizzo i collegati
			while(curr_node != NULL){
				//D printf("collegamento %d\n", curr_node -> key);
				index_reached = search_in_priority_queue(ptr_queue, curr_node -> key, n_queue);

				//se trovo un nodo
				if(index_reached != (uint)INFINITY){
					//caso in cui ho distanza minore raggiungendo quel nodo per questa via
					cost = ptr_queue[0] -> dist + curr_node -> weight;
					if(ptr_queue[index_reached] -> dist > cost){
						ptr_queue[index_reached] -> dist = cost;
						min_heapify(ptr_queue, index_reached, n_queue);
						//D print_priority_queue(ptr_queue, n);
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
			//Setto distanza e nome del nodo appena studiato a infinito
			delete_element_priority_queue(ptr_queue, 0, &n_queue);
			//D printf("Tolgo il primo\n");
			//D print_priority_queue(ptr_queue, n);
			min_heapify(ptr_queue, 0, n_queue);
			//D print_priority_queue(ptr_queue, n);
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
		ptr_queue[i] -> dist = (uint)INFINITY;

		i++;
	}
	while(i < n){
		ptr_queue[i] = (Ptr_queue)malloc(sizeof(Ptr_queue));
		ptr_queue[i] -> key = i;
		ptr_queue[i] -> dist = (uint)INFINITY;

		i++;
	}

}
