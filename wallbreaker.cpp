 
#pragma warning( disable : 4996 ) 

 
#include <cstdlib>
#include <vector>
#include <iostream>
#include <string>
#include <time.h>
#include "G2D.h"
 
 
using namespace std;
enum class ECRAN {ECRAN_ACCUEIL, ECRAN_OPTIONS, INIT_PARTIE, ECRAN_JEU, ECRAN_GAME_OVER,ECRAN_WELL_DONE};

ECRAN ecran = ECRAN::ECRAN_ACCUEIL;
 
struct _player
{
	string texture =
		"[RRRRYYYYYYYYYYYYYYYYRRRR]"
		"[RRRRYYYYYYYYYYYYYYYYRRRR]";
		
	V2 Size;
	int IdTex; 
	int Lives = 3;
	V2 Pos = V2(275,20);
	const V2 InitialPos = V2(275, 20);
	
};

struct _ball {
	float BallRadius = 15;
	V2 BallPos = V2(300, 50);
	V2 BallSpeed = V2(20, 50);
	V2 initialBallSpeed = V2(20, 50);
	const V2 InitialBallPos = V2(300, 50);
	
};

struct _brick {
	int power;
	V2 pos;
	V2 size = V2(30.5, 15);
	
	
};




struct GameData
{

    string Map =
				 "MMMMMMMMMMMMMMM"
				 "M             M"
				 "M             M"
				 "M             M"
				 "M             M"
				 "M             M"
				 "M             M"
				 "M             M"
				 "M             M"
				 "M             M"
				 "M             M"
				 "M             M"
				 "M             M"
				 "M             M"
				 "M             M";

	// indique la présence d'un mur à la case (x,y)
	bool Mur(int x, int y) { return Map[(15 - y - 1)*15+x] == 'M'; }  

	int Lpix = 40;  // largeur en pixels des cases du labyrinthe

	_player player;   // data du héros
	_ball ball;
	std::vector<_brick> bricks;


	int speed =5;
	int powers[3] = { 1,3,5 };
	int Score = 0;
	int cnt = 0;
	GameData() {}
};

GameData G;
double timer;

bool intersectRectangles(V2 Pos1, V2 Size1, V2 Pos2, V2 Size2) {
	if (Pos1.y + Size1.y < Pos2.y) return false;  // (1)
	if (Pos1.y > Pos2.y + Size2.y)   return false;  // (2)
	if (Pos1.x > Pos2.x + Size2.x)   return false;  // (3)
	if (Pos1.x + Size1.x < Pos2.x)   return false;  // (4)

	return true;
}

void render()
{	
	G2D::ClearScreen(Color::Gray);


	if ((ecran == ECRAN::ECRAN_GAME_OVER) && (timer > 0)) {
		G2D::DrawStringFontMono(V2(120, 400), string(" GAME OVER"), 50, 5, Color::Red);
		timer -= G2D::ElapsedTimeFromLastCallbackSeconds();
	}

	if ((ecran == ECRAN::ECRAN_WELL_DONE) && (timer > 0)) {
		G2D::DrawStringFontMono(V2(120, 400), string(" WELL DONE"), 50, 5, Color::Green);
		timer -= G2D::ElapsedTimeFromLastCallbackSeconds();
	}
	
	if (ecran == ECRAN::ECRAN_OPTIONS) {
		G2D:: DrawRectangle(V2 (100,200), V2 (400,50), Color::Green, true);
		G2D:: DrawRectangle(V2(100, 300), V2 (400,50), Color::Red, true);
		G2D::DrawStringFontMono(V2 (105,215), std::string("Press DOWN for less speed"), 20,  3,  Color::Black);
		G2D::DrawStringFontMono(V2 (105,315), std::string(" Press UP for more speed"), 20, 3, Color::Black);
		G2D::DrawStringFontMono(V2 (185,505), std::string(" Amount of speed: "+ to_string(G.speed)), 20, 3, Color::Yellow);
		G2D::DrawStringFontMono(V2(20, 20), std::string(" Press ENTER to exit screen"), 15, 3, Color::Yellow);
	}

	if (ecran == ECRAN::ECRAN_JEU) {

		
		for (int x = 0; x < 15; x++)
			for (int y = 0; y < 15; y++)
			{
				int xx = x * G.Lpix;
				int yy = y * G.Lpix;
				if (G.Mur(x, y))
					G2D::DrawRectangle(V2(xx, yy), V2(G.Lpix, G.Lpix), Color::Black, true);
			}
		for (int i = 0; i < G.bricks.size(); i++) {
			switch (G.bricks[i].power) {
			case 1:
				    G2D::DrawRectangle(G.bricks[i].pos, G.bricks[i].size, Color::Green, true);
				break;
			case 2:
			case 3:
					G2D::DrawRectangle(G.bricks[i].pos, G.bricks[i].size, Color::Yellow, true);
				break;
			case 4:
			case 5:
					G2D::DrawRectangle(G.bricks[i].pos, G.bricks[i].size, Color::Red, true);
				break;
			default:
				break;
			}
			
		}
		G2D::DrawRectWithTexture(G.player.IdTex, G.player.Pos, G.player.Size);
		G2D::DrawCircle(G.ball.BallPos, 5, Color::Red, true);
		G2D::DrawStringFontMono(V2(10, 570), std::string("Lives: " + std::to_string(G.player.Lives)), 30, 5, Color::Red);
		G2D::DrawStringFontMono(V2(280, 570), std::string("Score: " + std::to_string(G.Score)), 30, 5, Color::Red);
	}
	

	if (ecran == ECRAN::ECRAN_ACCUEIL) {
		G2D:: DrawRectangle(V2 (150,200), V2 (300,50), Color::Green, true);
		G2D:: DrawRectangle(V2(150, 300), V2 (300,50), Color::Blue, true);
		G2D::DrawStringFontMono(V2 (155,215), std::string("   Press P to play"), 20,  3,  Color::Black);
		G2D::DrawStringFontMono(V2 (155,315), std::string(" Press S for settings"), 20, 3, Color::Black);
		G2D::DrawStringFontMono(V2 (85,405), std::string("  Wall Breaker!"), 40, 3, Color::Yellow);
	}
	G2D::Show();
}

