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
	sfClock* RenderClock;
}Window;

typedef struct NeuralControl
{
	sfBool CanBeStarted, IsEpochRunning;
	uint EpochPerFrame;
}NeuralControl;
//


void main(void)
{
	//-Setup Main Window
	Window Main_Window =
	{
		.mode = {.height = Window_height, .width = Window_width, .bitsPerPixel = 32 },
		.image = sfImage_createFromColor(Window_width,Window_height, sfBlack),
		.sprite = sfSprite_create(),
		.canvas_bounds = {.top = 0, .left = 0, .width = Window_width, .height = Window_height },
	};
	Main_Window.texture = sfTexture_createFromImage(Main_Window.image, &Main_Window.canvas_bounds);
	sfSprite_setTexture(Main_Window.sprite, Main_Window.texture, sfFalse); //sprite <= texture
	//-


	//Get User Input Settings
	NeuralControl Ctrl =
	{
		.CanBeStarted = sfFalse,
		.IsEpochRunning = sfFalse,
		.EpochPerFrame = 1,
	};
	//todo input from user + disclaimer
	//

	//Create Neuro Network
	NeuralNet* Nnet = NeuralNet_create(1u, 16u, 1u, 0.5, 0.9, 0.5, 0.0);
	//


	//Create Main Window
	if ((Main_Window.render = sfRenderWindow_create(Main_Window.mode, "Neuro Visualize V0.7a by: KriszAime", sfClose, NULL)) == NULL) return;
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
				case sfMouseRight: 
				{
					double input, output;
					sfVector2i pos = sfMouse_getPosition((sfWindow *)Main_Window.render);
					input = (double)pos.x / Window_width;
					output = (double)pos.y / Window_height;
					//- Create new Pattern Point
					NeuralNet_addTrainingPattern(Nnet, &input, &output);
					//-
					//-Enable Epoch
					Ctrl.CanBeStarted = sfTrue;
					//-
				} break;
				default:
					break;
				}
			}
			if (Main_Window.events.type == sfEvtKeyPressed)
			{
				switch (Main_Window.events.key.code)
				{
				case sfKeyF1: if (Ctrl.CanBeStarted == sfTrue)NeuralNet_doEpoch(Nnet, 1); printf("epoch: 1\r\n"); break; //do once learning
				case sfKeyF2: if (Ctrl.CanBeStarted == sfTrue)NeuralNet_doEpoch(Nnet, 10); printf("epoch: 10\r\n"); break; //do once learning
				case sfKeyF3: if (Ctrl.CanBeStarted == sfTrue) { Ctrl.IsEpochRunning = sfTrue; printf("Start\r\n"); } break; //start frame / epoch
				case sfKeyF4: Ctrl.IsEpochRunning = sfFalse; printf("Stop\r\n"); break; //stop frame / epoch
				
				case sfKeyF5: Ctrl.EpochPerFrame++; printf("Epoch/frame: %u\r\n", Ctrl.EpochPerFrame); break;
				case sfKeyF6: if (Ctrl.EpochPerFrame > 1) { Ctrl.EpochPerFrame--; printf("Epoch/frame: %u\r\n", Ctrl.EpochPerFrame); } break;
				case sfKeyF7: Ctrl.EpochPerFrame+=100; printf("Epoch/frame: %u\r\n", Ctrl.EpochPerFrame); break;
				case sfKeyF8: if (Ctrl.EpochPerFrame > 100) { Ctrl.EpochPerFrame-=100; printf("Epoch/frame: %u\r\n", Ctrl.EpochPerFrame); } break;
				
				case sfKeyF9: NeuralNet_reRandomizeWeights(Nnet); break; //amnesia :p
				case sfKeyF10: 
				{
					//
					Ctrl.IsEpochRunning = sfFalse;
					Ctrl.CanBeStarted = sfFalse;
					//
					NeuralNet_destroy(Nnet);
					Nnet = NeuralNet_create(1u, 100u, 1u, 0.5, 0.9, 0.5, 0.0);
					if(Nnet == NULL) sfRenderWindow_close(Main_Window.render);
					//
				}break; //rebirth :p
				case sfKeyEscape: sfRenderWindow_close(Main_Window.render); break;
				default:
					break;
				}
			}
		}
		//

		//-Doing Epoch / frame
		if (Ctrl.IsEpochRunning == sfTrue)NeuralNet_doEpoch(Nnet, Ctrl.EpochPerFrame);
		//-

		//-Draw Neuron Activity
		for (size_t i = 0; i < Window_width; i++)
		{
			double input, output;

			//-Clear
			for (size_t y = 0; y < Window_height; y++) sfImage_setPixel(Main_Window.image, i, y, sfBlack);
			//-

			//-Get Output Activity
			input = (double)i / (double)Window_width;
			NeuralNet_getOutputActivation(Nnet, &input, &output);
			//-

			//- Draw Output Activity
			if((output < 1.) && (output >= 0.)) 
				sfImage_setPixel(Main_Window.image, i, (uint)(output * (double)Window_height), sfGreen);
			//-

		}
		//- Draw Training Points
		for (size_t t = 0; t < Nnet->Patterns.PatternCount; t++)
		{
			sfVector2i pos;
			pos.x = (int)(Nnet->Patterns.Inputs[t][0] * Window_width);
			pos.y = (int)(Nnet->Patterns.Targets[t][0] * Window_height);
			sfImage_setPixel(Main_Window.image, pos.x, pos.y, sfRed);
		}
		//-
		//-

		//-Update render target
		sfTexture_updateFromImage(Main_Window.texture, Main_Window.image, 0, 0);
		sfRenderWindow_drawSprite(Main_Window.render, Main_Window.sprite, NULL); //draw render <= sprite
		sfRenderWindow_display(Main_Window.render); //update render window
		//-

		//-Target Fps
		sfInt64 Delta = sfTime_asMicroseconds(sfClock_getElapsedTime(Main_Window.RenderClock));
		if (Delta < ((1. / Refresh_FPS) * 1000 * 1000)) sfSleep((sfTime) { .microseconds = (sfInt64)((1. / Refresh_FPS) * 1000. * 1000.) - Delta });
		sfClock_restart(Main_Window.RenderClock);

		//-
	}
	//

	//-free memory
	NeuralNet_destroy(Nnet);
	sfRenderWindow_destroy(Main_Window.render);
	sfImage_destroy(Main_Window.image);
	sfTexture_destroy(Main_Window.texture);
	sfSprite_destroy(Main_Window.sprite);
	sfClock_destroy(Main_Window.RenderClock);
	//-
	return;
}