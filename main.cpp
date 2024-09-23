#include <iostream>
#include <fstream>
#include <GL/glut.h>
#include <cmath>

using namespace std;

// function declarations
void init();
void display();
void plane();
void box();
void displayScores();
void loadHighScore();
void saveHighScore();
void gameReset();
void displaystartscreen(float time);

void keyboard(unsigned char key, int x, int y);
void specialKeyDown(int key, int x, int y);
void specialKeyUp(int key, int x, int y);

//Our global variables
std::string scoring;
int score = 0;							// Current score
int highScore = 0;						// Highest recorded score
bool gameOver = false;					// Track if the game is over
bool gameStarted = false;				// Track if the game has started
bool keyState[256] = {false};			// Tracks the state of regular keys
bool specialKeyState[256] = {false};	// Tracks the state of special keys

//High score file name
const std::string highScoreFile = "highscore.txt";

// Variables for level speed
int level = 2;							// 1: beginner, 2: intermediate, 3: advanced 4: impossible
float speed = 2.0;						// Default speed for medium

//plane and box position variables
int px = 0, py = 0;						//Plane position
int bx = 0, by = 0;						//Box position

void init() {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glColor3f(0.0f, 0.0f, 0.0f);
    glPointSize(4.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 600.0, 0.0, 400.0);

    loadHighScore();
}

//Load high score from our file
void loadHighScore() {
    std::ifstream inputFile(highScoreFile);
    if (inputFile.is_open()) {
        inputFile >> highScore;
        inputFile.close();
    } else {
        highScore = 0;
    }
}

//Save high score to our file
void saveHighScore() {
    std::ofstream outputFile(highScoreFile);
    if (outputFile.is_open()) {
        outputFile << highScore;
        outputFile.flush();
        outputFile.close();
    }
}

//Used for handling key presses for level selection and game controls
void keyDown(unsigned char key, int x, int y) {
    keyState[key] = true;

   // Check if the game hasn't started, and handle level selection
    if (!gameStarted) {
        if (key == '1'){			// Easy level
            level = 1;
            speed = 1.0;
            gameStarted = true;
            gameOver = false;
        } 
		else if (key == '2'){		// Medium level
            level = 2;
            speed = 2.0;
            gameStarted = true;
            gameOver = false;
        } 
		else if (key == '3'){		// Hard level
            level = 3;
            speed = 3.0;
            gameStarted = true;
            gameOver = false;
        } 
		else if (key == '4'){		// Impossible level
            level = 4;
            speed = 4.0;
            gameStarted = true;
            gameOver = false;
        }
    }

	// For handling game over screen controls
	// Loading the level page
	if (key == 'l' || key == 'L'){
        if (gameOver) {
            gameStarted = false; 
			gameOver = false;
			gameReset();
            glutPostRedisplay();        }
    }

	//Resetting the game
	if (key == 'r' || key == 'R') {
        if (gameOver)
            gameReset();
    }

	//Exiting the game
    if (key == 'x' || key == 'X') {
        exit(0); 
	}
}

//Handle key releases
void keyUp(unsigned char key, int x, int y) {
    keyState[key] = false;
}

//Handle special key presses
void specialKeyDown(int key, int x, int y) {
    specialKeyState[key] = true; 
}

//Handle special key releases
void specialKeyUp(int key, int x, int y) {
    specialKeyState[key] = false;
}

//For updating the plane movement based on key states
void updatePlaneMovement() {
    if (specialKeyState[GLUT_KEY_UP] || keyState['w'] || keyState['W']) {
        if (py + 340 < 400) {
            py += 4;
        }
    }
    if (specialKeyState[GLUT_KEY_DOWN] || keyState['s'] || keyState['S']) {
        if (py > -310) {
            py -= 4;
        }
    }
}

