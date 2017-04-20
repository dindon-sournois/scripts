#include <mpd/client.h>
#include <stdio.h>

int main(void)
{
    struct mpd_connection *conn;
    struct mpd_song *song;

    conn = mpd_connection_new(0, 0, 0);
    if (!mpd_send_command(conn, "currentsong")) return 1;
    song = mpd_recv_song(conn);
    if (!song) return 1;

    printf("%s\n", mpd_song_get_uri(song));

    mpd_connection_free(conn);
    return 0;
}
