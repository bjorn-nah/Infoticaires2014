#include "huc.h"
#include "intro.h"

/* include psg / mml player. */
#include "sound/psglib.c"
#include "snginit.c"

#incbin(twistmap,"introinfo.fmp");
#incchr_ex(twisttiles,"titre02.pcx",0,0,20,7,0,\
			"twistauto.pcx",0,0,32,2,0);
#incpal(twistpal,"titre02.pcx");

#define COS_FIRST  128
#define COS_SECOND 256
#define COS_THIRD  384
#define COS_FOURTH 512
#define DEC_TULE 140


int VSyncCnt, TimerCnt, MainCnt,  SubCnt;

/* table des cosinus pour [0,PI/2] (ie) [0,128(inclus)]*/
const int cosSinTable[129]=
{
256,255,255,255,255,255,255,255,254,254,254,253,253,252,252,251,251,
250,249,249,248,247,246,245,244,244,243,242,241,239,238,237,236,
235,234,232,231,230,228,227,225,224,222,221,219,217,216,214,212,
211,209,207,205,203,201,199,197,195,193,191,189,187,185,183,181,
178,176,174,171,169,167,164,162,159,157,155,152,149,147,144,142,
139,136,134,131,128,126,123,120,117,115,112,109,106,103,100,97,
95,92,89,86,83,80,77,74,71,68,65,62,59,56,53,49,
46,43,40,37,34,31,28,25,21,18,15,12,9,6,3,0
};

main()
{
	int  song;
	int  psgMainStatus;
	int  psgSubStatus;

	int time, frame, tule, decal, dist, xtemp;
	int xabs, yord, decx, decy, vy, vtemp, stemp;

	char aff;
	int line, col, page, curr;
	int eff, effx, effy;

	init_satb();

	time = 0;
	curr = 0;
	eff = 0;
	xabs = 24;
	satb_update();  
	set_map_data(twistmap,32,28);
	set_tile_data(twisttiles);
	load_tile(0x1000);

	load_map(0,0,0,0,32,28);
	load_palette(0,twistpal,1); 

	/*--------------------------*/
	/* set up psg / mml player. */
	/*--------------------------*/
	psgInit(5);
	psgOn(0); 
	psgPlay(0);

	/*-------------------------*/
	/* init &run / mml player. */
	/*-------------------------*/
	sngInit();
	psgDelay( 0 );

	/* bidouille 2000 */

	set_color_rgb(2, 7, 7, 7);
	set_font_color(2, 0);
	set_font_pal(0);
	load_default_font();

	/* boucle principale */
	for (;;)
	{	
		/* trucs propres à squirrel */
		psgMainStatus = psgMStat();
		psgSubStatus  = psgSStat();

		/*--------------------------*/
		/* gestion du texte         */
		/*--------------------------*/		

		/* init de la val d'effacement */
		if (time % 400 == 380)
		{effx = 2;}

		/* routine d'effacement du texte */
		if (time % 400 > 380)
		{
			for(effy = 10; effy < 26; effy++)
			{
				if (effx < 20)
				{
				aff = text[1];
				put_char(aff, effx, effy);
				}	
			}
			effx++;
		}

		/* routine d'affichage du texte */
		if (time % 400 < 288)
		{
			aff = text[curr];
			line = curr / 18 % 16 + 10;
			col = curr % 18 + 2;
			put_char(aff, col, line);
			curr++;
		}

		/* loop du texte */
		if (curr >= sizeof(text))
		{curr = 0;}


		/*--------------------------*/
		/* twistbar                 */
		/*--------------------------*/
		time++;
		for(yord = 0; yord < 28; yord++)
		{
			/* comportement de la torsion */
			vy = (time - yord) % 512; ;
			xtemp = (time + yord) % 512;
			sin(xtemp, &stemp);
			cos(vy, &vtemp);
			dist = (vtemp * 3 - stemp) / 7;
			
			/* affichage des tules correspondantes */
			frame = dist % 16;
			for(tule = 0; tule < 4; tule++)
			{
				decal = (frame * 4) + tule;
				
				decx = xabs + tule;
				decy = yord;
				put_tile(decal + DEC_TULE, decx, decy);
				
			}
			
		}

		satb_update();
		vsync();
		
	}
}

/* fake cosinus : by Mooz */

/*--------------------------*/
/* fake cosinus             */
/*--------------------------*/

cos(val, result)
int val, *result;
{
  if(val > COS_FOURTH)
    val = val - COS_FOURTH;
  
  if(val <= COS_FIRST)
  {
    *result = cosSinTable[val];
    return;
  } 
                  
  if(val <= COS_SECOND)
  {
    *result = -cosSinTable[COS_SECOND - val];
    return;
  }  

  if(val <= COS_THIRD)
  {  
    *result = -cosSinTable[val - COS_SECOND];
    return;
  }  

  *result = cosSinTable[COS_FOURTH - val];      
}


/*--------------------------*/
/* fake sinus               */
/*--------------------------*/

sin(val, result)
int val, *result;
{
  if(val <= COS_FIRST)
    *result = cosSinTable[COS_FIRST - val];
  else  
    if(val <= COS_SECOND)
      *result = cosSinTable[val - COS_FIRST];
    else
      if(val <= COS_THIRD)
        *result = -cosSinTable[COS_THIRD - val];
      else
          *result = -cosSinTable[val - COS_THIRD];      
}
