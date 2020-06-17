#include "stdafx.h"
#include <algorithm>
#include "extmappingtool.h"
#include "TriggerSystem.h"
#include "triggerdialog.h"
#include "serializer.h"
#include "deserializer.h"
#include "SFmpqapi\SFmpqapi.h"
#pragma comment (lib, "D:\\sfmpq\\SFmpq.lib" )
ExtMappingTool::ExtMappingTool(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	ui.saveButton->setDisabled(true);

}

void ExtMappingTool::handleIcons() {
	//ui.saveButton->setIcon(QIcon("qttest2.ico"));
	ui.saveButton->setIcon(this->style()->standardIcon(QStyle::SP_DialogSaveButton));
	ui.loadButton->setIcon(this->style()->standardIcon(QStyle::SP_DialogOpenButton));
	
}

void ExtMappingTool::on_factionsButton_clicked() {
	if (api.first_open[DialogMenu::Factions]) {
		factionDlg.setVisible(true);
	}
	else {
		api.first_open[DialogMenu::Factions] = false;
		if (factionDlg.exec()) {

		}
	}
}
void ExtMappingTool::on_upgReqButton_clicked() {

	
	if (api.first_open[DialogMenu::UpgReqEditor]) {
		upgReqMenu.setVisible(true);
	}
	else {
		api.first_open[DialogMenu::UpgReqEditor] = false;
		if (upgReqMenu.exec()) {

		}
	}
	
}

void ExtMappingTool::on_upgradesButton_clicked() {
	if (api.first_open[DialogMenu::Upgrades]) {
		extendedUpg.setVisible(true);
	}
	else {
		api.first_open[DialogMenu::Upgrades] = false;
		if (extendedUpg.exec()) {

		}
	}
}

void ExtMappingTool::on_diplomacyButton_clicked() {
	if (api.first_open[DialogMenu::Diplomacy]) {
		diplomacy_dlg.setVisible(true);
	}
	else {
		api.first_open[DialogMenu::Diplomacy] = false;
		if (diplomacy_dlg.exec()) {

		}
	}
}

void ExtMappingTool::on_alliancesButton_clicked() {
	if (api.first_open[DialogMenu::Diplomacy]) {
		allianceDlg.setVisible(true);
	}
	else {
		api.first_open[DialogMenu::Alliances] = false;
		if (allianceDlg.exec()) {

		}
	}
}

void ExtMappingTool::on_triggersButton_clicked() {
	//for alliances/disp button / example

	if (api.first_open[DialogMenu::ExtendedTriggers]) {
		trg.setVisible(true);
	}
	else {
		api.first_open[DialogMenu::ExtendedTriggers] = false;
		if (trg.exec()) {

		}
	}

	/*triggerMenu dialog(this);
	if (dialog.exec()) {
	}*/
	
}



void ExtMappingTool::WriteTempArchive() {
	std::ofstream temp;
	temp.open("temp.dat", std::ios::binary | std::ios::out);
	/*
	test_stream << editor_info;
	test_stream << file_info;
	test_stream << trigger_setup;//is it required?

	test_stream << trigger_system;
	
	test_stream << alliances;*/
	//
	int size_presumed=0;
	temp << std::noskipws << version;
	temp << std::noskipws << disposition;
	temp << std::noskipws << faction_list;
	temp << std::noskipws << upgrades_data;
	temp << std::noskipws << alliances;
	temp.close();
}

void ExtMappingTool::FillDataFromInterface() {
	std::ofstream log;
	log.open("log.log", std::ios::app);
	
	
	diplomacy_dlg.fillDataFromInterface();
	trg.fillDataFromInterface();
	factionDlg.fillDataFromInterface();
	allianceDlg.fillDataFromInterface();
	for (auto a : faction_list) {
		log << "Data in boxes: " << WriteInt(a.index) << " " << WriteInt(a.iterator) << '\n';
	}
	extendedUpg.fillDataFromInterface();
	log << "2\n";
	for (auto a : faction_list) {
		log << "Data in boxes: " << WriteInt(a.index) << " " << WriteInt(a.iterator) << '\n';
	}
	log.close();
}

void ExtMappingTool::ClearData() {
//	diplomacy_dlg.clearData();
//	trg.clearData();
//	factionDlg.clearData();
}

void ExtMappingTool::DeleteTempFile() {
	if (remove("temp.dat") != 0)
		perror("Error: File can't be deleted");
}

