/*
This is a Visualization program how neurons learning/training.
by: KriszAime
*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <SFML/Graphics.h>
#include <SFML/Graphics/RenderWindow.h>
#include "NeuralNet.h"


//const
#define Refresh_FPS 30.0 //period: 33.33ms
#define Window_height 600
#define Window_width 800
//


//strc
typedef struct Window
{
	sfVideoMode mode;
	sfRenderWindow* render;
	sfEvent events;
	sfSprite* sprite; //drawable to render
	sfTexture* texture; //will be linked to sprite
	sfImage* image; //will be copy to texture
	sfIntRect canvas_bounds;
	sfClock *RenderClock;
}Window;

//


//-Functions
float lerp(float a, float b, float f)
{
	return (a * (1.0f - f)) + (b * f);
}
//-



void main(void)
{
	//-Setup Main Window
	Window Main_Window =
	{
		.mode = {.height = Window_height, .width = Window_width, .bitsPerPixel = 32 },
		.image = sfImage_createFromColor(Window_width,Window_height, sfBlack),
		.sprite = sfSprite_create(),
		.canvas_bounds = { .top = 0, .left = 0, .width = Window_width, .height = Window_height },
	};
	Main_Window.texture = sfTexture_createFromImage(Main_Window.image, &Main_Window.canvas_bounds);
	sfSprite_setTexture(Main_Window.sprite, Main_Window.texture, sfFalse); //sprite <= texture
	//-


	//Setup Neuro Network
	NeuralNet Nnet;
	//todo input from user + disclaimer
	//

	//Create Neuro Network (test for now)
	float TestInput[4][2] = { {0, 0},  {1, 0},  {0, 1},  {1, 1} };
	float TestTarget[4][1] = { {0},		{1},	{1},	{0} };
	NeuralNet_Construct(&Nnet, 2u, 2u, 1u, 0.5f, 0.9f, 0.5f, NNtypeRandom);
	for (int i = 0; i < 4;i++)NeuralNet_addTrainingPattern(&Nnet, TestInput[i], TestTarget[i]);
	//


	//Create Main Window
	if ((Main_Window.render = sfRenderWindow_create(Main_Window.mode, "Neuro Visualize V0.1a by: KriszAime", sfClose, NULL)) == NULL) return;
	//

	//-Start Clock
	Main_Window.RenderClock = sfClock_create();
	//-

	//Main refresh loop
	while (sfRenderWindow_isOpen(Main_Window.render))
	{
		//handling events
		while (sfRenderWindow_pollEvent(Main_Window.render, &Main_Window.events))
		{
			if (Main_Window.events.type == sfEvtClosed)
			{
				sfRenderWindow_close(Main_Window.render);
			}
			if (Main_Window.events.type == sfEvtMouseButtonPressed)
			{
				switch (Main_Window.events.mouseButton.button)
				{
				case sfMouseLeft: break;
				case sfMouseRight: /* this is where new point method goes */ break;
				default:
					break;
				}
			}
		}
		//


		//-Draw Neuron Activity
		for (size_t i = 0; i < Window_width; i++)
		{
			//-Clear
			for (size_t y = 0; y < Window_height; y++) sfImage_setPixel(Main_Window.image, i, y, sfBlack);
			//-

			//- Draw Output Activity
			
			//-

			//- Draw Training Points

			//-
			
		}
		//-
		
		//-Update render target
		sfTexture_updateFromImage(Main_Window.texture, Main_Window.image, 0, 0);
		sfRenderWindow_drawSprite(Main_Window.render, Main_Window.sprite, NULL); //draw render <= sprite
		sfRenderWindow_display(Main_Window.render); //update render window
		//-

		//-Target Fps
		sfInt64 Delta = sfTime_asMicroseconds(sfClock_getElapsedTime(Main_Window.RenderClock));
		if (Delta < ((1. / Refresh_FPS) * 1000 * 1000)) sfSleep((sfTime){ .microseconds = ((1. / Refresh_FPS) * 1000. * 1000.) - (double)Delta});
		sfClock_restart(Main_Window.RenderClock);
		
		//-
	}
	//

	//-free memory

	//-
	return;
}