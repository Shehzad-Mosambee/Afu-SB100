/*#########################################################################
                 Amoi Tech Mercury Application Build System
                 All Rights Reserved 2020
                 Author:
#########################################################################*/

#ifndef __MERCURY_NETWORK_H__
#define __MERCURY_NETWORK_H__

#include "MercuryDef.h"

#define htons(s) ((((s) & 0xff00) >> 8) |(((s) & 0x00ff) << 8))

#if 0
#define TCP_KEEPALIVE         0x8     /* Start keeplives after this period */
#define TCP_KEEPIDLE         0x2006     /* Start keeplives after this period */
#define TCP_KEEPINTVL        0x2007     /* Interval between keepalives */
#define TCP_KEEPCNT          0x2008     /* Number of keepalives before death */
#endif

/////socket opt //////
#define IPPROTO_IP_OFFSET 	0x10000
#define IPPROTO_TCP_OFFSET  0x20000

/*
 * Level number for (get/set)sockopt() to apply to socket itself.
 */
#define  SOL_SOCKET  0xfff    /* options for socket level */
#define IPPROTO_IP      0
#define IPPROTO_TCP     6
#define IPPROTO_UDP     17


// for level SOL_SOCKET
/*
 * Option flags per-socket. These must match the SOF_ flags in ip.h (checked in init.c)
 */
#define SO_REUSEADDR   0x0004 /* Allow local address reuse */
#define SO_KEEPALIVE   0x0008 /* keep connections alive */
#define SO_BROADCAST   0x0020 /* permit to send and to receive broadcast messages (see IP_SOF_BROADCAST option) */

/*
 * Additional options, not kept in so_options.
 */
#define SO_DEBUG        0x0001 /* Unimplemented: turn on debugging info recording */
#define SO_ACCEPTCONN   0x0002 /* socket has had listen() */
#define SO_DONTROUTE    0x0010 /* Unimplemented: just use interface addresses */
#define SO_USELOOPBACK  0x0040 /* Unimplemented: bypass hardware when possible */
#define SO_LINGER       0x0080 /* linger on close if data present */
#define SO_DONTLINGER   ((int)(~SO_LINGER))
#define SO_OOBINLINE    0x0100 /* Unimplemented: leave received OOB data in line */
#define SO_REUSEPORT    0x0200 /* Unimplemented: allow local address & port reuse */
#define SO_SNDBUF       0x1001 /* Unimplemented: send buffer size */
#define SO_RCVBUF       0x1002 /* receive buffer size */
#define SO_SNDLOWAT     0x1003 /* Unimplemented: send low-water mark */
#define SO_RCVLOWAT     0x1004 /* Unimplemented: receive low-water mark */
#define SO_SNDTIMEO     0x1005 /* send timeout */
#define SO_RCVTIMEO     0x1006 /* receive timeout */
#define SO_ERROR        0x1007 /* get error status and clear */
#define SO_TYPE         0x1008 /* get socket type */
#define SO_CONTIMEO     0x1009 /* Unimplemented: connect timeout */
#define SO_NO_CHECK     0x100a /* don't create UDP checksum */
#define SO_BINDTODEVICE 0x100b /* bind to device */

/*
 * Options for level IPPROTO_IP
 */
#define IP_TOS          IPPROTO_IP_OFFSET + 1
#define IP_TTL          IPPROTO_IP_OFFSET + 2
#define IP_PKTINFO      IPPROTO_IP_OFFSET + 8

/*
 * Options for level IPPROTO_TCP
 */
#define TCP_NODELAY    IPPROTO_TCP_OFFSET+0x01    /* don't delay send to coalesce packets */
#define TCP_KEEPALIVE  IPPROTO_TCP_OFFSET+0x02    /* send KEEPALIVE probes when idle for pcb->keep_idle milliseconds */
#define TCP_KEEPIDLE   IPPROTO_TCP_OFFSET+0x03    /* set pcb->keep_idle  - Same as TCP_KEEPALIVE, but use seconds for get/setsockopt */
#define TCP_KEEPINTVL  IPPROTO_TCP_OFFSET+0x04    /* set pcb->keep_intvl - Use seconds for get/setsockopt */
#define TCP_KEEPCNT    IPPROTO_TCP_OFFSET+0x05    /* set pcb->keep_cnt   - Use number of probes sent for get/setsockopt */


// Address families.
#define AF_UNSPEC       0           /* unspecified */
#define AF_NS           1           /* local to host (pipes, portals) */
#define AF_INET         2           /* internetwork: UDP, TCP, etc. */
#define AF_INET6    3     /* IPv6 */
#define AF_MAX          (AF_INET6 +  1)

//PDP type
#define IP_TYPE           1
#define IPV6_TYPE         2
#define IPV4V6_TYPE   	  3

// Socket state bits
#define SS_NOFDREF          0x0001  /* no file table ref any more */
#define SS_ISCONNECTED      0x0002  /* socket connected to a peer */
#define SS_ISCONNECTING     0x0004  /* in process of connecting to peer */
#define SS_ISDISCONNECTING  0x0008  /*  in process  of disconnecting */
#define SS_CANTSENDMORE     0x0010  /* can't send more data to peer */
#define SS_CANTRCVMORE      0x0020  /* can't receive more data from peer */
#define SS_RCVATMARK        0x0040  /* at mark on input */
#define SS_PRIV             0x0080  /* privileged for broadcast, raw... */
#define SS_NBIO             0x0100  /* non-blocking ops */
#define SS_ASYNC            0x0200  /* async i/o notify */
#define SS_UPCALLED         0x0400  /* zerocopy data has been upcalled (for select) */
#define SS_INUPCALL         0x0800  /* inside zerocopy upcall (reentry guard) */
#define SS_UPCFIN           0x1000  /* inside zerocopy upcall (reentry guard) */
#define SS_WASCONNECTING    0x2000  /* SS_ISCONNECTING w/possible pending error */

