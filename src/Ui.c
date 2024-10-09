#include "../include/Ui.h"



WidgetManager* UI_CreateWidgetManager() {
    WidgetManager* wm = (WidgetManager*)malloc(sizeof(WidgetManager));
    if (!wm) {
        fprintf(stderr, "Failed to allocate memory for Widgetmanager! Malloc failed!\n");
        return NULL;
    }

    wm->labels = NULL;
    wm->buttons = NULL;
    wm->fonts = NULL;
    wm->progressBars = NULL;
    wm->numButtons = 0;
    wm->numLabels = 0;
    wm->numFonts = 0;
    wm->numProgressBar = 0;

    return wm;
}

void UI_Clean(WidgetManager* wm) {
    if (wm) {
        UI_DestroyButtons(wm);
        UI_DestroyLabels(wm);
        UI_DestroyFonts(wm);
        UI_DestroyProgressBars(wm);
        free(wm);
        wm = NULL;
    }
}

// FONT START

static bool UI_AddFontToWM(WidgetManager* wm, TTF_Font* font) {
    if (!wm || !font) return false;

    TTF_Font** newptr = (TTF_Font**)realloc(wm->fonts, (wm->numFonts + 1) * sizeof(TTF_Font*));
    if (!newptr) {
        fprintf(stderr, "Error Creating Fonts Array! Realloc Failed: %s\n", SDL_GetError());
        return false;
    }

    wm->fonts = newptr;
    wm->fonts[wm->numFonts++] = font;
    return true;

}

TTF_Font* UI_LoadFont(WidgetManager* wm, const char* fontPath, int fontSize) {
    TTF_Font* font = TTF_OpenFont(fontPath, fontSize);
    if (!font) {
        fprintf(stderr, "Error Loading '%s' Font! Error: %s\n", fontPath, TTF_GetError());
        return NULL;
    }
    return font;
}

void UI_DestroyFonts(WidgetManager* wm) {
    if (wm->fonts) {
        for (int i = 0; i < wm->numFonts; ++i) {
            TTF_CloseFont(wm->fonts[i]); 
            wm->fonts[i] = NULL;
        }
        free(wm->fonts);
        wm->fonts = NULL;
        wm->numFonts = 0;
    }
}

// FONT END

// LABEL START

void UI_DestroyLabel(Label* label) {
    if (!label) return;
    if (!label->texture) {
        SDL_DestroyTexture(label->texture);
        label->texture = NULL;
    }
    free(label);
    label = NULL;
}

void UI_DestroyLabels(WidgetManager* wm) {
    if (wm->labels) {
        for (int i = 0; i < wm->numLabels; ++i) {
            UI_DestroyLabel(wm->labels[i]);
        }
        free(wm->labels);
        wm->labels = NULL;
        wm->numLabels = 0;
    }
}

static bool UI_AddLabelToWM(WidgetManager* wm, Label* label) {
    if (!label || !wm) return false;

    Label** newptr = (Label**)realloc(wm->labels, (wm->numLabels + 1) * sizeof(Label*));
    if (!newptr) {
        fprintf(stderr, "Error Creating Label Array! Realloc Failed: %s\n", SDL_GetError());
        return false;
    }

    wm->labels = newptr;
    wm->labels[wm->numLabels++] = label;
    return true;
}

static SDL_Texture* UI_CreateLabelTxture(RenderContext* rc, const char* text, SDL_Color* color, TTF_Font* font) {
    SDL_Surface* labelSurface = TTF_RenderText_Blended(font, text, *color);
    if (!labelSurface) {
        fprintf(stderr, "Cannot Render ttf_rendertext_blended! Error: %s\n", TTF_GetError());
        return NULL;
    }

    SDL_Texture* labelTexture = SDL_CreateTextureFromSurface(rc->renderer, labelSurface);
    SDL_FreeSurface(labelSurface);

    if (!labelTexture) {
        fprintf(stderr, "Cannot convert surface to texture! Error: %s\n", SDL_GetError());
        return NULL;
    }

    return labelTexture;
}

