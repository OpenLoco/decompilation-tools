#include <idc.idc>

static loco_strlen(i, offset)
{
    auto size, chr;

    if (i == 337)
        return 513;
    if (i == 338)
        return 513;
    if (i == 649)
        return 64;
    if (i == 1250)
        return 513;
    if (i == 1506)
        return 129;
    if (i == 1719)
        return 512;
    if (i == 2039)
        return 513;
    if (i == 2040)
        return 513;
    if (i == 2042)
        return 73;
    if (i == 2045)
        return 77;

    size = 0;

    while (1) {
        chr = Byte(offset + size);
        size++;

        if (chr == 17) {
            size = size + 2;
        }

        if (chr == 23) {
            size = size + 4;
        }

        if (chr == 0x00) {
            break;
        }
    }

    return size;
}

static initStrings(void)
{
    auto i, addr, size, offset, len;
    addr = 0x5183FC;
    size = 0x2000;
    OpOff(addr, 0, 0);
    MakeArray(addr, size);
    SetColor(addr, CIC_ITEM, 0xFFFFFF);

    for (i = 0; i < size; i++) {
        offset = Dword(addr + i * 4);

        if (offset == 0) {
            break;
        }

        len = loco_strlen(i, offset);
        Message("0x%X => %d,\n", offset, len);

        MakeUnknown(offset, len, 1 | 2);
        MakeName(offset, form("_str%04d", i));
        MakeStr(offset, offset + len);

        SetColor(offset, CIC_ITEM, 0xFFFFFF);
    }
}
