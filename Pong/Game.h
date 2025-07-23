#include "SDL3/SDL.h"

struct Vector2
{
	float x;
	float y;
};

class Game
{
public:
	Game();

	bool Initialize();
	void RunLoop();
	void Shutdown();

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;

	Uint64 mTicksCount;
	bool mIsRunning;
	int mPaddle1Dir;
	int mPaddle2Dir;
	Vector2 mPaddle1Pos;
	Vector2 mPaddle2Pos;
	Vector2 mBallPos;
	Vector2 mBallVel;
};
