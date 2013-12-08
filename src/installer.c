/* $Id$ */
/* Copyright (c) 2013 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS Desktop Installer */
/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. */


#include <stdio.h>
#include <string.h>
#include <libintl.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <System.h>
#include <Desktop.h>
#include "installer.h"
#include "../config.h"
#define _(string) gettext(string)
#define N_(string) (string)


/* Installer */
/* private */
/* types */
struct _Installer
{
	char * profile;

	Config * config;

	/* widgets */
	GtkWidget * window;
	/* preferences */
	GtkWidget * pr_window;
	/* find */
	GtkWidget * fi_dialog;
	GtkListStore * fi_store;
	GtkWidget * fi_text;
	/* about */
	GtkWidget * ab_window;
};


/* prototypes */
/* useful */
/* callbacks */
static void _installer_on_about(gpointer data);
static void _installer_on_cancel(gpointer data);
static void _installer_on_close(gpointer data);
static gboolean _installer_on_closex(gpointer data);
static void _installer_on_prepare(GtkWidget * widget, GtkWidget * page,
		gpointer data);


/* public */
/* functions */
/* essential */
/* installer_new */
static void _new_page_first(Installer * installer, GdkPixbuf * pixbuf);
static void _new_page_last(Installer * installer, GdkPixbuf * pixbuf);
static void _new_set_title(Installer * installer);

Installer * installer_new(char const * profile)
{
	Installer * installer;
	GtkIconTheme * icontheme;
	GtkAccelGroup * group;
	GtkWidget * widget;
	char const * icon;
#if GTK_CHECK_VERSION(2, 14, 0)
	const int flags = GTK_ICON_LOOKUP_FORCE_SIZE;
#else
	const int flags = 0;
#endif
	GdkPixbuf * pixbuf = NULL;

	if((installer = object_new(sizeof(*installer))) == NULL)
		return NULL;
	installer->profile = strdup(profile);
	installer->config = config_new();
	/* check for errors */
	if(installer->profile == NULL
			|| installer->config == NULL
			/* FIXME no longer expect an absolute path */
			|| config_load(installer->config, profile) != 0)
	{
		object_delete(installer);
		return NULL;
	}
	/* widgets */
	icontheme = gtk_icon_theme_get_default();
	group = gtk_accel_group_new();
	/* window */
	installer->window = gtk_assistant_new();
	gtk_window_add_accel_group(GTK_WINDOW(installer->window), group);
	g_object_unref(group);
	gtk_window_set_default_size(GTK_WINDOW(installer->window), 400, 300);
	g_signal_connect_swapped(installer->window, "delete-event", G_CALLBACK(
				_installer_on_closex), installer);
	g_signal_connect_swapped(installer->window, "cancel", G_CALLBACK(
				_installer_on_cancel), installer);
	g_signal_connect_swapped(installer->window, "close", G_CALLBACK(
				_installer_on_close), installer);
	g_signal_connect(installer->window, "prepare", G_CALLBACK(
				_installer_on_prepare), installer);
	_new_set_title(installer);
#if GTK_CHECK_VERSION(2, 6, 0)
	gtk_window_set_icon_name(GTK_WINDOW(installer->window),
			"system-software-install");
#endif
	widget = gtk_button_new_from_stock(GTK_STOCK_ABOUT);
	g_signal_connect_swapped(widget, "clicked", G_CALLBACK(
				_installer_on_about), installer);
	gtk_widget_show(widget);
	gtk_assistant_add_action_widget(GTK_ASSISTANT(installer->window),
			widget);
	/* load the desired icon */
	if((icon = config_get(installer->config, NULL, "icon")) != NULL)
		pixbuf = gtk_icon_theme_load_icon(icontheme, icon, 48, flags,
				NULL);
	/* first page */
	_new_page_first(installer, pixbuf);
	/* last page */
	_new_page_last(installer, pixbuf);
	if(pixbuf != NULL)
		g_object_unref(pixbuf);
	gtk_widget_show_all(installer->window);
	/* about */
	installer->ab_window = NULL;
	return installer;
}

