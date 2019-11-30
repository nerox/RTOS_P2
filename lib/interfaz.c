
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <gtk/gtk.h>
#include <interfaz.h>
#include <algoritmos.h>


GtkWidget *assistant;
GtkWidget *view;
GtkWidget *window;
GtkTreeModel *model;
bool slide = 0;
bool rm_state = 0;
bool edf_state = 0;
bool llf_state = 0;
bool first_time = 0;
int test_time = 0;
GtkWidget *view_summary;
GtkTreeModel *model_summary;

void
on_assistant_apply (GtkWidget *widget, gpointer data)
{
  create_tasks();
  gtk_main_quit();
}

void
on_assistant_close_cancel (GtkWidget *widget, gpointer data)
{
  GtkWidget **assistant = (GtkWidget **) data;

  gtk_widget_destroy (*assistant);
  *assistant = NULL;
  for (uint i = 0; i < NUMBER_OF_TASKS; i++) {
    printf("Process %d Ci = %d Pi = %i\n",i, TASKS_TO_SCHEDULE[i].computation_time , TASKS_TO_SCHEDULE[i].period_time );
  }
  printf("test_destroy\n" );
  gtk_main_quit();
}

void cell_edited_comp_time (GtkCellRendererText *cell,
                                  gchar               *path_string,
                                  gchar               *new_text,
                                  gpointer             user_data)
{
  GtkTreeModel *ListModel;
  ListModel = gtk_tree_view_get_model (GTK_TREE_VIEW(view));

  uint aux_int = atoi(new_text);
  printf("int %d char %s\n",aux_int,new_text );


  GtkListStore *store = (GtkListStore*) ListModel;
  GtkTreeIter iter;
  GtkTreePath *path;

  path = gtk_tree_path_new_from_string (path_string);
       gtk_tree_model_get_iter (GTK_TREE_MODEL (ListModel),
                                &iter,
                                path);
  gtk_list_store_set (store, &iter,
              COL_COMPUTATION_TIME, aux_int ,
              -1);
  printf("Ci %s\n", path_string);
  TASKS_TO_SCHEDULE[atoi(path_string)].computation_time = aux_int;
}

void cell_edited_period (GtkCellRendererText *cell,
                                  gchar               *path_string,
                                  gchar               *new_text,
                                  gpointer             user_data)
{
  GtkTreeModel *ListModel;
  ListModel = gtk_tree_view_get_model (GTK_TREE_VIEW(view));

  uint aux_int = atoi(new_text);

  printf("int %d char %s\n",aux_int,new_text );


  GtkListStore *store = (GtkListStore*) ListModel;
  GtkTreeIter iter;
  GtkTreePath *path;

  path = gtk_tree_path_new_from_string (path_string);
       gtk_tree_model_get_iter (GTK_TREE_MODEL (ListModel),
                                &iter,
                                path);
  gtk_list_store_set (store, &iter,
              COL_PERIOD, aux_int ,
              -1);
  printf("Pi %s\n", path_string);
  TASKS_TO_SCHEDULE[atoi(path_string)].period_time = aux_int;
}

gboolean update_model (gpointer user_data, int current_page)
{
  GtkListStore *store = (GtkListStore*) user_data;
  GtkTreeIter iter;
  GtkTreePath *path;
  path = gtk_tree_path_new_from_string ("1");
       gtk_tree_model_get_iter (GTK_TREE_MODEL (user_data),
                                &iter,
                                path);


  for (uint i = 0; i < NUMBER_OF_TASKS; i++) {
    gtk_list_store_append (store, &iter);
    gtk_list_store_set (store, &iter,
                        COL_TASK_ID, i,
                        COL_COMPUTATION_TIME, 0,
                        COL_PERIOD, 0,
                        -1);
    if (current_page == 2) {
      TASKS_TO_SCHEDULE[i].id=i;
      TASKS_TO_SCHEDULE[i].period_time = 0;
      TASKS_TO_SCHEDULE[i].computation_time = 0;
    }
  }

  return TRUE;
}

