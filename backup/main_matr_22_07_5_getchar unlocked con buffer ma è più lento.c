/*
INIZIO: 16/07/21
PSEUDOCODICE
1---Salvo n e k.
2---Costruisco bst*1 di k elementi con valori ad inf.
3---Per ciascun AggiungiGrafo
    3.1   costruisci matrice di adiacenza
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


//Coda con priorità
typedef struct Queue{
	uint key;
	//int prev;
	uint dist;
} queue;
typedef queue *Ptr_queue;

int power(int base, int exp);
uint atoi_personal(char* str, int h);

Ptr_queue* build_ranking(int k);
void add_ranked(Ptr_queue* ranking, int graph_num, int distance, int k);
void print_ranking(Ptr_queue* ptr_queue, int n);

uint** create_adjacency_matr(int n);
void build_adjacency_matr(uint** ptr_matr, int n);
void print_adjacency_matr(uint** ptr_matr, int n);


Ptr_queue* build_priority_queue(int n);
void print_priority_queue(Ptr_queue* ptr_queue, int n);
void clear_priority_queue(Ptr_queue* ptr_queue, int n);
void min_heapify(Ptr_queue* ptr_queue, int to_move, int n);
void min_heapify_modified(Ptr_queue* ptr_queue, int to_move, int n);
uint search_in_priority_queue(Ptr_queue* ptr_queue, int to_search, int n);
void delete_element_priority_queue(Ptr_queue* ptr_queue, int to_remove, int* n);

void dijkstra(uint** ptr_matr, Ptr_queue* ptr_queue, int n);

int somma(Ptr_queue* ptr_queue, int n);

void max_heapify(Ptr_queue* ptr_queue, int to_move, int n);
void max_heapify_modified(Ptr_queue* ptr_queue, int to_move, int n);


int main(){
    int n, //numero nodi
        k; //lunghezza classifica
    Ptr_queue* ranking; //array*1 con k migliori
    //char key_word[MAXKEY]; //parola di stacco
	uint** ptr_matr; //array di puntatori alla liste
	Ptr_queue* ptr_queue; //array (coda) di struct
	int sum; //somma cammini minimi
	int i, j, h, flag;
	int void_line;
	char c;

	flag = 0;

    //1. LETTURA INPUT n e k
    if(scanf("%d %d\n", &n, &k)){}

    //2. COSTRUZIONE ARRAY*1
    ranking = build_ranking(k);


	ptr_matr = create_adjacency_matr(n);
	//3.2 Costruisco queue (basta una volta sola) per algoritmo di Dijkstra
	ptr_queue = build_priority_queue(n);
	//D print_priority_queue(ptr_queue, n);
	j = 0;
    //3. AGGIUNGI GRAFO O TOPK
    //AggiungiGrafo
	flockfile(stdin);
    while(1){
		sum = 0;
		//Leggo key word
		c = getchar_unlocked();
        if(c == EOF){
			break;
		}
		else if(flag == 1){
			printf("\n");
			flag = 0;
		}

		//caso AggiungiGrafo
        if (c == 'A'){
			//Leggo grafo
			while(c != '\n'){
				c = getchar_unlocked();
			}
            build_adjacency_matr(ptr_matr, n);

			//D print_adjacency_matr(ptr_matr, n);
			//D printf("----------INIZIA DIJKSTRA _ %d-esimo----------\n", j);
			//Calcolo somma cammini minimi
			void_line = 1;
			for(h = 0; h < n - 1; h++){
				if(ptr_matr[0][h] != 0){
					void_line = 0;
					break;
				}
			}


			if(void_line == 0){
				dijkstra(ptr_matr, ptr_queue, n);

				sum = somma(ptr_queue, n);
				//D printf("somma[%d]: %d\n", j, sum);
				//ripulisco la coda
				clear_priority_queue(ptr_queue, n);
				//D printf("---nodo fatto\n");
			}
			//D printf("la somma è: %d\n", sum);
			//Inserisco somma in array*1
			add_ranked(ranking, j, sum, k);
			//D print_ranking(ranking, k);
			j++;
        }
        //caso TopK
        else{
			while(c != '\n'){
				c = getchar_unlocked();
			}
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
    }
	flockfile(stdin);

    return 0;
}


int power(int base, int exp){
	int res;

	if(exp == 0){
		return 1;
	}

	res = base;
	exp -= 1;
	while(exp != 0){
		res = res * base;
		exp -= 1;
	}

	return res;
}

uint atoi_personal(char* str, int h){
	int i;
	uint num;

	num = 0;
	for(i = 0; i < h; i++){
		num = num + (str[i] - '0') * power(10, h - i - 1);
		//D printf("%d\n", num);
	}

	return num;
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

	if(graph_num < k){
		ranking[graph_num] -> key = graph_num;
		ranking[graph_num] -> dist = distance;
		max_heapify_modified(ranking, graph_num, k);
	}
	else{
		if(ranking[0] -> dist > distance){
			ranking[0] -> key = graph_num;
			ranking[0] -> dist = distance;
			max_heapify(ranking, 0, k);
		}
	}
	/*
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
	}*/
}