static void _new_page_first(Installer * installer, GdkPixbuf * pixbuf)
{
	GtkAssistant * assistant = GTK_ASSISTANT(installer->window);
	GtkWidget * widget;
	char const * package;
	char const * title;
	char const * description;
	char buf[256];

	package = config_get(installer->config, NULL, "package");
	title = config_get(installer->config, NULL, "title");
	description = config_get(installer->config, NULL, "description");
	if(description != NULL)
		widget = gtk_label_new(description);
	else
	{
		snprintf(buf, sizeof(buf), "%s%s", (package != NULL)
				? _("Welcome to the installer for ")
				: _("Welcome to the installer"),
				(package != NULL) ? package : "");
		widget = gtk_label_new(buf);
	}
	gtk_assistant_insert_page(assistant, widget, 0);
	gtk_assistant_set_page_complete(assistant, widget, TRUE);
	if(pixbuf != NULL)
		gtk_assistant_set_page_header_image(assistant, widget, pixbuf);
	if(title != NULL)
		gtk_assistant_set_page_title(assistant, widget, title);
	else
	{
		if(package != NULL)
			snprintf(buf, sizeof(buf), _("%s installer"), package);
		gtk_assistant_set_page_title(assistant, widget, buf);
	}
	gtk_assistant_set_page_type(assistant, widget,
			GTK_ASSISTANT_PAGE_INTRO);
}

static void _new_page_last(Installer * installer, GdkPixbuf * pixbuf)
{
	GtkWidget * widget;

	widget = gtk_label_new(_("Congratulations!\n"
				"The installation has completed."));
	gtk_assistant_append_page(GTK_ASSISTANT(installer->window), widget);
	gtk_assistant_set_page_complete(GTK_ASSISTANT(installer->window),
			widget, TRUE);
	if(pixbuf != NULL)
		gtk_assistant_set_page_header_image(GTK_ASSISTANT(
					installer->window), widget, pixbuf);
	gtk_assistant_set_page_title(GTK_ASSISTANT(installer->window), widget,
			_("Installation complete"));
	gtk_assistant_set_page_type(GTK_ASSISTANT(installer->window), widget,
			GTK_ASSISTANT_PAGE_SUMMARY);
}

static void _new_set_title(Installer * installer)
{
	char buf[256];

	/* FIXME replace with a configuration value instead */
	snprintf(buf, sizeof(buf), "%s%s", _("Installer - "),
			installer->profile);
	gtk_window_set_title(GTK_WINDOW(installer->window), buf);
}


/* installer_delete */
void installer_delete(Installer * installer)
{
	object_delete(installer);
}


/* useful */
/* installer_about */
/* callbacks */
static gboolean _about_on_closex(gpointer data);

void installer_about(Installer * installer)
{
	char const * authors[2] = { NULL, NULL };
	char const * comments;
	char const * copyright;
	char const * icon = "system-software-install";
	char const * license;
	char const * package = PACKAGE;
	char const * version = VERSION;
	char const * website;
	char const * p;

	if(installer->ab_window != NULL)
	{
		gtk_widget_show(installer->ab_window);
		return;
	}
	/* load the configuration */
	authors[0] = config_get(installer->config, NULL, "authors");
	comments = config_get(installer->config, NULL, "comments");
	copyright = config_get(installer->config, NULL, "copyright");
	if((p = config_get(installer->config, NULL, "icon")) != NULL)
		icon = p;
	license = config_get(installer->config, NULL, "license");
	if((p = config_get(installer->config, NULL, "package")) != NULL)
		package = p;
	if((p = config_get(installer->config, NULL, "version")) != NULL)
		version = p;
	website = config_get(installer->config, NULL, "website");
	/* create the window */
	installer->ab_window = desktop_about_dialog_new();
	gtk_window_set_transient_for(GTK_WINDOW(installer->ab_window),
			GTK_WINDOW(installer->window));
	if(authors[0] != NULL)
		desktop_about_dialog_set_authors(installer->ab_window,
				authors);
	if(comments != NULL)
		desktop_about_dialog_set_comments(installer->ab_window,
				comments);
	if(copyright != NULL)
		desktop_about_dialog_set_copyright(installer->ab_window,
				copyright);
	desktop_about_dialog_set_logo_icon_name(installer->ab_window, icon);
	if(license != NULL)
		desktop_about_dialog_set_license(installer->ab_window, license);
	desktop_about_dialog_set_name(installer->ab_window, package);
	desktop_about_dialog_set_translator_credits(installer->ab_window,
			_("translator-credits"));
	desktop_about_dialog_set_version(installer->ab_window, version);
	if(website != NULL)
		desktop_about_dialog_set_website(installer->ab_window, website);
	g_signal_connect_swapped(G_OBJECT(installer->ab_window), "delete-event",
			G_CALLBACK(_about_on_closex), installer);
	gtk_widget_show(installer->ab_window);
}

