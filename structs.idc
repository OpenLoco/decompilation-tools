#include <idc.idc>

static initStructs(void)
{
    loco_initWindow();
    loco_initCompany();
    loco_initTown();
    loco_initUnk1();
    loco_initStation();
    loco_initObjects();
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

static loco_makePointerArray(offset, count, name)
{
    MakeUnknown(offset, count * 4, 1 | 2);
    MakeData(offset, FF_DWRD, 4, BADADDR);
    OpOff(offset, 1, 0);
    MakeName(offset, name);
    MakeArray(offset, count);
    SetColor(offset, CIC_ITEM, 0xFFFFFF);
}

static loco_initObjects(void)
{
    loco_makeStubStruct("object_repository_item", 0x8);
    loco_makeStructArray(0x4FE0B8, "object_repository_item", 64, "object_repository");

    loco_makePointerArray(0x0050C3D0, 1, "_interfaceObjects");
    loco_makePointerArray(0x0050C3D4, 128, "_soundObjects");
    loco_makePointerArray(0x0050C5D4, 1, "_currencyObjects");
    loco_makePointerArray(0x0050C5D8, 32, "_steamObjects");
    loco_makePointerArray(0x0050C658, 8, "_rockObjects");
    loco_makePointerArray(0x0050C678, 1, "_waterObjects");
    loco_makePointerArray(0x0050C67C, 32, "_landObjects");
    loco_makePointerArray(0x0050C6FC, 1, "_townNamesObjects");
    loco_makePointerArray(0x0050C700, 32, "_cargoObjects");
    loco_makePointerArray(0x0050C780, 32, "_wallObjects");
    loco_makePointerArray(0x0050C800, 16, "_trainSignalObjects");
    loco_makePointerArray(0x0050C840, 4, "_levelCrossingObjects");
    loco_makePointerArray(0x0050C850, 1, "_streetLightObjects");
    loco_makePointerArray(0x0050C854, 16, "_tunnelObjects");
    loco_makePointerArray(0x0050C894, 8, "_bridgeObjects");
    loco_makePointerArray(0x0050C8B4, 16, "_trainStationObjects");
    loco_makePointerArray(0x0050C8F4, 8, "_trackExtraObjects");
    loco_makePointerArray(0x0050C914, 8, "_trackObjects");
    loco_makePointerArray(0x0050C934, 16, "_roadStationObjects");
    loco_makePointerArray(0x0050C974, 4, "_roadExtraObjects");
    loco_makePointerArray(0x0050C984, 8, "_roadObjects");
    loco_makePointerArray(0x0050C9A4, 8, "_airportObjects");
    loco_makePointerArray(0x0050C9C4, 8, "_dockObjects");
    loco_makePointerArray(0x0050C9E4, 224, "_vehicleObjects");
    loco_makePointerArray(0x0050CD64, 64, "_treeObjects");
    loco_makePointerArray(0x0050CE64, 1, "_snowObjects");
    loco_makePointerArray(0x0050CE68, 1, "_climateObjects");
    loco_makePointerArray(0x0050CE6C, 1, "_hillShapeObjects");
    loco_makePointerArray(0x0050CE70, 128, "_buildingObjects");
    loco_makePointerArray(0x0050D070, 1, "_scaffoldingObjects");
    loco_makePointerArray(0x0050D074, 16, "_industryObjects");
    loco_makePointerArray(0x0050D0B4, 1, "_regionObjects");
    loco_makePointerArray(0x0050D0B8, 32, "_competitorsObjects");
    loco_makePointerArray(0x0050D138, 1, "_scenarioTextObjects");
}
