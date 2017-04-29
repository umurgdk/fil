/* main.c
 *
 * Copyright (C) 2017 Umur Gedik
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE X CONSORTIUM BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glib.h>
#include <gio/gio.h>
#include <gtk/gtk.h>
#include <json-glib/json-glib.h>

#include "models.h"
#include "adapters/fil-timeline-model.h"
#include "adapters/fil-toot-cell-renderer.h"

/*======================================================================
 * User Interface
 *
 */

static void
get_toots (FilTimelineModel *timeline_model)
{
  static const char *json_content =
      "[{\"content\": \"Hello mastodon!\"}, {\"content\": \"another post\"}]";

  JsonParser *parser = json_parser_new_immutable ();

  GError *err = NULL;
  json_parser_load_from_data (parser, json_content, strlen (json_content), &err);

  if (err) {
    g_print ("JSON PARSING FAILED!%s\n", err->message);
    g_error_free(err);
    return;
  }

  JsonNode *root = json_parser_get_root (parser);
  JsonArray *arr = json_node_get_array (root);
  guint number_of_toots = json_array_get_length (arr);

  JsonObject *obj;
  for (guint i = 0; i < number_of_toots; i++) {
    obj = json_array_get_object_element (arr, i);
    const char *content = json_object_get_string_member (obj, "content");

    Toot *toot = g_new(Toot, 1);
    toot->content = g_strdup (content);

    fil_timeline_model_prepend_toot (timeline_model, toot);
  }

  g_object_unref (parser);
}

static void
activate (GtkApplication *app,
          gpointer user_data)
{
  GtkWidget *window;

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "slider");
  gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);

  // TODO: add other widgets
  FilTimelineModel *timeline_model = fil_timeline_model_new (NULL);

  GtkWidget *list_view = gtk_tree_view_new_with_model (GTK_TREE_MODEL(timeline_model));

  FilTootCellRenderer *cell_renderer = fil_toot_cell_renderer_new ();
  GtkTreeViewColumn *column = gtk_tree_view_column_new_with_attributes ("Timeline",
                                                                        GTK_CELL_RENDERER(cell_renderer),
                                                                        "toot", FIL_TIMELINE_TOOT,
                                                                        NULL);

  gtk_tree_view_append_column (GTK_TREE_VIEW(list_view), column);

  GtkWidget *scroll = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(scroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

  gtk_container_add (GTK_CONTAINER(scroll), list_view);
  gtk_widget_show (list_view);

  gtk_container_add (GTK_CONTAINER(window), scroll);
  gtk_widget_show (scroll);

//  g_idle_add (add_toots, timeline_model);
  get_toots (timeline_model);
  gtk_widget_show (window);
}

int
main (int argc,
      char *argv[])
{
  g_autoptr(GtkApplication) app = NULL;
  int status;

  app = gtk_application_new ("org.gnome.Slider", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);

  status = g_application_run (G_APPLICATION (app), argc, argv);

  return status;
}

