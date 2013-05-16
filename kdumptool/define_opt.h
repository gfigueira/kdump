/**
 * This file is included from different places with different
 * definitions of DEFINE_OPT, which should be a macro that takes
 * four arguments:
 *
 *      @name    option name (C identifier)
 *      @type    data type (String, Int, Bool)
 *      @defval  default value
 *      @usage   option usage flags (MKINITRD, KEXEC, DUMP)
 */

DEFINE_OPT(KDUMP_KERNELVER, String, "", KEXEC)
DEFINE_OPT(KDUMP_CPUS, Int, 1, KEXEC | DUMP)
DEFINE_OPT(KDUMP_COMMANDLINE, String, "", KEXEC)
DEFINE_OPT(KDUMP_COMMANDLINE_APPEND, String, "", KEXEC)
#if HAVE_FADUMP
DEFINE_OPT(KDUMP_FADUMP, Bool, false, MKINITRD)
#endif
DEFINE_OPT(KEXEC_OPTIONS, String, "", KEXEC)
DEFINE_OPT(MAKEDUMPFILE_OPTIONS, String, "", DUMP)
DEFINE_OPT(KDUMP_IMMEDIATE_REBOOT, Bool, true, DUMP)
DEFINE_OPT(KDUMP_TRANSFER, String, "", DUMP)
DEFINE_OPT(KDUMP_SAVEDIR, String, "/var/log/dump", MKINITRD | DUMP)
DEFINE_OPT(KDUMP_KEEP_OLD_DUMPS, Int, 0, DUMP)
DEFINE_OPT(KDUMP_FREE_DISK_SIZE, Int, 64, DUMP)
DEFINE_OPT(KDUMP_VERBOSE, Int, 0, KEXEC | DUMP)
DEFINE_OPT(KDUMP_DUMPLEVEL, Int, 31, DUMP)
DEFINE_OPT(KDUMP_DUMPFORMAT, String, "compressed", DUMP)
DEFINE_OPT(KDUMP_CONTINUE_ON_ERROR, Bool, true, DUMP)
DEFINE_OPT(KDUMP_REQUIRED_PROGRAMS, String, "", MKINITRD)
DEFINE_OPT(KDUMP_PRESCRIPT, String, "", DUMP)
DEFINE_OPT(KDUMP_POSTSCRIPT, String, "", DUMP)
DEFINE_OPT(KDUMP_COPY_KERNEL, Bool, "", DUMP)
DEFINE_OPT(KDUMPTOOL_FLAGS, String, "", DUMP)
DEFINE_OPT(KDUMP_NETCONFIG, String, "auto", MKINITRD)
DEFINE_OPT(KDUMP_SMTP_SERVER, String, "", DUMP)
DEFINE_OPT(KDUMP_SMTP_USER, String, "", DUMP)
DEFINE_OPT(KDUMP_SMTP_PASSWORD, String, "", DUMP)
DEFINE_OPT(KDUMP_NOTIFICATION_TO, String, "", DUMP)
DEFINE_OPT(KDUMP_NOTIFICATION_CC, String, "", DUMP)
DEFINE_OPT(KDUMP_HOST_KEY, String, "", DUMP)