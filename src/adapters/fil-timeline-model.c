
#include "fil-timeline-model.h"
#include "../models.h"

static const GType fil_timeline_model_column_types[] = {
    G_TYPE_POINTER
};

struct _FilTimelineModel {
    GObject parent_instance;

    guint stamp;
    GList *toots;
    guint  count;
};

static void
fil_timeline_model_init (FilTimelineModel *model);

static void
fil_timeline_model_finalize (GObject *object);

static void
fil_timeline_model_class_init (FilTimelineModelClass *klass);

static void
fil_timeline_model_tree_model_interface_init (GtkTreeModelIface *iface);

// GtkTreeModel Interface Implementation
static GtkTreeModelFlags
fil_timeline_model_get_flags (GtkTreeModel *model);

static gint
fil_timeline_model_get_n_columns (GtkTreeModel *model);

static GType
fil_timeline_model_get_column_type (GtkTreeModel *model,
                                    gint index_);

static gboolean
fil_timeline_model_get_iter (GtkTreeModel *model,
                             GtkTreeIter *iter,
                             GtkTreePath *path);

static GtkTreePath *
fil_timeline_model_get_path (GtkTreeModel *model,
                             GtkTreeIter *iter);

static void
fil_timeline_model_get_value (GtkTreeModel *model,
                              GtkTreeIter *iter,
                              gint column,
                              GValue *value);

static gboolean
fil_timeline_model_iter_next (GtkTreeModel *model,
                              GtkTreeIter *iter);

static gboolean
fil_timeline_model_iter_previous (GtkTreeModel *model,
                                  GtkTreeIter *iter);

static gboolean
fil_timeline_model_iter_children (GtkTreeModel *model,
                                  GtkTreeIter *iter,
                                  GtkTreeIter *parent);

static gboolean
fil_timeline_model_iter_has_child (GtkTreeModel *model,
                                   GtkTreeIter *iter);

static gint
fil_timeline_model_iter_n_children (GtkTreeModel *model,
                                    GtkTreeIter *iter);

static gboolean
fil_timeline_model_iter_nth_child (GtkTreeModel *model,
                                   GtkTreeIter *iter,
                                   GtkTreeIter *parent,
                                   gint n);

static gboolean
fil_timeline_model_iter_parent (GtkTreeModel *model,
                                GtkTreeIter *iter,
                                GtkTreeIter *child);

static void
fil_timeline_model_ref_node (GtkTreeModel *model,
                             GtkTreeIter *iter);

static void
fil_timeline_model_unref_node (GtkTreeModel *model,
                               GtkTreeIter *iter);

static GObjectClass *parent_class = NULL;

G_DEFINE_TYPE_WITH_CODE(FilTimelineModel, fil_timeline_model, G_TYPE_OBJECT,
                        G_IMPLEMENT_INTERFACE (GTK_TYPE_TREE_MODEL, fil_timeline_model_tree_model_interface_init))

FilTimelineModel *
fil_timeline_model_new (GList *toots_list)
{
  FilTimelineModel *model = g_object_new (FIL_TYPE_TIMELINE_MODEL, NULL);
  model->toots = toots_list;
  model->count = g_list_length(toots_list);

  return model;
}

void
fil_timeline_model_prepend_toot (FilTimelineModel *model, Toot *toot)
{
  g_return_if_fail (FIL_IS_TIMELINE_MODEL (model));
  g_return_if_fail (toot);

  model->toots = g_list_append (model->toots, toot);
  model->count += 1;
  guint index = model->count - 1;

  GtkTreePath *path = gtk_tree_path_new();

  gtk_tree_path_append_index (path, index);

  GtkTreeIter iter;
  iter.stamp = model->stamp;
  iter.user_data = toot;
  iter.user_data2 = g_list_last (model->toots);

  gtk_tree_model_row_inserted (GTK_TREE_MODEL(model), path, &iter);

  gtk_tree_path_free(path);
}

