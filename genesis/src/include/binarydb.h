/*
// Full copyright information is available in the file ../doc/CREDITS
*/

#ifndef cdc_binarydb_h
#define cdc_binarydb_h

#ifdef S_IRUSR
#define READ_WRITE              (S_IRUSR | S_IWUSR)
#define READ_WRITE_EXECUTE      (S_IRUSR | S_IWUSR | S_IXUSR)
#else
#define READ_WRITE 0600
#define READ_WRITE_EXECUTE 0700
#endif

void   init_binary_db(void);
void   init_new_db(void);
Int    init_db(Int force_textdump);
Int    db_get(Obj * object, Long name);
Int    db_put(Obj * object, Long name);
Int    db_check(Long name);
Int    db_del(Long name);
char * db_traverse_first(void);
char * db_traverse_next(void);
Int    db_backup(char * out);
void   db_close(void);
void   db_flush(void);
void   init_core_objects(void);
Float  db_fragmentation(void);

#endif

