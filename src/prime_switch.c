#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <unistd.h>

int getVideoMode();
void setMode(int mode);
bool Contains(char MyChar[], char Search[]);
int checkRoot();

char nvidia[42]="/usr/share/X11/xorg.conf.d/10-nvidia.conf\0";
char modulesload[36]="/etc/modules-load.d/nvidia-drm.conf\0";
char intel[45]="/usr/share/X11/xorg.conf.d/10-intel.conf\0";
char modprobe[37]="/etc/modprobe.d/nvidia-modprobe.conf\0";
char intelBusID[8]="\0";
char nvidiaBusID[8]="\0";

GtkWidget       *window;


int main(int argc, char *argv[]) {
    int opt;
    int mode;
    while ((opt = getopt(argc, argv, "nims")) != -1) {
        switch (opt) {
            case 'n':
		printf("Setting Prime To Nvidia\n");
		if (checkRoot() != 1) {
			printf("You must be root to switch the mode\n");
			exit(0);
		}
		setMode(1);
                break;
            case 'i':
                printf("Setting Prime to Intel\n");
		if (checkRoot() != 1) {
			printf("You must be root to switch the mode\n");
			exit(0);
		}
		setMode(2);
                break;
            case 'm':
		mode = getVideoMode();
		char cardname[8]="\0";
		if (mode == 1) {
			strcpy(cardname,"Nvidia\0");
		}
		if (mode == 2) {
			strcpy(cardname,"Intel\0");
		}
		if (mode == 0) {
			strcpy(cardname,"Unknown\0");
		}
                printf("Mode: %s\n",cardname);
                break;
            case 's':
                if (getVideoMode() == 1) {
			printf("Using xrandr to setup modesetting across all monitors\n");
			system("xrandr --setprovideroutputsource modesetting NVIDIA-0");
			system("sleep 1");
			system("xrandr --output eDP-1-1 --auto");
		}
                break;
            default:
                fprintf(stderr, "Usage: %s [-nim]\n", argv[0]);
                fprintf(stderr, "-n..................Switch to Nvidia\n");
                fprintf(stderr, "-i..................Switch to Intel\n");
                fprintf(stderr, "-m..................Get The Current Mode\n");
                fprintf(stderr, "-s..................Setup the screen xrandr required\n");
		exit(0);
        }
    }

    if (argc >= 2) {
        exit(0);
    }

    mode = getVideoMode();

    GtkBuilder      *builder;
    GtkWidget       *modelabel;
    GtkWidget       *buttonlabel;
    gtk_init(&argc, &argv);
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "/etc/prime_switch/prime_switch.glade", NULL);
    gtk_builder_connect_signals(builder, NULL);
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    modelabel = GTK_WIDGET(gtk_builder_get_object(builder, "mode_label"));
    buttonlabel = GTK_WIDGET(gtk_builder_get_object(builder, "switch_button"));
    if (mode == 1) {
        char modelbl[35] = "\nThe Current Mode is Nvidia\n";
        gtk_label_set_text(GTK_LABEL(modelabel), modelbl);
        gtk_button_set_label(GTK_BUTTON(buttonlabel), "Change To Intel");
    } else if (mode == 2) {
        char modelbl[35] = "\nThe Current Mode is Intel\n";
        gtk_label_set_text(GTK_LABEL(modelabel), modelbl);
        gtk_button_set_label(GTK_BUTTON(buttonlabel), "Change To Nvidia");
    } else {
        char modelbl[35] = "\nThe Current Mode is Unknown\n";
        gtk_label_set_text(GTK_LABEL(modelabel), modelbl);
        gtk_button_set_label(GTK_BUTTON(buttonlabel), "Change To Intel");
    }
    //gtk_builder_connect_signals(builder, NULL);
    g_object_unref(builder);
    gtk_widget_show(window);

    gtk_main();



    return 0;
}

