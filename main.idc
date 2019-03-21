#include "enums.idc"
#include "functions.idc"
#include "names.idc"
#include "objects.idc"
#include "strings.idc"
#include "structs.idc"
#include "variables.idc"
#include "widgets.idc"
#include <idc.idc>

static main(void)
{
    initEnums();
    initStructs();
    initStrings();
    initObjects();
    initFunctions();
    initWidgets();
    initNames();
    initVariables();
}
