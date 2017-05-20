#include <postgres.h>
#include <utils/guc.h>

#include "guc.h"

bool		guc_disable_optimizations = false;
bool		guc_optimize_plain_tables = false;


void
_guc_init(void)
{
	/* Main database to connect to. */
	DefineCustomBoolVariable("timescaledb.disable_optimizations",
							 "Disable all timescale query optimizations",
							 NULL,
							 &guc_disable_optimizations,
							 false,
							 PGC_USERSET,
							 0,
							 NULL,
							 NULL,
							 NULL);
	DefineCustomBoolVariable("timescaledb.optimize_plain_tables",
						"Apply timescale query optimization to plain tables",
							 "Apply timescale query optimization to plain tables in addition to hypertables",
							 &guc_optimize_plain_tables,
							 false,
							 PGC_USERSET,
							 0,
							 NULL,
							 NULL,
							 NULL);
}

void
_guc_fini(void)
{
}
