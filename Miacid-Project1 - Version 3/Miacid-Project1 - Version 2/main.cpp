#pragma region Project Description and Comments

//	ARTIFICIAL INTELLIGENGE
//	ASSIGNMENT ONE - LOTUS BOARD GAME

//	DESCRIPTION:
//		-	Two to Four players participate in the game of Lotus
//		-	Players take turns choosing which piece to move
//		-	Pieces can be placed on top of other pieces to create stacks
//		-	The distance a piece can move is up to the size of the stack
//		-	The first player to get all of their pieces to the finish zone wins
//		-	There exists a special location which acts as a trampoline
//		-	It is possible to have two computer, artificially controlled, players as well
//		-	One A.I is state based; the other is rule based.

//	NOTES:
//		-	picopng.cpp used from http://members.gamedev.net/lode/projects/LodePNG/picopng.cpp
//		-	picopng.cpp originally created by Lode Vandevenne (copyright 2005 - 2008)
//		-	picopng.cpp license allows for any usage and modification
//		-	picopng.h, the PNG class, and other PNG functions created from scratch for our purposes

//	PLEASE READ ! IMPORTANT ! :
//		- There is an error with the game that will cause it to end with a debug assertion
//		- We have tried countlessly to fix the problem, yet it appears completely randomly for no apparent reason
//		- Please try to ignore the problem as it has little to do with the A.I. itself
//		- It has also occurred during playthroughs with regular players as well
//		- We are very sorry for the problem and wish it did not exist

//	GROUP NAME:
//		Miacid

//	AUTHORS:
//		Brent Shanks
//		Matthew Shelley

#pragma endregion

#pragma region Includes, Prototpes, Defines and Variables

// Include the Foundation
#include "foundation.h"

// Function Prototypes
int main(int argc, char** argv);
static bool Init();
static void Exit();
static void InitGame();
static void GameLoop();
static void RenderCallback();
static void ReshapeCallback();
static void IdleCallback();

// Disable the console window (uncomment to enable)
//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

// Defines
#define WINDOW_TITLE "Lotus Board Game - Miacid"
#define WINDOW_POSX 384
#define WINDOW_POSY 144
#define WINDOW_SIZEX 512
#define WINDOW_SIZEY 512

#define DEFAULT_SCENE SCENE_SELECT_PLAYERS

// Variables common to this file

HANDLE hThread; // for creating the thread
DWORD dwID;
DWORD dwRetVal = 0;

#pragma endregion

#pragma region Init and Exit

static bool Init()
{
	// Create handle for the update (input) thread
	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)GameLoop, NULL, 0, &dwID);

	return 1;
}

static void Exit()
{
	// Close the thread
	dwRetVal = WaitForSingleObject(hThread, 1);
	CloseHandle(hThread);
}

#pragma endregion

#pragma region Core Game Loop

// Load up the game...
static void InitGame()
{
	// Set the initial scene
	GameData()->SceneState = DEFAULT_SCENE;

	// Load Graphics
	GameData()->LoadingImage.decode("images/loading.png");
	
	GameData()->LotusBoard.decode("images/Lotus_Board.png");
	GameData()->SetBoardBackground(&(GameData()->LotusBoard));

	GameData()->LotusWhite.decode("images/unit_white.png");
	GameData()->LotusBlack.decode("images/unit_black.png");
	GameData()->LotusRed.decode("images/unit_red.png");
	GameData()->LotusBlue.decode("images/unit_blue.png");
	GameData()->SetBoardPieces(&(GameData()->LotusWhite), &(GameData()->LotusBlack),
		&(GameData()->LotusRed), &(GameData()->LotusBlue));

	GameData()->LotusNumbers[0].decode("images/numpcs_1.png");
	GameData()->LotusNumbers[1].decode("images/numpcs_2.png");
	GameData()->LotusNumbers[2].decode("images/numpcs_3.png");
	GameData()->LotusNumbers[3].decode("images/numpcs_4.png");
	GameData()->LotusNumbers[4].decode("images/numpcs_5.png");
	GameData()->LotusNumbers[5].decode("images/numpcs_6.png");
	GameData()->LotusNumbers[6].decode("images/numpcs_7.png");
	GameData()->LotusNumbers[7].decode("images/numpcs_8.png");
	GameData()->LotusNumbers[8].decode("images/numpcs_9.png");
	GameData()->LotusNumbers[9].decode("images/numpcs_10.png");
	GameData()->LotusNumbers[10].decode("images/numpcs_11.png");
	GameData()->LotusNumbers[11].decode("images/numpcs_12.png");
	GameData()->LotusNumbers[12].decode("images/numpcs_13.png");
	GameData()->LotusNumbers[13].decode("images/numpcs_14.png");
	GameData()->LotusNumbers[14].decode("images/numpcs_15.png");
	GameData()->LotusNumbers[15].decode("images/numpcs_16.png");
	GameData()->LotusNumbers[16].decode("images/numpcs_17.png");
	GameData()->LotusNumbers[17].decode("images/numpcs_18.png");
	GameData()->LotusNumbers[18].decode("images/numpcs_19.png");
	GameData()->LotusNumbers[19].decode("images/numpcs_20.png");
	GameData()->LotusNumbers[20].decode("images/numpcs_21.png");
	GameData()->LotusNumbers[21].decode("images/numpcs_22.png");
	GameData()->LotusNumbers[22].decode("images/numpcs_23.png");
	GameData()->LotusNumbers[23].decode("images/numpcs_24.png");
	GameData()->SetBoardNumbers(GameData()->LotusNumbers);

	// Player Select Screen
	GameData()->LotusFrontBoard.decode("images/Lotus_Front_Board.png");

	GameData()->LotusPlayers.decode("images/Players.png");
	
	GameData()->LotusPlayer1.decode("images/Player1-Type.png");
	GameData()->LotusPlayer2.decode("images/Player2-Type.png");
	GameData()->LotusPlayer3.decode("images/Player3-Type.png");
	GameData()->LotusPlayer4.decode("images/Player4-Type.png");
	GameData()->LotusFinished.decode("images/finished.png");
	GameData()->LotusAIRule.decode("images/ai-rule.png");
	GameData()->LotusAIState.decode("images/ai-state.png");
	GameData()->LotusHuman.decode("images/human.png");
	GameData()->LotusNotPlaying.decode("images/not_playing.png");
	GameData()->LotusTurn.decode("images/won.png");
	GameData()->LotusWinner.decode("images/turn.png");
	
	// Game has now finished loading
	GameData()->gLoaded = 1;
}

