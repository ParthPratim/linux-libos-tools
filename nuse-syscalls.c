/*
 * System call table for NUSE/x86-64, copied from arch/x86/um/syscall_*.c
 */

#include <stdio.h>
#include <sys/types.h>
#define _LINUX_TYPES_H
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>

#include <rumpuser_port.h>
#include <rump/rump.h> /* XXX: for rfork flags */
#include <rump/rumpuser.h>
#include <sys/syscallargs.h>
#include "generated/utsrelease.h"
#include "generated/compile.h"


#define RUMPSERVER_DEFAULT "/tmp/rump-server-nuse"


/* XXX: should be auto-generated by make KBUILD_SRC= ARCH=x86 archheaders */
#if 0
/* #include <asm/syscalls_64.h> */
#undef __SYSCALL_64
#define __SYSCALL_64(nr, sym, compat) [nr] = sym,
#define __SYSCALL_COMMON(nr, sym, compat) __SYSCALL_64(nr, sym, compat)

typedef void (*sys_call_ptr_t)(void);

struct nuse_sys_call_t {
	int syscall_num;
	void *syscall_handler;
};

const struct nuse_sys_call_t sys_call_table[] = {
	{0, read},
	{1, write},
	{2, open},
	{3, close},
	{7, poll},
	{16, ioctl},
	{23, select},
	{41, socket},
	{42, connect},
	{43, accept},
	{44, sendto},
	{45, recvfrom},
	{46, sendmsg},
	{47, recvmsg},
	{49, bind},
	{50, listen},
	{51, getsockname},
	{52, getpeername},
	{54, setsockopt},
	{55, getsockopt},
	{213, epoll_create},
	{232, epoll_wait},
	{233, epoll_ctl},
};
#endif


/* XXX */
//#include <linux/socket.h>
struct user_msghdr {
	void		*msg_name;	/* ptr to socket address structure */
	int		msg_namelen;		/* size of socket address structure */
	struct iovec	*msg_iov;	/* scatter/gather array */
	size_t	msg_iovlen;		/* # elements in msg_iov */
	void		*msg_control;	/* ancillary data */
	size_t	msg_controllen;		/* ancillary data buffer length */
	unsigned int	msg_flags;		/* flags on received message */
};

/* XXX (see rumpuser_sp.c) */
extern __thread struct spclient *th_spc;
struct lwp;			/* dummy */

struct user_msghdr;
struct epoll_event;
typedef unsigned int nfds_t;

int nuse_socket(int domain, int type, int protocol);
int nuse_close(int fd);
ssize_t nuse_recvmsg(int fd, const struct user_msghdr *msghdr, int flags);
ssize_t nuse_sendmsg(int fd, const struct user_msghdr *msghdr, int flags);
int nuse_sendmmsg(int fd, struct mmsghdr *msgvec, unsigned int vlen,
		unsigned int flags);
int nuse_recvmmsg(int fd, struct mmsghdr *msgvec, unsigned int vlen,
		  unsigned int flags, struct timespec *timeout);
int nuse_getsockname(int fd, struct sockaddr *name, socklen_t *namelen);
int nuse_getpeername(int fd, struct sockaddr *name, socklen_t *namelen);
int nuse_bind(int fd, struct sockaddr *name, int namelen);
int nuse_connect(int fd, const struct sockaddr *v1, int v2);
int nuse_listen(int fd, int v1);
int nuse_accept4(int fd, struct sockaddr *addr, int *addrlen, int flags);
int nuse_accept(int fd, struct sockaddr *addr, socklen_t *addrlen);
ssize_t nuse_write(int fd, const void *buf, size_t count);
ssize_t nuse_writev(int fd, const struct iovec *iov, size_t count);
ssize_t nuse_sendto(int fd, const void *buf, size_t len, int flags,
		const struct sockaddr *dest_addr, socklen_t addrlen);
ssize_t nuse_send(int fd, const void *buf, size_t len, int flags);
ssize_t nuse_read(int fd, void *buf, size_t count);
ssize_t nuse_recvfrom(int fd, void *buf, size_t len, int flags,
		struct sockaddr *from, socklen_t *fromlen);
int nuse_recv(int fd, void *buf, size_t count, int flags);
int nuse_setsockopt(int fd, int level, int optname,
		const void *optval, int optlen);
