/*
-----------------------------------------------------------------------------
 Nome      : SlidingPuzzle.cpp
 Data	   : 20 de abril de 2013
 Curso     : PROGRAMACAO/MIEIC 2012/13 ~ FEUP
 Autores   : Danilo A. D. Carvalho // Renan A. B. Vieira
 Grupo     : T7G02
 Sist. Op. : Desenvolvido em Visual Studio 2010 / Windows 7
 Descriçao : Implementacao do jogo "Sliding Puzzle". Este codigo apresenta dois modos de jogo,
			 [T]TRAINING MODE e [C]COMPETITION MODE. Em ambos os modos,

  O jogador é desafiado a resolver puzzles de tamanho 3x3, 4x4 ou 5x5.
  Podem ser gerados novos puzzles ou carregar antigos (salvos em jogos anteriores).
 [T] Modo em que o jogador resolve puzzles apenas para treino.
 [C] Modo em que o jogador entra em competicao com outros jogadores.
	 Caso resolva o puzzle entre os "PLAYERS_SCORE_MAX" melhores tempos, os dados do jogador 
	 são armazenados. Desta forma, este pode ser desafiado posteriormente.
-----------------------------------------------------------------------------
*/
/*bibliotecas utilizadas*/
#include <iostream>
#include <cctype>
#include <vector>
#include <string>
#include <sstream>
#include <fstream> 
#include <ctime>
#include <stdio.h>
#include <algorithm>
using namespace std;

const int SHUFFLETIMES = 2;       /* Indica quantas vezes o tabuleiro ordenado será embaralhado */
const int PLAYERS_SCORE_MAX = 10; /* Indica quantos melhores tempos sao armazenados no ficheiro */
const int BOARD_SIZE_MIN = 3; /* Indica o tamnho minino de tabuleiro permitido */
const int BOARD_SIZE_MAX = 5; /* Indica o tamnho maximo de tabuleiro permitido */

	 /*------------------------------------------ readChar ----------------------------------
     |  Funcao readChar
	 |
	 |  Proposito: Realiza a leitura de variaveis do tipo char com controle
	 |             para eventuais erros de insercao do utilizador. 
	 |     A leitura do dado inserido pelo utilizador e' adicionado a uma string 
	 |  auxiliar e esta e' adicionada a uma stringstream. Da stringstream a 
	 |  informacao e' escrita em um caracter se for possivel tal insercao. Caso a 
	 |  insercao tenha sido possivel e' verificado se a informacao digitada pelo 
	 |  utilizador corresponde ao que e' permitido pelo programa.
	 |       Ex: Caso seja solicitado uma pergunta cuja resposta permitida seja: 'Y' ou 'N'
	 |           e' verificado se o utilizador digitou um desses dois caracteres.
	 |
	 |  Parametros:
	 |     message (OUT)- Variavel que guarda a mensagem que sera escrita para o utilizador
	 |                    solicitando a insercao de dados.
	 |
	 |	   option (IN)  - Variavel que informa os possiveis caracteres que podem ser inseridos
	 |                    pelo utilizador.
	 |
	 |  Retorno: Char.  Caracter valido digitado pelo utilizador.
	 *--------------------------------------------------------------------------------------*/
char readChar(string message, string option){
	stringstream readChar;
	string auxReadChar;
	char str;

	cout << message;
	while(true){
		readChar.clear();
		auxReadChar.clear();
		fflush(stdin);
		getline(cin,auxReadChar);
		readChar.str(auxReadChar);
		readChar >> str;
		for (size_t i=0; i<option.length(); i++)
			if(option[i] == str || toupper(option[i]) == str)
				return tolower(str);
		cout << "\n<<Respostas Aceitaveis: [ ";
		for (size_t i=0; i<option.length(); i++){
			cout << option[i];
			if(i<option.length()-1)
				cout << " ou ";
		}
		cout << " ]>>\n" << "\nIntroduza novamente: ";
	}
}

	 /*------------------------------------------ readInt --------------------------------------
     |  Funcao readInt
	 |
	 |  Proposito: Realiza a leitura de variaveis do tipo inteiro com controle
	 |             para eventuais erros de insercao do utilizador. 
	 |     A leitura do dado inserido pelo utilizador e' adicionado a uma string 
	 |  auxiliar e esta e' adicionada a uma stringstream. Da stringstream a 
	 |  informacao e' escrita em um inteiro se for possivel tal insercao. Caso a 
	 |  insercao tenha sido possivel e' verificado se a informacao digitada pelo 
	 |  utilizador corresponde ao que e' permitido pelo programa.
	 |       Ex: Caso seja solicitado um inteiro dentro de um determinado intervalo
	 |	         (1 a 5 por exemplo) e' feita a verificacao se o inteiro digitado se 
	 |           encontra dentro desse intervalo.
	 |
	 |  Parametros:
	 |     message (OUT) - Variavel que guarda a mensagem que sera escrita para o utilizador
	 |                     solicitando a insercao de dados.
	 |
	 |	   min (IN) - Variavel que delimita o inicio do intervalo que o inteiro inserido e'
	 |	              suposto estar.
	 |
	 |	   max (IN) - Variavel que delimita o final do intervalo que o inteiro inserido e'
	 |	              suposto estar.
	 |
	 |  Retorno: Int. inteiro valido digitado pelo utilizador.
	 *--------------------------------------------------------------------------------------*/