V2 Rebond(V2 v, V2 n)
{
	n.normalize();
	V2 t(n.y, -n.x);  // rotation de 90° du vecteur n
	float vt = prodScal(v , t);     // produit scalaire, vt est un float
	float vn = prodScal( v , n);     // produit scalaire, vn est un float
	V2 r = vt * t - vn * n; // * entre un flottant et un V2
	return r;
}
bool  SameDirection(V2 u, V2 v) {
	float dir = u.x * v.x + u.y * v.y;
	if (dir > 0) return true;
	else return false;
}

bool OppositeDirection(V2 u, V2 v) {
	float dir = u.x * v.x + u.y * v.y;
	if (dir < 0) return true;
	else return false;
}

float DistPointSegment(V2 AB, V2 A, V2 P) {
	V2 T = AB;
	V2 N = V2(T.y, -T.x);
	N.normalize();
	V2 AP = A - P;
	return abs(prodScal(AP,N));

}

float Distance(V2 point1, V2 point2)
{
	return sqrt((point1.x - point2.x) * (point1.x - point2.x) + (point1.y - point2.y) * (point1.y - point2.y));
}

bool  InterSegmentCercle(V2 A, V2 B, V2 Center) {

	if (Distance(A, Center) <= G.ball.BallRadius) return true;  // zone 1
	if (Distance(B, Center) <= G.ball.BallRadius) return true;  // zone 3
	if (SameDirection(B - A, Center - A) && SameDirection(A - B, Center - B)) { // C en zone 2
		if (DistPointSegment(B - A, A, Center) <= G.ball.BallRadius) return true;


	}
	return false;
}

ECRAN gestion_jeu() {

	V2 NewPlayerPos = G.player.Pos;
	if (G2D::IsKeyPressed(Key::LEFT) && G.player.Pos.x>40){
		G.player.Pos.x=G.player.Pos.x-5;
		
	}
	if (G2D::IsKeyPressed(Key::RIGHT) && G.player.Pos.x<485) {
		G.player.Pos.x=G.player.Pos.x+5;
		
	}

	double dt = G2D::ElapsedTimeFromLastCallbackSeconds();
	V2 NewPos = G.ball.BallPos + G.ball.BallSpeed * dt;
	V2 NewSpd = G.ball.BallSpeed;
	bool collision = false;
	if (G.ball.BallPos.y < 0) {
		G.player.Lives--;
		if (G.player.Lives > 0) {
			G.ball.BallPos = G.ball.InitialBallPos;
			G.ball.BallSpeed = G.ball.initialBallSpeed*G.speed;
			G.player.Pos = G.player.InitialPos;
			_sleep(1000);
			return ECRAN::ECRAN_JEU;
		}
		if (G.player.Lives == 0) {
			return ECRAN::ECRAN_GAME_OVER;
		}
	}
	if (NewPos.x > 560) {
		collision = true;
		V2 n = V2(-1, 0);
		NewPos = G.ball.BallPos;
		NewSpd = Rebond(NewSpd, n);
	}

	if (NewPos.y > 555) {
		collision = true;
		V2 n = V2(0, -1);
		NewPos = G.ball.BallPos;
		NewSpd = Rebond(NewSpd, n);
	}

	if (NewPos.x < 40) {
		collision = true;
		V2 n = V2(1, 0);
		NewPos = G.ball.BallPos;
		NewSpd = Rebond(NewSpd, n);
	}

	if (InterSegmentCercle(V2(G.player.Pos.x, G.player.Pos.y +G.player.Size.y), V2(G.player.Pos.x+G.player.Size.x, G.player.Pos.y), NewPos)) {
		collision = true;
		V2 n = V2(0, 1);
		NewSpd = Rebond(NewSpd, n);
		NewPos = G.ball.BallPos;
	}
	for (int i = 0; i < G.bricks.size(); i++) {
		if (InterSegmentCercle(V2(G.bricks[i].pos.x, G.bricks[i].pos.y + G.bricks[i].size.y), V2(G.bricks[i].pos.x + G.bricks[i].size.x, G.bricks[i].pos.y + G.bricks[i].size.y), NewPos) && (G.bricks[i].power>0)) {
			collision = true;
			V2 n = V2(0, -1);
			NewSpd = Rebond(NewSpd, n);
			NewPos = G.ball.BallPos;
			G.bricks[i].power--;
			if (G.bricks[i].power == 0) {
				G.Score = G.Score + 100;
				G.cnt++;
				if (G.cnt == G.bricks.size()) {
					return ECRAN::ECRAN_WELL_DONE;
				}
			}
		}
	}

	if (collision == true)
	{
		G.ball.BallSpeed = NewSpd;
	}
	else
	{
		G.ball.BallPos = NewPos;
		G.ball.BallSpeed = NewSpd;
	}

	if (G.player.Lives == 0) {
			return ECRAN::ECRAN_GAME_OVER;
	}
	return ECRAN::ECRAN_JEU;
}