Label* UI_CreateLabel(RenderContext* rc, WidgetManager* wm, const char* text, int posX, int posY, SDL_Color* color, TTF_Font* font) {

    Label* label = (Label*)malloc(sizeof(Label));;
    if (!label) {
        fprintf(stderr, "Malloc Failed while allocating Label Object!\n");
        return NULL;
    }

    label->color = *color;
    label->font = font;
    label->rect.x = posX;
    label->rect.y = posY;
    label->texture = UI_CreateLabelTxture(rc, text, color, label->font);
    if (label->texture) {
        SDL_QueryTexture(label->texture, NULL, NULL, &(label->rect.w), &(label->rect.h));
    }
    else {
        UI_DestroyLabel(label);
        return NULL;
    }


    if (wm) {
        if (!UI_AddLabelToWM(wm, label)) {
            UI_DestroyLabel(label);
            return NULL;
        }
    }

    return label;
}

void UI_UpdateLabel(RenderContext* rc, Label* label, const char* text, int posX, int posY) {
    if (!label) return;

    if (text && rc) {
        SDL_Texture* newTexture = UI_CreateLabelTxture(rc, text, &label->color, label->font);
        if (!newTexture) return;


        if (label->texture) {
            SDL_DestroyTexture(label->texture);
            label->texture = NULL;
        }

        label->texture = newTexture;
        SDL_QueryTexture(label->texture, NULL, NULL, &(label->rect.w), &(label->rect.h));
    }

    if (posX != NONE && posY != NONE) {
        label->rect.x = posX;
        label->rect.y = posY;
    }
}

void UI_RenderLabel(RenderContext* rc, Label* label) {
    if (label) renderTextureEa(rc, label->texture, NULL, &label->rect);
}

// LABEL END


// BUTTON START 


static bool UI_AddButtonToWM(WidgetManager* wm, Button* button) {
    if (!wm || !button) {
        fprintf(stderr, "WidgetManager or button is null!\n");
        return false;
    }

    Button** newptr = (Button**)realloc(wm->buttons, (wm->numButtons + 1) * sizeof(Button*));
    if (newptr == NULL) {
        fprintf(stderr, "Error Creating Button Array! Realloc Failed: %s\n", SDL_GetError());
        return false;
    }

    wm->buttons = newptr;
    wm->buttons[wm->numButtons++] = button;
    return true;
}

Button* UI_CreateButtonEx(
    RenderContext* rc,
    WidgetManager* wm,
    char id,
    const char* text,
    int posX,
    int posY,
    int width,
    int height,
    SDL_Color* normalLabelColor,
    SDL_Color* hoverLabelColor,
    SDL_Color* pressLabelColor,
    SDL_Color* buttonColor,
    TTF_Font* font) {

    Button* button = (Button*)malloc(sizeof(Button));
    if (!button) {
        fprintf(stderr, "Failed to allocate memory for button\n");
        return NULL;
    }

    button->id = id;
    button->state = BUTTON_NORMAL;

    button->normalLabel = UI_CreateLabel(rc, wm, text, posX, posY, normalLabelColor, font);
    button->currentLabel = button->normalLabel;
    button->hoverLabel = UI_CreateLabel(rc, wm, text, posX, posY, hoverLabelColor, font);
    button->pressLabel = UI_CreateLabel(rc, wm, text, posX, posY, pressLabelColor, font);

    button->rect.x = posX;
    button->rect.y = posY;
    if (height == NONE && width == NONE) {
        button->rect.w = button->normalLabel->rect.w;
        button->rect.h = button->normalLabel->rect.h;
    }

    if (buttonColor) button->color = *buttonColor;
    else button->color = (SDL_Color){ 0, 0, 0, 0 };

    
    if (!UI_AddButtonToWM(wm, button)) {
        free(button);
        button = NULL;
        return NULL;
    }

    return button;
}


void UI_DestroyButtons(WidgetManager* wm) {
    if (wm->buttons) {
        for (int i = 0; i < wm->numButtons; ++i) {
            free(wm->buttons[i]);
            wm->buttons[i] = NULL;
        }
        free(wm->buttons);
        wm->buttons = NULL;
        wm->numButtons = 0;
    }
}


