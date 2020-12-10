#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

// Campos da tabela de paginas
#define PT_FIELDS 6           // 4 campos na tabela
#define PT_FRAMEID 0          // Endereco da memoria fisica
#define PT_MAPPED 1           // Endereco presente na tabela
#define PT_DIRTY 2            // Pagina dirty
#define PT_REFERENCE_BIT 3    // Bit de referencia
#define PT_REFERENCE_MODE 4   // Tipo de acesso, converter para char
#define PT_AGING_COUNTER 5    // Contador para aging

// Tipos de acesso
#define READ 'r'
#define WRITE 'w'

// Define a funcao que simula o algoritmo da politica de subst.
typedef int (*eviction_f)(int8_t**, int, int, int, int, int);

typedef struct {
    char *name;
    void *function;
} paging_policy_t;

// Codifique as reposicoes a partir daqui!
// Cada metodo abaixo retorna uma pagina para ser trocada. Note tambem
// que cada algoritmo recebe:
// - A tabela de paginas
// - O tamanho da mesma
// - A ultima página acessada
// - A primeira modura acessada (para fifo)
// - O numero de molduras
// - Se a ultima instrucao gerou um ciclo de clock
//
// Adicione mais parâmetros caso ache necessário

//A pagina mais antiga pode ser removida
int fifo(int8_t** page_table, int num_pages, int prev_page,
         int fifo_frm, int num_frames, int clock) {
         
         int i; //Incrementador 

		 do {
		 	//Se o endereço físico é o mais velho para ser retornado
		 	if(page_table[i][PT_FRAMEID] == 0){
         		return i;
			 }
			 
			 ++i; //Incrementa para o próximo
			 
		 } while (i < num_pages); //Roda até encontrar a página vitima
		 	 	
    return -1;
}

int second_chance(int8_t** page_table, int num_pages, int prev_page,
                  int fifo_frm, int num_frames, int clock) {
    return -1;
}

int nru(int8_t** page_table, int num_pages, int prev_page,
        int fifo_frm, int num_frames, int clock) {
    return -1;
}

int aging(int8_t** page_table, int num_pages, int prev_page,
          int fifo_frm, int num_frames, int clock) {
    return -1;
}

int random_page(int8_t** page_table, int num_pages, int prev_page,
                int fifo_frm, int num_frames, int clock) {
    int page = rand() % num_pages;
    while (page_table[page][PT_MAPPED] == 0) // Encontra pagina mapeada
        page = rand() % num_pages;
    return page;
}

// Simulador a partir daqui

int find_next_frame(int *physical_memory, int *num_free_frames,
                    int num_frames, int *prev_free) {
    if (*num_free_frames == 0) {
        return -1;
    }

    // Procura por um frame livre de forma circula na memória.
    // Nao e muito eficiente, mas fazer um hash em C seria mais custoso.
    do {
        *prev_free = (*prev_free + 1) % num_frames;
    } while (physical_memory[*prev_free] == 1);

    return *prev_free;
}

int simulate(int8_t **page_table, int num_pages, int *prev_page, int *fifo_frm,
             int *physical_memory, int *num_free_frames, int num_frames,
             int *prev_free, int virt_addr, char access_type,
             eviction_f evict, int clock) {
    if (virt_addr >= num_pages || virt_addr < 0) {
        printf("Invalid access \n");
        exit(1);
    }

    if (page_table[virt_addr][PT_MAPPED] == 1) {
        page_table[virt_addr][PT_REFERENCE_BIT] = 1;
        return 0; // Not Page Fault!
    }

    int next_frame_addr;
    if ((*num_free_frames) > 0) { // Ainda temos memoria fisica livre!
        next_frame_addr = find_next_frame(physical_memory, num_free_frames,
                                          num_frames, prev_free);
        if (*fifo_frm == -1)
            *fifo_frm = next_frame_addr;
        *num_free_frames = *num_free_frames - 1;
    } else { // Precisamos liberar a memoria!
        assert(*num_free_frames == 0);
        int to_free = evict(page_table, num_pages, *prev_page, *fifo_frm,
                            num_frames, clock);
        assert(to_free >= 0);
        assert(to_free < num_pages);
        assert(page_table[to_free][PT_MAPPED] != 0);

        next_frame_addr = page_table[to_free][PT_FRAMEID];
        *fifo_frm = (*fifo_frm + 1) % num_frames;
        // Libera pagina antiga
        page_table[to_free][PT_FRAMEID] = -1;
        page_table[to_free][PT_MAPPED] = 0;
        page_table[to_free][PT_DIRTY] = 0;
        page_table[to_free][PT_REFERENCE_BIT] = 0;
        page_table[to_free][PT_REFERENCE_MODE] = 0;
        page_table[to_free][PT_AGING_COUNTER] = 0;
    }

    // Coloca endereco fisico na tabela de paginas!
    int8_t *page_table_data = page_table[virt_addr];
    page_table_data[PT_FRAMEID] = next_frame_addr;
    page_table_data[PT_MAPPED] = 1;
    if (access_type == WRITE) {
        page_table_data[PT_DIRTY] = 1;
    }
    page_table_data[PT_REFERENCE_BIT] = 1;
    page_table_data[PT_REFERENCE_MODE] = (int8_t) access_type;
    *prev_page = virt_addr;

    if (clock == 1) {
    	int i;
        for (i = 0; i < num_pages; i++)
            page_table[i][PT_REFERENCE_BIT] = 0;
    }

    return 1; // Page Fault!
}