void ExtMappingTool::on_saveButton_clicked() {
	/*
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Save Map File"), "",
		tr("StarCraft Brood War map (*.scx);;All Files (*)"));
	if (fileName.isEmpty())
		return;
	else {
		QFile file(fileName);
		if (!file.open(QIODevice::WriteOnly)) {
			QMessageBox::information(this, tr("Unable to open file"),
				file.errorString());
			return;
		}
//		QDataStream out(&file);
//		out.setVersion(QDataStream::Qt_4_5);
//		out << contacts;
	}*/
	if (api.file_is_open) {
		/*
		if (api.hMpq == NULL) {
			QMessageBox::information(this, tr("Mpq archive pointer is null"), "");
			return;
		}*/
		ClearEditorReadWriteData();
		FillDataFromInterface();

		WriteTempArchive();
		ClearData();
		auto path = QDir::currentPath()+"/temp.dat";
		auto file_str = path.toStdString();
		auto subpath = QDir::currentPath() + "/data/factions.txt";
		auto fact_str = subpath.toStdString();

		std::basic_string<TCHAR> temp_conv(file_str.begin(), file_str.end());
		std::string fileName_str = api.fileName.toStdString();
		HANDLE newMpqHandle = MpqOpenArchiveForUpdate(fileName_str.c_str(), MOAU_OPEN_EXISTING|MOAU_MAINTAIN_LISTFILE, 262);
		
		if (newMpqHandle == NULL) {
			QMessageBox::information(this, tr("Archive is 0 (null)"), api.fileName);
			return;
		}
		if (reinterpret_cast<int>(newMpqHandle) == -1) {
			QMessageBox::information(this, tr("Archive is -1 (null)"), api.fileName);
			return;
		}
		int eax = reinterpret_cast<int>(newMpqHandle);
		QMessageBox::information(this, tr("Archive"), QString::fromStdString("EAX: "+WriteInt(eax)));
		if (!MpqAddFileToArchive(newMpqHandle, file_str.c_str(), "extension.dat",
			MAFA_REPLACE_EXISTING)) {
			QMessageBox::information(this, tr("Unable to add the file to mpq archive"), api.fileName);
			return;
		}		
		if (!MpqAddFileToArchive(newMpqHandle, fact_str.c_str(), "factions.ini",
			MAFA_REPLACE_EXISTING)) {
			QMessageBox::information(this, tr("Unable to add the faction data to mpq archive"), api.fileName);
			return;
		}
		if (!MpqCloseUpdatedArchive(newMpqHandle, 0)) {
			QMessageBox::information(this, tr("Can't close the archive"), api.fileName);
			return;
		}
			
		//DeleteTempFile();
	}
}
void ExtMappingTool::ResetInterface(bool read_from_data) {
	diplomacy_dlg.close();
	trg.close();
	factionDlg.close();
	extendedUpg.close();
	diplomacy_dlg.ResetInterface(read_from_data);
	factionDlg.ResetInterface(read_from_data);
	trg.ResetInterface();
	extendedUpg.ResetInterface(read_from_data);
	allianceDlg.ResetInterface(read_from_data);
	

	for (int i = 0; i < 25; i++) {
		api.first_open[i] = false;
	}
}
#include <iostream>
#include <fstream>
#include <ostream>
void ExtMappingTool::ReadTempArchiveToData(std::string filename){
	//deserialize from temp external file
	std::ifstream temp;
	temp.open(filename, std::ios::binary | std::ios::in);
	/*
	test_stream >> editor_info;
	test_stream >> file_info;
	test_stream >> trigger_setup;//is it required?

	test_stream >> trigger_system;
	test_stream >> alliances;*/
	//
	QMessageBox::information(this, tr("Unable to open file"),
		QString::fromStdString("Temp archive player array size: " + WriteInt(upgrades_data.size())));
	QMessageBox::information(this, tr("Debug"),
		QString::fromStdString("Try to deserialize disposition"+WriteInt(disposition.size())+" "+filename));
//	std::ofstream log;
//	log.open("log.log", std::ios::app);
//	log << "Read Disposition\n";
//	log.close();
	//log << disposition;
	QMessageBox::information(this, tr("Debug"),
		QString::fromStdString("Read archive to disposition: " + WriteInt(disposition.size())));


	int vt = 0;
	
	temp >> std::noskipws >> vt;
	if (vt != version) {
		QMessageBox::information(this, tr("Error"),
			QString::fromStdString("Version in archive is incorrect, version " + WriteInt(version) +
				" is required (current version is " + WriteInt(vt) + "). Contact iquare for details."));
		QApplication::quit();
	}
	version = vt;
	temp >> std::noskipws >> disposition;
	temp >> std::noskipws >> faction_list;
	temp >> std::noskipws >> upgrades_data;
	temp >> std::noskipws >> alliances;
	int count = 0;
	char c;
	while (temp.good()) {
		temp.read(&c, 1);
		count++;
	}
	QMessageBox::information(this, tr("Debug"),
		QString::fromStdString("Spare chars: " + WriteInt(count) + " " + WriteInt((int)c)));
	temp.close();
}
void ExtMappingTool::ClearEditorReadWriteData() {
	editor_info.clear();
	file_info.clear();
	trigger_setup.clear();
	trigger_system.clear();
	faction_list.clear();
	alliances.clear();
	disposition.clear();
	upgrades_data.clear();
}
void ExtMappingTool::SetTempArchiveToInterface(){
	
}

namespace Header {
	enum {
		None,

		Type,
		Ver,
		Iver,
		Ive2,
		Vcod,
		Iown,
		Ownr,
		Era,
		Dim,
		Side,
		Mtxm,
		Puni,
		Upgr,
		Ptec,
		Unit,
		Isom,
		Tile,
		Dd2,
		Thg2,
		Mask,
		Str,
		Uprp,
		Upus,
		Mrgn,
		Trig,
		Mbrf,
		Sprp,
		Forc,
		Wav,
		Unis,
		Upgs,
		Tecs,
		Swnm,
		Colr,
		Pupx,
		Ptex,
		Unix,
		Upgx,
		Tecx,


	};
}

