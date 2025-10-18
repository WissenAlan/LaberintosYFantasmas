#include "header/SDL_INIT.h"
#include "header/menus.h"
void menu_pausa(tGame *g)
{
    // creo los rectangulos de los botones
    SDL_Rect btnContinuar = { WINDOW_WIDTH/2 -150,WINDOW_HEIGHT/2-40,300,60};
    SDL_Rect btnGuardarySalir = {WINDOW_WIDTH/2 - 150,WINDOW_HEIGHT/2+40,300,60};
    // creo los rectangulos mas para el texto
    SDL_Rect botonc;
    SDL_Rect botons;
    SDL_Texture* textura;
    SDL_Surface * superficie;
    // pongo el renderer en un color para pintar ese rectangulo
    SDL_SetRenderDrawColor(g->renderer,169, 169, 169,255);
    SDL_RenderFillRect(g->renderer, &btnContinuar);
    SDL_SetRenderDrawColor(g->renderer,169, 169, 169,255);
    SDL_RenderFillRect(g->renderer,&btnGuardarySalir);
    SDL_Color white = {255,255,255,255};
    // boton continuar
    superficie= TTF_RenderText_Blended(g->text_f,"CONTINUAR",white);
    textura= SDL_CreateTextureFromSurface(g->renderer,superficie);
    SDL_FreeSurface(superficie);
    // pido el tamaño de la textura
    SDL_QueryTexture(textura, NULL, NULL, &botonc.w, &botonc.h);
    // marco el tamaño de los cuadrados donde va a ir  el texto
    botonc.x = btnContinuar.x + (btnContinuar.w - botonc.w)/2;
    botonc.y = btnContinuar.y + (btnContinuar.h - botonc.h)/2;
    SDL_RenderCopy(g->renderer,textura,NULL,&botonc);
    SDL_DestroyTexture(textura);

    // boton guardar y salir
    superficie= TTF_RenderText_Blended(g->text_f,"Salir Al Menu",white);
    textura= SDL_CreateTextureFromSurface(g->renderer,superficie);
    SDL_FreeSurface(superficie);
    SDL_QueryTexture(textura, NULL, NULL, &botons.w, &botons.h);
    botons.x = btnGuardarySalir.x + (btnGuardarySalir.w - botons.w)/2;
    botons.y = btnGuardarySalir.y + (btnGuardarySalir.h - botons.h)/2;
    SDL_RenderCopy(g->renderer,textura,NULL,&botons);

    SDL_DestroyTexture(textura);

    // se carga en pantalla
    SDL_RenderPresent(g->renderer);
    while(g->is_pausing)
    {
        // se verifican los eventos
        while(SDL_PollEvent(&g->eventos))
        {
            if(g->eventos.type == SDL_QUIT)
            {
                g->inicio = false;
                g->is_running = false;
                g->is_pausing=false;
            }
            if(g->eventos.type == SDL_KEYDOWN)
            {
                if(g->eventos.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                    g->is_pausing = false;
            }
            if(g->eventos.type == SDL_MOUSEBUTTONDOWN && g->eventos.button.button == SDL_BUTTON_LEFT )
            {
                SDL_Point click = {g->eventos.button.x,g->eventos.button.y};
                if(SDL_PointInRect(&click,&btnContinuar))
                {
                    g->is_pausing = false;
                    Mix_PlayChannel(-1,g->sonidomenu,0);
                }

                if(SDL_PointInRect(&click,&btnGuardarySalir))
                {
                    //GuardarPatron(g->matriz, filas_R, columnas_R);

                    g->custom=false;
                    g->is_pausing=false;
                    g->inicio=true;
                    Mix_PlayChannel(-1,g->sonidomenu,0);
                }

            }


        }
    }
}
void subMenuCustom(tGame *g)
{
    bool mouseclickLeft=false,mouseclickRight=false;
//    inicializarmatriz(g->matriz,filas_R,columnas_R);
    while(g->custom && g->is_running)
    {
        while(SDL_PollEvent(&g->eventos))
        {
            switch(g->eventos.type)
            {
            case SDL_QUIT:
                g->is_running=false;
                g->custom=false;
                g->inicio=false;
                break;
            case SDL_KEYDOWN:
                switch(g->eventos.key.keysym.scancode)
                {
                case SDL_SCANCODE_ESCAPE:
                    g->is_pausing=true;
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
        }
        game_drawlab(g);
        SDL_Delay(15);
        if(g->is_pausing)
        {
            menu_pausa(g);
        }
    }
    // funcion para guardar en archivo proximamente parte 3
}
/*
void matriz_draw(struct game *g)
{
    int i, j;
    SDL_Rect rect = {0,0,SIZE-1,SIZE-1};
    SDL_SetRenderDrawColor(g->renderer,CUADRADO_COLOR);
    for(i=0; i<filas_R; i++)
    {
        rect.y= SIZE*i;
        for(j=0; j<columnas_R; j++)
        {
            rect.x=SIZE *j;
            if(g->matriz[i][j] == Celula_Viva)
            {
                g->estilo(g,rect);
            }

        }
    }

}*/
void game_drawlab(tGame *g)
{
    char mapa[16][16]={{'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},
            {'.','#','.','#','.','#','.','#','.','#','.','#','.','#','.','#'},
            {'.','.','.','#','.','#','.','#','.','#','.','#','.','#','.','#'},
            {'#','.','#','#','.','#','.','#','.','#','.','#','.','#','.','#'}};
    int i,j;
    SDL_Rect rect = {0,0,30-3,30-3}; // padding 3
    SDL_SetRenderDrawColor(g->renderer,0,0,0,255);
    SDL_RenderClear(g->renderer);
    SDL_SetRenderDrawColor(g->renderer,CUADRADO_COLOR);
    for(i=0; i<16; i++)
    {
        rect.y= 30*i;
        for(j=0; j<16; j++)
        {
            rect.x=30*j;
            if(mapa[i][j] == '.')
            {

                SDL_RenderFillRect(g->renderer,&rect);
            }

        }
    }
   SDL_RenderPresent(g->renderer);
}
void cargarTexturasMenu(tGame *g,SDL_Rect botonP,SDL_Rect botonC,SDL_Rect botonI,SDL_Rect botonS)
{
    SDL_Color black = {255,255,255,255};
    SDL_Rect textP = {75,250};
    SDL_Rect textC = {50,330}; // custom
    SDL_Rect textI = {30,410}; // controles
    SDL_Rect textS  = {85,490};

    SDL_Texture* textura;
    SDL_Surface * superficie;


    SDL_RenderClear(g->renderer);
    SDL_RenderCopy(g->renderer,g->fondo,NULL,NULL);

    SDL_SetRenderDrawColor(g->renderer,169, 169, 169,255);

    SDL_RenderFillRect(g->renderer,&botonP);
    SDL_RenderFillRect(g->renderer,&botonI);
    //SDL_RenderFillRect(g->renderer,&botonS);
    SDL_RenderFillRect(g->renderer,&botonC);


    // boton play
    superficie= TTF_RenderText_Blended(g->titulo_f,"Jugar",black);
    textura= SDL_CreateTextureFromSurface(g->renderer,superficie);
    SDL_QueryTexture(textura, NULL, NULL, &textP.w, &textP.h);
    SDL_FreeSurface(superficie);
    textP.x = botonP.x + (botonP.w / 2) - (textP.w / 2);
    textP.y = botonP.y + (botonP.h / 2) - (textP.h / 2);
    SDL_RenderCopy(g->renderer,textura,NULL,&textP);
    SDL_DestroyTexture(textura);

    //boton guia
    superficie = TTF_RenderText_Blended(g->titulo_f,"Salir",black);
    textura= SDL_CreateTextureFromSurface(g->renderer,superficie);
    SDL_QueryTexture(textura, NULL, NULL, &textI.w, &textI.h);
    SDL_FreeSurface(superficie);
    textI.x = botonI.x + (botonI.w / 2) - (textI.w / 2);
    textI.y = botonI.y + (botonI.h / 2) - (textI.h / 2);
    SDL_RenderCopy(g->renderer,textura,NULL,&textI);
    SDL_DestroyTexture(textura);
    //boton custom
    superficie = TTF_RenderText_Blended(g->titulo_f,"Ranking",black);
    textura= SDL_CreateTextureFromSurface(g->renderer,superficie);
    SDL_QueryTexture(textura, NULL, NULL, &textC.w, &textC.h);
    SDL_FreeSurface(superficie);
    textC.x = botonC.x + (botonC.w / 2) - (textC.w / 2);
    textC.y = botonC.y + (botonC.h / 2) - (textC.h / 2);
    SDL_RenderCopy(g->renderer,textura,NULL,&textC);
    SDL_DestroyTexture(textura);

    /* //boton salir
    superficie = TTF_RenderText_Blended(g->titulo_f,"Salir",black);
    textura= SDL_CreateTextureFromSurface(g->renderer,superficie);
    SDL_QueryTexture(textura, NULL, NULL, &textS.w, &textS.h);
    SDL_FreeSurface(superficie);
    SDL_RenderCopy(g->renderer,textura,NULL,&textS);
    SDL_DestroyTexture(textura);*/
    // musica de menu
    Mix_HaltMusic();
    Mix_PlayMusic(g->musica,-1);
    SDL_RenderPresent(g->renderer);
}
void menu_inicio(tGame *g)
{
    SDL_Rect botonP = {20,250,240,60};
    SDL_Rect botonC = {20,330,240,60};   // custom
    SDL_Rect botonI  = {20,410,240,60};  // controles
    SDL_Rect botonS  = {20,490,240,60};
    SDL_Point click;
    cargarTexturasMenu(g,botonP,botonC,botonI,botonS);
    while(g->inicio)
    {
        // se verifican los eventos
        while(SDL_PollEvent(&g->eventos))
        {
            if(g->eventos.type == SDL_QUIT)
            {
                g->inicio = false;
                g->is_running = false;
            }
            if(g->eventos.type == SDL_KEYDOWN)
            {
                if(g->eventos.key.keysym.scancode == SDL_SCANCODE_M)
                {
                    Mix_HaltMusic();
                }
            }
            if(g->eventos.type == SDL_MOUSEBUTTONDOWN && g->eventos.button.button == SDL_BUTTON_LEFT )
            {
                click.x= g->eventos.button.x;
                click.y= g->eventos.button.y;
                if(SDL_PointInRect(&click,&botonS))
                {

                    g->inicio = false;
                    g->is_running = false;
                }
                if(SDL_PointInRect(&click,&botonP))
                {
                    g->inicio = false;
                    g->is_running=true;
                    Mix_PlayChannel(-1,g->sonidomenu,0);

                }
                if(SDL_PointInRect(&click,&botonC))
                {
                    g->custom=true;
                    subMenuCustom(g);
                    cargarTexturasMenu(g,botonP,botonC,botonI,botonS);
                }
                if(SDL_PointInRect(&click,&botonI))
                {
                    g->controles=true;
                    submenuguia(g);
                    cargarTexturasMenu(g,botonP,botonC,botonI,botonS);
                }
            }
        }
    }
    Mix_HaltMusic();
}
void submenuguia(tGame *g)
{
    int i;
    SDL_Texture* textura;
    SDL_Surface * superficie;
    SDL_Point click;
    SDL_Rect cuadradoCerrar ={WINDOW_WIDTH*0.455,WINDOW_HEIGHT-124,130,45}; //modificar por constantes
    SDL_RenderCopy(g->renderer,g->guia,NULL,NULL);// se carga la imagen

    SDL_RenderPresent(g->renderer);

        SDL_Rect rank_rect = { (WINDOW_WIDTH*0.20), 160, 100, 30}; // modificar por constantes
        SDL_Rect name_rect = { (WINDOW_WIDTH - 100) / 2, 160, 100, 30 }; // modificar por constantes
        SDL_Rect score_rect = { WINDOW_WIDTH - 300 - 120, 160, 120, 30 }; // modificar por constantes
        SDL_Surface * superficieR=TTF_RenderText_Blended(g->titulo_f,"RANK",(SDL_Color){255,255,255,255});
        SDL_Surface * superficieN = TTF_RenderText_Blended(g->titulo_f,"NAME",(SDL_Color){255,255,255,255});
        SDL_Surface * superficieS=TTF_RenderText_Blended(g->titulo_f,"SCORE",(SDL_Color){255,255,255,255});
        SDL_Texture* rank_tex = SDL_CreateTextureFromSurface(g->renderer,superficieR);
        SDL_Texture* name_tex = SDL_CreateTextureFromSurface(g->renderer,superficieN);
        SDL_Texture* score_tex = SDL_CreateTextureFromSurface(g->renderer,superficieS);


        SDL_RenderCopy(g->renderer, rank_tex, NULL, &rank_rect);
        SDL_RenderCopy(g->renderer, name_tex, NULL, &name_rect);
        SDL_RenderCopy(g->renderer, score_tex, NULL, &score_rect);
        for(i=0;i<10;i++)
        {
            rank_rect.y+=35;
            name_rect.y+=35;
            score_rect.y+=35;
            superficieR= TTF_RenderText_Blended(g->titulo_f,"1.",(SDL_Color){255,255,255,255});
            superficieN = TTF_RenderText_Blended(g->titulo_f,"JDPHA",(SDL_Color){255,255,255,255});
            superficieS= TTF_RenderText_Blended(g->titulo_f,"1000",(SDL_Color){255,255,255,255});
            rank_tex = SDL_CreateTextureFromSurface(g->renderer,superficieR);
            name_tex = SDL_CreateTextureFromSurface(g->renderer,superficieN);
            score_tex = SDL_CreateTextureFromSurface(g->renderer,superficieS);
            SDL_RenderCopy(g->renderer, rank_tex, NULL, &rank_rect);
            SDL_RenderCopy(g->renderer, name_tex, NULL, &name_rect);
            SDL_RenderCopy(g->renderer, score_tex, NULL, &score_rect);
            SDL_FreeSurface(superficieR);
            SDL_FreeSurface(superficieN);
            SDL_FreeSurface(superficieS);
            SDL_DestroyTexture(rank_tex);
            SDL_DestroyTexture(name_tex);
            SDL_DestroyTexture(score_tex);
        }


    SDL_RenderPresent(g->renderer);
    while(g->controles)
    {
        while(SDL_PollEvent(&g->eventos))
        {
            switch(g->eventos.type)
            {
            case SDL_QUIT:
                g->is_running=false;
                g->custom=false;
                g->inicio=false;
                g->controles=false;
                break;
            case SDL_KEYDOWN:
                switch(g->eventos.key.keysym.scancode)
                {
                case SDL_SCANCODE_ESCAPE:
                    g->controles=false;
                    break;
                default:
                    break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if(g->eventos.button.button == SDL_BUTTON_LEFT)
                {
                    click.x= g->eventos.button.x;
                    click.y= g->eventos.button.y;
                    if(SDL_PointInRect(&click,&cuadradoCerrar))
                    {
                        g->controles=false;
                    }
                }
                break;

            default:
                break;
            }
        }
    }

}
