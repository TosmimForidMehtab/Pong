#include <iostream>
#include <SDL2/SDL.h>
#define WIDTH 800
#define HEIGHT 600
#define BALL_SIZE 20
#define PADDLE_SIZE 20
#define PADDLE_HEIGHT 100
#define SCORE_WIDTH 40
#define SCORE_HEIGHT 20
using namespace std;

SDL_Window* window;
SDL_Event e;
SDL_Renderer* renderer;
bool quit;
SDL_Rect playerPaddle, aiPaddle, ball, score, dest;
double bx, by, speedX, speedY;


void Reset()
{
    ball.x = (WIDTH/2) - (BALL_SIZE/2); // Width of window/2 - Size of ball/2
    ball.y = (HEIGHT/2) - (BALL_SIZE/2); // Height of window/2 - Size of ball/2

    // The value of ball.x & ball.y has to be in fraction to be able to move
    bx = ball.x;
    by = ball.y;

    // For random ball speed
    speedX = (rand() % 5) - 2; // Subtraction is done so that the ball can move in negative axis also.
    speedY = (rand() % 5) - 2; // Subtraction is done so that the ball can move in negative axis also.

    // Speed should not be zero, so changing it to random speed
    if(speedX >= -0.01 and speedX <= 0.01) speedX = 1.0;
    if(speedY >= -0.01 and speedY <= 0.01) speedY = -1.0;

    speedX /= 10.0;
    speedY /= 10.0;
}

void Restart()
{
    playerPaddle.x = PADDLE_SIZE;
    playerPaddle.y = HEIGHT/2 - PADDLE_HEIGHT/2;
    aiPaddle.x = WIDTH - PADDLE_SIZE - 20;
    aiPaddle.y = HEIGHT/2 - PADDLE_HEIGHT/2;
    score.x = (WIDTH/2) - SCORE_WIDTH/2;
    score.y = 0;
    Reset();
}

void Start()
{
    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow("PONG", 50, 50, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);

    //Player Paddle
    playerPaddle.x = PADDLE_SIZE;
    playerPaddle.y = HEIGHT/2 - PADDLE_HEIGHT/2; // Height of window/2 - Height of paddle/2
    playerPaddle.w = PADDLE_SIZE;
    playerPaddle.h = PADDLE_HEIGHT;

    //AI paddle
    aiPaddle.x = WIDTH - PADDLE_SIZE - 20; // Width of window - width of paddle - gap
    aiPaddle.y = HEIGHT/2 - PADDLE_HEIGHT/2; // Height of window/2 - Height of paddle/2
    aiPaddle.w = PADDLE_SIZE;
    aiPaddle.h = PADDLE_HEIGHT;

    // Ball
    Reset();
    ball.w = ball.h = BALL_SIZE;

    // Score
    score.x = (WIDTH/2) - SCORE_WIDTH/2; // Screen width/2 - score width/2
    score.y = 0;
    score.w = SCORE_WIDTH;
    score.h = SCORE_HEIGHT;
}

void Event()
{
    while (SDL_PollEvent(&e)) // Pressing the close button closes the window
    {
        if(e.type == SDL_QUIT)
        {
            quit = true;
        }

        else if(e.type == SDL_KEYDOWN)
        {
            if(e.key.keysym.sym == SDLK_UP)
            {
                playerPaddle.y -= 50;
            }
            else if(e.key.keysym.sym == SDLK_DOWN)
            {
                playerPaddle.y += 50;
            }

            // Restart Logic
            else if(e.key.keysym.sym == SDLK_r)
            {
                Restart();
            }


            if(playerPaddle.y < 0) playerPaddle.y = 0;
            else if(playerPaddle.y > HEIGHT - playerPaddle.h)
                playerPaddle.y = HEIGHT - playerPaddle.h;

        }

        else if(e.type == SDL_MOUSEMOTION)
        {
            SDL_GetMouseState(&playerPaddle.x, &playerPaddle.y);
            playerPaddle.x = PADDLE_SIZE; // So that the paddle does not change its x
            //position with mouse
            playerPaddle.y -= playerPaddle.h/2; // ??? ep4
            if(playerPaddle.y < 0) playerPaddle.y = 0;
            else if(playerPaddle.y > HEIGHT - playerPaddle.h)
                playerPaddle.y = HEIGHT - playerPaddle.h;
        }
    }
}


