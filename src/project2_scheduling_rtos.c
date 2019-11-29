
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <gtk/gtk.h>
#include <interfaz.h>
#include <algoritmos.h>



int main (int argc, char **argv)
{
	gtk_init (&argc, &argv);

  interfaz_setup ();
  gtk_main ();

	return 0;
}