void fill_out_model_summary ()
{
  GtkTreeModel *ListModel;
  ListModel = gtk_tree_view_get_model (GTK_TREE_VIEW(view_summary));

  test_time = 0;

  GtkListStore *store = (GtkListStore*) ListModel;
  GtkTreeIter iter;
  GtkTreePath *path;

  for (uint i = 0; i < NUMBER_OF_TASKS; i++) {
  path = gtk_tree_path_new_from_indices (i,-1);
  gtk_tree_model_get_iter (GTK_TREE_MODEL (ListModel),
                                &iter,
                                path);

    gtk_list_store_set (store, &iter,
                        COL_COMPUTATION_TIME, TASKS_TO_SCHEDULE[i].computation_time,
                        COL_PERIOD, TASKS_TO_SCHEDULE[i].period_time,
                        -1);

    if (TASKS_TO_SCHEDULE[i].computation_time > TASKS_TO_SCHEDULE[i].period_time){
      test_time = test_time + 1;
    }
    printf("Task %d Ci %d Di %d\n", i, TASKS_TO_SCHEDULE[i].computation_time,TASKS_TO_SCHEDULE[i].period_time);
  }

  GtkWidget *current_page;
  gint page_number;

  page_number = gtk_assistant_get_current_page (GTK_ASSISTANT(assistant));
  current_page = gtk_assistant_get_nth_page (GTK_ASSISTANT(assistant), page_number);


  if (test_time > 0)
      gtk_assistant_set_page_complete (GTK_ASSISTANT(assistant), current_page, FALSE);
  else
      gtk_assistant_set_page_complete (GTK_ASSISTANT(assistant), current_page, TRUE);
}


GtkWidget *
create_view_and_model (int num_process, int current_page)
{
  GtkCellRenderer     *renderer;
  GtkTreeModel        *model;
  GtkWidget           *view_generic;

  view_generic = gtk_tree_view_new ();

  /* --- Column #1 --- */

  renderer = gtk_cell_renderer_text_new ();
  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view_generic),
                                               -1,
                                               "Task ID",
                                               renderer,
                                               "text", COL_TASK_ID,
                                               NULL);

  /* --- Column #2 --- */

  renderer = gtk_cell_renderer_text_new ();
  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view_generic),-1, "Computation Time (Ci)",
                                               renderer, "text", COL_COMPUTATION_TIME,NULL);

if (current_page == 3) {
  g_object_set(renderer, "editable", TRUE, NULL);
}


  g_signal_connect(renderer, "edited", (GCallback) cell_edited_comp_time, NULL);

  renderer = gtk_cell_renderer_text_new ();
  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view_generic),
                                             -1,
                                             "Period (Pi)",
                                             renderer,
                                             "text", COL_PERIOD,
                                             NULL);

  if (current_page == 3) {
   g_object_set(renderer, "editable", TRUE, NULL);
  }

  g_signal_connect(renderer, "edited", (GCallback) cell_edited_period, NULL);

  model = GTK_TREE_MODEL(gtk_list_store_new (NUM_COLS, G_TYPE_UINT, G_TYPE_UINT, G_TYPE_UINT));
  gtk_tree_view_set_model (GTK_TREE_VIEW (view_generic), model);

  g_object_unref (model);

  return view_generic;
}

void
on_entry_changed (GtkWidget *widget, gpointer data)
{
  GtkAssistant *assistant = GTK_ASSISTANT (data);
  GtkWidget *current_page;
  gint page_number;
  const gchar *text;

  page_number = gtk_assistant_get_current_page (assistant);
  current_page = gtk_assistant_get_nth_page (assistant, page_number);
  text = gtk_entry_get_text (GTK_ENTRY (widget));
  printf("char: %s\n", text);
  NUMBER_OF_TASKS = atoi(text);
  printf("numero: %d\n", NUMBER_OF_TASKS);
  if (text && *text)
    gtk_assistant_set_page_complete (assistant, current_page, TRUE);
  else
    gtk_assistant_set_page_complete (assistant, current_page, FALSE);
}

