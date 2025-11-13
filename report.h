#ifndef REPORT_H
#define REPORT_H
#include "utils.h"

typedef enum { REP_INVENTORY, REP_MEMBERS, REP_RESERVATIONS } ReportType;

void generateReport(Library *lib);

#endif