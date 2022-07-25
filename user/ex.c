#include<arp.h>
#include<ether.h>
#include<icmp.h>
#include<ip.h>
#include<net.h>
#include<sock.h>
#include<tcp.h>
#include<udp.h>
#include<util.h>

#include<stdlib.h>
#include<string.h>

struct test_s {
	uint8_t s1,s2,s3,s4;
};
uint8_t convert(int data) {
	uint8_t *conv =(uint8_t*)"convert" + data;
	return *conv;	
}
struct test_s *test_s_get(int size) {
	int *ch = malloc(sizeof(uint8_t)*size);
	for(int i=1;i< (size);i++) {
		ch[i] = convert(i);
	}
	struct test_s *ts =  (struct test_s*)ch;
	return ts;
}
char  *get_strc(void) {
	struct test_s *ts;
	char *strc = malloc(sizeof(char)*(sizeof(struct test_s)+4+1));
	char *intc = malloc(100);
	struct tcp_pcb *pcb = NULL;//malloc(sizeof(struct tcp_pcb)*sizeof(struct tcp_hdr));
	uint8_t data =2; 
	pcb = tcp_pcb_alloc(); // previous: tcp_pcb_get :: returns NULL
	struct tcp_pcb *pcbb = tcp_pcb_get(tcp_pcb_id(pcb));
	int queue_add_return = tcp_retransmit_queue_add(pcbb, 0,0,&data, 1);
	queue_add_return = tcp_state(tcp_open());	
	queue_add_return += tcp_state(queue_add_return);	
	ts = test_s_get(4);
	for(int i=1;i<5;i++) strcat(strc, (char*)(ts+i));
	sprintf(intc, "%d", queue_add_return);
	strcat(strc, intc);
	free(ts);
	return strc;
}
struct tcp_hdr *check(void) {
	struct tcp_hdr *hdr = malloc(sizeof(struct tcp_pcb)*sizeof(struct tcp_hdr));
	struct tcp_pcb *pcb = tcp_pcb_alloc();
       	pcb = tcp_pcb_get(tcp_pcb_id(pcb));
	struct tcp_hdr *tcphdr = (struct tcp_hdr*)pcb;
	for(int i=0;i<4;i++) hdr[i] = *(struct tcp_hdr*)(pcb+i);
	return tcphdr;
}

int main(void) {
	//
	//struct tcp_hdr *hdr = check();
	//printf("%d", crete_tcp_hdr(hdr));
	//printf("%lu <-> %lu\n", sizeof(struct tcp_pcb), sizeof(struct tcp_hdr));
	//int seq = *(int*)&((&hdr)[0]);
	return 0;
}


