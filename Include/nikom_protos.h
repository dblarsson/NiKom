#ifndef NIKOM_PROTOS_H
#define NIKOM_PROTOS_H

extern struct Library *NiKomBase;

LONG RexxEntry(struct RexxMsg *);
void LockNiKomBase(void);
void UnLockNiKomBase(void);

void Matrix2NiKom(void);
struct FidoText *ReadFidoText(char *, struct TagItem *);
struct FidoText *ReadFidoTextTags(char *, unsigned long tag1Type, ... );
void FreeFidoText(struct FidoText *);
int WriteFidoText(struct FidoText *,struct TagItem *);
int WriteFidoTextTags(struct FidoText *,unsigned long tag1Type, ... );
void ReScanFidoConf(struct Mote *,int);
void UpdateFidoConf(struct Mote *);
void UpdateAllFidoConf(void);
void ReScanAllFidoConf(void);
struct NodeType *GetNodeType(long);
int ReNumberConf(struct Mote *, int, int);
int WriteConf(struct Mote *);
void InitServermem(struct System *);
struct Mote *GetConfPoint(int);
int MaySeeConf(int, int, struct User *);
int MayBeMemberConf(int, int, struct User *);
int MayReadConf(int, int, struct User *);
int MayWriteConf(int, int, struct User *);
int MayReplyConf(int, int, struct User *);
int MayAdminConf(int, int, struct User *);
int IsMemberConf(int, int, struct User *);
void GetNiKomVersion(int *, int *, char *);
void ConvChrsToAmiga(char *, int, int);
void ConvChrsFromAmiga(char *, int, int);
void StripAnsiSequences(char *);
int SetNodeState(int, int);
int SendNodeMessage(int, int, char *);
/* NiKHash *NewNiKHash(int);
void DeleteNiKHash(NiKHash *);
int InsertNiKHash(NiKHash *, int, void *);
void *GetNiKHashData(NiKHash *, int);
void *RemoveNiKHashData(NiKHash *, int); */
int CreateUser(LONG, struct TagItem *);
int CreateUserTags(LONG, unsigned long tag1Type, ... );
int DeleteUser(LONG, ULONG nummer);
int EditUser(LONG, struct TagItem *);
int EditUserTags(LONG, unsigned long tag1Type, ... );
void *ReadUser(LONG, LONG);
int NiKParse(char *, char);
int MarkTextRead(int, int);
int MarkTextUnRead(int, int);

/* PrgCat.c */

int AddProgramData( int, char *, char *, char *, char *);
char *GetProgramData( int, char *, char *, char *);
int FreeProgramCategory(int);
int LoadProgramCategory(int);
int SaveProgramCategory(int);
void DisplayAll(char *, struct ProgramCategory *);
void Debuglog(char *);

/* Misc.c */

int NiKParse(char *, char);
int SysInfo(char *);

/* Crypt.c */

int CheckPassword(int, char *);
char *CryptPassword(char *);

#endif /* NIKOM_PROTOS_H */