int nuse_getsockopt(int fd, int level, int optname,
		void *optval, unsigned int *optlen);
int open(const char *pathname, int flags, mode_t mode);
int open64(const char *pathname, int flags, mode_t mode);
int nuse_ioctl(int fd, int request, ...);
int poll(struct pollfd *fds, nfds_t nfds, int timeout);
int select(int nfds, fd_set *readfds, fd_set *writefds,
	fd_set *exceptfds, struct timeval *timeout);
int epoll_create(int size);
int epoll_ctl(int epollfd, int op, int fd, struct epoll_event *event);
int epoll_wait(int epollfd, struct epoll_event *events,
			int maxevents, int timeout);
/* XXX */
int nuse_socketpair(int, int, int, int *);


#if	BYTE_ORDER == BIG_ENDIAN
#define	SCARG(p,k)	((p)->k.be.datum)	/* get arg from args pointer */
#elif	BYTE_ORDER == LITTLE_ENDIAN
#define	SCARG(p,k)	((p)->k.le.datum)	/* get arg from args pointer */
#else
#error	"what byte order is this machine?"
#endif


static int nuse_copyin(const void *uaddr, void *kaddr, size_t len)
{
	int error = 0;

	if (__predict_false(uaddr == NULL && len)) {
		return EFAULT;
	}

	if (!th_spc) {
		memcpy(kaddr, uaddr, len);
	} else if (len) {
		error = rumpuser_sp_copyin(th_spc, uaddr, kaddr, len);
	}

	return error;
}

static int nuse_copyout(const void *kaddr, void *uaddr, size_t len)
{
	int error = 0;

	if (__predict_false(uaddr == NULL && len)) {
		return EFAULT;
	}

	if (!th_spc) {
		memcpy(uaddr, kaddr, len);
	}
	else if (len) {
		error = rumpuser_sp_copyout(th_spc, kaddr, uaddr, len);
	}

	return error;
}

static int
sys___socket30(struct lwp *l, const struct sys___socket30_args *uap,
	register_t *retval)
{
	/* {
	   syscallarg(int)	domain;
	   syscallarg(int)	type;
	   syscallarg(int)	protocol;
	   } */
	int error;

	error = nuse_socket(SCARG(uap, domain), SCARG(uap, type),
			SCARG(uap, protocol));
	if (error != -1) {
		*retval = error;
		error = 0;
	}
	return error;
}

static int sys_bind(struct lwp *l, const struct sys_bind_args *uap,
		register_t *retval)
{
	/* {
		syscallarg(int)				s;
		syscallarg(const struct sockaddr *)	name;
		syscallarg(unsigned int)		namelen;
	} */
	int error, len;
	void *name;

	len = SCARG(uap, namelen);
	if (len > 0 && SCARG(uap, name) == NULL)
		return EINVAL;
	name = malloc(len);

	error = nuse_copyin(SCARG(uap, name), name, len);
	if (error)
		goto end;

	error = nuse_bind(SCARG(uap, s), name, len);
	if (error != -1) {
		*retval = error;
		error = 0;
	}

end:
	free(name);
	return error;
}

static int sys_listen(struct lwp *l, const struct sys_listen_args *uap,
		register_t *retval)
{
	/* {
		syscallarg(int)	s;
		syscallarg(int)	backlog;
	} */
	int		error;

	error = nuse_listen(SCARG(uap, s), SCARG(uap, backlog));
	*retval = error;
	return error;
}

static int sys_accept(struct lwp *l, const struct sys_accept_args *uap,
		register_t *retval)
{
	/* {
		syscallarg(int)			s;
		syscallarg(struct sockaddr *)	name;
		syscallarg(unsigned int *)	anamelen;
	} */
	int error;

	error = nuse_accept(SCARG(uap, s), SCARG(uap, name),
			SCARG(uap, anamelen));
	*retval = error;
	return error;
}

static int sys_connect(struct lwp *l, const struct sys_connect_args *uap,
		register_t *retval)
{
	/* {
		syscallarg(int)				s;
		syscallarg(const struct sockaddr *)	name;
		syscallarg(unsigned int)		namelen;
	} */
	int		error;

	error = nuse_connect(SCARG(uap, s), SCARG(uap, name),
			SCARG(uap, namelen));
	*retval = error;
	return error;
}

