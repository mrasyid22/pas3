/*
// Tambahkan di sini nama para kontributor (mrasyid22, enbeqi, kamilakaffah07, ramyazhar, nazilarahma, nethaniasonya, tesajps)
// Harap UPDATE tanggal revisi!
// Tue Jun  30 17:46:47 WIB 2020
 */

#include     "share.h"
char*        progs[]={P01, P02, P03, P04, P05, P06};
char         tmpStr[256]={};
extern char* akunGitHub;
extern int   delay;
extern int   boss;
myshare*     mymap;

// Tambahkan di sini nama para kontributor (mrasyid22, enbeqi, kamilakaffah07, ramyazhar, nazilarahma, nethaniasonya, tesajps)
// Harap UPDATE tanggal revisi!
// Tue Jun  30 17:50:47 WIB 2020
void init(int isboss, int argc, char* argv[]) {
    if (isboss == BOSS) {
        int ssize=sizeof(myshare);
        int fd   =open(SHAREMEM, MYFLAGS, CHMOD);
        fchmod   (fd, CHMOD);
        ftruncate(fd, ssize);
        mymap=mmap(NULL, ssize, MYPROTECTION, MYVISIBILITY, fd, 0);
        close(fd);

        mymap->state = OPEN;

        sem_init (&(mymap->mutex), 1, MUTEX); // lihat manual sem_init()!

        if (argc > 1)
        {
            mymap->state = OPEN;
            printf("ShareMemory is OPEN, BYE BYE ==== ====\n");
            exit(0);
        }

    } else {
        sleep(delay);
        if( access(SHAREMEM, F_OK ) == -1 ) {
            printf("No \"%s\" file.\n", SHAREMEM);
            exit(-1);
        }
        int fd=open(SHAREMEM, O_RDWR, CHMOD);
        int ssize=sizeof(myshare);
        mymap=mmap(NULL, ssize, MYPROTECTION, MYVISIBILITY, fd, 0);
        close(fd);
    }
}

// Tambahkan di sini nama para kontributor (akunGitHub)
// Harap UPDATE tanggal revisi!
// Tue Jun  9 17:46:47 WIB 2020
void myPrint(char* str1, char* str2) {
    printf("%s[%s]\n", str1, str2);
    fflush(NULL);
}

// Tambahkan di sini nama para kontributor (mrasyid22, enbeqi, kamilakaffah07, ramyazhar, nazilarahma, nethaniasonya, tesajps)
// Harap UPDATE tanggal revisi!
// Tue Jun  29 23:55:47 WIB 2020
int getEntry(char* akunGitHub) {
    int entry=-1;
    sem_wait (&(mymap->mutex));

    mymap->mutexctr++;
    int entryCounter;
    entryCounter = mymap->entry;
    usrinfo listOfUser[MAXPROGS+1];
    memcpy(listOfUser, mymap->progs, sizeof(listOfUser));

    for(int j=0; j< entryCounter; j++){
      if(listOfUser[j].akun[0] != '\0'){
        mymap->progs[j].stamp++;
        if(strcmp(listOfUser[j].akun, akunGitHub) == 0){
          entry =j;
          break;
        }
      }
    }

    if(entry == -1){
      entry = mymap->entry++;
      mymap->progs[entry].stamp++;
    }
  
    sem_post (&(mymap->mutex));
    return entry;
}

// Tambahkan di sini nama para kontributor (mrasyid22, enbeqi, kamilakaffah07, ramyazhar, nazilarahma, nethaniasonya, tesajps)
// Harap UPDATE tanggal revisi!
// Tue Jun  29 23:56:47 WIB 2020
void display(int entry) {
    sem_wait (&(mymap->mutex));
    memset(tmpStr,0,255);
    mymap->mutexctr++;
    mymap->progs[entry].stamp++;

    printf("%s[progs[%.2d] TIME[%.2d] MUTEX[%.2d] MMAP[%s] ", akunGitHub, entry, mymap->mutexctr, mymap->progs[entry].stamp, "OPEN");

    for (int i = 0; i < mymap->entry; i++)
    {
       if(mymap->progs[i].akun == NULL){
            printf("[]");
        } else {
          printf("[%s]", mymap->progs[i].akun);
        }
    }

    printf("]\n");

    sem_post (&(mymap->mutex));
}

// Tambahkan di sini nama para kontributor (mrasyid22, enbeqi, kamilakaffah07, ramyazhar, nazilarahma, nethaniasonya, tesajps)
// Harap UPDATE tanggal revisi!
// Tue Jun  29 23:57:47 WIB 2020
void putInfo(char* akun, int entry) {
    sem_wait (&(mymap->mutex));
    strcpy(mymap->progs[entry].akun, akun);
    mymap->mutexctr++;
    mymap->progs[entry].stamp++;
    sem_post (&(mymap->mutex));
}

// Tambahkan di sini nama para kontributor (mrasyid22, enbeqi, kamilakaffah07, ramyazhar, nazilarahma, nethaniasonya, tesajps)
// Harap UPDATE tanggal revisi!
// Tue Jun  29 23:57:47 WIB 2020
void checkOpen(void) {
    if(mymap->state == CLOSED){
      printf("ShareMemory is NOT OPEN, BYE BYE ==== ====\n");
      exit(0);
    }
}

// Tambahkan di sini nama para kontributor (mrasyid22, enbeqi, kamilakaffah07, ramyazhar, nazilarahma, nethaniasonya, tesajps)
// Harap UPDATE tanggal revisi!
// Tue Jun  30 19:33:47 WIB 2020
void myWait(int boss, int entry){
  if(boss == BOSS){
    for(int i = 0; i< mymap->entry; i++){
      wait(NULL);
    }
    mymap->state = CLOSED;
  }
}

// Tambahkan di sini nama para kontributor (mrasyid22, enbeqi, kamilakaffah07, ramyazhar, nazilarahma, nethaniasonya, tesajps)
// Harap UPDATE tanggal revisi!
// Tue Jun  30 19:46:47 WIB 2020
int main(int argc, char* argv[]) {
  sprintf(tmpStr, "START PID[%d] PPID[%d]", getpid(), getppid());
  myPrint(akunGitHub, tmpStr);
  init(boss, argc, argv);
  if (boss == BOSS){
    if (!fork())
    {
        execlp(progs[1], progs[1], NULL) < 0;
    }
    else
    {
      if (!fork())
      {
          execlp(progs[2], progs[2], NULL) < 0;
      }
      else
      {
        if (!fork())
        {
            execlp(progs[3], progs[3], NULL) < 0;
        }
        else
        {
          if (!fork())
          {
              execlp(progs[4], progs[4], NULL) < 0;
          }
          else
          {
            if (!fork())
            {
              execlp(progs[5], progs[5], NULL) < 0;
            }
            else
            {
              if (!fork())
              {
                execlp(progs[6], progs[6], NULL) < 0;
              }
            }
          }
        }
      }
    }
  }
  checkOpen();
  sleep  (delay);


  sleep(delay);
  int entry = getEntry(akunGitHub);

  sleep  (delay);

  display(entry);

  sleep  (delay);

  putInfo(akunGitHub, entry);

  sleep  (delay);

  display(entry);

  myWait (boss, entry);

  myPrint(akunGitHub, "BYEBYE =====  ===== =====");
}