#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *dir, char* filename) {
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(dir, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", dir);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", dir);
    close(fd);
    return;
  }


  strcpy(buf, dir);
  p = buf+strlen(buf);
  *p++ = '/';
  while(read(fd, &de, sizeof(de)) == sizeof(de)){
    if(de.inum == 0)
      continue;

    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;

    if(stat(buf, &st) < 0){
      printf("find: cannot stat %s\n", buf);
      continue;
    }

    if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) {
      continue;
    }


    if (st.type == T_DIR) {
      find(buf, filename);
    }


    if (strcmp(filename, de.name) == 0) {
      fprintf(1, "%s\n", buf);
    }
  }

  close(fd);
  
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
      fprintf(1, "Usage: find <directory> <filename>\n");
      exit(0);
    }
    
    find(argv[1], argv[2]);
    exit(0);
}
