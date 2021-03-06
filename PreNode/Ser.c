#include <exec/types.h>
#include <exec/memory.h>
#include <intuition/intuition.h>
#include <dos/dos.h>
#include <dos/dostags.h>
#include <proto/exec.h>
#include <proto/intuition.h>
#include <proto/dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dos.h>
#include "NiKomstr.h"
#include "NiKomLib.h"
#include "PreNodeFuncs.h"

#define ERROR	10
#define OK	0
#define EKO	1
#define EJEKO	0
#define NOCARRIER	32
#define KONVCHAR 400

int CXBRK(void) { return(0); }

struct IntuitionBase *IntuitionBase=NULL;
struct RsxLib *RexxSysBase;
struct Library *UtilityBase, *NiKomBase;
struct Window *NiKwind=NULL;
struct MsgPort *rexxport, *nikomnodeport;

char rexxportnamn[20], pubscreen[40],nodid[20], nikomnodeportnamn[15];
int inloggad, getty,gettybps,relogin=FALSE,ypos,xpos,ysize,xsize;

extern char commandhistory[], inmat[], outbuffer[], reggadnamn[];
extern int hangupdelay, dtespeed, highbaud, nodnr, nodestate;
extern struct System *Servermem;

struct Window *openmywindow(char *screenname) {
	static char titel[32];
	sprintf(titel,"NiKom PreNode, nod #%d SER:",nodnr);
	return(OpenWindowTags(NULL,WA_Left,xpos,
										WA_Top,ypos,
										WA_Width,xsize,
										WA_Height,ysize,
										WA_IDCMP,IDCMP_CLOSEWINDOW,
										WA_MinWidth,50,
										WA_MinHeight,10,
										WA_MaxWidth,~0,
										WA_MaxHeight,~0,
										WA_SizeGadget,TRUE,
										WA_SizeBBottom, TRUE,
										WA_DragBar,TRUE,
										WA_DepthGadget,TRUE,
										WA_CloseGadget,TRUE,
										WA_SimpleRefresh,TRUE,
										WA_Title,titel,
										WA_ScreenTitle,"NiKom � Tomas K�rki 1996-1998",
										WA_AutoAdjust,TRUE,
										WA_PubScreenName,screenname,
										TAG_DONE));
}


void cleanup(int kod,char *text)
{
	shutdownnode(NODSER);
	CloseIO();
	if(NiKwind) CloseWindow(NiKwind);
	if(RexxSysBase) CloseLibrary((struct Library *)RexxSysBase);
	if(nikomnodeport) {
		RemPort(nikomnodeport);
		DeleteMsgPort(nikomnodeport);
	}
	if(rexxport) {
		RemPort(rexxport);
		DeleteMsgPort(rexxport);
	}
	if(NiKomBase) CloseLibrary(NiKomBase);
	if(UtilityBase) CloseLibrary(UtilityBase);
	if(IntuitionBase) CloseLibrary((struct Library *)IntuitionBase);
	printf("%s",text);
	exit(kod);
}

