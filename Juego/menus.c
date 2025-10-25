#include "header/SDL_INIT.h"
#include "header/menus.h"
void crearBoton(SDL_Rect cuadrado,TTF_Font* fuente,SDL_Texture* textura,SDL_Color color,char *texto,SDL_Renderer* render,int ajustar)
{
    SDL_Surface * superficie=TTF_RenderText_Blended(fuente,texto,color);
    SDL_RenderCopy(render,textura,NULL,&cuadrado);
    textura= SDL_CreateTextureFromSurface(render,superficie);
    if(ajustar == VERDADERO)
    {
        SDL_QueryTexture(textura, NULL, NULL, &cuadrado.w, &cuadrado.h);
    }
    SDL_RenderCopy(render,textura,NULL,&cuadrado);
    SDL_FreeSurface(superficie);
}
void menu_pausa(tGame *g)
{

    char puntaje[TAM_BUFFER];
    SDL_Color white = {255,255,255,255}; // color RGB
    /// Lo sumado a las coordenadas de rectangulo es el desplazamiento para centrarlos
    SDL_Rect resultadoGame={(WINDOW_WIDTH - ANCHOCUADRADO)/2,(WINDOW_HEIGHT - LARGOCUADRADO)/2,ANCHOCUADRADO,LARGOCUADRADO};
    SDL_Rect textoResult={(WINDOW_WIDTH - ANCHOCUADRADO)/2 + 10,(WINDOW_HEIGHT - LARGOCUADRADO)/2 + 25,450-13,LARGOTEXTO}; // ancho modificado
    SDL_Rect textoMenu={(WINDOW_WIDTH - ANCHOCUADRADO)/2 + 37,(WINDOW_HEIGHT - LARGOCUADRADO)/2 + 190,ANCHOCUADRADO,LARGOTEXTO};
    SDL_Rect textoReset={(WINDOW_WIDTH - ANCHOCUADRADO)/2 + 37,(WINDOW_HEIGHT - LARGOCUADRADO)/2 + 150,ANCHOCUADRADO,LARGOTEXTO};
    SDL_Rect textoPuntaje={(WINDOW_WIDTH - ANCHOCUADRADO)/2 + 60,(WINDOW_HEIGHT - LARGOCUADRADO)/2 + 100,ANCHOCUADRADO,LARGOTEXTO};
    SDL_Texture* textura;
    SDL_SetRenderDrawColor(g->renderer,169, 169, 169,255);
    SDL_RenderFillRect(g->renderer, &resultadoGame);
    if(g->m.exit == VERDADERO)
    {
        crearBoton(textoResult,g->text_f,textura,white," Ganaste",g->renderer,0);
    }
    else{
            crearBoton(textoResult,g->text_f,textura,white," Perdiste",g->renderer,0);
    }
    sprintf(puntaje,"Puntaje : [%d]",getPuntosJugador(&g->p));
    crearBoton(textoPuntaje,g->text_f,textura,white,puntaje,g->renderer,1);
    crearBoton(textoReset,g->text_f,textura,white,"Presiona R : Jugar",g->renderer,1);
    crearBoton(textoMenu,g->text_f,textura,white,"Presiona M : Menu",g->renderer,1);
    SDL_DestroyTexture(textura);
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
                if(g->eventos.key.keysym.scancode == SDL_SCANCODE_M)
                {
                    g->is_pausing=false;
                    g->inicio=true;
                    reiniciarJuego(g);
                }
                if(g->eventos.key.keysym.scancode == SDL_SCANCODE_R)
                {
                    g->is_pausing=false;
                    reiniciarJuego(g);

                }
        }
    }
}
}
int reiniciarJuego(tGame *g)
{
    char linea[TAM_BUFFER];
    char *valor;
    eliminarMatriz(g->m.mat, g->m.filMapa, g->m.colMapa);
    if(colaVacia(&(g->colaMov))!=COLA_VACIA)
        vaciarCola(&(g->colaMov));
    crearMapa(&g->m);
    FILE*config = fopen(ARCHIVO_CONFIG, "r");
    g->is_running=true;
    g->is_pausing=false;
    g->ranking=false;
    if (!config)
    {
        printf("No hay archivo de configuracion de mapa.\nSe genera el juego en predeterminado.\n");
        g->cantFant = 3;
        g->cantVidas = 3;
        g->cantPremios = 2;
        g->m.filMapa = 21;
        g->m.colMapa = 21;
        g->cantVidasExt = 1;
    }
    else
    {
        fgets(linea, TAM_BUFFER, config);
        asignarConfig(linea, &(g->m.filMapa));
        fgets(linea, TAM_BUFFER, config);
        asignarConfig(linea, &g->m.colMapa);
        fgets(linea, TAM_BUFFER, config);
        asignarConfig(linea, &g->cantVidas);
        fgets(linea, TAM_BUFFER, config);
        asignarConfig(linea, &g->cantFant);
        fgets(linea, TAM_BUFFER, config);
        asignarConfig(linea, &g->cantPremios);
        fgets(linea, TAM_BUFFER, config);
        asignarConfig(linea, &g->cantVidasExt);
        fclose(config);
    }
    crearJugador(&g->p, g->cantVidas);
    if (llenarMapa(&g->p, &g->m, g->cantFant, g->cantPremios, g->cantVidasExt) == FALSE)
    {
        return FALSE ;
    }
}
void cargarTexturasMenu(tGame *g,SDL_Rect botonP,SDL_Rect botonR,SDL_Rect botonS)
{
    SDL_Color black = {255,255,255,255};
    SDL_Rect textP = {75,250};
    SDL_Rect textR = {50,330};
    SDL_Rect textS = {30,410};

    SDL_Texture* textura;
    SDL_Surface * superficie;
    SDL_RenderClear(g->renderer);
    SDL_RenderCopy(g->renderer,g->fondo,NULL,NULL);

    SDL_SetRenderDrawColor(g->renderer,169, 169, 169,255);

    SDL_RenderFillRect(g->renderer,&botonP);
    SDL_RenderFillRect(g->renderer,&botonR);
    SDL_RenderFillRect(g->renderer,&botonS);


    // boton play
    superficie= TTF_RenderText_Blended(g->titulo_f,"Jugar",black);
    textura= SDL_CreateTextureFromSurface(g->renderer,superficie);
    SDL_QueryTexture(textura, NULL, NULL, &textP.w, &textP.h);
    SDL_FreeSurface(superficie);
    textP.x = botonP.x + (botonP.w / 2) - (textP.w / 2);
    textP.y = botonP.y + (botonP.h / 2) - (textP.h / 2);
    SDL_RenderCopy(g->renderer,textura,NULL,&textP);
    SDL_DestroyTexture(textura);

    //boton ranking
    superficie = TTF_RenderText_Blended(g->titulo_f,"Ranking",black);
    textura= SDL_CreateTextureFromSurface(g->renderer,superficie);
    SDL_QueryTexture(textura, NULL, NULL, &textR.w, &textR.h);
    SDL_FreeSurface(superficie);
    textR.x = botonR.x + (botonR.w / 2) - (textR.w / 2);
    textR.y = botonR.y + (botonR.h / 2) - (textR.h / 2);
    SDL_RenderCopy(g->renderer,textura,NULL,&textR);
    SDL_DestroyTexture(textura);
    //boton salir
    superficie = TTF_RenderText_Blended(g->titulo_f,"Salir",black);
    textura= SDL_CreateTextureFromSurface(g->renderer,superficie);
    SDL_QueryTexture(textura, NULL, NULL, &textS.w, &textS.h);
    SDL_FreeSurface(superficie);
    textS.x = botonS.x + (botonS.w / 2) - (textS.w / 2);
    textS.y = botonS.y + (botonS.h / 2) - (textS.h / 2);
    SDL_RenderCopy(g->renderer,textura,NULL,&textS);
    SDL_DestroyTexture(textura);

    //musica menu
    Mix_HaltMusic();
    Mix_PlayMusic(g->musica,-1);
    SDL_RenderPresent(g->renderer);
}
void menu_inicio(tGame *g)
{
    SDL_Rect botonP = {20,250,240,60};
    SDL_Rect botonR = {20,330,240,60};
    SDL_Rect botonS  = {20,410,240,60};
    SDL_Point click;
    cargarTexturasMenu(g,botonP,botonR,botonS);
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
                if(SDL_PointInRect(&click,&botonR))
                {
                    g->ranking=true;
                    submenuranking(g);
                    cargarTexturasMenu(g,botonP,botonR,botonS);
                }
            }
        }
    }
    Mix_HaltMusic();
}
void submenuranking(tGame *g)
{
    int i;
    SDL_Texture* textura;
    SDL_Surface * superficie;
    SDL_Point click;
    SDL_Rect cuadradoCerrar ={WINDOW_WIDTH*0.455,WINDOW_HEIGHT-124,130,45}; //modificar por constantes
    SDL_RenderCopy(g->renderer,g->rank,NULL,NULL);// se carga la imagen

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
        // modificar cuando esten los rankings
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
        }
        SDL_DestroyTexture(rank_tex);
        SDL_DestroyTexture(name_tex);
        SDL_DestroyTexture(score_tex);


    SDL_RenderPresent(g->renderer);
    while(g->ranking)
    {
        while(SDL_PollEvent(&g->eventos))
        {
            switch(g->eventos.type)
            {
            case SDL_QUIT:
                g->is_running=false;
                g->inicio=false;
                g->ranking=false;
                break;
            case SDL_KEYDOWN:
                switch(g->eventos.key.keysym.scancode)
                {
                case SDL_SCANCODE_ESCAPE:
                    g->ranking=false;
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
                        g->ranking=false;
                    }
                }
                break;

            default:
                break;
            }
        }
    }

}