void Update()
{
    bx += speedX;
    by += speedY;

    // Collision detection
    //LEFT
    if(bx <= 0.0)
    {
        bx = 0.0;
        //Reversing the direction after collision
        speedX *= -1.0;
        score.x += 25;

        // Resetting
        Reset();
    }
    //RIGHT
    if(bx >= WIDTH*1.0 - ball.w)
    {
        bx = WIDTH*1.0-ball.w;
        speedX *= -1.0;
        score.x -= 25;

        // Resetting
        Reset();
    }
    //TOP
    if(by <= 0.0)
    {
        by = 0.0;
        speedY *= -1.0;
    }
    //BOTTOM
    if(by >= HEIGHT*1.0 - ball.h)
    {
        by = HEIGHT*1.0 - ball.h;
        speedY *= -1.0;
    }

    // AIPaddle
    if(aiPaddle.y + (aiPaddle.h/2) > ball.y+ (ball.h/2) && rand()% 10 == 0)
    {
        aiPaddle.y--;
    }
    else if(aiPaddle.y + (aiPaddle.h/2) < ball.y+ (ball.h/2) && rand()% 10 == 0)
    {
        aiPaddle.y++;
    }

    if(aiPaddle.y < 0) aiPaddle.y = 0;
    else if(aiPaddle.y > HEIGHT - aiPaddle.h)
        aiPaddle.y = HEIGHT - aiPaddle.h;

    //Player Paddle Collision
    if((ball.x >= playerPaddle.x && ball.x < playerPaddle.x + playerPaddle.w) &&
            (ball.y >= playerPaddle.y && ball.y < playerPaddle.y + playerPaddle.h))
    {
        ball.x = playerPaddle.x+ playerPaddle.w;
        speedX = abs(speedX);
    }

    //AI Paddle Collision
    if((ball.x + ball.w >= aiPaddle.x && ball.x + ball.w < aiPaddle.x + aiPaddle.w) &&
            (ball.y >= aiPaddle.y && ball.y < aiPaddle.y + aiPaddle.h))
    {
        ball.x = aiPaddle.x - ball.w;
        speedX = -abs(speedX);
    }

    // Score Boundary
    if(score.x >= WIDTH - score.w - aiPaddle.w)
    {
        score.x = WIDTH - score.w - aiPaddle.w;
        speedX = 0;
        speedY = 0;
    }
    if(score.x <= 20+ playerPaddle.w)
    {
        speedX = 0;
        speedY = 0;
        score.x = 20 + playerPaddle.w;
    }

    // Update the real ballValues so that Rendering does not get affeted
    ball.x = bx;
    ball.y = by;
}

void Render()
{
    SDL_SetRenderDrawColor(renderer,0, 0, 0, 255);
    SDL_RenderClear(renderer); // Clearing the pixels with the above color
    // Drawing in the window
    //Score
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    if(score.x >= 500)
    {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    }
    if(score.x <= 300)
    {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    }
    SDL_RenderFillRect(renderer, &score);
    //Other
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &playerPaddle);
    SDL_RenderFillRect(renderer, &aiPaddle);

    SDL_RenderFillRect(renderer, &ball);

    SDL_RenderPresent(renderer);// Presenting the modified pixels to the screen
}

void End()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
int main(int argc, char **argv)
{
    //Initialization
    Start();

    //Game-loop
    while(!quit)
    {
        //Event
        Event();

        //Update
        Update();

        //Render
        Render();

    }
    End();
    return 0;
}
