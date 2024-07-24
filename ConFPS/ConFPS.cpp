#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <chrono>

#include <stdio.h>
#include <Windows.h>
#include <iostream>
#include <string>

int nScreenWidth = 320;			// Console Screen Size X (columns)
int nScreenHeight = nScreenWidth / 3;			// Console Screen Size Y (rows)
int nMapWidth = 32;				// World Dimensions
int nMapHeight = 32;
int ScreenPixels = nScreenWidth * nScreenHeight;

float fPlayerX = 11;			// Player Start Position
float fPlayerY = 11;
float fPlayerA = 0.0f;			// Player Start Rotation
float fFOV = 3.14159f / 3.0f;	// Field of View
float fDepth = 128;			// Maximum rendering distance
float fSpeed = 5.0f;			// Walking Speed

bool bHitWall;
bool bBoundary;
float fDistanceToWall;
float fStepSize;
short nShade = ' ';

std::wstring WindowTitle = L"Con FPS";

enum ConsoleColor {
    Black = 0,
    Blue = 1,
    Green = 2,
    Cyan = 3,
    Red = 4,
    Magenta = 5,
    Brown = 6,
    LightGray = 7,
    DarkGray = 8,
    LightBlue = 9,
    LightGreen = 10,
    LightCyan = 11,
    LightRed = 12,
    LightMagenta = 13,
    Yellow = 14,
    White = 15
};

enum MapType {
    MapOriginal,
    MapEmpty,
    MapPeni
};

bool IsKeyPressed(int vKey) {
    return GetAsyncKeyState(vKey) & 0x8000;
}

void HandlePlayerMovement(float& fPlayerX, float& fPlayerY, float fPlayerA, float fSpeed, float fElapsedTime, std::wstring map) {
    if (IsKeyPressed((unsigned short)'W')) {
        fPlayerX += sinf(fPlayerA) * fSpeed * fElapsedTime;
        fPlayerY += cosf(fPlayerA) * fSpeed * fElapsedTime;
        if (map.c_str()[(int)fPlayerX * nMapWidth + (int)fPlayerY] == '#') {
            fPlayerX -= sinf(fPlayerA) * fSpeed * fElapsedTime;
            fPlayerY -= cosf(fPlayerA) * fSpeed * fElapsedTime;
        }
    }

    if (IsKeyPressed((unsigned short)'S')) {
        fPlayerX -= sinf(fPlayerA) * fSpeed * fElapsedTime;
        fPlayerY -= cosf(fPlayerA) * fSpeed * fElapsedTime;
        if (map.c_str()[(int)fPlayerX * nMapWidth + (int)fPlayerY] == '#') {
            fPlayerX += sinf(fPlayerA) * fSpeed * fElapsedTime;
            fPlayerY += cosf(fPlayerA) * fSpeed * fElapsedTime;
        }
    }
}

void SetColor(ConsoleColor textColor, ConsoleColor bgColor) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (bgColor << 4) | textColor);
}

