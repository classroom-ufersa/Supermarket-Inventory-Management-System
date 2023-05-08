#include <stdio.h>
#include <stdlib.h>
#include "departamento.h"
#include "produto.c"

// typedef struct departamento Departamento;

struct departamento{
   char nome[50]; 
   char porte[20]; 
   int quantidade_produtos;
   Produto* lista_produtos;
   Departamento* proximo_departamento;
   Departamento* departamento_anterior;
};

void imprime_menu(){
  printf("\n\t\tMenu\n");
  printf("1 - Adicionar produto;\n2 - Remover produto;\n3 - Listar produtos cadastrados;\n4 - Editar produto;\n5 - Buscar produto;\n6 - Consultar produtos disponíveis em um dado departamento;\n7 - Consultar quantitativo de produtos por departamento;\n8 - Sair.\n");
}


Departamento* ler_departamento_txt(void){ 
  Departamento* departamento;  
  Departamento* departamento_auxiliar;   
   int verifica_departamento = 0; 
   char linha[100]; 
  
   FILE* arquivo_departamento = fopen("/workspaces/SupermarketManagement/Supermarket_Managment/departamentos.txt", "rt");     
   if (arquivo_departamento == NULL){ 
     printf("erro no arquivo departamento!"); 
     exit(1); 
   } 
    
   while(fgets(linha, 100, arquivo_departamento) != NULL){ 
  
  departamento_auxiliar =  ler_linha_departamento(linha);
     if (verifica_departamento == 0){  
       departamento = departamento_auxiliar;
       verifica_departamento++; 
     }else{ 
       departamento_auxiliar->proximo_departamento = departamento;
       departamento->departamento_anterior = departamento_auxiliar;
       departamento = departamento_auxiliar;
       verifica_departamento++; 
     } 
   } 
   fclose(arquivo_departamento);
  return departamento;
}

Departamento* ler_linha_departamento(char* linha){
   Departamento* departamento = aloca_departamento();  
   sscanf(linha, " %[^|]| %[^\n]", departamento->nome, departamento->porte); 
   // printf("%s\t%s\n",departamento->info_departamento->nome, departamento->info_departamento->porte); 
   // printf("%s",linha); 
   return departamento;
}

Departamento* aloca_departamento(void){ 
   Departamento* novo_departamento = (Departamento*)malloc(sizeof(Departamento)); 
   if(novo_departamento == NULL){ 
     printf("erro na alocação do departamento!\n"); 
     exit(1); 
   } 
   novo_departamento->quantidade_produtos=0;
   novo_departamento->lista_produtos=NULL;
   novo_departamento->departamento_anterior=NULL;
   novo_departamento->proximo_departamento=NULL;
   return novo_departamento;
    
}


Departamento* ler_txt(){
  Departamento* departamento = ler_departamento_txt();
  departamento = ler_produto_txt(departamento);
  return departamento;
}


void lista_departamento_imprime(Departamento* departamento){
  Departamento* departamento_auxiliar = departamento; 
  Produto* lista_produtos;
  while(departamento_auxiliar != NULL){
    printf("%s %s\n", departamento_auxiliar->nome, departamento_auxiliar->porte);
    lista_produtos = departamento_auxiliar->lista_produtos;
    while (lista_produtos != NULL){
      printf("%s %s %s %d %.2f\n",lista_produtos->tipo, lista_produtos->validade,lista_produtos->fabricacao,lista_produtos->estoque, lista_produtos->preco);
      lista_produtos = lista_produtos->proximo_produto;
    }
    printf("-----------------------------------------------------------\n");
    departamento_auxiliar = departamento_auxiliar->proximo_departamento;
  }
}


