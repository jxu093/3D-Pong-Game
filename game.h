class Paddle { //CLASS FOR THE "PADDLE"
private: //self explanatory variables
	float _position[3];
	float _direction[2];
	float _size;
	float _maxSpeed;
public:
	Paddle(bool isHuman, float Speed, float size, float boundaryZ); //constructor
	float get_positionX (); float get_positionY (); float get_positionZ (); //getter for position variables
	float get_directionX(); float get_directionY(); //getter for direction variables
	float get_size (); //get size
	float get_maxSpeed ();
	void set_direction (float x, float y); //set the x/y direction. No need for z for paddle
	void update(float boundX, float boundY); //update function to move paddle
};

class PongBall { //CLASS FOR THE PONGBALL AKA SPONGE
private:
	float _position[3];
	float _direction[3];
	float _speed;
	float _size;
	bool _isBouncing; //whether ball is bouncing off a wall -taken into consideration for when updating direction
	bool _isOut; //whether ball went out of game stage, in other words it was scored in someone's goal
	void wallBounce(int bounceDirection, float boundX, float boundY); //function to update direction when bouncing off wall
	void paddleBounce(int bounceDirection, float pdX, float pdY, float boundZ); //update direction when bouncing off player or cpu
public:
	PongBall(float speed, float size); //constructor
	bool isOut(); //retrieve _isOut value
	int isColliding(float boundZ); //whether or not the ball is entering a player/cpu's goal zone (1 for player 2 for cpu)
	bool isIncoming(); //ball is heading towards cpu
	float get_positionX(); float get_positionY(); float get_positionZ(); //retrieve position values
	// float get_directionX(); float get_directionY();
	float get_size();
	void set_direction(float x, float y, float z);
	int update(float boundX, float boundY, float boundZ, float pdX1, float pdY1, float ppX1, float ppY1, float pdX2, float pdY2, float ppX2, float ppY2, float pSize); //update ball position
};

class Game { //GAME CLASS
private:
	bool _started; //game started
	bool _startDrawing; //objects ready to be drawn
	bool _gameOver; //game over

	int _maxScore; //what score game ends at
	int _playerScore, _cpuScore; //respective scores for player and cpu
	float _boundaryX, _boundaryY, _boundaryZ; //boundaries for game stage

	float _padSize, _padSpeed, _ballSize, _ballSpeed; //paddle size and speed, pong ball size and speed
	bool _highDifficulty; //whether or not the game is set to high difficulty

	void pointScored(int ps); //function to adjust game state after point scored
	void cpuLogic(); //simple AI for CPU movement
public:
	//pointers for game objects allowing for mutability
	Paddle* _cpu; //cpu paddle object
	Paddle* _player; //player paddle object
	PongBall* _gameball; //pong ball object
	bool _playerWin; //whether victory belongs to player or cpu
	Game(float boundX, float boundY, float boundZ, float padSize, float padSpeed, float ballSize, float ballSpeed, int maxScore, bool highDifficulty);//constructor
	~Game();//destructor
	void set_difficulty(bool dif);//change difficulty
	bool gameOver();//whether game is over
	bool gameStarted();//whether game has started
	bool canDraw();//whether objects can be drawn without errors
	void playerControl(bool upPressed, bool downPressed, bool leftPressed, bool rightPressed); //function to move player piece based on keyboard input
	void startGame(); //function to start a new game
	void launch(float x, float y); //launch the ball
	void update(); //UPDATE THE GAME


};