void
create_page1 (GtkWidget *assistant)
{
  GtkWidget *box, *label, *entry;

  box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 12);
  gtk_container_set_border_width (GTK_CONTAINER (box), 12);

  label = gtk_label_new ("Ingrese la cantidad de procesos:");
  gtk_box_pack_start (GTK_BOX (box), label, FALSE, FALSE, 0);

  entry = gtk_entry_new ();
  gtk_entry_set_activates_default (GTK_ENTRY (entry), TRUE);
  gtk_widget_set_valign (entry, GTK_ALIGN_CENTER);
  gtk_box_pack_start (GTK_BOX (box), entry, TRUE, TRUE, 0);
  g_signal_connect (G_OBJECT (entry), "changed",
                    G_CALLBACK (on_entry_changed), assistant);

  gtk_widget_show_all (box);
  gtk_assistant_append_page (GTK_ASSISTANT (assistant), box);
  gtk_assistant_set_page_title (GTK_ASSISTANT (assistant), box, "Procesos");
  gtk_assistant_set_page_type (GTK_ASSISTANT (assistant), box, GTK_ASSISTANT_PAGE_INTRO);
}


void
algorithm_toggled_cb_rm (GtkToggleButton *toggle_button,
            gpointer         user_data)
{
  GtkAssistant *assistant = GTK_ASSISTANT (user_data);
  GtkWidget *current_page;
  gint page_number;

  page_number = gtk_assistant_get_current_page (assistant);
  current_page = gtk_assistant_get_nth_page (assistant, page_number);

  if (gtk_toggle_button_get_active (toggle_button)){
      rm_state = true;
      printf("RM activo bool %d\n",rm_state);
    }
  else {
    rm_state = false;
    printf("RM inactivo bool %d\n",rm_state);
    }
  if ((rm_state == 1) || (edf_state == 1) || (llf_state == 1))
      gtk_assistant_set_page_complete (assistant, current_page, TRUE);
  else
      gtk_assistant_set_page_complete (assistant, current_page, FALSE);
}

void
algorithm_toggled_cb_edf (GtkToggleButton *toggle_button,
            gpointer         user_data)
{
  GtkAssistant *assistant = GTK_ASSISTANT (user_data);
  GtkWidget *current_page;
  gint page_number;

  page_number = gtk_assistant_get_current_page (assistant);
  current_page = gtk_assistant_get_nth_page (assistant, page_number);

  if (gtk_toggle_button_get_active (toggle_button)){
      edf_state = true;
      printf("EDF activo bool %d\n",edf_state);
    }
  else {
    edf_state = false;
    printf("EDF inactivo bool %d\n",edf_state);
    }
  if ((rm_state == 1) || (edf_state == 1) || (llf_state == 1))
      gtk_assistant_set_page_complete (assistant, current_page, TRUE);
  else
      gtk_assistant_set_page_complete (assistant, current_page, FALSE);
}

void
algorithm_toggled_cb_llf (GtkToggleButton *toggle_button,
            gpointer         user_data)
{
  GtkAssistant *assistant = GTK_ASSISTANT (user_data);
  GtkWidget *current_page;
  gint page_number;

  page_number = gtk_assistant_get_current_page (assistant);
  current_page = gtk_assistant_get_nth_page (assistant, page_number);

  if (gtk_toggle_button_get_active (toggle_button)){
      llf_state = true;
      printf("LLF activo bool %d\n",llf_state);
    }
  else {
    llf_state = false;
    printf("LLF inactivo bool %d\n",llf_state);
    }
  if ((rm_state == 1) || (edf_state == 1) || (llf_state == 1))
      gtk_assistant_set_page_complete (assistant, current_page, TRUE);
  else
      gtk_assistant_set_page_complete (assistant, current_page, FALSE);
}