void run(int8_t **page_table, int num_pages, int *prev_page, int *fifo_frm,
         int *physical_memory, int *num_free_frames, int num_frames,
         int *prev_free, eviction_f evict, int clock_freq) {
    int virt_addr;
    char access_type;
    int i = 0;
    int clock = 0;
    int faults = 0;
    while (scanf("%d", &virt_addr) == 1) {
        getchar();
        scanf("%c", &access_type);
        clock = ((i+1) % clock_freq) == 0;
        faults += simulate(page_table, num_pages, prev_page, fifo_frm,
                           physical_memory, num_free_frames, num_frames, prev_free,
                           virt_addr, access_type, evict, clock);
        i++;
    }
    printf("%d\n", faults);
}

int parse(char *opt) {
    char* remainder;
    int return_val = strtol(opt, &remainder, 10);
    if (strcmp(remainder, opt) == 0) {
        printf("Error parsing: %s\n", opt);
        exit(1);
    }
    return return_val;
}

void read_header(int *num_pages, int *num_frames) {
    scanf("%d %d\n", num_pages, num_frames);
}

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Usage %s <algorithm> <clock_freq>\n", argv[0]);
        exit(1);
    }

    char *algorithm = argv[1];
    int clock_freq = parse(argv[2]);
    int num_pages;
    int num_frames;
    read_header(&num_pages, &num_frames);

    // Aponta para cada funcao que realmente roda a politica de parse
    paging_policy_t policies[] = {
            {"fifo", *fifo},
            {"second_chance", *second_chance},
            {"nru", *nru},
            {"aging", *aging},
            {"random", *random_page}
    };

    int n_policies = sizeof(policies) / sizeof(policies[0]);
    eviction_f evict = NULL;
    int i;
    for (i = 0; i < n_policies; i++) {
        if (strcmp(policies[i].name, algorithm) == 0) {
            evict = policies[i].function;
            break;
        }
    }

    if (evict == NULL) {
        printf("Please pass a valid paging algorithm.\n");
        exit(1);
    }

    // Aloca tabela de paginas
    int8_t **page_table = (int8_t **) malloc(num_pages * sizeof(int8_t*));
   
    for (i = 0; i < num_pages; i++) {
        page_table[i] = (int8_t *) malloc(PT_FIELDS * sizeof(int8_t));
        page_table[i][PT_FRAMEID] = -1;
        page_table[i][PT_MAPPED] = 0;
        page_table[i][PT_DIRTY] = 0;
        page_table[i][PT_REFERENCE_BIT] = 0;
        page_table[i][PT_REFERENCE_MODE] = 0;
        page_table[i][PT_AGING_COUNTER] = 0;
    }

    // Memoria Real e apenas uma tabela de bits (na verdade uso ints) indicando
    // quais frames/molduras estao livre. 0 == livre!
    int *physical_memory = (int *) malloc(num_frames * sizeof(int));
    for (i = 0; i < num_frames; i++) {
        physical_memory[i] = 0;
    }
    int num_free_frames = num_frames;
    int prev_free = -1;
    int prev_page = -1;
    int fifo_frm = -1;

    // Roda o simulador
    srand(time(NULL));
    run(page_table, num_pages, &prev_page, &fifo_frm, physical_memory,
        &num_free_frames, num_frames, &prev_free, evict, clock_freq);

    // Liberando os mallocs
    for (i = 0; i < num_pages; i++) {
        free(page_table[i]);
    }
    free(page_table);
    free(physical_memory);
}