static int sys_socketpair(struct lwp *l, const struct sys_socketpair_args *uap,
			register_t *retval)
{
	/* {
		syscallarg(int)		domain;
		syscallarg(int)		type;
		syscallarg(int)		protocol;
		syscallarg(int *)	rsv;
	} */

	int error;
	error = nuse_socketpair(SCARG(uap, domain),
				SCARG(uap, type),
				SCARG(uap, protocol),
				SCARG(uap, rsv));
	*retval = error;
	return error;
}

static int sys_sendto(struct lwp *l, const struct sys_sendto_args *uap,
		register_t *retval)
{
	/* {
		syscallarg(int)				s;
		syscallarg(const void *)		buf;
		syscallarg(size_t)			len;
		syscallarg(int)				flags;
		syscallarg(const struct sockaddr *)	to;
		syscallarg(unsigned int)		tolen;
	} */
	int error, len;
	void *buf;
	struct sockaddr *to;

	len = SCARG(uap, len);
	if (len > 0 && SCARG(uap, buf) == NULL)
		return EINVAL;
	buf = malloc(len);
	error = nuse_copyin(SCARG(uap, buf), buf, len);
	if (error)
		goto end;

	len = SCARG(uap, tolen);
	if (len > 0 && SCARG(uap, to) == NULL)
		return EINVAL;
	to = malloc(len);
	error = nuse_copyin(SCARG(uap, to), to, len);
	if (error)
		goto end;

	error = nuse_sendto(SCARG(uap, s),
			buf,
			SCARG(uap, len),
			SCARG(uap, flags),
			to,
			SCARG(uap, tolen));
	if (error != -1) {
		*retval = error;
		error = 0;
	}
end:
	return error;
}

static int sys_sendmsg(struct lwp *l, const struct sys_sendmsg_args *uap,
		register_t *retval)
{
	/* {
		syscallarg(int)				s;
		syscallarg(const struct msghdr *)	msg;
		syscallarg(int)				flags;
	} */
	int error, iovsz, i;
	struct user_msghdr msg;
	struct iovec *iov;
	void *rmsg_name;

	error = nuse_copyin(SCARG(uap, msg), &msg, sizeof(msg));
	if (error)
		return error;
	iovsz = msg.msg_iovlen * sizeof(struct iovec);
	iov = malloc(iovsz);
	error = nuse_copyin(msg.msg_iov, iov, iovsz);
	if (error)
		return error;

	for (i = 0; i < msg.msg_iovlen; i++) {
		void *riov = iov[i].iov_base;
		iov[i].iov_base = malloc(iov[i].iov_len);
		error = nuse_copyin(riov, iov[i].iov_base,
				iov[i].iov_len);
		if (error)
			return error;
	}
	msg.msg_iov = iov;

	rmsg_name = msg.msg_name;
	msg.msg_name = malloc(msg.msg_namelen);
	error = nuse_copyin(rmsg_name, msg.msg_name, msg.msg_namelen);
	if (error)
		return error;

	error = nuse_sendmsg(SCARG(uap, s),
				&msg,
				SCARG(uap, flags));

	if (error != -1) {
		*retval = error;
		error = 0;
	}
	free(iov);
	return error;
}

static int sys_recvfrom(struct lwp *l, const struct sys_recvfrom_args *uap,
			register_t *retval)
{
	/* {
		syscallarg(int)			s;
		syscallarg(void *)		buf;
		syscallarg(size_t)		len;
		syscallarg(int)			flags;
		syscallarg(struct sockaddr *)	from;
		syscallarg(unsigned int *)	fromlenaddr;
	} */
	int error;
	error = nuse_recvfrom(SCARG(uap, s),
			SCARG(uap, buf),
			SCARG(uap, len),
			SCARG(uap, flags),
			SCARG(uap, from),
			SCARG(uap, fromlenaddr));
	*retval = error;
	return error;
}

/* XXX */
struct iov_iter {
	int type;
	size_t iov_offset;
	size_t count;
	union {
		const struct iovec *iov;
		const struct kvec *kvec;
		const struct bio_vec *bvec;
	};
	unsigned long nr_segs;
};

