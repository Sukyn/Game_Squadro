
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;
typedef vector<string> vstring;
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <stdio.h> 
#include <assert.h> 

sf::RenderWindow window(sf::VideoMode(1200, 900), "My beautiful Squadro", sf::Style::Default);
sf::Font font;
sf::Time delay = sf::seconds((float)0.3);
float margin = 20.;
sf::Event event;


/** Fonction draw_validate_shape (fait par Colin)
*	Dessine le rectangle du boutton "Valider"	
*
*	@return le rectangle du bouton "valider"
*	
**/
sf::RectangleShape draw_validate_shape() {
	//Dessine le bouton "Valider"
	sf::RectangleShape validate_shape(sf::Vector2f(240, 50));
	validate_shape.setFillColor(sf::Color(0, 0, 0));
	validate_shape.setOutlineThickness(3);
	validate_shape.setOutlineColor(sf::Color(255, 165, 0));
	validate_shape.setPosition(450, 400);
	return validate_shape;
}

/** Fonction draw_validate_shape (fait par Colin)
*	Dessine le texte du boutton "Valider"
*
*	@return le texte du bouton "valider"
*
**/
sf::Text draw_validate_text() {
	sf::Text validate_text;
	validate_text.setFont(font);
	validate_text.setString("Valider");
	validate_text.setCharacterSize(40);
	validate_text.setFillColor(sf::Color::White);
	validate_text.setStyle(sf::Text::Bold);
	validate_text.setPosition(500, 400);
	return validate_text;
}

/** Fonction draw_validate_shape (fait par Colin)
*	Dessine le carré indiquant à quel joueur c'est le tour"
*
*	@return un carré rouge ou jaune en fonction de la personne à qui c'est le tour
*
**/
sf::RectangleShape draw_player_shape(int turn) {
	sf::RectangleShape player_shape(sf::Vector2f(50, 50));
	
	player_shape.setOutlineThickness(3);
	player_shape.setOutlineColor(sf::Color(255, 165, 0));
	player_shape.setPosition(1025, 300);

	if (turn == 0) {
		player_shape.setFillColor(sf::Color(255, 0, 0));
	}
	else {
		player_shape.setFillColor(sf::Color(255, 255, 0));
	}
	return player_shape;
}

/** Fonction draw_player_text (Créé par Colin)
*	Ecrit à quel joueur c'est à lui de jouer
*
*	@return "Au joueur rouge" ou "Au joueur jaune" 
**/
sf::Text draw_player_text(int turn) {
	sf::Text validate_text;
	validate_text.setFont(font);
	if (turn == 0) {
		validate_text.setString("Au joueur rouge");
	}
	else {
		validate_text.setString("Au joueur jaune");
	}
	
	validate_text.setCharacterSize(40);
	validate_text.setFillColor(sf::Color::Black);
	validate_text.setStyle(sf::Text::Bold);
	validate_text.setPosition(890, 400);
	return validate_text;
}

/**Fonction rotate_cells (créé par Colin, modifié par Axel et Colin)
*	@param cells,     un tableau 2D de string
*	@param clockwise, un bool permettant de choisir le sens de la rotation
*
*	@return un tableau 2D de string contenant le contenu de cells après rotation de 90°
**/
vector<vstring> rotate_cells(vector<vstring> cells,
	bool clockwise = false) {
	int grid_size = static_cast<int>(cells[0].size());
	//0° : cells[r][c] -- > cells[r][c]
	//90° : cells[r][c] -- > cells[H - 1 - c][r]
	//180° : cells[r][c] -- > cells[W - 1 - r][H - 1 - c]
	//270° : cells[r][c] -- > cells[c][W - 1 - r]
	vstring pawns = { "<","^",">","v","<" };
	vector<vstring> moved = {};
	for (int r = 0; r < grid_size; r++) {
		//add empty row
		moved.push_back({});
		for (int c = 0; c < grid_size; c++) {
			if (not clockwise) {
				//anti-clockwise rotate cell
				moved[r].push_back(cells[c][size_t(grid_size) - 1 - size_t(r)]);
				//anti-clockwise rotate pawn if needed
				for (int s = 1; s < 5; s++) {
					if (moved[r][c] == pawns[s]) {
						moved[r][c] = pawns[size_t(s) - 1];
						break;
					}
				}
			}
			else {
				//Clockwise rotate cell
				moved[r].push_back(cells[size_t(grid_size) - size_t(c) - 1][r]);

				//clockwise rotate pawn if needed
				for (int s = 0; s < 4; s++) {
					if (moved[r][c] == pawns[s]) {
						moved[r][c] = pawns[size_t(s) + 1];
						break;
					}
				}
			}
		}
	}
	return moved;
}

