#include "fil-toot-cell-renderer.h"
#include "../models.h"

enum PROPS {
    TOOT_PROP = 1,
    N_PROPS
};

struct _FilTootCellRenderer {
    GtkCellRenderer parent_instance;

    Toot *toot;
};

static GtkCellRendererClass *parent_class = NULL;

////////////////////////////////////////////////////////////////////////////////////////////////////
// #region FilCellRenderer Public Methods
//

FilTootCellRenderer *
fil_toot_cell_renderer_new()
{
  FilTootCellRenderer *instance = g_object_new (FIL_TYPE_TOOT_CELL_RENDERER, NULL);
  return instance;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// FilCellRenderer GObject Overrides
//

static void
fil_toot_cell_renderer_get_property(GObject    *object,
                                    guint       property_id,
                                    GValue     *value,
                                    GParamSpec *pspec)
{
  FilTootCellRenderer *self = FIL_TOOT_CELL_RENDERER (object);

  switch (property_id)
  {
    case TOOT_PROP:
      g_value_set_pointer (value, self->toot);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
  }
}

static void
fil_toot_cell_renderer_set_property(GObject      *object,
                                    guint         property_id,
                                    const GValue *value,
                                    GParamSpec   *pspec)
{
  FilTootCellRenderer *self = FIL_TOOT_CELL_RENDERER (object);

  switch (property_id)
  {
    case TOOT_PROP:
      self->toot = (Toot *)g_value_get_pointer (value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id,pspec);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// FilCellRenderer GtkCellRenderer Overrides
//

static void
fil_toot_cell_renderer_get_size(GtkCellRenderer      *cell,
                                GtkWidget            *widget,
                                const GdkRectangle   *cell_area,
                                gint                 *x_offset,
                                gint                 *y_offset,
                                gint                 *width,
                                gint                 *height)
{
  g_return_if_fail (cell_area);
  
  gint cell_width = cell_area->width;
  gint cell_height = cell_width / 10;

  *width = cell_width;
  *height = cell_height;
}

static void
fil_toot_cell_renderer_render(GtkCellRenderer      *cell,
                              cairo_t              *cr,
                              GtkWidget            *widget,
                              const GdkRectangle   *background_area,
                              const GdkRectangle   *cell_area,
                              GtkCellRendererState  flags)
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////
// FilCellRenderer Class Initialization
//

static void
fil_toot_cell_renderer_class_init(FilTootCellRendererClass *klass)
{
  GtkCellRendererClass *cell_class   = GTK_CELL_RENDERER_CLASS (klass);
  GObjectClass         *object_class = G_OBJECT_CLASS (klass);

  parent_class = g_type_class_peek_parent (klass);

  object_class->get_property = fil_toot_cell_renderer_get_property;
  object_class->set_property = fil_toot_cell_renderer_set_property;

  GParamSpec *toot_param = g_param_spec_pointer ("toot",
                                                 "Toot",
                                                 "Toot to display",
                                                 G_PARAM_READWRITE);

  g_object_class_install_property(object_class, TOOT_PROP, toot_param);

  cell_class->render = fil_toot_cell_renderer_render;
  cell_class->get_size = fil_toot_cell_renderer_get_size;
}

static void
fil_toot_cell_renderer_init(FilTootCellRenderer *self)
{

}

G_DEFINE_TYPE(FilTootCellRenderer, fil_toot_cell_renderer, GTK_TYPE_CELL_RENDERER)