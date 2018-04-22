#include "functions.idc"
#include "names.idc"
#include "strings.idc"
#include "structs.idc"
#include "widgets.idc"
#include <idc.idc>

static main(void)
{
    initFunctions();
    initWidgets();
    initStructs();
    initStrings();
    initNames();
}
