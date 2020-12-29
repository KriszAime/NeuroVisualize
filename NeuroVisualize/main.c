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
#define Refresh_FPS 60.0f //period: 16.66ms
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
	
	//


	//Create Main Window
	if ((Main_Window.render = sfRenderWindow_create(Main_Window.mode, "Neuro Visualize V0.1a by: KriszAime", sfClose, NULL)) == NULL) return;
	//

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
	}
	//

	//-free memory

	//-
	return;
}