/** ----------------------------------- OLD --------------------------------------------
 *Fonction draw_big_grid
 *  @param cells, un tableau 2D de string représentant l'état de la partie actuelle
 *	@param moves, un tableau 2D de string contenant le nombre de cases de déplacement autorisé sur chaque ligne
 *	@param row_label et colums_label, précisant quel équipe est de quel côté de la grille
 *
 *	dessine le plateau de jeu dans la console
 **/
 /*void draw_big_grid(vector<vstring> cells,
	 vector<vector<string>> moves,
	 string rows_label,
	 string columns_label) {
	 int grid_size = static_cast<int>(cells[0].size());
	 vstring sprite_pub = { "  =====  ", " squadro ", "  =====  " };
	 vstring sprite_left = { "    <<   ", "  <<<    ", "    <<   " };
	 vstring sprite_right = { "   >>    ", "    >>>  ", "   >>    " };
	 vstring sprite_top = { "    ^    ", "   ^^^   ", "  ^^ ^^  " };
	 vstring sprite_bottom = { "  vv vv  ", "   vvv   ", "    v    " };

	 //Draw rows
	 for (int r = 0; r < grid_size; r++) {
		 for (int inner = 0; inner < 6; inner++) {
			 //Draw rows labels R1..R5
			 if (inner != 3 || r == 0 || r == grid_size - 1) {
				 std::cout << "    ";
			 }
			 else {
				 if (r < 10) {
					 std::cout << " " << rows_label << r << " ";
				 }
				 else {
					 std::cout << " " << rows_label << r;
				 }
			 }
			 //Draw columns
			 for (int c = 0; c < grid_size; c++) {
				 //Cell top border
				 if (inner == 0) {
					 std::cout << "+---------";
				 }
				 //Cell top margin
				 else if (inner == 1) {
					 std::cout << "|         ";
				 }
				 //Cell bottom margin
				 else if (inner == 5) {
					 std::cout << "|       " << moves[r][c] << " ";
				 }
				 //Cell content
				 else {
					 std::cout << "|";
					 if (cells[r][c] == "x") {
						 std::cout << sprite_pub[size_t(inner) - 2];
					 }
					 else if (cells[r][c] == ".") {
						 std::cout << "         ";
					 }
					 else if (cells[r][c] == "<") {
						 std::cout << sprite_left[size_t(inner) - 2];
					 }
					 else if (cells[r][c] == ">") {
						 std::cout << sprite_right[size_t(inner) - 2];
					 }
					 else if (cells[r][c] == "^") {
						 std::cout << sprite_top[size_t(inner) - 2];
					 }
					 else if (cells[r][c] == "v") {
						 std::cout << sprite_bottom[size_t(inner) - 2];
					 }
				 }
			 }
			 //Draw columns right border
			 if (inner == 0) {
				 std::cout << "+" << std::endl;
			 }
			 else {
				 std::cout << "|" << std::endl;
			 }
		 }
	 }
	 //Draw row bottom border
		 std::cout << "    ";
		 for (int c = 0; c < grid_size; c++) {
			 std::cout << "+---------";
		 }
		 std::cout << "+" << std::endl;
		 //Draw row columns labels
		 std::cout << "    ";
		 for (int c = 0; c < grid_size; c++) {
			 if (c == 0 || c == grid_size - 1) {
				 //No label
				 std::cout << "          ";
			 }
			 else {
				 //Column label
				 if (c < 10) {
					 std::cout << "     " << columns_label << c << "   ";
				 } else {
					 std::cout << "     " << columns_label << c << "  ";
				 }
			 }
		 }
		 std::cout << " " << std::endl;
 }
 */



 /** Fonction rotate_grid (fait par Colin)
 *	@param cells,     un tableau 2D de string représentant l'état de la partie actuelle
 *	@param moves,     un tableau 2D de string contenant le nombre de cases de déplacement autorisé sur chaque ligne
 *	@param grid_size, un entier contenant la taille de la grille de jeu
 *	@param clockwise, un bool permettant de choisir le sens de la rotation
 *
 *	Prend les tableaux cells et moves et leur effectue une rotation de 90°
 **/
void rotate_grid(vector<vstring>* cells,
	vector<vstring>* moves,
	vstring* labels,
	int* grid_size,
	bool clockwise = false) {
	//Rotate game cells
	*cells = rotate_cells(*cells, clockwise);
	//Rotate allowed moves
	*moves = rotate_cells(*moves, clockwise);
	//Swap row/columns labels
	string temp = (*labels)[0];
	(*labels)[0] = (*labels)[1];
	(*labels)[1] = temp;
}

