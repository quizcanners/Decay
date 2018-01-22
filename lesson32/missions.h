
bool LoadQuest(int pack, int no)
{
//string line;
std::string path;
std::stringstream s;
s<<QPack[pack].packName<<"/"<<no;
s>>path;
ifstream map("misions/"+path+".map");
if (map.is_open())		
{
		if (getline (map,path))
			QPack[pack].mission[no].text=path;
		else
		{
			s<<"No data pack "<<pack<<" no "<<no;
			s>>QPack[pack].mission[no].text;
		}
map.close();
return 1;
}
return 0;
}

void LoadMissionPacks()
{
	//std::ofstream map( "misions/PacksList.map" );
string line;
 ifstream map( "misions/PacksList.map" );
 map>>qedit.MissonPacksCount;
 QPack=new MissionPack[qedit.MissonPacksCount];
 for (int i=0; i<qedit.MissonPacksCount; i++)
    if ( getline (map,line) )
	{
		QPack[i].packName=line;
		QPack[i].nameIMG=TextOnlyToImage(QPack[i].packName);
	}
	map.close();  

	for (int i=0; i<qedit.MissonPacksCount; i++)
	{
		int no=0;
			 while (LoadQuest(i, no)) 
			 {
				 no++;
			 }
	}

}
void readMission()
{
if ((qedit.WritingQuestDescription) && (!play))
{


	if (event.type==SDL_KEYDOWN)
	{
// selecting mission 
		if ( keystates[  SDLK_UP ] ) qedit.CurMissionPack=min(qedit.CurMissionPack+1,qedit.MissonPacksCount-1); else
		if ( keystates[  SDLK_DOWN ] ) qedit.CurMissionPack=max(qedit.CurMissionPack-1,0); else
		if ( keystates[  SDLK_RIGHT ] ) qedit.curMission=min(qedit.curMission+1,missionsperpack-1); else
		if ( keystates[  SDLK_LEFT ] ) qedit.curMission=max(qedit.curMission-1,0); else goto 
noreload;
		Inputstr.clear();
		MissionDescription * MD = &QPack[qedit.CurMissionPack].mission[qedit.curMission];
		Inputstr=MD->text;
		  SDL_FreeSurface( Inputtext );
			  //Render a new text surface
			  Inputtext = TextOnlyToImage(Inputstr);
		//qedit.targetQO->scriptText=TextOnlyToImage(qedit.targetQO->scriptPtr->text);
noreload:

	if ( keystates[  SDLK_ESCAPE ] ) {Inputstr.clear(); qedit.WritingQuestDescription=0;}
		else
		if ( keystates[  SDLK_RETURN ] ) {
			MissionDescription * MD = &QPack[qedit.CurMissionPack].mission[qedit.curMission];
			MD->text.clear();
			qedit.targetQO->scriptPtr=MD;
			Uint32 tsize=Inputstr.size();
			qedit.targetQO->scriptPtr->text=new char[tsize];
			qedit.targetQO->scriptPtr->text=Inputstr;
			qedit.targetQO->scriptText=TextOnlyToImage(qedit.targetQO->scriptPtr->text);
			Inputstr.clear(); qedit.WritingQuestDescription=0;}
			else 
				InputText();
	}
}
}