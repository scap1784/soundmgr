/*
** File		hook.h
** Author	lucaze_b <alexis1.lucazeau@epitech.eu>
** Desc		Header for Kernel hook system
*/

#ifndef		HOOK_H__
# define	HOOK_H__

# pragma 	pack(1)
typedef struct ServiceDescriptorEntry {
        unsigned int *ServiceTableBase;
        unsigned int *ServiceCounterTableBase;
        unsigned int NumberOfServices;
        unsigned char *ParamTableBase;
} SSDT_Entry;
# pragma 	pack()

# define	SYSTEMSERVICE(_f1) \
			KeServiceDescriptorTable.ServiceTableBase[*(PULONG) ((PUCHAR) _f1 + 1)]
# define	SYSCALL_INDEX(_f2) \
			*(PULONG) ((PUCHAR) _f2 + 1)
# define	HOOK_SYSCALL(_f3, _h1, _o1) \
			_o1 = (PVOID) InterlockedExchange((PLONG) &gMappedSystemCallTable[SYSCALL_INDEX(_f3)], (LONG) _h1)
# define	UNHOOK_SYSCALL(_f4, _o2) \
			InterlockedExchange((PLONG) &gMappedSystemCallTable[SYSCALL_INDEX(_f4)], (LONG) _o2)

VOID		removeHooks();
NTSTATUS	initHooks();

#endif		/*! HOOK_H__ */