// socket error code
#define	EPERM 1		/* Not owner */
#define	ENOENT 2	/* No such file or directory */
#define	ESRCH 3		/* No such process */
#define	EINTR 4		/* Interrupted system call */
#define	EIO 5		/* I/O error */
#define	ENXIO 6		/* No such device or address */
#define	E2BIG 7		/* Arg list too long */
#define	ENOEXEC 8	/* Exec format error */
#define	EBADF 9		/* Bad file number */
#define	ECHILD 10	/* No children */
#define	EAGAIN 11	/* No more processes */
#define	ENOMEM 12	/* Not enough space */
#define	EACCES 13	/* Permission denied */
#define	EFAULT 14	/* Bad address */
#ifdef __LINUX_ERRNO_EXTENSIONS__
#define	ENOTBLK 15	/* Block device required */
#endif
#define	EBUSY 16	/* Device or resource busy */
#define	EEXIST 17	/* File exists */
#define	EXDEV 18	/* Cross-device link */
#define	ENODEV 19	/* No such device */
#define	ENOTDIR 20	/* Not a directory */
#define	EISDIR 21	/* Is a directory */
#define	EINVAL 22	/* Invalid argument */
#define	ENFILE 23	/* Too many open files in system */
#define	EMFILE 24	/* File descriptor value too large */
#define	ENOTTY 25	/* Not a character device */
#define	ETXTBSY 26	/* Text file busy */
#define	EFBIG 27	/* File too large */
#define	ENOSPC 28	/* No space left on device */
#define	ESPIPE 29	/* Illegal seek */
#define	EROFS 30	/* Read-only file system */
#define	EMLINK 31	/* Too many links */
#define	EPIPE 32	/* Broken pipe */
#define	EDOM 33		/* Mathematics argument out of domain of function */
#define	ERANGE 34	/* Result too large */
#define	ENOMSG 35	/* No message of desired type */
#define	EIDRM 36	/* Identifier removed */
#ifdef __LINUX_ERRNO_EXTENSIONS__
#define	ECHRNG 37	/* Channel number out of range */
#define	EL2NSYNC 38	/* Level 2 not synchronized */
#define	EL3HLT 39	/* Level 3 halted */
#define	EL3RST 40	/* Level 3 reset */
#define	ELNRNG 41	/* Link number out of range */
#define	EUNATCH 42	/* Protocol driver not attached */
#define	ENOCSI 43	/* No CSI structure available */
#define	EL2HLT 44	/* Level 2 halted */
#endif
#define	EDEADLK 45	/* Deadlock */
#define	ENOLCK 46	/* No lock */
#ifdef __LINUX_ERRNO_EXTENSIONS__
#define EBADE 50	/* Invalid exchange */
#define EBADR 51	/* Invalid request descriptor */
#define EXFULL 52	/* Exchange full */
#define ENOANO 53	/* No anode */
#define EBADRQC 54	/* Invalid request code */
#define EBADSLT 55	/* Invalid slot */
#define EDEADLOCK 56	/* File locking deadlock error */
#define EBFONT 57	/* Bad font file fmt */
#endif
#define ENOSTR 60	/* Not a stream */
#define ENODATA 61	/* No data (for no delay io) */
#define ETIME 62	/* Stream ioctl timeout */
#define ENOSR 63	/* No stream resources */
#ifdef __LINUX_ERRNO_EXTENSIONS__
#define ENONET 64	/* Machine is not on the network */
#define ENOPKG 65	/* Package not installed */
#define EREMOTE 66	/* The object is remote */
#endif
#define ENOLINK 67	/* Virtual circuit is gone */
#ifdef __LINUX_ERRNO_EXTENSIONS__
#define EADV 68		/* Advertise error */
#define ESRMNT 69	/* Srmount error */
#define	ECOMM 70	/* Communication error on send */
#endif
#define EPROTO 71	/* Protocol error */
#define	EMULTIHOP 74	/* Multihop attempted */
#ifdef __LINUX_ERRNO_EXTENSIONS__
#define	ELBIN 75	/* Inode is remote (not really error) */
#define	EDOTDOT 76	/* Cross mount point (not really error) */
#endif
#define EBADMSG 77	/* Bad message */
#define EFTYPE 79	/* Inappropriate file type or format */
#ifdef __LINUX_ERRNO_EXTENSIONS__
#define ENOTUNIQ 80	/* Given log. name not unique */
#define EBADFD 81	/* f.d. invalid for this operation */
#define EREMCHG 82	/* Remote address changed */
#define ELIBACC 83	/* Can't access a needed shared lib */
#define ELIBBAD 84	/* Accessing a corrupted shared lib */
#define ELIBSCN 85	/* .lib section in a.out corrupted */
#define ELIBMAX 86	/* Attempting to link in too many libs */
#define ELIBEXEC 87	/* Attempting to exec a shared library */
#endif
#define ENOSYS 88	/* Function not implemented */
#ifdef __CYGWIN__
#define ENMFILE 89      /* No more files */
#endif
#define ENOTEMPTY 90	/* Directory not empty */
#define ENAMETOOLONG 91	/* File or path name too long */
#define ELOOP 92	/* Too many symbolic links */
#define EOPNOTSUPP 95	/* Operation not supported on socket */
#define EPFNOSUPPORT 96 /* Protocol family not supported */
#define ECONNRESET 104  /* Connection reset by peer */
#define ENOBUFS 105	/* No buffer space available */
#define EAFNOSUPPORT 106 /* Address family not supported by protocol family */
#define EPROTOTYPE 107	/* Protocol wrong type for socket */
#define ENOTSOCK 108	/* Socket operation on non-socket */
#define ENOPROTOOPT 109	/* Protocol not available */
#ifdef __LINUX_ERRNO_EXTENSIONS__
#define ESHUTDOWN 110	/* Can't send after socket shutdown */
#endif
#define ECONNREFUSED 111	/* Connection refused */
#define EADDRINUSE 112		/* Address already in use */
#define ECONNABORTED 113	/* Software caused connection abort */
#define ENETUNREACH 114		/* Network is unreachable */
#define ENETDOWN 115		/* Network interface is not configured */
#define ETIMEDOUT 116		/* Connection timed out */
#define EHOSTDOWN 117		/* Host is down */
#define EHOSTUNREACH 118	/* Host is unreachable */
#define EINPROGRESS 119		/* Connection already in progress */
#define EALREADY 120		/* Socket already connected */
#define EDESTADDRREQ 121	/* Destination address required */
#define EMSGSIZE 122		/* Message too long */
#define EPROTONOSUPPORT 123	/* Unknown protocol */
#ifdef __LINUX_ERRNO_EXTENSIONS__
#define ESOCKTNOSUPPORT 124	/* Socket type not supported */
#endif
#define EADDRNOTAVAIL 125	/* Address not available */
#define ENETRESET 126		/* Connection aborted by network */
#define EISCONN 127		/* Socket is already connected */
#define ENOTCONN 128		/* Socket is not connected */
#define ETOOMANYREFS 129
#ifdef __LINUX_ERRNO_EXTENSIONS__
#define EPROCLIM 130
#define EUSERS 131
#endif
#define EDQUOT 132
#define ESTALE 133
#define ENOTSUP 134		/* Not supported */
#ifdef __LINUX_ERRNO_EXTENSIONS__
#define ENOMEDIUM 135   /* No medium (in tape drive) */
#endif
#ifdef __CYGWIN__
#define ENOSHARE 136    /* No such host or network path */
#define ECASECLASH 137  /* Filename exists with different case */
#endif
#define EILSEQ 138		/* Illegal byte sequence */
#define EOVERFLOW 139	/* Value too large for defined data type */
#define ECANCELED 140	/* Operation canceled */
#define ENOTRECOVERABLE 141	/* State not recoverable */
#define EOWNERDEAD 142	/* Previous owner died */
#ifdef __LINUX_ERRNO_EXTENSIONS__
#define ESTRPIPE 143	/* Streams pipe error */
#endif
#define EWOULDBLOCK EAGAIN	/* Operation would block */

