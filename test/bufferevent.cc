#include <event2/event.h>

#include <event2/bufferevent.h>

#include <event2/buffer.h>

#include <event2/util.h>



#include <stdlib.h>

#include <errno.h>

#include <string.h>



struct info {

    const char *name;

    size_t total_drained;

};



void read_callback(struct bufferevent *bev, void *ctx)

{

    struct info *inf = ctx;

    struct evbuffer *input = bufferevent_get_input(bev);

    size_t len = evbuffer_get_length(input);

    if (len) {

        inf->total_drained += len;

        evbuffer_drain(input, len);

        printf("Drained %lu bytes from %s/n",

             (unsigned long) len, inf->name);

    }

}



void event_callback(struct bufferevent *bev, short events, void *ctx)

{

    struct info *inf = ctx;

    struct evbuffer *input = bufferevent_get_input(bev);

    int finished = 0;



    if (events & BEV_EVENT_EOF) {

        size_t len = evbuffer_get_length(input);

        printf("Got a close from %s.  We drained %lu bytes from it, "

            "and have %lu left./n", inf->name,

            (unsigned long)inf->total_drained, (unsigned long)len);

        finished = 1;

    }

    if (events & BEV_EVENT_ERROR) {

        printf("Got an error from %s: %s/n",

            inf->name, evutil_socket_error_to_string(EVUTIL_SOCKET_ERROR()));

        finished = 1;

    }

    if (finished) {

        free(ctx);

        bufferevent_free(bev);

    }

}



struct bufferevent *setup_bufferevent(void)

{

    struct bufferevent *b1 = NULL;

    struct info *info1;



    info1 = malloc(sizeof(struct info));

    info1->name = "buffer 1";

    info1->total_drained = 0;



    /* ... Here we should set up the bufferevent and make sure it gets

       connected... */



    /* Trigger the read callback only whenever there is at least 128 bytes

       of data in the buffer. */

    bufferevent_setwatermark(b1, EV_READ, 128, 0);



    bufferevent_setcb(b1, read_callback, NULL, event_callback, info1);



    bufferevent_enable(b1, EV_READ); /* Start reading. */

    return b1;

}




#include <event2/bufferevent.h>

#include <event2/buffer.h>



#include <ctype.h>



void

read_callback_uppercase(struct bufferevent *bev, void *ctx)

{

        /* This callback removes the data from bev's input buffer 128

           bytes at a time, uppercases it, and starts sending it

           back.



           (Watch out!  In practice, you shouldn't use toupper to implement

           a network protocol, unless you know for a fact that the current

           locale is the one you want to be using.)

         */



        char tmp[128];

        size_t n;

        int i;

        while (1) {

                n = bufferevent_read(bev, tmp, sizeof(tmp));

                if (n <= 0)

                        break; /* No more data. */

                for (i=0; i<n; ++i)

                        tmp[i] = toupper(tmp[i]);

                bufferevent_write(bev, tmp, n);

        }

}



struct proxy_info {

        struct bufferevent *other_bev;

};

void

read_callback_proxy(struct bufferevent *bev, void *ctx)

{

        /* You might use a function like this if you're implementing

           a simple proxy: it will take data from one connection (on

           bev), and write it to another, copying as little as

           possible. */

        struct proxy_info *inf = ctx;



        bufferevent_read_buffer(bev,

            bufferevent_get_output(inf->other_bev));

}



struct count {

        unsigned long last_fib[2];

};



void

write_callback_fibonacci(struct bufferevent *bev, void *ctx)

{

        /* Here's a callback that adds some Fibonacci numbers to the

           output buffer of bev.  It stops once we have added 1k of

           data; once this data is drained, we'll add more. */

        struct count *c = ctx;



        struct evbuffer *tmp = evbuffer_new();

        while (evbuffer_get_length(tmp) < 1024) {

                 unsigned long next = c->last_fib[0] + c->last_fib[1];

                 c->last_fib[0] = c->last_fib[1];

                 c->last_fib[1] = next;



                 evbuffer_add_printf(tmp, "%lu", next);

        }



        /* Now we add the whole contents of tmp to bev. */

        bufferevent_write_buffer(bev, tmp);



        /* We don't need tmp any longer. */

        evbuffer_free(tmp);

}
