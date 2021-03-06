#ifndef TIMESCALEDB_SCANNER_H
#define TIMESCALEDB_SCANNER_H

#include <postgres.h>
#include <access/relscan.h>
#include <utils/fmgroids.h>
#include <access/heapam.h>
#include <nodes/lockoptions.h>

typedef enum ScannerType
{
	ScannerTypeHeap,
	ScannerTypeIndex,
} ScannerType;

/* Tuple information passed on to handlers when scanning for tuples. */
typedef struct TupleInfo
{
	Relation	scanrel;
	HeapTuple	tuple;
	TupleDesc	desc;
	/* return index tuple if it was requested -- only for index scans */
	IndexTuple	ituple;
	TupleDesc	ituple_desc;

	/*
	 * If the user requested a tuple lock, the result of the lock is passed on
	 * in lockresult.
	 */
	HTSU_Result lockresult;
} TupleInfo;

typedef struct ScannerCtx
{
	Oid			table;
	Oid			index;
	ScannerType scantype;
	ScanKey		scankey;
	int			nkeys,
				norderbys;
	bool		want_itup;
	LOCKMODE	lockmode;
	struct
	{
		LockTupleMode lockmode;
		LockWaitPolicy waitpolicy;
		bool		enabled;
	}			tuplock;
	ScanDirection scandirection;
	void	   *data;			/* User-provided data passed on to filter()
								 * and tuple_found() */

	/*
	 * Optional handler called before a scan starts, but relation locks are
	 * acquired.
	 */
	void		(*prescan) (void *data);

	/*
	 * Optional handler called after a scan finishes and before relation locks
	 * are released. Passes on the number of tuples found.
	 */
	void		(*postscan) (int num_tuples, void *data);

	/*
	 * Optional handler to filter tuples. Should return true for tuples that
	 * should be passed on to tuple_found, or false otherwise.
	 */
	bool		(*filter) (TupleInfo *ti, void *data);

	/*
	 * Handler for found tuples. Should return true to continue the scan or
	 * false to abort.
	 */
	bool		(*tuple_found) (TupleInfo *ti, void *data);
} ScannerCtx;

/* Performs an index scan or heap scan and returns the number of matching
 * tuples. */
int			scanner_scan(ScannerCtx *ctx);

#endif   /* TIMESCALEDB_SCANNER_H */