#define __ELASTERROR 2000	/* Users can add values starting here */



#define MC_NW_MCC_MAX_LEN 3
#define MC_NW_MNC_MAX_LEN 3
#define MC_NW_LONG_EONS_MAX_LEN 32
#define MC_NW_SHORT_EONS_MAX_LEN 32

typedef int TCPIP_SOCKET_T;
typedef  unsigned int     TCPIP_IPADDR_T;
typedef  uint32     TCPIP_HOST_HANDLE;          /* async gethostbyname request handle */
typedef  int 		TCPIP_ERROR_E;
typedef  uint32     TCPIP_NETID_T;              /* TCPIP net interface ID */

typedef struct
{
    TCPIP_IPADDR_T  ipaddr;     /* host IP */
    TCPIP_IPADDR_T  snmask;     /* subnet mask */
    TCPIP_IPADDR_T  gateway;    /* gateway */
    TCPIP_IPADDR_T  dns1;       /* primary DNS */
    TCPIP_IPADDR_T  dns2;       /* secondary DNS */
} TCPIP_NETIF_IPADDR_T;

// TCPIP IPv6 address, 128 bit
#define TCPIP_IP6_ADDR_LEN_BYTES    16
typedef struct {
    union {
        uint8   u6_addr8[TCPIP_IP6_ADDR_LEN_BYTES];
        uint16  u6_addr16[TCPIP_IP6_ADDR_LEN_BYTES>>1];
        uint32  u6_addr32[TCPIP_IP6_ADDR_LEN_BYTES>>2];
    } u6_addr;
} TCPIP_IPADDR6_T;

typedef enum
{
    PLMN_NO_SERVICE = 0 ,          // no service
    PLMN_NORMAL_GSM_ONLY = 0x01 ,     // voice service available
    PLMN_NORMAL_GPRS_ONLY = 0x02 ,    // data service available
    PLMN_NORMAL_GSM_GPRS_BOTH = 0x03 ,// voice and data service available

    PLMN_NORMAL_CS_ONLY = 0x01 ,     // voice service available
    PLMN_NORMAL_PS_ONLY = 0x02 ,    // data service availabe
    PLMN_NORMAL_CS_PS_BOTH = 0x03 ,// voice and data service available

    PLMN_EMERGENCY_ONLY = 0x04,       // emergency service available

    PLMN_EMERGENCY_SIM_INVALID = 0x05,        /* emergency; MM in limited service state and
                                               no further PLMN access allowed until power
                                             off or new SIM inserted */
    PLMN_EMERGENCY_GPRS_ONLY = 0x06,      // data service availabe but emergency; MM in limited service state
    PLMN_EMERGENCY_SIM_INVALID_GPRS_ONLY = 0x07,   /* data service availabe but emergency; MM in limited service state and
                                                  no further PLMN access allowed until power
                                               off or new SIM inserted */

    PLMN_REGISTER_SERVICE = 0x08,    //attaching after camping on
    PLMN_REGISTER_GPRS_ONLY = 0x09, // data service availabe but attaching;
    PLMN_FULL_PS_SERVICE = 0x0A,       /*full PS service, no cs service*/
	PLMN_NORMAL_EPS = 0x0B ,		//EPS service available        
    MERCURY_ENUM_PHONEPLMN_MAX = 0x7fffffff
} MERCURY_PHONE_PLMN_STATUS_E;

typedef enum
{
	MN_INVALID_STATE, /* this field is invalid */
	MN_ATTACHED_STATE, /* cs or ps has been attached */
	MN_DETACHED_SATE, /* cs or ps has been detached */
	MN_NO_SERVICE,  /* no service for cs or ps */
	MERCURY_ENUM_ATTACH_MAX = 0x7fffffff
} MERCURY_ATTACH_STATE_E;


typedef struct
{
    MERCURY_PHONE_PLMN_STATUS_E  plmn_status;        // plmn statue
    BOOL                plmn_is_roaming;    // if plmn is roaming plmn
    WORD                  mcc;
    WORD                  mnc;
    WORD                  mnc_digit_num;
    WORD                  lac;
    WORD                  cell_id;
    WORD               	rat_type;		    	//0:gsm; 1:gsm compact; 7:lte
    BOOL                long_cell_id_flag;		// 0: is short, is stored in the cell_id; 1: is long, the full cell id is stored in long_cell_id
    DWORD              	long_cell_id;    		// consist of 4 bytes	
}MERCURY_NETWORK_STATUS_T;

typedef enum
{
    SOCKET_TYPE_TCP = 0,
    SOCKET_TYPE_UDP,
    SOCKET_TYPE_RAWDATA,
    MERCURY_ENUM_SOCKETTYPE_MAX = 0x7fffffff
}SOCKET_TYPE_E;

typedef enum
{
    SOCKET_TXDATA=0,  		/* get count of bytes in sb_snd */
    SOCKET_RXDATA,     		/* get count of bytes in sb_rcv */
    SOCKET_MYADDR,    		/* return my IP address */
    SOCKET_ACK,       		/*peer acked bytes in current TCP connection*/
    SOCKET_LINGER,       	/* linger on close if data present */
    SOCKET_SACK,			/* Allow TCP SACK (Selective acknowledgment) */
    SOCKET_WSOPT,			/* Set scaling window option */
    SOCKET_TSPOT,			/* Set TCP timestamp option */
    SOCKET_BIGCWND,			/* Large initial TCP Congenstion window */
	SOCKET_NOSLOWSTART,		/* suppress slowstart on this socket */
    SOCKET_MSS,				/* force packets to all be MAX size */
    SOCKET_ACKDELAYTIME,	/* Set time for delayed acks */
    SOCKET_NOACKDELAY,		/* suppress delayed ACKs */
    SOCKET_MAXSEG,			/* set TCP maximum segment size */
    SOCKET_NODELAY,			/* Disable Nagle Algorithm */
    SOCKET_OPT_MAX,
    MERCURY_ENUM_SOCKETOPT_MAX = 0x7fffffff
}SOCKET_OPT_E;