int ExtMappingTool::header_match(u32 str) {
	std::string header;
	header.resize(4);
	header[0] = str & 0x000000ff;
	header[1] = (str & 0x0000ff00) >> 8;
	header[2] = (str & 0x00ff0000) >> 16;
	header[3] = (str & 0xff000000) >> 24;
//	QMessageBox::information(this, tr(""), QString::fromStdString("[" + header + "] chunk"));

	if (header == "TYPE") {
//		QMessageBox::information(this, tr(""), "type chunk");
		return Header::Type;
	}
	if (header == "VER ")
	{
//		QMessageBox::information(this, tr(""), "ver chunk");
		return Header::Ver;
	}
	
	if (header == "IVER") {
		return Header::Iver;
	}
	if (header == "IVE2") {
		return Header::Ive2;
	}
	if (header == "VCOD") {
		return Header::Vcod;
	}
	if (header == "IOWN") {
		return Header::Iown;
	}
	if (header == "OWNR") {
		return Header::Ownr;
	}
	if (header == "ERA ") {
		return Header::Era;
	}
	if (header == "DIM ") {
		return Header::Dim;
	}
	if (header == "SIDE") {
		return Header::Side;
	}
	if (header == "MTXM") {
		return Header::Mtxm;
	}
	if (header == "PUNI") {
		return Header::Puni;
	}
	if (header == "UPGR") {
		return Header::Upgr;
	}
	if (header == "PTEC") {
		return Header::Ptec;
	}
	if (header == "UNIT") {
		return Header::Unit;
	}
	if (header == "ISOM") {
		return Header::Isom;
	}
	if (header == "TILE") {
		return Header::Tile;
	}
	if (header == "DD2 "){
		return Header::Dd2;
	}
	if (header == "THG2"){
		return Header::Thg2;
	}
	if (header == "MASK"){
		return Header::Mask;
	}
	if (header == "STR "){
		return Header::Str;
	}
	if (header == "UPRP"){
		return Header::Uprp;
	}
	if (header == "UPUS"){
		return Header::Upus;
	}
	if (header == "MRGN") {
		return Header::Mrgn;
	}
	if (header == "TRIG"){
		return Header::Trig;
	}
	if (header == "MBRF") {
		return Header::Mbrf;
	}
	if (header == "SPRP"){
		return Header::Sprp;
	}
	if (header == "FORC") {
		return Header::Forc;
	}
	if (header == "WAV "){
		return Header::Wav;
	}
	if (header == "UNIS"){
		return Header::Unis;
	}
	if (header == "UPGS") {
		return Header::Upgs;
	}
	if (header == "TECS"){
		return Header::Tecs;
	}
	if (header == "SWNM"){
		return Header::Swnm;
	}
	if (header == "COLR"){
		return Header::Colr;
	}
	if (header == "PUPx") {
		return Header::Pupx;
	}
	if (header == "PTEx") {
		return Header::Ptex;
	}
	if (header == "UNIx") {
		return Header::Unix;
	}
	if (header == "UPGx") {
		return Header::Upgx;
	}
	if (header == "TECx") {
		return Header::Tecx;
	}
	return Header::None;
}