/** Fonction allow_dot (Créé par Colin, modifié par Axel et Colin)
*	@param coord_y, une ligne du tableau de jeu
*	@param moves,   un tableau 2D de string contenant le nombre de cases de déplacement autorisé sur chaque ligne
*	@param grid,    un tableau 2D de string représentant l'état actuel de la partie
*
*	@return le nombre de cases que le pion fera si on bouge ce pion (sans compter les pions sur le passage)
**/
int allow_dot(int coord_y,
	vector<vstring> moves,
	vector<vstring> grid) {
	int grid_size = static_cast<int>(moves[0].size());
	int coord_x = 0;
	int type = 0;
	if (coord_y < 1 || coord_y > grid_size - 1) {
		return 0;
	}

	//Pour savoir dans quel sens est le pion
	// ">" = 0
	// "<" = 1
	for (int r = 0; r < grid_size; r++) {
		if (grid[coord_y][r] == "<" || grid[coord_y][r] == ">") {
			coord_x = r;
			if (grid[coord_y][coord_x] == ">") {
				type = 0;
			}
			else {
				type = 1;
			}
		}
	}
	if (type == 0) {
		int m = atoi(moves[coord_y][0].c_str());
		if (coord_x + m < (grid_size - 1)) {
			return m;
		}
		else {
			return (grid_size - 1 - coord_x);
		}
	}
	else {
		int m = atoi(moves[coord_y][size_t(grid_size) - 1].c_str());
		if (coord_x - m > 0) {
			return m;
		}
		else {
			return coord_x;
		}
	}
}


/** Fonction is_game_over (Créé par Axel)
*	@param cells, un tableau 2D de string représentant l'état de la partie actuelle
*
*	@return true si un des 2 joueurs a gagné, false sinon
**/
bool is_game_over(vector <vstring> cells) {
	int grid_size = static_cast<int>(cells[0].size());
	int nDoneJ1 = 0;
	int nDoneJ2 = 0;
	for (int r = 1; r < grid_size - 1; r++) {
		if (cells[r][0] == "<") {
			nDoneJ1++;
		}
		if (cells[0][r] == "^") {
			nDoneJ2++;
		}
	}
	if (nDoneJ2 > (grid_size - 4) || nDoneJ1 > (grid_size - 4)) {
		return true;
	}
	else {
		return false;
	}
}



/** Foction move_dot (créé par Colin, recorrigé par Axel)
*	@param coord_y, une ligne du tableau de jeu
*	@param allowed, un entier
*	@param moves,   un tableau 2D de string contenant le nombre de cases de déplacement autorisé sur chaque ligne
*	@param grid,    un tableau 2D de string représentant l'état actuel de la partie
*
*	@return un tableau 2D contenant l'étar de la partie après déplacement du pion demandé
**/
vector<vstring> move_dot(int coord_y,
	int allowed,
	vector<vstring> moves,
	vector<vstring> grid) {
	int grid_size = static_cast<int>(moves[0].size());
	int coord_x = 0;
	int type = 0;

	if (coord_y < 1 || coord_y >(grid_size - 1)) {
		return grid;
	}

	//Pour savoir dans quel sens est le pion
	// ">" = 0
	// "<" = 1
	for (int r = 0; r < grid_size; r++) {
		if (grid[coord_y][r] == "<" || grid[coord_y][r] == ">") {
			coord_x = r;
			if (grid[coord_y][coord_x] == ">") {
				type = 0;
			}
			else {
				type = 1;
			}
		}
	}

	while (allowed > 0) {
		if (type == 0) {
			int next_coord_x = size_t(coord_x) + 1;

			if (grid[coord_y][next_coord_x] == "^" || grid[coord_y][next_coord_x] == "v") {

				if (grid[coord_y][next_coord_x] == "^") {
					grid[size_t(grid_size) - 1][next_coord_x] = "^";
					grid[coord_y][coord_x] = ".";
					coord_x = next_coord_x;
					allowed = 1;
				}
				else {
					grid[0][next_coord_x] = "v";
					grid[coord_y][next_coord_x] = ">";
					grid[coord_y][coord_x] = ".";
					coord_x = next_coord_x;
					allowed = 1;
				}

			}
			else {
				grid[coord_y][next_coord_x] = ">";
				grid[coord_y][coord_x] = ".";
				coord_x = next_coord_x;
				allowed--;
			}

			if (coord_x == grid_size - 1) {
				grid[coord_y][coord_x] = "<";
			}
		}

		else {
			int next_coord_x = size_t(coord_x) - 1;
			if (grid[coord_y][next_coord_x] == "^" || grid[coord_y][next_coord_x] == "v") {

				if (grid[coord_y][next_coord_x] == "^") {
					grid[size_t(grid_size) - 1][next_coord_x] = "^";
					grid[coord_y][coord_x] = ".";
					coord_x = next_coord_x;
				}
				else {
					grid[0][next_coord_x] = "v";
					grid[coord_y][next_coord_x] = "<";
					grid[coord_y][coord_x] = ".";
					coord_x = next_coord_x;
				}
				allowed--;
				if (allowed == 0)
					allowed = 1;
			}
			else {
				grid[coord_y][next_coord_x] = "<";
				grid[coord_y][coord_x] = ".";
				coord_x = next_coord_x;
				allowed--;
			}
		}
	}
	return grid;
}