int readInt(string message, int min, int max){
	stringstream readInt;
	string auxReadInt;
	int num = 0;

	cout << message;
	while(true){
		fflush(stdin);
		num = 0;
		getline(cin,auxReadInt);
		readInt.str(auxReadInt);
		readInt >> num;
		if(num == 0){
			cout << "\n<<O numero digitado apresenta um formato invalido>>" <<endl;
			readInt.clear();
			auxReadInt.clear();
		}
		else 
			if(num < min || num > max){
				cout << "\n<<Numero deve estar entre " << min << " e " << max << ">>" << endl;
				readInt.clear();
				auxReadInt.clear();
			}
			else
				return num;

		cout << "\nIntroduza novamente: ";
	}
}

	 /*------------------------------------------ readQuit ------------------------------------
     |  Funcao readQuit
	 |
	 |  Proposito: Realiza a leitura de variaveis do tipo inteiro e do caracter 'q' com controle
	 |             para eventuais erros de insercao do utilizador. 
	 |     A leitura do dado inserido pelo utilizador e' adicionado a uma string 
	 |  auxiliar e esta e' adicionada a uma stringstream. Caso seja o caracter 'q' ou 'Q'
	 |  a funcao retorna o valor -1 indicando que o jogador deseja sair da situacao atual.
	 |  Caso contrario, verifica se fora um numero digitado e se este esta no intercalo permitido.
	 |  Pede ao utilizador digitar valores validos ate que este o faca.
	 |       Ex: Caso seja solicitado um inteiro dentro de um determinado intervalo
	 |	         (1 a 5 por exemplo) e' feita a verificacao se o inteiro digitado se encontra 
	 |	         desse intervalo, ou se o jogador digitou o caracter 'q' ou 'Q'
	 |
	 |  Parametros:
	 |    message (OUT) - Variavel que guarda a mensagem que sera escrita para o utilizador
	 |                    solicitando a insercao de dados.
	 |
	 |	  min (IN) - Variavel que delimita o inicio do intervalo que o inteiro inserido e'
	 |	             suposto estar.
	 |
	 |	  max (IN) - Variavel que delimita o final do intervalo que o inteiro inserido e'
	 |	             suposto estar.
	 |
	 |  Retorno: Int. Inteiro valido digitado pelo utilizador ou -1 (para o caracter 'q' ou 'Q')
	 *--------------------------------------------------------------------------------------*/
int readQuit(string message, int min, int max){
	istringstream opt;
	string line;
	int num;
	char q;

	cout << message;
	cin.clear();
	fflush(stdin);
	while(true){
		getline(cin, line);
		istringstream opt(line);
		opt.get(q);
		if(q == 'q' || q == 'Q')
			return -1;
		num = atoi(line.c_str());
		if(num < min || num > max)
			cout << "\n\n<<Numero deve estar entre " << min << " e " << max << ">>" << " ou [Q]uit" <<endl;
		else
			return num;
		cout << "\nIntroduza novamente: ";
	}
}

	/*------------------------------------------------ showBoard ----------------------
     |  Funcao showBoard
	 |
	 |  Proposito: Mostra no ecra o tabuleiro passado como parametro
	 |    Ex: para um 3x3:      1 3 4
	 |                          2   8
	 |						    7 6 5
	 |  Parametros:
	 |	board      (IN) - tabuleiro que se deseja mostrar
	 |
	 |  Retorno:  Nada.  (Esta funcao e' do tipo void)
	 *---------------------------------------------------------------------------------*/
void showBoard(const vector<vector<int> > &board){
	cout << endl;
	for(size_t i=0 ; i<board.size() ; i++){
		for(size_t j=0 ; j<board[i].size() ; j++){
			if(!board[i][j])
				cout << " " << "\t";
			else
				cout << board[i][j] << "\t";
		}
		cout << endl;
	}
}

	/*------------------------------------------------ showTime -------------------------
     |  Funcao showTime
	 |
	 |  Proposito: Mostra no ecra o tempo em minutos e segundos em que o jogador demorou
	 |             para resolver o puzzle.
	 |     Ex:  Tempo de Jogo: 0 minutos e 15 segundos
	 |
	 |  Parametros:
	 |		time1 (IN) - tempo inicial do jogo, i.e. logo antes de entrar na resolucao
	 |                   do puzzle.
	 |		Ex:   0 segundos
	 |
	 |		time2 (IN) - tempo final do jogo, i.e. logo depois de encerrar a resolucao
	 |                   do puzzle.
	 |		Ex:   15 segundos
	 |
	 |  Retorno:  Nada.  (Esta funcao e' do tipo void)
	 *---------------------------------------------------------------------------------*/
