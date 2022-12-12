


#include <stdio.h>
#include <allegro.h>
#include <time.h>


enum Zaznaczenie_t {brak, ZazLewy, ZazPrawy};

enum stan_gry_t {poczatek_gry, w_trakcie, stan_przegrana, wygrana};

struct Pole
{
    int WspX;
    int WspY;
    int ilosc_bomb;
    BITMAP *bm;
    enum Zaznaczenie_t zazn;


};



int mx = 0, my = 0, mb = 0;
void myszka()
{
    if( mx != mouse_x || my != mouse_y || mb != mouse_b )
    {
        mx = mouse_x+5;
        my = mouse_y;
        mb = mouse_b;
    }
};
#define OKNO_H 480
#define OKNO_W 640

#define ROZM_PLANSZY 8

#define WIELNOSC_PRZCISKU   20
#define ODSTEP_PRZCISKU     2
#define ILOSC_BOMB          1

enum rodzaj_wskaznika {brak_wskaznika, wskaznik_czcionka,wskaznik_ogolny,wskaznik_bitmapa};




union wskazniki
{
    FONT *czcionka;
    void * ogolny;
    BITMAP *Bitmapa;

};

struct element_listy
{
    struct element_listy *Poprzedni;
    enum rodzaj_wskaznika RodzajWskaznika;
    union wskazniki Wskaznik;
    struct element_listy *Nastepny;
};









void odslon_pole (int x, int y,struct Pole ** Plansza, BITMAP *bomba,int  *IloscOdslonietychPol)
{
    if((x>=0&&y>=0) &&(x<ROZM_PLANSZY&&y<ROZM_PLANSZY))
    {
        if (Plansza[x][y].zazn == brak)
        {
            char tekst[5];

            if(Plansza[x][y].ilosc_bomb == -1)
            {
                sprintf(tekst,"B");





                //    blit( Plansza[x][y], screen, 0, 0, Plansza[x][y].WspX, Plansza[x][y].WspY, WIELNOSC_PRZCISKU, WIELNOSC_PRZCISKU );
            }
            else
            {
                (*IloscOdslonietychPol)++;

                sprintf(tekst,"%d ",Plansza[x][y].ilosc_bomb);
            }

            clear_to_color( Plansza[x][y].bm, makecol( 170, 170, 170 ) );

            if(Plansza[x][y].ilosc_bomb !=0)
            {

                if(Plansza[x][y].ilosc_bomb ==-1)
                {
                    masked_blit( bomba, Plansza[x][y].bm, 0, 0, 0, 0, bomba->w, bomba->h );
                }
                else
                {
                    textout_ex(Plansza[x][y].bm,
                               font,
                               tekst,//
                               5,
                               5,
                               makecol(255,0,0), makecol(170,170,170));
                }


            }


            blit( Plansza[x][y].bm, screen, 0, 0, Plansza[x][y].WspX, Plansza[x][y].WspY, WIELNOSC_PRZCISKU, WIELNOSC_PRZCISKU );

            Plansza[x][y].zazn = ZazLewy;

            if(Plansza[x][y].ilosc_bomb ==0)
            {
                //odslon_pole( x-1,  y-1,Plansza);
                odslon_pole( x-1,  y,Plansza,bomba, IloscOdslonietychPol);
                // odslon_pole( x-1,  y+1,Plansza);
                odslon_pole( x,  y-1,Plansza,bomba, IloscOdslonietychPol);
                odslon_pole( x,  y+1,Plansza,bomba, IloscOdslonietychPol);
                //odslon_pole( x+1,  y-1,Plansza);
                odslon_pole( x+1,  y,Plansza,bomba,IloscOdslonietychPol);
                //odslon_pole( x+1,  y+1,Plansza);
            }

        }
    }





}

void zwieksz_ilosc_bomb(int x, int y,struct Pole ** Plansza)
{
    if (x>=0 && x < ROZM_PLANSZY  && y>=0 && y < ROZM_PLANSZY && Plansza[x][y].ilosc_bomb!=-1)
    {
        Plansza[x][y].ilosc_bomb++;
    }
}

