typedef int (*orig_getaddrinfo_type)(const char*, const char*, const struct addrinfo*, struct addrinfo**);
extern "C" int getaddrinfo(const char* node, const char* service,
const struct addrinfo* hints, struct addrinfo** res) {
static orig_getaddrinfo_type orig_getaddrinfo = (orig_getaddrinfo_type)dlsym(RTLD_NEXT, "getaddrinfo");
// START HOST
// (1)
if (node && strcmp(node, "localhost") == 0) {
printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (2)
if (node && strcmp(node, "ip6-localhost") == 0) {
printf("[HOOK] Redirecting %s to ::1\n", node);
return orig_getaddrinfo("::1", service, hints, res);
}
// (3)
if (node && strcmp(node, "down.anticheatexpert.com") == 0) {
printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (4)
if (node && strcmp(node, "sdksp.video.qq.com") == 0) {
printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (5)
if (node && strcmp(node, "sq2.qq.com") == 0) {
printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (6)
if (node && strcmp(node, "sq1.qq.com") == 0) {
printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (7)
if (node && strcmp(node, "sqlobby.qq.com") == 0) {
printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (8)
if (node && strcmp(node, "speed.ino.qq.com") == 0) {
printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (9)
if (node && strcmp(node, "sq3.qq.com") == 0) {
printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (10)
if (node && strcmp(node, "sdksp.video.qq.com") == 0) {
printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (11)
if (node && strcmp(node, "sqlobby.pg.com7") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (12)
if (node && strcmp(node, "spinfo.wps.cn") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (13)
if (node && strcmp(node, "sp.gmossp-sp.jp") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (14)
if (node && strcmp(node, "ssp.cibn.starschina.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (15)
if (node && strcmp(node, "sp3.cndm.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (16)
if (node && strcmp(node, "sspapi.youxiaoad.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (17)
if (node && strcmp(node, "sp.fastclick.net") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (18)
if (node && strcmp(node, "sq9.pg.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (19)
if (node && strcmp(node, "sqlobby.pg.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (20)
if (node && strcmp(node, "speed.ino.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (21)
if (node && strcmp(node, "sqlobby.pg.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (22)
if (node && strcmp(node, "sdksp.video.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (23)
if (node && strcmp(node, "c.isdspeed.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (24)
if (node && strcmp(node, "sqlobby.pg.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (25)
if (node && strcmp(node, "sq9.pg.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (26)
if (node && strcmp(node, "sqlobby.pg.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (27)
if (node && strcmp(node, "pingma.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (28)
if (node && strcmp(node, "sqimg.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (29)
if (node && strcmp(node, "sq11.pg.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (30)
if (node && strcmp(node, "sq12.pg.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (31)
if (node && strcmp(node, "sq2.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 1.0.1.0\n", node);
    return orig_getaddrinfo("1.0.1.0", service, hints, res);
}
// (32)
if (node && strcmp(node, "cloud.gsdk.proximabeta.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (33)
if (node && strcmp(node, "pingma.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (34)
if (node && strcmp(node, "182.254.116.117") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (35)
if (node && strcmp(node, "file.igamecj.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (36)
if (node && strcmp(node, "vmp.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (37)
if (node && strcmp(node, "dlied1.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (38)
if (node && strcmp(node, "119.167.164.85") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (39)
if (node && strcmp(node, "down.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (40)
if (node && strcmp(node, "tencentgames.helpshift.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (41)
if (node && strcmp(node, "140.207.69.43") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (42)
if (node && strcmp(node, "idcconcig.gcloud.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 1.0.1.0\n", node);
    return orig_getaddrinfo("1.0.1.0", service, hints, res);
}
// (43)
if (node && strcmp(node, "pay.igamecj.com") == 0) {
    printf("[HOOK] Redirecting %s to 1.2.2.0\n", node);
    return orig_getaddrinfo("1.2.2.0", service, hints, res);
}
// (44)
if (node && strcmp(node, "szmg.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (45)
if (node && strcmp(node, "android.bugly.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 1.0.1.0\n", node);
    return orig_getaddrinfo("1.0.1.0", service, hints, res);
}
// (46)
if (node && strcmp(node, "140.207.127.155") == 0) {
    printf("[HOOK] Redirecting %s to 1.2.2.0\n", node);
    return orig_getaddrinfo("1.2.2.0", service, hints, res);
}
// (47)
if (node && strcmp(node, "140.207.123.184") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (48)
if (node && strcmp(node, "file-igamecj.akamaized.net") == 0) {
    printf("[HOOK] Redirecting %s to 1.0.1.0\n", node);
    return orig_getaddrinfo("1.0.1.0", service, hints, res);
}
// (49)
if (node && strcmp(node, "app.adjust.com") == 0) {
    printf("[HOOK] Redirecting %s to 1.2.2.0\n", node);
    return orig_getaddrinfo("1.2.2.0", service, hints, res);
}
// (50)
if (node && strcmp(node, "sq1.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (51)
if (node && strcmp(node, "sqlobby.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 1.0.1.0\n", node);
    return orig_getaddrinfo("1.0.1.0", service, hints, res);
}
// (52)
if (node && strcmp(node, "speed.ino.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 1.2.2.0\n", node);
    return orig_getaddrinfo("1.2.2.0", service, hints, res);
}
// (53)
if (node && strcmp(node, "sq3.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (54)
if (node && strcmp(node, "sdksp.video.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 1.0.1.0\n", node);
    return orig_getaddrinfo("1.0.1.0", service, hints, res);
}
// (55)
if (node && strcmp(node, "sqlobby.pg.com7") == 0) {
    printf("[HOOK] Redirecting %s to 1.2.2.0\n", node);
    return orig_getaddrinfo("1.2.2.0", service, hints, res);
}
// (56)
if (node && strcmp(node, "spinfo.wps.cn") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (57)
if (node && strcmp(node, "sp.gmossp-sp.jp") == 0) {
    printf("[HOOK] Redirecting %s to 1.0.1.0\n", node);
    return orig_getaddrinfo("1.0.1.0", service, hints, res);
}
// (58)
if (node && strcmp(node, "ssp.cibn.starschina.com") == 0) {
    printf("[HOOK] Redirecting %s to 1.2.2.0\n", node);
    return orig_getaddrinfo("1.2.2.0", service, hints, res);
}
// (59)
if (node && strcmp(node, "sp3.cndm.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (60)
if (node && strcmp(node, "sspapi.youxiaoad.com") == 0) {
    printf("[HOOK] Redirecting %s to 1.0.1.0\n", node);
    return orig_getaddrinfo("1.0.1.0", service, hints, res);
}
// (61)
if (node && strcmp(node, "sp.fastclick.net") == 0) {
    printf("[HOOK] Redirecting %s to 1.2.2.0\n", node);
    return orig_getaddrinfo("1.2.2.0", service, hints, res);
}
// (62)
if (node && strcmp(node, "sq9.pg.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (63)
if (node && strcmp(node, "sqlobby.pg.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 1.0.1.0\n", node);
    return orig_getaddrinfo("1.0.1.0", service, hints, res);
}
// (64)
if (node && strcmp(node, "speed.ino.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 1.2.2.0\n", node);
    return orig_getaddrinfo("1.2.2.0", service, hints, res);
}
// (65)
if (node && strcmp(node, "sqlobby.pg.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (66)
if (node && strcmp(node, "sdksp.video.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 1.0.1.0\n", node);
    return orig_getaddrinfo("1.0.1.0", service, hints, res);
}
// (67)
if (node && strcmp(node, "c.isdspeed.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 1.2.2.0\n", node);
    return orig_getaddrinfo("1.2.2.0", service, hints, res);
}
// (68)
if (node && strcmp(node, "sqlobby.pg.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (69)
if (node && strcmp(node, "sq9.pg.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 1.0.1.0\n", node);
    return orig_getaddrinfo("1.0.1.0", service, hints, res);
}
// (70)
if (node && strcmp(node, "sqlobby.pg.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 1.2.2.0\n", node);
    return orig_getaddrinfo("1.2.2.0", service, hints, res);
}
// (71)
if (node && strcmp(node, "pingma.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (72)
if (node && strcmp(node, "sqimg.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 1.0.1.0\n", node);
    return orig_getaddrinfo("1.0.1.0", service, hints, res);
}
// (73)
if (node && strcmp(node, "sq11.pg.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 1.2.2.0\n", node);
    return orig_getaddrinfo("1.2.2.0", service, hints, res);
}
// (74)
if (node && strcmp(node, "sq12.pg.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (75)
if (node && strcmp(node, "sq2.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 1.0.1.0\n", node);
    return orig_getaddrinfo("1.0.1.0", service, hints, res);
}
// (76)
if (node && strcmp(node, "sq1.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 1.2.2.0\n", node);
    return orig_getaddrinfo("1.2.2.0", service, hints, res);
}
// (77)
if (node && strcmp(node, "sqlobby.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (78)
if (node && strcmp(node, "speed.ino.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 1.0.1.0\n", node);
    return orig_getaddrinfo("1.0.1.0", service, hints, res);
}
// (79)
if (node && strcmp(node, "sq3.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 1.2.2.0\n", node);
    return orig_getaddrinfo("1.2.2.0", service, hints, res);
}
// (80)
if (node && strcmp(node, "sdksp.video.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (81)
if (node && strcmp(node, "sqlobby.pg.com7") == 0) {
    printf("[HOOK] Redirecting %s to 1.0.1.0\n", node);
    return orig_getaddrinfo("1.0.1.0", service, hints, res);
}
// (82)
if (node && strcmp(node, "spinfo.wps.cn") == 0) {
    printf("[HOOK] Redirecting %s to 1.2.2.0\n", node);
    return orig_getaddrinfo("1.2.2.0", service, hints, res);
}
// (83)
if (node && strcmp(node, "sp.gmossp-sp.jp") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (84)
if (node && strcmp(node, "ssp.cibn.starschina.com") == 0) {
    printf("[HOOK] Redirecting %s to 1.0.1.0\n", node);
    return orig_getaddrinfo("1.0.1.0", service, hints, res);
}
// (85)
if (node && strcmp(node, "sp3.cndm.com") == 0) {
    printf("[HOOK] Redirecting %s to 1.2.2.0\n", node);
    return orig_getaddrinfo("1.2.2.0", service, hints, res);
}
// (86)
if (node && strcmp(node, "sspapi.youxiaoad.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (87)
if (node && strcmp(node, "sp.fastclick.net") == 0) {
    printf("[HOOK] Redirecting %s to 1.0.1.0\n", node);
    return orig_getaddrinfo("1.0.1.0", service, hints, res);
}
// (88)
if (node && strcmp(node, "sq9.pg.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 1.2.2.0\n", node);
    return orig_getaddrinfo("1.2.2.0", service, hints, res);
}
// (89)
if (node && strcmp(node, "sqlobby.pg.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (90)
if (node && strcmp(node, "speed.ino.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 1.0.1.0\n", node);
    return orig_getaddrinfo("1.0.1.0", service, hints, res);
}
// (91)
if (node && strcmp(node, "sqlobby.pg.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (92)
if (node && strcmp(node, "sdksp.video.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 1.2.2.0\n", node);
    return orig_getaddrinfo("1.2.2.0", service, hints, res);
}
// (93)
if (node && strcmp(node, "c.isdspeed.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (94)
if (node && strcmp(node, "sqlobby.pg.com") == 0) {
    printf("[HOOK] Redirecting %s to 1.0.1.0\n", node);
    return orig_getaddrinfo("1.0.1.0", service, hints, res);
}
// (95)
if (node && strcmp(node, "sq9.pg.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 1.2.2.0\n", node);
    return orig_getaddrinfo("1.2.2.0", service, hints, res);
}
// (96)
if (node && strcmp(node, "sqlobby.pg.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (97)
if (node && strcmp(node, "sqlobby.pg.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 1.0.1.0\n", node);
    return orig_getaddrinfo("1.0.1.0", service, hints, res);
}
// (98)
if (node && strcmp(node, "pingma.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 1.2.2.0\n", node);
    return orig_getaddrinfo("1.2.2.0", service, hints, res);
}
// (99)
if (node && strcmp(node, "sqimg.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}
// (100)
if (node && strcmp(node, "sq11.pg.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 1.0.1.0\n", node);
    return orig_getaddrinfo("1.0.1.0", service, hints, res);
}

if (node && strcmp(node, "clientreport.gfm.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}

if (node && strcmp(node, "banner.alimama.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}

// SERVER 

// @kero_200
if (node && strcmp(node, "ig-us-sdkapi.igamecj.com") == 0) {
    printf("[HOOK] Redirecting %s to 255.255.255.255\n", node);
    return orig_getaddrinfo("255.255.255.255", service, hints, res);
}


if (node && strcmp(node, "ig-us-sdkapi.igamecj.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}


if (node && strcmp(node, "lobby.igamecj.com") == 0) {
    printf("[HOOK] Redirecting %s to 255.255.255.255\n", node);
    return orig_getaddrinfo("255.255.255.255", service, hints, res);
}


if (node && strcmp(node, "lobby.igamecj.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}


if (node && strcmp(node, "lobby.igamecj.com") == 0) {
    printf("[HOOK] Redirecting %s to 58.240.57.33\n", node);
    return orig_getaddrinfo("58.240.57.33", service, hints, res);
}


if (node && strcmp(node, "lobby.igamecj.com") == 0) {
    printf("[HOOK] Redirecting %s to 10.147.203.158\n", node);
    return orig_getaddrinfo("10.147.203.158", service, hints, res);
}


if (node && strcmp(node, "krlobby.igamecj.com") == 0) {
    printf("[HOOK] Redirecting %s to 255.255.255.255\n", node);
    return orig_getaddrinfo("255.255.255.255", service, hints, res);
}


if (node && strcmp(node, "krlobby.igamecj.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}

if (node && strcmp(node, "krlobby.igamecj.com") == 0) {
    printf("[HOOK] Redirecting %s to 218.168.124.255\n", node);
    return orig_getaddrinfo("218.168.124.255", service, hints, res);
}


if (node && strcmp(node, "krlobby.igamecj.com") == 0) {
    printf("[HOOK] Redirecting %s to 104.194.212.19\n", node);
    return orig_getaddrinfo("104.194.212.19", service, hints, res);
}

if (node && strcmp(node, "krlobby.igamecj.com") == 0) {
    printf("[HOOK] Redirecting %s to 106.61.26.249\n", node);
    return orig_getaddrinfo("106.61.26.249", service, hints, res);
}


if (node && strcmp(node, "file.igamecj.com") == 0) {
    printf("[HOOK] Redirecting %s to 255.255.255.255\n", node);
    return orig_getaddrinfo("255.255.255.255", service, hints, res);
}


if (node && strcmp(node, "file.igamecj.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}

//====================================================
if (node && strcmp(node, "graph.facebook.com") == 0) {
    printf("[HOOK] Redirecting %s to 157.240.15.1\n", node);
    return orig_getaddrinfo("157.240.15.1", service, hints, res);
}

if (node && strcmp(node, "g83naxx1ena.gph.easebar.com") == 0) {
    printf("[HOOK] Redirecting %s to 104.84.150.151\n", node);
    return orig_getaddrinfo("104.84.150.151", service, hints, res);
}

if (node && strcmp(node, "g83naxx1ena.gph.easebar.com") == 0) {
    printf("[HOOK] Redirecting %s to 104.84.150.164\n", node);
    return orig_getaddrinfo("104.84.150.164", service, hints, res);
}

if (node && strcmp(node, "firebaselogging.googleapis.com") == 0) {
    printf("[HOOK] Redirecting %s to 74.125.200.95\n", node);
    return orig_getaddrinfo("74.125.200.95", service, hints, res);
}

if (node && strcmp(node, "pubsub.googleapis.com") == 0) {
    printf("[HOOK] Redirecting %s to 74.125.200.95\n", node);
    return orig_getaddrinfo("74.125.200.95", service, hints, res);
}

if (node && strcmp(node, "firebaseinstallations.googleapis.com") == 0) {
    printf("[HOOK] Redirecting %s to 74.125.200.95\n", node);
    return orig_getaddrinfo("74.125.200.95", service, hints, res);
}

if (node && strcmp(node, "c5ipmc-launches.appsflyersdk.com") == 0) {
    printf("[HOOK] Redirecting %s to 54.192.16.11\n", node);
    return orig_getaddrinfo("54.192.16.11", service, hints, res);
}

if (node && strcmp(node, "drpf-g83naxx1ena.proxima.nie.easebar.com") == 0) {
    printf("[HOOK] Redirecting %s to 8.211.97.138\n", node);
    return orig_getaddrinfo("8.211.97.138", service, hints, res);
}

if (node && strcmp(node, "unisdk.proxima.nie.easebar.com") == 0) {
    printf("[HOOK] Redirecting %s to 8.211.97.138\n", node);
    return orig_getaddrinfo("8.211.97.138", service, hints, res);
}

if (node && strcmp(node, "who.easebar.com") == 0) {
    printf("[HOOK] Redirecting %s to 52.28.45.149\n", node);
    return orig_getaddrinfo("52.28.45.149", service, hints, res);
}

if (node && strcmp(node, "who.nie.easebar.com") == 0) {
    printf("[HOOK] Redirecting %s to 52.28.45.149\n", node);
    return orig_getaddrinfo("52.28.45.149", service, hints, res);
}

//============================°°°°

if (node && strcmp(node, "file.igamecj.com") == 0) {
    printf("[HOOK] Redirecting %s to 218.168.124.255\n", node);
    return orig_getaddrinfo("218.168.124.255", service, hints, res);
}


if (node && strcmp(node, "file.igamecj.com") == 0) {
    printf("[HOOK] Redirecting %s to 104.194.212.19\n", node);
    return orig_getaddrinfo("104.194.212.19", service, hints, res);
}

if (node && strcmp(node, "file.igamecj.com") == 0) {
    printf("[HOOK] Redirecting %s to 106.61.26.249\n", node);
    return orig_getaddrinfo("106.61.26.249", service, hints, res);
}

if (node && strcmp(node, "pubg.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 192.168.0.100\n", node);
    return orig_getaddrinfo("192.168.0.100", service, hints, res);
}


if (node && strcmp(node, "hy.pubgm.qq.com") == 0) {
    printf("[HOOK] Redirecting %s to 106.61.26.249\n", node);
    return orig_getaddrinfo("106.61.26.249", service, hints, res);
}

if (node && strcmp(node, "com.netease.newspike") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}

if (node && strcmp(node, "com.netease.newspike") == 0) {
    printf("[HOOK] Redirecting %s to 104.194.212.19\n", node);
    return orig_getaddrinfo("104.194.212.19", service, hints, res);
}

if (node && strcmp(node, "com.netease.newspike") == 0) {
    printf("[HOOK] Redirecting %s to 218.168.124.255\n", node);
    return orig_getaddrinfo("218.168.124.255", service, hints, res);
}

// Bloodstrike IPs
if (node && strcmp(node, "bloodstrike.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}

if (node && strcmp(node, "api.bloodstrike.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}

if (node && strcmp(node, "matchmaker.bloodstrike.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}

if (node && strcmp(node, "cdn.bloodstrike.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}

if (node && strcmp(node, "stats.bloodstrike.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}

if (node && strcmp(node, "auth.bloodstrike.com") == 0) {
    printf("[HOOK] Redirecting %s to 127.0.0.1\n", node);
    return orig_getaddrinfo("127.0.0.1", service, hints, res);
}

return orig_getaddrinfo(node, service, hints, res);
}