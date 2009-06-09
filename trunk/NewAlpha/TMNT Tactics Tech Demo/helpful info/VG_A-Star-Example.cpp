/*****************************************************************************
IsoHex23_1.cpp
Ernest S. Pazera
29NOV2000
Start a WIN32 Application Workspace, add in this file
Requires GDICanvas.h/cpp
No other libs are required
*****************************************************************************/

//////////////////////////////////////////////////////////////////////////////
//INCLUDES
//////////////////////////////////////////////////////////////////////////////
#define WIN32_LEAN_AND_MEAN  

#include <windows.h>   
#include <stdlib.h>
#include "gdicanvas.h"

//////////////////////////////////////////////////////////////////////////////
//DEFINES
//////////////////////////////////////////////////////////////////////////////
//name for our window class
#define WINDOWCLASS "ISOHEX23"
//title of the application
#define WINDOWTITLE "IsoHex 23-1"

//dimensions of map
const int MAPWIDTH=16;
const int MAPHEIGHT=16;
//dimensions of tiles
const int TILEWIDTH=16;
const int TILEHEIGHT=16;

//names of tiles
const int TILEEMPTY=0;
const int TILEBLOCK=1;
const int TILESTART=2;
const int TILEEND=3;
const int TILEPATH=4;
const int TILESELECT=5;

//////////////////////////////////////////////////////////////////////////////
//PROTOTYPES
//////////////////////////////////////////////////////////////////////////////
bool Prog_Init();//game data initalizer
void Prog_Loop();//main game loop
void Prog_Done();//game clean up
void FindPath();//find the map
void PutTile(int x,int y,int tilenum);//put a tile
void DrawMap();//draw the map
void PlaceTile(int x,int y, int tilenum);//place a tile on the map

//////////////////////////////////////////////////////////////////////////////
//GLOBALS
//////////////////////////////////////////////////////////////////////////////
HINSTANCE hInstMain=NULL;//main application handle
HWND hWndMain=NULL;//handle to our main window

//map for the ai
CGDICanvas gdicMap;
//images for ai items
CGDICanvas gdicAI;

//map
int Map[MAPWIDTH][MAPHEIGHT];//contains tile data about the map
int MapPath[MAPWIDTH][MAPHEIGHT];//contains pathfinding info about the map
//map marks
bool MapMark[MAPWIDTH][MAPHEIGHT];//contains marked tiles for pathfinding
//currently selected tile
int TileSelected=0;

//////////////////////////////////////////////////////////////////////////////
//WINDOWPROC
//////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK TheWindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	//which message did we get?
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		{
			//get x and y positions
			int x=LOWORD(lParam);
			int y=HIWORD(lParam);
			//divide x and y by the tilewidth and tileheight
			x/=TILEWIDTH;
			y/=TILEHEIGHT;
			//decide if we are on the map or the toolbar
			if(y<MAPHEIGHT)
			{
				//we are in the map
				PlaceTile(x,y,TileSelected);
			}
			else
			{
				//we are in the toolbar
				//check if we are on one of the tiles
				if(x<4) TileSelected=x;
				DrawMap();
				InvalidateRect(hWndMain,NULL,FALSE);
			}
		}break;
	case WM_DESTROY://the window is being destroyed
		{

			//tell the application we are quitting
			PostQuitMessage(0);

			//handled message, so return 0
			return(0);

		}break;
	case WM_PAINT://the window needs repainting
		{
			//a variable needed for painting information
			PAINTSTRUCT ps;
			
			//start painting
			HDC hdc=BeginPaint(hwnd,&ps);

			//redraw the map
			BitBlt(hdc,0,0,TILEWIDTH*MAPWIDTH,TILEHEIGHT*MAPHEIGHT+TILEHEIGHT,gdicMap,0,0,SRCCOPY);

			//end painting
			EndPaint(hwnd,&ps);
					
			//handled message, so return 0
			return(0);
		}break;
	}

	//pass along any other message to default message handler
	return(DefWindowProc(hwnd,uMsg,wParam,lParam));
}