void aktualizuj_liczniki( BITMAP * BitmapLiczniki, int licznik_flag, int czas,BITMAP * Bitmapsmile)
{
    char text1[10];
    char text2[10];
    int Poczatek_x = (OKNO_W - (ROZM_PLANSZY * WIELNOSC_PRZCISKU + (ROZM_PLANSZY-1)* ODSTEP_PRZCISKU )) /2;
    clear_to_color( BitmapLiczniki, makecol( 0, 0, 0 ) );

    sprintf(text1,"%03d",licznik_flag );
    sprintf(text2,"%03d",czas );


    masked_blit( Bitmapsmile, BitmapLiczniki, 0, 0, ( BitmapLiczniki->w  )/2- Bitmapsmile->w/2,3, Bitmapsmile->w, Bitmapsmile->h);



    textout_ex(BitmapLiczniki,
               font,
               text1,//
               0,
               5,
               makecol(255,0,0),
               makecol( 0,0,0) );

    textout_ex(BitmapLiczniki,
               font,
               text2,//
               150,
               5,
               makecol(255,0,0),
               makecol( 0,0,0) );


    blit( BitmapLiczniki, screen, 0, 0, Poczatek_x, 38, BitmapLiczniki->w, BitmapLiczniki->h);

}

void DodajBitmapeDoListy(struct element_listy *Lista,BITMAP *bitmapa )
{
    struct element_listy *wsk, *poprzedni = NULL;

    int Licznik = 0;

    wsk = Lista;

    while ( wsk->RodzajWskaznika != brak_wskaznika && wsk->Nastepny!= NULL)
    {
        wsk = wsk->Nastepny;
        Licznik++;
    }

    if(wsk->RodzajWskaznika ==brak_wskaznika )
    {
        wsk->RodzajWskaznika = wskaznik_bitmapa;
        wsk->Wskaznik.Bitmapa = bitmapa;
        Licznik++;
    }
    else
    {
        wsk->Nastepny = (struct element_list *)malloc (sizeof(struct element_listy ) );

        poprzedni = wsk;
        Licznik++;
        wsk = wsk->Nastepny;
        wsk->Poprzedni = poprzedni;
        wsk->RodzajWskaznika = wskaznik_bitmapa;
        wsk->Wskaznik.Bitmapa = bitmapa;
        wsk->Nastepny = NULL;
        Licznik++;

    }

    printf("ilosc elementow w liscie: %d\n",Licznik);
}


void DodajWskaznikOgolnyDoListy(struct element_listy *Lista,void* pWskaznik )
{
    struct element_listy *wsk, *poprzedni = NULL;

    int Licznik = 0;

    wsk = Lista;

    while ( wsk->RodzajWskaznika != brak_wskaznika && wsk->Nastepny!= NULL)
    {
        wsk = wsk->Nastepny;
        Licznik++;
    }

    if(wsk->RodzajWskaznika ==brak_wskaznika )
    {
        wsk->RodzajWskaznika = wskaznik_ogolny;
        wsk->Wskaznik.ogolny = pWskaznik;
        Licznik++;
    }
    else
    {
        wsk->Nastepny = (struct element_list *)malloc (sizeof(struct element_listy ) );


        poprzedni = wsk;
        Licznik++;
        wsk = wsk->Nastepny;
        wsk->Poprzedni = poprzedni;
        wsk->RodzajWskaznika = wskaznik_ogolny;
        wsk->Wskaznik.ogolny = pWskaznik;
        wsk->Nastepny = NULL;
        Licznik++;

    }

    printf("ilosc elementow w liscie: %d\n",Licznik);
}