void showTime(time_t time1, time_t time2){
	time_t second = time2 - time1;
	int min=0;
	while(second>=60){
		min++;
		second = second-60;
	}
	cout << "Tempo de Jogo: " << min << " minutos e " << second << " segundos\n"<< endl;
}

	/*------------------------------------------------ trivialBoard ----------------------
     |  Funcao trivialBoard
	 |
	 |  Proposito: Verifica se o tabuleiro passado como parametro e' um tabuleiro ordenado
	 |  Ex: para um 3x3:      1 2 3  este tabuleiro esta ordenado!
	 |                        4 5 6
	 |						  7 8 0
	 |
	 |  Parametros:
	 |	   board (IN) - tabuleiro que se deseja verificar
	 |  Ex: para um 3x3:      1 3 4
	 |                        2 0 8
	 |						  7 6 5
	 |
	 |  Retorno:  bool.   TRUE se e' o tabuleiro ordenado, FALSE caso contrario
	 *---------------------------------------------------------------------------------*/
bool trivialBoard(const vector<vector<int> > &board){
	int numTile = 1;
	for(size_t i=0 ; i<board.size() ; i++){
		for(size_t j=0 ; j<board[i].size() ; j++){
			if((board[i][j] != numTile) && (numTile < board.size()*board.size()))
				return false;
			numTile++;
		}
	}
	return true;
}

	/*------------------------------------------------ swapTile ------------------------
     |  Funcao swapTile
	 |
	 |  Proposito: Efetua a troca da peca vazia pela a peca indicada pelo utilizador
	 |             (caso seja possivel). Para isto armazena a posicao da celula vazia e 
	 |             verifica se esta e' adjacente a peca pedida para trocar.
	 |
	 |  Parametros:
	 |	   board (OUT) - tabuleiro que se deseja efetuar a troca de pecas
	 |  Ex: para um 3x3:      1 3 4
	 |                        2 0 8
	 |						  7 6 5
	 |
	 |     tile (IN) - peca indicada pela utilizador que se deseja mover
	 |  Ex: mover a peca 2 e' uma operacao possivel, mover a peca 1 nao!
	 |
	 |  Retorno:  bool   TRUE se foi possivel mover a peca pedida, FALSE caso contrario
	 *---------------------------------------------------------------------------------*/
bool swapTile(vector<vector<int> > &board, int tile){
	int white_x, white_y;
	int aux;
	bool swap = false;

	for(size_t i=0 ; i<board.size() ; i++)
		for(size_t j=0 ; j<board[i].size() ; j++)
			if(board[i][j] == 0){
				white_x = i;
				white_y = j;
			}
			if(white_x + 1 < board.size() && board[white_x+1][white_y]==tile){
				aux = board[white_x][white_y];
				board[white_x][white_y] = board[white_x+1][white_y];
				board[white_x+1][white_y] = aux;
				swap = true;
			}
			if(white_x - 1 >= 0 && board[white_x-1][white_y]==tile){
				aux = board[white_x][white_y];
				board[white_x][white_y] = board[white_x-1][white_y];
				board[white_x-1][white_y] = aux;
				swap = true;
			}
			if(white_y + 1 < board.size() && board[white_x][white_y+1]==tile){
				aux = board[white_x][white_y];
				board[white_x][white_y] = board[white_x][white_y+1];
				board[white_x][white_y+1] = aux;
				swap = true;
			}
			if(white_y - 1 >= 0 && board[white_x][white_y-1]==tile){
				aux = board[white_x][white_y];
				board[white_x][white_y] = board[white_x][white_y-1];
				board[white_x][white_y-1] = aux;
				swap = true;
			}
			if(swap)
				return true;
			return false;
}

	/*------------------------------------------------ generateBoard ----------------------
     |  Funcao generateBoard
	 |
	 |  Proposito: Gera o tabuleiro inicial ordenado
	 |  Ex: para um 3x3:      1 2 3   (este e' um exemplo de tabuleiro ordenado)
	 |                        4 5 6
	 |						  7 8 0
	 |
	 |  Parametros:
	 |	   board (OUT) - Endereco do tabuleiro, pre-declarado
	 |
	 |	   sizeBoard (IN) - Variavel responsavel pela identificacao do tamanho do 
	 |                      tabuleiro.
	 |	   Ex :  Valores inteiros entre BOARD_SIZE_MIN e BOARD_SIZE_MAX, neste caso [3,5]
	 |
	 |  Retorno:  Nada.  (Esta funcao e' do tipo void)
	 *---------------------------------------------------------------------------------*/
void generateBoard(vector<vector<int> > &board, int sizeBoard){
	int numTile = 1;
	for(size_t i=0 ; i<sizeBoard ; i++){
		for(size_t j=0 ; j<sizeBoard ; j++){
			board[i].push_back(numTile);
			numTile++;
		}
	}
	board[sizeBoard-1][sizeBoard-1] = 0;
}

	/*------------------------------------------------ shuffleBoard ----------------------
     |  Funcao shuffleBoard
	 |
	 |  Proposito: Embaralha o tabuleiro com verificacao de nao entregar o tabuleiro terminado.
	 |     Efetua troca aleatorias de pecas por posicoes validas SHUFFLETIMES vezes.
	 |  Ex: para um 3x3:       1 2 3 com SHUFFLETIMES = 1
	 |                         4 5 0
	 |						   7 8 6
	 |
	 |  Parametros:
	 |	  board  (OUT) - endereco do tabuleiro
	 |
	 |  Retorno:  Nada.  (Esta funcao e' do tipo void)
	 *---------------------------------------------------------------------------------*/
