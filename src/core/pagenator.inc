#define MAX_PAGE_OBJECTS (43)

new const MaximumPagesType[3] = {2, 17, 11};

new PlayerText: PagenatorHUD[MAX_PLAYERS][5],
	PlayerText: PagenatorCell[MAX_PLAYERS][18];

enum ePagenator
{
	Type,
	Page,
	
	Num,
	Model
}
new PagenatorWeapon[MAX_PAGE_OBJECTS][ePagenator] =
{
	{0, 1, 1, 331}, // 0
	{0, 1, 2, 333}, // 1
	{0, 1, 3, 334}, // 2
	{0, 1, 4, 335}, // 3
	{0, 1, 5, 336}, //4
	{0, 1, 6, 337}, //5
	{0, 1, 7, 338}, //6
	{0, 1, 8, 339}, //7
	{0, 1, 9, 341}, //8
	{0, 1, 10, 321}, //9
	{0, 1, 11, 322}, //10
	{0, 1, 12, 323}, //11
	{0, 1, 13, 324}, //12
	{0, 1, 14, 325}, //13
	{0, 1, 15, 326}, //14
	{0, 1, 16, 342}, //15
	{0, 1, 17, 343}, //16
	{0, 1, 18, 344}, //17
	{0, 2, 22, 346}, //18
	{0, 2, 23, 347}, //19
	{0, 2, 24, 348}, //20
	{0, 2, 25, 349}, //21
	{0, 2, 26, 350}, //22
	{0, 2, 27, 351}, //23
	{0, 2, 28, 352}, //24
	{0, 2, 29, 353}, //25
	{0, 2, 30, 355}, //26
	{0, 2, 31, 356}, //27
	{0, 2, 32, 372}, //28
	{0, 2, 33, 357}, //29
	{0, 2, 34, 358}, //30
	{0, 2, 35, 359}, //31
	{0, 2, 36, 360}, //32
	{0, 2, 37, 361}, //33
	{0, 2, 38, 362}, //34
	{0, 2, 39, 363}, //35
	{0, 3, 40, 364}, //36
	{0, 3, 41, 365}, //37
	{0, 3, 42, 366}, //38
	{0, 3, 43, 367}, //39
	{0, 3, 44, 368}, //40
	{0, 3, 45, 369}, //41
	{0, 3, 46, 371} //42
}; // 0 - Weapons, 1 - Skins, 2 - Vehicles

new PagenatorType[MAX_PLAYERS],
	PagenatorPage[MAX_PLAYERS],
	bool: PagenatorShowed[MAX_PLAYERS];