void
create_page2 (GtkWidget *assistant)
{
  GtkWidget *box, *rm;
  GtkWidget *edf, *llf;

  box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 12);
  gtk_container_set_border_width (GTK_CONTAINER (box), 12);

  rm = gtk_check_button_new_with_label ("Rate Monotonic (RM)");
  edf = gtk_check_button_new_with_label ("Earliest Deadline First (EDF)");
  llf = gtk_check_button_new_with_label ("Least Laxity First (LLF)");

  g_signal_connect (GTK_TOGGLE_BUTTON (rm), "toggled", G_CALLBACK (algorithm_toggled_cb_rm), assistant);
  g_signal_connect (GTK_TOGGLE_BUTTON (edf), "toggled", G_CALLBACK (algorithm_toggled_cb_edf), assistant);
  g_signal_connect (GTK_TOGGLE_BUTTON (llf), "toggled", G_CALLBACK (algorithm_toggled_cb_llf), assistant);
  gtk_box_pack_start (GTK_BOX (box), rm, FALSE, FALSE, 0);
  gtk_box_pack_start (GTK_BOX (box), edf, FALSE, FALSE, 0);
  gtk_box_pack_start (GTK_BOX (box), llf, FALSE, FALSE, 0);
  gtk_widget_show_all (box);
  gtk_assistant_append_page (GTK_ASSISTANT (assistant), box);
  gtk_assistant_set_page_title (GTK_ASSISTANT (assistant), box, "Calendarizador");
  gtk_assistant_set_page_type (GTK_ASSISTANT (assistant), box, GTK_ASSISTANT_PAGE_INTRO);
}



void
create_page5 (GtkWidget *assistant)
{
  GtkWidget *box1;
  GtkWidget *label;
  GtkWidget *grid;
  GtkWidget *algoritmos, *rm, *llf, *edf, *slides;

  box1 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_show (box1);

  view_summary = create_view_and_model (NUMBER_OF_TASKS, 4);
  gtk_box_pack_start (GTK_BOX (box1), view_summary, TRUE, TRUE, 0);
  gtk_widget_show (view_summary);


  algoritmos = gtk_label_new ("Algoritmos:");
  rm = gtk_label_new ("   - RM");
  llf = gtk_label_new ("  - LLF");
  edf = gtk_label_new ("  - EDF");
  grid = gtk_grid_new ();
  gtk_grid_attach (GTK_GRID (grid), algoritmos, 0, 0, 1, 1);
  gtk_widget_show (algoritmos);

  if ((rm_state == 0) && (llf_state == 0) && (edf_state == 1)) {
    gtk_grid_attach (GTK_GRID (grid), edf, 0, 1, 1, 1);
    gtk_widget_show (edf);
    ALGORITHMS_TO_EXECUTE = 2;
  }
  else if ((rm_state == 0) && (llf_state == 1) && (edf_state == 0)) {
    gtk_grid_attach (GTK_GRID (grid), llf, 0, 1, 1, 1);
    gtk_widget_show (llf);
    ALGORITHMS_TO_EXECUTE = 3;
  }
  else if ((rm_state == 0) && (llf_state == 1) && (edf_state == 1)) {
    gtk_grid_attach (GTK_GRID (grid), edf, 0, 1, 1, 1);
    gtk_grid_attach (GTK_GRID (grid), llf, 0, 2, 1, 1);
    gtk_widget_show (edf);
    gtk_widget_show (llf);
    ALGORITHMS_TO_EXECUTE = 6;
  }
  else if ((rm_state == 1) && (llf_state == 0) && (edf_state == 0)) {
    gtk_grid_attach (GTK_GRID (grid), rm, 0, 1, 1, 1);
    gtk_widget_show (rm);
    ALGORITHMS_TO_EXECUTE = 1;
  }
  else if ((rm_state == 1) && (llf_state == 0) && (edf_state == 1)) {
    gtk_grid_attach (GTK_GRID (grid), rm, 0, 1, 1, 1);
    gtk_grid_attach (GTK_GRID (grid), edf, 0, 2, 1, 1);
    gtk_widget_show (edf);
    gtk_widget_show (rm);
    ALGORITHMS_TO_EXECUTE = 4;
  }
  else if ((rm_state == 1) && (llf_state == 1) && (edf_state == 0)) {
    gtk_grid_attach (GTK_GRID (grid), rm, 0, 1, 1, 1);
    gtk_grid_attach (GTK_GRID (grid), llf, 0, 2, 1, 1);
    gtk_widget_show (rm);
    gtk_widget_show (llf);
    ALGORITHMS_TO_EXECUTE = 5;
  }
  else if ((rm_state == 1) && (llf_state == 1) && (edf_state == 1)) {
    gtk_grid_attach (GTK_GRID (grid), rm, 0, 1, 1, 1);
    gtk_grid_attach (GTK_GRID (grid), edf, 0, 2, 1, 1);
    gtk_grid_attach (GTK_GRID (grid), llf, 0, 3, 1, 1);
    gtk_widget_show (edf);
    gtk_widget_show (rm);
    gtk_widget_show (llf);
    ALGORITHMS_TO_EXECUTE = 7;
  }

  if (slide == 0) {
    slides = gtk_label_new ("> Slide Unico");
  }
  else{
    slides = gtk_label_new ("> Slide Independiente");
  }

  gtk_grid_attach (GTK_GRID (grid), slides, 0, 5, 1, 1);
  gtk_widget_show (slides);

  gtk_box_pack_start (GTK_BOX (box1), grid, TRUE, TRUE, 0);
  gtk_widget_show (grid);

  label = gtk_label_new ("Si el boton Apply no esta habilitado, en alguna tarea el Ci es mayor a Pi");
  gtk_box_pack_start (GTK_BOX (box1), label, TRUE, TRUE, 0);
  gtk_widget_show (label);

  gtk_assistant_append_page (GTK_ASSISTANT (assistant), box1);
  gtk_assistant_set_page_type (GTK_ASSISTANT (assistant), box1, GTK_ASSISTANT_PAGE_CONFIRM);
  gtk_assistant_set_page_complete (GTK_ASSISTANT (assistant), box1, TRUE);
  gtk_assistant_set_page_title (GTK_ASSISTANT (assistant), box1, "Resumen");
}