void shuffleBoard(vector<vector<int> > &board){
	int i = 0, tile;
	while(i<SHUFFLETIMES || trivialBoard(board)){
		do{
			tile = 1 + rand() % (board.size()*board.size());
		}while(!swapTile(board,tile));
		i++;
	}
}

	 /*------------------------------------------------ playPuzzle ----------------------
     |  Funcao playPuzzle
	 |
	 |  Proposito: Mostra o tabuleiro ao utilizador e pede para que este escolha uma peca
	 |             para mover ate que que o puzzle seja resolvido, ou que resolva sair.
	 |
	 |  Parametros:
	 |	    board (OUT) - endereco do tabuleiro
	 |
	 |  Retorno:  Int.	retorna a quantidade de movimentos validos utilizadas para resolver
	 |					o puzzle
	 *---------------------------------------------------------------------------------*/
int playPuzzle(vector<vector<int> > &board){
	int tile, movement=0;
	while(!trivialBoard(board)){
		showBoard(board);
		tile = readQuit("\nQual peca deseja mover? ou [Q]uit: ", 1, board.size()*board.size()-1);
		if(tile==-1)
			return -1;
		if(!swapTile(board,tile))
			cout << "\n<<Movimento Invalido...>>\n";
		else
			movement++;
	}
	showBoard(board);
	cout << "\n\n<<Fim de Jogo>>\n\n";
	return movement;
}

	 /*------------------------------------------ registerPuzzleNumbers ------------------
     |  Funcao registerPuzzleNumbers
	 |
	 |  Proposito: Geracao ou atualizacao do arquivo registerPuzzleNumbers.txt o qual e' 
	 |             responsavel pelo controle do numero de tabuleiros existentes para cada 
	 |             tamanho de tabuleiro possivel.
	 |		Inicialmente e' verificado se o arquivo registerPuzzleNumbers.txt existe,
	 |  caso nao exista, cria-se um arquivo texto novo e logo em seguida escreve-se neste.
	 |		Caso o arquivo exista, o conteudo deste e' passado para uma string.
	 |	Na string e' executada uma busca para achar a linha onde sera atualizada 
	 |  a informacao de quantos tabuleiros existem associado ao seu respectivo tamanho.
	 |      Se a linha ja existe e' utilizado um replace na string para atualizar a 
	 |  informacao. Caso nao exista, e' adicionado a string uma nova linha com
	 |  o novo tamanho de tabuleiro que foi gerado.
	 |      A string atualizada e' entao escrita no arquivo registerPuzzleNumbers.txt.
	 |		  
	 |  Parametros:
	 |	   sizeBoard (IN) - Variavel responsavel pela identificacao do tamanho do 
	 |                      tabuleiro.
	 |	   Ex:  Valores inteiros entre BOARD_SIZE_MIN e BOARD_SIZE_MAX.
	 |
	 |	   boardNumber (IN) - Variavel responsavel por informar o numero de tabuleiros 
	 |                        existentes do tamanho informado por sizeBoard.
	 |	
	 |  Retorno:  Nada.  (Esta e' uma funcao do tipo void)
	 *-----------------------------------------------------------------------------------*/
void registerPuzzleNumbers(int sizeBoard, int boardNumber){
	ifstream fin; 
	ofstream fout;
	stringstream aux, boardSize, BoardNumberRepresentation;
	string s,s2;

	boardSize << sizeBoard << "x" << sizeBoard;
	BoardNumberRepresentation << boardNumber << "\n";

	fin.open("puzzle_numbers.txt");
	if(fin.fail()){
		fout.open("puzzle_numbers.txt");
		fout << boardSize.str() << '\t' << boardNumber;
	}
	else{
		while(getline(fin, s))
			aux << s << "\n";

		s2 = aux.str();
		int boardSizeFind = aux.str().find(boardSize.str());
		fout.open("puzzle_numbers.txt");
		if(boardSizeFind == -1){
			aux << boardSize.str() << '\t' << boardNumber;
			s2 = aux.str();
		}
		else
			s2.replace(boardSizeFind+4, 2, BoardNumberRepresentation.str());
		fout << s2;
	}
	fin.close();
	fout.close();
}

	 /*------------------------------------------------ registerBoard -------------------
     |  Funcao registerBoard
	 |
	 |  Proposito:  Cria um novo ficheiro no formato apropriado com o tabuleiro incial
	 |              proposto ao jogador resolver e adiciona suas informacoes. Uma vez que este 
	 |              tabuleiro foi um novo gerado pelo programa, as informacoes deste jogador 
	 |              sao unicas neste instante (unico a resolver este novo puzzle ate' entao).
	 |
	 |  Parametros:
	 |	   board (OUT) - tabuleiro inicial a ser salvo no ficheiro
	 |
	 |	   size  (IN) - indica o tamanhado do tabuleiro.
	 |	   Ex:   valores dentro do intervalo [BOARD_SIZE_MIN,BOARD_SIZE_MAX]
	 |
	 |	   name (OUT) - Variavel passada por referencia que guarda a informacao 
	 |                  a respeito do nome do jogador.
	 |	   Ex :  Joao.
	 |
	 |     age (OUT) - Variavel passada por referencia que guarda a informacao 
	 |                 a respeito da idade do jogador.
	 |     Ex: 15.
	 |
	 |     sex (OUT) - Variavel passada por referencia que guarda a informacao 
	 |                 a respeito do sexo do jogador.
	 |     Ex: m ou f.
	 |
	 |	   time (OUT) - Tempo total do jogo, i.e. tempo gasto na resolucao do puzzle.
	 |	   Ex:   4 (em segundos)
	 |
	 |  Retorno:  Nada.  (ESta e' uma funcao do tipo void.)
	 *-------------------------------------------------------------------------*/
