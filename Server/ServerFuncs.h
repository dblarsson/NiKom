/* Prototypes for functions defined in NiKSerVersion.c */

void GetServerversion(void);

/* Prototypes for functions defined in NiKServer.c */
int bamtest(char *foobar,int bar);
void bamset(char *foobar,int bar);
void bamclear(char *foobar,int bar);
void getkmd(void);
void getnycklar(void);
void getconfig(void);
void getmoten(void);
void getareor(void);
void getfiler(void);
void gettextmot(void);
int adduser(int nummer);
void scanuserdir(char *dirnamn);
void getnamn(void);
void getgrupper(void);
void getinfo(void);
void getsenaste(void);
int getcfgfilestring(char *str,BPTR fh,char *vart);
void getstatus(void);
void initflaggor(void);
int getzone(char *);
int getnode(char *);
int getnet(char *);
int getpoint(char *);
void getfidocfg(void);
void getnodetypescfg(void);
void sparatext(struct NiKMess *message);
void radera(int texter);
void writeinfo(void);
void addhost(int);
void freefilemem(void);
void freecommandmem(void);
void freeshortusermem(void);
void freeloginmem(void);
void freegroupmem(void);
void cleanup(int kod,char *fel);
void main(void);

/* Prototypes for functions defined in NiKHost.c */
int readuser(int nummer,struct User *);
int writeuser(int nummer,struct User *);
void userinfo(struct RexxMsg *mess);
void motesinfo(struct RexxMsg *mess);
void chgmote(struct RexxMsg *mess);
void writemeet(struct Mote *motpek);
void nikparse(struct RexxMsg *mess);
void senaste(struct RexxMsg *mess);
void sysinfo(struct RexxMsg *mess);
void rexxreadcfg(struct RexxMsg *mess);
void handlerexx(struct RexxMsg *mess);
int parsenamn(char *skri);
int matchar(char *skrivet,char *facit);
char *hittaefter(char *strang);
int parse(char *skri);
int parsemot(char *skri);
int parsearea(char *skri);
int parsenyckel(char *skri);
struct Fil *parsefil(char *skri,int area);
char *getusername(int nummer);
int userexists(int nummer);
int getuserstatus(int nummer);
void kominfo(struct RexxMsg *mess);
void filinfo(struct RexxMsg *mess);
void areainfo(struct RexxMsg *mess);
void chguser(struct RexxMsg *mess);
void skapafil(struct RexxMsg *mess);
int valnamn(char *,int);

/* Prototypes for functions defined in NiKHost2.c */
int readtexthead(int nummer,struct Header *head);
int writetexthead(int nummer,struct Header *head);
void rexxnodeinfo(struct RexxMsg *mess);
void rexxnextfile(struct RexxMsg *mess);
void rexxraderafil(struct RexxMsg *mess);
void createtext(struct RexxMsg *mess);
int updatenextletter(int user);
void createletter(struct RexxMsg *mess);
void textinfo(struct RexxMsg *mess);
void nextunread(struct RexxMsg *mess);
void freeeditlist(void);
int readuserbitmap(int nummer,char *bitmap,int bmnr);
int writeuserbitmap(int nummer,char *bitmap,int bmnr);
struct Mote *getmotpek(int);
char *getmotnamn(int);
void meetright(struct RexxMsg *);
void meetmember(struct RexxMsg *);
void chgmeetright(struct RexxMsg *);
void chgfile(struct RexxMsg *);
int parsegrupp(char *);
void keyinfo(struct RexxMsg *);
void getdir(struct RexxMsg *);
int choosedir(int, char *, int);
void deloldtexts(struct RexxMsg *);
void movefile(struct RexxMsg *);
void rexxnextpatternfile(struct RexxMsg *);

/* Prototypes for functions defined in NiKHost2.c */
void rxsendnodemess(struct RexxMsg *);

/* Prototypes for functions defined in NiKHost3.c */
void rxsendnodemess(struct RexxMsg *);
void rexxstatusinfo(struct RexxMsg *);
void rexxarearight(struct RexxMsg *);
void sortbps(long *bps[], long *);
void swapbps(long *bps, long *);
void rexxstatusinfo(struct RexxMsg *);
void rexxsysteminfo(struct RexxMsg *);
int arearatt(int, int, struct User *);
void rexxgetprogramdata(struct RexxMsg *);
void rexxaddprogramdata(struct RexxMsg *);
void rexxmarktextread(struct RexxMsg *);
void rexxmarktextunread(struct RexxMsg *);
void rexxconsoletext(struct RexxMsg *);
void rexxcheckuserpassword(struct RexxMsg *);

/* NodeComm.c */
void setnodestate(struct NiKMess *);
struct MsgPort *SafePutToPort(struct NiKMess *, char *);
long sendnodemess(short, long, long, long, long);
