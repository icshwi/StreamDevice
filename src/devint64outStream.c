/***************************************************************
* Stream Device record interface for long output records       *
*                                                              *
*                                                              *
* (C) 1999 Dirk Zimoch (zimoch@delta.uni-dortmund.de)          *
* (C) 2005 Dirk Zimoch (dirk.zimoch@psi.ch)                    *
*                                                              *
* This is an EPICS record Interface for StreamDevice.          *
* Please refer to the HTML files in ../doc/ for a detailed     *
* documentation.                                               *
*                                                              *
* If you do any changes in this file, you are not allowed to   *
* redistribute it any more. If there is a bug or a missing     *
* feature, send me an email and/or your patch. If I accept     *
* your changes, they will go to the next release.              *
*                                                              *
* DISCLAIMER: If this software breaks something or harms       *
* someone, it's your problem.                                  *
*                                                              *
***************************************************************/

#include "int64outRecord.h"
#include "devStream.h"

static long readData(dbCommon *record, format_t *format)
{
    int64outRecord *i64o = (int64outRecord *)record;

    switch (format->type)
    {
        case DBF_ULONG:
        case DBF_LONG:
        case DBF_ENUM:
        {
            long val;
            if (streamScanf(record, format, &val) == ERROR) return ERROR;
            if (format->type == DBF_LONG)
                i64o->val = val;
            else
                i64o->val = (unsigned long)val;
            return OK;
        }
    }
    return ERROR;
}

static long writeData(dbCommon *record, format_t *format)
{
    int64outRecord *i64o = (int64outRecord *)record;

    switch (format->type)
    {
        case DBF_ULONG:
        case DBF_ENUM:
        case DBF_LONG:
            return streamPrintf(record, format, (long)i64o->val);
    }
    return ERROR;
}

static long initRecord(dbCommon *record)
{
    int64outRecord *i64o = (int64outRecord *)record;

    return streamInitRecord(record, &i64o->out, readData, writeData) == ERROR ?
        ERROR : OK;
}

struct {
    long number;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN write;
} devint64outStream = {
    5,
    streamReport,
    streamInit,
    initRecord,
    streamGetIointInfo,
    streamWrite
};

epicsExportAddress(dset,devint64outStream);