typedef enum
{
    PDP_ID0=1,
    PDP_ID1,
    PDP_ID2,
    MERCURY_ENUM_PDPID_MAX = 0x7fffffff
}PDP_ID_E;

typedef struct
{
    unsigned short  port;           /* port number */
    unsigned long   ip_addr;        /* ip address */
}SOCKET_ADDR_S;

/* Berkeley style "Socket address" */
typedef struct sci_sockaddr
{
    unsigned short  family;         /* address family */
    unsigned short  port;           /* port number */
    unsigned long   ip_addr;        /* ip address */
    char            sa_data[8];     /* up to 14 bytes of direct address */
}SOCKET_ADDR_EXT_S;

typedef struct
{
    unsigned short  port;           /* port number */
    unsigned char   ip_addr[16];        /* ip address */
}V6_SOCKET_ADDR_S;

/* the definitions to support the select() function. These are about
 * as UNIX-like as we can make 'em on embedded code. They are also
 * fairly compatable with WinSock's select() definitions.
 */
typedef struct    /* the select socket array manager */
{
    unsigned        fd_count;       /* how many are SET? */
    long            fd_array[12];   /* an array of SOCKETs, define FD_SETSIZE 12 tcpip internal definition */
} MERCURY_FD_SET_S;

typedef struct
{
    uint32 cell_exist;
    uint16  arfcn;
    uint8   bsic;
    uint8   rxlev;
    uint16  mcc;
    uint16  mnc;
    uint16  mnc_digit_num;
    uint16  lac;
    uint32  cell_id;
} NCELLS_INFO_T;

typedef struct
{
    uint32 cell_exist;
    uint16  arfcn;
    uint8   bsic;
    uint8   rxlev;
    uint16  mcc;
    uint16  mnc;
    uint16  mnc_digit_num;
    uint16  lac;
    uint32  cell_id;
} SCELL_INFO_T;

#define NCELL_MAX_NUM  10

typedef struct
{
    NCELLS_INFO_T ncells[NCELL_MAX_NUM];
    SCELL_INFO_T      scell;
}MERCURY_CELLS_INFO_T;

typedef struct
{
    uint16  arfcn;
    uint8   bsic;
    uint8   rxlev;
    uint16  mcc;
    uint16  mnc;
    uint16  mnc_digit_num;
    uint16  lac;
    uint16  cell_id;
} MC_NW_GSM_SCELL_V2_T;

typedef struct
{
    uint16  arfcn;
    uint8   bsic;
    uint8   rxlev;
    uint16  mcc;
    uint16  mnc;
    uint16  mnc_digit_num;
    uint16  lac;
    uint16  cell_id;
} MC_NW_GSM_NCELL_V2_T;

typedef struct
{
    uint16  mcc;
    uint16  mnc;
    uint16	mnc_digit_num;
    uint32 	frequency;  		/**E-UTRA absolute radio frequency channel number of the cell. RANGE: 0 TO 65535. */
    int 	cell_id;       		/**<Cell ID, (0 indicates information is not represent).*/
    int 	pcid;           	/**Physical cell ID. Range: 0 to 503. */
    uint16 	rsrp;
    uint16 	rsrq;
    uint16 	Srxlev;
    int 	tac;            	/**Tracing area code (This field should be ignored when cid is not present). */	
}MC_NW_LTE_NCELL_V2_T;

typedef struct
{
    uint16  mcc;
    uint16  mnc;
    uint16  mnc_digit_num;
    int 	earfcn;         /**E-UTRA absolute radio frequency channel number of the cell. RANGE: 0 TO 65535. */
    int 	cell_id;        /**<Cell ID, (0 indicates information is not represent).*/
    int 	tac;            /**Tracing area code (This field should be ignored when cid is not present). */
    int 	pcid;           /**Physical cell ID. Range: 0 to 503. */
    uint16 	bandInfo;       //< band1-band256
    uint16 	rsrp;
    uint16 	rsrq;
    uint16 	Srxlev;
    uint8   rxlev;	
    uint16 	netMode;  		//0--tdd  1--fdd		
}MC_NW_LTE_SCELL_V2_T;

typedef struct
{
    uint8             valid_flag;                        /**< Must be set to TRUE if gsm_info is being passed. */	
    MC_NW_GSM_SCELL_V2_T scell;	
    uint8             ncell_num;                         /**< Must be set to the number of elements in entry*/	
    MC_NW_GSM_NCELL_V2_T ncells[NCELL_MAX_NUM];
}MC_NW_GSM_CELL_INFO_V2_T;

typedef struct
{
    uint8             valid_flag;                        /**< Must be set to TRUE if gsm_info is being passed. */	
    MC_NW_LTE_SCELL_V2_T scell;	
    uint8             ncell_num;                         /**< Must be set to the number of elements in entry*/	
    MC_NW_LTE_NCELL_V2_T ncells[NCELL_MAX_NUM];
}MC_NW_LTE_CELL_INFO_V2_T;

/** Gets cell information. */
typedef struct 
{
    MC_NW_GSM_CELL_INFO_V2_T    gsm;  /**<   GSM cell information (Serving and neighbor. */  
    MC_NW_LTE_CELL_INFO_V2_T    lte;  /**<   LTE cell information (Serving and neighbor). */
}MERCURY_CELLS_INFO_V2_T;

typedef struct
{
    uint8 nQci;			//specifies a class of EPS QoS
    uint32 nDlGbr;		//indicates DL GBR in case of GBR QCI. The value is in kbit/s.
    uint32 nUlGbr;		//indicates UL GBR in case of GBR QCI. The value is in kbit/s.
    uint32 nDlMbr;		//indicates DL MBR in case of GBR QCI. The value is in kbit/s.
    uint32 nUlMbr;		//indicates UL MBR in case of GBR QCI. The value is in kbit/s.
} MC_EPS_QOS_T;

typedef struct{
    /* R97/98 */
    uint32        reliability;
    uint32        peak_thrput;
    uint32        delay_class;
    uint32        precedence;
    uint32        mean_thrput;
}MERCURY_GPRS_QOS_R98_T;

typedef struct{
    /* R97/98 */
    uint32            reliability;
    uint32        peak_thrput;
    uint32                 delay_class;
    uint32            precedence;
    uint32        mean_thrput;

    /* R99 */
    BOOLEAN    is_r99;
    uint32        delivery_of_err_sdu;
    uint32        delivery_order;
    uint32        traffic_class;
    uint32        max_sdu_size;
    uint32        max_bit_rate_for_uplink;
    uint32        max_bit_rate_for_downlink;
    uint32        residual_ber;
    uint32        sdu_err_ratio;
    uint32        traffic_handling_prio;
    uint32        transfer_delay;
    uint32        guaranteed_bit_rate_for_uplink;
    uint32        guaranteed_bit_rate_for_downlink;
}MERCURY_GPRS_TOTAL_QOS_T;

