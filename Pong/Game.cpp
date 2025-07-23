#include "Game.h"

const int thickness = 15;
const float paddleH = 100.0f;
const int width = 1024;
const int height = 768;
const float paddelVel = 300.0f;
const float ballAcc = 2.0f;
const float maxSpeed = 500.0f;

Game::Game()
:mWindow(nullptr)
,mRenderer(nullptr)
,mTicksCount(0)
,mIsRunning(true)
,mPaddle1Dir(0)
,mPaddle2Dir(0)
{
	
}

bool Game::Initialize()
{	
	mWindow = SDL_CreateWindow("Pong", width, height, 0);

	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}
	
	mRenderer = SDL_CreateRenderer(mWindow, NULL);

	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}
	
	mPaddle1Pos.x = 10.0f;
	mPaddle1Pos.y = height / 2.0f;
	mPaddle2Pos.x = width - 10.0f - thickness;
	mPaddle2Pos.y = height / 2.0f;

	mBallPos.x = width/2.0f;
	mBallPos.y = height/2.0f;
	mBallVel.x = -200.0f;
	mBallVel.y = 235.0f;
	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_EVENT_QUIT:
				mIsRunning = false;
				break;
		}
	}
	
	const bool* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}
	
	// Update paddle1 direction based on W/S keys
	mPaddle1Dir = 0;
	if (state[SDL_SCANCODE_W])
	{
		mPaddle1Dir -= 1;
	}
	if (state[SDL_SCANCODE_S])
	{
		mPaddle1Dir += 1;
	}

	// Update paddle2 direction based on I/K keys
	mPaddle2Dir = 0;
	if (state[SDL_SCANCODE_I])
	{
		mPaddle2Dir -= 1;
	}
	if (state[SDL_SCANCODE_K])
	{
		mPaddle2Dir += 1;
	}
}

void Game::UpdateGame()
{
	// Delta time is the difference in ticks from last frame
	// (converted to seconds)
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	
	// Clamp maximum delta time value
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	// Update tick counts (for next frame)
	mTicksCount = SDL_GetTicks();
	
	if (mPaddle1Dir != 0)
	{
		mPaddle1Pos.y += mPaddle1Dir * paddelVel * deltaTime;
		if (mPaddle1Pos.y < (paddleH/2.0f + thickness))
		{
			mPaddle1Pos.y = paddleH/2.0f + thickness;
		}
		else if (mPaddle1Pos.y > (height - paddleH/2.0f - thickness))
		{
			mPaddle1Pos.y = height - paddleH/2.0f - thickness;
		}
	}

	if (mPaddle2Dir != 0)
	{
		mPaddle2Pos.y += mPaddle2Dir * paddelVel * deltaTime;
		if (mPaddle2Pos.y < (paddleH / 2.0f + thickness))
		{
			mPaddle2Pos.y = paddleH / 2.0f + thickness;
		}
		else if (mPaddle2Pos.y > (height - paddleH / 2.0f - thickness))
		{
			mPaddle2Pos.y = height - paddleH / 2.0f - thickness;
		}
	}
	
	// Update ball position based on ball velocity
	mBallPos.x += mBallVel.x * deltaTime;
	mBallPos.y += mBallVel.y * deltaTime;
	
	// Bounce if needed
	float diff1 = mPaddle1Pos.y - mBallPos.y;
	float diff2 = mPaddle2Pos.y - mBallPos.y;
	diff1 = (diff1 > 0.0f) ? diff1 : -diff1;
	diff2 = (diff2 > 0.0f) ? diff2 : -diff2;

	if ((diff1 <= paddleH / 2.0f && mBallPos.x <= 25.0f && mBallPos.x >= 15.0f && mBallVel.x < 0.0f) ||
		(diff2 <= paddleH / 2.0f && mBallPos.x >= width - 25.0f && mBallPos.x <= width - 15.0f && mBallVel.x > 0.0f))
	{
		mBallVel.x *= -1.0f;
	}	
	else if (mBallPos.x <= 0.0f || mBallPos.x >= width) 
	{
		mIsRunning = false;
	}
	
	if (mBallPos.y <= thickness && mBallVel.y < 0.0f)
	{
		mBallVel.y *= -1;
	}
	else if (mBallPos.y >= (height - thickness) && mBallVel.y > 0.0f)
	{
		mBallVel.y *= -1;
	}

	if (fabs(mBallVel.x) < maxSpeed && fabs(mBallVel.y) < maxSpeed)
	{
		if (mBallVel.x > 0)
			mBallVel.x += ballAcc * deltaTime;
		else
			mBallVel.x -= ballAcc * deltaTime;

		if (mBallVel.y > 0)
			mBallVel.y += ballAcc * deltaTime;
		else
			mBallVel.y -= ballAcc * deltaTime;
	}	
}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, 255);
	
	// Clear back buffer
	SDL_RenderClear(mRenderer);

	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	
	// Draw top wall
	SDL_FRect wall{0, 0, width, thickness};
	SDL_RenderFillRect(mRenderer, &wall);
	
	// Draw bottom wall
	wall.y = height - thickness;
	SDL_RenderFillRect(mRenderer, &wall);
		
	// Draw paddle1
	SDL_FRect paddle1{
		static_cast<int>(mPaddle1Pos.x),
		static_cast<int>(mPaddle1Pos.y - paddleH/2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddle1);

	// Draw paddle2
	SDL_FRect paddle2{
		static_cast<int>(mPaddle2Pos.x),
		static_cast<int>(mPaddle2Pos.y - paddleH / 2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddle2);
	
	// Draw ball
	SDL_FRect ball{	
		static_cast<int>(mBallPos.x - thickness/2),
		static_cast<int>(mBallPos.y - thickness/2),
		thickness,
		thickness
	};
	SDL_RenderFillRect(mRenderer, &ball);
	
	// Swap front buffer and back buffer
	SDL_RenderPresent(mRenderer);
}

void Game::Shutdown()
{
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}
