#include "header/SDL_INIT.h"
#include "header/menus.h"
void crearBoton(SDL_Rect cuadrado, TTF_Font* fuente, SDL_Color color, char *texto, SDL_Renderer* render, int ajustar)
{
    SDL_Texture *textura = NULL;
    SDL_Surface * superficie = TTF_RenderText_Blended(fuente, texto, color);
    SDL_RenderCopy(render, textura, NULL, &cuadrado);
    textura = SDL_CreateTextureFromSurface(render, superficie);
    if (ajustar == VERDADERO)
        SDL_QueryTexture(textura, NULL, NULL, &cuadrado.w, &cuadrado.h);
    SDL_RenderCopy(render, textura, NULL, &cuadrado);
    SDL_FreeSurface(superficie);
    SDL_DestroyTexture(textura);
}
void menu_pausa(tGame *g)
{
    char puntaje[TAM_BUFFER];
    SDL_Color white = {255, 255, 255, 255}; // color RGB
    /// Lo sumado a las coordenadas de rectangulo es el desplazamiento para centrarlos
    SDL_Rect resultadoGame = {(WINDOW_WIDTH - ANCHOCUADRADO) / 2, (WINDOW_HEIGHT - LARGOCUADRADO) / 2, ANCHOCUADRADO, LARGOCUADRADO};
    SDL_Rect textoResult = {(WINDOW_WIDTH - ANCHOCUADRADO) / 2 + 10, (WINDOW_HEIGHT - LARGOCUADRADO) / 2 + 25, 450 - 13, LARGOTEXTO}; // ancho modificado
    SDL_Rect textoMenu= {(WINDOW_WIDTH - ANCHOCUADRADO)/2 + 37,(WINDOW_HEIGHT - LARGOCUADRADO)/2 + 200,ANCHOCUADRADO,LARGOTEXTO};
    SDL_Rect textoReset= {(WINDOW_WIDTH - ANCHOCUADRADO)/2 + 37,(WINDOW_HEIGHT - LARGOCUADRADO)/2 + 170,ANCHOCUADRADO,LARGOTEXTO};
    SDL_Rect textoPuntaje= {(WINDOW_WIDTH - ANCHOCUADRADO)/2 + 60,(WINDOW_HEIGHT - LARGOCUADRADO)/2 + 90,ANCHOCUADRADO,LARGOTEXTO};
    SDL_Rect textoCantMov= {(WINDOW_WIDTH - ANCHOCUADRADO)/2 + 60,(WINDOW_HEIGHT - LARGOCUADRADO)/2 + 120,ANCHOCUADRADO,LARGOTEXTO};
    SDL_SetRenderDrawColor(g->renderer, 169, 169, 169, 255);
    SDL_RenderFillRect(g->renderer, &resultadoGame);
    if (g->m.exit == VERDADERO)
        crearBoton(textoResult, g->text_f, white, " Ganaste", g->renderer, 0);
    else
        crearBoton(textoResult, g->text_f, white, " Perdiste", g->renderer, 0);
    sprintf(puntaje, "Puntaje :   [%d]", getPuntosJugador(&g->p));
    crearBoton(textoPuntaje, g->text_f, white, puntaje, g->renderer, 1);
    sprintf(puntaje,"Movimientos: [%d]",contarMovs(&g->colaMovsJugador));
    crearBoton(textoCantMov,g->text_f,white,puntaje,g->renderer,1);
    crearBoton(textoReset, g->text_f, white, "Presiona R : Jugar", g->renderer, 1);
    crearBoton(textoMenu, g->text_f, white, "Presiona M : Menu", g->renderer, 1);
    SDL_RenderPresent(g->renderer);
    ///mostrar g->colaMovsJugador, funcion contarMovs;
    while (g->is_pausing)
    {
        // se verifican los eventos
        while (SDL_PollEvent(&g->eventos))
        {
            if (g->eventos.type == SDL_QUIT)
            {
                g->inicio = false;
                g->is_running = false;
                g->is_pausing = false;
            }
            if (g->eventos.type == SDL_KEYDOWN)
            {
                if (g->eventos.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                    g->is_pausing = false;
                if (g->eventos.key.keysym.scancode == SDL_SCANCODE_M)
                {
                    g->is_pausing = false;
                    g->inicio = true;
                    reiniciarJuego(g);
                }
                if (g->eventos.key.keysym.scancode == SDL_SCANCODE_R)
                {
                    g->is_pausing = false;
                    reiniciarJuego(g);
                }
            }
        }
    }
}
int reiniciarJuego(tGame *g)
{
    eliminarMatriz(g->m.mat, g->m.filMapa, g->m.colMapa);
    if (colaVacia(&(g->colaMov)) != COLA_VACIA)
        vaciarCola(&(g->colaMov));
    g->is_running = true;
    g->is_pausing = false;
    g->ranking = false;
    crearJugador(&g->p, g->cantVidas);
    if (llenarMapa(&g->p, &g->m, g->cantFant, g->cantPremios, g->cantVidasExt) == FALSE)
        return FALSE;
    return VERDADERO;
}
void crearBotonTextCentrado(SDL_Rect cuadrado, TTF_Font* fuente, SDL_Texture* textura, SDL_Color colorLetra, char *nombre, SDL_Renderer* render)
{
    SDL_Rect texto;
    SDL_Surface *superficie = TTF_RenderText_Blended(fuente, nombre, colorLetra);
    textura = SDL_CreateTextureFromSurface(render, superficie);
    SDL_QueryTexture(textura, NULL, NULL, &texto.w, &texto.h);
    SDL_FreeSurface(superficie);
    texto.x = cuadrado.x + (cuadrado.w / 2) - (texto.w / 2);
    texto.y = cuadrado.y + (cuadrado.h / 2) - (texto.h / 2);
    SDL_RenderCopy(render, textura, NULL, &texto);
    SDL_DestroyTexture(textura);
}
void cargarTexturasMenu(tGame *g, SDL_Rect botonP, SDL_Rect botonR, SDL_Rect botonS)
{
    SDL_Texture* textura;
    SDL_Surface * superficie;
    SDL_RenderClear(g->renderer);
    SDL_RenderCopy(g->renderer, g->fondo, NULL, NULL);
    SDL_SetRenderDrawColor(g->renderer, 169, 169, 169, 255);
    SDL_RenderFillRect(g->renderer, &botonP);
    SDL_RenderFillRect(g->renderer, &botonR);
    SDL_RenderFillRect(g->renderer, &botonS);
    crearBotonTextCentrado(botonP, g->titulo_f, textura, NEGRO, "Jugar", g->renderer);
    crearBotonTextCentrado(botonR, g->titulo_f, textura, NEGRO, "Ranking", g->renderer);
    crearBotonTextCentrado(botonS, g->titulo_f, textura, NEGRO, "Salir", g->renderer);
    //musica menu
    SDL_RenderPresent(g->renderer);
}
void menu_inicio(tGame *g)
{
    SDL_Rect botonP = {(WINDOW_WIDTH - 240) / 2, 250, 240, 60};
    SDL_Rect botonR = {(WINDOW_WIDTH - 240) / 2, 330, 240, 60};
    SDL_Rect botonS  = {(WINDOW_WIDTH - 240) / 2, 410, 240, 60};
    SDL_Point click;
    char* rank;
    Mix_HaltMusic();
    Mix_PlayMusic(g->musica, -1);
    while (g->inicio)
    {
        cargarTexturasMenu(g, botonP, botonR, botonS);
        // se verifican los eventos
        while (SDL_PollEvent(&g->eventos))
        {
            if (g->eventos.type == SDL_QUIT)
            {
                g->inicio = false;
                g->is_running = false;
            }
            if (g->eventos.type == SDL_KEYDOWN)
            {
                if (g->eventos.key.keysym.scancode == SDL_SCANCODE_M)
                    Mix_HaltMusic();
            }
            if (g->eventos.type == SDL_MOUSEBUTTONDOWN && g->eventos.button.button == SDL_BUTTON_LEFT)
            {
                click.x = g->eventos.button.x;
                click.y = g->eventos.button.y;
                if (SDL_PointInRect(&click, &botonS))
                {
                    g->inicio = false;
                    g->is_running = false;
                }
                if (SDL_PointInRect(&click, &botonP))
                {
                    g->inicio = false;
                    g->is_running = true;
                    Mix_PlayChannel(-1, g->sonidomenu, 0);
                }
                if (SDL_PointInRect(&click, &botonR))
                {
                    g->ranking = true;
                    submenuranking(g);
                }
            }
        }
    }
    Mix_HaltMusic();
}
void menuIngresarNombre(tGame *g)
{
    int anchoTexto, text_w, text_h, dest_y;
    SDL_Color white = {255, 255, 255, 255};
    SDL_Texture* textura;
    SDL_Surface * superficie;
    SDL_RenderClear(g->renderer);
    SDL_RenderCopy(g->renderer, g->rank, NULL, NULL);
    SDL_Rect textointerfaz = {(WINDOW_WIDTH - 700) / 2, (WINDOW_HEIGHT - 50) / 3 - 30, 700, 60};
    SDL_Rect cuadradoText = {(WINDOW_WIDTH - 700) / 2, (WINDOW_HEIGHT - 50) / 3, 700, 60};
    strncpy(g->p.nombre, "", bufferNombreSize);
    SDL_StartTextInput();
    g->is_writing = true;
    while (g->is_writing)
    {
        while (SDL_PollEvent(&g->eventos))
        {
            if (g->eventos.type == SDL_QUIT)
            {
                g->is_writing = false;
                g->is_running = false;
                g->inicio = false;
            }
            if (g->eventos.type == SDL_KEYDOWN)
            {
                if (g->eventos.key.keysym.sym == SDLK_RETURN && strlen(g->p.nombre) >= 1)
                    g->is_writing = false;
                if (g->eventos.key.keysym.sym == SDLK_BACKSPACE && strlen(g->p.nombre) > 0)

                    *(g->p.nombre + strlen(g->p.nombre) - 1) = '\0';
            }
            if (g->eventos.type == SDL_TEXTINPUT)
            {
                if (strlen(g->p.nombre) + strlen(g->eventos.text.text) < bufferNombreSize)
                    strcat(g->p.nombre, g->eventos.text.text);
            }
        }
        SDL_RenderClear(g->renderer);
        SDL_RenderCopy(g->renderer, g->fondonombre, NULL, NULL);
        crearBoton(textointerfaz, g->text_f, white, "Ingrese Su Nombre (MAX 6)", g->renderer, 1);
        SDL_SetRenderDrawColor(g->renderer, 169, 169, 169, 255);
        SDL_RenderFillRect(g->renderer, &cuadradoText);
        TTF_SizeText(g->text_f, g->p.nombre, &text_w, &text_h);
        dest_y = cuadradoText.y + (cuadradoText.h - text_h) / 2;
        SDL_Rect textDestRect;
        textDestRect.x = textointerfaz.x + 5;
        textDestRect.y = dest_y;
        textDestRect.w = text_w;
        textDestRect.h = text_h;
        crearBoton(textDestRect, g->titulo_f, white, g->p.nombre, g->renderer, 1);
        TTF_SizeText(g->text_f, g->p.nombre, &anchoTexto, NULL);
        if (SDL_GetTicks() % 1000 < 500)
        {
            SDL_Rect cursorRect = {cuadradoText.x + anchoTexto + 2, cuadradoText.y + 5, 3, cuadradoText.h - 10};
            SDL_SetRenderDrawColor(g->renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(g->renderer, &cursorRect);
        }
        SDL_RenderPresent(g->renderer);
        SDL_Delay(16);
    }
    SDL_StopTextInput();
}
void submenuranking(tGame *g)
{
    SDL_Color dorado ={ 220, 200, 150, 255 };
    SDL_Texture* textura;
    SDL_Surface * superficie;
    SDL_Point click;
    SDL_RenderCopy(g->renderer, g->rank, NULL, NULL); // se carga la imagen
    SDL_RenderPresent(g->renderer);
    SDL_Rect TOP4name_rect = {WINDOW_WIDTH-1160,WINDOW_HEIGHT-280,130,30};
    SDL_Rect TOP4point_rect = {WINDOW_WIDTH-1160,WINDOW_HEIGHT-250,130,20};
    SDL_Rect TOP2name_rect = {WINDOW_WIDTH-930,WINDOW_HEIGHT-280,130,30};
    SDL_Rect TOP2point_rect = {WINDOW_WIDTH-930,WINDOW_HEIGHT-250,130,20};
    SDL_Rect TOP1name_rect = {WINDOW_WIDTH-695,WINDOW_HEIGHT-280,130,30};
    SDL_Rect TOP1point_rect = {WINDOW_WIDTH-695,WINDOW_HEIGHT-250,130,20};
    SDL_Rect TOP3name_rect = {WINDOW_WIDTH-455,WINDOW_HEIGHT-280,130,30};
    SDL_Rect TOP3point_rect = {WINDOW_WIDTH-455,WINDOW_HEIGHT-250,130,20};
    SDL_Rect TOP5name_rect = {WINDOW_WIDTH-230,WINDOW_HEIGHT-280,130,30};
    SDL_Rect TOP5point_rect = {WINDOW_WIDTH-230,WINDOW_HEIGHT-250,130,20};
    crearBoton(TOP5name_rect, g->titulo_f, dorado,"5.JDPHA", g->renderer, 0);
    crearBoton(TOP5point_rect, g->titulo_f, dorado, "100", g->renderer, 1);
    crearBoton(TOP4name_rect, g->titulo_f, dorado,"4.JDPHA", g->renderer, 0);
    crearBoton(TOP4point_rect, g->titulo_f, dorado, "100", g->renderer, 1);
    crearBoton(TOP3name_rect, g->titulo_f,dorado,"3.JDPHA", g->renderer, 0);
    crearBoton(TOP3point_rect, g->titulo_f,dorado, "100", g->renderer, 1);
    crearBoton(TOP2name_rect, g->titulo_f, dorado,"2.JDPHA", g->renderer, 0);
    crearBoton(TOP2point_rect, g->titulo_f,dorado, "100", g->renderer, 1);
    crearBoton(TOP1name_rect, g->titulo_f,dorado,"1.JDPHA", g->renderer, 0);
    crearBoton(TOP1point_rect, g->titulo_f,dorado, "100", g->renderer, 1);
    SDL_RenderPresent(g->renderer);
    while (g->ranking)
    {
        while (SDL_PollEvent(&g->eventos))
        {
            switch (g->eventos.type)
            {
            case SDL_QUIT:
                g->is_running = false;
                g->inicio = false;
                g->ranking = false;
                break;
            case SDL_KEYDOWN:
                switch (g->eventos.key.keysym.scancode)
                {
                case SDL_SCANCODE_ESCAPE:
                    g->ranking = false;
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
        }
    }
}