static int sys_recvmsg(struct lwp *l, const struct sys_recvmsg_args *uap,
		register_t *retval)
{
	/* {
		syscallarg(int)			s;
		syscallarg(struct msghdr *)	msg;
		syscallarg(int)			flags;
	} */
	int error, iovsz, i;
	struct user_msghdr msg;
	struct iovec *liov, *riov;

	error = nuse_copyin(SCARG(uap, msg), &msg, sizeof(msg));
	if (error)
		return error;
	iovsz = msg.msg_iovlen * sizeof(struct iovec);
	liov = malloc(iovsz);
	riov = malloc(iovsz);
	msg.msg_name = malloc(msg.msg_namelen);

	error = nuse_copyin(msg.msg_iov, riov, iovsz);
	if (error)
		return error;

	for (i = 0; i < msg.msg_iovlen; i++) {
		liov[i].iov_base = malloc(riov[i].iov_len);
		liov[i].iov_len = riov[i].iov_len;
	}

	msg.msg_iov = liov;
	error = nuse_recvmsg(SCARG(uap, s),
			&msg,
			SCARG(uap, flags));
	if (error != -1) {
		*retval = error;
		error = 0;
	}

	if (error == 0) {
		for (i = 0; i < msg.msg_iovlen; i++) {
			error = nuse_copyout(liov[i].iov_base,
					riov[i].iov_base,
					liov[i].iov_len);
		}
	}

	if (msg.msg_control != NULL)
		error = nuse_copyout(&msg.msg_control,
				(SCARG(uap, msg))
				+ sizeof(void *) + sizeof(int)
				+ sizeof(struct iov_iter) + sizeof(void *),
				msg.msg_controllen);

	/* XXX: no idea how msg_name should copyout to client */
#if 0
	if (error == 0)
		error = rumpuser_sp_copyout(th_spc, &msg.msg_name,
					rmsg_name, msg.msg_namelen);
#endif

	free(riov);
	free(liov);
	free(msg.msg_name);
	return error;
}

static int sys_sendmmsg(struct lwp *l, const struct sys_sendmmsg_args *uap,
			register_t *retval)
{
	/* {
		syscallarg(int)			s;
		syscallarg(struct mmsghdr *)	mmsg;
		syscallarg(unsigned int)	vlen;
		syscallarg(unsigned int)	flags;
	} */
	int error;
	error = nuse_sendmmsg(SCARG(uap, s),
			SCARG(uap, mmsg),
			SCARG(uap, vlen),
			SCARG(uap, flags));
	*retval = error;
	return error;
}

static int sys_recvmmsg(struct lwp *l, const struct sys_recvmmsg_args *uap,
			register_t *retval)
{
	/* {
		syscallarg(int)			s;
		syscallarg(struct mmsghdr *)	mmsg;
		syscallarg(unsigned int)	vlen;
		syscallarg(unsigned int)	flags;
		syscallarg(struct timespec *)	timeout;
	} */
	int error;
	error = nuse_recvmmsg(SCARG(uap, s),
			SCARG(uap, mmsg),
			SCARG(uap, vlen),
			SCARG(uap, flags),
			SCARG(uap, timeout));
	*retval = error;
	return error;
}

#if 0
int
sys_shutdown(struct lwp *l, const struct sys_shutdown_args *uap,
    register_t *retval)
{
	/* {
		syscallarg(int)	s;
		syscallarg(int)	how;
	} */
	struct socket	*so;
	int		error;

	error = fd_getsock(SCARG(uap, s), &so);
	if (error != 0)
		return error;
	solock(so);
	error = soshutdown(so, SCARG(uap, how));
	sounlock(so);
	fd_putfile(SCARG(uap, s));
	return error;
}
#endif

static int sys_setsockopt(struct lwp *l, const struct sys_setsockopt_args *uap,
			register_t *retval)
{
	/* {
		syscallarg(int)			s;
		syscallarg(int)			level;
		syscallarg(int)			name;
		syscallarg(const void *)	val;
		syscallarg(unsigned int)	valsize;
	} */
	int error, len;
	void *val;

	len = SCARG(uap, valsize);
	if (len > 0 && SCARG(uap, val) == NULL)
		return EINVAL;
	val = malloc(len);

	error = nuse_copyin(SCARG(uap, val), val, len);
	if (error)
		goto end;

	error = nuse_setsockopt(SCARG(uap, s),
				SCARG(uap, level),
				SCARG(uap, name),
				val,
				SCARG(uap, valsize));
end:
	*retval = error;
	free(val);
	return error;
}