//////////////////////////////////////////////////////////////////////////////
//WINMAIN
//////////////////////////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd)
{
	//assign instance to global variable
	hInstMain=hInstance;

	//create window class
	WNDCLASSEX wcx;

	//set the size of the structure
	wcx.cbSize=sizeof(WNDCLASSEX);

	//class style
	wcx.style=CS_OWNDC | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;

	//window procedure
	wcx.lpfnWndProc=TheWindowProc;

	//class extra
	wcx.cbClsExtra=0;

	//window extra
	wcx.cbWndExtra=0;

	//application handle
	wcx.hInstance=hInstMain;

	//icon
	wcx.hIcon=LoadIcon(NULL,IDI_APPLICATION);

	//cursor
	wcx.hCursor=LoadCursor(NULL,IDC_ARROW);

	//background color
	wcx.hbrBackground=(HBRUSH)GetStockObject(BLACK_BRUSH);

	//menu
	wcx.lpszMenuName=NULL;

	//class name
	wcx.lpszClassName=WINDOWCLASS;

	//small icon
	wcx.hIconSm=NULL;

	//register the window class, return 0 if not successful
	if(!RegisterClassEx(&wcx)) return(0);

	//create main window
	hWndMain=CreateWindowEx(0,WINDOWCLASS,WINDOWTITLE, WS_BORDER | WS_SYSMENU | WS_CAPTION| WS_VISIBLE,0,0,320,240,NULL,NULL,hInstMain,NULL);

	//error check
	if(!hWndMain) return(0);

	//if program initialization failed, then return with 0
	if(!Prog_Init()) return(0);

	//message structure
	MSG msg;

	//message pump
	for(;;)	
	{
		//look for a message
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			//there is a message

			//check that we arent quitting
			if(msg.message==WM_QUIT) break;
			
			//translate message
			TranslateMessage(&msg);

			//dispatch message
			DispatchMessage(&msg);
		}

		//run main game loop
		Prog_Loop();
	}
	
	//clean up program data
	Prog_Done();

	//return the wparam from the WM_QUIT message
	return(msg.wParam);
}

//////////////////////////////////////////////////////////////////////////////
//INITIALIZATION
//////////////////////////////////////////////////////////////////////////////
bool Prog_Init()
{
	//set the client area size
	RECT rcTemp;
	SetRect(&rcTemp,0,0,MAPWIDTH*TILEWIDTH,MAPHEIGHT*TILEHEIGHT+TILEHEIGHT);//256x256 client area
	AdjustWindowRect(&rcTemp,WS_BORDER | WS_SYSMENU | WS_CAPTION| WS_VISIBLE,FALSE);//adjust the window size based on desired client area
	SetWindowPos(hWndMain,NULL,0,0,rcTemp.right-rcTemp.left,rcTemp.bottom-rcTemp.top,SWP_NOMOVE);//set the window width and height

	//create map image
	HDC hdc=GetDC(hWndMain);
	gdicMap.CreateBlank(hdc,MAPWIDTH*TILEWIDTH,MAPHEIGHT*TILEHEIGHT+TILEHEIGHT);
	FillRect(gdicMap,&rcTemp,(HBRUSH)GetStockObject(BLACK_BRUSH));
	ReleaseDC(hWndMain,hdc);

	//load in tiles
	gdicAI.Load(NULL,"ai.bmp");

	//start out the map
	for(int x=0;x<MAPWIDTH;x++)
	{
		for(int y=0;y<MAPHEIGHT;y++)
		{
			Map[x][y]=TILEEMPTY;
		}
	}
	//set initial start and end points
	Map[0][0]=TILESTART;
	Map[MAPWIDTH-1][MAPHEIGHT-1]=TILEEND;
	//draw the map
	DrawMap();

	return(true);//return success
}

//////////////////////////////////////////////////////////////////////////////
//CLEANUP
//////////////////////////////////////////////////////////////////////////////
void Prog_Done()
{
	//////////////////////////
	//clean up code goes here
	//////////////////////////
}