typedef enum{
	MC_NW_MODE_AUTO = 0,
	MC_NW_MODE_GSM_ONLY = 2,
	MC_NW_MODE_LTE_ONLY = 4	
}MC_NW_MODE_E;

typedef enum
{
    AUTH_PAP       = 0,
    AUTH_CHAP      = 1,
    AUTH_PAP_CHAP  = 2,
    AUTH_NONE = 3
} PCO_AUTH_TYPE_E;

// PING result type
typedef enum
{
    PINGRES_SUCCESS         = 0,    /* ping OK, received ping echo reply */
    PINGRES_DNS_TIMEOUT     = 1,    /* ping fail, DNS timeout */
    PINGRES_DNS_ERROR       = 2,    /* ping fail, DNS error */
    PINGRES_ICMP_TIMEOUT    = 3,    /* ping fail, icmp timeout */
    PINGRES_ICMP_ERROR      = 4     /* ping fail, icmp error */
}TCPIP_PING_RESULT_E;

typedef enum
{
    CONTEXT_ACTIVATED			= 0,
    CONTEXT_ACTIVATED_PENDING,
    CONTEXT_DEACTIVATED_PENDING,
    CONTEXT_DEACTIVATED,
    CONTEXT_MODIFY_PENDING,
    CONTEXT_NOT_USED = 0xff
}MC_GPRS_PDP_STATE_E;

typedef struct
{
    uint16  mcc;
    uint16  mnc;
    uint16  mnc_digit_num;	
	uint8 nRat;
    uint32 nArfcn;
    uint16 nTac;
    uint16 nPci;
    uint16 nRSRP;
    uint16 nRSRQ;
    uint16 nRSSI;
    uint16 nBand;
} MC_NW_FREQ_SCAN_CELL_INFO;

/*scan freq notify struct*/
typedef struct
{
    uint8 num;								/*cell number*/
    MC_NW_FREQ_SCAN_CELL_INFO cell[20];    	/*cell information, include mcc,mnc,rssi,etc*/
}MC_NW_FREQ_SCAN_INFO;

// PING result callback function
typedef void (*TCPIP_PING_CALLBACK_FPTR)(
    TCPIP_PING_RESULT_E     res,            /* ping result, 0 - succeed; other - fail */
    uint32                  time_delay,     /* ping time delay, only valid when success, unit: ms */
    uint16       ping_handle );  /* ping handle */

typedef void (*TCPIP_PING_CALLBACK_EX_FPTR)(
    TCPIP_PING_RESULT_E     res,            /* ping result, 0 - succeed; other - fail */
    uint32                  time_delay,     /* ping time delay, only valid when success, unit: ms */
    uint16       ping_handle,  /* ping handle */
    uint8             ttl,
    char*            ipaddr);

typedef void (*TCPIP_DHCP_CALLBACK_FPTR)(
    int                       res,        /* DHCP result - 0: OK; else - error */
    const TCPIP_NETIF_IPADDR_T* addr_ptr,   /* ip addresses pointer */
    TCPIP_IPADDR_T              dhcp_addr,  /* DHCP server address pointer */
    uint32               netid );    /* net interface ID */

typedef struct
{
  char long_eons[MC_NW_LONG_EONS_MAX_LEN+1];
  char short_eons[MC_NW_SHORT_EONS_MAX_LEN+1];
  char mcc[MC_NW_MCC_MAX_LEN+1];
  char mnc[MC_NW_MNC_MAX_LEN+1];
}MC_NW_OPERATOR_INFO_T;

typedef enum
{
	MC_CE_MODE_PS_ONLY_DATA_CENTRIC = 0,	/*0 PS mode 2 of operation: The UE registers only to EPS services, and UE?��s usage setting is "data centric"*/
	MC_CE_MODE_PS_CS_BOTH_VOICE_CENTRIC,	/*1 CS/PS mode 1 of operation: The UE registers only to both EPS and non-EPS services, and UE?��s usage setting is "voice centric"*/
	MC_CE_MODE_PS_CS_BOTH_DATA_CENTRIC,		/*2 CS/PS mode 2 of operation: The UE registers only to both EPS and non-EPS services, and UE?��s usage setting is "data centric"*/
	MC_CE_MODE_PS_ONLY_VOICE_CENTRIC,		/*3 PS mode 1 of operation: The UE registers only to EPS services, and UE?��s usage setting is "voice centric"*/
	MC_CE_MODE_MAX,
}MC_CE_MODE_E;
/*
define GPRS Mobile Station Class enum for gsm 2g network
*/
typedef enum
{
	MC_CG_CLASS_PS_CS_BOTH = 0,	     //means that the MT would operate PS and CS services but not simultaneously
	MC_CG_CLASS_PS_ONLY,	         //means that the MT would only operate PS services
	MC_CG_CLASS_MAX,
}MC_CG_CLASS_E;
int NetworkGetState(MERCURY_NETWORK_STATUS_T* pstatus);
/*
rssi level
0        -113dBm or less
1        -111 dBm
2...30   -109...-53dBm
31       -51dBm or greater
99       unknown or not detectable
*/
int NetworkGetSignalQuality(BYTE* prssi,WORD* prxfull);
/*
id form PDP_ID0 to PDP_ID2
if apn is null then user the default apn
if user and password are null then we think it is not need password
*/
int NetworkSetAPN(BYTE id,BYTE* apn,BYTE* user, BYTE* password);

int NetworkOpenPDP(BYTE id);
int NetworkClosePDP(BYTE id);