void registerBoard(const vector<vector<int> > &board, int size, string name, int age, char sex, int time){
	ofstream fout;
	ifstream fin;
	int boardNumber=1;
	stringstream filename, registerPlayer;

	filename << "puzzle_" << size << "x" << size << "_" << boardNumber << ".txt"; //puzzle_NxN_SSS
	fin.open(filename.str());
	while(!fin.fail()){
		boardNumber++;
		fin.close();
		filename.str("");
		filename << "puzzle_" << size << "x" << size << "_" << boardNumber << ".txt";
		fin.open(filename.str());
	}

	fout.open(filename.str());
	if (fout.fail( )){
		cerr << "Falha na abertura do arquivo!\n";
		exit(1);
	}

	for(size_t i=0 ; i<board.size() ; i++){
		for(size_t j=0 ; j<board[i].size() ; j++){
			fout << board[i][j] << "\t";
		}
		fout << endl;
	}
	registerPlayer << name << " " << age << " " << sex << " " << " " << time;
	fout << "\n" << registerPlayer.str() << endl;
	registerPuzzleNumbers(size, boardNumber);
	fout.close();
}

	 /*------------------------------------------------ registerPlayer -----------------------
     |  Funcao registerPlayer
	 |
	 |  Proposito:  Cadastra o jogador coletando as informacoes passadas pelo utilizador e as 
	 |              armazena nas variaveis passadas por referencia.
	 |
	 |  Parametros:
	 |	   name (OUT) - Variavel passada por referencia que guarda a informacao 
	 |                  a respeito do nome do jogador.
	 |	   Ex:  Joao.
	 |
	 |     age (OUT) - Variavel passada por referencia que guarda a informacao 
	 |                 a respeito da idade do jogador.
	 |     Ex: 15.
	 |
	 |     sex (OUT) - Variavel passada por referencia que guarda a informacao 
	 |                  a respeito do sexo do jogador.
	 |     Ex: m ou f.
	 |
	 |  Retorno:  Nada.  (Esta e' uma funcao do tipo void.)
	 *-----------------------------------------------------------------------------------------*/
void registerPlayer(string &name, int &age, char &sex){
	cout << "*Cadastro de Jogador!*\n" << endl;
	cout << "Digite seu nome: ";
	getline(cin,name);
	age = readInt("Digite sua idade: ", 0, 150);
	sex = readChar("Digite seu sexo: ","mf");
}

	/*------------------------------------------------ ascending ----------------------
     |  Funcao ascending
	 |
	 |  Proposito: funcao auxiliar utilizada para ordenar vetor de forma crescente
	 |
	 |  Parametros:
	 |	   i        (IN) - inteito que se deseja considerar na comparacao
	 |		         Ex:  3
	 |	   j        (IN) - inteito que se deseja considerar na comparacao
	 |		         Ex:  4
	 |
	 |  Retorno:  bool.   (TRUE se i<j, FALSE caso contrario).
	 *---------------------------------------------------------------------------------*/
bool ascending(int i,int j){
	return (i<j);
}

	/*------------------------------------------------ updateBoard --------------------------
     |  Funcao updateBoard
	 |
	 |  Proposito: Acessa o ficheiro selecionado e atualiza as informacoes deste, com o tabuleiro
	 |	           inicial gerado e os dados dos PLAYERS_SCORE_MAX jogadores com melhores tempos.
	 |  Para tal, o ficheiro com o tabuleiro selecionado e' acessado e com um caracter especial,
	 |  os dados sao delimitados de forma a que os tempos possam ser acessados pela posicao anterior
	 |  a este caracter especial. Os tempos sao extraidos e armazenados. Os tempos sao ordenados 
	 |  crescentemente pela funcao ascending(). As informacoes sao sobrescritas com apenas os 
	 |  jogadores de melhores tempos.
	 |
	 |  Parametros:
	 |	   board (OUT) - Tabuleiro que se deseja mostrar
	 |
	 |	   boardSelect (IN) - Indica o tabuleiro selecionado.
	 |	   Ex:   Valores de tabuleiros existentes em ficheiros na memoria.
	 |
	 |	   size (IN) - Indica o tamanhado do tabuleiro.
	 |	   Ex:   Valores dentro do intervalo [BOARD_SIZE_MIN,BOARD_SIZE_MAX]
	 |
	 |	   name (OUT) - Variavel passada por referencia que guarda a informacao 
	 |                   a respeito do nome do jogador.
	 |	   Ex:  Joao.
	 |
	 |     age (OUT) - Variavel passada por referencia que guarda a informacao 
	 |                 a respeito da idade do jogador.
	 |     Ex: 15.
	 |
	 |     sex (OUT) - Variavel passada por referencia que guarda a informacao 
	 |                 a respeito do sexo do jogador.
	 |     Ex: m ou f.
	 |
	 |	   time (IN) - Tempo total do jogo, i.e. tempos gasto na resolucao do puzzle.
	 |				 Ex:   4 segundos
	 |
	 |  Retorno:  Nada.  (Esta e' uma funcao do tipo void.)
	 *--------------------------------------------------------------------------------------*/