void DodajCzcionkeDoListy(struct element_listy *Lista,FONT *czcionka )
{
    struct element_listy *wsk, *poprzedni = NULL;

    int Licznik = 0;

    wsk = Lista;

    while ( wsk->RodzajWskaznika != brak_wskaznika && wsk->Nastepny!= NULL)
    {
        Licznik++;
        wsk = wsk->Nastepny;

    }

    if(wsk->RodzajWskaznika ==brak_wskaznika )
    {
        wsk->RodzajWskaznika = wskaznik_czcionka;
        wsk->Wskaznik.czcionka = czcionka;
        Licznik++;
    }
    else
    {
        wsk->Nastepny = (struct element_list *)malloc (sizeof(struct element_listy ) );


        poprzedni = wsk;
        Licznik++;
        wsk = wsk->Nastepny;
        wsk->Poprzedni = poprzedni;
        wsk->RodzajWskaznika = wskaznik_czcionka;
        wsk->Wskaznik.czcionka = czcionka;
        wsk->Nastepny = NULL;
        Licznik++;

    }

    printf("ilosc elementow w liscie: %d\n",Licznik);
}


void ZwolnijListe(struct element_listy *Lista)
{
    struct element_listy *wsk;;

    int Licznik = 0;

    wsk = Lista;

    if (wsk->RodzajWskaznika != brak_wskaznika)
    {
        Licznik++;
    }

    while ( wsk->RodzajWskaznika != brak_wskaznika && wsk->Nastepny!= NULL)
    {
        wsk = wsk->Nastepny;
        if (wsk->RodzajWskaznika != brak_wskaznika)
        {
            Licznik++;
        }

    }

    printf("Zliczono %d elementow w liscie\n", Licznik);

    while (wsk != NULL)
    {
        if(wsk->RodzajWskaznika == wskaznik_bitmapa )
        {
            destroy_bitmap( wsk->Wskaznik.Bitmapa);
            wsk->Wskaznik.Bitmapa = NULL;
            Licznik--;
        }

        if(wsk->RodzajWskaznika == wskaznik_czcionka )
        {
            destroy_font( wsk->Wskaznik.czcionka);
            wsk->Wskaznik.czcionka = NULL;
            Licznik--;
        }

        if(wsk->RodzajWskaznika == wskaznik_ogolny )
        {
            free (wsk->Wskaznik.ogolny);

            wsk->Wskaznik.ogolny = NULL;
            Licznik--;
        }



        wsk = wsk->Poprzedni;

        if (wsk != NULL && wsk->Nastepny != NULL)
        {
            free(wsk->Nastepny);
            wsk->Nastepny = NULL;
        }

    }

    printf("ilosc elementow po zwolnieniu pamieci: %d\n",Licznik);
}


