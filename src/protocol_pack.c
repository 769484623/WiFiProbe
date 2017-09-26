#include"protocol_pack.h"
u_char mac_addr[6];
int get_mac()
{
    int sockfd;
    struct ifreq tmp;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if( sockfd < 0)
    {
        perror("create socket fail\n");
        return -1;
    }
    memset(&tmp,0,sizeof(struct ifreq));
    strncpy(tmp.ifr_name,"eth0",sizeof(tmp.ifr_name)-1);
    if( (ioctl(sockfd,SIOCGIFHWADDR,&tmp)) < 0 )
    {
        printf("mac ioctl error\n");
        return -1;
    }
    mac_addr[0] = (unsigned char)tmp.ifr_hwaddr.sa_data[0];
    mac_addr[1] = (unsigned char)tmp.ifr_hwaddr.sa_data[1];
    mac_addr[2] = (unsigned char)tmp.ifr_hwaddr.sa_data[2];
    mac_addr[3] = (unsigned char)tmp.ifr_hwaddr.sa_data[3];
    mac_addr[4] = (unsigned char)tmp.ifr_hwaddr.sa_data[4];
    mac_addr[5] = (unsigned char)tmp.ifr_hwaddr.sa_data[5];
    //printf("local mac:%x:%x:%x:%x:%x:%x\n", mac_addr[0],mac_addr[1],mac_addr[2],mac_addr[3],mac_addr[4],mac_addr[5]);
    close(sockfd);

    return 0;
}
