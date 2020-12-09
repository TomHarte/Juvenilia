#include <sys/stat.h>
#include <dirent.h>
#include "menu.h"
#include "data.h"
#include "../all.h"
#include <string.h>
#include "../z80/z80.h"
#include <ctype.h>

struct listname
{
	char *name;
	struct listname *next;
};

char *simplify_path(char *path)
{
	char *temp, *newpath;
//	int nestlevel;

	newpath = strdup(path);
	temp = newpath + strlen(newpath) - 1;

	while(temp > newpath && *temp != '/')
		temp--;

	if(!strcmp(temp, "/.."))
	{
		temp--;
        
		while(temp > newpath && *temp != '/')
			temp--;

		if(*(temp-1) == ':')
			temp++;

		*temp = '\0';
	}

	if(!strcmp(temp, "/."))
	{
		*temp = '\0';
	}

	if(newpath[2] == newpath[3] && newpath[2] == '/')
	{
		temp = newpath + 2;
                while(temp < newpath+strlen(newpath)-1)
		{
			*temp = *(++temp);
		}
		*temp = '\0';
	}

	free(path);
        
	return newpath;
}

struct listname *add_to_list(struct listname *lastel, char *name)
{
	struct listname *loc;

	if(!lastel)
	{
		loc = malloc(sizeof(struct listname));
	}
	else
	{
		loc = lastel->next = malloc(sizeof(struct listname));
	}

	loc->name = strdup(name);
	loc->next = NULL;
	return loc;
}

void destroy_list(struct listname *list)
{
	struct listname *nel;
        
	while(list)
	{
		nel = list->next;
		free(list->name);
		free(list);
		list = nel;
	}
}

int list_length(struct listname *list)
{
	int len = 0;
        
	while(list)
	{
		len++;
		list = list->next;
	}

	return len;
}

void assemble_scr(char *descl1, char *descl2, int *listpos, struct listname *list, int listlen)
{
	int y, c;

	set_palette(maindata[MainPalette].dat);
	clear(map);
	rectfill(scr, 0, 0, SCREEN_W, SCREEN_H, 255);
	
	draw_sprite(scr, maindata[Title].dat, 0, 0);
	draw_sprite(scr, maindata[SelectFile].dat, (SCREEN_W >> 1) - (((BITMAP *)maindata[SelectFile].dat)->w >> 1), 52);
	draw_sprite(scr, maindata[ScrollCircles].dat, 0, 72);
	draw_sprite(map, maindata[ListMap].dat, 0, 72);
	draw_sprite(scr, maindata[CancelButton].dat, 0, SCREEN_H - ((BITMAP *)maindata[CancelButton].dat)->h);
	draw_sprite(map, maindata[ButtonMap].dat, 0, SCREEN_H - ((BITMAP *)maindata[CancelButton].dat)->h);

	textout_centre(scr, maindata[MainFont].dat, descl1, 160, 161-text_height(maindata[MainFont].dat), 0);
	textout_centre(scr, maindata[MainFont].dat, descl2, 160, 161, 0);

	if(*listpos < 0)
		*listpos = 0;
        
	if(*listpos >= listlen)
		*listpos = listlen-1;
        
	c = 0;
	while(list && c < *listpos)
	{
		list = list->next;
		c++;
	}
        

	c = 0;
	y = 103;
	while(list && c < listlen && y < 388-text_height(maindata[MainFont].dat))
	{
		rectfill(map, 480 - (text_length(maindata[MainFont].dat, list->name) >> 1), y, 480 + (text_length(maindata[MainFont].dat, list->name) >> 1), y + text_height(maindata[MainFont].dat), 4);
		textout_centre(scr, maindata[MainFont].dat, list->name, 480, y, 0);
		c++;
		list = list->next;
		y += text_height(maindata[MainFont].dat);
	}
}

struct listname *add_directories(struct listname *start, struct listname **first)
{
	DIR *dirc;
	struct dirent *dentry;
	struct stat sbuf;
	char *fname;

	*first = NULL;

