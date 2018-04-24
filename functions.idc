#include <idc.idc>

static initFunctions(void)
{
    del_func(0x004C7174);
    add_func(0x004CA4DF, 0x004CA5F5);
    add_func(0x004CA4DF, 0x004CA5F5);
    add_func(0x004CA679, 0x004CA6AE);
    add_func(0x004CA679, 0x004CA6AE);
    add_func(0x004CA6AE, 0x004CA750);
    add_func(0x004CA6AE, 0x004CA750);
    add_func(0x004CA750, 0x004CA7F6);
    add_func(0x004CA7F6, 0x004CA88B);
    add_func(0x004CA88B, 0x004CA920);
    add_func(0x004CA920, 0x004CAAB9);
    add_func(0x004CAB8E, 0x004CABE8);
    add_func(0x004CABE8, 0x004CABFE);
    add_func(0x004CABFE, 0x004CAC5F);
    add_func(0x004CAC5F, 0x004CACD4);
    add_func(0x004CACD4, 0x004CAD5F);
    add_func(0x004CAD5F, 0x004CADE8);
    add_func(0x004CADE8, 0x004CB00B);
    add_func(0x004CB00B, 0x004CB0CA);
    add_func(0x004CB0CA, 0x004CB0E0);
    add_func(0x004CB0E0, 0x004CB164);
    add_func(0x004CB164, 0x004CB29C);
    add_func(0x004CB29C, 0x004CB2D6);
    add_func(0x004CB2D6, 0x004CB31C);
    add_func(0x004CB31C, 0x004CB74F);
    add_func(0x004CB74F, 0x004CB8D8);
    add_func(0x004CB8D8, 0x004CB966);
    add_func(0x004CBBD2, 0x004CBCAC);
    add_func(0x004CED6F, 0x004CEE0B);
    add_func(0x004CF6B0, 0x004CF74E);
    add_func(0x004CF74E, 0x004CF76D);
    add_func(0x004C7174, 0x004C71F6);
    add_func(0x004C71F6, 0x004C7334);
    add_func(0x004C7334, 0x004C74BB);
    add_func(0x004C74BB, 0x004C76A7);
    add_func(0x004C76A7, 0x004C7722);
    add_func(0x004C7722, 0x004C7903);
    add_func(0x004C7903, 0x004C7AE7);
    add_func(0x004C7AE7, 0x004C8048);
    add_func(0x004C8048, 0x004C877D);

    del_func(0X4395A4);
    del_func(0x4395B1);
    del_func(0x4395BC);
    add_func(0X4395A4, 0X4395B1);
    add_func(0X4395B1, 0X4395BC);
    add_func(0X4395BC, 0X4395C7);
    add_func(0X4395C7, 0X4395D6);
    add_func(0X4395D6, 0X4395DE);
}