void ExtMappingTool::ReadChunk(int chk_id, std::ifstream &temp, u32 header){
	
	//+add braces to old cases
	//+add pushbacks
	//+..must respect vectors

	//ADD STR DATA
//	QMessageBox::information(this, tr(""), QString::fromStdString("Read chunk [" + WriteInt(chk_id) + "]"));
	switch (chk_id) {
		case Header::Type:
		{
			TypeChunk chunk;
			temp.read(reinterpret_cast<char*>(&chunk), sizeof(chunk));
			Chk.type_chunks.push_back(chunk);
			break;
		}
		case Header::Colr:
		{
			ColrChunk chunk;
			temp.read(reinterpret_cast<char*>(&chunk), sizeof(chunk));
			Chk.colr_chunks.push_back(chunk);
			break;
		}
		case Header::Dd2:
		{
			Dd2Chunk chunk;
			temp.read(reinterpret_cast<char*>(&chunk.header), sizeof(chunk.header));
			for (int i = 0; i < chunk.header.size / 8; i++) {
				ChkDd2Elem e;
				temp.read(reinterpret_cast<char*>(&e), sizeof(e));
				chunk.doodads.push_back(e);
			}
			Chk.dd2_chunks.push_back(chunk);
			break;
		}
		case Header::Dim:
		{
			DimChunk chunk;
			temp.read(reinterpret_cast<char*>(&chunk), sizeof(chunk));
			Chk.dim_chunks.push_back(chunk);
			break;
		}
		case Header::Era:
		{
			EraChunk chunk;
			temp.read(reinterpret_cast<char*>(&chunk), sizeof(chunk));
			Chk.era_chunks.push_back(chunk);
			break;
		}
		case Header::Forc:
		{
			ForcChunk chunk;
			temp.read(reinterpret_cast<char*>(&chunk), sizeof(chunk));
			Chk.forc_chunks.push_back(chunk);
			break;
		}
		case Header::Iown:
		{
			IownChunk chunk;
			temp.read(reinterpret_cast<char*>(&chunk), sizeof(chunk));
			Chk.iown_chunks.push_back(chunk);
			break;
		}
		case Header::Isom:
		{
			IsomChunk chunk;
			if (Chk.dim_chunks.size() == 0) {
				QMessageBox::information(this, tr("Forgot match"), "NO DIM CHUNKS (ISOM)");
				return;
			}
			int width = Chk.dim_chunks[0].width;
			int height = Chk.dim_chunks[0].height;
			int n = ((width / 2) + 1) * (height + 1) * 4;
			//QMessageBox::information(this, tr("ISOM_CHUNK"), QString::fromStdString("N: " + WriteInt(n)));
			//QMessageBox::information(this, tr("ISOM_CHUNK"), QString::fromStdString("TELLG " + WriteInt(temp.tellg())));
			//QMessageBox::information(this, tr("ISOM_CHUNK"), QString::fromStdString("DIMENSIONS: " + WriteInt(width) + " " + WriteInt(height)));
			
			temp.read(reinterpret_cast<char*>(&chunk.header), sizeof(chunk.header));
			
			u16 iso;
			for (int i = 0; i < n; i++) {
				temp.read(reinterpret_cast<char*>(&iso), sizeof(iso));
				chunk.isom_data.push_back(iso);
			}
			/*
			temp.read(reinterpret_cast<char*>(&chunk.header), sizeof(chunk.header));

			chunk.isom_data.resize(n);
			for (int i = 0; i < n; i++) {
				temp.read(reinterpret_cast<char*>(&chunk.isom_data[i]), sizeof(u16));
			}
			Chk.isom_chunks.push_back(chunk);*/
			break;
		}
		case Header::Ive2:
		{
			Ive2Chunk chunk;
			temp.read(reinterpret_cast<char*>(&chunk), sizeof(chunk));
			Chk.ive2_chunks.push_back(chunk);
			break;
		}
		case Header::Iver:
		{
			IverChunk chunk;
			temp.read(reinterpret_cast<char*>(&chunk), sizeof(chunk));
			Chk.iver_chunks.push_back(chunk);
			break;
		}
		case Header::Mask:
		{
			MaskChunk chunk;
			//temp.read(reinterpret_cast<char*>(&chunk), sizeof(chunk));
			if (Chk.dim_chunks.size() == 0) {
				QMessageBox::information(this, tr("Forgot match"), "NO DIM CHUNKS (MTXM)");
				return;
			}
			int width = Chk.dim_chunks[0].width;
			int height = Chk.dim_chunks[0].height;
			temp.read(reinterpret_cast<char*>(&chunk.header), sizeof(chunk.header));
			chunk.fog.resize(width);
			for (int i = 0; i < width; i++) {
				chunk.fog[i].resize(height);
			}
			for (int i = 0; i < width; i++) {
				for (int j = 0; j < height; j++) {
					temp.read(reinterpret_cast<char*>(&chunk.fog[i][j]), sizeof(u8));
				}
			}
			Chk.mask_chunks.push_back(chunk);
			break;
		}
		case Header::Mbrf:
		{
			TrigChunk chunk;
			
			temp.read(reinterpret_cast<char*>(&chunk), sizeof(chunk));
			Chk.mbfr_chunks.push_back(chunk);
			break;
		}
		case Header::Mrgn:
		{
			MrgnChunk chunk;
			temp.read(reinterpret_cast<char*>(&chunk.header), sizeof(chunk.header));
			for (int i = 0; i < chunk.header.size/20; i++) {
				ChkLocElem e;
				temp.read(reinterpret_cast<char*>(&e), sizeof(e));
				chunk.locs.push_back(e);
			}
			Chk.mrgn_chunks.push_back(chunk);
			break;
		}
		case Header::Mtxm:
		{
			MtxmChunk chunk;
			//temp.read(reinterpret_cast<char*>(&chunk), sizeof(chunk));
			if (Chk.dim_chunks.size() == 0) {
				QMessageBox::information(this, tr("Forgot match"), "NO DIM CHUNKS (MTXM)");
				return;
			}
			int width = Chk.dim_chunks[0].width;
			int height = Chk.dim_chunks[0].height;
			temp.read(reinterpret_cast<char*>(&chunk.header), sizeof(chunk.header));
			chunk.tiles.resize(width);
			for (int i = 0; i < width; i++) {
				chunk.tiles[i].resize(height);
			}
			for (int i = 0; i < width; i++) {
				for (int j = 0; j < height; j++) {
					temp.read(reinterpret_cast<char*>(&chunk.tiles[i][j]), sizeof(u16));
				}
			}
			Chk.mtxm_chunks.push_back(chunk);
			break;
		}
		case Header::Ownr:
		{
			OwnrChunk chunk;
			temp.read(reinterpret_cast<char*>(&chunk), sizeof(chunk));
			Chk.ownr_chunks.push_back(chunk);
			break;
		}
		case Header::Ptec:
		{
			PtecChunk chunk;
			temp.read(reinterpret_cast<char*>(&chunk), sizeof(chunk));
			Chk.ptec_chunks.push_back(chunk);
			break;
		}
		case Header::Ptex:
		{
			PtexChunk chunk;
			temp.read(reinterpret_cast<char*>(&chunk), sizeof(chunk));
			Chk.ptex_chunks.push_back(chunk);
			break;
		}
		case Header::Puni:
		{
			PuniChunk chunk;
			temp.read(reinterpret_cast<char*>(&chunk), sizeof(chunk));
			Chk.puni_chunks.push_back(chunk);
			break;
		}
		case Header::Pupx:
		{
			PupxChunk chunk;
			temp.read(reinterpret_cast<char*>(&chunk), sizeof(chunk));
			Chk.pupx_chunks.push_back(chunk);
			break;
		}
		case Header::Side:
		{
			SideChunk chunk;
			temp.read(reinterpret_cast<char*>(&chunk), sizeof(chunk));
			Chk.side_chunks.push_back(chunk);
			break;
		}
		case Header::Sprp:
		{
			SprpChunk chunk;
			temp.read(reinterpret_cast<char*>(&chunk), sizeof(chunk));
			Chk.sprp_chunks.push_back(chunk);
			break;
		}
		case Header::Str:
		{
			StrChunk chunk;
//			temp.read(reinterpret_cast<char*>(&chunk), sizeof(chunk));
	/*
	struct StrChunk {
	Chunk header;
	u16 string_count;
	std::vector<u16> string_ids;//length is string count
	std::vector<std::string> strings_data;
	/*
	u16[Number of strings]: 1 integer for each string specifying the offset (the spot where the string starts in the section from the start of it).
	Strings: After the offsets, this is where every string in the map goes, one after another. Each one is terminated by a null character.
	*/
			temp.read(reinterpret_cast<char*>(&chunk.header), sizeof(chunk.header));
			int space_left = chunk.header.size;
			temp.read(reinterpret_cast<char*>(&chunk.string_count), sizeof(chunk.string_count));
			space_left -= 2;
//			QMessageBox::information(this, tr("STR CHUNK"), QString::fromStdString("string count: " + WriteInt(chunk.string_count) + " , header count: " + WriteInt(chunk.header.size)));
			for (int i = 0; i < chunk.string_count; i++) {
				u16 id;
				temp.read(reinterpret_cast<char*>(&id), sizeof(id));
				chunk.string_ids.push_back(id);
				space_left -= 2;
			}


		
			while (space_left > 0) {
				char c;
				std::string str;
				temp.read(reinterpret_cast<char*>(&c), sizeof(c));
				if (c == '0') {
					chunk.strings_data.push_back(str);
					str.clear();
				}
				else {
					str.push_back(c);
				}
				space_left--;

			}
			/*
			int count = chunk.string_count;
			QMessageBox::information(this, tr(""), "Start strings...");
			while (count > 0) {
				char c='a';
				temp.read(reinterpret_cast<char*>(&c), sizeof(c));
				std::string str;
				while (c != 0) {
					str.push_back(c);
				}
				chunk.strings_data.push_back(str);
				count--;
			}*/
			QMessageBox::information(this, tr(""), "End strings");
			Chk.str_chunks.push_back(chunk);
			break;
		}
		case Header::Swnm:
		{
			SwnmChunk chunk;
			temp.read(reinterpret_cast<char*>(&chunk), sizeof(chunk));
			Chk.swnm_chunks.push_back(chunk);
			break;
		}
		case Header::Tecs:
		{
			TecsChunk chunk;
			temp.read(reinterpret_cast<char*>(&chunk), sizeof(chunk));
			Chk.tecs_chunks.push_back(chunk);
			break;
		}
		case Header::Tecx:
		{
			TecxChunk chunk;
			temp.read(reinterpret_cast<char*>(&chunk), sizeof(chunk));
			Chk.tecx_chunks.push_back(chunk);
			break;
		}
		case Header::Thg2:
		{
			Thg2Chunk chunk;
			temp.read(reinterpret_cast<char*>(&chunk.header), sizeof(chunk.header));
			for (int i = 0; i < chunk.header.size / 10; i++) {
				ChkThg2Elem e;
				temp.read(reinterpret_cast<char*>(&e), sizeof(e));
				chunk.sprites.push_back(e);
			}
			Chk.thg2_chunks.push_back(chunk);
			break;
		}
		case Header::Tile:
		{
			TileChunk chunk;
			if (Chk.dim_chunks.size() == 0) {
				QMessageBox::information(this, tr("Forgot match"), "NO DIM CHUNKS (TILE)");
				return;
			}
			int width = Chk.dim_chunks[0].width;
			int height = Chk.dim_chunks[0].height;
			temp.read(reinterpret_cast<char*>(&chunk.header), sizeof(chunk.header));
			chunk.tile_data.resize(width);
			for (int i = 0; i < width; i++) {
				chunk.tile_data[i].resize(height);
			}
			for (int i = 0; i < width; i++) {
				for (int j = 0; j < height; j++) {
					temp.read(reinterpret_cast<char*>(&chunk.tile_data[i][j]), sizeof(u16));
				}
			}
			Chk.tile_chunks.push_back(chunk);
			break;
		}
		case Header::Trig:
		{
			TrigChunk chunk;
			QMessageBox::information(this, tr("Forgot match"), QString::fromStdString(WriteInt(sizeof(ChkTrigAction)) + " " + WriteInt(sizeof(ChkTrigCondition)) + " " +
				WriteInt(sizeof(ChkTrigExecution))));
			temp.read(reinterpret_cast<char*>(&chunk), sizeof(chunk));
			Chk.trig_chunks.push_back(chunk);
			break;
		}
		case Header::Unis:
		{
			UnisChunk chunk;
			temp.read(reinterpret_cast<char*>(&chunk), sizeof(chunk));
			Chk.unis_chunks.push_back(chunk);
			break;
		}
		case Header::Unit:
		{
			UnitChunk chunk;
			temp.read(reinterpret_cast<char*>(&chunk.header), sizeof(chunk.header));
			//QMessageBox::information(this, tr("UNIT CHUNK"), QString::fromStdString("unit header data: " + WriteInt(chunk.header.name)+" "+WriteInt(chunk.header.size)));
			for (int i = 0; i < chunk.header.size/36;i++) {
				UnitChkElem e;
				temp.read(reinterpret_cast<char*>(&e), sizeof(e));
				chunk.units.push_back(e);
			}
			Chk.unit_chunks.push_back(chunk);
			break;
		}
		case Header::Unix:
		{
			UnixChunk chunk;
			temp.read(reinterpret_cast<char*>(&chunk), sizeof(chunk));
			Chk.unix_chunks.push_back(chunk);
			break;
		}
		case Header::Upgr:
		{
			UpgrChunk chunk;
			temp.read(reinterpret_cast<char*>(&chunk), sizeof(chunk));
			Chk.upgr_chunks.push_back(chunk);
			break;
		}
		case Header::Upgs:
		{
			UpgsChunk chunk;
			temp.read(reinterpret_cast<char*>(&chunk), sizeof(chunk));
			Chk.upgs_chunks.push_back(chunk);
			break;
		}
		case Header::Upgx:
		{	
			UpgxChunk chunk;
			temp.read(reinterpret_cast<char*>(&chunk), sizeof(chunk));
			Chk.upgx_chunks.push_back(chunk);
			break;
		}
		case Header::Uprp:
		{
			UprpChunk chunk;
			temp.read(reinterpret_cast<char*>(&chunk), sizeof(chunk));
			Chk.uprp_chunks.push_back(chunk);
			break;
		}
		case Header::Upus:
		{
			UpusChunk chunk;
			temp.read(reinterpret_cast<char*>(&chunk), sizeof(chunk));
			Chk.upus_chunks.push_back(chunk);
			break;
		}
		case Header::Vcod:
		{
			VcodChunk chunk;
			temp.read(reinterpret_cast<char*>(&chunk), sizeof(chunk));
			Chk.vcod_chunks.push_back(chunk);
			break;
		}
		case Header::Ver:
		{
			VerChunk chunk;
//			QMessageBox::information(this, tr("VerChunk"), QString::fromStdString("START TELLG " + WriteInt(temp.tellg())));
			temp.read(reinterpret_cast<char*>(&chunk), sizeof(chunk));
//			QMessageBox::information(this, tr("VerChunk"), QString::fromStdString("Name " + WriteInt(chunk.header.name)));
//			QMessageBox::information(this, tr("VerChunk"), QString::fromStdString("Size " + WriteInt(chunk.header.size)));
//			QMessageBox::information(this, tr("VerChunk"), QString::fromStdString("Version " + WriteInt(chunk.version)));
//			QMessageBox::information(this, tr("VerChunk"), QString::fromStdString("TELLG " + WriteInt(temp.tellg())));
			Chk.ver_chunks.push_back(chunk);
			break;
		}
		case Header::Wav:
		{
			WavChunk chunk;
			temp.read(reinterpret_cast<char*>(&chunk), sizeof(chunk));
			Chk.wav_chunks.push_back(chunk);
			break;
		}
		default:
			QMessageBox::information(this, tr("Forgot match"), QString::fromStdString(WriteInt(chk_id)));
			break;
	};
	





	/*
	u32 name = 0;
	Chunk ch;
	ch.name = header;
	temp.read(reinterpret_cast<char*>(&ch.size), sizeof(ch.size));
	switch (chk_id) {
	case Header::Type:
		TypeChunk type;
		type.header = ch;

		break;
	case Header::Ver:
		VerChunk ver;
		ver.header = ch;
		temp.read(reinterpret_cast<char*>(&ver.version), sizeof(ver.version));
		Chk.ver_chunks.push_back(ver);
		break;
	case Header::Iver:
		break;
	case Header::Ive2:
		break;
	default:
		break;
	}*/
}