void main(int argc,char *argv[]) {
	int going=TRUE,forsok=2,car=1,x,connectbps, i;
	struct NodeType *nt;
	char *tmppscreen,commandstring[100], configname[50] = "NiKom:DatoCfg/SerNode.cfg";
	FILE *fil;
	if(argc>1) for(x=1;x<argc;x++) {
		if(argv[x][0]=='-') {
			if(argv[x][1]=='G') getty=TRUE;
			else if(argv[x][1]=='B') gettybps=atoi(&argv[x][2]);
			else if(argv[x][1]=='C') connectbps = atoi(&argv[x][2]);
		} else strcpy(configname,argv[x]);
	}
	if(!(IntuitionBase=(struct IntuitionBase *)OpenLibrary("intuition.library",0)))
		cleanup(ERROR,"Kunde inte �ppna intuition.library\n");
	if(!(UtilityBase=OpenLibrary("utility.library",37L)))
		cleanup(ERROR,"Kunde inte �ppna utility.library\n");
	if(!(NiKomBase=OpenLibrary("nikom.library",0L)))
		cleanup(ERROR,"Kunde inte �ppna nikom.library\n");
	if(!initnode(NODSER)) cleanup(ERROR,"Kunde inte registrera noden i Servern\n");
	if(!(nikomnodeport = CreateMsgPort()))
		cleanup(ERROR,"Kunde inte skapa NiKomNode-porten");
	sprintf(nikomnodeportnamn,"NiKomNode%d",nodnr);
	nikomnodeport->mp_Node.ln_Name = nikomnodeportnamn;
	nikomnodeport->mp_Node.ln_Pri = 1;
	AddPort(nikomnodeport);
	sprintf(rexxportnamn,"NiKomPreRexx%d",nodnr);
	if(!(rexxport=(struct MsgPort *)CreateMsgPort()))
		cleanup(ERROR,"Kunde inte �ppna RexxPort\n");
	rexxport->mp_Node.ln_Name=rexxportnamn;
	rexxport->mp_Node.ln_Pri=50;
	AddPort(rexxport);
	if(!(RexxSysBase=(struct RsxLib *)OpenLibrary("rexxsyslib.library",0L)))
		cleanup(ERROR,"Kunde inte �ppna rexxsyslib.library\n");
	getnodeconfig(configname);
	if(pubscreen[0]=='-') tmppscreen=NULL;
	else tmppscreen=pubscreen;
	if(!(NiKwind=openmywindow(tmppscreen)))
		cleanup(ERROR,"Kunde inte �ppna f�nstret\n");
	if(getty) dtespeed = gettybps;
	else dtespeed = highbaud;
	if(!OpenIO(NiKwind)) cleanup(ERROR,"Couldn't setup IO");
	if(!getkeyfile())
		cleanup(ERROR,"Korrupt nyckelfil\n");
	strcpy(Servermem->nodid[nodnr],nodid);
	conreqtkn();
	serreqtkn();
	Delay(50);
	for(;;) {
	inloggad=-1;
	Servermem->idletime[nodnr] = time(NULL);
	Servermem->inloggad[nodnr]=-1;
	if(getty) Servermem->connectbps[nodnr] = connectbps;
	else waitconnect();
	Servermem->idletime[nodnr] = time(NULL);
	Servermem->inloggad[nodnr]=-2; /* S�tt till <Uppringd> f�r att �ven hantera -getty-fallet */
reloginspec:
	updateinactive();
	Servermem->inne[nodnr].flaggor = Servermem->cfg.defaultflags;
	if(!getty) Delay(100);
	Servermem->inne[nodnr].rader=0;
	Servermem->inne[nodnr].chrset = CHRS_LATIN1;
	sendfile("NiKom:Texter/Inlogg.txt");
	if(Servermem->cfg.ar.preinlogg) sendrexx(Servermem->cfg.ar.preinlogg);
	car=TRUE;
	Servermem->inne[nodnr].chrset = 0;
	memset(commandhistory,0,1000);
	going=1;
	while(going && going<=Servermem->cfg.logintries) {
		putstring("\r\nNamn: ",-1,0);
		if(getstring(EKO,40,NULL)) { car=FALSE; break; }
		if(!stricmp(inmat,Servermem->cfg.ny) && !(Servermem->cfg.cfgflags & NICFG_CLOSEDBBS))
		{
/*			if(!reggadnamn[0] && ((struct ShortUser *)Servermem->user_list.mlh_TailPred)->nummer >= 4)
				puttekn("\n\n\rDenna demoversion av NiKom kan hantera maximalt 5 anv�ndare.\n\r",-1);
			else { */
				if((car=nyanv())==2) goto panik;
				going=FALSE;
/*			} */
		} else if((inloggad=parsenamn(inmat))>=0) {
			readuser(inloggad,&Servermem->inne[nodnr]);
			forsok=2;
			while(forsok) {
				puttekn("\r\nL�sen: ",-1);
				if(Servermem->inne[nodnr].flaggor & STAREKOFLAG)
				{
					if(getstring(STAREKO,15,NULL)) { car=FALSE; break; }
				}
				else
				{
					if(getstring(EJEKO,15,NULL)) { car=FALSE; break; }
				}
				if(CheckPassword(inloggad, inmat))
				{
					forsok=FALSE;
					going=FALSE;
				} else forsok--;
			}
			if(going && (Servermem->cfg.logmask & LOG_FAILINLOGG)) {
				sprintf(outbuffer,"Nod %d, %s angivet som namn, fel l�sen.", nodnr,getusername(inloggad));
				logevent(outbuffer);
			}
			if(going) going++;
		} else if(inloggad==-1) puttekn("\r\nHittar ej namnet\r\n",-1);
	}
	if(!car) {
		if(getty) cleanup(OK,"");
		disconnect();
		continue;
	}
	if(going) {
		putstring("\n\n\rTyv�rr. Du har f�rs�kt maximalt antal g�nger att logga in. Kopplar ned.\n\r",-1,0);
		goto panik;      /* Urrk vad fult. :-) */
	}
	Servermem->inloggad[nodnr]=inloggad;
	Servermem->idletime[nodnr] = time(NULL);
	if(Servermem->inne[nodnr].shell) {
		nt =  GetNodeType(Servermem->inne[nodnr].shell);
		if(!nt) Servermem->inne[nodnr].shell=0;
	}
	if(!Servermem->inne[nodnr].shell) {
		putstring("\n\n\rDu har ingen f�rinst�lld nodtyp.\n\n\rV�lj mellan:\n\n\r",-1,0);
		for(x=0; x<MAXNODETYPES; x++) {
			if(Servermem->nodetypes[x].nummer==0) break;
			sprintf(outbuffer,"%2d: %s\n\r",Servermem->nodetypes[x].nummer,Servermem->nodetypes[x].desc);
			putstring(outbuffer,-1,0);
		}
		going=TRUE;
		while(going) {
			putstring("\n\rVal: ",-1,0);
			if(getstring(EKO,2,NULL)) goto panik;
			if(atoi(inmat)<1) putstring("\n\rDu m�ste ange ett positivt heltal.\n\r",-1,0);
			else if(!(nt=GetNodeType(atoi(inmat)))) putstring("\n\rFinns ingen s�dan nodtyp.\n\r",-1,0);
			else going=FALSE;
		}
		putstring("\n\n\rVill du anv�nda denna nodtyp varje g�ng du loggar in?",-1,0);
		if(jaellernej('j','n',1)) {
			putstring("Ja\n\r",-1,0);
			Servermem->inne[nodnr].shell=nt->nummer;
		} else putstring("Nej\n\r",-1,0);
	}
	abortinactive();
	abortserial();

	sprintf(commandstring,"%s -N%d -B%d %s",nt->path,nodnr,dtespeed,configname);
	CloseConsole();
	CloseWindow(NiKwind);
	NiKwind = NULL;
	RemPort(nikomnodeport);

	i = 0;
	if(Servermem->connectbps[nodnr] > 0)
	{
		while(Servermem->info.bps[i] != Servermem->connectbps[nodnr] && Servermem->info.bps[i] > 0 && i<49)
			i++;

		if(i<49)
		{
			if(Servermem->info.bps[i] == Servermem->connectbps[nodnr])
				Servermem->info.antbps[i]++;
			else
			{
				Servermem->info.bps[i] = Servermem->connectbps[nodnr];
				Servermem->info.antbps[i]++;
			}
		}

		if(!(fil = fopen("NiKom:datocfg/sysinfo.dat","w")))
		{
			/* putstring("Kunde inte spara nya sysinfo.dat..\n",-1,0); */
		}

		if(fwrite((void *)&Servermem->info,sizeof(Servermem->info),1,fil) != 1)
		{
			/* putstring("Kunde inte skriva till nya sysinfo.dat....\n",-1,0); */
		}
		fclose(fil);
	}

	nodestate = SystemTags(commandstring, SYS_UserShell, TRUE, TAG_DONE);
	AddPort(nikomnodeport);
	if(!getty || (nodestate & NIKSTATE_RELOGIN)) {
		if(!(NiKwind = openmywindow(tmppscreen))) cleanup(ERROR,"Kunde inte �ppna f�nstret\n");
		OpenConsole(NiKwind);
	}
	serreqtkn();
	if(nodestate & NIKSTATE_RELOGIN) goto reloginspec;
panik:
	Delay(hangupdelay);
	if(getty) cleanup(OK,"");
	disconnect();
	}
}