void on_change_mode_clicked() {
    if (checkRoot() != 1) {
        GtkWidget* error = gtk_message_dialog_new(GTK_WINDOW(window), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,"You Must Be Root");
        gtk_dialog_run(GTK_DIALOG(error));
        gtk_widget_destroy(error);
	return;
    }
    int mode = getVideoMode();
    if (mode == 1) {
        setMode(2);
    }
    if (mode == 2) {
        setMode(1);
    }
    GtkWidget* errorDone = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,"Done Switching you just need to logout and log back in\nI figure you can handle that");
    gtk_dialog_run(GTK_DIALOG(errorDone));
    gtk_widget_destroy(errorDone);
    gtk_main_quit();
    exit(0);
}

int checkRoot() {
	if(geteuid() != 0) {
		return 0;
	} else {
		return 1;
	}
}

void setMode(int mode) {
    FILE *fp;
    char path[1035];

    fp = popen("lspci | grep VGA", "r");
    if (fp == NULL) {
        printf("Failed to run command\n" );
        exit(1);
    }

    while (fgets(path, sizeof(path)-1, fp) != NULL) {
	if(Contains(path, "NVIDIA") == true) {
		memcpy( nvidiaBusID, &path[0], 7 );
		nvidiaBusID[8] = '\0';
	}
	if(Contains(path, "Intel") == true) {
		memcpy( intelBusID, &path[0], 7 );
		intelBusID[8] = '\0';
	}
    }
    /* close */
    pclose(fp);

    if (nvidiaBusID[0] == 0) {
        printf("No Nvidia Card Found\n");
    } else {
        nvidiaBusID[5]=':';
	printf("Found NVIDIA card at: %s\n",nvidiaBusID);
    }
    if (intelBusID[0] == 0) {
        printf("No Intel Card Found\n");
    } else {
        intelBusID[5]=':';
	printf("Found Intel card at: %s\n",intelBusID);
    }

    if (mode == 1) {
	remove(intel);
        fp = fopen(nvidia, "w");

        fprintf(fp, "Section \"ServerLayout\"\n");
        fprintf(fp, "\tIdentifier \"layout\"\n");
        fprintf(fp, "\tScreen 0 \"nvidia\"\n");
        fprintf(fp, "\tInactive \"intel\"\n");
        fprintf(fp, "EndSection\n\n");

	fprintf(fp,"Section \"Files\"\n");
	fprintf(fp,"\tModulePath     \"/usr/lib/nvidia\"\n");
	fprintf(fp,"\tModulePath     \"/usr/lib/xorg/modules/linux\"\n");
	fprintf(fp,"\tModulePath     \"/usr/lib/xorg/modules\"\n");
	fprintf(fp,"EndSection\n\n");

	fprintf(fp,"Section \"Device\"\n");
	fprintf(fp,"\tIdentifier \"nvidia\"\n");
	fprintf(fp,"\tDriver \"nvidia\"\n");
	fprintf(fp,"\tBusID \"%s\"\n",nvidiaBusID);
	fprintf(fp,"\tOption \"AllowEmptyInitialConfiguration\"\n");
	fprintf(fp,"EndSection\n\n");

	fprintf(fp,"Section \"Screen\"\n");
	fprintf(fp,"\tIdentifier \"nvidia\"\n");
	fprintf(fp,"\tDevice \"nvidia\"\n");
	fprintf(fp,"EndSection\n\n");

	fprintf(fp,"Section \"Device\"\n");
	fprintf(fp,"\tIdentifier  \"intel\"\n");
	fprintf(fp,"\tDriver      \"modesetting\"\n");
	fprintf(fp,"EndSection\n\n");

	fprintf(fp,"Section \"Screen\"\n");
	fprintf(fp,"\tIdentifier \"intel\"\n");
	fprintf(fp,"\tDevice \"intel\"\n");
	fprintf(fp,"EndSection\n\n");
        fclose(fp);

        fp = fopen(modprobe, "w");
	fprintf(fp, "install nvidia modprobe -i nvidia-current $CMDLINE_OPTS\n");
	fprintf(fp, "install nvidia-modeset modprobe nvidia ; modprobe -i nvidia-current-modeset $CMDLINE_OPTS\n");
	fprintf(fp, "install nvidia-drm modprobe nvidia-modeset ; modprobe -i nvidia-current-drm modesetting=1 modeset=1 $CMDLINE_OPTS\n");
	fprintf(fp, "install nvidia-uvm modprobe nvidia ; modprobe -i nvidia-current-uvm $CMDLINE_OPTS\n");
	fprintf(fp, "remove nvidia modprobe -r -i nvidia-drm nvidia-modeset nvidia-uvm nvidia\n");
	fprintf(fp, "remove nvidia-modeset modprobe -r -i nvidia-drm nvidia-modeset\n");
	fprintf(fp, "alias	pci:v000010DEd00000E00sv*sd*bc04sc80i00*	nvidia\n");
	fprintf(fp, "alias	pci:v000010DEd00000AA3sv*sd*bc0Bsc40i00*	nvidia\n");
	fprintf(fp, "alias	pci:v000010DEd*sv*sd*bc03sc02i00*		nvidia\n");
	fprintf(fp, "alias	pci:v000010DEd*sv*sd*bc03sc00i00*		nvidia\n");
	fclose(fp);

        fp = fopen(modulesload, "w");
	fprintf(fp, "nvidia-drm");
	fclose(fp);
	system("modprobe nvidia-drm");
	printf("All Done! Just need to log out and back in again, I think you can handle that!\n");
    }

    if (mode == 2) {
        remove(modprobe);
        remove(nvidia);
        remove(modulesload);
	fp = fopen(intel, "w");
	fprintf(fp, "Section \"OutputClass\"\n");
	fprintf(fp, "\tIdentifier \"intel\"\n");
	fprintf(fp, "\tModulePath \"/usr/lib/xorg/modules/extensions\"\n");
	fprintf(fp, "EndSection\n\n");

	fprintf(fp, "Section \"Device\"\n");
	fprintf(fp, "\tIdentifier \"intel\"\n");
	fprintf(fp, "\tDriver \"intel\"\n");
	fprintf(fp, "\tBusID \"PCI:%s\"\n",intelBusID);
	fprintf(fp, "\tOption \"DRI\" \"true\"\n");
	fprintf(fp, "\tOption \"TearFree\" \"true\"\n");
	fprintf(fp, "EndSection\n\n");

	fprintf(fp, "Section \"DRI\"\n");
	fprintf(fp, "\tGroup \"video\"\n");
	fprintf(fp, "\tMode 0666\n");
	fprintf(fp, "EndSection\n");

	fclose(fp);
	printf("All Done! Just need to log out and back in again, I think you can handle that!\n");

    }
}


int getVideoMode() {

    if( access( nvidia, F_OK ) != -1 ) {
        return 1;
    }
    if( access( intel, F_OK ) != -1 ) {
        return 2;
    }
    return 0;

}
// called when window is closed
void on_window_main_destroy() {
    gtk_main_quit();
}


bool Contains(char MyChar[], char Search[]) {
    int LoopOne = 0;
    int LoopTwo;
    int LoopThree;
    int MyCharSize = strlen(MyChar);
    int SearchSize = strlen(Search);
    int Yes = 0;

    while(LoopOne < MyCharSize) {
        if(MyChar[LoopOne] == Search[0]) {
            LoopTwo = 0;
            LoopThree = LoopOne;
            while(LoopTwo < SearchSize) {
                if(MyChar[LoopThree] == Search[LoopTwo]) {
                    Yes++;
		}
                LoopTwo++;
                LoopThree++;
            }
            if(Yes == SearchSize) {
                return true;
            }
        }
        LoopOne++;
    }
    return false;
}