/** Fonction initialize_cells (créé par Colin)
*	Demande au joueur la taille de la grille de jeu voulue
*
*	@return cells, un tableau 2D contenant l'état initial du plateau de jeu en début de partie
**/
vector <vstring> initialize_cells(int grid_size) {
	vector<vstring> cells;
	for (int r = 0; r < grid_size; r++) {
		cells.push_back(vstring());
		for (int c = 0; c < grid_size; c++) {
			if ((r == 0 && c == 0) || (r == 0 && c == grid_size - 1) || (r == grid_size - 1 && c == 0) || (r == grid_size - 1 && c == grid_size - 1)) {
				cells[r].push_back("x");
			}
			else if (c == 0) {
				cells[r].push_back(">");
			}
			else if (r == 0) {
				cells[r].push_back("v");
			}
			else {
				cells[r].push_back(".");
			}
		}
	}
	return cells;
}


/** Fonction initialize_moves (créé par Colin)
*	@param grid_size, la taille de la grille de jeu
*
*	@return moves, un tableau 2D contenant le nombre de cases de déplacement autorisé sur chaque ligne
**/
vector <vstring> initialize_moves(int grid_size = 7) {
	vector<vstring> cells;
	for (int r = 0; r < grid_size; r++) {
		cells.push_back(vstring());
		for (int c = 0; c < grid_size; c++) {
			string push = " ";
			if ((r == 0 && c == 0) || (r == 0 && c == grid_size - 1) || (r == grid_size - 1 && c == 0) || (r == grid_size - 1 && c == grid_size - 1)) {
				push = " ";
			}
			else if (c == 0) {
				if (r <= grid_size / 2) {
					push = to_string(((grid_size - r) % (3)) + 1);
				}
				else {
					push = to_string(-(((-grid_size - r) % (-3)) - 1));
				}
			}
			else if (r == 0) {
				if (c <= grid_size / 2) {
					push = to_string(((grid_size + c) % 3) + 1);
				}
				else {
					push = to_string(-(((-grid_size + c) % (-3)) - 1));
				}
			}
			else if (r == grid_size - 1) {
				if (c <= grid_size / 2) {
					push = to_string(((grid_size - c) % 3) + 1);
				}
				else {
					push = to_string(-(((-grid_size - c) % (-3)) - 1));
				}
			}
			else if (c == grid_size - 1) {
				if (r <= grid_size / 2) {
					push = to_string(((grid_size + r) % 3) + 1);
				}
				else {
					push = to_string(-(((-grid_size + r) % (-3)) - 1));
				}
			}
			cells[r].push_back(push);
		}
	}
	return cells;
}

/** Fonction write_game (créé par Axel)
*	@param cells,  un tableau 2D de string représentant l'état de la partie actuelle
*
*	Prend le contenu de cells et l'écrit dans un fichier "etat_jeu.txt"
**/
void write_game(vector<vstring> cells) {
	ofstream folder;
	int size = static_cast<int>(cells[0].size());
	folder.open("etat_jeu.txt");

	for (int r = 0; r < size; r++) {
		for (int c = 0; c < size; c++) {

			folder << cells[r][c] << " ";

		}

		folder << endl;
	}

	folder.close();
}

/** Fonction readFile_etat_jeu (créé par Axel et Colin) 
*	@param size, la taille de la grille de jeu
*
*	@return le contenu du fichier lu dans un tableau 2D de string cells
**/
vector<vstring> readFile_etat_jeu(int size = 7) {
	vector<vstring> grid = vector<vstring>(size);
	ifstream doc;
	doc.open("etat_jeu.txt");
	string txt;
	for (int r = 0; r < size; r++) {
		grid[r] = vstring(size);
		for (int c = 0; c < size; c++) {
			doc >> txt;
			grid[r][c] = txt;


		}
	}

	doc.close();
	return grid;
}


/** Fonction ReadFile_resultat (créé par Axel)
*	@param size, la taille de la grille de jeu
*
*	@return le coup que l'IA a joué
**/
int readFile_resultat() {

	ifstream doc;
	doc.open("resultat.txt");
	int movePlayed;

	doc >> movePlayed;

	return movePlayed;
}