// The main loop that continues while the game is in action. Most everything happens here.
static void GameLoop()
{
	// Initialize the game
	InitGame();

	// Loop until we're told to quit
	while (true)
	{
		switch (GameData()->SceneState)
		{
			case SCENE_SELECT_PLAYERS:
				GameSelectPlayers();
				break;

			case SCENE_MAINBOARD:
				GameMainBoard();
				break;

			case SCENE_RESULTS:
				GameResults();
				break;

			default:
				break; // do nothing
		}

		if (!GameData()->gRunning) break; // shutdown
	}

	// Exit the game
	exit(0);
}

#pragma endregion

#pragma region User Input

// Retrieve and store input from the mouse (just concerned with left clicks)
static void MouseCallback(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		// Make the x,y position relative to the center 512x512 box (the rest is useless)
		float transx = (float)(glutGet(GLUT_WINDOW_WIDTH)-WINDOW_SIZEX)/2;
		float transy = (float)(glutGet(GLUT_WINDOW_HEIGHT)-WINDOW_SIZEY)/2;
		x -= (int)transx;
		y -= (int)transy;

		if (x >= 0 && x <= WINDOW_SIZEX && y >= 0 && y <= WINDOW_SIZEY)
			GameData()->SetLastClick(x, y);
		else
			GameData()->ResetLastClick();

		printf("X: %i; Y: %i\n", x, y);
	}
}

#pragma endregion

#pragma region Render Function

// The main rendering function; it draws everything
static void RenderCallback()
{
	if (!GameData()->gRunning) return; //don't draw anything (should be shutting down)
	
	// Clear Buffers
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	// Setup projection (viewing) matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));

	// Setup model view (object mode) matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST); // images will be drawn in the order they are given
	
	// Center everything drawn on the screen to a 512x512 box
	float transx = (float)(glutGet(GLUT_WINDOW_WIDTH)-WINDOW_SIZEX)/2;
	float transy = (float)(glutGet(GLUT_WINDOW_HEIGHT)-WINDOW_SIZEY)/2;
	glTranslatef(transx, -transy, 0);

	// Rendering code...
	if (!GameData()->gLoaded)
		RenderLoading();
	else
	{
		switch (GameData()->SceneState)
		{
			case SCENE_SELECT_PLAYERS:
				RenderSelectPlayers();
				break;

			case SCENE_MAINBOARD:
				RenderMainBoard();
				break;

			case SCENE_RESULTS:
				RenderResults();
				break;

			default:
				RenderLoading();
				break; //assume we are busy loading...
		}
	}

	// Swap buffers and draw the frame
	glutSwapBuffers();
}

#pragma endregion

#pragma region GLUT Callbacks

// Call this function when the window is resized
static void ReshapeCallback(int width, int height)
{
	glViewport(0, 0, width, height);
}

// Continuously redraw the screen when there is nothing else to do
static void IdleCallback()
{
	glutPostRedisplay();
}

#pragma endregion

#pragma region main()

// Boot up the demo
int main(int argc, char** argv)
{
	// Initialize glut
	glutInit(&argc, argv);
	glutInitWindowPosition(WINDOW_POSX, WINDOW_POSY);
	glutInitWindowSize(WINDOW_SIZEX, WINDOW_SIZEY);
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	int mainHandle = glutCreateWindow(WINDOW_TITLE);
	glutSetWindow(mainHandle);
	glutDisplayFunc(RenderCallback);
	glutReshapeFunc(ReshapeCallback);
	glutIdleFunc(IdleCallback);
	glutMouseFunc(MouseCallback);
	atexit(Exit);

	// Setup default render states
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);

	// Setup lighting
	glEnable(GL_LIGHTING);
	float ambientColor[] = {0.0f, 0.1f, 0.2f, 0.0f};
	float diffuseColor[] = {1.0f, 1.0f, 1.0f, 0.0f};
	float specularColor[] = {0.0f, 0.0f, 0.0f, 0.0f};	
	float position[] = {100.0f, 100.0f, 400.0f, 1.0f};
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientColor);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseColor);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularColor);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_LIGHT0);

	// Initialize the game
	if (Init())
		glutMainLoop();

	return 0;
}

#pragma endregion