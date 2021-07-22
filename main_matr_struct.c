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
#include <stdbool.h>
#define MAXKEY 16


//Coda con priorità
struct Queue{
	uint key;
	uint dist;
};

struct Hash{
	bool visited; //1 if visited
	uint pos; //pos of that node in ptr_queue
};


int power(int base, int exp);
uint atoi_personal(char* str, int h);

struct Queue* build_ranking(int k);
void add_ranked(struct Queue* ranking, int graph_num, int distance, int k);
void print_ranking(struct Queue* ptr_queue, int n);

uint** create_adjacency_matr(int n);
void build_adjacency_matr(uint** ptr_matr, int n);
void print_adjacency_matr(uint** ptr_matr, int n);


struct Hash* create_hash(int n);
struct Queue* build_priority_queue(int n);
void print_priority_queue(struct Queue* ptr_queue, int n);
void clear_priority_queue(struct Queue* ptr_queue, struct Hash* hash, int n);
void min_heapify(struct Queue* ptr_queue, struct Hash* hash, int to_move, int n);
void min_heapify_modified(struct Queue* ptr_queue, struct Hash* hash, int to_move, int n);
uint search_in_priority_queue(struct Queue* ptr_queue, struct Hash* hash, int to_search);
void delete_element_priority_queue(struct Queue* ptr_queue, struct Hash* hash, int to_remove, int* n);

void dijkstra(uint** ptr_matr, struct Queue* ptr_queue, struct Hash* hash, int n);

int somma(struct Queue* ptr_queue, int n);

void max_heapify(struct Queue* ptr_queue, int to_move, int n);
void max_heapify_modified(struct Queue* ptr_queue, int to_move, int n);

void print_topk(struct Queue* ranking, int k);


int main(){
    int n, //numero nodi
        k; //lunghezza classifica
    struct Queue* ranking; //array*1 con k migliori
    //char key_word[MAXKEY]; //parola di stacco
	uint** ptr_matr; //array di puntatori alla liste
	struct Queue* ptr_queue; //array (coda) di struct
	int sum; //somma cammini minimi
	int j, h, flag;
	int void_line;
	char c;
	struct Hash* hash;

	flag = 0;

    //1. LETTURA INPUT n e k
    if(scanf("%d %d\n", &n, &k)){}

    //2. COSTRUZIONE ARRAY*1
    ranking = build_ranking(k);


	ptr_matr = create_adjacency_matr(n);
	//3.2 Costruisco queue (basta una volta sola) per algoritmo di Dijkstra
	ptr_queue = build_priority_queue(n);
	//D print_priority_queue(ptr_queue, n);
	hash = create_hash(n);
	j = 0;
    //3. AGGIUNGI GRAFO O TOPK
    //AggiungiGrafo
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
		/*
		sum = 0;
		//Leggo key word
        if(fgets(key_word, MAXKEY, stdin) == NULL){
			break;
		}
		else if(flag == 1){
			printf("\n");
			flag = 0;
		}*/

		//caso AggiungiGrafo
        if (c == 'A'){
			/*while(c != '\n'){
				c = getchar_unlocked();
			}*/
			//Leggo grafo
			//leggermente più veloce di while(c!='\n')
			getchar_unlocked();
			getchar_unlocked();
			getchar_unlocked();
			getchar_unlocked();
			getchar_unlocked();
			getchar_unlocked();
			getchar_unlocked();
			getchar_unlocked();
			getchar_unlocked();
			getchar_unlocked();
			getchar_unlocked();
			getchar_unlocked();
			getchar_unlocked();

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
				dijkstra(ptr_matr, ptr_queue, hash, n);

				sum = somma(ptr_queue, n);
				//D printf("somma[%d]: %d\n", j, sum);
				//ripulisco la coda
				clear_priority_queue(ptr_queue, hash, n);

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
			/*
			while(c != '\n'){
				c = getchar_unlocked();
			}*/
			getchar_unlocked();
			getchar_unlocked();
			getchar_unlocked();
			getchar_unlocked();

			print_topk(ranking, k);
			flag = 1;
        }
    }

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


struct Queue* build_ranking(int k){
	struct Queue* ranking;
	int i;


	ranking = (struct Queue*)malloc(k * sizeof(struct Queue));

	//imposto gli altri nodi a infinito
	for(i = 0; i < k; i++){
		ranking[i].key = (uint)INFINITY;
		ranking[i].dist = (uint)INFINITY;
	}


	return ranking;
}