/** Fonction PawnOnRoad (créé par Axel)
*	@param cells,    le tableau de l'état du jeu actuel
*	@param moves,    explicitant le nombre de déplacement autorisé par ligne
*	@param coord_y,  une ligne de la grille de jeu
*
*	@return le nombre de pions "mangés" si on déplace le pion de la ligne coord_y, 0 sinon
**/
int pawnOnRoad(vector<vstring> cells, vector<vstring> moves, int coord_y) {

	int grid_size = static_cast<int>(moves[0].size());
	int coord_x = 0;
	int type = 0;
	int pawnEaten = 0;

	if (coord_y < 1 || coord_y >(grid_size - 1)) {
		return 0;
	}
	if (coord_x == 0) {
		return 0;
	}

	//Pour savoir dans quel sens est le pion
	// ">" = 0
	// "<" = 1
	for (int r = 0; r < grid_size; r++) {
		if (cells[coord_y][r] == "<" || cells[coord_y][r] == ">") {
			coord_x = r;
			if (cells[coord_y][coord_x] == ">") {
				type = 0;
			}
			else {
				type = 1;
			}
		}
	}

	int allowed = allow_dot(coord_y, moves, cells);
	assert(allowed < 4); //Le maximum de points de déplacement attribuables est de 3
	int next_coord_x;

	while (allowed > 0 || (coord_x == grid_size - 1 || coord_x == 0)) {

		allowed--;

		if (type == 0) {
			next_coord_x = coord_x + 1;
		}
		else {
			next_coord_x = coord_x - 1;
		}
		if (coord_x == grid_size - 1 || coord_x == 0)
			break;

		if (cells[coord_y][next_coord_x] != ".") {
			pawnEaten++;
			allowed = 1;
		}

		coord_x = next_coord_x;
	}
	assert(pawnEaten < (grid_size-1)); //Le maximum de pions mangés est du nombre de case - 2
	return pawnEaten;

}


/** Fonction IA_squadro (créé par Axel, modifié par Colin)
*	Lit le contenu du fichier "etat_jeu.txt" et écrit le coup à jouer dans "resultat.txt"
**/
void IA_squadro() {

	vector<vstring> cellsIA = readFile_etat_jeu(7);
	vector<vstring> movesIA = initialize_moves(7);
	int moveToPlay = 0;
	int grid_size = movesIA.size();
	ofstream doc;
	doc.open("resultat.txt");

	// L'IA réfléchit au coup qu'elle va jouer

	vector<int> movesToPlay;
	do {
		for (int r = 0; r < 5; r++) {

			if (pawnOnRoad(cellsIA, movesIA, r) != 0) {
				movesToPlay.push_back(r);
			}
		}

		movesToPlay.push_back(-1);

		if (movesToPlay[0] == -1) {
			moveToPlay = (int)(rand() % 5) + 1;
		}
		else {
			int random = (int)(rand() % (movesToPlay.size()-2));
			moveToPlay = movesToPlay[random];
		}

	} while (allow_dot(moveToPlay, movesIA, cellsIA) == 0);
	//_______________________________________

	assert(moveToPlay > 0 && moveToPlay < (grid_size)); //Le choix du pion déplacé doit être compris entre 1 et grid_size - 1
	doc << moveToPlay << ",";
	doc.close();
}

/** Fonction initialize_virtual_grid (créé par Axel, modifié par Colin)
*	Dessine la grille de jeu dans la fenêtre
*
*	@param size, la taille de la grille
**/
void initialize_virtual_grid(int size) {
	window.clear(sf::Color::White);
	window.setSize(sf::Vector2u(1020, 740));

	sf::Text text_squadro;
	text_squadro.setFont(font);
	text_squadro.setCharacterSize(50);
	text_squadro.setFillColor(sf::Color::Black);
	text_squadro.setStyle(sf::Text::Bold);
	text_squadro.setString("Squadro");
	text_squadro.setPosition(925, 50);

	window.draw(text_squadro);


	float length = 860.;
	sf::RectangleShape linev(sf::Vector2f(2, length));
	sf::RectangleShape lineh(sf::Vector2f(length, 2));

	linev.setFillColor(sf::Color::Black);
	lineh.setFillColor(sf::Color::Black);

	//On dessine les lignes de la grille
	for (float i = 0; i <= size; i++) {

		linev.setPosition(margin + (length * i) / size, margin);
		lineh.setPosition(margin, margin + (length * i) / size);

		window.draw(lineh);
		window.draw(linev);

	}


}