extern BOOLEAN SoOptPreSetFlagGet(void);
extern void SoOptPreSetFlagSet(BOOLEAN preset_flag);
extern TCPIP_SOCKET_T  SocketCreate(SOCKET_TYPE_E type);
extern int SocketConnect(TCPIP_SOCKET_T so,SOCKET_ADDR_S* addr_ptr, int addr_len);
extern int SocketSend( TCPIP_SOCKET_T so, char* buf, int len);
extern int SocketSendTo(TCPIP_SOCKET_T so, char* buf, int len, SOCKET_ADDR_S* to );
extern int SocketRecv(TCPIP_SOCKET_T so, char* buf, int len);
extern int SocketRecvFrom(TCPIP_SOCKET_T so, char* buf, int len,SOCKET_ADDR_S* from );
extern int SocketClose(TCPIP_SOCKET_T so);
extern int SocketShutdown(TCPIP_SOCKET_T so, int how);
extern int SocketErrNo(TCPIP_SOCKET_T so);
extern int INetAtoN( char* ip_str, TCPIP_IPADDR_T* ipaddr_ptr );
extern char* INetNtoA( TCPIP_IPADDR_T ipaddr );
/*
time_out uint ms
return 0 -- faile   other --- request handle
*/
extern TCPIP_HOST_HANDLE NetworkGetHostByName(char* name_ptr,uint32  time_out);
extern int FlightModeSet(BOOLEAN enable);
extern int GprsAttach(void);
extern int GprsDetach(void);
extern int SocketGetOpt(TCPIP_SOCKET_T so, SOCKET_OPT_E opt, uint32* pdata);
extern int SocketGetState( TCPIP_SOCKET_T so, short* state_ptr);
extern BOOLEAN Network_GetPDPStatus(void);
/*
��ȡ�ٽ���վ������վ��Ϣ
cell_exist = 1 ������ȡ����Ӧ�Ļ�վ��Ϣ
�����Ի�ȡ6���ٽ���վ��Ϣ
*/
extern uint32 Network_BaseStationInfoGet(MERCURY_CELLS_INFO_T* cell);

/*
dns1Ϊ��DNS, dns2Ϊ��DNS
*/
extern int Network_SetDns(TCPIP_IPADDR_T dns1, TCPIP_IPADDR_T dns2);

/*
��pdp�Ѽ���ǰ���¿�ʹ�ô˽ӿ�
������ΪNULL��ʾ����ȡ����ֵ
*/
extern int Network_NetAddrGet(char *pszIp, char *pszMask, char *pszGateway, char *pszDns);
extern void Network_ForceCampon(uint16 arfcn);
extern void Network_CancelForceCampon(void);
/*****************************************************************************/
//uint32 penalty_time��ʾ��ֹʱ�䣬��λΪs
//uint8 retransmit_num��������ش�����
/*****************************************************************************/
extern int Network_SetGprsMassRetransmitParam(uint32 penalty_time, uint8 retransmit_num);
extern int Network_SetAuthType(PCO_AUTH_TYPE_E type);

#define NET_BAND_GSM                       0x00
#define NET_BAND_DCS                       0x01
#define NET_BAND_GSM_DCS                   0x02
#define NET_BAND_PCS                       0x03
#define NET_BAND_GSM850                    0x04
#define NET_BAND_GSM_PCS                   0x05
#define NET_BAND_GSM850_DCS                0x06
#define NET_BAND_GSM850_PCS                0x07
#define NET_BAND_GSM850_GSM                0x08
#define NET_BAND_GSM850_GSM_PCS            0x09
#define NET_BAND_GSM850_GSM_DCS            0x0a
#define NET_BAND_GSM_DCS_PCS               0x0b
#define NET_BAND_GSM850_GSM_DCS_PCS        0x0c
#define NET_BAND_DCS_PCS                   0x0d
#define NET_BAND_GSM850_DCS_PCS            0x0e
extern int Network_SelectBand(int userBand);
extern uint16 Network_PingRequest(char * faddr_ptr,uint32 data_size,uint32 time_out,TCPIP_PING_CALLBACK_FPTR callback_fptr);
extern void Network_PingCancel(uint16 pingHandle);
extern uint8 Network_GetTaPwr(uint8* pTa, uint8* pPwr);
extern int SocketSetOpt(TCPIP_SOCKET_T so, int opt, uint32* pdata);

extern uint16 Network_PingV4Request(char* faddr_ptr,uint32 data_size,uint32 time_out,TCPIP_PING_CALLBACK_EX_FPTR    callback_fptr);
extern uint16 Network_PingV6Request(char* faddr_ptr,uint32 data_size,uint32 time_out,TCPIP_PING_CALLBACK_EX_FPTR    callback_fptr);

extern int NetworkSetPdpType(uint8 type);

/*
ipv4 �� ipv6�󲿷�socket����api����ͬ��
��ʹ��ipv6ʱ��create/connect/sendto/rcvfrom�⼸����������Ҫ������Ľӿ�
*/
extern TCPIP_SOCKET_T  SocketCreateV6(SOCKET_TYPE_E type);
extern int SocketConnectV6(TCPIP_SOCKET_T so,V6_SOCKET_ADDR_S* addr_ptr, int addr_len);
extern int SocketSendToV6(TCPIP_SOCKET_T so, char* buf, int len, V6_SOCKET_ADDR_S* to );
extern int SocketRecvFromV6(TCPIP_SOCKET_T so, char* buf, int len,V6_SOCKET_ADDR_S* from );

/***********************************************************************************
mercury ƽ̨�ײ㼯����9051������������
��Ҫappָ��spi cs�߼�id �� ���ڿ���9051�����gpio(rst �� INT)
ʹ��˳��:
1��ETH_RegInterface ����nei_id
2��ETH_DhcpRequest
3��ETH_SocketCreate (��create�����⣬�����socket�ӿ�ͬgprs)
***********************************************************************************/
extern TCPIP_SOCKET_T  ETH_SocketCreate(SOCKET_TYPE_E type,uint32 netID);
/*
ipv6Flag Ϊ0 ���ʾ������ipv6 ����ֵ��ʾ����ipv6����ͬʱ��ʾ��ַǰ׺����
����ֵΪ�ó��ص�netid
*/
extern uint32 ETH_RegInterface(uint32 spiID, uint32 gpioRst, uint32 gpioInt, uint8* mac, uint32 ipv6Flag);
extern void ETH_DeRegInterface(uint32 netid);
/*
ipv4�龰�£�������ע��ɹ���ip��ȡ�����ַ�ʽ
1��ͨ��dhcp��ȡ(ETH_DhcpRequest)
2��ͨ���ֶ�����ip(ETH_SetIpAddress)

ipv6�龰�£�ip�Ļ�ȡtcpip����������RS����ȥ��ȡ����������
*/
extern int ETH_DhcpRequest(TCPIP_DHCP_CALLBACK_FPTR callback_fptr,uint32 time_out,uint32 net_id );
extern void ETH_DhcpCancel(uint32 net_id);
extern int ETH_SetIpAddress(const TCPIP_NETIF_IPADDR_T*	addr_ptr, uint32 net_id );
extern BOOLEAN ETH_GetIpAddress( TCPIP_NETIF_IPADDR_T*	 addr_ptr,uint32  net_id );