//For displaying the score, level name, high score and game over messages
void displayScores() {

    // Display the current score
    scoring = "Score: " + std::to_string(score);
    glColor3f(1, 0, 0);
    glRasterPos2f(10, 390);
    for (char c : scoring) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }

    // Display the current level
    std::string level_name = "init";
    
	if (level == 1)
        level_name = "Paper Pilot :)";
    else if (level == 2)
        level_name = "Origami Ace :)";
    else if (level == 3)
        level_name = "Wind Warrior :)";
    else if (level == 4)
        level_name = "Tornado Tamer :)";
    
    glColor3f(0.2, 0.5, 1); 
	glRasterPos2f(550, 380);
    for (char c : level_name) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }

    // Display the high score
    std::string highScoreText = "High Score: " + std::to_string(highScore);
    
	glColor3f(0, 0.6, 0.3); 
	glRasterPos2f(10, 370);
    for (char c : highScoreText) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }

    // Game over message
    if (gameOver) {
        glColor4f(0.9, 0.9, 0.9, 0.8);
        glBegin(GL_POLYGON);
            glVertex2f(100, 150);
            glVertex2f(100, 250);
            glVertex2f(550, 250);
            glVertex2f(550, 150);
        glEnd();

        // Display game over text
        std::string gameOverText = "Score: " + std::to_string(score) + " points. Press 'L' to choose a new level, 'R' to retry, or 'X' to exit the game.";
        
		glColor3f(0, 0, 1); 
		glRasterPos2f(170, 200);
        for (char c : gameOverText) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
        }
    }
    glFlush();
}

//Draw the player's plane
void plane() {
    glColor3f(0.747, 0.747, 0.747);
    glBegin(GL_POLYGON);
        glVertex2i(165 + px, 310 + py);
        glVertex2i(250 + px, 340 + py);
        glVertex2i(170 + px, 340 + py);
    glEnd();

	glColor3f (0.847,0.847,0.847);
	glBegin(GL_POLYGON);
		glVertex2i(160 + px, 330 + py);
		glVertex2i(250 + px, 340 + py);
		glVertex2i(170 + px, 340 + py);
    glEnd();
}

//Draw the obstacle aka box
void box() {
    int bxmax = 600 + bx; // Maximum x-coordinate of the box
	int bxmin = 550 + bx; // Minimum x-coordinate of the box
	int bymax = 200 + by; // Maximum y-coordinate of the box
	int bymin = 0 + by;   // Minimum y-coordinate of the box


    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
        glVertex2i(bxmin, bymin);
        glVertex2i(bxmax, bymin);
        glVertex2i(bxmax, bymax);
        glVertex2i(bxmin, bymax);
    glEnd();

	//Start moving the obstacles only if the game has been started
    if (!gameOver && gameStarted) { 
        bx -= speed; // moves the box to the left
		if (bx < -600) { //checks if the obstacle has moved past the left boundary of the screen
            bx = 0; //reset position to set at the right side of the screen
            score++; //increments score per avoided obstacle
            
			if (score > highScore) {
                highScore = score;
                saveHighScore();
            }
            by = rand() % 365; //sets different vertical position for each obstacle
        }

        // Collision detection
		// 250 + px > bxmin: if the right edge of plane is beyond the left edge of box
		// 165 + px < bxmax: if the left edge of plane is before the right edge of box
		//(340 + py) > bymin: if the top edge of plane is below the top edge of box
		//(310 + py) < bymax: if the bottom edge of plane is above the bottom edge of box.
        if (250 + px > bxmin && 165 + px < bxmax && (340 + py) > bymin && (310 + py) < bymax) {
            gameOver = true;  // Set game over state
        }
    }
}
   