void
fil_timeline_model_remove_toot (FilTimelineModel *model, Toot *toot)
{
  g_return_if_fail (FIL_IS_TIMELINE_MODEL (model));
  g_return_if_fail (toot);

  GtkTreeIter iter;
  iter.stamp = model->stamp;
  iter.user_data = toot;

  GtkTreePath *path = fil_timeline_model_get_path (GTK_TREE_MODEL(model), &iter);

  model->toots = g_list_remove (model->toots, toot);
  g_signal_emit_by_name(model, "row-deleted", path, NULL);

  gtk_tree_path_free(path);
}

void
fil_timeline_model_init (FilTimelineModel *model)
{
  do
  {
    model->stamp = g_random_int ();
  }
  while (model->stamp == 0);
}

void
fil_timeline_model_finalize (GObject *object)
{
  G_OBJECT_CLASS (fil_timeline_model_parent_class)->finalize(object);
}

void
fil_timeline_model_class_init (FilTimelineModelClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->finalize = fil_timeline_model_finalize;

  // virtual function overrides go here
  // property and signal definitions go here
}

/**
 * GTK_TREE_MODEL interface implementation
 */


void
fil_timeline_model_tree_model_interface_init (GtkTreeModelIface *iface)
{
  iface->get_n_columns = fil_timeline_model_get_n_columns;
  iface->get_column_type = fil_timeline_model_get_column_type;
  iface->get_iter = fil_timeline_model_get_iter;
  iface->get_path = fil_timeline_model_get_path;
  iface->get_value = fil_timeline_model_get_value;
  iface->get_flags = fil_timeline_model_get_flags;
  iface->iter_next = fil_timeline_model_iter_next;
  iface->iter_previous = fil_timeline_model_iter_previous;
  iface->iter_children = fil_timeline_model_iter_children;
  iface->iter_has_child = fil_timeline_model_iter_has_child;
  iface->iter_n_children = fil_timeline_model_iter_n_children;
  iface->iter_nth_child = fil_timeline_model_iter_nth_child;
  iface->iter_parent = fil_timeline_model_iter_parent;
//  iface->
}

GtkTreeModelFlags
fil_timeline_model_get_flags (GtkTreeModel *model)
{
  return GTK_TREE_MODEL_ITERS_PERSIST | GTK_TREE_MODEL_LIST_ONLY;
}

gint
fil_timeline_model_get_n_columns (GtkTreeModel *model)
{
  return 1;
}

GType fil_timeline_model_get_column_type (GtkTreeModel *model, gint index_)
{
  return G_TYPE_STRING;
}

gboolean fil_timeline_model_get_iter (GtkTreeModel *model, GtkTreeIter *iter, GtkTreePath *path)
{
  g_return_val_if_fail (FIL_IS_TIMELINE_MODEL (model), FALSE);

  FilTimelineModel *timeline_model = FIL_TIMELINE_MODEL (model);

  gint *indices = gtk_tree_path_get_indices (path);
  gint  depth   = gtk_tree_path_get_depth (path);

  g_assert(depth == 1);
  gint index = indices[0];

  if (index >= 0 && index < g_list_length (timeline_model->toots)) {
    GList *toot_element = g_list_nth (timeline_model->toots, (guint) index);
    Toot *toot = (Toot *)toot_element->data;
    g_assert(toot != NULL);

    iter->stamp = timeline_model->stamp;
    iter->user_data = toot;
    iter->user_data2 = toot_element;
    return TRUE;
  }

  return FALSE;
}

GtkTreePath *fil_timeline_model_get_path (GtkTreeModel *model, GtkTreeIter *iter)
{
  g_return_val_if_fail (FIL_IS_TIMELINE_MODEL (model), NULL);
  g_return_val_if_fail (iter, NULL);
  g_return_val_if_fail (iter->user_data, NULL);

  FilTimelineModel *timeline_model = FIL_TIMELINE_MODEL (model);

  GtkTreePath *path = gtk_tree_path_new ();
  gint toot_index = g_list_index (timeline_model->toots, iter->user_data);

  gtk_tree_path_append_index (path, toot_index);
  return path;
}

