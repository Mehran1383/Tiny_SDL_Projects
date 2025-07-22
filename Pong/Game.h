#include "SDL3/SDL.h"

#include <vector>

struct Vector2
{
	float x;
	float y;
};

struct Ball
{
	Vector2 position;
	Vector2 velocity;
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

	Uint32 mTicksCount;
	bool mIsRunning;
	int mPaddle1Dir;
	int mPaddle2Dir;
	Vector2 mPaddle1Pos;
	Vector2 mPaddle2Pos;
	std::vector<Ball> balls;
};