	if(dirc = opendir(cdir))
	{
		while(dentry = readdir(dirc))
		{
			fname = malloc(strlen(cdir) + 2 + strlen(dentry->d_name));
			sprintf(fname, "%s/%s", cdir, dentry->d_name);
			stat(fname, &sbuf);

			if(S_ISDIR(sbuf.st_mode))
			{
				free(fname);
				fname = malloc(2 + strlen(dentry->d_name));
				sprintf(fname, "/%s", dentry->d_name);
				start = add_to_list(start, fname);

				if(!(*first))
					*first = start;
			}

			free(fname);
		}
                
		closedir(dirc);
	}

	return start;
}

struct listname *add_files_by_mask(struct listname *start, char *mask)
{
	DIR *dirc;
	struct dirent *dentry;
//	struct stat sbuf;
	char *fname, *temp, *temp2;
//	int c;

	if(dirc = opendir(cdir))
	{
		while(dentry = readdir(dirc))
		{
			fname = strdup(dentry->d_name);
			temp = fname + strlen(fname) - 1;
			while(*temp != '.' && temp > fname)
				temp--;

			temp++;
			temp2 = temp;
			while(*temp2)
			{
				*temp2 = tolower(*temp2);
				temp2++;
			}

			if(!strcmp(temp, mask))
			{
				start = add_to_list(start, dentry->d_name);
			}

			free(fname);
		}
                
		closedir(dirc);
	}

	return start;
}

char *select_file(char *mask, char *descl1, char *descl2)
{
	int ow, oh, done, listpos, listlen, c;// obpp, sel;
	BITMAP *old;
	PALETTE opal;
	char *ret, *temp;
	struct listname *namelist, *listptr;
        
	listptr = add_directories(NULL, &namelist);
	listptr = add_files_by_mask(listptr, mask);
	listlen = list_length(namelist);
        
	ow = SCREEN_W;
	oh = SCREEN_H;
	get_palette(opal);
	old = create_bitmap(SCREEN_W,SCREEN_H);
	blit(screen, old, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        
	set_color_depth(8);
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0);

	listpos = 0;
	ret = NULL;
	done = 0;
        
	while(!done)
	{
		assemble_scr(descl1, descl2, &listpos, namelist, listlen);
		switch(go_menu())
		{
			case 4 :
				listptr = namelist;
				c = listpos + (mouse_y - 103) / text_height(maindata[MainFont].dat);
				while(c--)
				{
					listptr = listptr->next;
				}
                                
				temp = listptr->name;
				if(*temp == '/')
				{
					temp = malloc(strlen(cdir) + strlen(listptr->name)+1);
					sprintf(temp, "%s%s", cdir, listptr->name);
					free(cdir);
					cdir = simplify_path(temp);
                                        
					destroy_list(namelist);
					listptr = add_directories(NULL, &namelist);
					listptr = add_files_by_mask(listptr, mask);
					listlen = list_length(namelist);
					listpos = 0;
				}
				else
				{
					ret = malloc(strlen(listptr->name) + strlen(cdir) + 2);
					sprintf(ret, "%s/%s", cdir, listptr->name);
					done = 1;
				}
			break;
                
			case 1 :
				done = 1;
			break;
                        
			case 2 :
				listpos--;
			break;
                        
			case 3 :
				listpos++;
			break;
		}
	}
        
        
	set_color_depth(coldepth & 63);
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, ow, oh, 0, 0);
	blit(old, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
	set_palette(opal);
	destroy_bitmap(old);

	destroy_list(namelist);

	return ret;
}

int load_16kb_rom(char *name, int startaddr)
{
	PACKFILE *romstream;
	int c;

	if(romstream = pack_fopen(name, "r"))
	{
		for(c = 0; c < 4096; c++)
		{
			pack_fread(&mempool[startaddr + (c << 2)], 4, romstream);
		}

		pack_fclose(romstream);
		return 0;
	}
        
	return 1;
}

int load_8kb_rom(char *name, int startaddr)
{
	PACKFILE *romstream;
	int c;

	if(romstream = pack_fopen(name, "r"))
	{
		for(c = 0; c < 2048; c++)
		{
			pack_fread(&mempool[startaddr + (c << 2)], 4, romstream);
		}

		pack_fclose(romstream);
		return 0;
	}
        
	return 1;
}

