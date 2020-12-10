# :computer: Gerenciador de Memória

Projeto desenvolvido para a disciplina de Sistemas Operacionais, do curso de Sistemas de Informação da Universidade Federal de Viçosa - Campus Rio Paranaíba.
- Marcos Aurélio Duarte Souza | 5165
- Matheus Medeiros Santana | xxxx

[:link: Link do respositírio](https://github.com/matheus6d/gerenciador-memoria)

## :arrow_forward: Execução do Algorítimo

Existem dois arquivos importantes para execução do código: "vmm.c" e "anomaly.dat".
Para executar, digite o comando: 

```
$ gcc -Wall vmm.c -o vmm
```
O comando a cima, gera o arquivo para ser executado. Para acessar o programa, utilizaremos o código:

```
$ ./vmm [FUNCAO] 10 < anomaly.dat
```
Ao inserir esse código, estamos passando para a função os parametros necessários para serem analisados.
Em [FUNCAO] substitua por:
- random: caso queira utilizar a função fornecida pelo professor;
- fifo: para utilizar a função implementada pelos autores do projeto.

![exemplo1](imagem/exemplo1.png)

## :memo: Função FIFO implementada pelos autores

```
int fifo(int8_t** page_table, int num_pages, int prev_page,int fifo_frm, int num_frames, int clock) {
         
         int i;

		 do {
		 	
		 	if(page_table[i][PT_FRAMEID] == 0){
         		return i;
			 }
			 
			 ++i;
			 
		 } while (i < num_pages);
		 	 	
    return -1;
}

```
_comentar a função_

## :mag_right: RANDOM _versus_ FIFO

:arrow_right: **FIFO - First in First out**

text

![fifo](imagem/fifo.png)

:arrow_right: **RANDOM**

text

![random](imagem/random.png)

### :page_facing_up: Tabela de Resultados

| Execução | Page Fault FIFO | Page Fault RANDOM |
|:----------|:-------------:|:------:|
| 1| 9 | 7 |
| 2| 9 | 8 |
| 3| 9| 10 |
| 4| 9 | 8 | 
| 5| 9 | 10 |
| 6| 9 | 9 |
| 7|9 | 8 |
| 8| 9 | 8 |
| 9| 9| 9 | 
| 10| 9 | 9 | 
| MÉDIA| 9 | 9,5 | 

_comentarios_