static gboolean _about_on_closex(gpointer data)
{
	Installer * installer = data;

	gtk_widget_hide(installer->ab_window);
	return TRUE;
}


/* installer_close */
int installer_close(Installer * installer)
{
	GtkAssistant * assistant = GTK_ASSISTANT(installer->window);
	gint n;
	GtkWidget * page;
	GtkWidget * dialog;
	int res;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	n = gtk_assistant_get_current_page(assistant);
	page = gtk_assistant_get_nth_page(assistant, n);
	/* check for unsaved changes */
	if(gtk_assistant_get_n_pages(assistant) == (n + 1)
			&& page != NULL
			&& gtk_assistant_get_page_complete(assistant, page))
	{
		/* we are on the last page and it is complete */
		gtk_widget_hide(installer->window);
		gtk_main_quit();
		return 0;
	}
	dialog = gtk_message_dialog_new(GTK_WINDOW(installer->window),
			GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
			GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO,
#if GTK_CHECK_VERSION(2, 6, 0)
			"%s", _("Question"));
	gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog),
#endif
			"%s", _("The install operation is not complete.\n"
				"Do you really want to cancel it?"));
	gtk_window_set_title(GTK_WINDOW(dialog), _("Question"));
	res = gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
	if(res == GTK_RESPONSE_YES)
	{
		gtk_widget_hide(installer->window);
		gtk_main_quit();
		return 0;
	}
	return 1;
}


/* installer_error */
int installer_error(Installer * installer, char const * message, int ret)
{
	GtkWidget * dialog;

	dialog = gtk_message_dialog_new(GTK_WINDOW(installer->window),
			GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR,
			GTK_BUTTONS_CLOSE,
#if GTK_CHECK_VERSION(2, 6, 0)
			"%s", _("Error"));
	gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog),
#endif
			"%s", message);
	gtk_window_set_title(GTK_WINDOW(dialog), _("Error"));
	g_signal_connect(G_OBJECT(dialog), "response", G_CALLBACK(
				gtk_widget_destroy), NULL);
	gtk_widget_show(dialog);
	return ret;
}


/* private */
/* useful */
/* callbacks */
/* installer_on_about */
static void _installer_on_about(gpointer data)
{
	Installer * installer = data;

	installer_about(installer);
}


/* installer_on_cancel */
static void _installer_on_cancel(gpointer data)
{
	Installer * installer = data;

	installer_close(installer);
}


/* installer_on_close */
static void _installer_on_close(gpointer data)
{
	Installer * installer = data;

	installer_close(installer);
}


/* installer_on_closex */
static gboolean _installer_on_closex(gpointer data)
{
	Installer * installer = data;

	installer_close(installer);
	return TRUE;
}


/* installer_on_prepare */
static void _installer_on_prepare(GtkWidget * widget, GtkWidget * page,
		gpointer data)
{
	Installer * installer = data;

	/* FIXME implement */
}
