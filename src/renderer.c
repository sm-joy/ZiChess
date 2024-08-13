#include "../include/Renderer.h"

Renderer* createRenderer(const char* pWinTitle, int pWinWidth, int pWinHeight, const char* pWinIconPath) {
    Renderer* renderer = (Renderer*)malloc(sizeof(Renderer));
    if (!renderer) {
        fprintf(stderr, "Error allocating memory for Renderer\n");
        return NULL;
    }

    renderer->winIcon = NULL;
    renderer->loadedTextures = NULL;
    renderer->createdTexts = NULL;
    renderer->numTexts = 0;
    renderer->numTextures = 0;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        free(renderer);
        renderer = NULL;
        return NULL;
    }
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    int imageFormats = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP;
    if (!(IMG_Init(imageFormats) & imageFormats)) {
        fprintf(stderr, "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        SDL_Quit();
        free(renderer);
        return NULL;
    }

    if (TTF_Init() == -1) {
        fprintf(stderr, "SDL_ttf initialization failed: %s\n", TTF_GetError());
        IMG_Quit();
        SDL_Quit();
        free(renderer);
        renderer = NULL;
        return NULL;
    }

    renderer->window = NULL;
    renderer->window = SDL_CreateWindow(pWinTitle, 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        pWinWidth, 
        pWinHeight, 
        SDL_WINDOW_SHOWN);
    if (renderer->window == NULL) {
        fprintf(stderr, "Error initializing SDL_Window: %s\n", SDL_GetError());
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        free(renderer);
        renderer = NULL;
        return NULL;
    }

    renderer->renderer = NULL;
    renderer->renderer = SDL_CreateRenderer(renderer->window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer->renderer == NULL) {
        fprintf(stderr, "Error initializing SDL_Renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(renderer->window);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        free(renderer);
        renderer = NULL;
        return NULL;
    }
    if (SDL_SetRenderDrawBlendMode(renderer->renderer, SDL_BLENDMODE_BLEND)) {
        fprintf(stderr, "Error enabling alpha belnding: %s\n", SDL_GetError());
    }


    if (pWinIconPath != NULL) setWindowIcon(renderer, pWinIconPath);

    return renderer;
}

void destroyRenderer(Renderer* renderer) {
    if (renderer) {
        destroyTextures(renderer);
        destroyTexts(renderer);
        if (renderer->winIcon != NULL) SDL_FreeSurface(renderer->winIcon);
        if (renderer->window != NULL) SDL_DestroyWindow(renderer->window);
        if (renderer->renderer != NULL) SDL_DestroyRenderer(renderer->renderer);

        destroyTextures(renderer);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        free(renderer);
        renderer = NULL;
    }
}

void destroyTextures(Renderer* renderer) {
    if (renderer->loadedTextures) {
        for (int i = 0; i < renderer->numTextures; ++i) {
            SDL_DestroyTexture(renderer->loadedTextures[i]);
        }
        free(renderer->loadedTextures);
        renderer->loadedTextures = NULL;
        renderer->numTextures = 0;
    }
}

void destroyTexts(Renderer* renderer) {
    if (renderer->createdTexts) {
        for (int i = 0; i < renderer->numTexts; ++i) {
            destroyText(renderer->createdTexts[i]);
        }
        free(renderer->createdTexts);
        renderer->createdTexts = NULL;
        renderer->numTexts = 0;
    }
}

void destroyText(Text* textobj) {
    SDL_DestroyTexture(textobj->texture);
    free(textobj->text);
    textobj->text = NULL;
}


void clearWindow(Renderer* renderer) {
    SDL_RenderClear(renderer->renderer);
}

void setRenderDrawColor(Renderer* renderer, SDL_Color* pColor) {
    SDL_SetRenderDrawColor(renderer->renderer, pColor->r, pColor->g, pColor->b, pColor->a);
}

void setBackgroundColor(Renderer* renderer, SDL_Color* pColor) {
    SDL_SetRenderDrawColor(renderer->renderer, pColor->r, pColor->g, pColor->b, pColor->a);
    SDL_RenderClear(renderer->renderer);
}

void displayWindow(Renderer* renderer) {
    SDL_RenderPresent(renderer->renderer);
}

void setWindowIcon(Renderer* renderer, const char* pWinIconPath) {
    renderer->winIcon = IMG_Load(pWinIconPath);
    if (renderer->winIcon != NULL) {
        SDL_SetWindowIcon(renderer->window, renderer->winIcon);
    }
    else {
        fprintf(stderr, "Error Loading '%s' Icon! Error: %s\n", pWinIconPath, SDL_GetError());
    }
}

void renderRect(Renderer* renderer, SDL_Rect* pRect, SDL_Color* pColor, int pFilled) {
    setRenderDrawColor(renderer, pColor);
    if (pFilled) {
        SDL_RenderFillRect(renderer->renderer, pRect);
    }
    else {
        SDL_RenderDrawRect(renderer->renderer, pRect);
    }
}

void renderTextureEx(Renderer* renderer, SDL_Texture* pTexture, int pPosX, int pPosY, int pTextureWidth, int pTextureHeight) {
    SDL_Rect destinationRect = { pPosX, pPosY, pTextureWidth, pTextureHeight };
    SDL_RenderCopy(renderer->renderer, pTexture, NULL, &destinationRect);
}

void renderTexture(Renderer* renderer, SDL_Texture* texture, int posX, int posY) {
    int height = 0, width = 0;
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);
    renderTextureEx(renderer, texture, posX, posY, width, height);
}