Departamento* ler_produto_txt(Departamento* departamento){
    Departamento* departamento_auxiliar = departamento;
    Produto* produto;
    char linha[100];

    FILE* arquivo_produto = fopen("/workspaces/SupermarketManagement/Supermarket_Managment/produtos.txt","rt");
  if (arquivo_produto == NULL){
    printf("erro: arquivo produto!");
    exit(1);
  }

   while(fgets(linha, 100, arquivo_produto) != NULL){
    produto =  ler_linha_produto(linha);
    departamento_auxiliar = departamento;
    while(departamento_auxiliar != NULL){
      
      if (strcmp(departamento_auxiliar->nome,produto->nome_departamento) == 0){
        
        if (departamento_auxiliar->quantidade_produtos == 0){  
          departamento_auxiliar->lista_produtos = produto;
          departamento_auxiliar->quantidade_produtos++;
          printf("aaaaa\n");
        }else{
          produto->proximo_produto = departamento_auxiliar->lista_produtos;
          departamento_auxiliar->lista_produtos->produto_anterior = produto;
          departamento_auxiliar->lista_produtos = produto;
          departamento_auxiliar->quantidade_produtos++;
          printf("aaaaa\n");
        }
      }
      departamento_auxiliar = departamento_auxiliar->proximo_departamento;
    }
  }

  fclose(arquivo_produto);
  return (departamento);
}


void insere_novo_produto(Departamento* departamento, char* tipo, char* validade, char* fabricacao, int estoque, char* nome_departamento, float preco){
  Departamento* departamento_auxiliar = departamento;
  Produto* novo_produto = aloca_produto();
  if(novo_produto == NULL){
    printf("erro!!!");
    exit(1);
  }

  strcpy(novo_produto->tipo,tipo);
  strcpy(novo_produto->fabricacao,fabricacao);
  strcpy(novo_produto->nome_departamento,nome_departamento);
  strcpy(novo_produto->validade,validade);
  novo_produto->estoque = estoque;
  novo_produto->preco = preco;

  while(departamento_auxiliar != NULL){
    
    if(strcmp(departamento_auxiliar->nome,nome_departamento) == 0){
      novo_produto->proximo_produto = departamento_auxiliar->lista_produtos;
      departamento_auxiliar->lista_produtos->produto_anterior = novo_produto;
      departamento_auxiliar->lista_produtos = novo_produto;
      departamento_auxiliar->quantidade_produtos++;
    }
    departamento_auxiliar = departamento_auxiliar->proximo_departamento;
  }
  departamento = departamento_auxiliar;
}


void lista_imprime_txt(Departamento* departamentos){
  Departamento* departamento_auxiliar = departamentos;
  Produto* produtos;
  FILE* arquivo = fopen("arquivo_teste.txt","wt");
  while(departamento_auxiliar != NULL){
    fprintf(arquivo,"%s| %s\n",departamento_auxiliar->nome,departamento_auxiliar->porte);
    for(produtos = departamento_auxiliar->lista_produtos; produtos != NULL; produtos = produtos->proximo_produto){
      fprintf(arquivo," %s| %s| %s| %d|%.2f\n", produtos->tipo,produtos->fabricacao, produtos->validade, produtos->estoque, produtos->preco);
    }
    fprintf(arquivo,"****\n");
    departamento_auxiliar = departamento_auxiliar->proximo_departamento;
  }
  fclose(arquivo);
}



void busca_produto(Departamento* departamento, char* nome_produto){
  int achou_produto=0;
  Departamento* departamento_auxiliar = departamento;
  Produto* produto_auxiliar; 
   
   while(departamento_auxiliar != NULL){
    produto_auxiliar = departamento_auxiliar->lista_produtos;
    while (produto_auxiliar != NULL){
      
   if(strcmp(produto_auxiliar->tipo, nome_produto) == 0){
      printf("nome do departamento que se encontra o produto: %s\n",produto_auxiliar->nome_departamento);
      printf("preço do produto: %.2f\n",produto_auxiliar->preco);
      printf("data de fabricação do produto: %s\n",produto_auxiliar->fabricacao);
      printf("data de validade do produto: %s\n",produto_auxiliar->validade);
      printf("quantidade disponivel em estoque: %d\n\n",produto_auxiliar->estoque);
      achou_produto++;
    }
      produto_auxiliar = produto_auxiliar->proximo_produto;
    }
    departamento_auxiliar = departamento_auxiliar->proximo_departamento;
  }
   if(achou_produto == 0){
      printf("produto não encontrado");
    }
}


void produtos_por_departamento(Departamento* departamento){
  Departamento* departamento_auxiliar = departamento;
 
  while(departamento_auxiliar!=NULL){
      printf("\ndepartamento: %s\nquantidade de produtos: %d\n\n",departamento_auxiliar->nome,departamento_auxiliar->quantidade_produtos);
   
    departamento_auxiliar = departamento_auxiliar->proximo_departamento;
  }

}