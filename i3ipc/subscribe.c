#include <assert.h>
#include <sys/wait.h>
#include <unistd.h>
#include <glib/gprintf.h>
#include <i3ipc-glib/i3ipc-glib.h>

i3ipcCon *focused_window;

void
on_window_event(i3ipcConnection *conn, i3ipcWindowEvent *e, gpointer data)
{
  /* TODO: don't spawn shell... */
  pid_t pid = fork();
  if (pid == 0)
    {
      execlp("i3b-send-signal", "i3b-send-signal", "20", (char*)NULL);
      assert(0);
    }
  else
    {
      waitpid(pid, NULL, 0);
    }
}


void
back_and_forth_focus(i3ipcConnection *conn, GError *err)
{
  GError *tmp = NULL;
  gchar command;
  gulong id;

  if (focused_window != NULL)
    {
      GObject *object = G_OBJECT(focused_window);
      GObjectClass *class = G_OBJECT_GET_CLASS(object);
      class->get_property(object, 1, (GValue*)&id, NULL);
      g_sprintf(&command, "[con_id=%ld] focus", id);
      g_printf("%p\n%ld\n", focused_window, id);

      i3ipcCon *root = i3ipc_connection_get_tree(conn, &err);
      if (tmp != NULL)
        {
          g_propagate_error(&err, tmp);
          return;
        }

      i3ipc_con_command(root, &command, &tmp);
      if (tmp != NULL)
        {
          g_propagate_error(&err, tmp);
          return;
        }
    }
}

void
on_focus_window_event(i3ipcConnection *conn, i3ipcWindowEvent *e, gpointer data)
{

  GError *err = (GError *)data;
  GError *tmp = NULL;

  i3ipcCon *root = i3ipc_connection_get_tree(conn, &tmp);
  if (tmp != NULL)
    {
      g_propagate_error(&err, tmp);
      return;
    }

  i3ipcCon *focused = i3ipc_con_find_focused(root);
  if (tmp != NULL)
    {
      g_propagate_error(&err, tmp);
      return;
    }

  /* back_and_forth_focus(conn, tmp); */

  focused_window = focused;
  on_window_event(conn, e, data);
}

void
on_ipc_shutdown(i3ipcConnection *conn, gpointer data)
{
  g_object_unref(conn);
}

void
subscribe_to_window_events(i3ipcConnection *conn, GError **err)
{
  GError *tmp = NULL;
  i3ipcCommandReply *reply;
  reply = i3ipc_connection_subscribe(conn, I3IPC_EVENT_WINDOW, &tmp);
  if (tmp != NULL)
    {
      g_propagate_error(err, tmp);
      return;
    }

  g_signal_connect_after(conn, "window::focus", G_CALLBACK(on_focus_window_event), (gpointer)tmp);
  g_signal_connect_after(conn, "window::new", G_CALLBACK(on_window_event), NULL);
  g_signal_connect_after(conn, "window::close", G_CALLBACK(on_window_event), NULL);
  g_signal_connect_after(conn, "window::title", G_CALLBACK(on_window_event), NULL);
  i3ipc_command_reply_free(reply);
}

void
init_connection(GError *err)
{
  GError *tmp = NULL;
  i3ipcConnection *conn;

  conn = i3ipc_connection_new(NULL, &tmp);
  if (tmp != NULL)
    {
      g_propagate_error(&err, tmp);
      g_free(conn);
      return;
    }

  g_signal_connect(conn, "ipc-shutdown", G_CALLBACK(on_ipc_shutdown), NULL);

  i3ipcCon *root = i3ipc_connection_get_tree(conn, &tmp);
  if (tmp != NULL)
    {
      g_propagate_error(&err, tmp);
      return;
    }

  i3ipcCon *focused = i3ipc_con_find_focused(root);
  if (tmp != NULL)
    {
      g_propagate_error(&err, tmp);
      return;
    }
  focused_window = focused;

  subscribe_to_window_events(conn, &tmp);
  if (tmp != NULL)
    {
      g_propagate_error(&err, tmp);
      g_free(conn);
      return;
    }
  i3ipc_connection_main(conn); /* should never return unless i3 was restarted */
}

int
main() {

  GError *err = NULL;

  while (1)
    {
      init_connection(err);
      if (err != NULL)
        {
          fprintf(stderr, "Error while trying to connect to i3wm: %s\n", err->message);
          g_error_free(err);
          err = NULL;
        }
    }

  assert(0);
  return 0; /* never returns */
}