/*
Network_SetDnsV6 �� Network_GetIpv6Address
�ӿ�gprs��·���غ�������·����(DM9051)������
DM9851���� -- netid������Ӧ������·id
gprs���� -- net_id ��-1
*/
extern int Network_SetDnsV6(TCPIP_IPADDR6_T* dns,uint32  net_id);
extern BOOLEAN Network_GetIpv6Address(TCPIP_IPADDR6_T* ip6addr_ptr,TCPIP_IPADDR6_T* localaddr_ptr,TCPIP_IPADDR6_T* dns_ptr,uint32 net_id );
/*
������·����(DM9051) ping�ӿ�
*/
extern uint16 ETH_PingV4Request(char* faddr_ptr,uint32 data_size,uint32 time_out,TCPIP_PING_CALLBACK_EX_FPTR    callback_fptr,uint32 netid);
extern uint16 ETH_PingV6Request(char* faddr_ptr,uint32 data_size,uint32 time_out,TCPIP_PING_CALLBACK_EX_FPTR    callback_fptr,uint32 netid);

extern int SocketBind(TCPIP_SOCKET_T so, struct sci_sockaddr* addr_ptr, int addr_len);
extern int SocketListen( TCPIP_SOCKET_T so, int backlog);
extern TCPIP_SOCKET_T SocketAccept(TCPIP_SOCKET_T so, struct sci_sockaddr* addr_ptr, int* addr_len);
extern int SocketSelect(MERCURY_FD_SET_S* in,  MERCURY_FD_SET_S* out,MERCURY_FD_SET_S* ex, long tv);

extern void FdClr( TCPIP_SOCKET_T so, MERCURY_FD_SET_S* set);
extern void FdSet( TCPIP_SOCKET_T so, MERCURY_FD_SET_S* set);
extern int   FdIsSet( TCPIP_SOCKET_T so, MERCURY_FD_SET_S* set);
extern void FdZero(MERCURY_FD_SET_S* set);

extern int PS_StatusSet(int state);
extern void PosModeSet(BOOLEAN pos_mode_flag);
extern BOOLEAN PosModeGet(void);
extern uint8 Network_GetT3302(void);
extern int Network_SetT3302(uint8 user_t3302_value);
extern int Network_ReadPDPStatus(BYTE id);

extern int NetworkSetAndOpenPDP(uint8* APNcn);
extern int NetworkResetAndClosePDP(void);
extern int ReadQosProfile_R98(uint32 PDP_ID, MERCURY_GPRS_QOS_R98_T *qos_r98_ptr, BOOLEAN is_minqos);
extern int SetQosProfile_R98(uint32 PDP_ID, MERCURY_GPRS_QOS_R98_T *qos_r98_ptr, BOOLEAN is_minqos);
extern int ReadTotalQosProfile(uint32 PDP_ID, MERCURY_GPRS_TOTAL_QOS_T *total_qos_ptr, BOOLEAN is_minqos, BOOLEAN is_negotiated);
extern int SetTotalQosProfile(uint32 PDP_ID, MERCURY_GPRS_TOTAL_QOS_T *total_qos_ptr, BOOLEAN is_minqos);
extern int ReadQosProfile_Eps(uint32 PDP_ID, MC_EPS_QOS_T *qos_eps_ptr);
extern int SetQosProfile_Eps(uint32 PDP_ID, MC_EPS_QOS_T *qos_eps_ptr);
extern int NetworkSetMode(MC_NW_MODE_E mode);
extern int NetworkGetMode(MC_NW_MODE_E *mode);

/**
 * �Ƿ�֧��volte����
 *
 * @param: no
 * @return 1: volte support;  0: volte not support
 */
extern BOOLEAN NetworkVolteIsSupport(void);

/**
 * volte���ܿ���/�ر�����
 *
 * @param [BOOLEAN] enable  0: disable; 1: enable
 * @return 0:SUCCESS  other value:FAIL
 */
extern int NetworkVolteSet(BOOLEAN enable);

/**
 * volte���ö�ȡ
 *
 * @param [BOOLEAN *] enable  0: disable; 1: enable
 * @return 0:SUCCESS  other value:FAIL
 */
extern int NetworkVolteGet(BOOLEAN *enable);

/**
 * lock /unlock lte band
 *
 * @param [uint8] bandnum: Ҫ����band���������5��
 * @param [uint16 *] band: bandƵ��ֵ�����5��
 * @return 0:SUCCESS  other value:FAIL
 * Note: ���Ҫ������bandnum=0, band = NULL
 */
extern int NetworkSetLteBandLock(uint8 bandnum, uint16 *band);

/**
 *  ��ѯlte lock band 
 *
 * @param [uint8 *] bandnum: ����band�ĸ���
 * @param [uint16 *] band: bandƵ��ֵ�����5��
 * @return 0:SUCCESS  other value:FAIL
 */
extern int NetworkGetLteBandLock(uint8 *bandnum, uint16 band[5]);

/**
 * lock /unlock lte freq
 *
 * @param [uint8] freqnum: Ҫ����Ƶ��ĸ��������9��
 * @param [uint16 *] freq: Ƶ��ֵ�����9��
 * @return 0:SUCCESS  other value:FAIL
 * Note: ���Ҫ������freqnum=0, freq = NULL
 */
extern int NetworkSetLteFreqLock(uint8_t freqnum, uint32_t *freq);

/**
 *  ��ѯlte lock freq
 *
 * @param [uint8 *] freqnum: ����Ƶ��ĸ���
 * @param [uint16 *] freq: Ƶ��ֵ�����9��
 * @return 0:SUCCESS  other value:FAIL
 */
extern int NetworkGetLteFreqLock(uint8_t *freqnum, uint32_t freq[9]);

/**
 *   ��������ͨѶ�󣬿��ٽ���͹���ʱ�䣬��Χ 1-20s
 */
uint32_t NetworkSetRRCRel(uint32_t timeOut);

uint32_t NetworkGetRRCRel(void);

/**
 *  set socket opt
 *
 * @param [in] s: socketid 
 * @param [in] level: IPPROTO_TCP IPPROTO_IP  SOL_SOCKET
 * @param [in] optname: Additional options 
 * @param [in] optval: opt data
 * @param [in] optlen
 */
int setsockopt (int s, int level, int optname, const void *optval, int optlen);

int getsockopt (int s, int level, int optname, void *optval, int *optlen);

/**
 *	��ȡ�ٽ���վ������վ��Ϣ(�ṹ����Ϣ����GSM��LTE��վ)
 *
 * @param [MERCURY_CELLS_INFO_V2_T *] cell: ��վ��Ϣ
 * @return 0:SUCCESS  other value:FAIL
 */