//////////////////////////////////////////////////////////////////////////////
//MAIN GAME LOOP
//////////////////////////////////////////////////////////////////////////////
void Prog_Loop()
{
	///////////////////////////
	//main game logic goes here
	///////////////////////////
}

void FindPath()//find the path
{
	POINT ptStart;
	POINT ptEnd;
	POINT ptPath;
	ptStart.x=-1;
	ptEnd.x=-1;
	int x;
	int y;
	int nx;
	int ny;
	bool found;
	int lowvalue;
	//find the start
	for(x=0;x<MAPWIDTH;x++)
	{
		for(y=0;y<MAPHEIGHT;y++)
		{
			//check for the start
			if(Map[x][y]==TILESTART)
			{
				ptStart.x=x;
				ptStart.y=y;
			}
		}
	}
	//find the end
	for(x=0;x<MAPWIDTH;x++)
	{
		for(y=0;y<MAPHEIGHT;y++)
		{
			//check for the end
			if(Map[x][y]==TILEEND)
			{
				ptEnd.x=x;
				ptEnd.y=y;
			}
		}
	}
	//if no start or end, exit function
	if(ptStart.x==-1 || ptEnd.x==-1) return;
	//fill out the path array
	for(x=0;x<MAPWIDTH;x++)
	{
		for(y=0;y<MAPHEIGHT;y++)
		{
			switch(Map[x][y])
			{
			case TILESTART://place a 0 at the start
				{
					MapPath[x][y]=0;
				}break;
			case TILEBLOCK://place a 999 at any blocks
				{
					MapPath[x][y]=999;
				}break;
			case TILEEMPTY://if empty, place a -1
				{
					MapPath[x][y]=-1;
				}break;
			default://anything else, place a -1
				{
					MapPath[x][y]=-1;
				}break;
			}
		}
	}
	//scan for pathable tiles
	do
	{
		//havent found one yet
		found=false;
		//scan the map
		for(x=0;x<MAPWIDTH;x++)
		{
			for(y=0;y<MAPHEIGHT;y++)
			{
				MapMark[x][y]=false;
				//make sure this is a "-1" square
				if(MapPath[x][y]==-1)
				{
					//scan the neighbors
					for(nx=x-1;nx<=x+1;nx++)
					{
						for(ny=y-1;ny<=y+1;ny++)
						{
							//make sure the neighbor is on the map
							if(nx>=0 && ny>=0 && nx<MAPWIDTH && ny<MAPHEIGHT && !(nx==x && ny==y))
							{
								//check against negatives and 999
								if(MapPath[nx][ny]>=0 && MapPath[nx][ny]!=999)
								{
									//mark the map
									MapMark[x][y]=true;
									//mark it as found
									found=true;
								}
							}
						}
					}
				}
			}
		}
		//now scan the marks
		for(x=0;x<MAPWIDTH;x++)
		{
			for(y=0;y<MAPHEIGHT;y++)
			{
				//if this square is marked
				if(MapMark[x][y])
				{
					//set low value very high
					lowvalue=999;
					//loop through neighbors
					for(nx=x-1;nx<=x+1;nx++)
					{
						for(ny=y-1;ny<=y+1;ny++)
						{
							//make sure the neighbor is on the map
							if(nx>=0 && ny>=0 && nx<MAPWIDTH && ny<MAPHEIGHT)
							{
								if(MapPath[nx][ny]>=0)//must be a non-negative value
								{
									//assign the value if it is lower
									if(MapPath[nx][ny]<lowvalue) lowvalue=MapPath[nx][ny];
								}
							}
						}
					}
					//assign the value to the path map
					MapPath[x][y]=lowvalue+1;
				}
			}
		}
	}
	while(found);
	//done with pathfinding
	//check to see that ptEnd has found a path
	if(MapPath[ptEnd.x][ptEnd.y]!=-1)
	{
		//start the path
		ptPath=ptEnd;
		//take the value from the map
		lowvalue=MapPath[ptEnd.x][ptEnd.y];
		while(lowvalue>0)
		{
			found=false;
			do
			{
				do
				{
					//pick a random neighbor
					nx=rand()%3-1;
					ny=rand()%3-1;
				}while((nx==0 && ny==0) || (ptPath.x+nx)<0 || (ptPath.x+nx)>=MAPWIDTH || (ptPath.y+ny)<0 || (ptPath.y+ny)>=MAPHEIGHT);
				//check to see if the value is lower
				if(MapPath[ptPath.x+nx][ptPath.y+ny]<lowvalue)
				{
					//found!
					found=true;
					//set tile to path tile
					Map[ptPath.x][ptPath.y]=TILEPATH;
					//move the path
					ptPath.x+=nx;
					ptPath.y+=ny;
					lowvalue=MapPath[ptPath.x][ptPath.y];
				}
			}
			while(!found);
		}
		//replace the end tile
		Map[ptEnd.x][ptEnd.y]=TILEEND;
	}
}