/** Fonction draw_virtual_grid (créé par Axel, modifié par Colin)
*	Dessine les jetons sur la grille et à qui c'est le tour de jouer
*
*	@param size,  la taille de la grille
*	@param cells, l'état actuel de la partie
*	@param moves, le nombre de déplacements autorisés par lignes
*	@param turn,  disant à qui c'est le tour de jouer
**/
void draw_virtual_grid(int size, vector<vstring> cells, vector<vstring> moves, int turn) {
	
	float length = 860.;
	initialize_virtual_grid(size);
	window.draw(draw_player_shape(turn));
	window.draw(draw_player_text(turn));
	sf::Text text_moves;
	text_moves.setCharacterSize(210/size);
	text_moves.setFillColor(sf::Color::Black);
	text_moves.setFont(font);

	for (int i = 1; i <= size - 2; i++) {
		text_moves.setPosition(23 + (length/size * i), 21);
		text_moves.setString(moves[0][i]);
		window.draw(text_moves);

		text_moves.setPosition(23 + (length / size * i), 30 + (length / size * (size - 1)));
		text_moves.setString(moves[size-1][i]);
		window.draw(text_moves);
	}

	for (int i = 1; i <= size - 2; i++) {
		text_moves.setPosition(23, 21 + length/size * i);
		text_moves.setString(moves[i][0]);
		window.draw(text_moves);

		text_moves.setPosition(30 + (length/size * (size-1)), 21 + length / size * i);
		text_moves.setString(moves[i][size-1]);
		window.draw(text_moves);

	}

	float radius = (float)((length) / (size * 2.5));
	assert(radius > 0); //Le radius doit être supérieur à 0 (sinon les triangles seraient invisibles)
	sf::CircleShape yellow_triangle(radius, 3);
	sf::CircleShape red_triangle(radius, 3);

	yellow_triangle.setFillColor(sf::Color::Yellow);
	yellow_triangle.setOrigin(radius, radius);

	red_triangle.setFillColor(sf::Color::Red);
	red_triangle.setOrigin(radius, radius);

	for (int j = 0; j < size; j++) {
		for (int i = 0; i < size; i++) {
			yellow_triangle.setRotation(0);
			red_triangle.setRotation(90);



			if (cells[i][j] != "." && cells[i][j] != "x") {
				if (cells[i][j] == "^" || cells[i][j] == "v")
				{
					yellow_triangle.setPosition((((length * j) / (float)(size)+(length * (j + 1)) / (float)(size)) / 2) + margin,
						(((length * i) / (float)(size)+(length * (i + 1)) / (float)(size)) / 2) + margin);


					if (cells[i][j] == "v")
						yellow_triangle.setRotation(180);

					window.draw(yellow_triangle);

				}
				else
				{
					red_triangle.setPosition((((length * j) / (float)(size)+(length * (j + 1)) / (float)(size)) / 2 + margin),
						(((length * i) / (float)(size)+(length * (i + 1)) / (float)(size)) / 2) + margin);

					if (cells[i][j] == "<")
						red_triangle.setRotation(270);

					window.draw(red_triangle);


				}


			}
		}
	}

	window.display();
}

/** Fonction define_grid_size (créé par Colin)
*	Demande au joueur la taille de la grille de jeu voulue
*
*	@return la taille de la grille
**/
int define_grid_size() {

	window.clear();
	sf::Text Choisissez;
	// choix de la police à utiliser
	Choisissez.setFont(font); // font est un sf::Font
	// choix de la chaîne de caractères à afficher
	Choisissez.setString("Choisissez la taille de la grille :");
	// choix de la taille des caractères
	Choisissez.setCharacterSize(24);
	// choix de la couleur du texte
	Choisissez.setFillColor(sf::Color::White);
	// choix du style du texte
	Choisissez.setStyle(sf::Text::Bold);
	Choisissez.setPosition(400, 0);

	window.draw(Choisissez);

	//Dessine le " - "
	sf::RectangleShape rectangle_moins(sf::Vector2f(120, 50));
	rectangle_moins.setFillColor(sf::Color(255, 165, 0));
	rectangle_moins.setOutlineThickness(0);
	rectangle_moins.setOutlineColor(sf::Color(0, 0, 0));
	rectangle_moins.setPosition(100, 50);

	window.draw(rectangle_moins);

	//Dessine le " + "
	sf::RectangleShape rectangle_plushaut(sf::Vector2f(120, 50));
	rectangle_plushaut.setFillColor(sf::Color(255, 165, 0));
	rectangle_plushaut.setOutlineThickness(0);
	rectangle_plushaut.setOutlineColor(sf::Color(0, 0, 0));
	rectangle_plushaut.setPosition(1000, 50);
	window.draw(rectangle_plushaut);
	sf::RectangleShape rectangle_plusbas(sf::Vector2f(50, 120));
	rectangle_plusbas.setFillColor(sf::Color(255, 165, 0));
	rectangle_plusbas.setOutlineThickness(0);
	rectangle_plusbas.setOutlineColor(sf::Color(0, 0, 0));
	rectangle_plusbas.setPosition(1035, 15);
	window.draw(rectangle_plusbas);


	window.draw(draw_validate_shape());

	window.draw(draw_validate_text());




	int grid_size = 7;
	bool validation = false;

	//Dessine la variable de la taille au centre
	sf::Text grid_size_display;
	grid_size_display.setFont(font);
	grid_size_display.setString(to_string(grid_size));
	grid_size_display.setCharacterSize(70);
	grid_size_display.setFillColor(sf::Color::White);
	grid_size_display.setStyle(sf::Text::Bold);
	grid_size_display.setPosition(550, 200);
	window.draw(grid_size_display);

	window.display();
	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
		}
		if (event.type == sf::Event::MouseButtonPressed)
		{
			int x(event.mouseButton.x);
			int y(event.mouseButton.y);
			if (event.mouseButton.x > 1000 && event.mouseButton.y < 250) {
				grid_size = grid_size + 2;
			}
			else if (event.mouseButton.x < 250 && event.mouseButton.y < 200) {
				if (grid_size > 5) {
					grid_size = grid_size - 2;
				}
			}
			else if (event.mouseButton.x > 500 && event.mouseButton.x < 900 && event.mouseButton.y < 800 && event.mouseButton.y > 300) {
				return grid_size;
			}
			grid_size_display.setString(to_string(grid_size));
			window.clear();
			window.draw(grid_size_display);
			window.draw(rectangle_plusbas);
			window.draw(rectangle_moins);
			window.draw(rectangle_plushaut);
			window.draw(Choisissez);
			window.draw(draw_validate_shape());
			window.draw(draw_validate_text());
			window.display();
			sf::sleep(delay);
		}
	}
	assert(grid_size > 0); //Si la grille vaut 0 ou moins elle ne peut pas s'initialiser
	return -1;
}