void renderText(Renderer* renderer, Text* text, int posX, int posY) {
    if (text == NULL) {
        return;
    }
    renderTextureEx(renderer, text->texture, posX, posY, text->width, text->height);
}

void renderTextCenterRect(Renderer* renderer, const char* pText, SDL_Color* pColor, SDL_Rect* pRect, TTF_Font* font) {
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, pText, *pColor);
    if (textSurface == NULL) {
        fprintf(stderr, "Cannot Render ttf_rendertext_blended! Error: %s\n", TTF_GetError());
        return;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer->renderer, textSurface);
    if (textTexture == NULL) {
        fprintf(stderr, "Cannot convert surface to texture! Error: %s\n", SDL_GetError());
        SDL_FreeSurface(textSurface);
        return;
    }

    int x = pRect->x + (pRect->w - textSurface->w) / 2;
    int y = pRect->y + (pRect->h - textSurface->h) / 2;

    renderTextureEx(renderer, textTexture, x, y, textSurface->w, textSurface->h);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}


TTF_Font* loadFont(Renderer* renderer, const char* pFontPath, int pFontSize) {
     TTF_Font* font = TTF_OpenFont(pFontPath, pFontSize);
    if (font == NULL) {
        fprintf(stderr, "Error Loading '%s' Font! Error: %s\n", pFontPath, TTF_GetError());
        return NULL;
    }
    return font;
} 


void renderTextureEa(Renderer* renderer, SDL_Texture* pTexture, SDL_Rect* srcRect, SDL_Rect* dstRect)
{
    SDL_RenderCopy(renderer->renderer, pTexture, srcRect, dstRect);
}


SDL_Texture* createTextTxture(Renderer* renderer, const char* text, SDL_Color* color, TTF_Font* font) {
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, text, *color);
    if (textSurface == NULL) {
        fprintf(stderr, "Cannot Render ttf_rendertext_blended! Error: %s\n", TTF_GetError());
        return NULL;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer->renderer, textSurface);
    if (textTexture == NULL) {
        fprintf(stderr, "Cannot convert surface to texture! Error: %s\n", SDL_GetError());
        SDL_FreeSurface(textSurface);
        return NULL;
    }

    SDL_FreeSurface(textSurface);
    return textTexture;
}

bool addText(Renderer* renderer, Text* textobj) {
    if (textobj == NULL) {
        return false;
    }

    Text** newptr = (Text**)realloc(renderer->createdTexts, (renderer->numTexts + 1) * sizeof(Text*));
    if (newptr == NULL) {
        fprintf(stderr, "Error Creating Text! Realloc Failed: %s\n", SDL_GetError());
        return false;
    }

    renderer->createdTexts = newptr;
    renderer->createdTexts[renderer->numTexts++] = textobj;
    return true;
}

Text* createText(Renderer* renderer, const char* text, SDL_Color* color, TTF_Font* font)
{
    Text* textobj = NULL;
    textobj = (Text*)malloc(sizeof(Text));

    if (textobj == NULL) {
        fprintf(stderr, "Malloc Failed while allocating Text Object!\n");
        return NULL;
    }

    textobj->text = SDL_strdup(text);
    textobj->color = color;
    textobj->font = font;
    textobj->texture = createTextTxture(renderer, text, color, textobj->font);

    if (textobj->texture) {
        SDL_QueryTexture(textobj->texture, NULL, NULL, &(textobj->width), &(textobj->height));
    }
    else {
        textobj->width = 0;
        textobj->height = 0;
    }

    if (!addText) {
        destroyText(textobj);
        return NULL;
    }

    return textobj;
}


Button* createButton(Renderer* renderer, const char* text, SDL_Color* buttonColor, SDL_Color* textColor, TTF_Font* font, int width, int height, int x, int y) {
    Button* button = (Button*)malloc(sizeof(Button));
    if (button == NULL) {
        fprintf(stderr, "Failed to allocate memory for button\n");
        return NULL;
    }

    Text* textobj = createText(renderer, text, textColor, font);
    if (textobj == NULL) {
        fprintf(stderr, "Failed to create button text\n");
        return NULL;
    }

    button->text = textobj;
    button->rect.x = x;
    button->rect.y = y;
    if (width == 0 && height == 0) {
        button->rect.w = textobj->width;
        button->rect.h = textobj->height;
    }
    button->color = *buttonColor;
    button->isHovered = false;
    button->isPressed = false;

    button->hoverText = NULL;
    button->pressText = NULL;

    return button;
}

Button* createButtonEx(Renderer* renderer, int x, int y, int width, int height, Text* normalText, Text* hoverText, Text* pressText, SDL_Color* buttonColor) {
    Button* button = (Button*)malloc(sizeof(Button));
    if (button == NULL) {
        fprintf(stderr, "Failed to allocate memory for button\n");
        return NULL;
    }
    button->text = normalText;
    button->rect.x = x;
    button->rect.y = y;
    if (height == 0 && width == 0) {
        button->rect.w = normalText->width;
        button->rect.h = normalText->height;
    }
    button->color = *buttonColor;
    button->hoverText = hoverText;
    button->pressText = pressText;
    button->isHovered = false;
    button->isPressed = false;
    return button;
}



