#include <idc.idc>

static initStructs(void)
{
    loco_initWindow();
    loco_initCompany();
    loco_initTown();
    loco_initUnk1();
    loco_initStation();
}

static loco_setStructMember(id, offset, name, type, size)
{
    if (GetMemberName(id, name) == 0) {
        AddStrucMember(id, name, offset, type, 0, size);
    } else {
        SetMemberName(id, offset, name);
        SetMemberType(id, offset, type, 0, size);
    }
}

static loco_makeStructArray(offset, type, count, name)
{
    auto size = GetStrucSize(GetStrucIdByName(type));

    MakeUnknown(offset, count * size, 1 | 2);
    MakeStruct(offset, type);
    MakeName(offset, name);
    MakeArray(offset, count);
    SetColor(offset, CIC_ITEM, 0xFFFFFF);
}

static loco_makeStubStruct(name, size)
{
    auto id;

    id = GetStrucIdByName(name);
    if (id == -1) {
        id = AddStruc(-1, name);
    }

    loco_setStructMember(id, size - 1, "end", FF_BYTE | FF_DATA, 1);

    return id;
}

static loco_initWindow(void)
{
    loco_makeStubStruct("window_t", 0x88E);
    loco_makeStructArray(0x11370AC, "window_t", 12, "_windows");
}

static loco_initCompany(void)
{
    loco_makeStubStruct("company_t", 0x8FA8);
    loco_makeStructArray(0x531784, "company_t", 15, "_companies");
}

static loco_initTown(void)
{
    loco_makeStubStruct("town_t", 0x270);
    loco_makeStructArray(0x5B825C, "town_t", 80, "_towns");
}

static loco_initUnk1(void)
{
    loco_makeStubStruct("unk1_t", 0x453);
    loco_makeStructArray(0x5C455C, "unk1_t", 128, "_unk1");
}

static loco_initStation(void)
{
    loco_makeStubStruct("station_t", 0x3D2);
    loco_makeStructArray(0x5E6EDC, "station_t", 1024, "_stations");
}