/** Fonction ask_player_name (créé par Colin)
*	Demande le nom du joueur dans le menu
*
*	@param position, la "position" du joueur (soit le 1er, soit le 2e)
*	@return le nom choisi par le joueur
**/
string ask_player_name(string position) {
	window.clear();
	std::string name_memory;
	sf::Text name;
	sf::Text ask_name;
	ask_name.setFont(font);
	ask_name.setString("Entrez le nom du " + position + " joueur :");
	ask_name.setCharacterSize(30);
	ask_name.setFillColor(sf::Color::White);
	ask_name.setStyle(sf::Text::Bold);

	window.draw(ask_name);
	window.draw(draw_validate_shape());
	window.draw(draw_validate_text());
	window.display();

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (event.type == sf::Event::TextEntered)
		{
			// Handle ASCII characters only
			if (event.text.unicode < 128)
			{

				name_memory += event.text.unicode;
				sf::sleep(delay);
			}
			name.setString(name_memory);
			name.setFont(font);
			name.setCharacterSize(70 - name_memory.size());
			name.setFillColor(sf::Color::White);
			name.setStyle(sf::Text::Bold);
			name.setPosition((550 - (float)(name_memory.size()) * 10), 200);
			window.clear();
			window.draw(name);
			window.draw(ask_name);
			window.draw(draw_validate_shape());
			window.draw(draw_validate_text());
			window.display();



		}
		if (event.type == sf::Event::MouseButtonPressed)
		{
			int x(event.mouseButton.x);
			int y(event.mouseButton.y);
			if (event.mouseButton.x > 500 && event.mouseButton.x < 700 && event.mouseButton.y < 450 && event.mouseButton.y > 300) {
				return name_memory;

				sf::sleep(delay);
			}
		}
	}
	return "Default";
}

/** Fonction gamemode_selection (créé par Colin)
*	Demande le mode de jeu voulu par le/les joueurs
*
*	@param grid_size la taille de la grille de jeu
*	@return 1 pour "humain vs IA", 0 pour "Humain vs humain" 
**/
int gamemode_selection(int grid_size) {
	sf::Text gamemode_text_human;
	sf::Text gamemode_text_IA;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(draw_validate_shape());

		gamemode_text_human.setFont(font);
		gamemode_text_human.setString("Humain VS Humain");
		gamemode_text_human.setCharacterSize(20);
		gamemode_text_human.setFillColor(sf::Color::White);
		gamemode_text_human.setStyle(sf::Text::Bold);
		gamemode_text_human.setPosition(480, 410);
		window.draw(gamemode_text_human);

		gamemode_text_IA.setFont(font);

		if (grid_size == 7) {
			gamemode_text_IA.setString("Humain VS IA");



			gamemode_text_IA.setFillColor(sf::Color::White);


			gamemode_text_IA.setCharacterSize(20);
			gamemode_text_IA.setStyle(sf::Text::Bold);
			gamemode_text_IA.setPosition(496, 610);
			sf::RectangleShape IA_shape(sf::Vector2f(240, 50));
			IA_shape.setFillColor(sf::Color(0, 0, 0));
			IA_shape.setOutlineThickness(3);
			IA_shape.setOutlineColor(sf::Color(255, 165, 0));
			IA_shape.setPosition(450, 600);
			window.draw(IA_shape);
			window.draw(gamemode_text_IA);
			if (event.type == sf::Event::MouseButtonPressed)
			{
				int x(event.mouseButton.x);
				int y(event.mouseButton.y);
				if (event.mouseButton.x > 500 && event.mouseButton.x < 700 && event.mouseButton.y < 650 && event.mouseButton.y > 450) {
					return 1;
				}
			}
		}
		if (event.type == sf::Event::MouseButtonPressed)
		{
			int x(event.mouseButton.x);
			int y(event.mouseButton.y);

			if (event.mouseButton.x > 500 && event.mouseButton.x < 700 && event.mouseButton.y < 450 && event.mouseButton.y > 300) {
				return 0;
			}
		}
		window.display();
	}
	return -1;
}

