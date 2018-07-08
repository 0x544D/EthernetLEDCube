#include <gtk/gtk.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>

#include <sys/types.h>
#include <ifaddrs.h>

#include "cube.h"

#define CUBE_DIM    6

void *cubeSim(void*);
GtkBuilder* builder; 
GtkWidget       *interfaceSelectBox;
GtkListStore    *liststore;
 
int main(int argc, char *argv[])
{
    
    GtkWidget       *window;


    pthread_t cubeAnimatorThread;
    if(pthread_create(&cubeAnimatorThread, NULL, cubeAnimator, NULL)) {
        perror("creating cubeAnimator thread failed!");
        return 1;
    }

    pthread_t cubePWMcontrolThread;
    if(pthread_create(&cubePWMcontrolThread, NULL, cubePWMController, NULL)) {
        perror("creating cubePWMController thread failed!");
        return 1;
    }

    pthread_t senderThread;
    if(pthread_create(&senderThread, NULL, ethSend, NULL)) {
        perror("creating sender thread failed!");
        return 1;
    }

    gtk_init(&argc, &argv);
 
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "gui.glade", NULL);
 
    window = GTK_WIDGET(gtk_builder_get_object(builder, "mainWindow"));
    gtk_builder_connect_signals(builder, NULL);

   
    //fill interface combo box
    liststore = (GtkListStore*)gtk_builder_get_object(builder, "interfaceListStore");

    struct ifaddrs *addrs,*tmp;

    getifaddrs(&addrs);
    tmp = addrs;

    while (tmp)
    {
    	if (tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_PACKET) {
    		if (tmp->ifa_name[0] != 'l' && tmp->ifa_name[1] != '0' && tmp->ifa_name[2] != '\0'){
    			gtk_list_store_insert_with_values(liststore, NULL, -1, 0, tmp->ifa_name, -1);
    		}
    	}

    	tmp = tmp->ifa_next;
    }

    freeifaddrs(addrs);


    interfaceSelectBox = GTK_WIDGET(gtk_builder_get_object(builder, "interfaceSelectBox"));

    GtkTreeIter iter;
    gtk_tree_model_iter_nth_child ((GtkTreeModel*)liststore, &iter, NULL, 0);
    gtk_combo_box_set_active_iter((GtkComboBox*)interfaceSelectBox, &iter);

    gtk_widget_show(window);   

    srand(time(NULL)); 
   
    pthread_t cubeSimThread;
    if(pthread_create(&cubeSimThread, NULL, cubeSim, NULL)) {
        perror("creating cube sim thread failed!");
        return 1;
    }

             
    gtk_main();

    g_object_unref(builder);

 
    return 0;
}


void *cubeSim(void* void_ptr) {

    GtkWidget *cubeSim;
    cubeSim = GTK_WIDGET(gtk_builder_get_object(builder, "cubeSim"));

    while(1) {
        gtk_widget_queue_draw(cubeSim);
        usleep(20000);
    }

}

void selectBoxChanged_cb()
{
	printf("combo changed\n");


	//GtkWidget *interfaceSelectBox;
	//interfaceSelectBox = GTK_WIDGET(gtk_builder_get_object(builder, "interfaceSelectBox"));
    GtkTreeIter iter;
	gtk_combo_box_get_active_iter((GtkComboBox*)interfaceSelectBox, &iter);
	gchar* ifName;
	gtk_tree_model_get((GtkTreeModel*)liststore, &iter, 0, &ifName, -1);
	initEthernet(ifName);
}


void btnMode1_clicked_cb()
{
    printf("btn1 pressed\n");
    cubeSetAnimationMode(1);
}

void btnMode2_clicked_cb()
{
    printf("btn2 pressed\n");
    cubeSetAnimationMode(2);
}

void btnMode3_clicked_cb()
{
    printf("btn3 pressed\n");
    cubeSetAnimationMode(3);
}

void btnMode4_clicked_cb()
{
    printf("btn4 pressed\n");
    cubeSetAnimationMode(4);
}

void btnMode5_clicked_cb()
{
    printf("btn5 pressed\n");
    cubeSetAnimationMode(5);
}

void btnOff_clicked_cb()
{
    printf("btnOff pressed\n");
    cubeSetAnimationMode(0);
}
 

void main_destroy()
{
    gtk_main_quit();
}

gboolean draw_callback (GtkWidget *widget, cairo_t *cr, gpointer data)
{
  guint width, height;
  //GdkRGBA color;

  width = gtk_widget_get_allocated_width (widget);
  height = gtk_widget_get_allocated_height (widget);

  double spacing = width / (CUBE_DIM + 2);
  double ledSize = spacing * 0.09;

  cairo_set_source_rgba (cr, 0.2, 0.2, 0.2, 1);
  cairo_rectangle (cr, 0, 0, width, height);
  cairo_fill (cr);

  cairo_set_source_rgba (cr, 1, 0, 0, 1);

  for (int y = 0; y < CUBE_DIM; y++) {

    double yOffset = y * ledSize * 1.8;

    for (uint8_t x = 0; x < CUBE_DIM; x++) {
        for (uint8_t z = 0; z < CUBE_DIM; z++) {


            /*
            double r = (rand()%256) / 255.0;
           double g = (rand()%256) / 255.0;
           double b = (rand()%256) / 255.0;*/

           double r = ledCube[x][y][z].r / 255.0;
           double g = ledCube[x][y][z].g / 255.0;
           double b = ledCube[x][y][z].b / 255.0;
           
           cairo_set_source_rgba (cr, r, g, b, 1);
           cairo_arc (cr, (spacing*(x+1))+yOffset, (spacing*((CUBE_DIM-1-z)+1.8))-(yOffset*0.7), ledSize, 0, 2 * G_PI);
           cairo_fill (cr);
       }
   }
}


return FALSE;
}