int main()
{
	// Create Screen Buffer
	wchar_t* screen = new wchar_t[ScreenPixels];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    COORD Size = {nScreenWidth, nScreenHeight};
    SetConsoleScreenBufferSize(hConsole, Size);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

    std::wstring MapOG;
	MapOG += L"################################";
	MapOG += L"#..............................#";
	MapOG += L"#......#########...............#";
	MapOG += L"#......##......................#";
	MapOG += L"#......##......................#";
	MapOG += L"#......##......................#";
	MapOG += L"#..............................#";
	MapOG += L"#########......................#";
	MapOG += L"##.............##..##..........#";
	MapOG += L"#......####..####..##..........#";
	MapOG += L"#......##......##..##..........#";
	MapOG += L"#......##......##..#############";
	MapOG += L"#......##......##..#############";
	MapOG += L"#......##########..............#";
	MapOG += L"#..............................#";
    MapOG += L"#..............................#";
    MapOG += L"#..............................#";
    MapOG += L"#..............#..#............#";
    MapOG += L"#.............#....#...........#";
    MapOG += L"##############......############";
    MapOG += L"#..............................#";
    MapOG += L"#..............................#";
    MapOG += L"#...............##.............#";
    MapOG += L"#.............######...........#";
    MapOG += L"#...........##########.........#";
    MapOG += L"#.............######...........#";
    MapOG += L"#...............##.............#";
    MapOG += L"#..............................#";
    MapOG += L"#..............................#";
    MapOG += L"#..............................#";
    MapOG += L"#..............................#";
	MapOG += L"################################";

    std::wstring MapEmpty;
    MapEmpty += L"################################";
    MapEmpty += L"#..............................#";
    MapEmpty += L"#..............................#";
    MapEmpty += L"#..............................#";
    MapEmpty += L"#..............................#";
    MapEmpty += L"#..............................#";
    MapEmpty += L"#..............................#";
    MapEmpty += L"#..............................#";
    MapEmpty += L"#..............................#";
    MapEmpty += L"#..............................#";
    MapEmpty += L"#..............................#";
    MapEmpty += L"#..............................#";
    MapEmpty += L"#..............................#";
    MapEmpty += L"#..............................#";
    MapEmpty += L"#..............................#";
    MapEmpty += L"#..............................#";
    MapEmpty += L"#..............................#";
    MapEmpty += L"#..............................#";
    MapEmpty += L"#..............................#";
    MapEmpty += L"#..............................#";
    MapEmpty += L"#..............................#";
    MapEmpty += L"#..............................#";
    MapEmpty += L"#..............................#";
    MapEmpty += L"#..............................#";
    MapEmpty += L"#..............................#";
    MapEmpty += L"#..............................#";
    MapEmpty += L"#..............................#";
    MapEmpty += L"#..............................#";
    MapEmpty += L"#..............................#";
    MapEmpty += L"#..............................#";
    MapEmpty += L"#..............................#";
    MapEmpty += L"################################";

    std::wstring MapPeni;
    MapPeni += L"################################";
    MapPeni += L"#..............................#";
    MapPeni += L"#..............................#";
    MapPeni += L"#...........########...........#";
    MapPeni += L"#...........########...........#";
    MapPeni += L"#...........########...........#";
    MapPeni += L"#...........########...........#";
    MapPeni += L"#...........########...........#";
    MapPeni += L"#...........########...........#";
    MapPeni += L"#...........########...........#";
    MapPeni += L"#...........########...........#";
    MapPeni += L"#...........########...........#";
    MapPeni += L"#...........########...........#";
    MapPeni += L"#...........########...........#";
    MapPeni += L"#...........########...........#";
    MapPeni += L"#...........########...........#";
    MapPeni += L"#...........########...........#";
    MapPeni += L"#...........########...........#";
    MapPeni += L"#...........########...........#";
    MapPeni += L"#...........########...........#";
    MapPeni += L"#...........########...........#";
    MapPeni += L"#...........########...........#";
    MapPeni += L"#...........########...........#";
    MapPeni += L"#...........########...........#";
    MapPeni += L"#..............................#";
    MapPeni += L"#..............................#";
    MapPeni += L"#..#########........#########..#";
    MapPeni += L"#..#########........#########..#";
    MapPeni += L"#..#########........#########..#";
    MapPeni += L"#..#########........#########..#";
    MapPeni += L"#..............................#";
    MapPeni += L"################################";

    std::wstring MapDoom;
    MapDoom += L"################################";
    MapDoom += L"#..............................#";
    MapDoom += L"#..............................#";
    MapDoom += L"#..............................#";
    MapDoom += L"#..........##############......#";
    MapDoom += L"#.........#..............#.....#";
    MapDoom += L"#.........#..............#.....#";
    MapDoom += L"#.........#....#....#....#.....#";
    MapDoom += L"#.........#..............#.....#";
    MapDoom += L"#.........#...##....##...#.....#";
    MapDoom += L"#.........#..##......##..#.....#";
    MapDoom += L"#.........###.#......#.###.....#";
    MapDoom += L"#.............#......#.........#";
    MapDoom += L"#..............##..##..........#";
    MapDoom += L"#...............#..#...........#";
    MapDoom += L"#............####..#####.......#";
    MapDoom += L"#............#.........######..#";
    MapDoom += L"#............#..............#..#";
    MapDoom += L"#.............##.#.#.#####..#..#";
    MapDoom += L"#.............#........###..#..#";
    MapDoom += L"#.............#........###..####";
    MapDoom += L"#.............#................#";
    MapDoom += L"#...........####...............#";
    MapDoom += L"#...........#..........##......#";
    MapDoom += L"#...........#..#.......#....####";
    MapDoom += L"#....########..###....##.......#";
    MapDoom += L"######.....##..#.............###";
    MapDoom += L"##...........................###";
    MapDoom += L"######.....#####.............###";
    MapDoom += L"#....#######....##########..####";
    MapDoom += L"#..............................#";
    MapDoom += L"################################";

    std::wstring MapSquare;
    MapSquare += L"####";
    MapSquare += L"#..#";
    MapSquare += L"#..#";
    MapSquare += L"####";

    std::wstring MapVoid;
    MapVoid += L".";

    std::wstring map = MapOG;
    std::wstring SelectedMapName = L"OG";
    bool DisplayMinimap = true;
    float HighFPS = 0;

	auto tp1 = std::chrono::system_clock::now();
	auto tp2 = std::chrono::system_clock::now();

	while (1)
	{
		tp2 = std::chrono::system_clock::now();
        std::chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float fElapsedTime = elapsedTime.count();

        if (IsKeyPressed(VK_F1))
        {
            map = MapOG;
            nMapWidth = 32;
            nMapHeight = 32;
            SelectedMapName = L"OG";
        }

        if (IsKeyPressed(VK_F2))
        {
            map = MapEmpty;
            nMapWidth = 32;
            nMapHeight = 32;
            SelectedMapName = L"Empty";
        }

        if (IsKeyPressed(VK_F3))
        {
            map = MapPeni;
            nMapWidth = 32;
            nMapHeight = 32;
            SelectedMapName = L"Peni";
        }

        if (IsKeyPressed(VK_F4))
        {
            map = MapDoom;
            nMapWidth = 32;
            nMapHeight = 32;
            SelectedMapName = L"Doom";
        }


        if (IsKeyPressed(VK_F5))
        {
            map = MapSquare;
            nMapWidth = 4;
            nMapHeight = 4;
            fPlayerX = 2;
            fPlayerY = 2;
            SelectedMapName = L"Square";
        }

        if (IsKeyPressed(VK_F6))
        {
            map = MapVoid;
            nMapWidth = 1;
            nMapHeight = 1;
            fPlayerX = 1;
            fPlayerY = 1;
            SelectedMapName = L"Void";
        }

        if (IsKeyPressed('M'))
            DisplayMinimap = !DisplayMinimap;

        HandlePlayerMovement(fPlayerX, fPlayerY, fPlayerA, fSpeed, fElapsedTime, map);

        if (IsKeyPressed((unsigned short)'A'))
            fPlayerA -= (fSpeed * 0.75f) * fElapsedTime;

        if (IsKeyPressed((unsigned short)'D'))
            fPlayerA += (fSpeed * 0.75f) * fElapsedTime;

		for (int x = 0; x < nScreenWidth; x++)
		{
			float fRayAngle = (fPlayerA - fFOV / 2.0f) + ((float)x / (float)nScreenWidth) * fFOV;			
			fDistanceToWall = 0.0f;

			bHitWall = false;
			bBoundary = false;

			float fEyeX = sinf(fRayAngle);
			float fEyeY = cosf(fRayAngle);

			while (!bHitWall && fDistanceToWall < fDepth)
			{
				fDistanceToWall += 0.1f;
				int nTestX = (int)(fPlayerX + fEyeX * fDistanceToWall);
				int nTestY = (int)(fPlayerY + fEyeY * fDistanceToWall);

				if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight)
				{
					bHitWall = true;
					fDistanceToWall = fDepth;
				}
				else
				{
					if (map.c_str()[nTestX * nMapWidth + nTestY] == '#')
					{
						bHitWall = true;

                        std::vector<std::pair<float, float>> p;

						for (int tx = 0; tx < 2; tx++)
							for (int ty = 0; ty < 2; ty++)
							{
								float vy = (float)nTestY + ty - fPlayerY;
								float vx = (float)nTestX + tx - fPlayerX;
								float d = sqrt(vx * vx + vy * vy);
								p.push_back(std::make_pair(d, (fEyeX * vx / d) + (fEyeY * vy / d)));
							}

						sort(p.begin(), p.end(), [](const std::pair<float, float>& left, const std::pair<float, float>& right) {return left.first < right.first; });

						bBoundary = (acos(p.at(0).second) < 0.01 || acos(p.at(1).second) < 0.01 || acos(p.at(2).second) < 0.01);
					}
				}
			}

			int nCeiling = (float)(nScreenHeight / 2.0) - nScreenHeight / ((float)fDistanceToWall);
			int nFloor = nScreenHeight - nCeiling;

            nShade = 0x2593;

            /*if (fDistanceToWall <= fDepth / 4.0f) nShade = 0x2588;
            else if (fDistanceToWall < fDepth / 3.0f) nShade = 0x2593;
            else if (fDistanceToWall < fDepth / 2.0f) nShade = 0x2592;
            else if (fDistanceToWall < fDepth) nShade = 0x2591;
            else nShade = ' ';*/

			if (bBoundary)		nShade = ' ';

			for (int y = 0; y < nScreenHeight; y++)
			{
				if (y <= nCeiling)
					screen[y * nScreenWidth + x] = ' '; // Sky
				else if (y > nCeiling && y <= nFloor)
					screen[y * nScreenWidth + x] = nShade; // Wall
				else // Ground
				{
					//float b = 1.0f - (((float)y - nScreenHeight / 2.0f) / ((float)nScreenHeight / 2.0f));
					//if (b < 0.25)		nShade = 176;
					//else if (b < 0.5)	nShade = 176;
					//else if (b < 0.75)	nShade = '.';
					//else if (b < 0.9)	nShade = 721;
					//else				nShade = 721;

                    //if (b < 0.25)		nShade = 0x2588;
                    //else if (b < 0.5)	nShade = 0x2593;
                    //else if (b < 0.75)	nShade = 0x2592;
                    //else if (b < 0.9)	nShade = 0x2591;
                   // else				nShade = ' ';

                    screen[y * nScreenWidth + x] = 0x2592;
				}
			}
		}

        float FPS = 1.0f / fElapsedTime;
        //if (FPS > HighFPS) // optional
            //HighFPS = FPS;

        std::wstring NewWindowTitle = L"ConFPS | FPS: " + std::to_wstring(FPS) + L" | Map: " + SelectedMapName + L" | Controls: WS: Movement, AD : Look, F1 - 5 : Map Selection";

        if (NewWindowTitle != WindowTitle)
        {
            WindowTitle = NewWindowTitle;
            SetConsoleTitle(WindowTitle.c_str());
        }

        if (DisplayMinimap)
        {
            for (int nx = 0; nx < nMapWidth; nx++)
                for (int ny = 0; ny < nMapWidth; ny++)
                    screen[(ny + 1) * nScreenWidth + nx] = map[ny * nMapWidth + nx];
            screen[((int)fPlayerX + 1) * nScreenWidth + (int)fPlayerY] = 'o';
        }

		screen[ScreenPixels - 1] = '\0';
		WriteConsoleOutputCharacter(hConsole, screen, ScreenPixels, { 0,0 }, &dwBytesWritten);
	}

	return 0;
}