uint32 Network_BaseStationInfoGetV2(MERCURY_CELLS_INFO_V2_T* cell);

int NetworkGetAPN(BYTE nCid,BYTE* apn,BYTE* user, BYTE* password);

/**
 *	ɨƵ����ȡ��վ��Ϣ
 *
 * @param [uint32 *] timeout: ��λs������ֵΪ60s��
 * @return 0:SUCCESS  other value:FAIL
 * Note: 1.ɨƵʱ��Ҫ�ر�Э��ջ��������Э��ջ������������ڲ����ȹر���ɨƵ������ٻָ���
 *	 2.ɨƵ���̣���Ҫ����Э��ջ����Ӱ��ɨƵ
 *     3.�첽���ýӿڣ������NOTIFY��Ϣ��ʽ֪ͨӦ��
 *		Notify Class Id: NOTIFY_CLASS_NW
 *		Notify Id: NOTIFY_ID_NW_SCAN_FREQ_OK  	�ɹ�:��վ��Ϣ���ź�ǿ�ȴӸߵ�����������pData�����У��ṹ������MC_NW_FREQ_SCAN_INFO
 *		Notify Id: NOTIFY_ID_NW_SCAN_FREQ_FAIL  	ʧ��
 *		Notify Id: NOTIFY_ID_NW_SCAN_FREQ_TIMEOUT  ��ʱ
 */
int NetworkScanFreq(uint32 timeout);


/**
 *	����Ĭ�ϳ����Զ�attach �� �Զ�PDP ����, Ĭ�ϳ���ID Ϊ1
 *
 * @param [in] flag: 1 �����Զ���� 0 �ر��Զ�����
 * @param [in] sim_id: sim��ID, ȡֵ0,1 
 * return: -1, fail, 0 , success
 */
int NetworkSetAutoAttachFlag(uint8_t flag, uint8_t sim_id);

/**
 *	����Ĭ�ϳ����Զ�attach �� �Զ�PDP ����, Ĭ�ϳ���ID Ϊ1
 *
 * @param [in] flag: 1 �����Զ���� 0 �ر��Զ�����
 * @param [in] sim_id: sim��ID, ȡֵ0,1 
 */
uint8_t NetworkGetAutoAttachFlag(uint8_t sim_id);


/**
 *	����SIM ID, ���û������Ĭ��SIM id , ��Ĭ��Ϊ0
 *
 * @param [in] sim_id: sim��ID, ȡֵ0,1 
 */
void NetworkSetSimId(uint8_t sim_id);

/**
 *	��ȡSIM ID
 *
 * @return simid
 */
uint8_t NetworkGetSimId(void);


/**
 *	����Ĭ��SIM ID, δ����Ĭ��Ϊsim 0, ��Ҫ������Ч
 *
 * @param [in] sim_id: sim��ID, ȡֵ0,1 
 */
void NetworkSetDefaultSim(uint8_t sim_id);

/**
 *	��ȡĬ��SIM ID
 *
 * @return simid
 */
uint8_t NetworkGetDefaultSim(void);

/**
 *	ָ������ҵ���sim id, δ������ʹ��NetworkSetSimId()
 *  ��Ҳδ����,��ʹ��Ĭ��sim id
 *
 * @param [in] sim_id: sim��ID, ȡֵ0,1 
 */
void NetworkSetDefaultDataSim(bool enable, uint8_t sim_id);

/**
 *	��ȡ����ҵ��SIM ID
 *
 * @return simid
 */
uint8_t NetworkGetDefaultDataSim(void);

/**
 *	��ȡ��Ӫ������
 * @param [out] info: ��Ӫ��������Ϣ
 * return: 0 :success; others: fail, 
 */
int NetworkGetOperatorName(MC_NW_OPERATOR_INFO_T  *info);


/**
 * Set GPRS Mobile Station Class and modes of operation for EPS, include cs/ps both and ps only mode.  
 * @param [in]: sim_id: sim card ID, range: 0,1 
 * @param [in]: cgclass: cs / ps mode for gsm
 * @param [in]: cemode: cs / ps mode for eps(lte) 
 * return: 0 :success; others: fail, 
 * Note: 1. if the class is not change, will return success soon.
 *       2. it the class is change, the value will be stored in the nv and remain even if power on next time.
 *       3. if the protocal stack is on, will turn off it, and turn on it after change the class automatically.
 *       4. for eps(lte), cemode default value is MC_CE_MODE_PS_CS_BOTH_VOICE_CENTRIC, and suggest set to MC_CE_MODE_PS_ONLY_DATA_CENTRIC for ps only.
 *	   5. if the module is not support gsm, you can ignore the second param cgclass.
 */
int NetworkSetServiceMode(uint8_t nSim, MC_CG_CLASS_E cgclass, MC_CE_MODE_E cemode);

/**
 * Get GPRS Mobile Station Class, include cs/ps both and ps only two modes
 * @param [in]: sim_id: sim card ID, range: 0,1 
 * @param [out]: cgclass: cs / ps mode for gsm
 * @param [out]: cemode: cs / ps mode for eps(lte) 
 * return: 0 :success; others: fail, 
 */
int NetworkGetServiceMode(uint8_t nSim, MC_CG_CLASS_E *cgclass, MC_CE_MODE_E *cemode);

/**
* Get network statue as NetworkGetState()
param[out] pstatus :network status
param[in] nSim : sim id: 0  sim1, 1 sim2
return: 0 susccess, -1 sim absent, -2 pstatus is NULL
**/
int NetworkGetState_Ex(MERCURY_NETWORK_STATUS_T* pstatus, uint8_t nSim);


/**
* FlightModeSet
param[in] enable : true enable flightmode
param[in] nSim : sim id: 0  sim1, 1 sim2
return: 0 susccess
**/
int FlightModeSet_Ex(BOOLEAN enable, uint8_t nSim);

/**
* FlightModeSet
param[in] nSim : sim id: 0  sim1, 1 sim2
return: flightmode
**/
int FlightModeGet_Ex( uint8_t nSim);

/**
 *   ��������ͨѶ�󣬿��ٽ���͹���ʱ�䣬��Χ 1-20s

 param[in] timeout :  unit S
 param[in] nSim : sim id: 0  sim1, 1 sim2
 */
uint32_t NetworkSetRRCRel_Ex(uint32_t timeout, uint8_t nSim);

/**
 *   ��������ͨѶ�󣬿��ٽ���͹���ʱ�䣬��Χ 1-20s
 param[in] nSim : sim id: 0  sim1, 1 sim2
 return rrc release time
 */
uint32_t NetworkGetRRCRel_Ex(uint8_t nSim);


#endif //__MERCURY_NETWORK_H__

