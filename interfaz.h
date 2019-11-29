#ifndef INTERFAZ_H
#define INTERFAZ_H

extern GtkWidget *assistant;
extern GtkWidget *view;
extern GtkWidget *window;
extern GtkTreeModel *model;
extern bool slide;
extern bool rm_state;
extern bool edf_state;
extern bool llf_state;
extern bool first_time;
extern int test_time;

extern GtkWidget *view_summary;
extern GtkTreeModel *model_summary;

enum
{
  COL_TASK_ID,
  COL_COMPUTATION_TIME ,
  COL_PERIOD,
  NUM_COLS
} ;

void on_assistant_apply (GtkWidget *widget, gpointer data);

void on_assistant_close_cancel (GtkWidget *widget, gpointer data);

void cell_edited_comp_time (GtkCellRendererText *cell, gchar *path_string, gchar *new_text, gpointer user_data);

void cell_edited_period (GtkCellRendererText *cell,gchar *path_string, gchar *new_text,gpointer user_data);

gboolean update_model (gpointer user_data, int current_page);

GtkWidget *create_view_and_model (int num_process, int current_page);

void on_entry_changed (GtkWidget *widget, gpointer data);

void create_page1 (GtkWidget *assistant);

void algorithm_toggled_cb_rm (GtkToggleButton *toggle_button, gpointer user_data);

void algorithm_toggled_cb_edf (GtkToggleButton *toggle_button, gpointer user_data);

void algorithm_toggled_cb_llf (GtkToggleButton *toggle_button, gpointer user_data);

void create_page2 (GtkWidget *assistant);

void create_page5 (GtkWidget *assistant);

void button_toggled_cb (GtkWidget *button, gpointer   user_data);

void create_page3 (GtkWidget *assistant);

void create_page4 (GtkWidget *assistant);

void on_assistant_prepare (GtkWidget *widget, GtkWidget *page, gpointer data);

void interfaz_setup ();



#endif