/** Fonction move_selection (Créé par Colin)
*	Renvoie le coup joué par le joueur
*	
*	@param turn, indiquant à quel joueur c'est le tour
*	@param size, la taille de la grille de jeu
*	@return quel pion est joué (soit 1, 2, 3, 4 ou 5)
**/
int move_selection(int turn, int size) {
	float length = 860.;
	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (event.type == sf::Event::MouseButtonPressed)
		{
			int y(event.mouseButton.y);
			int x(event.mouseButton.x);
			if (turn == 0) {
				return ((event.mouseButton.y + margin) / (length / size));
				//return (event.mouseButton.y - margin);
			}
			else {
				return ((length - margin - (event.mouseButton.x)) / (length / size));
			}
		}
		if (event.type == sf::Event::TextEntered)
		{
			if (event.text.unicode < 128)
			{
				if (turn == 0) {
					return (event.text.unicode - 48);
				}
				else {
					return (size - 1 - (event.text.unicode - 48));
				}
			}
		}
		
	}
	return 0;
}


int main() {
	if (!font.loadFromFile("arial.ttf"))
	{
		// erreur
	}
	vstring labels = { "c", "r" };
	bool clockwise;
	int coord_y;
	int allowed = 0;
	bool game = true;
	int botJ2 = 3;
	bool isJ2aBot = false;

	std::string name2_memory;


	int grid_size;
	vector<vstring> cells;
	vector<vstring> moves;
	sf::Text ask_j2_name;
	string name1;
	string name2;

	grid_size = define_grid_size();
	assert(grid_size > 4); //Stoppe le programme si la taille de la grille est inférieure à 5
	window.clear();
	cells = initialize_cells(grid_size);
	moves = initialize_moves(grid_size);

	name1 = ask_player_name("premier");

	if (gamemode_selection(grid_size) == 1) {
		isJ2aBot = true;
	}
	else {
		name2 = ask_player_name("second");
	}

	initialize_virtual_grid(grid_size);
	window.setPosition(sf::Vector2i(50, 50));
	int turn = 0;
	draw_virtual_grid(grid_size, cells, moves, turn);

	
	while (game == true) {
		while (window.isOpen())
		{
			while (window.pollEvent(event))
			{
				// "close requested" event: we close the window
				if (event.type == sf::Event::Closed)
					window.close();
			}
			assert(turn == 0);
			draw_virtual_grid(grid_size, cells, moves, turn);
			
			do {
				coord_y = move_selection(turn, grid_size);
				allowed = allow_dot(coord_y, moves, cells);
				sf::sleep(delay);
			} while (allowed == 0);
			
			cells = move_dot(coord_y, allowed, moves, cells);
			write_game(cells);
			// vérifier si la partie est finie
			if (is_game_over(cells)) {
				game = false;
				break;
			}
			//______________________________
			
			turn = 1;
			draw_virtual_grid(grid_size, cells, moves, turn);
			window.draw(draw_player_shape(turn));
			
			rotate_grid(&cells, &moves, &labels, &grid_size, clockwise = false);

			if (!isJ2aBot) {

				
				
				do {
					coord_y = move_selection(turn, grid_size);
					allowed = allow_dot(coord_y, moves, cells);
					sf::sleep(delay);
				} while (allowed == 0);
			
			}
			else {

				IA_squadro();
				coord_y = readFile_resultat();
				allowed = allow_dot(coord_y, moves, cells);
		
		
		}
			cells = move_dot(coord_y, allowed, moves, cells);

			// vérifier si la partie est finie
			if (is_game_over(cells)) {
				game = false;
				break;
			}
			//______________________________
			rotate_grid(&cells, &moves, &labels, &grid_size, clockwise = true);
			write_game(cells);
			turn = 0;
		
			
		}

	}
	sf::Text winner;
	winner.setFont(font);
	winner.setCharacterSize(35);
	winner.setStyle(sf::Text::Bold);
	winner.setPosition(200, 400);
	if (name1 == "") {
		name1 = "Anonymous OwO";
	}
	if (name2 == "") {
		name2 = "Anonymous OwO";
	}
	if (labels[0] == "c") {
		winner.setString("Victoire du joueur rouge : " + name1);
		winner.setFillColor(sf::Color::Red);
	}
	else {
		winner.setString("Victoire du joueur jaune : " + name2);
		winner.setFillColor(sf::Color::Yellow);
	}

	
	window.clear(sf::Color::White);
	window.draw(winner);
	window.display();
	sf::sleep(delay);
	sf::sleep(delay);
	sf::sleep(delay);
	sf::sleep(delay);
	sf::sleep(delay);
	sf::sleep(delay);
	sf::sleep(delay);
	sf::sleep(delay);
	
	draw_virtual_grid(grid_size, cells, moves, turn);
	sf::sleep(delay);
	sf::sleep(delay);
	sf::sleep(delay);
	sf::sleep(delay);
	sf::sleep(delay);
	sf::sleep(delay);
	sf::sleep(delay);
	sf::sleep(delay);
	sf::sleep(delay);
	sf::sleep(delay);
	sf::sleep(delay);
	//
	return 0;
	
}
