#ifndef enemies_h
#define enemies_h

struct enemies {
	int SectorTransition = 0;
};

void EnemiesInit(struct maindata *lunadata);
void EnemiesAddWave(struct maindata *lunadata);
void EnemiesUpdate(struct maindata *lunadata);
void EnemiesDraw(struct maindata *lunadata);
void EnemiesCheckVsBullets(struct maindata *lunadata);
void EnemiesDoWaveAction(struct maindata *lunadata);

#endif