void print_ranking(Ptr_queue* ptr_queue, int n){
	int i;

	printf("Ranking:\n");
	for(i = 0; i < n; i++){
		printf("%d, %d\n", ptr_queue[i] -> key, ptr_queue[i] -> dist);
	}
}




uint** create_adjacency_matr(int n){
    uint** ptr_matr;
    int i;


    ptr_matr = (uint**)malloc(n * sizeof(uint*));
    for(i = 0; i < n; i++){
        ptr_matr[i] = (uint*)malloc((n - 1) * sizeof(uint));
    }

    return ptr_matr;
}


void build_adjacency_matr(uint** ptr_matr, int n){
    uint i, j, h, k, num; //supporto
	char c;
	char number[10], line[100000];
	int line_len;
	//uint len;

	//Creo liste per ciascun nodo: nodo i-esimo collegato al j-esimo con costo num
	for(i = 0; i < n; i++){
		c = getchar_unlocked();
		for(h = 0; c != '\n'; h++){
			line[h] = c;
			c = getchar_unlocked();
		}
		line[h] = ',';
		line_len = h + 1;

		//D printf("riga matrice: %s\n", line);
		h = 0; //cifra del numero letta
		k = 0; //numero dei numeri letto
		for(j = 0; j < line_len; j++){
			if(line[j] != ','){
				number[h] = line[j];
				h++;
			}
			else{
				//D printf("%s\n", number);
				if(k == 0){
					//D printf("non converto %s (%d)\n", number, h);
				}
				else if(i == k){
					//D printf("non converto %s (%d)\n", number, h);
	                ptr_matr[i][k - 1] = 0;
	            }
				else{
					num = atoi_personal(number, h);
	            	ptr_matr[i][k - 1] = num;
					//D printf("convertito: %d\n", num);
	            }
				h = 0;
				k++;
			}

			/*
			for(k = 0; k < line_len; k++){

			}
			h = 0;
			//c = fgetc(stdin);
			c = getchar_unlocked();
			for(h = 0; c != ','; h++){
				//D printf("%c\n", c);
				number[k + h] = c;
				//c = fgetc(stdin);
				c = getchar_unlocked();
				h++;
			}

			//if(scanf("%d,", &num)){}
			//D printf("%s, %d\n", number, h);

			//D printf("%d\n", num);

			if(j == 0){

			}
            else if(i == j){

                ptr_matr[i][j - 1] = 0;
            }

            else{
				num = atoi_personal(number, h);
            	ptr_matr[i][j - 1] = num;
            }

			//memset(number, 0, h); peggio: 27.30% vs 26.86%

			non necessario per come ho fatto atoi personal: togliendolo scendo a 26.08%
			len = h;
			for(h = 0; h < len; h++){
				number[h] = 0;
			}*/
		}

		//L'ultimo numero lo leggo a parte
		//if(scanf("%d\n", &num)){}
		/*
		h = 0;
		//c = fgetc(stdin);
		c = getchar_unlocked();
		while(c != '\n'){
			//D printf("%c\n", c);
			number[h] = c;
			//c = fgetc(stdin);
			c = getchar_unlocked();
			h++;
		}
		num = atoi_personal(number, h);

		ptr_matr[i][n - 2] = num;


		//memset(number, 0, h);

		len = h;
		for(h = 0; h < len; h++){
			number[h] = 0;
		}*/
	}

}