void updateBoard(vector<vector<int> > &board, int boardSelect, int size, string name, int age, char sex, int time){
	string s,s2;
	ofstream fout;
	ifstream fin;
	stringstream filename, registerPlayer, aux;
	int i=0, j, k=0, registers=-1, pos=0, posI;
	vector<int> seconds, scoreTimes;
	vector<string> scores;

	filename << "puzzle_" << size << "x" << size << "_" << boardSelect << ".txt"; //puzzle_NxN_SSS
	fin.open(filename.str());
	while(getline(fin, s)){
		if(i>size){
			aux << s << "@#$ ";
			scores.push_back(s);
		}
		i++;
	}
	fin.close();
	s2 = aux.str();

	while(pos!=-1){
		pos = s2.find("@#$",k);
		k = pos+1;
		registers++;
	}
	k=0;
	pos=0;
	s.clear();
	for(int i=0,j=1 ; i<registers ; i++){
		pos = s2.find("@#$",k);
		j=1;
		k = pos+1;
		posI=pos-1;
		while(aux.str().substr(posI,1).compare(" ")){
			posI--;
			j++;
		}
		s = aux.str().substr(posI, j);
		scoreTimes.push_back(stoi(s,nullptr,10));
		seconds.push_back(stoi(s,nullptr,10));
	}
	scoreTimes.push_back(time);
	seconds.push_back(time);
	registerPlayer << name << " " << age << " " << sex << " " << " " << time;
	scores.push_back(registerPlayer.str());
	std::sort (seconds.begin(), seconds.end(),ascending);

	fout.open(filename.str());
	if (fout.fail( )){
		cerr << "Falha na abertura do arquivo!\n";
		exit(1);
	}
	for(size_t i=0 ; i<board.size() ; i++){
		for(size_t j=0 ; j<board[i].size() ; j++){
			fout << board[i][j] << "\t";
		}
		fout << endl;
	}

	fout << "\n";
	for(int i=0 ; i<scoreTimes.size() && i<PLAYERS_SCORE_MAX ; i++){
		j=0;
		while(seconds[i] != scoreTimes[j]){
			j++;
		}
		scoreTimes[j] = -1;
		fout << scores[j] << endl;
	}
	fout.close();
}

	/*------------------------------------------------ boardsInMemory ----------------------
     |  Funcao boardsInMemory
	 |
	 |  Proposito: Tentar acessar o ficheiro "puzzle_numbers.txt" e procurar tabuleiros
	 |	           existentes de tamanho sizeBoard e acessar a informacao referente a 
	 |	           quantidade de puzzles armazenados na memoria deste tipo.
	 |
	 |  Parametros:
	 |	   sizeBoard  (IN) - indica o tamanhado do tabuleiro.
	 |	   Ex:   Valores dentro do intervalo [BOARD_SIZE_MIN,BOARD_SIZE_MAX]
	 |
	 |     Retorno:  Int.   (Retorna a quantidade de tabuleiros armazenados na memoria de 
	 |					     tamanho sizeBoard).
	 *------------------------------------------------------------------------------------*/
int boardsInMemory(int sizeBoard){
	stringstream boardSizeStyle, aux;
	int posSizeFind, boardNumber=0;
	ifstream fin; 
	string s,s2;

	boardSizeStyle << sizeBoard << "x" << sizeBoard;
	fin.open("puzzle_numbers.txt");
	if(fin.fail()){
		fin.close();
		return boardNumber;
	}
	while(getline(fin, s))
		aux << s << "\n";
	posSizeFind = aux.str().find(boardSizeStyle.str());
	if(posSizeFind == -1){
		fin.close();
		return boardNumber;
	}

	s2 = aux.str().substr(posSizeFind+4);
	boardNumber = stoi(s2,nullptr,10);
	fin.close();
	return boardNumber;
}

	 /*------------------------------------------------ showAllBoards -----------------------
     |  Funcao showAllBoards
	 |
	 |  Proposito: Mostra os tabuleiros guardados na memoria. Caminha pelos arquivos gerados 
	 |             pelo Competition Mode lendo-os e imprimindo-os no ecrã junto com um numero 
	 |             associado para identifica-lo.	
	 |
	 |  Parametros:
	 |	   sizeBoard (IN) - Variavel responsavel pela indentificacao do tamanho do tabuleiro.
	 |     Ex:  Valores inteiros entre BOARD_SIZE_MIN e BOARD_SIZE_MAX
	 |
	 |  Retorno:  Nada.  (Esta e' uma funcao do tipo void.)
	 *--------------------------------------------------------------------------------------*/
