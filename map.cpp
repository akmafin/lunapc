#include "lunapc.h"

void MapInit(maindata *lunadata) {
	lunadata->map.HscrollSpeed = 1;
	lunadata->map.MinGap = 16;
	lunadata->map.CeilHeight = 0;
	lunadata->map.PrevCeilHeight = 0;
	lunadata->map.FloorHeight = 0;
	lunadata->map.FloorDir = 0;
	lunadata->map.CeilDir = 0;
	lunadata->map.MinHeight = 2;
}

void MapGenerateColumn(maindata *lunadata) {
	int x, y, mod, ch = 135, NotRandomChange = 0;

	lunadata->map.MinHeight = 2;
	if(lunadata->enemies.SectorTransition != 0) {
		lunadata->map.MinHeight = 0;
		if(lunadata->map.CeilHeight != 0) {
			y = -1;
			lunadata->map.CeilHeight--;
			if(lunadata->map.CeilHeight == 0)
				y = 0;
			lunadata->map.CeilDir = y;
		}
		
		if(lunadata->map.FloorHeight != 0) {
			y = -1;
			lunadata->map.FloorHeight--;
			if(lunadata->map.FloorHeight == 0)
				y = 0;
			lunadata->map.FloorDir = y;
		}
		
		lunadata->map.CeilHeight += lunadata->map.FloorHeight;
		if(lunadata->map.CeilHeight == 0) {
			lunadata->enemies.SectorTransition++;
		}
		
		// EndTransitionFadeOut
		if(lunadata->enemies.SectorTransition >= 40) {
			MapChangeMapColors(lunadata);
			EnemiesAddWave(lunadata);
		} else {
			NotRandomChange = 1;
		}
	}

	if(!NotRandomChange) {
		// Ceiling
		RandomGet(lunadata);
		if(lunadata->RandomNum >= lunadata->map.DirRandom) {
			RandomGet(lunadata);
			if(!(lunadata->RandomNum & 0x80)) {
				if(lunadata->map.CeilDir != 1)
					lunadata->map.CeilDir++;
			} else {
				if(lunadata->map.CeilDir != -1)
					lunadata->map.CeilDir--;
			}
			if(lunadata->map.CeilDir == 1) {
				if((23 - lunadata->map.CeilHeight - lunadata->map.FloorHeight) < lunadata->map.MinGap)
					lunadata->map.CeilDir = 0;
			} else {
				lunadata->map.CeilHeight += lunadata->map.CeilDir;
				if(lunadata->map.CeilHeight <= lunadata->map.MinHeight) {
					lunadata->map.CeilDir = 0;
					lunadata->map.CeilHeight = lunadata->map.MinHeight;
				}
			}
		}

		// Floor
		RandomGet(lunadata);
		if(lunadata->RandomNum >= lunadata->map.DirRandom) {
			RandomGet(lunadata);
			if(!(lunadata->RandomNum & 0x80)) {
				if(lunadata->map.FloorDir != 1)
					lunadata->map.FloorDir++;
			} else {
				if(lunadata->map.FloorDir != -1)
					lunadata->map.FloorDir--;
			}
		}
		if(lunadata->map.FloorDir == 1) {
			if((23 - lunadata->map.CeilHeight - lunadata->map.FloorHeight) < lunadata->map.MinGap)
				lunadata->map.FloorDir = 0;
		} else {
			lunadata->map.FloorHeight += lunadata->map.FloorDir;
			if(lunadata->map.FloorHeight <= lunadata->map.MinHeight) {
				lunadata->map.FloorDir = 0;
				lunadata->map.FloorHeight = lunadata->map.MinHeight;
			}
		}
	}
	
	//Clear column
	for(int i = 0; i < 24; i++)
		lunadata->map.ColData[i] = 0;
	
	// Apply ceil
	for(int i = lunadata->map.CeilHeight; i >= 0; i--)
		lunadata->map.ColData[i] = 135;
	lunadata->map.ColData[lunadata->map.CeilHeight - 1] = 132;
	
	// Apply floor
	mod = 24 - lunadata->map.FloorHeight;
	if(lunadata->map.FloorDir < 0)
		mod--;
	
	// Reset char
	ch = 135;

	// Draw Floor
	x = lunadata->map.FloorHeight;
	if(lunadata->map.FloorDir < 0) {
		x++;
	} else {
		x--;
		lunadata->map.ColData[mod + x] = ch;
	}
	while (x > 0) {
		if(x == 1) {
			if(lunadata->map.FloorDir > 0) {
				ch = lunadata->map.GrassTop[2];
			} else {
				if(lunadata->map.FloorDir < 0) {
					ch = lunadata->map.GrassTop[0];
				} else {
					ch = 129;
				}
			}
		}
		x--;
		lunadata->map.ColData[mod + x] = ch;
	}
	
	// Decorate floor
	// Fix ceiling
	// Add random holes and fix the floor slopes
}

void MapAdvanceMap(maindata *lunadata) {
	lunadata->map.NeedToShift = 0;
	lunadata->map.Hscroll -= lunadata->map.HscrollSpeed;
	if(lunadata->map.Hscroll < 0) {
		lunadata->map.Hscroll = lunadata->map.Hscroll & 7;
		MapGenerateColumn(lunadata);
		lunadata->map.NeedToShift = 1;
	}
}

void MapScreenShift(maindata *lunadata) {
	if(lunadata->map.NeedToShift) {
		for(int y = 0; y < 24; y++) {
			for(int x = 1; x < 40; x++)
				lunadata->SCREEN[y * 40 + x - 1] = lunadata->SCREEN[y * 40 + x];
			lunadata->SCREEN[y * 40 + 39] = lunadata->map.ColData[y];
		}
	}
}

void MapChangeMapColors(maindata *lunadata) {
}
