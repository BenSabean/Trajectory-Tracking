// first OpenVG program
// Anthony Starks (ajstarks@gmail.com)
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "VG/openvg.h"
#include "VG/vgu.h"
#include "fontinfo.h"
#include "shapes.h"
#include <string.h>
#include <pthread.h>

void setScreen(int* width, int* height) {
	init(width, height);	// Graphics initialization
}

void renderBg(int width, int height) {
	Background(135, 206, 235);//(sky blue)	
	Fill(96, 128, 56, 1);//(Green Grass)
	Rect(0, 0, width, height/5);
}

void renderTank(int width, int height, int x) {
	Translate(10, 75);
	Fill(44, 53, 57, 1);//Track color(gunmetal color)
	Stroke(0, 0, 0, 1);
        StrokeWidth(2);
        Ellipse(45, 10, 80, 30);//Tracks
	Fill(29, 33, 13, 1);//Army Green Color
	Stroke(0, 0, 0, 1);
	StrokeWidth(2);
	Rect(10, 10, 70, 30);//body(army green color)
	Rect(20, 35, 40, 25);//turret
	Rect(60, 45, 60, 10);//barrel
	Rect(5, 10, 80, 15);//Track Cover
}

void renderBoxes() {
	Fill(0, 0, 0, 1);
	Rect(1275, 770, 125, 30);//Y (Start upper far right)
	Rect(1125, 770, 125, 30);//X
	Rect(975, 770, 125, 30);//Angle
	Rect(825, 770, 125, 30);//Vi
	Rect(675, 770, 125, 30);//Start Pos
}

void renderText(char* y, char *x, char* deg, char* vi, char* pos) {
	Fill(255, 255, 255, 1);
	Text(1280, 775, y, SerifTypeface, 20);
	Text(1130, 775, x, SerifTypeface, 20);
	Text(980, 775, deg, SerifTypeface, 20);
	Text(830, 775, vi, SerifTypeface, 20);
	Text(680, 775, pos, SerifTypeface, 20);
}

int main() {

	int width, height;
	float x, y, v, d;
        int i = 0;
	char s[3];
	char yvalue[] = "300.50";
	//TEST
	char test[10];
	y = atof(yvalue);
	printf("%f\n", y);
	sprintf(test, "%f", y);
	printf("%s\n", test);
	//END OF TEST
	setScreen(&width, &height);
	printf("w: %d\nh: %d\n", width, height);
	while(i != 5) {
		char y[10] = "Y: ";
		char x[10] = "X: ";
    		char deg[10] = "Deg: ";
		char vi[10] = "Vi: ";
		char pos[10] = "Pos: ";
		strcat(y, yvalue);
		strcat(x, "20.9");
		strcat(deg, "45");
		strcat(vi, "10.9");
		strcat(pos, "0.0");
		Start(width, height);	// Start the picture
		renderBg(width, height);
		renderTank(width, height, 0);
		renderBoxes();
		renderText(y, x, deg, vi, pos);
		End();
		i++;
	}
	fgets(s, 2, stdin);	// look at the pic, end with [RETURN]
	finish();		// Graphics cleanup
	exit(0);
}

