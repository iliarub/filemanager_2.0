#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "handler.h"

struct fileatr* odir(const char* dname, int *listsize )
{
	DIR *dp = opendir(dname);
	struct dirent *dir;
	struct stat st;
	chdir(dname);
	struct fileatr* tmp=NULL;
	*listsize=0;
	while((dir=readdir(dp))!=0)
	{
		stat(dir->d_name, &st);
		tmp=realloc(tmp, ((*listsize)+1)*sizeof(struct fileatr));
		strcpy(tmp[*listsize].fname, dir->d_name);
		tmp[*listsize].fsize=(int)st.st_size;
		*listsize=*listsize+1;

	}
closedir(dp);
return tmp;
}