void drawPaperPlane(float x, float y) {
     // Main body of the plane - light gray base
    glColor3f(0.8f, 0.8f, 0.8f);		// Light gray color
    glBegin(GL_TRIANGLES);
        glVertex2f(x, y);				// Tip of the plane
        glVertex2f(x - 15, y - 5);		// Left wing
        glVertex2f(x + 15, y - 5);		// Right wing
    glEnd();

    // Add a darker central fold to create a crease effect
    glColor3f(0.6f, 0.6f, 0.6f);		// Slightly darker gray
    glBegin(GL_TRIANGLES);
        glVertex2f(x, y);				// Tip of the plane
        glVertex2f(x - 8, y - 2.5);		// Center left
        glVertex2f(x + 8, y - 2.5);		// Center right
    glEnd();

    // Draw left wing fold for a more 3D look
    glColor3f(0.7f, 0.7f, 0.7f);		// Intermediate gray for the fold
    glBegin(GL_TRIANGLES);
        glVertex2f(x, y);				// Tip of the plane
        glVertex2f(x - 15, y - 5);		// End of left wing
        glVertex2f(x - 8, y - 2.5);		// Midpoint on the left
    glEnd();

    // Draw right wing fold for a more 3D look
    glColor3f(0.9f, 0.9f, 0.9f);		// Even lighter gray for highlights
    glBegin(GL_TRIANGLES);
        glVertex2f(x, y);				// Tip of the plane
        glVertex2f(x + 8, y - 2.5);		// Midpoint on the right
        glVertex2f(x + 15, y - 5);		// End of right wing
    glEnd();

    // Adding a tail fold to make it look more like an origami plane
    glColor3f(0.75f, 0.75f, 0.75f);		// Slightly darker tail fold
    glBegin(GL_TRIANGLES);
        glVertex2f(x - 2.5, y - 5);		// Left tail
        glVertex2f(x + 2.5, y - 5);		// Right tail
        glVertex2f(x, y - 10);			// Bottom of the tail fold
    glEnd();
}

void displayStartScreen(float time) {
    glClear(GL_COLOR_BUFFER_BIT);		// Clear the screen

    // Set title text
    glColor3f(0, 0, 1);
	float yPos = 350; 
	glRasterPos2f(230, yPos);
    std::string topText = "Flight of the Origami";
    
    for (char c : topText) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }

    // Set instructions text color and position
    glColor3f(0, 0, 1); 
	float yPosition = 250;
	glRasterPos2f(200, yPosition);
    
    std::string startText = "Press '1' for Paper Pilot (Beginner)\nPress '2' for Origami Ace (Intermediate)\nPress '3' for Wind Warrior (Advanced)\nPress '4' for Tornado Tamer (Impossible)";
    
    for (char c : startText) {
        if (c == '\n') {
            yPosition -= 30;				// Move down for next line
            glRasterPos2f(200, yPosition);	// Update the raster position
        } else {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
        }
    }

    // Animate three paper planes
    float plane1X = fmod(time * 7, 600); 
    float plane1Y = plane1X / 2;				// Moving from bottom-left to right-center

    float plane2X = fmod(time * 5, 300); 
    float plane2Y = 300 + (plane2X / 2);		// Moving from left-center to top-center
										
	float plane3X = 300 + fmod(time * 5, 600);
	float plane3Y = 400 - (plane3X / 2);		//Moves from top center to right center


    drawPaperPlane(plane1X, plane1Y);
    drawPaperPlane(plane2X, plane2Y);
	drawPaperPlane(plane3X, plane3Y);


    glFlush();
}

// Main display function
void display() {
	static float time = 0.0f;					// Static to maintain its value across calls
    time += 0.1f;								// Increment time for animation
	
    glClear(GL_COLOR_BUFFER_BIT);

    if (!gameStarted) {
        displayStartScreen(time);				// Show start screen
    } else {
        if (!gameOver) {
            updatePlaneMovement();
            plane();
            box();
        }
        displayScores();
    }

    glFlush();
}

// Resets the game state to the starting conditions
void gameReset() {
    score = 0;
    bx = 0;
    by = 0;
	px = 0;
    py = 0;
    gameOver = false;
    glutPostRedisplay();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 400);
    glutInitWindowPosition(100, 150);
    glutCreateWindow("Paper Plane Game");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutSpecialFunc(specialKeyDown);
    glutSpecialUpFunc(specialKeyUp);
    glutIdleFunc(display);
    glutMainLoop();

    return 0;
}

