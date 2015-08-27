
#ifndef UTILITY_H
#define UTILITY_H

//  The number of frames
int frameCount = 0;
float fps = 0;
int currentTime = 0;
int previousTime = 0;

GLvoid *font_style = GLUT_BITMAP_TIMES_ROMAN_24;

void printw(float x, float y, float z, char* format, ...)
{
	
	va_list args;	//  Variable argument list
	int len;		//	String length
	int i;			//  Iterator
	char * text;	//	Text

	//  Initialize a variable argument list
	va_start(args, format);
	
	//  Return the number of characters in the string referenced the list of arguments.
	//  _vscprintf doesn't count terminating '\0' (that's why +1)
	len = _vscprintf(format, args) + 1;

	//  Allocate memory for a string of the specified size
	text = (char *)malloc(len * sizeof(char));

	//  Write formatted output using a pointer to the list of arguments
	vsprintf_s(text, len, format, args);
	
	//  End using variable argument list 
	va_end(args);

	//  Specify the raster position for pixel operations.
	glRasterPos3f(x, y, z);
	
	//  Draw the characters one by one
	//for (i = 0; text[i] != '\0'; i++)
		//glutBitmapCharacter(font_style, text[i]);
	
	//  Free the allocated memory for the string
	free(text);
}

void drawFPS()
{

	glLoadIdentity();

	//printw(-0.9, -0.9, 0, "FPS: %4.2f", fps);
}

void calculateFPS()
{

	frameCount++;

	currentTime = SDL_GetTicks();

	int timeInterval = currentTime - previousTime;

	if (timeInterval > 1000)
	{
		fps = frameCount / (timeInterval / 1000.0f);

		previousTime = currentTime;
		std::cout << fps << std::endl;
		frameCount = 0;
	}
}

#endif