static int sys_getsockopt(struct lwp *l, const struct sys_getsockopt_args *uap,
			register_t *retval)
{
	/* {
		syscallarg(int)			s;
		syscallarg(int)			level;
		syscallarg(int)			name;
		syscallarg(void *)		val;
		syscallarg(unsigned int *)	avalsize;
	} */
	int error;
	unsigned int valsize;
	void *data = NULL;

	if (SCARG(uap, val) != NULL) {
		error = nuse_copyin(SCARG(uap, avalsize), &valsize, sizeof(valsize));
		if (error)
			return error;
		data = malloc(valsize);
	} else
		valsize = 0;

	error = nuse_getsockopt(SCARG(uap, s),
					SCARG(uap, level),
					SCARG(uap, name),
					data,
					&valsize);

	if (valsize > 0) {
		error = nuse_copyout(data, SCARG(uap, val), valsize);
		if (error)
			goto out;

		error = nuse_copyout(&valsize, SCARG(uap, avalsize), sizeof(valsize));
		if (error)
			goto out;
	}

out:
	free(data);
	*retval = error;
	return error;
}

static int sys_getsockname(struct lwp *l, const struct sys_getsockname_args *uap,
			register_t *retval)
{
	/* {
		syscallarg(int)			fdes;
		syscallarg(struct sockaddr *)	asa;
		syscallarg(unsigned int *)	alen;
	} */
	int error;
	socklen_t valsize;
	struct sockaddr *data;

	if (SCARG(uap, asa) != NULL) {
		error = nuse_copyin(SCARG(uap, alen), &valsize, sizeof(valsize));
		if (error)
			return error;
		data = malloc(valsize);
	} else
		valsize = 0;

	error = nuse_getsockname(SCARG(uap, fdes),
				data,
				&valsize);

	if (valsize > 0) {
		error = nuse_copyout(data, SCARG(uap, asa), valsize);
		if (error)
			goto out;

		error = nuse_copyout(&valsize, SCARG(uap, alen), sizeof(valsize));
		if (error)
			goto out;
	}

out:
	free(data);
	*retval = error;
	return error;
}

/*
 * Get name of peer for connected socket.
 */
static int sys_getpeername(struct lwp *l, const struct sys_getpeername_args *uap,
			register_t *retval)
{
	/* {
		syscallarg(int)			fdes;
		syscallarg(struct sockaddr *)	asa;
		syscallarg(unsigned int *)	alen;
	} */
	int error;
	error = nuse_getpeername(SCARG(uap, fdes),
				SCARG(uap, asa),
				SCARG(uap, alen));
	*retval = error;
	return error;
}

/* XXX:XXX */
#define IFREQ_SIZE 40
static int sys_ioctl(struct lwp *l, const struct sys_ioctl_args *uap,
		register_t *retval)
{
	/* {
		syscallarg(int)		fd;
		syscallarg(u_long)	com;
		syscallarg(void *)	data;
	} */
	int error;
	void *data = NULL;

	/* if-level ioctl */
	if (SCARG(uap, com) >= SIOCGIFNAME) {
		data = malloc(IFREQ_SIZE);
		error = nuse_copyin(SCARG(uap, data), data, IFREQ_SIZE);
		if (error)
			return error;

		error = nuse_ioctl(SCARG(uap, fd),
					SCARG(uap, com),
					data);
		if (error != -1) {
			*retval = error;
			error = 0;
		}

		error = nuse_copyout(data, SCARG(uap, data), IFREQ_SIZE);
	} else {
		error = nuse_ioctl(SCARG(uap, fd),
					SCARG(uap, com),
					SCARG(uap, data));
		if (error != -1) {
			*retval = error;
			error = 0;
		}

	}

	return error;
}