void UI_RenderButton(RenderContext* rc, Button* button) {
    if (button->color.a == 0) renderTextureEa(rc, button->currentLabel->texture, NULL, &button->rect);
    else {
        renderRect(rc, &button->rect, &button->color, true);
        UI_RenderLabel(rc, button->currentLabel);
    }
}



// BUTTON END


//PROGRESSBAR START

static bool UI_AddProgressBarToWM(WidgetManager* wm, ProgressBar* progressBar) {
    if (!wm || !progressBar) {
        fprintf(stderr, "WidgetManager or ProgressBar is null!\n");
        return false;
    }

    ProgressBar** newptr = (ProgressBar**)realloc(wm->progressBars, (wm->numProgressBar + 1) * sizeof(ProgressBar*));
    if (newptr == NULL) {
        fprintf(stderr, "Error Creating ProgressBar Array! Realloc Failed: %s\n", SDL_GetError());
        return false;
    }

    wm->progressBars = newptr;
    wm->progressBars[wm->numProgressBar++] = progressBar;
    return true;
}

ProgressBar* UI_CreateProgressBar(RenderContext* rc, WidgetManager* wm, int maxProgress, int posX, int posY, int width, int height, SDL_Color* bgColor, SDL_Color* fgColor, SDL_Color* labelColor, TTF_Font* font) {
    if (!wm || !rc) {
        fprintf(stderr, "RenderContext or WidgetManager is NULL!\n");
        return NULL;
    }

    if (!bgColor || !fgColor) {
        fprintf(stderr, "Colors are NULL!\n");
        return NULL;
    }

    ProgressBar* pb = (ProgressBar*)malloc(sizeof(ProgressBar));

    if (!pb) {
        fprintf(stderr, "Faile to allocate memory for ProgressBar!\n");
        return NULL;
    }

    pb->maxProgress = maxProgress;
    pb->currentProgress = 0;
    pb->bgColor = *bgColor;
    pb->fgColor = *fgColor;
    pb->bgRect = (SDL_Rect){ posX, posY, width, height };
    pb->fgRect = (SDL_Rect){ posX, posY, 0, height };
    pb->progressLabel = UI_CreateLabel(rc, wm, "0%", 0, 0, labelColor, font);
    if (!pb->progressLabel) {
        free(pb);
        pb = NULL;
        return NULL;
    }
    UI_UpdateLabel(NULL, pb->progressLabel, NULL, ((width / 2) - (pb->progressLabel->rect.w / 2) + posX), ((height / 2) - (pb->progressLabel->rect.h / 2) + posY));


    if (!UI_AddProgressBarToWM) {
        free(pb);
        pb = NULL;
        return NULL;
    }

    return pb;
}

void UI_UpdateProgressBar(RenderContext* rc, ProgressBar* pb, int increment) {
    if (!pb) {
        fprintf(stderr, "ProgressBar is NULL!\n");
        return;
    }

    pb->currentProgress += increment;

    if (pb->currentProgress > pb->maxProgress) pb->currentProgress = pb->maxProgress;
    else if (pb->currentProgress < 0) pb->currentProgress = 0;

    float progressRatio = (float)pb->currentProgress / pb->maxProgress;
    pb->fgRect.w = (int)(pb->bgRect.w * progressRatio);

    char progressText[20];
    snprintf(progressText, sizeof(progressText), "%d%%", (int)(progressRatio * 100));

    UI_UpdateLabel(rc, pb->progressLabel, progressText, NONE, NONE);
}


void UI_DestroyProgressBars(WidgetManager* wm) {
    if (wm && wm->progressBars) {
        for (int i = 0; i < wm->numProgressBar; ++i) {
            free(wm->progressBars[i]);
            wm->progressBars[i] = NULL;
        }

        free(wm->progressBars);
        wm->progressBars = NULL;
        wm->numProgressBar = 0;
    }
}

void UI_RenderProgressBar(RenderContext* rc, ProgressBar* pb) {
    if (!rc || !pb) {
        fprintf(stderr, "RenderContext or ProgressBar is NULL!\n");
        return;
    }

    renderRect(rc, &pb->bgRect, &pb->bgColor, true);
    renderRect(rc, &pb->fgRect, &pb->fgColor, true);
    UI_RenderLabel(rc, pb->progressLabel);
}

// PROGRESSBAR END