void showAllBoards(int sizeBoard){
	stringstream filename;
	int boardSizeMemory=0;
	ifstream fin; 
	string s;

	boardSizeMemory = boardsInMemory(sizeBoard);
	for (int boardNumber=1 ; boardNumber <= boardSizeMemory; boardNumber++){
		filename << "puzzle_" << sizeBoard << "x" << sizeBoard << "_" << boardNumber <<".txt";
		fin.open(filename.str());
		cout << "Tabuleiro: [" << boardNumber << "]\n" << endl;
		filename.str("");
		while(getline(fin, s))
			cout << s << endl;
		cout << s << endl;
		fin.close();
	}
}

	/*------------------------------------------------ selectBoard --------------------------------
     |  Funcao selectBoard
	 |
	 |  Proposito: Verifica se ha tabuleiros na memoria do tamanho selecionado.
	 |             Caso haja, pede ao utilizador para selecionar um tabuleiro valido (id existente).
	 |
	 |  Parametros:
	 |	sizeBoard   (IN) - indica o tamanhado do tabuleiro.
	 |		         Ex:   valores dentro do intervalo [BOARD_SIZE_MIN,BOARD_SIZE_MAX]
	 |
	 |  Retorno:  Int.   (Retorna necessariamente um tabuleiro valido e que seja 
	 |					  selecionado pelo jogador. Caso nao existam tabuleiro do tamanho
	 |					  sizeBoard, retorna 0).
	 *-------------------------------------------------------------------------------------------*/
int selectBoard(int sizeBoard){
	int select, boardSizeMemory=0;
	boardSizeMemory = boardsInMemory(sizeBoard);
	if(!boardSizeMemory)
		return 0;
	showAllBoards(sizeBoard);
	select = readQuit("\n\nQual tabuleiro deseja jogar? ou [Q]uit: ", 1, boardSizeMemory);
	return select;
}

	/*------------------------------------------------ loadBoard ----------------------
     |  Funcao loadBoard
	 |
	 |  Proposito: Carrega um tabuleiro armazenado na memoria, dado o tamanho do tabuleiro e 
	 |             sua id, i.e. o numero que o identifica nos arquivos textos armazenados.
	 |
	 |  Parametros:
	 |	   board     (OUT) - endereco para o tabuleiro criado na funcao que chama
	 |					     loadBoard.
	 |
	 |	   sizeBoard   (IN) - indica o tamanhado do tabuleiro.
	 |		         Ex:   valores dentro do intervalo [BOARD_SIZE_MIN,BOARD_SIZE_MAX]
	 |
	 |	   boardSelect (IN) - Indica o id do tabuleiro selecionado em showAllBoards();
	 |		         Ex:   inteiros de [1, max de tabuleiro salvos de tamanho sizeBoard]
	 |
	 |  Retorno:  Nada.  (Esta funcao e' do tipo void)
	 *---------------------------------------------------------------------------------*/

void loadBoard(vector<vector<int> > &board, int sizeBoard, int boardSelect){
	stringstream filename, aux;
	ifstream fin; 
	string s, s2;
	int k=0, len;

	filename << "puzzle_" << sizeBoard << "x" << sizeBoard << "_" << boardSelect << ".txt";
	fin.open(filename.str());
	while(getline(fin, s))
		aux << s;
	for(int i=0 ; i<sizeBoard ; i++){
		for(int j=0 ; j<sizeBoard ; j++){
			if(aux.str()[k+1] == '\t')
				len = 1;
			else
				len = 2;
			s2 = aux.str().substr(k,len);
			board[i].push_back(stoi(s2,nullptr,10));
			k += len + 1;
			s2.clear();
		}
	}
	fin.close();
}

	/*------------------------------------------------ competitionMode ----------------------
     |  Funcao competitionMode
	 |
	 |  Proposito: Se encarrega de fazer as principais chamadas de funcoes no modo competicao,
	 |             alem de fazer interface com o jogador no ambito de questoes no escopo de 
	 |             competicao
	 |
	 |  Retorno:  Nada.  (Esta funcao e' do tipo void)
	 *---------------------------------------------------------------------------------*/