ECRAN gestion_GameOver(){
	if (timer <= 0.0) {
		timer = 3.0;
		G.player.Lives = 3;
		return ECRAN::ECRAN_ACCUEIL;
	}
	else
		return ECRAN::ECRAN_GAME_OVER;
}

ECRAN gestion_WellDone() {
	if (timer <= 0.0) {
		timer = 3.0;
		G.player.Lives = 3;
		return ECRAN::ECRAN_ACCUEIL;
	}
	else
		return ECRAN::ECRAN_WELL_DONE;
}

ECRAN gestion_ecran_accueil() {
	if (G2D::IsKeyPressed(Key::P)){		
		return ECRAN::INIT_PARTIE;
	}
	if (G2D::IsKeyPressed(Key::S)) {
		return ECRAN::ECRAN_OPTIONS;
	}
	else return ECRAN::ECRAN_ACCUEIL;
}

ECRAN InitPartie() {

	srand(time(NULL));
	G.bricks.clear();
	for (int i = 0; i < 17; i++) {
		for (int j = 0; j < 4; j++) {
			_brick b;
			b.power = (G.powers[rand() % 3]);
			b.pos = V2(i*b.size.x+40,j*b.size.y+450);
			G.bricks.push_back(b);
		}
	}
	
	G.player.Lives = 3;
	G.player.Pos = G.player.InitialPos;
	G.ball.BallPos = G.ball.InitialBallPos;
	G.ball.BallSpeed = G.ball.initialBallSpeed * G.speed;
	G.cnt = 0;
	timer = 3.0;
	G.Score = 0;
	return ECRAN::ECRAN_JEU;
}

ECRAN gestion_ecran_options() {
	if (G2D::IsKeyPressed(Key::UP)&& G.speed<15) {
		G.speed++;
	}
	if (G2D::IsKeyPressed(Key::DOWN) && G.speed > 1 ) {
		G.speed--;
	}
	if (G2D::IsKeyPressed(Key::ENTER)) {
		return ECRAN::ECRAN_ACCUEIL;
	}
	_sleep(100);
	return ECRAN::ECRAN_OPTIONS;
	
	
}





void Logic()
{
	if (ecran == ECRAN::ECRAN_ACCUEIL)
		ecran = gestion_ecran_accueil();

	if (ecran == ECRAN::ECRAN_OPTIONS)
		ecran = gestion_ecran_options();
		
	if (ecran == ECRAN::INIT_PARTIE)
		ecran = InitPartie();
	
	if (ecran == ECRAN::ECRAN_JEU)
		ecran = gestion_jeu();
	if (ecran == ECRAN::ECRAN_GAME_OVER)
		ecran = gestion_GameOver();
	if (ecran == ECRAN::ECRAN_WELL_DONE)
		ecran = gestion_WellDone();
}

void AssetsInit()
{
	// Size passé en ref et texture en param
	G.player.IdTex = G2D::InitTextureFromString(G.player.Size, G.player.texture);
	G.player.Size = G.player.Size * 3; // on peut zoomer la taille du sprite

	
}

int main(int argc, char* argv[])
{ 

	G2D::InitWindow(argc,argv,V2(G.Lpix * 15, G.Lpix * 15), V2(200,200), string("Wall Breaker"));
	  
	AssetsInit();

	G2D::Run(Logic,render);
 
}
 
  
 