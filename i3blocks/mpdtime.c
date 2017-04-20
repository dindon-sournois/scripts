#include <mpd/status.h>
#include <mpd/connection.h>
#include <stdio.h>

int main(void)
{
    unsigned time = 0;
    struct mpd_connection *conn;
    struct mpd_status *status;
    enum mpd_state state;

    conn = mpd_connection_new(NULL, 0, 0);
    status = mpd_run_status(conn);
    if (!status) return 0;
    time = mpd_status_get_elapsed_time(status);
    state = mpd_status_get_state(status);
    mpd_status_free(status);
    mpd_connection_free(conn);

    if (state > 1)
        printf("%u\n", time);

    return 0;
}