typedef int	sy_call_t(struct lwp *, const void *, register_t *);
struct rump_onesyscall {
	int ros_num;
	sy_call_t *ros_handler;
};
static const struct rump_onesyscall mysys[] = {
	/* { 6,	(sy_call_t *)sys_close }, */
	{ 27,	(sy_call_t *)sys_recvmsg },
	{ 28,	(sy_call_t *)sys_sendmsg },
	{ 29,	(sy_call_t *)sys_recvfrom },
	{ 30,	(sy_call_t *)sys_accept },
	{ 31,	(sy_call_t *)sys_getpeername },
	{ 32,	(sy_call_t *)sys_getsockname },
	{ 54,   (sy_call_t *)sys_ioctl },
	{ 98,	(sy_call_t *)sys_connect },
	{ 104,	(sy_call_t *)sys_bind },
	{ 105,	(sy_call_t *)sys_setsockopt },
	{ 106,	(sy_call_t *)sys_listen },
	{ 118,	(sy_call_t *)sys_getsockopt },
	{ 133,	(sy_call_t *)sys_sendto },
	/* { 134,	(sy_call_t *)sys_shutdown }, */
	{ 135,	(sy_call_t *)sys_socketpair },
	{ 394,	(sy_call_t *)sys___socket30 },
	/* { 456,	(sy_call_t *)sys_paccept }, */
	{ 475,	(sy_call_t *)sys_recvmmsg },
	{ 476,	(sy_call_t *)sys_sendmmsg },
};

static int nuse_hyp_syscall(int num, void *arg, long *retval)
{
	int rv, i;
	register_t regrv[2] = {0, 0};
	sy_call_t *syscall = NULL;
	struct lwp *l = NULL;

	/* XXX */
	for (i = 0; i < sizeof(mysys) / sizeof(mysys[0]) ; i++) {
		if (mysys[i].ros_num == num) {
			syscall = mysys[i].ros_handler;
			break;
		}
	}
	if (!syscall) {
		retval[0] = -1;
		return -1;
	}

	rv = syscall(l, (void *)arg, regrv);
	retval[0] = regrv[0];
	retval[1] = regrv[1];
	return rv;
}

int rumpclient_syscall(int num, void *data, size_t dlen, register_t *retval)
{
	return nuse_hyp_syscall(num, data, retval);
}

static void nuse_schedule(void){}
static void nuse_unschedule(void){}
static void nuse_user_unschedule(int nlocks, int *countp, void *interlock){}
static void nuse_user_schedule(int nlocks, void *interlock){}
static int nuse_lwproc_rfork(void *priv, int flags, const char *comm){return 0;}
static void nuse_lwproc_switch(struct lwp *newlwp){}
static void nuse_lwproc_release(void){}
static int nuse_lwproc_newlwp(pid_t pid){return 0;}
static struct lwp *nuse_lwproc_curlwp(void){return NULL;}
static void nuse_hyp_lwpexit(void){}
static void nuse_hyp_execnotify(const char *comm){}

static const struct rumpuser_hyperup nuse_hyp = {
	.hyp_schedule		= nuse_schedule,
	.hyp_unschedule		= nuse_unschedule,
	.hyp_backend_unschedule	= nuse_user_unschedule,
	.hyp_backend_schedule	= nuse_user_schedule,
	.hyp_lwproc_switch	= nuse_lwproc_switch,
	.hyp_lwproc_release	= nuse_lwproc_release,
	.hyp_lwproc_rfork	= nuse_lwproc_rfork,
	.hyp_lwproc_newlwp	= nuse_lwproc_newlwp,
	.hyp_lwproc_curlwp	= nuse_lwproc_curlwp,
	.hyp_lwpexit		= nuse_hyp_lwpexit,
	.hyp_syscall		= nuse_hyp_syscall,
	.hyp_execnotify		= nuse_hyp_execnotify,
	.hyp_getpid		= getpid,
};

void
nuse_syscall_proxy_init(void)
{
	char *url;
	char buf[64];

	url = getenv("RUMP_SERVER");
	if (!url) {
		sprintf(buf, "unix://%s.%d", RUMPSERVER_DEFAULT, getpid());
		url = strdup(buf);
	}

	if (rumpuser_init(RUMPUSER_VERSION, &nuse_hyp) != 0) {
		rumpuser_dprintf("rumpuser init failed\n");
		return /* EINVAL */;
	}
	umask (0007);
	rumpuser_sp_init(url, "Linux", UTS_RELEASE, UTS_MACHINE);
	rumpuser_dprintf("nuse syscall proxy start at %s\n", url);
}

void
nuse_syscall_proxy_exit(void)
{
	rumpuser_dprintf("rump_server finishing.\n");
	rumpuser_sp_fini(NULL);
}