void ExtMappingTool::parse_chk(std::string filename) {
	std::ifstream temp;
	temp.open(filename, std::ios::binary | std::ios::in);
	char c;
	bool read_chunk_header = true;
	while (temp.good()) {
		//temp.read(&c, 1);
		if (read_chunk_header) {
			

//			temp.read(&c, 1);
			
			u32 header;
			auto a = temp.tellg();
			temp.read(reinterpret_cast<char*>(&header), sizeof(header));
			temp.seekg(a);
//			QMessageBox::information(this, tr("Header"), QString::fromStdString(WriteInt(header)));
			if (header == 0) {
				//last was trig
				//not sure, must be double-checked
				return;
			}
			int match = header_match(header);
			//temp.close();
			

			if (match) {
				read_chunk_header = false;		
				ReadChunk(match, temp, header);
				read_chunk_header = true;
				//
//				temp.close();
//				QMessageBox::information(this, tr("read chunk test end"),"read chunk test end");
//				return;
				//
			}
			else {
				QMessageBox::information(this, tr("unknown chunk"), QString::fromStdString("Unknown Chunk "+WriteInt(header)));
			}
		}
	}
	temp.close();
}
void ExtMappingTool::on_saveUpgButton_clicked() {
	/*
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Save Graft Extension File"), "",
		tr("Brood War extension data plugin file (*.ext);;All Files (*)"));
	if (fileName.isEmpty())
		return;
	else {
		QFile file(fileName);
		if (!file.open(QIODevice::WriteOnly)) {
			QMessageBox::information(this, tr("Unable to open file"),
				file.errorString());
			return;
		}
		QMessageBox::information(this, tr(""),
			"Entry: " + QString::number(upgReqMenu.currentUpgrade));
		upgReqMenu.SaveEntry(upgReqMenu.currentUpgrade);
		std::ofstream temp;
		temp.open(fileName.toStdString().c_str(), std::ios::binary | std::ios::out);
		std::vector<UpgTreeDataElement> lister;

		for (auto a : req_tree_data) {
			lister.push_back(a.second);				
		}
		temp << std::noskipws << lister;
		QMessageBox::information(this, tr("Unable to open file"),
			QString::fromStdString("Save Lister:"));
		
		temp.close();
	}*/
}
void ExtMappingTool::on_loadUpgButton_clicked() {
	/*
	api.fileName = QFileDialog::getOpenFileName(this,
		tr("Open Graft Extension File"), "",
		tr("Brood War extension data plugin file (*.ext);;All Files (*)"));
	if (api.fileName.isEmpty())
	{
		return;
	}	
	else {
		
		QFile file(api.fileName);
		if (!file.open(QIODevice::ReadOnly)) {
			QMessageBox::information(this, tr("Unable to open file"),
				file.errorString());
			return;
		}
		QMessageBox::information(this, tr("Unable to open file"),
			QString::fromStdString("Try open, clear data [0]"));
		QMessageBox::information(this, tr("Unable to open file"),
			QString::fromStdString("N!"));
		req_tree_data.clear();
		
		QMessageBox::information(this, tr("Unable to open file"),
			QString::fromStdString("Clear"));

		std::ifstream temp;
		QMessageBox::information(this, tr("Unable to open file"),
			QString::fromStdString("Open file..."));
		temp.open(api.fileName.toStdString().c_str(), std::ios::binary | std::ios::in);
		QMessageBox::information(this, tr("Unable to open file"),
			QString::fromStdString("While good, open..."));
		//std::vector<UpgTreeDataElement> lister;
		while (temp.good()) {
			//UpgTreeDataElement upg;
			//temp.read(reinterpret_cast<char*>(&upg.upgrade_id), sizeof(upg.upgrade_id));
			//temp.read(reinterpret_cast<char*>(&upg.used), sizeof(upg.used));
			//temp >> std::noskipws >> upg.data;
			
			std::vector<UpgTreeDataElement> lister;
			temp >> lister;
			QMessageBox::information(this, tr("Unable to open file"),
				QString::fromStdString("Lister:"));
			for (auto a : lister) {
				QMessageBox::information(this, tr("Unable to open file"),
					QString::fromStdString("l1: " + WriteInt(a.upgrade_id) + " " + WriteInt(a.used) + " " + WriteInt(a.data.size())));
				for (int i = 0; i < a.data.size(); i++) {
					QMessageBox::information(this, tr("Unable to open file"),
						QString::fromStdString("l2: " + WriteInt(a.data[i].nested.size())));
				}
			}


			for (auto a : lister) {
				req_tree_data[a.upgrade_id] = a;
			}


		}
		QMessageBox::information(this, tr("Unable to open file"),
			QString::fromStdString("Finish read, it was good"));
		upgReqMenu.currentUpgrade = current_default_upg;
		upgReqMenu.current_upgrade_id = current_default_upg;
		upgReqMenu.LoadEntry(current_default_upg);
		temp.close();
	}*/
}
void ExtMappingTool::on_loadButton_clicked() {
	api.fileName = QFileDialog::getOpenFileName(this,
		tr("Open Map File"), "",
		tr("StarCraft Brood War map (*.scx);;All Files (*)"));
	if (api.fileName.isEmpty())
		return;
	else 
	{
		QFile file(api.fileName);
		if (!file.open(QIODevice::ReadOnly)) {
			QMessageBox::information(this, tr("Unable to open file"),
				file.errorString());
			return;
		}
		HANDLE hMpq = NULL;
		HANDLE hFile = NULL;
		std::string fileName_str = api.fileName.toStdString();
		std::basic_string<TCHAR> fileName_converted(fileName_str.begin(), fileName_str.end());
		if (!SFileOpenArchive(fileName_str.c_str(), 90, 0, &hMpq)) {
			QMessageBox::information(this, tr("Unable to open mpq archive"), api.fileName);
			return;
		}
		if (!SFileOpenFile("(listfile)", &hFile)) {
			QMessageBox::information(this, tr("Invalid mpq archive"), file.errorString());
			return;
		}
		bool read_chk = true;
		if (!SFileOpenFile("staredit\\scenario.chk", &hFile)) {
			QMessageBox::information(this, tr("Invalid SC map"), file.errorString());
			return;
		}
		if (read_chk) {
			if (!SFileOpenFile("staredit\\scenario.chk", &hFile)) {
				QMessageBox::information(this, tr("No previous ext data"),
					"Fake scenario.chk was not located");
				read_chk = false;
			}
			else {		
				PVOID pvReadBuffer;
				DWORD dwBytesToRead, dwBytesRead, dwFileLeft, dwFileSize, dwBytesWritten;
				const DWORD dwBufferSize = 0x400000;
				pvReadBuffer = new BYTE[dwBufferSize];
				dwFileLeft = dwFileSize = SFileGetFileSize(hFile, NULL);
				dwBytesToRead = std::min(dwFileLeft, dwBufferSize);
				char szOutFileName[MAX_PATH + 1];
				std::string dumpData = "dumpchk.dat";
				std::basic_string<TCHAR> conv(dumpData.begin(), dumpData.end());
				HANDLE hOutFile = CreateFile(conv.c_str(), GENERIC_WRITE,
					NULL, NULL, CREATE_ALWAYS, NULL, NULL);
				
				if (hOutFile == NULL) {
					QMessageBox::information(this, tr("File is not created"), file.errorString());
					return;
				}
				if (SFileReadFile(hFile, pvReadBuffer, dwBytesToRead, &dwBytesRead, NULL) && dwBytesRead)
				{
					if (!WriteFile(hOutFile, pvReadBuffer, dwBytesRead, &dwBytesWritten, NULL))
					{
						QMessageBox::information(this, tr("Can't write file"), file.errorString());
						return;
					}
					CloseHandle(hOutFile);
					parse_chk(dumpData);
					QMessageBox::information(this, tr(""), QString::fromStdString("Chk Info: "+WriteInt(Chk.unit_chunks[0].units.size())));
				}
				else {
					QMessageBox::information(this, tr("Can't read file"), file.errorString());
					return;
				}
			}
		}
		bool read_previous_data = true;
		if (!SFileOpenFile("extension.dat", &hFile)) {
			QMessageBox::information(this, tr("No previous ext data"), 
				"No previous extended data located");
			read_previous_data = false;
		}
		if (read_previous_data) {
			PVOID pvReadBuffer;
			DWORD dwBytesToRead, dwBytesRead, dwFileLeft, dwFileSize, dwBytesWritten;
			const DWORD dwBufferSize = 0x400000;
			pvReadBuffer = new BYTE[dwBufferSize];
			dwFileLeft = dwFileSize = SFileGetFileSize(hFile, NULL);
			dwBytesToRead = std::min(dwFileLeft, dwBufferSize);
			char szOutFileName[MAX_PATH + 1];
			std::string teststr = "dump.dat";
			std::basic_string<TCHAR> conv(teststr.begin(), teststr.end());
			HANDLE hOutFile = CreateFile(conv.c_str(), GENERIC_WRITE,
				NULL, NULL, CREATE_ALWAYS, NULL, NULL);
			if (hOutFile == NULL) {
				QMessageBox::information(this, tr("File is not created"), file.errorString());
				return;
			}
			if (SFileReadFile(hFile, pvReadBuffer, dwBytesToRead, &dwBytesRead, NULL) && dwBytesRead)
			{
				if (!WriteFile(hOutFile, pvReadBuffer, dwBytesRead, &dwBytesWritten, NULL))
				{
					QMessageBox::information(this, tr("Can't write file"), file.errorString());
					return;
				}
				CloseHandle(hOutFile);
				QMessageBox::information(this, tr("Debug"),
					QString::fromStdString("Read data from archive"));
				ClearEditorReadWriteData();
				QMessageBox::information(this, tr("Debug"),
					QString::fromStdString("Editor data cleared"));
				ReadTempArchiveToData(teststr);
				QMessageBox::information(this, tr("Debug"),
					QString::fromStdString("Temp archive set to data"));
				ResetInterface(true);
				SetTempArchiveToInterface();
				ClearEditorReadWriteData();
			}
			else {
				QMessageBox::information(this, tr("Can't read file"), file.errorString());
				return;
			}
			QMessageBox::information(this, tr("Read is finished, archive can be closed"), api.fileName);
		}
		SFileCloseFile(hFile);
		SFileCloseArchive(hMpq);
		api.file_is_open = true;
		api.current_map_file = fileName_str;
		ui.saveButton->setDisabled(false);/*
		QMessageBox::information(this, tr("File:"),
			QString::fromStdString(fileName_str));*/
	}
}