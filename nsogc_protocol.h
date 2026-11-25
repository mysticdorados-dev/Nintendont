/*
 * NSO GameCube controller (Switch 2), USB protocol primitives.
 * VID 057E / PID 2073, USB HID report 0x0A.
 *
 * Preparation for an experimental Nintendont integration.
 * This header DOES NOT install a USB device driver and does not make
 * the controller work by merely placing it in the Nintendont source tree.
 *
 * Decode and initialization derived from the separately hardware-tested
 * NSO GameCube vWii USB probe and SDL3's Switch 2 HIDAPI driver.
 * This module does not perform any firmware or flash writes.
 */
#ifndef NSOGC_PROTOCOL_H
#define NSOGC_PROTOCOL_H

#define NSOGC_USB_VID 0x057Eu
#define NSOGC_USB_PID 0x2073u
#define NSOGC_USB_HID_IN 0x81u
#define NSOGC_USB_VENDOR_OUT 0x02u
#define NSOGC_USB_VENDOR_IN 0x82u
#define NSOGC_USB_REPORT_GC 0x0Au

/* Independent of libc/libogc kernel integer typedefs. */
typedef unsigned char nsogc_u8;
typedef unsigned short nsogc_u16;

typedef struct {
    nsogc_u8 a, b, x, y, start, z;
    nsogc_u8 up, down, left, right;
    nsogc_u8 c, share, home;
    nsogc_u8 left_shoulder, left_click, right_click;
    nsogc_u16 lx, ly, rx, ry;  /* 12-bit axes: center ~ 2048 */
    nsogc_u8 lt, rt;          /* trigger analog bytes */
} nsogc_pad_state;

/* Caller must pass a buffer containing >=15 bytes and verify proper
 * USB transfer completion before passing it to this function.
 * Returns 0 on valid type 0x0A, -1 on malformed/unsupported report.
 */
static inline int nsogc_parse_gc_report(const nsogc_u8 *p,
                                        unsigned length,
                                        nsogc_pad_state *out)
{
    if (!p || !out || length < 15u || p[0] != NSOGC_USB_REPORT_GC) return -1;

    /* Hardware-confirmed mapping on vWii: A/B and X/Y differ from the
       initial upstream probe mapping. */
    out->a = (p[3] & 0x02u) != 0;
    out->b = (p[3] & 0x01u) != 0;
    out->x = (p[3] & 0x08u) != 0;
    out->y = (p[3] & 0x04u) != 0;
    out->right_click = (p[3] & 0x10u) != 0;
    out->z = (p[3] & 0x20u) != 0;
    out->start = (p[3] & 0x40u) != 0;

    out->down = (p[4] & 0x01u) != 0;
    out->right = (p[4] & 0x02u) != 0;
    out->left = (p[4] & 0x04u) != 0;
    out->up = (p[4] & 0x08u) != 0;
    out->left_click = (p[4] & 0x10u) != 0;
    out->left_shoulder = (p[4] & 0x20u) != 0;

    out->home = (p[5] & 0x01u) != 0;
    out->share = (p[5] & 0x02u) != 0;
    out->c = (p[5] & 0x10u) != 0;

    out->lx = (nsogc_u16)(p[6] | ((p[7] & 0x0Fu) << 8));
    out->ly = (nsogc_u16)((p[7] >> 4) | (p[8] << 4));
    out->rx = (nsogc_u16)(p[9] | ((p[10] & 0x0Fu) << 8));
    out->ry = (nsogc_u16)((p[10] >> 4) | (p[11] << 4));
    out->lt = p[13];
    out->rt = p[14];
    return 0;
}

/* Saturating conversion, no overflow, centered 0..255 for GameCube.
 * Does NOT correct stick calibration or choose invert-Y; those belong
 * in the eventual Nintendont-specific mapping layer.
 */
static inline nsogc_u8 nsogc_axis_to_gc_u8(nsogc_u16 raw)
{
    unsigned v = (raw > 4095u) ? 4095u : (unsigned)raw;
    return (nsogc_u8)((v * 255u + 2047u) / 4095u);
}

#endif /* NSOGC_PROTOCOL_H */