void competitionMode(){ 
	time_t time1, time2;
	int sizeBoard, movement, age, boardSelect;
	char sex, chooseBoard, playAgain = 's', playerRegistration = 's';
	vector<vector<int> > board, initialBoard;
	string name;

	cout << "\n*Entrando no COMPETITION MODE!*\n" << endl;
	while(playAgain == 's'){
		chooseBoard = readChar("\nDeseja um novo tabuleiro ou um jogado anteriormente?\n [N]ew ou [O]ld: ","no");
		switch(chooseBoard){
		case 'n':
			sizeBoard  = readInt("\nQual tamanho do tabuleiro? [3], [4] ou [5]: ", BOARD_SIZE_MIN, BOARD_SIZE_MAX);
			board.resize(sizeBoard);
			generateBoard(board,sizeBoard);
			shuffleBoard(board);
			initialBoard = board;
			time1 = time(NULL);
			movement = playPuzzle(board);
			time2 = time(NULL);
			if(movement!=-1){
				cout << movement << " movimentos\n";
				showTime(time1,time2);
				registerPlayer(name,age,sex);
				registerBoard(initialBoard, sizeBoard, name, age, sex, time2 - time1);
			}
			break;
		case 'o':
			sizeBoard  = readInt("\nQual tamanho do tabuleiro? [3], [4] ou [5]: ", BOARD_SIZE_MIN, BOARD_SIZE_MAX);
			board.resize(sizeBoard);
			boardSelect = selectBoard(sizeBoard);
			if(boardSelect>0){
				loadBoard(board, sizeBoard, boardSelect);
				initialBoard = board;
				time1 = time(NULL);
				movement = playPuzzle(board);
				time2 = time(NULL);
				if(movement!=-1){
					cout << movement << " movimentos\n";
					showTime(time1,time2);
					registerPlayer(name,age,sex);
					updateBoard(initialBoard, boardSelect, sizeBoard, name, age, sex, time2 - time1);
				}
			}
			else{
				if(!boardSelect)
					cout << "\nNao ha tabuleiros deste tamanho na memoria!!\n";
			}
			break;
		default:
			return;
			break;
		}
		playAgain = readChar("\nDeseja jogar COMPETITION MODE novamente? [S]im ou [N]ao: ","sn");
		if(playAgain == 'n'){
			cout << "\n*Saindo do COMPETITION MODE!*\n" << endl;
			return;
		}
		else{
			board.clear();
			initialBoard.clear();
		}
	}
}

	/*------------------------------------------------ trainingMode ----------------------
     |  Funcao trainingMode
	 |
	 |  Proposito: Se encarrega de fazer as principais chamadas de funcoes no modo treino,
	 |             alem de fazer interface com o jogador no ambito de questoes no escopo de 
	 |             treino
	 |
	 |  Retorno:  Nada.  (Esta funcao e' do tipo void)
	 *---------------------------------------------------------------------------------*/
void trainingMode(){
	time_t time1, time2;
	int sizeBoard, movement, boardSelect;
	char chooseBoard, wantResolve, playAgain = 's';
	vector<vector<int> > board;

	cout << "\n*Entrando no TRAINING MODE!*\n" << endl;
	while(playAgain == 's'){
		chooseBoard = readChar("\nDeseja um novo tabuleiro ou um jogado anteriormente?\n [N]ew ou [O]ld: ","no");
		switch(chooseBoard){
		case 'n':
			sizeBoard  = readInt("\nQual tamanho do tabuleiro? [3], [4] ou [5]: ", BOARD_SIZE_MIN, BOARD_SIZE_MAX);
			board.resize(sizeBoard);
			generateBoard(board,sizeBoard);
			do{
				shuffleBoard(board);
				showBoard(board);
				wantResolve = readChar("\nDeseja resolver este puzzle? [S]im ou [N]ao: ","sn");
			}while(wantResolve == 'n');
			time1 = time(NULL);
			movement = playPuzzle(board);
			time2 = time(NULL);
			if(movement!=-1){
				cout << movement << " movimentos\n";
				showTime(time1,time2);
			}
			break;
		case 'o':
			sizeBoard  = readInt("\nQual tamanho do tabuleiro? [3], [4] ou [5]: ", BOARD_SIZE_MIN, BOARD_SIZE_MAX);
			board.resize(sizeBoard);
			boardSelect = selectBoard(sizeBoard);
			if(boardSelect>0){
				loadBoard(board, sizeBoard, boardSelect);
				time1 = time(NULL);
				movement = playPuzzle(board);
				time2 = time(NULL);
				if(movement!=-1){
					cout << movement << " movimentos\n";
					showTime(time1,time2);
				}
			}
			else{
				if(!boardSelect)
					cout << "\nNao ha tabuleiros deste tamanho na memoria!!\n";
			}
			break;
		default:
			return;
			break;
		}
		playAgain = readChar("\nDeseja jogar TRAINING MODE novamente? [S]im ou [N]ao: ","sn");
		if(playAgain == 'n'){
			cout << "\n*Saindo do TRAINING MODE!*\n" << endl;
			return;
		}
		else
			board.clear();
	}
}

	/*------------------------------------------------ main ----------------------
     |  Funcao main
	 |
	 |  Proposito: Funcao de arranque do programa. Faz interface com o jogador no ambito
	 |             de qustiona-lo em qual modo de jogo deseja jogar e se deseja jogar de novo.
	 |
	 |  Retorno:  Int.  retorna 0, caso o programa chega ao fim sem nenhum problema.
	 *---------------------------------------------------------------------------------*/
int main(){
	bool onGame = true;
	char chooseMode, playAgain;
	srand((unsigned int) time(NULL));
	while(onGame){
		chooseMode = readChar("Qual modo deseja jogar?\n [T]raing ou [C]ompetition: ","tc");
		switch(chooseMode){
		case 't':
			trainingMode();
			break;
		case 'c':
			competitionMode();
			break;
		}
		playAgain = readChar("\nDeseja jogar SLIDING PUZZLE novamente? [S]im ou [N]ao: ","sn");
		if(playAgain == 'n')
			onGame = false;
		cout << endl;
	}
	cout << "\n\nSaindo do jogo!\n\n" << endl;
	return 0;
}