int main()
{
    int licznik_flag= ILOSC_BOMB;
    int poprzedni_licznik_flag= ILOSC_BOMB;

    int licznik_czasu = 0;
    int poprzedni_licznik_czasu = 0;
    int zliczaj_czas=0;

    enum stan_gry_t StanGry = poczatek_gry;
    enum stan_gry_t poprzedni_StanGry= poczatek_gry;

    int StartTime;
    int ActualTime;
    int i=0, j=0;
    int x = 0;
    int y = 0;
    int Poczatek_x = 0;

    struct element_listy Lista;

    Lista.Nastepny = NULL;
    Lista.Poprzedni = NULL;
    Lista.RodzajWskaznika= brak_wskaznika;

//   int tab[10];
    /*
        int * tab2;
        tab2 = calloc (sizeof (int),10 );

        free (tab2);

        int tabWW[10][20];

        int *tabWW2[10];
    */
    srand( time( NULL ) );

    time (&StartTime);
    time (&ActualTime);


    int ilosc_odslonietych_pol=0;
    FONT *czcionka = NULL;


    struct Pole **TablicaZPolami=NULL;

    allegro_init();
    set_color_depth( 16 );
    set_palette( default_palette );



    TablicaZPolami = calloc (sizeof (struct Pole *),ROZM_PLANSZY );

    if (TablicaZPolami==NULL)
    {
        set_gfx_mode( GFX_TEXT, 0, 0, 0, 0 );
        allegro_message( "blad alokacji" );
        ZwolnijListe(&Lista);
        allegro_exit();
        return 0;
    }
    DodajWskaznikOgolnyDoListy(&Lista,TablicaZPolami);

    for (i=0; i<ROZM_PLANSZY; i++)
    {
        TablicaZPolami[i] = calloc (sizeof (struct Pole),ROZM_PLANSZY );

        if (TablicaZPolami[i]==NULL)
        {
            set_gfx_mode( GFX_TEXT, 0, 0, 0, 0 );
            allegro_message( "blad alokacji" );
            ZwolnijListe(&Lista);
            allegro_exit();
            return 0;
        }

        DodajWskaznikOgolnyDoListy(&Lista,TablicaZPolami[i]);

        for (j=0; j<ROZM_PLANSZY; j++)
        {
            TablicaZPolami[i][j].bm = create_bitmap( WIELNOSC_PRZCISKU, WIELNOSC_PRZCISKU );

            if (TablicaZPolami[i][j].bm==NULL)
            {
                set_gfx_mode( GFX_TEXT, 0, 0, 0, 0 );
                allegro_message( "blad alokacji" );
                ZwolnijListe(&Lista);
                allegro_exit();
                return 0;
            }
            DodajBitmapeDoListy(&Lista, TablicaZPolami[i][j].bm  );
        }

    }




    install_keyboard();

    set_gfx_mode(GFX_AUTODETECT_WINDOWED, OKNO_W,OKNO_H,0,0);


    czcionka = load_font("czcionka.pcx", NULL,NULL);

    if( !czcionka )
    {

        set_gfx_mode( GFX_TEXT, 0, 0, 0, 0 );
        allegro_message( "nie udalo sie wczytac czcionki !" );
        ZwolnijListe(&Lista);
        allegro_exit();
        return 0;
    }

    DodajCzcionkeDoListy(&Lista,czcionka);


    Poczatek_x = (OKNO_W - (ROZM_PLANSZY * WIELNOSC_PRZCISKU + (ROZM_PLANSZY-1)* ODSTEP_PRZCISKU )) /2;

    BITMAP * BitmapLiczniki = NULL;
    BitmapLiczniki = create_bitmap( (WIELNOSC_PRZCISKU + ODSTEP_PRZCISKU) *ROZM_PLANSZY, 45 );

    if (BitmapLiczniki==NULL)
    {
        set_gfx_mode( GFX_TEXT, 0, 0, 0, 0 );
        allegro_message( "blad alokacji" );
        ZwolnijListe(&Lista);
        allegro_exit();
        return 0;
    }
    DodajBitmapeDoListy(&Lista, BitmapLiczniki );

    BITMAP * BitmapStopka = NULL;
    BitmapStopka = create_bitmap( (WIELNOSC_PRZCISKU + ODSTEP_PRZCISKU) *ROZM_PLANSZY+50, 90 );

    if (BitmapStopka==NULL)
    {
        set_gfx_mode( GFX_TEXT, 0, 0, 0, 0 );
        allegro_message( "blad alokacji" );
        ZwolnijListe(&Lista);
        allegro_exit();
        return 0;
    }

    DodajBitmapeDoListy(&Lista, BitmapStopka );

    BITMAP * smile = NULL;

    smile = load_bmp( "smile.bmp", default_palette );
    if( !smile )
    {

        set_gfx_mode( GFX_TEXT, 0, 0, 0, 0 );
        allegro_message( "nie mogę załadować obrazka smile !" );
        ZwolnijListe(&Lista);
        allegro_exit();
        return 0;
    }
    DodajBitmapeDoListy(&Lista, smile );

    BITMAP * przegrana = NULL;


    przegrana = load_bmp( "przegrana.bmp", default_palette );
    if( !przegrana )
    {
        set_gfx_mode( GFX_TEXT, 0, 0, 0, 0 );
        allegro_message( "nie mogę załadować obrazka przegrana !" );
        ZwolnijListe(&Lista);
        allegro_exit();
        return 0;
    }
    DodajBitmapeDoListy(&Lista, przegrana );
    BITMAP *flaga = load_bmp( "flaga2.bmp", default_palette  );
    if( !flaga )
    {
        set_gfx_mode( GFX_TEXT, 0, 0, 0, 0 );
        allegro_message( "Nie moge zaladowac grafiki ( flaga )" );
        ZwolnijListe(&Lista);
        allegro_exit();
    }
    DodajBitmapeDoListy(&Lista, flaga );
    BITMAP *bomba = load_bmp( "bomba.bmp", default_palette  );
    if( !bomba )
    {
        set_gfx_mode( GFX_TEXT, 0, 0, 0, 0 );
        allegro_message( "Nie moge zaladowac grafiki ( bomba )" );
        ZwolnijListe(&Lista);
        allegro_exit();
    }
    DodajBitmapeDoListy(&Lista, bomba );

    BITMAP * bufor = NULL;
    bufor = create_bitmap( 640, 20 );
    DodajBitmapeDoListy(&Lista, bufor );

    BITMAP * kursor;


    kursor = load_bmp( "kursor.bmp", default_palette  );
    if( !kursor )
    {
        set_gfx_mode( GFX_TEXT, 0, 0, 0, 0 );
        allegro_message( "Nie moge zaladowac grafiki ( kursor )" );
        ZwolnijListe(&Lista);
        allegro_exit();
    }
    DodajBitmapeDoListy(&Lista, kursor);
    install_mouse();
    enable_hardware_cursor();

    show_mouse( screen );



    set_mouse_sprite( kursor );
    unscare_mouse();


    int PrevMb =0;

    int WybIndexX=-1;
    int WybIndexY=-1;
    while( !key[ KEY_ESC ] )
    {
        if(StanGry == poczatek_gry)
        {
            ilosc_odslonietych_pol=0;
            licznik_flag= ILOSC_BOMB;
            poprzedni_licznik_flag= ILOSC_BOMB;

            licznik_czasu = 0;
            poprzedni_licznik_czasu = 0;
            zliczaj_czas=0;
            aktualizuj_liczniki(  BitmapLiczniki,  licznik_flag,  licznik_czasu,smile);

            for (i= 0; i < ROZM_PLANSZY; i++)
            {
                for (j= 0; j < ROZM_PLANSZY; j++)
                {
                    x = Poczatek_x+ j*(WIELNOSC_PRZCISKU + ODSTEP_PRZCISKU);
                    y = 100+ i*(WIELNOSC_PRZCISKU + ODSTEP_PRZCISKU);

                    TablicaZPolami[i][j].WspX = x;
                    TablicaZPolami[i][j].WspY = y;
                    TablicaZPolami[i][j].zazn = brak;
                    TablicaZPolami[i][j].ilosc_bomb = 0;

                    clear_to_color( TablicaZPolami[i][j].bm, makecol( 96, 96, 96 ) );

                }
            }

            int ilosc_wylosowanych=0;
            while (ilosc_wylosowanych<ILOSC_BOMB)
            {
                int wyl_X = rand()%8;
                int wyl_Y = rand()%8;

                if (TablicaZPolami[wyl_X][wyl_Y].ilosc_bomb!=-1)
                {
                    TablicaZPolami[wyl_X][wyl_Y].ilosc_bomb = -1;
                    ilosc_wylosowanych++;
                }
            }



            for (i=0; i<ROZM_PLANSZY; i++)
            {
                for (j=0; j<ROZM_PLANSZY; j++)
                {
                    if (TablicaZPolami[i][j].ilosc_bomb==-1)
                    {

                        zwieksz_ilosc_bomb( i-1,  j-1,TablicaZPolami);
                        zwieksz_ilosc_bomb( i-1,  j,TablicaZPolami);
                        zwieksz_ilosc_bomb( i-1,  j+1,TablicaZPolami);
                        zwieksz_ilosc_bomb( i,  j-1,TablicaZPolami);
                        zwieksz_ilosc_bomb( i,  j+1,TablicaZPolami);
                        zwieksz_ilosc_bomb( i+1,  j-1,TablicaZPolami);
                        zwieksz_ilosc_bomb( i+1,  j,TablicaZPolami);
                        zwieksz_ilosc_bomb( i+1,  j+1,TablicaZPolami);


                    }
                }

            }

            printf ("\n");
            for (i=0; i<ROZM_PLANSZY; i++)
            {
                for (j=0; j<ROZM_PLANSZY; j++)
                {
                    printf ("%2d ",TablicaZPolami[i][j].ilosc_bomb );
                }
                printf ("\n");
            }

            for (i= 0; i < ROZM_PLANSZY; i++)
            {
                for (j= 0; j < ROZM_PLANSZY; j++)
                {

                    blit( TablicaZPolami[i][j].bm, screen, 0, 0, TablicaZPolami[i][j].WspX, TablicaZPolami[i][j].WspY, WIELNOSC_PRZCISKU, WIELNOSC_PRZCISKU );
                }
            }

            clear_to_color( BitmapStopka, makecol( 0, 0, 0 ) );

            Poczatek_x = (OKNO_W - (ROZM_PLANSZY * WIELNOSC_PRZCISKU + (ROZM_PLANSZY-1)* ODSTEP_PRZCISKU )) /2;

            blit( BitmapStopka, screen, 0, 0, Poczatek_x, 300, BitmapStopka->w, BitmapStopka->h );

            StanGry= w_trakcie;
        }

        int IndexX=-1;
        int Indexy=-1;

        rest(5);

        PrevMb =mb;

        clear_to_color( bufor, makecol( 100, 100, 100 ) );
        myszka();


        if(mb==2 && PrevMb !=mb && StanGry== w_trakcie)
        {
            for(i=0; i<ROZM_PLANSZY; i++)
            {
                for(j=0; j<ROZM_PLANSZY; j++)
                {
                    if (mx >= TablicaZPolami[i][j].WspX && mx <= (TablicaZPolami[i][j].WspX+WIELNOSC_PRZCISKU) )
                    {

                        if(my >= TablicaZPolami[i][j].WspY && my <= (TablicaZPolami[i][j].WspY+WIELNOSC_PRZCISKU) )
                        {
                            IndexX=i;
                            Indexy=j;

                            WybIndexX=IndexX;
                            WybIndexY=Indexy;



                            if (TablicaZPolami[i][j].zazn == brak)
                            {

                                masked_blit( flaga, TablicaZPolami[i][j].bm, 0, 0, 5, 5, flaga->w, flaga->h );
                                blit( TablicaZPolami[i][j].bm, screen, 0, 0, TablicaZPolami[i][j].WspX, TablicaZPolami[i][j].WspY, WIELNOSC_PRZCISKU, WIELNOSC_PRZCISKU );


                                TablicaZPolami[i][j].zazn = ZazPrawy;


                                licznik_flag--;


                            }
                            else if (TablicaZPolami[i][j].zazn == ZazPrawy)
                            {
                                clear_to_color( TablicaZPolami[i][j].bm, makecol( 96, 96, 96 ) );
                                blit( TablicaZPolami[i][j].bm, screen, 0, 0, TablicaZPolami[i][j].WspX, TablicaZPolami[i][j].WspY, WIELNOSC_PRZCISKU, WIELNOSC_PRZCISKU );
                                TablicaZPolami[i][j].zazn = brak;

                                licznik_flag++;

                            }

                            break;
                        }
                    }
                }
                if (IndexX>=0)
                {
                    break;
                }
            }
        }

        if(mb==1 && PrevMb !=mb )
        {
            if (StanGry== w_trakcie)
            {

                for(i=0; i<ROZM_PLANSZY; i++)
                {
                    for(j=0; j<ROZM_PLANSZY; j++)
                    {
                        if (mx >= TablicaZPolami[i][j].WspX && mx <= (TablicaZPolami[i][j].WspX+WIELNOSC_PRZCISKU) )
                        {

                            if(my >= TablicaZPolami[i][j].WspY && my <= (TablicaZPolami[i][j].WspY+WIELNOSC_PRZCISKU) )
                            {
                                IndexX=i;
                                Indexy=j;

                                WybIndexX=IndexX;
                                WybIndexY=Indexy;

                                if (zliczaj_czas==0)
                                {
                                    zliczaj_czas=1;
                                    time (&StartTime);
                                }

                                if (TablicaZPolami[i][j].ilosc_bomb==-1 )
                                {

                                    for (int k= 0; k < ROZM_PLANSZY; k++)
                                    {
                                        for (int l= 0; l < ROZM_PLANSZY; l++)
                                        {

                                            if (TablicaZPolami[k][l].ilosc_bomb==-1 )
                                            {
                                                odslon_pole (k,l,TablicaZPolami,bomba, & ilosc_odslonietych_pol );
                                            }

                                        }
                                    }

                                    StanGry=stan_przegrana;
                                    break;

                                }


                                odslon_pole (i,j,TablicaZPolami,bomba,& ilosc_odslonietych_pol );

                                if(ilosc_odslonietych_pol==ROZM_PLANSZY*ROZM_PLANSZY-ILOSC_BOMB)
                                {
                                    clear_to_color( BitmapStopka, makecol( 0, 0, 0 ) );


                                    textout_ex(BitmapStopka,
                                               czcionka,

                                               "Wygrales!",//
                                               0,
                                               5,
                                               makecol(255,0,0),
                                               makecol( 0,0,0) );




                                    Poczatek_x = (OKNO_W - (ROZM_PLANSZY * WIELNOSC_PRZCISKU + (ROZM_PLANSZY-1)* ODSTEP_PRZCISKU )) /2;

                                    blit( BitmapStopka, screen, 0, 0, Poczatek_x, 300, BitmapStopka->w, BitmapStopka->h );


                                    StanGry=wygrana;
                                }

                                break;
                            }
                        }
                    }
                    if (IndexX>=0)
                    {
                        break;
                    }
                }
            }

            if (mx>300&& mx<340 && my >45&& my<84 )
            {
                StanGry = poczatek_gry;
            }
        }

        if (StanGry== w_trakcie)
        {
            if (zliczaj_czas==0)
            {
                licznik_czasu=0;
            }
            else
            {
                time (&ActualTime);
                licznik_czasu = ActualTime - StartTime;
            }
        }





        if ( licznik_flag!=poprzedni_licznik_flag || poprzedni_licznik_czasu != licznik_czasu || poprzedni_StanGry!=StanGry)
        {
            if (StanGry == stan_przegrana)
            {
                aktualizuj_liczniki(  BitmapLiczniki,  licznik_flag,  licznik_czasu,przegrana);
            }
            else
            {
                aktualizuj_liczniki(  BitmapLiczniki,  licznik_flag,  licznik_czasu,smile);
            }

            poprzedni_StanGry = StanGry;

            poprzedni_licznik_flag= licznik_flag;

            poprzedni_licznik_czasu = licznik_czasu;
        }


        textprintf_ex( bufor, font, 5, 5, makecol( 200, 200, 200 ), - 1, "Myszka x = %d, y = %d, klawisz = %d !, wyb pole: %d %d", mx, my, mb,WybIndexX+1,WybIndexY+ 1  );
        blit( bufor, screen, 0, 0, 0, 460, 640, 20 );

        clear_keybuf();

    }



    ZwolnijListe(&Lista);
    allegro_exit();
    return 0;


}
END_OF_MAIN()

