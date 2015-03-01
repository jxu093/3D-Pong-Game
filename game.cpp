#include "game.h"
#include <iostream>

//PADDLE STUFF
Paddle::Paddle(bool isHuman, float Speed, float size, float boundaryZ) {
	if (isHuman) _position[2] = 0; //If the paddle is for the human player set it to closer to the screen
	else _position[2] = boundaryZ; //if paddle is for CPU, set it to the "back end" of the screen
	_position[0] = -size/2;
	_position[1] = size/2;
	_maxSpeed = Speed;
	_size=size;

	_direction[0]=0; _direction[1]=0;
}

float Paddle::get_positionX() {
	return _position[0];
}

float Paddle::get_positionY() {
	return _position[1];
}

float Paddle::get_positionZ() {
	return _position[2];
}

float Paddle::get_size() {
	return _size;
}

float Paddle::get_maxSpeed() {
	return _maxSpeed;
}
float Paddle::get_directionX() {
	return _direction[0];
}

float Paddle::get_directionY() {
	return _direction[1];
}

void Paddle::set_direction(float x, float y) {
	_direction[0]=x;
	_direction[1]=y;
}

void Paddle::update(float boundX, float boundY) { //update paddle position based on direction values
	_position[0]+=_direction[0];
	if (_position[0]>=boundX-_size) _position[0]=boundX-_size;
	else if (_position[0]<=-boundX) _position[0]=-boundX;
	_position[1]+=_direction[1];
	if (_position[1]>=boundY) _position[1]=boundY;
	else if (_position[1]<=-boundY+_size) _position[1]=-boundY+_size;
}
//END PADDLE STUFF


//PONG BALL STUFF
PongBall::PongBall(float speed, float size) {//constructor
	//place in center
	_position[0]=-size/2;
	_position[1]=size/2;
	_position[2]=-1;

	//set attributes
	_speed=speed;
	_size=size;

	_direction[0]=0; _direction[1]=0; _direction[2]=0;
	_isBouncing=0; _isOut=0;
}

void PongBall::wallBounce(int bounceDirection, float boundX, float boundY) {
	//1=left wall, 2=right wall, 3=top wall, 4=bottom wall
	//reverse directions as appropriate
	if (bounceDirection==1) {
		_position[0]=-boundX;
		_direction[0]*=-1;
	}
	else if (bounceDirection==2) {
		_position[0]=boundX-_size;
		_direction[0]*=-1;
	}
	else if (bounceDirection==3) {
		_position[1]=boundY;
		_direction[1]*=-1;
	}
	else if(bounceDirection==4) {
		_position[1]=-boundY+_size;
		_direction[1]*=-1;
	}
	_isBouncing=1;
}

void PongBall::paddleBounce(int bounceDirection, float pdX, float pdY, float boundZ) {
	//2=computer 1=player
	if (bounceDirection==2) {
		// std::cout<<_direction[2];
		_position[2]=boundZ;
		_direction[2]*=-1;
		_direction[0]+=1.0*pdX;
		_direction[1]+=1.0*pdY;
		std::cout<<_direction[1];
	}
	else {
		_position[2]=-1;
		_direction[2]*=-1;
		_direction[0]+=1.0*pdX;
		_direction[1]+=1.0*pdY;
	}
	_isBouncing=1;//change ball state to bouncing
}

bool PongBall::isOut() {
	return _isOut;
}

bool PongBall::isIncoming() {
	return (_direction[2]<0); //if z direction is negative it means the ball is headed towards CPU
}

int PongBall::isColliding(float boundZ) {
	float pz=_position[2];
	if (pz>-1) return 1; //return 1 if ball entering player's zone
	else if (pz<boundZ) return 2; //return 2 if ball entering cpu's zone
	else return 0; //return 0 if ball still in play
}

float PongBall::get_positionX() {
	return _position[0];
}

float PongBall::get_positionY() {
	return _position[1];
}

float PongBall::get_positionZ() {
	return _position[2];
}

float PongBall::get_size() {
	return _size;
}

void PongBall::set_direction(float x, float y, float z) {
	_direction[0]=x;
	_direction[1]=y;
	_direction[2]=z;
}

// float PongBall::get_directionX() {
// 	return _direction[0];
// }

// float PongBall::get_directionY() {
// 	return _direction[1];
// }

int PongBall::update(float boundX, float boundY, float boundZ, float pdX1, float pdY1, float ppX1, float ppY1, float pdX2, float pdY2, float ppX2, float ppY2, float pSize) {
	
	if (_isBouncing) _isBouncing=0; //reset bouncing state
	float px=_position[0]+_direction[0]*_speed; //temporary values for convenience
	float py=_position[1]+_direction[1]*_speed;
	float pz=_position[2]+_direction[2]*_speed;

	int goalScored=0; //integer value to keep track of where ball is scored (or 0 if no score)
	// //check if ball reached end zone
	if (isColliding(boundZ)==1) {
		//check if ball hit paddle then bounce
		if (px+_size>ppX1 && px<ppX1+pSize && py>ppY1-pSize && py-_size<ppY1) paddleBounce(isColliding(boundZ),pdX1,pdY1,boundZ);
		//ball goes through end zone
		else {
			_isOut=1;
			goalScored=1;
		}
	}
	else if (isColliding(boundZ)==2) {
		// std::cout<<"hi";
		if (px+_size>ppX2 && px<ppX2+pSize && py>ppY2-pSize && py-_size<ppY2) paddleBounce(isColliding(boundZ),pdX2,pdY2,boundZ);
		//ball goes through end zone
		else {
			_isOut=1;
			goalScored=2;
		}
	}
	// std::cout<<px<<", "<<boundX<<", "<<py<<", "<<boundY<<"\n";
	//check if ball hits any wall
	if (px<=-boundX) wallBounce(1, boundX, boundY);
	else if (px>=boundX) wallBounce(2,boundX,boundY);
	if (py>=boundY) wallBounce(3,boundX,boundY);
	else if (py<=-boundY) wallBounce(4,boundX,boundY);
	// std::cout<<_isBouncing;
	if (!_isBouncing) {
		// std::cout<<_speed;
		_position[0]+=_direction[0]*_speed;
		_position[1]+=_direction[1]*_speed;
		_position[2]+=_direction[2]*_speed;
	}

	return goalScored;
}