//D
void print_adjacency_matr(uint** ptr_matr, int n){
	int i, j;

	printf("Adjacency Matr:\n");
	for(i = 0; i < n; i++){
		for(j = 0; j < n - 1; j++){
            printf("%d ", ptr_matr[i][j]);
        }
        printf("\n");
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
	Ptr_queue temp;

	*n_queue = *n_queue - 1;
	temp = ptr_queue[to_remove];
	ptr_queue[to_remove] = ptr_queue[*n_queue];
	ptr_queue[*n_queue] = temp;

}


void print_priority_queue(Ptr_queue* ptr_queue, int n){
	int i;

	printf("Priority Queue:\n");
	for(i = 0; i < n; i++){
		printf("%d, %d\n", ptr_queue[i] -> key, ptr_queue[i] -> dist);
	}
}


//Vedi slide 6 "data_structures_3"
void min_heapify(Ptr_queue* ptr_queue, int to_move, int n){
	uint left, right, posmin;
	Ptr_queue temp;

	//Il nodo 0 lo devo considerare come 1, altrimenti non trovo
	// il left e il right
	left = 2 * (to_move + 1) - 1;
	right = 2 * (to_move + 1);


	if(left < n && ptr_queue[left] -> dist < ptr_queue[to_move] -> dist){
		posmin = left;
	}
	else{
		posmin = to_move;
	}

	if(right < n && ptr_queue[right] -> dist < ptr_queue[posmin] -> dist){
		posmin = right;
	}

	if(posmin != to_move){
		temp = ptr_queue[to_move];
		ptr_queue[to_move] = ptr_queue[posmin];
		ptr_queue[posmin] = temp;

		min_heapify(ptr_queue, posmin, n);
	}
}


void min_heapify_modified(Ptr_queue* ptr_queue, int to_move, int n){
	int parent;
	Ptr_queue temp;

	parent = (int)(to_move / 2);

	if(parent != 0 && ptr_queue[parent] -> dist > ptr_queue[to_move] -> dist){
		temp = ptr_queue[to_move];
		ptr_queue[to_move] = ptr_queue[parent];
		ptr_queue[parent] = temp;

		min_heapify_modified(ptr_queue, parent, n);
	}
}

/*
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
}*/




uint search_in_priority_queue(Ptr_queue* ptr_queue, int to_search, int n){
	uint i;


	for(i = 0; i < n; i++){
		if(ptr_queue[i] -> key == to_search){
			return i;
		}
	}

	//non lo trovo
	return (uint)INFINITY;
}


void dijkstra(uint** ptr_matr, Ptr_queue* ptr_queue, int n){
    uint* curr_node;
    uint ndis;
    uint curr, to_reach;
    int j, n_queue;
	//modified contiene il numero dei nodi che sono stati modificati: se tale numero
	// è maggiore di n, allora posso usare la min_heapify normale
	//modified = 1; //lo 0 lo considero modificato
	n_queue = n;
	while(n_queue != 0){
		//Seleziono il nodo di partenza
		curr = ptr_queue[0] -> key;

		//Analizzo i successori del nodo corrente
		curr_node = ptr_matr[curr];

		for(j = 0; j < n - 1; j++){
			if(curr_node[j] != 0){
				ndis = ptr_queue[0] -> dist + curr_node[j];

				to_reach = search_in_priority_queue(ptr_queue, j + 1, n_queue);
				if (to_reach != (uint)INFINITY && ptr_queue[to_reach] -> dist > ndis){
					ptr_queue[to_reach] -> dist = ndis;
					//D printf("Prima della min_heapify:\n");
					//D print_priority_queue(ptr_queue, n_queue);

					min_heapify_modified(ptr_queue, to_reach, n_queue);


					//D printf("Dopo la min_heapify:\n");
					//D print_priority_queue(ptr_queue, n_queue);
				}
			}
		}
		//Elimino dalla coda il nodo di partenza
		delete_element_priority_queue(ptr_queue, 0, &n_queue);
		//D printf("Elimino il primo\n");
		//D printf("Prima della min_heapify esterna:\n");
		//D print_priority_queue(ptr_queue, n_queue);
		min_heapify(ptr_queue, 0, n_queue);
		//D printf("Dopo la min_heapify esterna:\n");
		//D print_priority_queue(ptr_queue, n_queue);
	}
}


void clear_priority_queue(Ptr_queue* ptr_queue, int n){
	int i;

	//Metto il primo nodo alla fine (dove c'è lo zero)
	ptr_queue[n-1] -> key = ptr_queue[0] -> key;
	ptr_queue[n-1] -> dist = (uint)INFINITY;

	ptr_queue[0] -> key = 0;
	ptr_queue[0] -> dist = 0;

	for(i = 1; i < n - 1; i++){
		ptr_queue[i] -> dist = (uint)INFINITY;

	}
	/*
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
	}*/

}

int somma(Ptr_queue* ptr_queue, int n){
	int i, somma;

	somma = 0;
	for(i = 0; i < n; i++){
		if(ptr_queue[i] -> dist != (uint)INFINITY){
			somma = somma + ptr_queue[i] -> dist;
		}

	}

	return somma;
}



//Vedi slide 6 "data_structures_3"
void max_heapify(Ptr_queue* ptr_queue, int to_move, int n){
	uint left, right, posmin;
	Ptr_queue temp;

	//Il nodo 0 lo devo considerare come 1, altrimenti non trovo
	// il left e il right
	left = 2 * (to_move + 1) - 1;
	right = 2 * (to_move + 1);


	if(left < n && ptr_queue[left] -> dist > ptr_queue[to_move] -> dist){
		posmin = left;
	}
	else{
		posmin = to_move;
	}

	if(right < n && ptr_queue[right] -> dist > ptr_queue[posmin] -> dist){
		posmin = right;
	}

	if(posmin != to_move){
		temp = ptr_queue[to_move];
		ptr_queue[to_move] = ptr_queue[posmin];
		ptr_queue[posmin] = temp;
		//D printf("Tra una max heapify e l'altra\n");
		//D print_ranking(ptr_queue, n);
		max_heapify(ptr_queue, posmin, n);
	}
}


void max_heapify_modified(Ptr_queue* ptr_queue, int to_move, int n){
	int parent;
	Ptr_queue temp;

	if(to_move == 0){
		return;
	}
	parent = (int)((to_move + 1) / 2) - 1;

	//D printf("%d\n", parent);
	if(ptr_queue[parent] -> dist < ptr_queue[to_move] -> dist){
		temp = ptr_queue[to_move];
		ptr_queue[to_move] = ptr_queue[parent];
		ptr_queue[parent] = temp;

		max_heapify_modified(ptr_queue, parent, n);
	}
}
