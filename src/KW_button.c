#include "KW_button.h"
#include "KW_button_internal.h"
#include "KW_label.h"
#include "KW_tilerenderer.h"
#include "KW_gui.h"

KW_Button * AllocButton();
void PaintButton(KW_Widget * widget, const KW_Rect * absolute, void * data);
void RenderButton(KW_Widget * widget);
void DestroyButton(KW_Widget * widget);
void ButtonGeometryChanged(KW_Widget * widget, const KW_Rect * newgeom, const KW_Rect * oldgeom);

static void MouseOver(KW_Widget * widget) {
  KW_Button * button = KW_GetWidgetData(widget, PaintButton);
  button->mouseover = KW_TRUE;
}


static void MouseLeave(KW_Widget * widget) {
  KW_Button * button = KW_GetWidgetData(widget, PaintButton);
  button->mouseover = KW_FALSE;
}

static void MousePress(KW_Widget * widget, int b) {
  KW_Button * button = KW_GetWidgetData(widget, PaintButton);
  (void) b;
  button->clicked = KW_TRUE;
}

static void MouseRelease(KW_Widget * widget, int b) {
  KW_Button * button = KW_GetWidgetData(widget, PaintButton);
  (void) b;
  button->clicked = KW_FALSE;
}


KW_Widget * KW_CreateButton(KW_GUI * gui, KW_Widget * parent, const char * text, const KW_Rect * geometry) {
  KW_Rect labelgeom;
  KW_Widget * widget = NULL;
  KW_Button * button = NULL;
  labelgeom.x = TILESIZE;
  labelgeom.y = TILESIZE;
  labelgeom.w = geometry->w - TILESIZE * 2;
  labelgeom.h = geometry->h - TILESIZE * 2;
  
  button = AllocButton();
  widget = KW_CreateWidget(gui, parent, geometry, PaintButton, DestroyButton, button);
  button->labelwidget = KW_CreateLabel(gui, widget, text, &labelgeom);
  KW_BlockWidgetInputEvents(button->labelwidget);
  KW_AddWidgetMouseOverHandler(widget, MouseOver);
  KW_AddWidgetMouseLeaveHandler(widget, MouseLeave);
  KW_AddWidgetMouseDownHandler(widget, MousePress);
  KW_AddWidgetMouseUpHandler(widget, MouseRelease);
  KW_AddWidgetGeometryChangeHandler(widget, ButtonGeometryChanged);
  KW_AddWidgetTilesetChangeHandler(widget, RenderButton);
  RenderButton(widget);
  return widget;
}

/** internal stuff **/
void PaintButton(KW_Widget * widget, const KW_Rect * absolute, void * data) {
  KW_Button * button = (KW_Button *) data;
  KW_RenderDriver * renderer = KW_GetWidgetRenderer(widget);
  /* don't draw frame if requested. */
  if (KW_QueryWidgetHint(widget, KW_WIDGETHINT_FRAMELESS) == KW_TRUE)
    return;

  if (button->mouseover)
    KW_RenderCopy(renderer, button->normal, NULL, absolute);
  else
    KW_RenderCopy(renderer, button->over, NULL, absolute);
}

void RenderButton(KW_Widget * widget) {
  KW_Rect targetgeom;
  KW_Surface * tileset = KW_GetWidgetTilesetSurface(widget);
  KW_Button * button = KW_GetWidgetData(widget, PaintButton);
  KW_RenderDriver * driver = KW_GetWidgetRenderer(widget);
  KW_GetWidgetAbsoluteGeometry(widget, &targetgeom);  
  if (button->normal != NULL) KW_ReleaseTexture(driver, button->normal);
  button->normal = KW_CreateTileFrameTexture(KW_GetWidgetRenderer(widget), tileset, 3, 3, targetgeom.w, targetgeom.h, KW_FALSE, KW_FALSE);
  if (button->over != NULL) KW_ReleaseTexture(driver, button->over);
  button->over = KW_CreateTileFrameTexture(driver, tileset, 3, 0, targetgeom.w, targetgeom.h, KW_FALSE, KW_FALSE);
}

void DestroyButton(KW_Widget * widget) {
  KW_Button * button = KW_GetWidgetData(widget, PaintButton);
  KW_RenderDriver * driver = KW_GetWidgetRenderer(widget);
  KW_ReleaseTexture(driver, button->over);
  KW_ReleaseTexture(driver, button->normal);
  free(button);
}

void ButtonGeometryChanged(KW_Widget * widget, const KW_Rect * newgeom, const KW_Rect * oldgeom) {
  KW_Rect labelgeom;
  KW_Button * button = KW_GetWidgetData(widget, PaintButton);
  
  if (oldgeom->w != newgeom->w || oldgeom->h != newgeom->h) {
    RenderButton(widget);
  }
  
  labelgeom.x = TILESIZE;
  labelgeom.y = TILESIZE;
  labelgeom.w = newgeom->w - TILESIZE * 2;
  labelgeom.h = newgeom->h - TILESIZE * 2;
  KW_SetWidgetGeometry(button->labelwidget, &labelgeom);
}


KW_Button * AllocButton() {
  KW_Button * button = calloc(sizeof(KW_Button), 1);
  return button;
}

void KW_SetButtonIcon(KW_Widget * widget, const KW_Rect * iconclip) {
  KW_Button * button = KW_GetWidgetData(widget, PaintButton);
  KW_SetLabelIcon(button->labelwidget, iconclip);
}

void KW_SetButtonText(KW_Widget * widget, const char * text) {
  KW_Button * button = KW_GetWidgetData(widget, PaintButton);
  KW_SetLabelText(button->labelwidget, text);
}



void KW_SetButtonFont(KW_Widget * widget, KW_Font * font) {
  KW_Button * button = KW_GetWidgetData(widget, PaintButton);
  KW_SetLabelFont(button->labelwidget, font);
}


void KW_SetButtonTextColor(KW_Widget * widget, KW_Color color) {
  KW_Button * button = KW_GetWidgetData(widget, PaintButton);
  KW_SetLabelTextColor(button->labelwidget, color);
}

KW_Color KW_GetButtonTextColor(KW_Widget * widget) {
  KW_Button * button = KW_GetWidgetData(widget, PaintButton);
  return KW_GetLabelTextColor(button->labelwidget);
}

KW_bool KW_WasButtonColorSet(KW_Widget * widget) {
  KW_Button *button = KW_GetWidgetData(widget, PaintButton);
  return KW_WasLabelTextColorSet(button->labelwidget);
}