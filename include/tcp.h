#include <stdint.h>
#include <sys/types.h>

#include "platform.h"

#include "util.h"
//#include "net.h"
#include "ip.h"
//#include "tcp.h"


#ifndef TCP_H
#define TCP_H


#define TCP_STATE_CLOSED       1
#define TCP_STATE_LISTEN       2
#define TCP_STATE_SYN_SENT     3
#define TCP_STATE_SYN_RECEIVED 4
#define TCP_STATE_ESTABLISHED  5
#define TCP_STATE_FIN_WAIT1    6
#define TCP_STATE_FIN_WAIT2    7
#define TCP_STATE_CLOSING      8
#define TCP_STATE_TIME_WAIT    9
#define TCP_STATE_CLOSE_WAIT  10
#define TCP_STATE_LAST_ACK    11

struct pseudo_hdr {
    uint32_t src;
    uint32_t dst;
    uint8_t zero;
    uint8_t protocol;
    uint16_t len;
};

struct tcp_hdr {
    uint16_t src;
    uint16_t dst;
    uint32_t seq;
    uint32_t ack;
    uint8_t off;
    uint8_t flg;
    uint16_t wnd;
    uint16_t sum;
    uint16_t up;
};

struct tcp_segment_info {
    uint32_t seq;
    uint32_t ack;
    uint16_t len;
    uint16_t wnd;
    uint16_t up;
};

struct tcp_pcb {
    int state;
    int mode; /* user command mode */
    struct ip_endpoint local;
    struct ip_endpoint foreign;
    struct {
        uint32_t nxt;
        uint32_t una;
        uint16_t wnd;
        uint16_t up;
        uint32_t wl1;
        uint32_t wl2;
    } snd;
    uint32_t iss;
    struct {
        uint32_t nxt;
        uint16_t wnd;
        uint16_t up;
    } rcv;
    uint32_t irs;
    uint16_t mtu;
    uint16_t mss;
    uint8_t buf[65535]; /* receive buffer */
    struct sched_ctx ctx;
    struct queue_head queue; /* retransmit queue */
    struct timeval tw_timer;
    struct tcp_pcb *parent;
    struct queue_head backlog;
};

struct tcp_queue_entry {
    struct timeval first;
    struct timeval last;
    unsigned int rto; /* micro seconds */
    uint32_t seq;
    uint8_t flg;
    size_t len;
};


struct tcp_pcb * tcp_pcb_alloc(void);
struct tcp_pcb *tcp_pcb_get(int id);
int tcp_pcb_id(struct tcp_pcb *pcb);
int tcp_retransmit_queue_add(struct tcp_pcb *pcb, uint32_t seq, uint8_t flg, uint8_t *data, size_t len);
extern int
tcp_init(void);

extern int
tcp_open_rfc793(struct ip_endpoint *local, struct ip_endpoint *foreign, int active);
extern int
tcp_state(int id);
extern int
tcp_close(int id);
extern ssize_t
tcp_send(int id, uint8_t *data, size_t len);
extern ssize_t
tcp_receive(int id, uint8_t *buf, size_t size);

extern int
tcp_open(void);
extern int
tcp_bind(int id, struct ip_endpoint *local);
extern int
tcp_connect(int id, struct ip_endpoint *foreign);
extern int
tcp_listen(int id, int backlog);
extern int
tcp_accept(int id, struct ip_endpoint *foreign);

#endif