void
button_toggled_cb (GtkWidget *button,
                   gpointer   user_data)
{
  const char *button_label;

  button_label = gtk_button_get_label (GTK_BUTTON (button));
  //*************True: Independiente False: Unico***********
  printf("True: Independiente False: Unico\n");
  if ( strcmp(button_label, "Slide Unico")) {
    slide = true;
    printf("%s bool: %d\n", button_label, slide);
  }
  else{
    slide = false;
    printf("%s bool: %d\n", button_label, slide);
  }
  printf("actual bool value %d\n", slide);
}

void
create_page3 (GtkWidget *assistant)
{

  GtkWidget *box1;
    GtkWidget *box2;
    GtkWidget *button1;
    GtkWidget *button2;
    GtkWidget *separator;
    GSList *group;


    box1 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_show (box1);

    box2 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width (GTK_CONTAINER (box2), 10);
    gtk_box_pack_start (GTK_BOX (box1), box2, TRUE, TRUE, 0);
    gtk_widget_show (box2);

    button1 = gtk_radio_button_new_with_label (NULL, "Slide Unico");
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button1), FALSE);
    gtk_box_pack_start (GTK_BOX (box2), button1, TRUE, TRUE, 0);
    gtk_widget_show (button1);


    group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (button1));
    button2 = gtk_radio_button_new_with_label (group, "Slide Independiente");
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button2), FALSE);
    gtk_box_pack_start (GTK_BOX (box2), button2, TRUE, TRUE, 0);
    gtk_widget_show (button2);

    g_signal_connect (GTK_TOGGLE_BUTTON (button1), "toggled",
                    G_CALLBACK (button_toggled_cb), NULL);
    g_signal_connect (GTK_TOGGLE_BUTTON (button2), "toggled",
                    G_CALLBACK (button_toggled_cb), NULL);

    separator = gtk_separator_new (GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start (GTK_BOX (box1), separator, FALSE, TRUE, 0);
    gtk_widget_show (separator);

    box2 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width (GTK_CONTAINER (box2), 10);
    gtk_box_pack_start (GTK_BOX (box1), box2, FALSE, TRUE, 0);
    gtk_widget_show (box2);

  gtk_assistant_append_page (GTK_ASSISTANT (assistant), box1);
  gtk_assistant_set_page_type (GTK_ASSISTANT (assistant), box1, GTK_ASSISTANT_PAGE_INTRO);
  gtk_assistant_set_page_complete (GTK_ASSISTANT (assistant), box1, TRUE);
  gtk_assistant_set_page_title (GTK_ASSISTANT (assistant), box1, "Slide Option");
}

