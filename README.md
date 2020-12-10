# Gerenciador de Memória

Projeto desenvolvido para a disciplina de Sistemas Operacionais, do curso de Sistemas de Informação da Universidade Federal de Viçosa.
- Marcos Aurélio Duarte Souza | 5165
- Matheus Medeiros Santana | xxxx

## 1. Execução do Algorítimo

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

## 2. RANDOM X FIFO
