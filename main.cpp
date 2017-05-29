#include <stdio.h>
#include "tinyxml2.h"
#include <iostream>
#include "libpq-fe.h"
#include <cstring>
#include <stdlib.h>
#include <locale>

#pragma comment(lib, "libpq.lib")


using namespace tinyxml2;
using namespace std;

#ifndef XMLCheckResult
#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif


void error(char *mess)
{
	fprintf(stderr, "### %s\n", mess);
	getchar();
	exit(1);
}

PGconn *conn;
PGresult *res; 
// ########################################## MODEL ###############################################################
class Model {
public:

	    int leitura_xml(/*Model& model*/) {			//LEITURA DO ARQUIVO XML E PASSANDO OS DADOS PARA O MODEL
		XMLDocument xmlDoc;
		XMLError eResult = xmlDoc.LoadFile("C:/filmes.xml"); //Carregando arquivo externo xml para o programa
		XMLCheckResult(eResult); //Erro se o xml for invalido

		XMLNode * lista_filmes = xmlDoc.FirstChildElement("filmes");
		if (lista_filmes == nullptr) return XML_ERROR_FILE_READ_ERROR;

		XMLElement * filme = lista_filmes->FirstChildElement("filme");
		if (filme == nullptr) return XML_ERROR_PARSING_ELEMENT;

		while (filme != nullptr)
		{
			int id;
			XMLElement * valor = filme->FirstChildElement("id");
			if (valor == nullptr) return XML_ERROR_PARSING_ELEMENT;
			eResult = valor->QueryIntText(&id);
			XMLCheckResult(eResult);

			valor = filme->FirstChildElement("title");
			if (valor == nullptr) return XML_ERROR_PARSING_ELEMENT;
			const char* title = valor->GetText();

			valor = filme->FirstChildElement("description");
			if (valor == nullptr) return XML_ERROR_PARSING_ELEMENT;
			const char* descr = valor->GetText();

			valor = filme->FirstChildElement("genre");
			if (valor == nullptr) return XML_ERROR_PARSING_ELEMENT;
			const char* genre = valor->GetText();
			//model.
			escreve_BD(id, title, descr, genre); // UM METODO DE MODEL CHAMA O OUTRO
			filme = filme->NextSiblingElement("filme");
		}
	}
	void valores_banco(const char* id,
		const char* title,
		const char* description,
		const char* genre)
	{

	}
	/*METODO ENVIA DADOS AO BD*/
	void escreve_BD (const int& rec_id,			
						const char* rec_title, 
						const char* rec_description, 
						const char* rec_genre)
	{
		
		int id;
		const char* title;
		const char* description;
		const char* genre;
		id = rec_id;
		title = rec_title;
		description = rec_description; 
		genre = rec_genre;


		const char *values[4] = { (char *)&id, title, description, genre };
		//calcular o tamanho dos valores
		int lengths[4] = { sizeof(id), strlen(title), strlen(description), strlen(genre) };
		//state which parameters are binary
		int binary[4] = { 1, 0, 0, 0 };


		res = PQexecParams(conn,
			"INSERT INTO filmes(id, title, description, genre) VALUES ($1::integer , $2, $3, $4)",
			4, //number of parameters
			NULL, //ignore the Oid field
			values, //values to substitute $1....$4
			lengths, //the lengths, in bytes, of each of the parameter values
			binary, //whether the values are binary or not
			0); //we want the result in text format
		if (PQresultStatus(res) != PGRES_TUPLES_OK)
		{
			printf(PQerrorMessage(conn));
			PQclear(res);
		}
	}
	const string& leitura_BD(int aux) const		
	{
		if (aux == 2)
			res = PQexec(conn, "select title, description, genre from filmes order by title");
		if (aux == 3)
			res = PQexec(conn, "select  genre, count(genre) from filmes group by genre");
		if (aux == 4)
			res = PQexec(conn, "select title, description, genre from filmes order by id");
	if (PQresultStatus(res) != PGRES_TUPLES_OK)
		{
		printf(PQerrorMessage(conn));
		PQclear(res);
		PQfinish(conn);
		return "Erro!";
	}
	int nRegistros = PQntuples(res); 	// Obtém o número de registro
	int nCampos = PQnfields(res);		// Obtém o número de colunas
	printf("\n");
	for (int i = 0; i < nRegistros; i++) // Percorre todos os registros
	{
		printf("\t");
		for (int j = 0; j < nCampos; j++) // Percorre todas as colunas
		{
			printf("%-20s", PQgetvalue(res, i, j)); // Imprime o valor do campo
		}
		printf("\n");
	}

	}
};

class View {                                                                                                                                                                                    
public:
	void menu() {
		
		int j;
		cout << "\n\n\t";			
		for (j = 0; j<40; j++)	
		{						
			cout << "#";			
		}						
		cout << "\n\n\tTRABALHO MVC\n\t\t\t\t \n\t";

		for (j = 0; j<40; j++)	
		{			
			cout << "#";		
		}					

		cout << "\n\n\t1==>\ LER XML -> GRAVAR BD" << endl;		
		cout << "\t2==>\ CONSULTAR DADOS EM ORDEM ALFABETICA PELO TITULO" << endl;
		cout << "\t3==>\ CONSULTAR DADOS AGRUPADOS POR GENERO" << endl;				
		cout << "\t4==>\ CONSULTAR DADOS EM ORDEM PELO ID" << endl;				
		cout << "\t0==>\ SAIR = 0\n" << endl;			
	}

	void mostra_leitura_BD(const Model& modelo, int aux){
		modelo.leitura_BD(aux);
	}
};
//#################### CONTROLLER ###################################################################################
class Controller {
public:
	Model modelo; // objeto modelo 
	View exibicao; // objeto view
	int iniciar_prog (){
	int iniciar;
		while(true){
			exibicao.menu();
			cin >> iniciar;
			if (iniciar == 1)
			{	
				modelo.leitura_xml(); // chama seu metodo 
				getchar();
			}
			if (iniciar == 2 || iniciar == 3 || iniciar == 4) exibicao.mostra_leitura_BD(modelo, iniciar); 
			if (iniciar == 0)return 0;
		}
	}
};


int main()
{
	setlocale(LC_ALL, "Portuguese");
	conn = PQsetdbLogin( //CONECTANDO COM O BANCO POSTGRES
		"localhost", // host
		"5432", // port
		NULL, // options
		NULL, // tty
		"locadora", // dbName
		"postgres", // login
		"141987" // password
	);


	if (PQstatus(conn) != CONNECTION_OK)
	{
		printf(PQerrorMessage(conn));
		PQfinish(conn);
		return 1;
	}

	Model      model; //Sempre que você pensar em manipulação de dados, pense em model. Ele é responsável pela leitura e escrita de dados, e também de suas validações.
	View       view; //Simples: a camada de interação com o usuário. Ela apenas faz a  exibição dos dados, sendo ela por meio de um html ou xml.
	Controller controller; //O responsável por receber todas as requisições do usuário. Seus métodos chamados actions são responsáveis por uma página, controlando qual model usar e qual view será mostrado ao usuário.


	Controller Controle; // objeto controller 
	Controle.iniciar_prog(); // acessa seu metodo 

	PQclear(res); // Fecha o acesso aos registros
	PQfinish(conn); // Fecha a conexão com o banco 

	return 0;
}