void fil_timeline_model_get_value (GtkTreeModel *model, GtkTreeIter *iter, gint column, GValue *value)
{
  g_return_if_fail (FIL_IS_TIMELINE_MODEL (model));
  g_return_if_fail (iter && iter->user_data);
  g_return_if_fail (column < FIL_TIMELINE_COUNT);

  Toot *toot = (Toot *) iter->user_data;

  g_value_init (value, fil_timeline_model_column_types[column]);

  switch (column) {
    case FIL_TIMELINE_TOOT:
      g_value_set_pointer (value, toot);
      break;

    default:
      break;
  }
}

gboolean fil_timeline_model_iter_next (GtkTreeModel *model, GtkTreeIter *iter)
{
  g_return_val_if_fail (FIL_IS_TIMELINE_MODEL (model), FALSE);
  g_return_val_if_fail (iter && iter->user_data2, FALSE);

  GSList *list_element = iter->user_data2;
  if (list_element->next == NULL)
    return FALSE;


  iter->user_data = list_element->next->data;
  iter->user_data2 = list_element->next;

  return TRUE;
}

gboolean fil_timeline_model_iter_previous (GtkTreeModel *model, GtkTreeIter *iter)
{
  g_return_val_if_fail (FIL_IS_TIMELINE_MODEL (model), FALSE);
  g_return_val_if_fail (iter && iter->user_data, FALSE);
  g_return_val_if_fail (iter->stamp == FIL_TIMELINE_MODEL (model)->stamp, FALSE);

//  FilTimelineModel *timeline_model = FIL_TIMELINE_MODEL (model);
//
//  gint element_index = g_slist_index (timeline_model->toots, iter->user_data);
//
//  if (element_index > 0 && element_index < g_slist_length (timeline_model->toots))
//  {
//    iter->stamp = timeline_model->stamp;
//    iter->user_data = g_slist_nth_data (timeline_model->toots, (guint) (element_index - 1));
//
//    return TRUE;
//  }

  return FALSE;
}

gboolean fil_timeline_model_iter_children (GtkTreeModel *model, GtkTreeIter *iter, GtkTreeIter *parent)
{
//  g_return_val_if_fail (parent == NULL || parent->user_data != NULL, FALSE);
//  g_return_val_if_fail (FIL_IS_TIMELINE_MODEL (model), FALSE);
//
//  FilTimelineModel *timeline_model = FIL_TIMELINE_MODEL (model);
//  if (timeline_model->toots == NULL)
//    return FALSE;
//
//  iter->stamp = timeline_model->stamp;
//  iter->user_data = timeline_model->toots->data;

    return FALSE;

//  return TRUE;
}

gboolean fil_timeline_model_iter_has_child (GtkTreeModel *model, GtkTreeIter *iter)
{
  return FALSE;
}

gint fil_timeline_model_iter_n_children (GtkTreeModel *model, GtkTreeIter *iter)
{
//  if (iter == NULL)
//  {
//    return g_slist_length(FIL_TIMELINE_MODEL (model)->toots);
//  }

  return 0;
}

gboolean fil_timeline_model_iter_nth_child (GtkTreeModel *model, GtkTreeIter *iter, GtkTreeIter *parent, gint n)
{
//  g_return_val_if_fail (FIL_IS_TIMELINE_MODEL (model), FALSE);
//  g_return_val_if_fail (parent == NULL, FALSE);
//
//  FilTimelineModel *timeline_model = FIL_TIMELINE_MODEL (model);
//  g_return_val_if_fail (n >= 0 && n < g_slist_length (timeline_model->toots), FALSE);
//
//  Toot *toot = g_slist_nth_data (timeline_model->toots, (guint) n);
//
//  g_assert(toot != NULL);
//
//  iter->stamp = timeline_model->stamp;
//  iter->user_data = toot;

  return FALSE;
}

gboolean fil_timeline_model_iter_parent (GtkTreeModel *model, GtkTreeIter *iter, GtkTreeIter *child)
{
  return FALSE;
}
