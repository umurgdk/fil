#pragma once

#include <glib-object.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define FIL_TYPE_TOOT_CELL_RENDERER fil_toot_cell_renderer_get_type()
G_DECLARE_FINAL_TYPE (FilTootCellRenderer, fil_toot_cell_renderer, FIL, TOOT_CELL_RENDERER, GtkCellRenderer);

FilTootCellRenderer *
fil_toot_cell_renderer_new();

G_END_DECLS