void create_page4 (GtkWidget *assistant){
  printf("Cantidad de Procesos: %d\n", NUMBER_OF_TASKS);
  view = create_view_and_model (NUMBER_OF_TASKS, 3);

  gtk_widget_show_all (view);
  gtk_assistant_append_page (GTK_ASSISTANT (assistant), view);
  gtk_assistant_set_page_type (GTK_ASSISTANT (assistant), view, GTK_ASSISTANT_PAGE_INTRO);
  gtk_assistant_set_page_complete (GTK_ASSISTANT (assistant), view, TRUE);
  gtk_assistant_set_page_title (GTK_ASSISTANT (assistant), view, "Caracteristicas");
}

void
on_assistant_prepare (GtkWidget *widget, GtkWidget *page, gpointer data)
{
  gint current_page, n_pages;
  gchar *title;
  GtkTreeModel *ListModel;

  current_page = gtk_assistant_get_current_page (GTK_ASSISTANT (widget));
  n_pages = gtk_assistant_get_n_pages (GTK_ASSISTANT (widget));
  printf("page %d\n", current_page );
  title = g_strdup_printf ("Sample assistant (%d of %d)", current_page + 1, n_pages);
  gtk_window_set_title (GTK_WINDOW (widget), title);
  g_free (title);


  if (current_page == 1) {
    TASKS_TO_SCHEDULE =  (task_information *)malloc(NUMBER_OF_TASKS * sizeof(task_information));
  }
  else if (current_page == 2) {
    ListModel = gtk_tree_view_get_model (GTK_TREE_VIEW(view));
    update_model(ListModel, 2);
  }
  else if (current_page == 3 && first_time == 0){
    create_page5 (assistant);
    ListModel = gtk_tree_view_get_model (GTK_TREE_VIEW(view_summary));
    update_model(ListModel, 3);
  }
  else if (current_page == 4){
    first_time = 1;
    fill_out_model_summary();
    }
}

void interfaz_setup (){
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "Window");
  gtk_container_set_border_width (GTK_CONTAINER (window), 10);

  assistant = gtk_assistant_new();
    gtk_window_set_default_size (GTK_WINDOW (assistant), -1, 350);



    gtk_window_set_screen (GTK_WINDOW (assistant),gtk_widget_get_screen (window));

    create_page1 (assistant);
    create_page2 (assistant);
    create_page3 (assistant);
    create_page4 (assistant);
    g_signal_connect (G_OBJECT (assistant), "cancel",G_CALLBACK(gtk_main_quit), G_OBJECT(window));
    g_signal_connect (G_OBJECT (assistant), "close",
                      G_CALLBACK (on_assistant_close_cancel), &assistant);
    g_signal_connect (G_OBJECT (assistant), "apply",
                      G_CALLBACK (on_assistant_apply), NULL);
    g_signal_connect (G_OBJECT (assistant), "prepare",
                      G_CALLBACK (on_assistant_prepare), NULL);
    g_signal_connect(G_OBJECT(window), "destroy",G_CALLBACK(gtk_main_quit), G_OBJECT(window));

  gtk_widget_show(assistant);

}