//END PONGBALL STUFF


//BEGIN GAME STUFF
Game::Game(float boundX, float boundY, float boundZ, float padSize, float padSpeed, float ballSize, float ballSpeed, int maxScore, bool highDifficulty) {//CONSTRUCTOR
	
	//set values for game
	_boundaryX=boundX;
	_boundaryY=boundY;
	_boundaryZ=boundZ;

	_padSize=padSize;
	_padSpeed=padSpeed;
	_ballSize=ballSize;
	_ballSpeed=ballSpeed;
	_highDifficulty=highDifficulty;

	_maxScore=maxScore;

	// startGame();
	_gameOver=0;
	_started=0;
	_startDrawing=0;
	// std::cout<< _gameOver <<" wtf\n";
}

//destructor to ensure dynamically allocated memory is freed
Game::~Game() {
	delete _cpu;
	delete _player;
	delete _gameball;
}

void Game::set_difficulty(bool dif) {//change difficulty
	_highDifficulty=dif;
}

bool Game::gameStarted() {
	return _started;
}

bool Game::canDraw() {
	return _startDrawing;
}

void Game::playerControl(bool upPressed, bool downPressed, bool leftPressed, bool rightPressed) {
	// std::cout<<upPressed<<downPressed<<leftPressed<<rightPressed<<"\n";
	float x, y;
	float speed = _player->get_maxSpeed();
	if (!(upPressed && downPressed)) { //nothing happens if up and down pressed at same time
		if (upPressed) y=speed;
		else if (downPressed) y=-speed;
	}
	if (!(leftPressed && rightPressed)) { //nothing happens if left and right pressed at same time
		if (leftPressed) x=-speed;
		else if (rightPressed) x=speed;
	}
	_player->set_direction(x,y); //change direction based on what keys are pressed
	std::cout<<" ";
	_player->update(_boundaryX,_boundaryY); //update position based on new directions
	_player->set_direction(0,0); //reset directions
}

void Game::cpuLogic() { //simple AI
	float cpuX = _cpu->get_positionX();
	float cpuY = _cpu->get_positionY();
	float speed = _cpu->get_maxSpeed();
	float size = _cpu->get_size();

	float ballX = _gameball->get_positionX();
	float ballY = _gameball->get_positionY();
	float ballSize = _gameball->get_size();

	float moveX=0;
	float moveY=0;

	if (_gameball->isIncoming()) {//ball is incoming, time for cpu to move.. otherwise cpu remains still
		if (cpuX>=ballX+ballSize) moveX=-speed; //move left if ball is to the left
		else if (cpuX+size<=ballX) moveX=speed; //move right if ball is to the right
		if (cpuY<=ballY-ballSize) moveY=speed; //move up " "
		else if (cpuY-size>=ballY) moveY=-speed; //move down " "
	}

	_cpu->set_direction(moveX,moveY);
	_cpu->update(_boundaryX,_boundaryY); //update position, then reset directions
	_cpu->set_direction(0,0);
}

void Game::startGame() {

	_startDrawing = 1; //objects can be safely drawn
	if (_gameOver) {//stopped from previous game, clear the objects 
		_gameOver=0;
		delete _gameball;
		delete _player;
		delete _cpu;
	}

	if(_highDifficulty) _cpu = new Paddle(0,_padSpeed*1.5,_padSize,_boundaryZ); //high difficulty, cpu has greater speed
	else _cpu = new Paddle(0,_padSpeed*0.9,_padSize,_boundaryZ); //regular difficulty, cpu has lower speed
	_player = new Paddle(1,_padSpeed,_padSize,_boundaryZ);
	_gameball = new PongBall(_ballSpeed,_ballSize);

	//reset scores
	_playerScore=0;
	_cpuScore=0;
}

bool Game::gameOver() {
	return _gameOver;
}

void Game::pointScored(int ps) {
	if (ps==1) { //goal scored on player side
		_cpuScore++; 
	}
	else if (ps==2) { //goal scored on cpu side
		_playerScore++;
	}
	if (_maxScore==_cpuScore || _maxScore==_playerScore) { //max score reached by player or cpu
		_gameOver=1;
		_started=0;
		if (ps==2) _playerWin=1; //player recognized as winner
		else _playerWin=0; //player recognized as loser

	}
}

void Game::update() {
	if (_started) {
		cpuLogic(); //run AI

		//temporary values for convenience
		float pdX1=_player->get_directionX();
		float pdY1=_player->get_directionY();
		float ppX1=_player->get_positionX();
		float ppY1=_player->get_positionY();

		float pdX2=_cpu->get_directionX();
		float pdY2=_cpu->get_directionY();
		float ppX2=_cpu->get_positionX();
		float ppY2=_cpu->get_positionY();

		//update gameball position and return indicator of point score
		int ps=_gameball->update(_boundaryX,_boundaryY,_boundaryZ,pdX1,pdY1,ppX1,ppY1,pdX2,pdY2,ppX2,ppY2,_padSize);

		if (ps!=0) pointScored(ps); //if ps is not 0 then a point was scored so we call the pointScored function

	}
}

void Game::launch(float x, float y) {
	_started=1;//let the games begin
	_gameball->set_direction(x,y,-0.5); //set direction of ball
}