public OnPlayerConnect(playerid)
{
	PagenatorHUD[playerid][0] = CreatePlayerTextDraw(playerid, 150.000000, 120.000000, "_");
	PlayerTextDrawBackgroundColor(playerid, PagenatorHUD[playerid][0], 255);
	PlayerTextDrawFont(playerid, PagenatorHUD[playerid][0], 1);
	PlayerTextDrawLetterSize(playerid, PagenatorHUD[playerid][0], 0.200000, 19.599994);
	PlayerTextDrawColor(playerid, PagenatorHUD[playerid][0], -1);
	PlayerTextDrawSetOutline(playerid, PagenatorHUD[playerid][0], 0);
	PlayerTextDrawSetProportional(playerid, PagenatorHUD[playerid][0], 1);
	PlayerTextDrawSetShadow(playerid, PagenatorHUD[playerid][0], 1);
	PlayerTextDrawUseBox(playerid, PagenatorHUD[playerid][0], 1);
	PlayerTextDrawBoxColor(playerid, PagenatorHUD[playerid][0], 877231871);
	PlayerTextDrawTextSize(playerid, PagenatorHUD[playerid][0], 473.000000, 71.000000);
	PlayerTextDrawSetSelectable(playerid, PagenatorHUD[playerid][0], 0);

	PagenatorHUD[playerid][1] = CreatePlayerTextDraw(playerid, 307.000000, 123.000000, "Object");
	PlayerTextDrawAlignment(playerid, PagenatorHUD[playerid][1], 2);
	PlayerTextDrawBackgroundColor(playerid, PagenatorHUD[playerid][1], 255);
	PlayerTextDrawFont(playerid, PagenatorHUD[playerid][1], 1);
	PlayerTextDrawLetterSize(playerid, PagenatorHUD[playerid][1], 0.330000, 1.199999);
	PlayerTextDrawColor(playerid, PagenatorHUD[playerid][1], -1);
	PlayerTextDrawSetOutline(playerid, PagenatorHUD[playerid][1], 0);
	PlayerTextDrawSetProportional(playerid, PagenatorHUD[playerid][1], 1);
	PlayerTextDrawSetShadow(playerid, PagenatorHUD[playerid][1], 0);
	PlayerTextDrawSetSelectable(playerid, PagenatorHUD[playerid][1], 0);

	PagenatorCell[playerid][0] = CreatePlayerTextDraw(playerid, 160.000000, 140.000000, "_");
	PlayerTextDrawBackgroundColor(playerid, PagenatorCell[playerid][0], 877223679);
	PlayerTextDrawFont(playerid, PagenatorCell[playerid][0], 5);
	PlayerTextDrawLetterSize(playerid, PagenatorCell[playerid][0], 0.500000, 1.000000);
	PlayerTextDrawColor(playerid, PagenatorCell[playerid][0], -1);
	PlayerTextDrawSetOutline(playerid, PagenatorCell[playerid][0], 0);
	PlayerTextDrawSetProportional(playerid, PagenatorCell[playerid][0], 1);
	PlayerTextDrawSetShadow(playerid, PagenatorCell[playerid][0], 1);
	PlayerTextDrawUseBox(playerid, PagenatorCell[playerid][0], 1);
	PlayerTextDrawBoxColor(playerid, PagenatorCell[playerid][0], -256);
	PlayerTextDrawTextSize(playerid, PagenatorCell[playerid][0], 42.000000, 39.000000);
	PlayerTextDrawSetSelectable(playerid, PagenatorCell[playerid][0], 1);

	PagenatorCell[playerid][1] = CreatePlayerTextDraw(playerid, 210.000000, 140.000000, "_");
	PlayerTextDrawBackgroundColor(playerid, PagenatorCell[playerid][1], 877223679);
	PlayerTextDrawFont(playerid, PagenatorCell[playerid][1], 5);
	PlayerTextDrawLetterSize(playerid, PagenatorCell[playerid][0], 0.500000, 1.000000);
	PlayerTextDrawColor(playerid, PagenatorCell[playerid][1], -1);
	PlayerTextDrawSetOutline(playerid, PagenatorCell[playerid][1], 0);
	PlayerTextDrawSetProportional(playerid, PagenatorCell[playerid][1], 1);
	PlayerTextDrawSetShadow(playerid, PagenatorCell[playerid][1], 1);
	PlayerTextDrawUseBox(playerid, PagenatorCell[playerid][1], 1);
	PlayerTextDrawBoxColor(playerid, PagenatorCell[playerid][1], -256);
	PlayerTextDrawTextSize(playerid, PagenatorCell[playerid][1], 42.000000, 39.000000);
	PlayerTextDrawSetSelectable(playerid, PagenatorCell[playerid][1], 1);

	PagenatorCell[playerid][2] = CreatePlayerTextDraw(playerid, 261.000000, 140.000000, "_");
	PlayerTextDrawBackgroundColor(playerid, PagenatorCell[playerid][2], 877223679);
	PlayerTextDrawFont(playerid, PagenatorCell[playerid][2], 5);
	PlayerTextDrawLetterSize(playerid, PagenatorCell[playerid][2], 0.500000, 1.000000);
	PlayerTextDrawColor(playerid, PagenatorCell[playerid][2], -1);
	PlayerTextDrawSetOutline(playerid, PagenatorCell[playerid][2], 0);
	PlayerTextDrawSetProportional(playerid, PagenatorCell[playerid][2], 1);
	PlayerTextDrawSetShadow(playerid, PagenatorCell[playerid][2], 1);
	PlayerTextDrawUseBox(playerid, PagenatorCell[playerid][2], 1);
	PlayerTextDrawBoxColor(playerid, PagenatorCell[playerid][2], -256);
	PlayerTextDrawTextSize(playerid, PagenatorCell[playerid][2], 42.000000, 39.000000);
	PlayerTextDrawSetSelectable(playerid, PagenatorCell[playerid][2], 1);

	PagenatorCell[playerid][3]= CreatePlayerTextDraw(playerid, 313.000000, 140.000000, "_");
	PlayerTextDrawBackgroundColor(playerid, PagenatorCell[playerid][3], 877223679);
	PlayerTextDrawFont(playerid, PagenatorCell[playerid][3], 5);
	PlayerTextDrawLetterSize(playerid, PagenatorCell[playerid][3], 0.500000, 1.000000);
	PlayerTextDrawColor(playerid, PagenatorCell[playerid][3], -1);
	PlayerTextDrawSetOutline(playerid, PagenatorCell[playerid][3], 0);
	PlayerTextDrawSetProportional(playerid, PagenatorCell[playerid][3], 1);
	PlayerTextDrawSetShadow(playerid, PagenatorCell[playerid][3], 1);
	PlayerTextDrawUseBox(playerid, PagenatorCell[playerid][3], 1);
	PlayerTextDrawBoxColor(playerid, PagenatorCell[playerid][3], -256);
	PlayerTextDrawTextSize(playerid, PagenatorCell[playerid][3], 42.000000, 39.000000);
	PlayerTextDrawSetSelectable(playerid, PagenatorCell[playerid][3], 1);

	PagenatorCell[playerid][4] = CreatePlayerTextDraw(playerid, 365.000000, 140.000000, "_");
	PlayerTextDrawBackgroundColor(playerid, PagenatorCell[playerid][4], 877223679);
	PlayerTextDrawFont(playerid, PagenatorCell[playerid][4], 5);
	PlayerTextDrawLetterSize(playerid, PagenatorCell[playerid][4], 0.500000, 1.000000);
	PlayerTextDrawColor(playerid, PagenatorCell[playerid][4], -1);
	PlayerTextDrawSetOutline(playerid, PagenatorCell[playerid][4], 0);
	PlayerTextDrawSetProportional(playerid, PagenatorCell[playerid][4], 1);
	PlayerTextDrawSetShadow(playerid, PagenatorCell[playerid][4], 1);
	PlayerTextDrawUseBox(playerid, PagenatorCell[playerid][4], 1);
	PlayerTextDrawBoxColor(playerid, PagenatorCell[playerid][4], -256);
	PlayerTextDrawTextSize(playerid, PagenatorCell[playerid][4], 42.000000, 39.000000);
	PlayerTextDrawSetSelectable(playerid, PagenatorCell[playerid][4], 1);

	PagenatorCell[playerid][5] = CreatePlayerTextDraw(playerid, 417.000000, 140.000000, "_");
	PlayerTextDrawBackgroundColor(playerid, PagenatorCell[playerid][5], 877223679);
	PlayerTextDrawFont(playerid, PagenatorCell[playerid][5], 5);
	PlayerTextDrawLetterSize(playerid, PagenatorCell[playerid][5], 0.500000, 1.000000);
	PlayerTextDrawColor(playerid, PagenatorCell[playerid][5], -1);
	PlayerTextDrawSetOutline(playerid, PagenatorCell[playerid][5], 0);
	PlayerTextDrawSetProportional(playerid, PagenatorCell[playerid][5], 1);
	PlayerTextDrawSetShadow(playerid, PagenatorCell[playerid][5], 1);
	PlayerTextDrawUseBox(playerid, PagenatorCell[playerid][5], 1);
	PlayerTextDrawBoxColor(playerid, PagenatorCell[playerid][5], -256);
	PlayerTextDrawTextSize(playerid, PagenatorCell[playerid][5], 42.000000, 39.000000);
	PlayerTextDrawSetSelectable(playerid, PagenatorCell[playerid][5], 1);

	PagenatorCell[playerid][6] = CreatePlayerTextDraw(playerid, 160.000000, 189.000000, "_");
	PlayerTextDrawBackgroundColor(playerid, PagenatorCell[playerid][6], 877223679);
	PlayerTextDrawFont(playerid, PagenatorCell[playerid][6], 5);
	PlayerTextDrawLetterSize(playerid, PagenatorCell[playerid][6], 0.500000, 1.000000);
	PlayerTextDrawColor(playerid, PagenatorCell[playerid][6], -1);
	PlayerTextDrawSetOutline(playerid, PagenatorCell[playerid][6], 0);
	PlayerTextDrawSetProportional(playerid, PagenatorCell[playerid][6], 1);
	PlayerTextDrawSetShadow(playerid, PagenatorCell[playerid][6], 1);
	PlayerTextDrawUseBox(playerid, PagenatorCell[playerid][6], 1);
	PlayerTextDrawBoxColor(playerid, PagenatorCell[playerid][6], -256);
	PlayerTextDrawTextSize(playerid, PagenatorCell[playerid][6], 42.000000, 39.000000);
	PlayerTextDrawSetSelectable(playerid, PagenatorCell[playerid][6], 1);

	PagenatorCell[playerid][7] = CreatePlayerTextDraw(playerid, 211.000000, 189.000000, "_");
	PlayerTextDrawBackgroundColor(playerid, PagenatorCell[playerid][7], 877223679);
	PlayerTextDrawFont(playerid, PagenatorCell[playerid][7], 5);
	PlayerTextDrawLetterSize(playerid, PagenatorCell[playerid][7], 0.500000, 1.000000);
	PlayerTextDrawColor(playerid, PagenatorCell[playerid][7], -1);
	PlayerTextDrawSetOutline(playerid, PagenatorCell[playerid][7], 0);
	PlayerTextDrawSetProportional(playerid, PagenatorCell[playerid][7], 1);
	PlayerTextDrawSetShadow(playerid, PagenatorCell[playerid][7], 1);
	PlayerTextDrawUseBox(playerid, PagenatorCell[playerid][7], 1);
	PlayerTextDrawBoxColor(playerid, PagenatorCell[playerid][7], -256);
	PlayerTextDrawTextSize(playerid, PagenatorCell[playerid][7], 42.000000, 39.000000);
	PlayerTextDrawSetSelectable(playerid, PagenatorCell[playerid][7], 1);

	PagenatorCell[playerid][8] = CreatePlayerTextDraw(playerid, 261.000000, 189.000000, "_");
	PlayerTextDrawBackgroundColor(playerid, PagenatorCell[playerid][8], 877223679);
	PlayerTextDrawFont(playerid, PagenatorCell[playerid][8], 5);
	PlayerTextDrawLetterSize(playerid, PagenatorCell[playerid][8], 0.500000, 1.000000);
	PlayerTextDrawColor(playerid, PagenatorCell[playerid][8], -1);
	PlayerTextDrawSetOutline(playerid, PagenatorCell[playerid][8], 0);
	PlayerTextDrawSetProportional(playerid, PagenatorCell[playerid][8], 1);
	PlayerTextDrawSetShadow(playerid, PagenatorCell[playerid][8], 1);
	PlayerTextDrawUseBox(playerid, PagenatorCell[playerid][8], 1);
	PlayerTextDrawBoxColor(playerid, PagenatorCell[playerid][8], -256);
	PlayerTextDrawTextSize(playerid, PagenatorCell[playerid][8], 42.000000, 39.000000);
	PlayerTextDrawSetSelectable(playerid, PagenatorCell[playerid][8], 1);

	PagenatorCell[playerid][9] = CreatePlayerTextDraw(playerid, 313.000000, 189.000000, "_");
	PlayerTextDrawBackgroundColor(playerid, PagenatorCell[playerid][9], 877223679);
	PlayerTextDrawFont(playerid, PagenatorCell[playerid][9], 5);
	PlayerTextDrawLetterSize(playerid, PagenatorCell[playerid][9], 0.500000, 1.000000);
	PlayerTextDrawColor(playerid, PagenatorCell[playerid][9], -1);
	PlayerTextDrawSetOutline(playerid, PagenatorCell[playerid][9], 0);
	PlayerTextDrawSetProportional(playerid, PagenatorCell[playerid][9], 1);
	PlayerTextDrawSetShadow(playerid, PagenatorCell[playerid][9], 1);
	PlayerTextDrawUseBox(playerid, PagenatorCell[playerid][9], 1);
	PlayerTextDrawBoxColor(playerid, PagenatorCell[playerid][9], -256);
	PlayerTextDrawTextSize(playerid, PagenatorCell[playerid][9], 42.000000, 39.000000);
	PlayerTextDrawSetSelectable(playerid, PagenatorCell[playerid][9], 1);

	PagenatorCell[playerid][10] = CreatePlayerTextDraw(playerid, 365.000000, 189.000000, "_");
	PlayerTextDrawBackgroundColor(playerid, PagenatorCell[playerid][10], 877223679);
	PlayerTextDrawFont(playerid, PagenatorCell[playerid][10], 5);
	PlayerTextDrawLetterSize(playerid, PagenatorCell[playerid][10], 0.500000, 1.000000);
	PlayerTextDrawColor(playerid, PagenatorCell[playerid][10], -1);
	PlayerTextDrawSetOutline(playerid, PagenatorCell[playerid][10], 0);
	PlayerTextDrawSetProportional(playerid, PagenatorCell[playerid][10], 1);
	PlayerTextDrawSetShadow(playerid, PagenatorCell[playerid][10], 1);
	PlayerTextDrawUseBox(playerid, PagenatorCell[playerid][10], 1);
	PlayerTextDrawBoxColor(playerid, PagenatorCell[playerid][10], -256);
	PlayerTextDrawTextSize(playerid, PagenatorCell[playerid][10], 42.000000, 39.000000);
	PlayerTextDrawSetSelectable(playerid, PagenatorCell[playerid][10], 1);

	PagenatorCell[playerid][11] = CreatePlayerTextDraw(playerid, 417.000000, 189.000000, "_");
	PlayerTextDrawBackgroundColor(playerid, PagenatorCell[playerid][11], 877223679);
	PlayerTextDrawFont(playerid, PagenatorCell[playerid][11], 5);
	PlayerTextDrawLetterSize(playerid, PagenatorCell[playerid][11], 0.500000, 1.000000);
	PlayerTextDrawColor(playerid, PagenatorCell[playerid][11], -1);
	PlayerTextDrawSetOutline(playerid, PagenatorCell[playerid][11], 0);
	PlayerTextDrawSetProportional(playerid, PagenatorCell[playerid][11], 1);
	PlayerTextDrawSetShadow(playerid, PagenatorCell[playerid][11], 1);
	PlayerTextDrawUseBox(playerid, PagenatorCell[playerid][11], 1);
	PlayerTextDrawBoxColor(playerid, PagenatorCell[playerid][11], -256);
	PlayerTextDrawTextSize(playerid, PagenatorCell[playerid][11], 42.000000, 39.000000);
	PlayerTextDrawSetSelectable(playerid, PagenatorCell[playerid][11], 1);

	PagenatorCell[playerid][12] = CreatePlayerTextDraw(playerid, 160.000000, 237.000000, "_");
	PlayerTextDrawBackgroundColor(playerid, PagenatorCell[playerid][12], 877223679);
	PlayerTextDrawFont(playerid, PagenatorCell[playerid][12], 5);
	PlayerTextDrawLetterSize(playerid, PagenatorCell[playerid][12], 0.500000, 1.000000);
	PlayerTextDrawColor(playerid, PagenatorCell[playerid][12], -1);
	PlayerTextDrawSetOutline(playerid, PagenatorCell[playerid][12], 0);
	PlayerTextDrawSetProportional(playerid, PagenatorCell[playerid][12], 1);
	PlayerTextDrawSetShadow(playerid, PagenatorCell[playerid][12], 1);
	PlayerTextDrawUseBox(playerid, PagenatorCell[playerid][12], 1);
	PlayerTextDrawBoxColor(playerid, PagenatorCell[playerid][12], -256);
	PlayerTextDrawTextSize(playerid, PagenatorCell[playerid][12], 42.000000, 39.000000);
	PlayerTextDrawSetSelectable(playerid, PagenatorCell[playerid][12], 1);

	PagenatorCell[playerid][13] = CreatePlayerTextDraw(playerid, 211.000000, 237.000000, "_");
	PlayerTextDrawBackgroundColor(playerid, PagenatorCell[playerid][13], 877223679);
	PlayerTextDrawFont(playerid, PagenatorCell[playerid][13], 5);
	PlayerTextDrawLetterSize(playerid, PagenatorCell[playerid][13], 0.500000, 1.000000);
	PlayerTextDrawColor(playerid, PagenatorCell[playerid][13], -1);
	PlayerTextDrawSetOutline(playerid, PagenatorCell[playerid][13], 0);
	PlayerTextDrawSetProportional(playerid, PagenatorCell[playerid][13], 1);
	PlayerTextDrawSetShadow(playerid, PagenatorCell[playerid][13], 1);
	PlayerTextDrawUseBox(playerid, PagenatorCell[playerid][13], 1);
	PlayerTextDrawBoxColor(playerid, PagenatorCell[playerid][13], -256);
	PlayerTextDrawTextSize(playerid, PagenatorCell[playerid][13], 42.000000, 39.000000);
	PlayerTextDrawSetSelectable(playerid, PagenatorCell[playerid][13], 1);

	PagenatorCell[playerid][14] = CreatePlayerTextDraw(playerid, 261.000000, 237.000000, "_");
	PlayerTextDrawBackgroundColor(playerid, PagenatorCell[playerid][14], 877223679);
	PlayerTextDrawFont(playerid, PagenatorCell[playerid][14], 5);
	PlayerTextDrawLetterSize(playerid, PagenatorCell[playerid][14], 0.500000, 1.000000);
	PlayerTextDrawColor(playerid, PagenatorCell[playerid][14], -1);
	PlayerTextDrawSetOutline(playerid, PagenatorCell[playerid][14], 0);
	PlayerTextDrawSetProportional(playerid, PagenatorCell[playerid][14], 1);
	PlayerTextDrawSetShadow(playerid, PagenatorCell[playerid][14], 1);
	PlayerTextDrawUseBox(playerid, PagenatorCell[playerid][14], 1);
	PlayerTextDrawBoxColor(playerid, PagenatorCell[playerid][14], -256);
	PlayerTextDrawTextSize(playerid, PagenatorCell[playerid][14], 42.000000, 39.000000);
	PlayerTextDrawSetSelectable(playerid, PagenatorCell[playerid][14], 1);

	PagenatorCell[playerid][15] = CreatePlayerTextDraw(playerid, 313.000000, 237.000000, "_");
	PlayerTextDrawBackgroundColor(playerid, PagenatorCell[playerid][15], 877223679);
	PlayerTextDrawFont(playerid, PagenatorCell[playerid][15], 5);
	PlayerTextDrawLetterSize(playerid, PagenatorCell[playerid][15], 0.500000, 1.000000);
	PlayerTextDrawColor(playerid, PagenatorCell[playerid][15], -1);
	PlayerTextDrawSetOutline(playerid, PagenatorCell[playerid][15], 0);
	PlayerTextDrawSetProportional(playerid, PagenatorCell[playerid][15], 1);
	PlayerTextDrawSetShadow(playerid, PagenatorCell[playerid][15], 1);
	PlayerTextDrawUseBox(playerid, PagenatorCell[playerid][15], 1);
	PlayerTextDrawBoxColor(playerid, PagenatorCell[playerid][15], -256);
	PlayerTextDrawTextSize(playerid, PagenatorCell[playerid][15], 42.000000, 39.000000);
	PlayerTextDrawSetSelectable(playerid, PagenatorCell[playerid][15], 1);

	PagenatorCell[playerid][16] = CreatePlayerTextDraw(playerid, 365.000000, 237.000000, "_");
	PlayerTextDrawBackgroundColor(playerid, PagenatorCell[playerid][16], 877223679);
	PlayerTextDrawFont(playerid, PagenatorCell[playerid][16], 5);
	PlayerTextDrawLetterSize(playerid, PagenatorCell[playerid][16], 0.500000, 1.000000);
	PlayerTextDrawColor(playerid, PagenatorCell[playerid][16], -1);
	PlayerTextDrawSetOutline(playerid, PagenatorCell[playerid][16], 0);
	PlayerTextDrawSetProportional(playerid, PagenatorCell[playerid][16], 1);
	PlayerTextDrawSetShadow(playerid, PagenatorCell[playerid][16], 1);
	PlayerTextDrawUseBox(playerid, PagenatorCell[playerid][16], 1);
	PlayerTextDrawBoxColor(playerid, PagenatorCell[playerid][16], -256);
	PlayerTextDrawTextSize(playerid, PagenatorCell[playerid][16], 42.000000, 39.000000);
	PlayerTextDrawSetSelectable(playerid, PagenatorCell[playerid][16], 1);

	PagenatorCell[playerid][17] = CreatePlayerTextDraw(playerid, 417.000000, 237.000000, "_");
	PlayerTextDrawBackgroundColor(playerid, PagenatorCell[playerid][17], 877223679);
	PlayerTextDrawFont(playerid, PagenatorCell[playerid][17], 5);
	PlayerTextDrawLetterSize(playerid, PagenatorCell[playerid][17], 0.500000, 1.000000);
	PlayerTextDrawColor(playerid, PagenatorCell[playerid][17], -1);
	PlayerTextDrawSetOutline(playerid, PagenatorCell[playerid][17], 0);
	PlayerTextDrawSetProportional(playerid, PagenatorCell[playerid][17], 1);
	PlayerTextDrawSetShadow(playerid, PagenatorCell[playerid][17], 1);
	PlayerTextDrawUseBox(playerid, PagenatorCell[playerid][17], 1);
	PlayerTextDrawBoxColor(playerid, PagenatorCell[playerid][17], -256);
	PlayerTextDrawTextSize(playerid, PagenatorCell[playerid][17], 42.000000, 39.000000);
	PlayerTextDrawSetSelectable(playerid, PagenatorCell[playerid][17], 1);

	PagenatorHUD[playerid][2] = CreatePlayerTextDraw(playerid, 282.000000, 282.000000, "<");
	PlayerTextDrawAlignment(playerid, PagenatorHUD[playerid][2], 2);
	PlayerTextDrawBackgroundColor(playerid, PagenatorHUD[playerid][2], 255);
	PlayerTextDrawFont(playerid, PagenatorHUD[playerid][2], 1);
	PlayerTextDrawLetterSize(playerid, PagenatorHUD[playerid][2], 0.300000, 1.000000);
	PlayerTextDrawColor(playerid, PagenatorHUD[playerid][2], -1);
	PlayerTextDrawSetOutline(playerid, PagenatorHUD[playerid][2], 0);
	PlayerTextDrawSetProportional(playerid, PagenatorHUD[playerid][2], 1);
	PlayerTextDrawSetShadow(playerid, PagenatorHUD[playerid][2], 0);
	PlayerTextDrawUseBox(playerid, PagenatorHUD[playerid][2], 1);
	PlayerTextDrawBoxColor(playerid, PagenatorHUD[playerid][2], 877226495);
	PlayerTextDrawTextSize(playerid, PagenatorHUD[playerid][2], 18.000000, 38.000000);
	PlayerTextDrawSetSelectable(playerid, PagenatorHUD[playerid][2], 1);

	PagenatorHUD[playerid][3] = CreatePlayerTextDraw(playerid, 334.000000, 282.000000, ">");
	PlayerTextDrawAlignment(playerid, PagenatorHUD[playerid][3], 2);
	PlayerTextDrawBackgroundColor(playerid, PagenatorHUD[playerid][3], 255);
	PlayerTextDrawFont(playerid, PagenatorHUD[playerid][3], 1);
	PlayerTextDrawLetterSize(playerid, PagenatorHUD[playerid][3], 0.300000, 1.000000);
	PlayerTextDrawColor(playerid, PagenatorHUD[playerid][3], -1);
	PlayerTextDrawSetOutline(playerid, PagenatorHUD[playerid][3], 0);
	PlayerTextDrawSetProportional(playerid, PagenatorHUD[playerid][3], 1);
	PlayerTextDrawSetShadow(playerid, PagenatorHUD[playerid][3], 0);
	PlayerTextDrawUseBox(playerid, PagenatorHUD[playerid][3], 1);
	PlayerTextDrawBoxColor(playerid, PagenatorHUD[playerid][3], 877226495);
	PlayerTextDrawTextSize(playerid, PagenatorHUD[playerid][3], 18.000000, 38.000000);
	PlayerTextDrawSetSelectable(playerid, PagenatorHUD[playerid][3], 1);

	PagenatorHUD[playerid][4] = CreatePlayerTextDraw(playerid, 438.000000, 282.000000, "Page: 1/10");
	PlayerTextDrawAlignment(playerid, PagenatorHUD[playerid][4], 2);
	PlayerTextDrawBackgroundColor(playerid, PagenatorHUD[playerid][4], 255);
	PlayerTextDrawFont(playerid, PagenatorHUD[playerid][4], 1);
	PlayerTextDrawLetterSize(playerid, PagenatorHUD[playerid][4], 0.220000, 1.000000);
	PlayerTextDrawColor(playerid, PagenatorHUD[playerid][4], -1);
	PlayerTextDrawSetOutline(playerid, PagenatorHUD[playerid][4], 0);
	PlayerTextDrawSetProportional(playerid, PagenatorHUD[playerid][4], 1);
	PlayerTextDrawSetShadow(playerid, PagenatorHUD[playerid][4], 0);
	PlayerTextDrawSetSelectable(playerid, PagenatorHUD[playerid][4], 0);
	
	PagenatorShowed[playerid] = false;
	PagenatorType[playerid] = 0;
	PagenatorPage[playerid] = 0;
    #if defined sandpage_OnPlayerConnect
    	return sandpage_OnPlayerConnect(playerid);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect sandpage_OnPlayerConnect
#if defined sandpage_OnPlayerConnect
forward sandpage_OnPlayerConnect(playerid);
#endif

public OnPlayerDisconnect(playerid, reason)
{
	for(new i; i != 5; i++)
	{
		PlayerTextDrawDestroy(playerid, PagenatorHUD[playerid][i]);
	}
	for(new i; i != 18; i++)
	{
		PlayerTextDrawDestroy(playerid, PagenatorCell[playerid][i]);
	}
    #if defined sandpage_OnPlayerDisconnect
    	return sandpage_OnPlayerDisconnect(playerid, reason);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect sandpage_OnPlayerDisconnect
#if defined sandpage_OnPlayerDisconnect
forward sandpage_OnPlayerDisconnect(playerid, reason);
#endif


ShowPagenatorForPlayer(playerid, type)
{
    PagenatorShowed[playerid] = true;
    PagenatorType[playerid] = type;
    PagenatorPage[playerid] = 0;
    SelectTextDraw(playerid, 0x767777FF);
    
	for(new i; i != 5; i++)
	{
		PlayerTextDrawShow(playerid, PagenatorHUD[playerid][i]);
	}
	for(new i; i != 18; i++)
	{
		PlayerTextDrawShow(playerid, PagenatorCell[playerid][i]);
	}
	UpdatePagenator(playerid);
	return 1;
}

HidePagenatorForPlayer(playerid)
{
	PagenatorShowed[playerid] = false;
	for(new i; i != 5; i++)
	{
	    PlayerTextDrawHide(playerid, PagenatorHUD[playerid][i]);
	}
	for(new i; i != 18; i++)
	{
	    PlayerTextDrawHide(playerid, PagenatorCell[playerid][i]);
	}
	CancelSelectTextDraw(playerid);
	return 1;
}

UpdatePagenator(playerid)
{
	new tdpmin;
	if(PagenatorType[playerid] != 0)
	{
		switch(PagenatorType[playerid])
		{
			case 1:
			{
				tdpmin = 0;
				PlayerTextDrawSetString(playerid, PagenatorHUD[playerid][1], "Skin");
			}
			case 2:
			{
				tdpmin = 400;
				PlayerTextDrawSetString(playerid, PagenatorHUD[playerid][1], "Vehicle");
			}
		}
		tdpmin = (tdpmin + (PagenatorPage[playerid] * 18));
		for(new i; i != 18; i++)
		{
			PlayerTextDrawHide(playerid, PagenatorCell[playerid][i]); // ((tdpmin + i) == 615 || ((tdpmin + i) > 311) && ((tdpmin + i) < 350)
			PlayerTextDrawSetPreviewModel(playerid, PagenatorCell[playerid][i], ((tdpmin + i) == 615 || (((tdpmin + i) > 311) && ((tdpmin + i) < 350))) ? (74) : (tdpmin + i));
			PlayerTextDrawShow(playerid, PagenatorCell[playerid][i]);
		}
	}
	else
	{
	    PlayerTextDrawSetString(playerid, PagenatorHUD[playerid][1], "Weapon");
	    for(new i; i != 18; i++)
		{
			PlayerTextDrawHide(playerid, PagenatorCell[playerid][i]);
			PlayerTextDrawSetPreviewModel(playerid, PagenatorCell[playerid][i], (((PagenatorPage[playerid] * 18) + i) > 42) ? (74) : (PagenatorWeapon[(PagenatorPage[playerid] * 18) + i][Model]));
			PlayerTextDrawShow(playerid, PagenatorCell[playerid][i]);
		}
	}
	new buf[16];
	format(buf, sizeof buf, "Page: %d/%d", PagenatorPage[playerid] + 1, MaximumPagesType[PagenatorType[playerid]] + 1);
	PlayerTextDrawSetString(playerid, PagenatorHUD[playerid][4], buf);
	return 1;
}

CallPagenatorFunc(playerid, clickedid)
{
    if(PagenatorType[playerid] == 0) GivePlayerWeapon(playerid, PagenatorWeapon[(PagenatorPage[playerid] * 18) + clickedid][Num], 999);
	if(PagenatorType[playerid] == 1) SetPlayerSkin(playerid, (PagenatorPage[playerid] * 18) + clickedid);
	if(PagenatorType[playerid] == 2) GivePlayerVehicle(playerid, 400 + ((PagenatorPage[playerid] * 18) + clickedid), 1, 1);
	HidePagenatorForPlayer(playerid);
	return 1;
}
