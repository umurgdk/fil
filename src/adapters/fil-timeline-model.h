#pragma once

#include <glib-object.h>
#include <gtk/gtk.h>

#include "../models.h"

G_BEGIN_DECLS

enum FilTimelineModelColumn {
    FIL_TIMELINE_TOOT = 0,
    FIL_TIMELINE_COUNT
};

#define FIL_TYPE_TIMELINE_MODEL fil_timeline_model_get_type()
G_DECLARE_FINAL_TYPE(FilTimelineModel, fil_timeline_model, FIL, TIMELINE_MODEL, GObject)

FilTimelineModel *
fil_timeline_model_new(GList *toots_list);

void
fil_timeline_model_prepend_toot(FilTimelineModel *model, Toot *toot);

void
fil_timeline_model_remove_toot(FilTimelineModel *model, Toot *toot);

G_END_DECLS

