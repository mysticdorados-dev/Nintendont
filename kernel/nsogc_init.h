/*
 * Nintendo GameCube NSO Switch 2 Controller (USB 057e:2073)
 * Initialization command definitions for Nintendont integration.
 *
 * These commands were used by the separately tested Wii/vWii
 * nsogc_probe. The actual USB transport must still be integrated into
 * Nintendont's ARM kernel; adding this header alone does not enable input.
 *
 * No controller firmware/flash writes are performed by these packets.
 */
#ifndef NINTENDONT_NSOGC_INIT_H
#define NINTENDONT_NSOGC_INIT_H

/* Struct and commands are header-private to avoid adding source files to
 * the Nintendont ARM kernel's build rules.  All arrays are <= 28 bytes. */
typedef struct {
    const unsigned char *bytes;
    unsigned int length;
    const char *name;
} nsogc_init_cmd;

static const unsigned char nsogc_cmd0[] = { 0x07,0x91,0x00,0x01,0x00,0x00,0x00,0x00 };
static const unsigned char nsogc_cmd1[] = { 0x0c,0x91,0x00,0x02,0x00,0x04,0x00,0x00,0x27,0x00,0x00,0x00 };
static const unsigned char nsogc_cmd2[] = { 0x11,0x91,0x00,0x01,0x00,0x00,0x00,0x00 };
static const unsigned char nsogc_cmd3[] = { 0x0a,0x91,0x00,0x08,0x00,0x14,0x00,0x00,
    0x01,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x35,0x00,0x46,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
static const unsigned char nsogc_cmd4[] = { 0x0c,0x91,0x00,0x04,0x00,0x04,0x00,0x00,0x27,0x00,0x00,0x00 };
static const unsigned char nsogc_cmd5[] = { 0x01,0x91,0x00,0x0c,0x00,0x00,0x00,0x00 };
static const unsigned char nsogc_cmd6[] = { 0x01,0x91,0x00,0x01,0x00,0x00,0x00,0x00 };
static const unsigned char nsogc_cmd7[] = { 0x08,0x91,0x00,0x02,0x00,0x04,0x00,0x00,0x01,0x00,0x00,0x00 };
static const unsigned char nsogc_cmd8[] = { 0x03,0x91,0x00,0x0a,0x00,0x04,0x00,0x00,0x0a,0x00,0x00,0x00 };
static const unsigned char nsogc_cmd9[] = { 0x03,0x91,0x00,0x0d,0x00,0x08,0x00,0x00,
    0x01,0x00,0xff,0xff,0xff,0xff,0xff,0xff };

#define NSOGC_CMD_ENTRY(n, label) {nsogc_cmd##n, sizeof(nsogc_cmd##n), label}
static const nsogc_init_cmd nsogc_init_sequence[] = {
    NSOGC_CMD_ENTRY(0, "Handshake"),
    NSOGC_CMD_ENTRY(1, "Feature mask"),
    NSOGC_CMD_ENTRY(2, "Handshake 2"),
    NSOGC_CMD_ENTRY(3, "Rumble setup"),
    NSOGC_CMD_ENTRY(4, "Enable features"),
    NSOGC_CMD_ENTRY(5, "Unknown init"),
    NSOGC_CMD_ENTRY(6, "Enable rumble"),
    NSOGC_CMD_ENTRY(7, "Grip buttons"),
    NSOGC_CMD_ENTRY(8, "GC report 0x0A"),
    NSOGC_CMD_ENTRY(9, "Start reports")
};
#undef NSOGC_CMD_ENTRY
#define NSOGC_INIT_COUNT (sizeof(nsogc_init_sequence)/sizeof(nsogc_init_sequence[0]))

#endif /* NINTENDONT_NSOGC_INIT_H */
