#ifndef hud_h
#define hud_h

void HudInit(struct maindata *lunadata);
void HudAddScore(struct maindata *lunadata);
void HudDisplayWaveNumber(struct maindata *lunadata);
void HudDecPower(struct maindata *lunadata);
void HudAddPower(struct maindata *lunadata);

#endif