void add_ranked(struct Queue* ranking, int graph_num, int distance, int k){

	if(graph_num < k){
		ranking[graph_num].key = graph_num;
		ranking[graph_num].dist = distance;
		max_heapify_modified(ranking, graph_num, k);
	}
	else{
		if(ranking[0].dist > distance){
			ranking[0].key = graph_num;
			ranking[0].dist = distance;
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


void print_ranking(struct Queue* ptr_queue, int n){
	int i;

	printf("Ranking:\n");
	for(i = 0; i < n; i++){
		printf("%d, %d\n", ptr_queue[i].key, ptr_queue[i].dist);
	}
}


struct Hash* create_hash(int n){
	struct Hash* hash;
	int i;

	hash = (struct Hash*)malloc(n * sizeof(struct Hash));

	hash[0].visited = 0;
	hash[0].pos = 0;
	for(i = 1; i < n; i++){
		hash[i].visited = 0;
		hash[i].pos = i;
	}

	return hash;
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
    uint i, j, h, num; //supporto
	char c;
	char number[10];
	//uint len;

	//Creo liste per ciascun nodo: nodo i-esimo collegato al j-esimo con costo num
	for(i = 0; i < n; i++){
		flockfile(stdin);
		//D printf("riga matrice: %s\n", line);
		for(j = 0; j < n-1; j++){
			h = 0;
			//c = fgetc(stdin);
			c = getchar_unlocked();
			while(c != ','){
				//D printf("%c\n", c);
				number[h] = c;
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

			/*non necessario per come ho fatto atoi personal: togliendolo scendo a 26.08%
			len = h;
			for(h = 0; h < len; h++){
				number[h] = 0;
			}*/
		}

		//L'ultimo numero lo leggo a parte
		//if(scanf("%d\n", &num)){}
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
		/*
		len = h;
		for(h = 0; h < len; h++){
			number[h] = 0;
		}*/
	}
	flockfile(stdin);
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


struct Queue* build_priority_queue(int n){
	struct Queue* ptr_queue;
	int i;


	ptr_queue = (struct Queue*)malloc(n * sizeof(struct Queue));

	//parto sempre dal nodo zero
	ptr_queue[0].key = 0;
	ptr_queue[0].dist = 0;

	//imposto gli altri nodi a infinito
	for(i = 1; i < n; i++){
		ptr_queue[i].key = i;
		ptr_queue[i].dist = (uint)INFINITY;
	}


	return ptr_queue;
}


void delete_element_priority_queue(struct Queue* ptr_queue, struct Hash* hash, int to_remove, int* n_queue){
	struct Queue temp;

	*n_queue = *n_queue - 1;
	//Modifico hash
	hash[ptr_queue[*n_queue].key].pos = to_remove;
	hash[ptr_queue[to_remove].key].pos = *n_queue;
	hash[ptr_queue[to_remove].key].visited = 1;
	//Modifico ptr_queue
	temp = ptr_queue[to_remove];
	ptr_queue[to_remove] = ptr_queue[*n_queue];
	ptr_queue[*n_queue] = temp;

}


void print_priority_queue(struct Queue* ptr_queue, int n){
	int i;

	printf("Priority Queue:\n");
	for(i = 0; i < n; i++){
		printf("%d, %d\n", ptr_queue[i].key, ptr_queue[i].dist);
	}
}


//Vedi slide 6 "data_structures_3"
void min_heapify(struct Queue* ptr_queue, struct Hash* hash, int to_move, int n){
	uint left, right, posmin;
	struct Queue temp;

	//Il nodo 0 lo devo considerare come 1, altrimenti non trovo
	// il left e il right
	left = 2 * (to_move + 1) - 1;
	right = 2 * (to_move + 1);


	if(left < n && ptr_queue[left].dist < ptr_queue[to_move].dist){
		posmin = left;
	}
	else{
		posmin = to_move;
	}

	if(right < n && ptr_queue[right].dist < ptr_queue[posmin].dist){
		posmin = right;
	}

	if(posmin != to_move){
		//Modifico hash
		hash[ptr_queue[to_move].key].pos = posmin;
		hash[ptr_queue[posmin].key].pos = to_move;
		//Modifico ptr_queue
		temp = ptr_queue[to_move];
		ptr_queue[to_move] = ptr_queue[posmin];
		ptr_queue[posmin] = temp;

		min_heapify(ptr_queue, hash, posmin, n);
	}
}


void min_heapify_modified(struct Queue* ptr_queue, struct Hash* hash, int to_move, int n){
	int parent;
	struct Queue temp;

	parent = (int)((to_move - 1) / 2);

	if(parent != 0 && ptr_queue[parent].dist > ptr_queue[to_move].dist){
		//Modifico hash table
		hash[ptr_queue[parent].key].pos = to_move;
		hash[ptr_queue[to_move].key].pos = parent;
		//Modifico ptr_queue
		temp = ptr_queue[to_move];
		ptr_queue[to_move] = ptr_queue[parent];
		ptr_queue[parent] = temp;

		min_heapify_modified(ptr_queue, hash, parent, n);
	}
}

/*
void min_heapify(struct Queue* ptr_queue, int to_move, int n){
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




uint search_in_priority_queue(struct Queue* ptr_queue, struct Hash* hash, int to_search){

	if(hash[to_search].visited == 0){
		return hash[to_search].pos;
	}
	else{
		return (uint)INFINITY;
	}
}


void dijkstra(uint** ptr_matr, struct Queue* ptr_queue, struct Hash* hash, int n){
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
		curr = ptr_queue[0].key;

		//Analizzo i successori del nodo corrente
		curr_node = ptr_matr[curr];

		for(j = 0; j < n - 1; j++){
			if(curr_node[j] != 0){
				ndis = ptr_queue[0].dist + curr_node[j];

				to_reach = search_in_priority_queue(ptr_queue, hash, j + 1);
				if (to_reach != (uint)INFINITY && ptr_queue[to_reach].dist > ndis){
					ptr_queue[to_reach].dist = ndis;
					//D printf("Prima della min_heapify:\n");
					//D print_priority_queue(ptr_queue, n_queue);

					min_heapify_modified(ptr_queue, hash, to_reach, n_queue);


					//D printf("Dopo la min_heapify:\n");
					//D print_priority_queue(ptr_queue, n_queue);
				}
			}
		}
		//Elimino dalla coda il nodo di partenza
		delete_element_priority_queue(ptr_queue, hash, 0, &n_queue);
		//D printf("Elimino il primo\n");
		//D printf("Prima della min_heapify esterna:\n");
		//D print_priority_queue(ptr_queue, n_queue);
		min_heapify(ptr_queue, hash, 0, n_queue);
		//D printf("Dopo la min_heapify esterna:\n");
		//D print_priority_queue(ptr_queue, n_queue);
	}
}


void clear_priority_queue(struct Queue* ptr_queue, struct Hash* hash, int n){
	int i;

	//Metto il primo nodo alla fine (dove c'è lo zero)
	hash[ptr_queue[0].key].pos = n - 1;
	ptr_queue[n-1].key = ptr_queue[0].key;
	//ptr_queue[n-1].dist = (uint)INFINITY;

	hash[0].pos = 0;
	ptr_queue[0].key = 0;
	ptr_queue[0].dist= 0;

	for(i = 1; i < n; i++){
		hash[i].visited = 0;
		ptr_queue[i].dist = (uint)INFINITY;

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

int somma(struct Queue* ptr_queue, int n){
	int i, somma;

	somma = 0;
	for(i = 0; i < n; i++){
		if(ptr_queue[i].dist != (uint)INFINITY){
			somma = somma + ptr_queue[i].dist;
		}

	}

	return somma;
}



//Vedi slide 6 "data_structures_3"
void max_heapify(struct Queue* ptr_queue, int to_move, int n){
	uint left, right, posmin;
	struct Queue temp;

	//Il nodo 0 lo devo considerare come 1, altrimenti non trovo
	// il left e il right
	left = 2 * (to_move + 1) - 1;
	right = 2 * (to_move + 1);


	if(left < n && ptr_queue[left].dist > ptr_queue[to_move].dist){
		posmin = left;
	}
	else{
		posmin = to_move;
	}

	if(right < n && ptr_queue[right].dist > ptr_queue[posmin].dist){
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


void max_heapify_modified(struct Queue* ptr_queue, int to_move, int n){
	int parent;
	struct Queue temp;

	if(to_move == 0){
		return;
	}
	parent = (int)((to_move + 1) / 2) - 1;

	//D printf("%d\n", parent);
	if(ptr_queue[parent].dist < ptr_queue[to_move].dist){
		temp = ptr_queue[to_move];
		ptr_queue[to_move] = ptr_queue[parent];
		ptr_queue[parent] = temp;

		max_heapify_modified(ptr_queue, parent, n);
	}
}



void print_topk(struct Queue* ranking, int k){
	int i;

	//D print_priority_queue(ranking, k);
	if(ranking[0].key != (uint)INFINITY){
		printf("%d", ranking[0].key);
	}
	for(i = 1; i < k; i++){
		if(ranking[i].key != (uint)INFINITY){
			printf(" %d", ranking[i].key);
		}
	}

	/*
	if(!feof(stdin)){
		printf("\n");
	}
	if(feof(stdin)){
		break;
	}*/
}