void PutTile(int x,int y,int tilenum)//put a tile
{
	//mask first
	BitBlt(gdicMap,x*TILEWIDTH,y*TILEHEIGHT,TILEWIDTH,TILEHEIGHT,gdicAI,tilenum*16,16,SRCAND);
	//then image
	BitBlt(gdicMap,x*TILEWIDTH,y*TILEHEIGHT,TILEWIDTH,TILEHEIGHT,gdicAI,tilenum*16,0,SRCPAINT);
}

void DrawMap()//draw the map
{
	//loop through the positions
	for(int x=0;x<MAPWIDTH;x++)
	{
		for(int y=0;y<MAPHEIGHT;y++)
		{
			//place the base tile first
			if(Map[x][y]!=TILEEMPTY && Map[x][y]!=TILESELECT)
			{
				PutTile(x,y,TILEEMPTY);
			}
			//place the tile
			PutTile(x,y,Map[x][y]);
		}
	}
	//clear out the bottom row
	RECT rcTemp;
	SetRect(&rcTemp,0,TILEHEIGHT*MAPHEIGHT,TILEWIDTH*MAPWIDTH,TILEHEIGHT+TILEHEIGHT*MAPHEIGHT);
	FillRect(gdicMap,&rcTemp,(HBRUSH)GetStockObject(BLACK_BRUSH));
	//place the toolbar
	for(int count=0;count<4;count++)
	{
		//put the tile
		PutTile(count,MAPHEIGHT,count);		
	}
	//put the seleciton
	PutTile(TileSelected,MAPHEIGHT,TILESELECT);
}

void PlaceTile(int x,int y, int tilenum)//place a tile on the map
{
	int tx;
	int ty;
	bool hasstart=(tilenum==TILESTART);
	bool hasend=(tilenum==TILEEND);
	//clear some tiles
	for(tx=0;tx<MAPWIDTH;tx++)
	{
		for(ty=0;ty<MAPHEIGHT;ty++)
		{
			//remove all path tiles
			if(Map[tx][ty]==TILEPATH) Map[tx][ty]=TILEEMPTY;
			//if we are placing the start, remove all start tiles
			if(Map[tx][ty]==TILESTART && tilenum==TILESTART) Map[tx][ty]=TILEEMPTY;
			//if we are placing the end, remove all end tiles
			if(Map[tx][ty]==TILEEND && tilenum==TILEEND) Map[tx][ty]=TILEEMPTY;
		}
	}
	//place the tile
	Map[x][y]=tilenum;
	//check for starting and ending point
	for(tx=0;tx<MAPWIDTH;tx++)
	{
		for(ty=0;ty<MAPHEIGHT;ty++)
		{
			//check for a start tile
			if(Map[tx][ty]==TILESTART) hasstart=true;
			//check for an end tile
			if(Map[tx][ty]==TILEEND) hasend=true;
		}
	}
	//find the path
	if(hasstart && hasend) FindPath();
	//draw the map
	DrawMap();
	//invalidate the window rect
	InvalidateRect(hWndMain,NULL,FALSE);
}

